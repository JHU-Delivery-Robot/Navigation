# Common compiler settings
DBGFLAGS := -g3 -ggdb -Og
# some GCC magic flags to autogen deps. In order:
# -MT $@ : set name of target in the dep makefile to be the original target
#   name in the main makefile
# -MMD : generate dependency information as a side effect of compilation,
#   ignoring system header deps (use -MD to include systen deps)
# -MP : add a target for each prereq to the list (in generated dep Makefile)
# -MF $(BUILDIR)/$*.mk : write the generated dep file to the listed path
DEPFLAGS  = -MT $@ -MMD -MP -MF $(BUILDIR)/$*.mk

# C++ compiler settings
CXX      := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++11
CXX.BASE  = $(CXX) $(DEPFLAGS) $(CXXFLAGS)

# C compiler settings
CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c11
CC.BASE  = $(CC) $(DEPFLAGS) $(CFLAGS)

# linker config
LD          := g++

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
