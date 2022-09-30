
ifeq ($(PLATFORM), x64)
SUFFIX=64
ARCHFLAGS=-m64 -fPIC
else
SUFFIX=
ARCHFLAGS=-m32
endif

INC_DIRS  = -I../../../include
INC_DIRS += -I../../../WSA_API/include \
-I../../../cmo_markit

LDLIBS  = -L../../../lib$(SUFFIX) -lwsa -lCmoLinux
LDLIBS += -L../../../build$(SUFFIX).Release/lib/

PROJ_NAME=moodys_credit_model_example

help:
	@echo 'make [PLATFORM=x64] [help|all|clean]'
	@echo 'to build exe, type:'
	@echo -e '\tmake all'
	@echo 'for 64-bit build, type:'
	@echo -e '\tmake PLATFORM=x64 all'
	
SRCS := $(wildcard *.cpp) $(wildcard $(addsuffix /*.cpp, $(SRCDIR)))
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.cpp,$(MISSING_DEPS)))


all: ${PROJ_NAME}.exe

deps : $(DEPS)

objs : $(OBJS)

clean:
	rm -f ${PROJ_NAME}.d ${PROJ_NAME}.o ${PROJ_NAME}.exe *.o *.d
	
%.o: %.cpp
	g++ -c  -DNDEBUG -DUNIX -D__UNIX $(INC_DIRS) $(ARCHFLAGS) -O2 -w -fmessage-length=0 -o $@ $<

$(PROJ_NAME).exe : $(OBJS)
	$(CC) -O2 -w -fmessage-length=0 $(ARCHFLAGS) -o $@ $(OBJS) $(LDLIBS)
