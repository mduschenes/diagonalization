# Commands
CC 		:= g++
MKDIR 	:= mkdir -p
RM 	:= rm -rf
RUN     := ./

# Directories
SRC_DIR := src
INC_DIR := include
LIB_DIR := lib
OBJ_DIR := obj
DEP_DIR := obj
EXE_DIR := bin
PROCESS_DIR := src
DIR := SRC_DIR INC_DIR LIB_DIR OBJ_DIR EXE_DIR

# Extensions
SRC_EXT := cpp
INC_EXT := hpp
LIB_EXT := a so
OBJ_EXT := o
DEP_EXT := d
EXE_EXT := out
PROCESS_EXT:= py

# Targets
SRC := main tensor hamiltonian io hdf5 utils
SRC := $(patsubst %,$(SRC_DIR)/%.$(SRC_EXT),$(SRC))

# LIB := /usr/local/lib/libhdf5_hl_cpp.a /usr/local/lib/libhdf5_cpp.a /usr/local/lib/libhdf5_hl.a /usr/local/lib/libhdf5.a
LIB := hdf5/libhdf5_hl_cpp.a hdf5/libhdf5_cpp.a hdf5/libhdf5_hl.a hdf5/libhdf5.a
LIB := $(patsubst %,$(LIB_DIR)/%,$(LIB))

OBJ := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.$(OBJ_EXT),$(SRC))

EXE := main
EXE := $(patsubst %,$(EXE_DIR)/%.$(EXE_EXT),$(EXE))

PROCESS := process
PROCESS := $(patsubst %,$(PROCESS_DIR)/%.$(PROCESS_EXT),$(PROCESS))
PROCESS_FLAGS := data data
PROCESS_DEP := data/data.hdf5

PROFILE := gprof
PROFILE_FLAGS := gprof -b -pset
PROFILE_DEP := gmon.out
PROFILE_OUT := profile.out


# Optional Flags
USE_DEP := 1
USE_OMP := 1
USE_MKL := 1
# USE_HDF5 := /usr/lib/x86_64-linux-gnu/hdf5/serial/
# USE_HDF5 := /usr/local/include/
USE_HDF5 := /usr/local/
USE_PROFILE :=

# Flags
# CC_FLAGS := -Ofast -funroll-loops -m64 -std=c++17 -fext-numeric-literals -fconcepts -fPIC -DNDEBUG -Wall -Wno-unknown-pragmas -Wno-unused-variable -I/usr/include -I$(INC_DIR)
CC_FLAGS := -O3 -m64 -std=c++17 -fext-numeric-literals -fconcepts -fPIC -DNDEBUG -Wall -Wno-unknown-pragmas -Wno-unused-variable -I/usr/include -I$(INC_DIR)
LD_FLAGS := -L/usr/lib -L$(LIB_DIR)
LD_LIBS  := 


# Compile Options
ifdef USE_DEP
# CC_FLAGS += -MMD -MP
endif

ifdef USE_OMP
USE_OMP = 1
CCFLAGS += -DUSE_OMP
LD_FLAGS += -fopenmp -msse2
endif

ifdef USE_HDF5
HDF5_DIR := $(USE_HDF5)
USE_HDF5 := 1
CC_FLAGS += -I$(HDF5_DIR) -DUSE_HDF5 -DOLD_HEADER_FILENAME -DHDF_NO_NAMESPACE -DNO_STATIC_CAST
LD_FLAGS += -L$(HDF5_DIR)lib 
LD_LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp -lz -ldl -lm -Wl,-rpath -Wl,/usr/local/lib
endif

ifdef USE_PROFILE
CC_FLAGS += -pg
endif


# User Libraries
CC_FLAGS += -I$(LIB_DIR)/eigen -I$(LIB_DIR)/lbfgspp/include -I$(LIB_DIR)/itertools
LD_FLAGS +=
LD_LIBS += -lpthread

# BLAS Libraries
ifdef USE_MKL
CC_FLAGS += -I${MKLROOT}/include
LD_FLAGS += -L${MKLROOT}/lib/intel64
LD_LIBS  += -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5 -fopenmp
else
CC_FLAGS +=
LD_FLAGS +=
LD_LIBS  += -lblas -llapack -fopenmp
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
.PHONY: all run test clean process profile
.SILENT : all run test clean process profile
.PRECIOUS : $(OBJ_DIR)/%.$(OBJ_EXT) $(DEP_DIR)/%.$(DEP_EXT)



# Rules
all: clean $(EXE)

run : $(EXE)
	$(RUN)$(EXE) $(RUN_ARGS)
	h5dump $(PROCESS_DEP)

test :

clean : clean_obj clean_data

process : clean $(PROCESS) $(PROCESS_DEP)
	h5dump $(PROCESS_DEP)
	$(PROCESS) $(PROCESS_DEP) $(PROCESS_FLAGS) 

profile : $(PROFILE_DEP)
	$(PROFILE) $(PROFILE_FLAGS) $(EXE) $(PROFILE_DEP) > $(PROFILE_OUT)


clean_obj :
	@$(RM) $(OBJ_DIR)
	@$(RM) $(EXE_DIR)

clean_data :
	@$(RM) $(PROCESS_DEP)
	@$(RM) $(PROFILE_DEP)

$(EXE_DIR)/%.$(EXE_EXT) : $(OBJ) $(LIB) | $(EXE_DIR)
	@$(CC) $(CC_FLAGS) $(OBJ) -o $@ $(LD_FLAGS) $(LD_LIBS)

$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT) $(LIB) | $(OBJ_DIR)
	@$(CC) $(CC_FLAGS) -c $< -o $@ $(LD_FLAGS) $(LD_LIBS)

$(OBJ_DIR) $(EXE_DIR) : 
	@$(MKDIR) $@

$(PROCESS_DEP): run
$(PROFILE_DEP): run

# ifdef USE_DEP
-include $(OBJ:.$(OBJ_EXT)=.$(DEP_EXT)) 
# endif
