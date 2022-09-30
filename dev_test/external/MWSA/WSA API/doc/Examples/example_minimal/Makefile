
ifeq ($(PLATFORM), x64)
SUFFIX=64
ARCHFLAGS=-m64 -fPIC
else
SUFFIX=
ARCHFLAGS=-m32
endif

INC_DIRS  = -I../../../include

ifeq ($(STATICLINK),1)
LDLIBS  = -rdynamic -lboost_filesystem -lboost_system -lboost_date_time -lboost_thread -lpthread -lz -ldl \
-L../../../lib$(SUFFIX) -Wl,-Bstatic -lwsa -Wl,-Bdynamic
else
LDLIBS  = -L../../../lib$(SUFFIX) -lwsa -lCmoLinux
endif

PROJ_NAME=example_minimal

help:
	@echo 'make [STATICLINK=1|0] [PLATFORM=x64] [help|all|clean]'
	@echo 'to build exe, type:'
	@echo -e '\tmake all'
	@echo 'for 64-bit build, type:'
	@echo -e '\tmake PLATFORM=x64 all'

all: ${PROJ_NAME}.exe

clean:
	rm -f ${PROJ_NAME}.d ${PROJ_NAME}.o ${PROJ_NAME}.exe

%.o: %.cpp
	$(CXX) -c  -DNDEBUG -DUNIX -D__UNIX $(INC_DIRS) $(ARCHFLAGS) -O2 -w -fmessage-length=0 -o $@ $<
%.exe : %.o
	$(CXX)  $(ARCHFLAGS) -O2 -w -fmessage-length=0 -o $@ $< $(LDLIBS)
