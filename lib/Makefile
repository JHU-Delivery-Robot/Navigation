# recursive makefile for tests
export PROJECT_ROOT
export CFGDIR

.DEFAULT_GOAL := all

all: stb catch

stb:
	$(MAKE) -C $@
catch:
	$(MAKE) -C $@

compiledb clean:
	$(MAKE) -C stb $@
	$(MAKE) -C catch $@

.PHONY: all stb catch clean
