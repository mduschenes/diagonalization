# Commands
CC 		:= g++
MKDIR 	:= mkdir -p
RM 	:= rm -rf
CMD     := ./

# Directories
SRC_DIR := src
INC_DIR := include
LIB_DIR := lib
OBJ_DIR := obj
DEP_DIR := obj
EXE_DIR := bin
PROCESS_DIR := src
DATA_DIR := data
DIR := SRC_DIR INC_DIR LIB_DIR OBJ_DIR EXE_DIR

# Extensions
SRC_EXT := cpp
INC_EXT := hpp
LIB_EXT := a so
OBJ_EXT := o
DEP_EXT := d
EXE_EXT := out
PROCESS_EXT:= py
DATA_EXT := hdf5

# Targets
SRC := hdf5 io linalg hamiltonian tensor physics utils
SRC := $(patsubst %,$(SRC_DIR)/%.$(SRC_EXT),$(SRC))

# LIB := /usr/local/lib/libhdf5_hl_cpp.a /usr/local/lib/libhdf5_cpp.a /usr/local/lib/libhdf5_hl.a /usr/local/lib/libhdf5.a
LIB := hdf5/libhdf5_hl_cpp.a hdf5/libhdf5_cpp.a hdf5/libhdf5_hl.a hdf5/libhdf5.a
LIB := $(patsubst %,$(LIB_DIR)/%,$(LIB))

OBJ := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.$(OBJ_EXT),$(SRC))

MAIN := main
PROGRAM := ising heisenberg
SEQ  := $(shell seq $(words $(PROGRAM)))
RUN := $(patsubst %,run_%,$(PROGRAM)) 
PLOT := $(patsubst %,plot_%,$(PROGRAM)) 

EXE := $(MAIN)
EXE := $(patsubst %,$(EXE_DIR)/%.$(EXE_EXT),$(EXE))

PROCESS := main
PROCESS := $(patsubst %,$(PROCESS_DIR)/%.$(PROCESS_EXT),$(PROCESS))
PROCESS_FLAGS :=
PROCESS_DEP := $(patsubst %,$(DATA_DIR)/data.$(DATA_EXT),$(MAIN))

EXES := $(PROGRAM)
EXES := $(patsubst %,$(EXE_DIR)/%.$(EXE_EXT),$(EXES))

PROCESSS := $(PROGRAM)
PROCESSS := $(patsubst %,$(PROCESS_DIR)/%.$(PROCESS_EXT),$(PROCESSS))
PROCESSS_FLAGS :=
PROCESSS_DEP := $(patsubst %,$(DATA_DIR)/%/data.$(DATA_EXT),$(PROGRAM))

PROFILE := gprof
PROFILE_FLAGS := gprof -b -pset
PROFILE_DEP := gmon.out
PROFILE_OUT := profile.out


# Optional Flags
USE_DEP := 1
USE_OMP := 1
USE_ARPACK := /usr/include/arpack
USE_MKL := /usr/include/mkl
USE_HDF5 := /usr/lib/x86_64-linux-gnu/hdf5/serial/
USE_PROFILE :=

# Flags
# CC_FLAGS := -Ofast -funroll-loops -m64 -std=c++17 -fext-numeric-literals -fconcepts -fPIC -DNDEBUG -Wall -Wextra -g -Wall -Wno-unknown-pragmas -Wno-unused-variable -I/usr/include -I$(INC_DIR)
CC_FLAGS := -O3 -funroll-loops -m64 -std=c++17 -fext-numeric-literals -fconcepts -fPIC -Wall -DNDEBUG -Wno-unused-but-set-variable -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-value -I/usr/include -I$(INC_DIR)
LD_FLAGS := -L/usr/lib -L$(LIB_DIR) -lstdc++
LD_LIBS  := 


# Compile Options
ifdef USE_DEP
CC_FLAGS += -MMD -MP
endif

ifdef USE_PROFILE
CC_FLAGS += -pg
endif


# User Libraries
# CC_FLAGS += -I$(LIB_DIR)/eigen -I$(LIB_DIR)/lbfgspp/include -I$(LIB_DIR)/itertools -I$(LIB_DIR)/spectra
# CC_FLAGS += -I$(LIB_DIR)/eigen -I$(LIB_DIR)/itertools -I$(LIB_DIR)/spectra/include
CC_FLAGS += -I$(LIB_DIR)/eigen -I$(LIB_DIR)/itertools
LD_FLAGS +=
LD_LIBS += -lpthread

# Arpack Libraries
ifdef USE_ARPACK
ARPACK_DIR := $(USE_ARPACK)
CC_FLAGS += -I$(ARPACK_DIR)
LD_FLAGS += -L$(ARPACK_DIR)/lib
LD_LIBS += -larpack
else
CC_FLAGS +=
LD_FLAGS +=
LD_LIBS +=
endif

# BLAS Libraries
ifdef USE_MKL
MKL_DIR := $(USE_MKL)
CC_FLAGS += -I$(MKL_DIR)/include
CC_FLAGS += -I$(MKL_DIR)
LD_FLAGS += -L$(MKL_DIR)/lib/intel64 
LD_FLAGS += -L$(MKL_DIR)/intel64 
LD_LIBS  += -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_rt -lmkl_core -liomp5 -fopenmp -lpthread -Wl,--no-as-needed -DMKL_LP64
else
CC_FLAGS +=
LD_FLAGS +=
LD_LIBS  += -lblas -llapack -fopenmp
endif


