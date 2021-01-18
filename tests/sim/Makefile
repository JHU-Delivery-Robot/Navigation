# Custom auto-dependency generating makefile
#
# The official docs way of autogenning (see:
# https://www.gnu.org/software/make/manual/html_node/Automatic-Prerequisites.html)
# deps causes make warnings. I've sourced an improved way from
# (http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/)
# which relies on GCC special flags to avoid reinvoking make. Pretty neat.
include $(CFGDIR)/build-meta.mk
###################################################
#            BEGIN MAKEFILE SETTINGS              #
###################################################
COMPILE.CXX   = $(CXX.BASE) $(DBGFLAGS) $(SIMTEST_INCLUDES)
LDLIBS       :=

# sub-project deps
SIMTEST_OBJS := test_obstacle_map test_sensor_gen
SIMTEST_DEPS := $(call objpath,$(SIMTEST_OBJS),$(BUILDIR)) $\
                $(call objpath,$(SIM_COMMON),$(SIM_ROOT)/$(BUILDIR)) $\
                $(call objpath,$(TEST_COMMON),$(TEST_COMMON_ROOT)/$(BUILDIR)) $\
                $(LIBCATCH_DEPS) $(LIBSTB_DEPS)

# Directory defs
SRCDIR  := .

# force default goal to be the one required by gradescope
.DEFAULT_GOAL  := all

###################################################
#            BEGIN MAKEFILE PROPER                #
###################################################
all: $(BUILDIR)/simtests

# Linker targets (DON'T REMOVE ANY TARGETS HERE!)
$(BUILDIR)/simtests: $(SIMTEST_DEPS) | $(BUILDIR)
	$(CXX) -o $@ $^ $(LDLIBS)

# object build targets (with the dep Makefiles listed as deps so that the files
# are rebuilt if the dependency files go missing for any reason
$(BUILDIR)/%.$(OBJ_EXT): $(SRCDIR)/%.$(CXX_EXT) $(BUILDIR)/%.$(MKDEP_EXT) | $(BUILDIR)
	$(COMPILE.CXX) -o $@ -c $< $(LDLIBS)


# utility and phony targets
clean:
	rm -rf $(BUILDIR)
compiledb:
	compiledb -o $(PROJECT_ROOT)/compile_commands.json make --always-make --dry-run all
.PHONY: all clean compiledb zip


# autogen deps includes and boilerplate to make everything just work (tm)
# target for buildir
$(BUILDIR):
	@mkdir -p $@

# define autogen'd deps files
SRC_DEP_FILES := $(patsubst $(SRCDIR)/%.$(CXX_EXT),$(BUILDIR)/%.$(MKDEP_EXT),$(wildcard $(SRCDIR)/*.$(CXX_EXT)))

# these empty targets are here to allow make to track the dependency files as
# deps themselves
$(SRC_DEP_FILES):

# include generated dependency files so objects are remade correctly
include $(wildcard $(SRC_DEP_FILES))
