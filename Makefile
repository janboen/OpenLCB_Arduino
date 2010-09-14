
# Top-level Makefile for OpenLCB C/C++ implementations

# Arduino sketch directories
SUBDIRS := $(foreach V,$(wildcard */Makefile),$(V:/Makefile=))

%.all:
	make -C $(@:.all=) all 

# make sure Arduino sketches build OK first
all: $(foreach v,$(SUBDIRS),$(v).all)
	# build common C library and test
	cd libraries/OlcbTestCAN; make run

