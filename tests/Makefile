# recursive makefile for tests
export PROJECT_ROOT
export CFGDIR

.DEFAULT_GOAL := all

all: all_tests
all_tests: simtests

tests-common:
	$(MAKE) -C common
simtests: tests-common
	$(MAKE) -C sim

compiledb:
	$(MAKE) -C sim $@
	$(MAKE) -C common $@
clean:
	$(MAKE) -C sim $@

.PHONY: all all_tests test-common simtests compiledb clean
