ROOTPATH = $(shell pwd)

include include.mk

TARGETS = CommonUtilitiesLib.a

export CXX
export AR
export CFLAGS
export LDFLAGS
export ROOTPATH
export TARGETS

.PHONY: TARGETS test

all: lib install

lib:
	$(MAKE) -C source/
	#mv source/$(TARGETS) release/

install:
	mkdir -p $(LIBS_PATH)/
	cp release/$(TARGETS) $(LIBS_PATH)/
	mkdir -p $(LIBS_PATH)/../include/base
	cp ./include/*.h $(LIBS_PATH)/../include/base
	
test: 
	$(MAKE) -C test

clean:
	rm -f source/*.o source/*.d
	$(MAKE) -C source clean
#	$(MAKE) -C test clean

distclean: clean

