sinclude ../include.mk
CXXFLAGS += -I../Common \
			-I../Framework \
			-I../thirdparty/libev \
			-I../thirdparty 
LDFLAGS += -L"../lib"
SRCDIRS = . protocol
SRCEXTS = .cpp .cc
LIBS = -lpthread -lrt -lFramework -lCommonUtilitiesLib -lev -lprotobuf
TARGET = echoServer

all : $(TARGET)

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS = $(addsuffix .o, $(basename $(SOURCES)))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)
	#$(AR) rcs $@ $(OBJS)
	#$(CXX) -fPIC -shared -o $@ $^

ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPS)
endif

%.d : %.cpp
	@$(make-prerequisite)
	
%.d : %.cc
	@$(make-prerequisite)

rebuild: clean all

clean:
	rm -f *.o *.d
	rm -f protocol/*.o
	rm -f protocol/*.d
show:
	@echo $(SOURCES)
	@echo $(OBJS)
	@echo $(DEPS)