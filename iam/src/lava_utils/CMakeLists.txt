SET(BINNAME "lava_utils")

PROJECT(${BINNAME})
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(LIBRARY_OUTPUT_PATH ../build)

if(USE_32BITS)
  message(STATUS "Using 32bits")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
else()
  message(STATUS "Using 64bits")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif(USE_32BITS)


SET(VERSION "const char version[] = \"Revision: 0.1.0\"")
SET(PLATFORM "const char platform[] = $ENV{PLATFORM}")

MESSAGE(STATUS "Project: ${BINNAME}")
MESSAGE(STATUS "VERSION: ${VERSION}")
MESSAGE(STATUS "PLATFORM: ${PLATFORM}")
MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

if(CMAKE_LAVA_SOURCE_DIR)
else()
SET(CMAKE_LAVA_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
endif(CMAKE_LAVA_SOURCE_DIR)

FILE(GLOB SOURCE_MAIN
	"${CMAKE_LAVA_SOURCE_DIR}/*.cpp"
)

MESSAGE(STATUS "${SOURCE_MAIN}")

FILE(GLOB SOURCE_INCLUDE
	"${CMAKE_LAVA_SOURCE_DIR}/include/*.h"
)


string(REGEX MATCH ".*clang.*" USE_CLANG ${CMAKE_CXX_COMPILER})
IF (NOT USE_CLANG)
   IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      SET(USE_CLANG TRUE)
   ENDIF()
ENDIF(NOT USE_CLANG)

if(USE_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else(USE_CLANG)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif(USE_CLANG)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	MESSAGE(STATUS "Debug")
	SET(MY_BUILD_TYPE debug)
else()
	MESSAGE(STATUS "Release")
	SET(MY_BUILD_TYPE release)
endif()

SET(CMAKE_BUILE_TYPE RELEASE)
SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fPIC -Wall -W -g3 -O2 -Wno-write-strings -Wno-deprecated -Wno-sign-compare -Wno-parentheses -Wno-reorder")

SET(CMAKE_MODULE_PATH ../include)
SET(SYS_PATH /usr/local)

INCLUDE_DIRECTORIES(
	"${CMAKE_LAVA_SOURCE_DIR}"
	"${CMAKE_LAVA_SOURCE_DIR}/../include"
	"${CMAKE_LAVA_SOURCE_DIR}/../lava_utils/include"
	"${CMAKE_LAVA_SOURCE_DIR}/../third_party_linux/centos/log4cxx/header"
)

LINK_DIRECTORIES(
	"${CMAKE_LAVA_SOURCE_DIR}/build"
	"${CMAKE_LAVA_SOURCE_DIR}/../third_party_linux/centos/log4cxx"
)

if(USE_32BITS)
	ADD_DEFINITIONS(-DLAVA_LINUX -DASIO_STANDALONE  -DVERSION=${VERSION} -DPLATFORM=${PLATFORM})
else()
	ADD_DEFINITIONS(-DLAVA_LINUX -DASIO_STANDALONE -DX64 -DVERSION=${VERSION} -DPLATFORM=${PLATFORM})
endif(USE_32BITS)

ADD_LIBRARY(lava_utils SHARED ${SOURCE_MAIN})

if(SVNVER)
	SET_TARGET_PROPERTIES(lava_utils PROPERTIES VERSION 0.1 SOVERSION $ENV{SVNVER})
else()
	SET_TARGET_PROPERTIES(lava_utils PROPERTIES VERSION 0.1 SOVERSION 0)
endif(SVNVER)

if(USE_CLANG)
	TARGET_LINK_LIBRARIES(lava_utils librt.dylib libpthread.dylib liblog4cxx.dylib libz.dylib libuuid.dylib)
else(USE_CLANG)
	TARGET_LINK_LIBRARIES(lava_utils  librt.so libpthread.so liblog4cxx.so libz.so libuuid.so)
endif(USE_CLANG)

SET_TARGET_PROPERTIES(lava_utils PROPERTIES PUBLIC_HEADER "${SOURCE_INCLUDE}")

INSTALL(TARGETS lava_utils
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION header/lava_utils
)

INSTALL(DIRECTORY  "${CMAKE_LAVA_SOURCE_DIR}/../include/" DESTINATION header/lava_base 
	FILES_MATCHING PATTERN "*.h")





