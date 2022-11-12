import torch
import torch.nn as nn
import torchvision
import os, sys
from torch.utils.data import DataLoader
import time
import argparse
from datetime import datetime
seed = 1234

# it is highly recommended that you use a logger to record the output from
# your training job because the default SBATCH log files could be lost
# when the job is preempted. Moreover, the logger provides handy information
# such as displaying time of the message, etc
import logging

# a demo pytorch model
class MLP(nn.Module):
    def __init__(self):
        super(MLP, self).__init__()
        self.FC = nn.Sequential(
                    nn.Linear(64, 32),
                    nn.ReLU(),
                    nn.Linear(32, 16),
                    nn.ReLU(),
                    nn.Linear(16, 2)
        )

    def forward(self, x):
        x = x.view(-1, 64)
        return self.FC(x)


# with a very large training data, an epoch may take longer than your permitted
# time slice. Therefore, you might want to save checkpoint after certain
# iterations within an epoch. However, if you are randomizing the batches for
# each epoch, you need to save the random state in case of preemption.,
# PyTorch currently does not have the out-of-box solution, but you could write
# an customized sampler to do so.
class StatefulSampler(torch.utils.data.sampler.Sampler):
    def __init__(self, data_source, shuffle=False):
        self.data = data_source
        self.shuffle = shuffle

        # initial dataloader index
        self.init_index()

    def init_index(self):

        if self.shuffle:
            self.indices = torch.randperm(len(self.data))
        else:
            self.indices = torch.arange(len(self.data))

        self.data_counter = 0

    def __iter__(self):
        return self

    def __len__(self):
        return len(self.data)

    def __next__(self):
        if self.data_counter == len(self.data):
            self.init_index()
            raise StopIteration()
        else:
            ele = self.indices[self.data_counter]
            self.data_counter += 1
            return int(ele)

    def state_dict(self, dataloader_iter=None):
        prefetched_num = 0
        # in the case of multiworker dataloader, the helper worker could be
        # pre-fetching the data that is not consumed by the main dataloader.
        # we need to subtract the unconsumed part .
        if dataloader_iter is not None:
            if dataloader_iter._num_workers > 0:
                batch_size = dataloader_iter._index_sampler.batch_size
                prefetched_num = \
                    (dataloader_iter._send_idx - dataloader_iter._rcvd_idx) * batch_size
        return {
                'indices': self.indices,
                'data_counter': self.data_counter - prefetched_num,
            }

    def load_state_dict(self, state_dict):
        self.indices = state_dict['indices']
        self.data_counter = state_dict['data_counter']

def prepare_rand_dataset():
    return torchvision.datasets.FakeData(size=1024*10, image_size=(8,8),
                                         num_classes=2,
                                         transform=torchvision.transforms.ToTensor())

def create_or_restore_training_state(dataset, batch_size, checkpoint_path):
    # initial configuration of the model
    model = MLP()
    optimizer = torch.optim.Adam(model.parameters())
    dataloader = DataLoader(dataset, shuffle=False, batch_size=batch_size,
                            sampler=StatefulSampler(dataset, shuffle=True),
                            num_workers=0)
    epoch = 0
    cur_loss = 0
    # restore training state if checkpoint exists
    if os.path.exists(checkpoint_path):
        training_state = torch.load(checkpoint_path)

        model.load_state_dict(training_state['model'])
        optimizer.load_state_dict(training_state['optimizer'])
        dataloader.sampler.load_state_dict(training_state['sampler'])
        epoch = training_state['epoch']
        cur_loss = training_state['cur_loss']
        rng = training_state['rng']
        torch.random.set_rng_state(rng)
        logging.info("training state restored at epoch {} and iteration {}".
              format(epoch, dataloader.sampler.data_counter))

    else:
        logging.info("No checkpoint detected, starting from initial state")

    return model, optimizer, dataloader, epoch, cur_loss


