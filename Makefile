# Root recursive Makefile
PROJECT_ROOT  := $(shell pwd)
CFGDIR        := $(PROJECT_ROOT)/buildcfg
export PROJECT_ROOT
export CFGDIR

# includes must come after this block since they depend on the PROJECT_ROOT var
include $(CFGDIR)/build-meta.mk

ARTEFACTS := roboctrl robosim simtests

.DEFAULT_GOAL := all

all: all-bins all-tests
all-bins: $(ARTEFACTS)
all-tests: simtests

test: all-tests
	./simtests

# common to all subprojects
project-common: lib
# recursive targets
# robot side software
roboctrl: project-common
	$(MAKE) -C robot
	cp robot/$(BUILDIR)/$@ $(PROJECT_ROOT)
robosim: project-common
	$(MAKE) -C sim
	cp sim/$(BUILDIR)/$@ $(PROJECT_ROOT)
simtests: project-common robosim
	$(MAKE) -C tests $@
	cp tests/sim/$(BUILDIR)/$@ $(PROJECT_ROOT)
lib:
	$(MAKE) -C lib



compiledb:
	@rm -f compile_commands.json
	$(MAKE) -C robot $@
	$(MAKE) -C sim $@
	$(MAKE) -C tests $@
	$(MAKE) -C lib $@
clean:
	rm -f $(ARTEFACTS)
	$(MAKE) -C robot $@
	$(MAKE) -C sim $@
	$(MAKE) -C tests $@
deep-clean: clean
	$(MAKE) -C lib clean

.PHONY: all all_bins all_tests roboctrl robosim tests-common simtests project-common lib compiledb clean deep-clean
