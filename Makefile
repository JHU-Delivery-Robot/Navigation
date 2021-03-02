# Root recursive Makefile
PROJECT_ROOT  := $(shell pwd)
CFGDIR        := $(PROJECT_ROOT)/buildcfg
export PROJECT_ROOT
export CFGDIR

# includes must come after this block since they depend on the PROJECT_ROOT var
include $(CFGDIR)/build-meta.mk

ARTEFACTS := roboctrl robosim simtests

.DEFAULT_GOAL := all

# metatargets
all: all-bins all-tests
all-bins: $(ARTEFACTS)
all-tests: simtests

# subproject targets
# common to all subprojects
project-common: hal lib
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
hal:
	$(MAKE) -C hal
lib:
	$(MAKE) -C lib

# test targets
# all tests
test: all-tests cov-clean
	./simtests

# coverage targets
coverage.json: test
	@gcovr -e lib -e tests -d --json -o $@
	@sed -i 's/build\///g' $@
	@echo "Generated intermediate $@"
cov-report: coverage.json
	@mkdir -p cov-report
	@gcovr --add-tracefile $< --html-details -o cov-report/cov.html
	@echo "Open file://$(PWD)/cov-report/cov.html to view detailed coverage info!"
cov: coverage.json
	@gcovr --add-tracefile $<

compiledb:
	rm -f compile_commands.json
	$(MAKE) -C robot $@
	$(MAKE) -C sim $@
	$(MAKE) -C tests $@
	$(MAKE) -C lib $@
cov-clean:
	rm -rf cov-report
	find . -regex '.*\.gcda' -delete
clean:
	rm -f $(ARTEFACTS)
	$(call_recursive)

deep-clean: clean
	$(MAKE) -C lib clean

# special make targets
.PHONY: all all_bins all_tests hal roboctrl robosim tests-common simtests project-common lib compiledb clean deep-clean cov-clean
.INTERMEDIATE: coverage.json

# utilities
define call_recursive
	$(MAKE) -C robot $@
	$(MAKE) -C sim $@
	$(MAKE) -C tests $@
endef
