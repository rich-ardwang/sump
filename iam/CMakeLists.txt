cmake_minimum_required(VERSION 2.6)

project(sfc_iam_login_server)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Debug, RelWithDebInfo, Release
set(CMAKE_BUILD_TYPE Release)
SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fPIC -Wall -W -g3 -O2 -Wno-write-strings -Wno-deprecated -Wno-sign-compare -Wno-parentheses -Wno-reorder")

set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/install_package)

include_directories($ENV{DIST_DIR}/include
					${PROJECT_SOURCE_DIR}/vendors/linux/asio-1.12.2/include
					${PROJECT_SOURCE_DIR}/vendors/linux/log4cxx/header
					${PROJECT_SOURCE_DIR}/vendors/linux/python3.6/header
					${PROJECT_SOURCE_DIR}/vendors/linux/zookeeper/header
					${PROJECT_SOURCE_DIR}/vendors/linux/dist2/include
					${PROJECT_SOURCE_DIR}/vendors/linux/lava/header/lava_base
					${PROJECT_SOURCE_DIR}/vendors/linux/lava/header/lava_box
					${PROJECT_SOURCE_DIR}/vendors/linux/lava/header/lava_comm
					${PROJECT_SOURCE_DIR}/vendors/linux/lava/header/lava_proto
					${PROJECT_SOURCE_DIR}/vendors/linux/lava/header/lava_utils
					${PROJECT_SOURCE_DIR}/vendors/linux/cJSON
					${PROJECT_SOURCE_DIR}/src/iam_login_server/core
					${PROJECT_SOURCE_DIR}/src/iam_login_server/http
					${PROJECT_SOURCE_DIR}/src/iam_login_server/lava
					${PROJECT_SOURCE_DIR}/src/iam_login_server/msgbus
					${PROJECT_SOURCE_DIR}/src/iam_login_server/process)

link_directories($ENV{DIST_DIR}/lib64
				 ${PROJECT_SOURCE_DIR}/vendors/linux/log4cxx
				 ${PROJECT_SOURCE_DIR}/vendors/linux/python3.6
				 ${PROJECT_SOURCE_DIR}/vendors/linux/zookeeper
				 ${PROJECT_SOURCE_DIR}/vendors/linux/lava/lib
				 ${PROJECT_SOURCE_DIR}/vendors/linux/dist2/lib64)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/iam_login_server)

install(FILES config/setting.ini DESTINATION ${INSTALL_DIR}/bin)

