# build system meta-include file.
# this is probably what you want to include from child makefiles unless you
# really know what you're doing
include $(CFGDIR)/base.mk 
include $(CFGDIR)/deps.mk
include $(CFGDIR)/headers.mk
