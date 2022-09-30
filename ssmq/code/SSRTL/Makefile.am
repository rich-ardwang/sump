AUTOMAKE_OPTIONS=foreign

INCLUDES=-I . -I$(top_srcdir) -I ${BOOST_ROOT}/include

noinst_LIBRARIES = libSSRTL.a

libSSRTL_a_SOURCES=SSConfig.cpp SSDateTime.cpp SSLog.cpp SSString.cpp SSThread.cpp SSType.cpp  SSQueue.cpp SSZip.cpp\
TinyXML/tinyxml.cpp TinyXML/tinyxmlparser.cpp TinyXML/tinyxmlerror.cpp	\
Atomic/Atomic_gcc.cpp Semaphore/Semaphore_posix.cpp SpinLock/SpinLock_posix.cpp \
zlib/adler32.c zlib/compress.c zlib/crc32.c zlib/deflate.c zlib/gzio.c zlib/infback.c\
zlib/inffast.c zlib/inflate.c zlib/inftrees.c zlib/trees.c zlib/uncompr.c zlib/zutil.c



all-am:$(all-am) CPLIBRARY

CPLIBRARY:
	mv 	$(noinst_LIBRARIES)  $(top_srcdir)/linux 
