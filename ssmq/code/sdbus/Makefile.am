
AUTOMAKE_OPTIONS=foreign

INCLUDES=-I$(top_srcdir)/include -I$(top_srcdir)  -I $(BOOST_ROOT)/include -I .

noinst_LIBRARIES = libsdbus.a

libsdbus_a_SOURCES=callback.cpp codec.cpp connection.cpp datetime.cpp dict.cpp engine.cpp expr.cpp hash_map.cpp message.cpp options.cpp \
    queue.cpp sdbus.cpp string.cpp type.cpp  variant.cpp zip.cpp impl/codec_impl.cpp impl/dict_impl.cpp 
DEFS += -std=c++0x

libsdbus$(LIBEXT): $(libsdbus_a_OBJECTS)
	${CXXLINK} $(CXXFLAGS) $(LIBLINK_FLAGS) $(libsdbus_a_OBJECTS) $(BOOSTLIBS) $(LIBS) -L$(top_srcdir)/linux -lSSRTL 

all-am:$(all-am) libsdbus$(LIBEXT)  CPLIBRARY

CPLIBRARY:
	mv libsdbus$(LIBEXT) $(top_srcdir)/linux