ifdef USE_OMP
USE_OMP = 1
CCFLAGS += -DUSE_OMP
LD_FLAGS += -fopenmp -msse2
endif

ifdef USE_HDF5
HDF5_DIR := $(USE_HDF5)
USE_HDF5 := 1
CC_FLAGS += -DUSE_HDF5 -DOLD_HEADER_FILENAME -DHDF_NO_NAMESPACE -DNO_STATIC_CAST -I$(HDF5_DIR)/include
LD_FLAGS += -L$(HDF5_DIR)/lib
# LD_LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp -lz -ldl -lm -Wl,-rpath -Wl,/usr/local/lib
LD_LIBS += -lhdf5 -lhdf5_hl -lhdf5_cpp -lz -ldl -lm -Wl,-rpath
endif

# CLI Arguments
ifeq ($(firstword $(MAKECMDGOALS)),$(filter all run runs process processs main $(PROGRAM) $(RUN),$(firstword $(MAKECMDGOALS))))
RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(RUN_ARGS):;@:)
endif

ifeq ($(firstword $(MAKECMDGOALS)),$(filter plot plots $(PLOT),$(firstword $(MAKECMDGOALS))))
PROCESS_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
$(eval $(PROCESS_ARGS):;@:)
endif


# Make Options
.PHONY: all run test clean plot process profile runs plots processs main $(PROGRAM) $(RUN) $(PLOT)
.SILENT : all run test clean plot process profile runs plots processs main $(PROGRAM) $(RUN) $(PLOT)
.PRECIOUS : $(OBJ_DIR)/%.$(OBJ_EXT) $(DEP_DIR)/%.$(DEP_EXT)
.SECONDEXPANSION:


# Rules
all : clean $(EXE)

run : $(EXE) clean_data
	$(CMD)$(EXE) $(RUN_ARGS)

runs : $(EXES) clean_data
	$(foreach i,$(SEQ),@$(CMD)$(word $(i),$(EXES)) $(RUN_ARGS);)	

$(RUN) : $(EXE_DIR)/$$(patsubst run_%,%,$$(@)).$(EXE_EXT)
	@$(CMD)$(EXE_DIR)/$(patsubst run_%,%,$@).$(EXE_EXT) $(RUN_ARGS)

test :

clean : clean_obj clean_data

plot : $(PROCESS) $(PROCESS_DEP)
	@$(PROCESS) $(PROCESS_ARGS) $(PROCESS_DEP) $(PROCESS_FLAGS)

plots : $(PROCESSS) $(PROCESSS_DEP)
	$(foreach i,$(SEQ),$(word $(i),$(PROCESSS)) $(word $(i),$(PROCESSS_DEP)) $(PROCESSS_FLAGS);)


$(PLOT) : $(PROCESS_DIR)/$$(patsubst plot_%,%,$$@).$(PROCESS_EXT)
	$(PROCESS_DIR)/$(patsubst plot_%,%,$@).$(PROCESS_EXT) $(DATA_DIR)/$(patsubst plot_%,%,$@)/data.$(DATA_EXT) $(PROCESS_FLAGS)

process : run plot

processs : runs plots

profile : $(EXE) $(PROFILE_DEP)
	$(PROFILE) $(PROFILE_FLAGS) $< $(PROFILE_DEP) > $(PROFILE_OUT)
	$(foreach i,$(SEQ),$(PROFILE) $(PROFILE_FLAGS) $(word $(i),$(EXES)) $(PROFILE_DEP) > $(PROFILE_OUT);)
# 	h5dump $(PROCESSS_DEP)

$(PROGRAM) : run_$$@ plot_$$@

main : run plot

clean_obj :
	@$(RM) $(OBJ_DIR)
	@$(RM) $(EXE_DIR)

clean_data :
	@$(RM) $(DATA_DIR)/*.$(DATA_EXT)
	@$(RM) $(DATA_DIR)/*/*.$(DATA_EXT)
	@$(RM) $(PROFILE_DEP)

$(EXE_DIR)/%.$(EXE_EXT) : $(OBJ_DIR)/%.$(OBJ_EXT) $(OBJ) $(LIB) | $(EXE_DIR)
	$(CC) $(CC_FLAGS) $< $(OBJ) -o $@ $(LD_FLAGS) $(LD_LIBS)

$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT) $(LIB) | $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@ $(LD_FLAGS) $(LD_LIBS)

$(OBJ_DIR) $(EXE_DIR) : 
	@$(MKDIR) $@

$(PROCESS_DEP):
$(PROCESSS_DEP):

# $(PROCESS_DEP): $(patsubst $(DATA_DIR)/data.$(DATA_EXT),$(MAIN),$$@)
# $(PROCESSS_DEP): $$($$(patsubst $(DATA_DIR)/%/data.$(DATA_EXT),%,$$@))

# Make Options
ifdef USE_DEP
-include $(OBJ:.$(OBJ_EXT)=.$(DEP_EXT)) 
endif