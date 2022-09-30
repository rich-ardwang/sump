
ifeq ($(PLATFORM), x64)
SUFFIX=64
ARCHFLAGS=-m64 -fPIC
else
SUFFIX=
ARCHFLAGS=-m32
endif

INC_DIRS  = -I../../../include

LDLIBS  = -L../../../lib$(SUFFIX) -lwsa -lCmoLinux

PROJ_NAME=example1

help:
	@echo 'make [PLATFORM=x64] [help|all|clean]'
	@echo 'to build exe, type:'
	@echo -e '\tmake all'
	@echo 'for 64-bit build, type:'
	@echo -e '\tmake PLATFORM=x64 all'

all: ${PROJ_NAME}.exe

clean:
	rm -f ${PROJ_NAME}.d ${PROJ_NAME}.o ${PROJ_NAME}.exe

%.o: %.cpp
	g++ -c  -DNDEBUG -DUNIX -D__UNIX $(INC_DIRS) $(ARCHFLAGS) -O2 -w -fmessage-length=0 -o $@ $<
%.exe : %.o
	g++  $(ARCHFLAGS) -O2 -w -fmessage-length=0 -o $@ $< $(LDLIBS)