# we need to be careful when saving checkpoints since preemption can also
# occur during checkpointing. Therefore, we need to make sure the checkpoint
# file is either kept untouched or successfully updated during this process.
def commit_state(model, optimizer, sampler, dataloader_iter, rng, epoch, cur_loss, checkpoint_path):

    temp_path = os.path.join(os.path.dirname(checkpoint_path), "temp.pt")

    training_state = {
        'model' : model.state_dict(),
        'optimizer' : optimizer.state_dict(),
        'sampler' : sampler.state_dict(dataloader_iter),
        'epoch': epoch,
        'cur_loss' : cur_loss,
        'rng' : rng
    }

    # first save to temp file
    torch.save(training_state, temp_path)
    # according to the GNU spec of rename, the state of checkpoint_path
    # is atomic, i.e. it will either be modified or not modified, but not in
    # between, during a system crash (i.e. preemtion)
    os.replace(temp_path, checkpoint_path)
    msg = datetime.now().strftime("%Y-%m-%d %H:%M:%S") + ": Checkpoint saved at " + checkpoint_path
    logging.info(msg)

def optimizer_to_dev(optimizer, device):
    for state in optimizer.state.values():
        for k, v in state.items():
            if torch.is_tensor(v):
                state[k] = v.to(device)

def train(model, optimizer, dataloader, epoch, epoch_loss,
          checkpoint_path, checkpoint_interval, max_epoch):
    if torch.cuda.is_available():
        device = torch.device("cuda")
    else:
        device = torch.device("cpu")

    criterion = nn.CrossEntropyLoss()
    start_time = time.time()
    model = model.to(device)
    optimizer_to_dev(optimizer, device)
    while epoch < max_epoch:

        # it is important to take a snapshot at the rng before looping because
        # before dataloader is iterated, it will invoke "random" which alters
        # the rng state. If you do not take the snapshot here and save the rng
        # during the dataset iteration, the loaded rng state contains the
        # alteration introduced by the "for" construct, but after restart, you will
        # change the rng state again by invoking "for" below, the rng state
        # you loaded will be different from when you checkpointed.
        rng = torch.random.get_rng_state()
        dataloader_iter = iter(dataloader)
        for train, label in dataloader_iter:
            train = train.to(device)
            label = label.to(device)

            ans = model(train)
            loss = criterion(ans, label)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            epoch_loss += float(loss)
            cur_time = time.time()
            if cur_time - start_time > checkpoint_interval:
                commit_state(model, optimizer, dataloader.sampler,
                             dataloader_iter, rng, epoch, epoch_loss, checkpoint_path)
                start_time = cur_time
        msg = "Epoch: {}, Loss: {}".format(epoch, epoch_loss)
        logging.info(msg)
        epoch += 1
        epoch_loss = 0

    return model


def main(args):

    if args.log_file is not None:
        logging.basicConfig(filename=args.log_file,level=logging.DEBUG)

    logging.info("starting training script")
    torch.random.manual_seed(seed)
    dataset = prepare_rand_dataset()
    checkpoint_path = args.checkpoint_path

    # check if the checkpoint exists and try to resume from the last checkpoint
    # if you are saving for every epoch, you can skip the part about
    # saving and loading the dataloader state.
    model, optimizer, dataloader, epoch, cur_loss= \
        create_or_restore_training_state(dataset, args.batch_size, checkpoint_path)

    # now we can start the training loop
    model = train(model, optimizer, dataloader, epoch, cur_loss,
                   checkpoint_path, args.checkpoint_interval,
                   args.num_epoch)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
      description='demo training script that performs checkpointing for PyTorch'
    )

    parser.add_argument(
        '--num_epoch',
        type=int,
        help='number of epochs to run',
        required=True
    )

    parser.add_argument(
        '--checkpoint_path',
        type=str,
        help='path to save and look for the checkpoint file',
        default=os.path.join(os.getcwd(), "checkpoint.pt")
    )

    parser.add_argument(
        '--batch_size',
        type=int,
        help='batch size per iteration',
        default=32
    )

    parser.add_argument(
        '--checkpoint_interval',
        type=int,
        help='period to take checkpoints in seconds',
        default=3600
    )

    parser.add_argument(
        '--log_file',
        type=str,
        help='specify the location of the output directory, default stdout',
        default=None
    )

    main(parser.parse_args())