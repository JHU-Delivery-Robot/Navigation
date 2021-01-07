# Self configuring makefile
# Just add dependencies at the indicated lines and watch the tedium of writing
# make rules get automated for you!

# compiler config
CC          := gcc
CFLAGS      := -std=c11 -pedantic -Wall -Wextra
DEBUG_FLAGS := -g -Og
# some GCC magic flags to autogen deps. In order:
# -MT $@ : set name of target in the dep makefile to be the original target
#   name in the main makefile
# -MMD : generate dependency information as a side effect of compilation,
#   ignoring system header deps (use -MD to include systen deps)
# -MP : add a target for each prereq to the list (in generated dep Makefile)
# -MF $(BUILDIR)/$*.mk : write the generated dep file to the listed path
DEPFLAGS     = -MT $@ -MMD -MP -MF $(BUILDIR)/$*.mk
COMPILE.CC   = $(CC) $(DEPFLAGS) $(CFLAGS) $(DEBUG_FLAGS)
# linker config
LD          := gcc
LDFLAGS     := -lm


# Directory defs
BUILDIR := build
SRCDIR  := .

# add all object dependencies here without the .o extension
MAIN_DEPS := main fastmath poten_map obstacle_map

# force deafult goal to be the correct one
.DEFAULT_GOAL := all


all: main

# magic compilation rule
main: $(addprefix $(BUILDIR)/,$(addsuffix .o,$(MAIN_DEPS))) | $(BUILDIR)
	$(LD) -o $@ $^ $(LDFLAGS)

# object build targets (with the dep Makefiles listed as deps so that the files
# are rebuilt if the dependency files go missing for any reason
$(BUILDIR)/%.o: $(SRCDIR)/%.c $(BUILDIR)/%.mk | $(BUILDIR)
	$(COMPILE.CC) -o $@ -c $< $(LDFLAGS)

# phony targets
clean:
	rm -rf *.o main *~ build/
compiledb:
	compiledb make --always-make --dry-run all
.PHONY: all clean compiledb


# autogen deps includes and boilerplate to make everything just work (tm)
# target for buildir
$(BUILDIR):
	@mkdir -p $@

# define autogen'd deps files
SRC_DEP_FILES  := $(patsubst $(SRCDIR)/%.c,$(BUILDIR)/%.mk,$(wildcard $(SRCDIR)/*.c))

# these empty targets are here to allow make to track the dependency files as
# deps themselves
$(SRC_DEP_FILES):

# include generated dependency files so objects are remade correctly
include $(wildcard $(SRC_DEP_FILES))
