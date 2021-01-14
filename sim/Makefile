# Custom auto-dependency generating makefile
#
# The official docs way of autogenning (see:
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html)
# deps causes make warnings. I've sourced an improved way from
# (http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/)
# which relies on GCC special flags to avoid reinvoking make. Pretty neat.

###################################################
#            BEGIN MAKEFILE SETTINGS              #
###################################################
CXX         := g++
DBGFLAGS    := -g3 -ggdb -Og
CXXFLAGS    := -Wall -Wextra -pedantic -std=c++11
# some GCC magic flags to autogen deps. In order:
# -MT $@ : set name of target in the dep makefile to be the original target
#   name in the main makefile
# -MMD : generate dependency information as a side effect of compilation,
#   ignoring system header deps (use -MD to include systen deps)
# -MP : add a target for each prereq to the list (in generated dep Makefile)
# -MF $(BUILDIR)/$*.mk : write the generated dep file to the listed path
DEPFLAGS     = -MT $@ -MMD -MP -MF $(BUILDIR)/$*.mk
COMPILE.CXX  = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DBGFLAGS)
# linker config
LD          := gcc
LDLIBS     := -lm

# Directory defs
BUILDIR := build
SRCDIR  := .

SRCEXT  := cpp
DEPEXT  := mk
OBJEXT  := o

# Source files needed only for the simulation program
SIM_DEPS      := main motion obstacle_map

# force default goal to be the one required by gradescope
.DEFAULT_GOAL  := all


###################################################
#            BEGIN MAKEFILE PROPER                #
###################################################
all : robotsim

# Linker targets (DON'T REMOVE ANY TARGETS HERE!)
robotsim : $(addprefix $(BUILDIR)/,$(addsuffix .$(OBJEXT),$(SIM_DEPS))) | $(BUILDIR)
	$(CXX) -o $@ $^ $(LDLIBS)

# object build targets (with the dep Makefiles listed as deps so that the files
# are rebuilt if the dependency files go missing for any reason
$(BUILDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) $(BUILDIR)/%.$(DEPEXT) | $(BUILDIR)
	$(COMPILE.CXX) -o $@ -c $< $(LDLIBS)


# utility and phony targets
clean :
	rm -f robotsim
	rm -rf build
compiledb :
	compiledb make --always-make --dry-run all
.PHONY : all clean compiledb zip


# autogen deps includes and boilerplate to make everything just work (tm)
# target for buildir
$(BUILDIR):
	@mkdir -p $@

# define autogen'd deps files
SRC_DEP_FILES  := $(patsubst $(SRCDIR)/%.$(SRCEXT),$(BUILDIR)/%.$(DEPEXT),$(wildcard $(SRCDIR)/*.$(SRCEXT)))

# these empty targets are here to allow make to track the dependency files as
# deps themselves
$(SRC_DEP_FILES) :

# include generated dependency files so objects are remade correctly
include $(wildcard $(SRC_DEP_FILES))
