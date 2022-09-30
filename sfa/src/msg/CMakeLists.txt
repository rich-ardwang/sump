# 版本限定 
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# 项目名称 
PROJECT(msg) 

# 检查c++编译器标志，设置c++11支持变量
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
# 使用变量设置编译标志
IF(COMPILER_SUPPORTS_CXX11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSE()
    MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

# 设置编译器选项：O3优化，显示所有警告
#SET(CMAKE_C_FLAGS "$ENV{CFLAGS} -O3 -Wall") 

#导入导出宏
ADD_DEFINITIONS("-D SFA_BUILD_DLL")
ADD_DEFINITIONS("-D SFA_EXPORT")

# 定义头文件的路径变量
#SET(INCLUDE_PATH ${PROJECT_SOURCE_DIR}) 
#MESSAGE(STATUS "Include Path, ${INCLUDE_PATH}") 

# 定义源文件路径变量
SET(SOURCE_PATH ${PROJECT_SOURCE_DIR}/src) 
MESSAGE(STATUS "Source Path , ${SOURCE_PATH}") 

# 包含头文件路径
INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
) 

# 添加源文件路径下所有源文件存放到变量中(*.c && *.cpp)
AUX_SOURCE_DIRECTORY(${SOURCE_PATH} SRC_LIST) 
AUX_SOURCE_DIRECTORY(${SOURCE_PATH}/msg SRC_LIST)
MESSAGE("${SRC_LIST}")

# 设置动态库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) 
MESSAGE(STATUS "Library Output Path, " ${PROJECT_BINARY_DIR/lib}) 

# 生成动态库(libmsg.so)
ADD_LIBRARY(msg SHARED ${SRC_LIST}) 

# 生成静态库（libmsg.a，target名字只能有一个，所以不能与动态库的名字一样）
# ADD_LIBRARY(msg_static STATIC ${SRC_LIST})

# 但是可以通过下面的命令更改静态库target生成的库名，这样就和动态库的名字一样的了
# SET_TARGET_PROPERTIES(msg_static PROPERTIES OUTPUT_NAME "msg") 

