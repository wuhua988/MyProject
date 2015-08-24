sinclude ../include.mk
CXXFLAGS +=  -I../Common \
	-I../thirdparty/libev \
	-I../thirdparty 
LDFLAGS += -L"/usr/local/lib" -L"../lib"
SRCDIRS = . redisClient Business
SRCEXTS = .cpp
LIBS = -lpthread -lrt -lCommonUtilitiesLib -lev
TARGET = libFramework.a

all : $(TARGET)

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS = $(addsuffix .o, $(basename $(SOURCES)))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	#$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)
	$(AR) rcs $@ $(OBJS)
	#$(CXX) -fPIC -shared -o $@ $^
	cp libFramework.a ../lib

ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPS)
endif

%.d : %.cpp
	@$(make-prerequisite)

rebuild: clean all

clean:
	rm -f *.o *.d
	rm -f redisClient/*.o redisClient/*.d
	rm -f Business/*.o Business/*.d
	rm $(TARGET)
show:
	@echo $(SOURCES)
	@echo $(OBJS)
	@echo $(DEPS)