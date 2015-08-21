sinclude ../include.mk

SRCDIRS = .
SRCEXTS = .cpp .c
LIBS = -lpthread -lrt
TARGET = libCommonUtilitiesLib.a

all : $(TARGET)

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
OBJS = $(addsuffix .o, $(basename $(SOURCES)))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	#$(CXX) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS)
	$(AR) rcs $@ $(OBJS)
#	$(CXX) -fPIC -shared -o $@ $^
	cp $@ ../lib 

ifneq ($(MAKECMDGOALS),clean)
sinclude $(DEPS)
endif

%.d: %.cpp
	@$(make-prerequisite)
	
%.d: %.c
	@$(make-prerequisite)

rebuild: clean all

clean:
	rm -f *.o *.d

show:
	@echo $(SOURCES)
	@echo $(OBJS)
	@echo $(DEPS)