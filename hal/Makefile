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
COMPILE.CC  = $(CXX.LIBASE)
LDLIBS     := -lm

HAL_DEPS := $(call objpath,$(HAL_OBJS),$(BUILDIR))

# Directory defs
SRCDIR  := .

# force default goal to be all
.DEFAULT_GOAL  := all

###################################################
#            BEGIN MAKEFILE PROPER                #
###################################################
all: $(HAL_DEPS)

# object build targets (with the dep Makefiles listed as deps so that the files
# are rebuilt if the dependency files go missing for any reason
$(BUILDIR)/%.$(OBJ_EXT): $(SRCDIR)/%.$(CXX_EXT) $(BUILDIR)/%.$(MKDEP_EXT) | $(BUILDIR)
	$(COMPILE.CC) -o $@ -c $< $(LDLIBS)

# utility and phony targets
clean:
	rm -rf $(BUILDIR)
compiledb:
	compiledb -o $(PROJECT_ROOT)/compile_commands.json make --always-make --dry-run all
.PHONY: all clean compiledb

# autogen deps includes and boilerplate to make everything just work (tm)
# target for buildir
$(BUILDIR):
	@mkdir -p $@

# define autogen'd deps files
SRC_DEP_FILES  := $(patsubst $(SRCDIR)/%.$(CXX_EXT),$(BUILDIR)/%.$(MKDEP_EXT),$(wildcard $(SRCDIR)/*.$(CXX_EXT)))

# these empty targets are here to allow make to track the dependency files as
# deps themselves
$(SRC_DEP_FILES) :

# include generated dependency files so objects are remade correctly
include $(wildcard $(SRC_DEP_FILES))
