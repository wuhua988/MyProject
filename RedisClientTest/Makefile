sinclude ../include.mk
CXXFLAGS += -I../Common \
			-I../Framework \
			-I../thirdparty/libev
LDFLAGS += -L"../lib"
SRCDIRS = . 
SRCEXTS = .cpp
LIBS = -lpthread -lrt -lFramework -lCommonUtilitiesLib -lev -lprotobuf
TARGET = redisCli

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
	rm $(TARGET)
show:
	@echo $(SOURCES)
	@echo $(OBJS)
	@echo $(DEPS)