#!/bin/bash
SHELL_FOLDER=$(dirname $(readlink -f "$0"))
cd $SHELL_FOLDER

ulimit -c unlimited
ulimit -n 120000
export LD_LIBRARY_PATH=../../../../../../output/lib:../../../../../../third_party_linux/centos6/qpid-proton:../../../../../../third_party_linux/centos6/qpid-cpp:../../../../../../third_party_linux/centos6/python3.6:../../../../../../third_party_linux/centos6/log4cxx:../../../../../../third_party_linux/centos6/openssl-1.1.1d:../../../../../../third_party_linux/centos6/zookeeper:LD_LIBRARY_PATH
export CLASSPATH=`find $SHELL_FOLDER/javalib  -name  *.jar | xargs | sed  "s/ /:/g"`

java  -Djava.library.path=$LD_LIBRARY_PATH  -cp  .:$CLASSPATH  sumscope.lava.test.testroles.LavaMqClientD
