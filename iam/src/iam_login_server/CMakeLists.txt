FILE(GLOB SERVER_SOURCE
"./core/*.cpp"
"./http/*.cpp"
"./lava/*.cpp"
"./msgbus/*.cpp"
"./process/*.cpp"
"./process/*.cpp"
"../../vendors/linux/cJSON/*.c"
)

message(STATUS "src_list: " ${SERVER_SOURCE})

add_executable(iam_login_server ${SERVER_SOURCE})

target_link_libraries(iam_login_server
	pthread
	msgbus
	log4cxx
	lava_box
	lava_utils
	lava_comm
	lava_proto
	zookeeper_mt
	python3.6m
	boost_thread
	boost_system)

install(TARGETS iam_login_server DESTINATION ${INSTALL_DIR}/bin)
