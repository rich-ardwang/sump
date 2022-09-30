cd /d %~dp0
set PATH=.\lava_win_x64;%PATH%;
java  -Dservice_log_dir=.  -Djava.library.path=./lava_win_x64  -cp   ./target/javalib/*;./target/lava-mq-test-1.2-SNAPSHOT.jar   sumscope.lava.test.testroles.LavaMqServerC