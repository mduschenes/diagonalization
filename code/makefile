# Commands
CC 		:= g++
MKDIR 	:= mkdir -p
RMDIR 	:= rm -rf
RUN     := ./

# Directories
SRC_DIR := src
INC_DIR := include
LIB_DIR := lib
OBJ_DIR := obj
DEP_DIR := obj
EXE_DIR := bin
CMD_DIR := src
DIR := SRC_DIR INC_DIR LIB_DIR OBJ_DIR EXE_DIR

# Extensions
SRC_EXT := cpp
INC_EXT := hpp
LIB_EXT := a so
OBJ_EXT := o
DEP_EXT := d
EXE_EXT := out
CMD_EXT:= py

# Targets
SRC :=  utils io tensor main
SRC := $(patsubst %,$(SRC_DIR)/%.$(SRC_EXT),$(SRC))

LIB := 
LIB := $(patsubst %,$(LIB_DIR)/%,$(LIB))

OBJ := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.$(OBJ_EXT),$(SRC))

EXE := main
EXE := $(patsubst %,$(EXE_DIR)/%.$(EXE_EXT),$(EXE))

CMD := process
CMD := $(patsubst %,$(CMD_DIR)/%.$(CMD_EXT),$(CMD))


# Optional Flags
USE_DEP := 1
USE_OMP := 1
USE_MKL := 1
USE_HDF5 := /usr/lib/x86_64-linux-gnu/hdf5/serial/

# Flags
CCFLAGS := -O2 -m64 -std=c++17 -fconcepts -fPIC -DNDEBUG -Wall -Wno-unknown-pragmas -Wno-unused-variable -I/usr/include -I$(INC_DIR)
LDFLAGS  := -L/usr/lib -L$(LIB_DIR)
LDLIBS   := 

# User Libraries
CCFLAGS += -I$(LIB_DIR)/eigen -I$(LIB_DIR)/lbfgspp/include
LDFLAGS +=
LDLIBS += -lpthread

# BLAS Libraries
ifdef USE_MKL
CCFLAGS += -I${MKLROOT}/include
LDFLAGS += -L${MKLROOT}/lib/intel64 
LDLIBS  += -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5
else
CCFLAGS +=
LDFLAGS +=
LDLIBS  += -lblas -llapack -fopenmp
endif



# Compile Options
ifdef USE_DEP
# CCFLAGS += -MMD -MP
endif

ifdef USE_OMP
USE_OMP = 1
CCFLAGS +=
LDFLAGS += -fopenmp
endif

ifdef USE_HDF5
HDF5_DIR := $(USE_HDF5)
USE_HDF5 := 1
CCFLAGS += -I$(HDF5_DIR) -DUSE_HDF5
LDFLAGS += -L$(HDF5_DIR)lib 
LDLIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp
endif


# CLI Arguments
ifeq (run,$(firstword $(MAKECMDGOALS)))
RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
endif

ifeq (process,$(firstword $(MAKECMDGOALS)))
RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
endif




# Make Options
.PHONY: all run test clean process
.SILENT : all run test clean process
.PRECIOUS : $(OBJ_DIR)/%.$(OBJ_EXT) $(DEP_DIR)/%.$(DEP_EXT)



# Rules
all: $(EXE)

run : $(EXE)
	$(RUN)$< $(RUN_ARGS)

test :

clean :
	@$(RMDIR) $(OBJ_DIR)
	@$(RMDIR) $(EXE_DIR)

process : $(CMD)
	$(RUN)$< $(RUN_ARGS)



$(EXE_DIR)/%.$(EXE_EXT) : $(OBJ) $(LIB) | $(EXE_DIR)
	$(CC) $(CCFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT) $(LIB) | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR) $(EXE_DIR) : 
	@$(MKDIR) $@



# ifdef USE_DEP
-include $(OBJ:.$(OBJ_EXT)=.$(DEP_EXT)) 
# endif
