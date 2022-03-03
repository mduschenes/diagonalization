import os, sys
import argparse
import time

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

import tensorflow as tf
from tensorflow.keras.layers import Dense, Flatten
from tensorflow.keras import Model

# it is highly recommended that you use a logger to record the output from
# your training job because the default SBATCH log files could be lost
# when the job is preempted. Moreover, the logger provides handy information
# such as displaying time of the message, etc
import logging

rand_seed = 1234


# simple demo multi-layer perceptron
class MLP(Model):
  def __init__(self):
    super(MLP, self).__init__()
    self.flatten = Flatten()
    self.d1 = Dense(128, activation='relu', input_shape=(28*28,))
    self.d2 = Dense(64, activation='relu')
    self.d3 = Dense(10)

  def call(self, x):
    x = self.flatten(x)
    x = self.d1(x)
    x = self.d2(x)
    return self.d3(x)


# checkpointing in TensorFlow is quite different from that in PyTorch, the
# biggest difference is that TensorFlow abstracts checkpointing as a class,
# i.e. tf.train.Checkpoint and tf.train.CheckpointManager. This exposes more
# utility than the simple torch.save/torch.load functionality, but it requires
# a better understanding of the class methods and usage.
def create_sample_dataset(batch_size):
    mnist = tf.keras.datasets.mnist
    (x_train, y_train), (_, _) = mnist.load_data()

    # Add a channels dimension
    x_train = x_train[..., tf.newaxis]
    train_ds = tf.data.Dataset.from_tensor_slices(
        (x_train, y_train)).shuffle(100, seed=rand_seed, reshuffle_each_iteration=False).batch(batch_size)

    return train_ds


def create_or_restore_training_state(dataset, checkpoint_dir):

    model = MLP()
    optimizer = tf.keras.optimizers.Adam()
    data_iter = iter(dataset)
    train_loss = tf.keras.metrics.Mean(name='train_loss')
    train_accuracy = tf.keras.metrics.SparseCategoricalAccuracy(name='train_accuracy')
    epoch = tf.Variable(0)
    global_rng_state = tf.random.experimental.get_global_generator().state

    # create the checkpoint variable
    checkpoint = tf.train.Checkpoint(epoch=epoch,
                                     optimizer=optimizer,
                                     model=model, data_iter=data_iter,
                                     train_loss=train_loss, train_accuracy=train_accuracy,
                                     global_rng_state=global_rng_state)

    checkpoint_manager = tf.train.CheckpointManager(checkpoint,
                                                    checkpoint_dir,
                                                    max_to_keep=3)

    # now, try to recover from the saved checkpoint, if successful, it should
    # re-populate the fields of the checkpointed variables.
    latest_checkpoint = checkpoint_manager.latest_checkpoint
    checkpoint.restore(latest_checkpoint).expect_partial()
    if latest_checkpoint:
        tf.random.experimental.set_global_generator(
            tf.random.experimental.Generator(
                state=global_rng_state, alg=1))
        logging.info("training state restored at epoch {}".
              format(int(epoch.numpy()) ))
    else:
        logging.info("No checkpoint detected, starting from initial state")

    return model, optimizer, data_iter, train_loss, train_accuracy, epoch, checkpoint_manager


@tf.function
def train_step(images, labels, model, optimizer, train_loss, train_accuracy):

    loss_object = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)

    with tf.GradientTape() as tape:
        predictions = model(images, training=True)
        loss = loss_object(labels, predictions)
    gradients = tape.gradient(loss, model.trainable_variables)
    optimizer.apply_gradients(zip(gradients, model.trainable_variables))
    train_loss(loss)
    train_accuracy(labels, predictions)



def train(model, optimizer, dataset, data_iter, train_loss, train_accuracy,
          epoch, max_epoch, checkpoint_interval, checkpoint_manager):

    start_time = time.time()
    while epoch < max_epoch:
        count = 0
        for images, labels in data_iter:
            train_step(images, labels, model, optimizer, train_loss, train_accuracy)

            cur_time = time.time()
            if cur_time - start_time > checkpoint_interval:
                path = checkpoint_manager.save()
                logging.info("Epoch {}, Training state saved at {}".format(
                    int(epoch.numpy()), path))
                start_time = time.time()
            count += 1
        template = 'Epoch {}, Loss: {}, Accuracy: {}'
        logging.info(template.format(int(epoch.numpy()),
                    train_loss.result(),
                    train_accuracy.result() * 100))
        data_iter = iter(dataset)

        # this attribute is available without the "_" in tf 2.2
        # Subtle! if you rebind the variable name to a different entity,
        # in this case, data_iter = iter(dataset), you need to update the
        # reference in the checkpoint as well otherwise the checkpoint will not save the
        # correct iterator.
        # Alternatively, all your checkpoint variables are directly read/written
        # through "checkpoint_manager._checkpoint", but the code looks clumsy.
        #
        # Notice that all other checkpointed variables are modified in-place
        # so they do not need an update to the checkpoint.
        checkpoint_manager._checkpoint.data_iter = data_iter
        epoch.assign_add(1)
        train_loss.reset_states()
        train_accuracy.reset_states()

def main(args):

    if args.log_file is not None:
        logging.basicConfig(filename=args.log_file,level=logging.DEBUG)

    logging.info("starting training script")

    tf.random.set_seed(rand_seed)
    dataset = create_sample_dataset(args.batch_size)
    checkpoint_dir = args.checkpoint_dir

    model, optimizer, data_iter, train_loss, train_accuracy, epoch, checkpoint_manager = \
        create_or_restore_training_state(dataset, checkpoint_dir)

    model = train(model, optimizer, dataset, data_iter,
                  train_loss, train_accuracy,
                  epoch, args.num_epoch,
                  args.checkpoint_interval, checkpoint_manager)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
      description='demo training script that performs checkpointing for TensorFlow'
    )

    parser.add_argument(
        '--num_epoch',
        type=int,
        help='number of epochs to run',
        required=True
    )

    parser.add_argument(
        '--checkpoint_dir',
        type=str,
        help='path to save and look for the checkpoint file',
        default=os.path.join(os.getcwd(), "checkpoints")
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


