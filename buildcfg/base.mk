# Common compiler settings
DEBUG_FLAGS   := -g3 -ggdb -Og
RELEASE_FLAGS := -O2
COVFLAGS      := -fprofile-arcs -ftest-coverage
# some GCC magic flags to autogen deps. In order:
# -MT $@ : set name of target in the dep makefile to be the original target
#   name in the main makefile
# -MMD : generate dependency information as a side effect of compilation,
#   ignoring system header deps (use -MD to include systen deps)
# -MP : add a target for each prereq to the list (in generated dep Makefile)
# -MF $(BUILDIR)/$*.mk : write the generated dep file to the listed path
DEPFLAGS       = -MT $@ -MMD -MP -MF $(BUILDIR)/$*.mk
# set default build config to debug
ifndef CONFIG
	CONFIG := debug
endif

# C++ compiler settings
CXX        := g++
CXXSTD     := -std=c++11
CXXFLAGS   := -Wall -Wextra -pedantic
CXX.BASE    = $(CXX) $(CXXSTD) $(DEPFLAGS) $(CXXFLAGS)
CXX.LIBASE  = $(CXX) $(CXXSTD) $(DEPFLAGS)
ifeq ($(CONFIG), release)
	CXXFLAGS += $(RELEASE_FLAGS)
else ifeq ($(CONFIG), debug)
	CXXFLAGS += $(DEBUG_FLAGS) $(COVFLAGS)
endif

# C compiler settings
CC        := gcc
CSTD      := -std=c11
CFLAGS    := -Wall -Wextra -pedantic
CC.BASE    = $(CC) $(CSTD) $(DEPFLAGS) $(CFLAGS)
CC.LIBASE  = $(CC) $(CSTD) $(DEPFLAGS)
ifeq ($(CONFIG), release)
	CFLAGS += $(RELEASE_FLAGS)
else ifeq ($(CONFIG), debug)
	CFLAGS += $(DEBUG_FLAGS) $(COVFLAGS)
endif

# linker config
LD          := g++
LDLIBS      :=
ifeq ($(CONFIG), debug)
	LDLIBS += -lgcov
endif

# directory defs
BUILDIR := build

# filexts
C_EXT     := c
CXX_EXT   := cpp
MKDEP_EXT := mk
OBJ_EXT   := o

# common macros
# Must use deferred assignment to ensure that $(call ...) can replace the
# vars
wrap = $(addprefix $(1)/,$(addsuffix $(2),$(3)))
objpath = $(addprefix $(2)/,$(addsuffix .$(OBJ_EXT),$(1)))
