LIBS_PATH =$(ROOTPATH)/../../release/lib
LDFLAGS =-L$(LIBS_PATH) -lbasefw -lttnet -lttutils -levent -lpthread  -lstdc++  -lrt -ldl
#CFLAGS = -g -Wall -Wextra -Wno-unused-parameter -fsigned-char -fno-omit-frame-pointer -D_GNU_SOURCE
CFLAGS = -g -Wall -Wno-deprecated



CXX = g++
AR  = ar
#INCFLAG = -I$(ROOTPATH)/include \
#	-I$(ROOTPATH)/../../release/include/ttutils \
#	-I$(ROOTPATH)/../../release/include/ttnet

CXX += $(INCFLAG)