CXXFLAGS = -g -Wall -march=x86-64
CXXFLAGS += $(COMPILER_FLAGS) $(INCLUDE_FLAG) ../PlatformHeader.h
LDFLAGS = -march=x86-64

CXX = g++

define	make-prerequisite
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's/\s*\(.*\)\.o\s*:/\1.o \1.d :/g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;
endef