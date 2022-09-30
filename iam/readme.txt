########################
####IAM项目开发说明#####
########################

一、总体说明
	login server是iam项目的重要组成部分，负责QB、QM、Excel或其他三方客户端接入dds的登录验证，
为方便扩展项目开发，将iam项目所使用的三方工具放在vendors目录下，将login_server放在src目录下，
日后如果iam项目需要开发其他应用程序或服务，则直接将source放在src目录下，并将新的工程整合在sln
中做统一管理即可。

二、工程目录说明
1、IAM工程root目录：/SFC/iam
2、/SFC/iam/bin：放置win32 debug环境测试所必需的exe、dll、配置文件等
3、/SFC/iam/config：放置配置文件的样例
4、/SFC/iam/doc：放置iam工程配套文档
5、/SFC/iam/include：放置iam工程共通的include头文件
6、/SFC/iam/lib：放置linux环境下编译所必需的所有库文件，CMakeLists.txt会使用此目录
7、/SFC/iam/src/iam_login_server：放置login server工程所有文件
8、/SFC/iam/vendors：放置iam工程依赖的所有3方库
9、/SFC/iam/vendors/linux：3方库的linux版本
10、/SFC/iam/vendors/windows：3方库的windows版本

三、login server开发须知
1、login server是完全使用c++11开发的，它不会用到boost库的任何功能，原则上我是不想加入boost模块的，
但是由于这个服务必需接入msgbus，而后者依赖boost，所有无法逃避编译时接入boost。
2、boost版本说明：
	2.1、windows编译环境使用的boost版本为1.47.0
	2.2、linux cent6.5环境使用的boost版本可能略高，因为msgbus曾经为centos7升级过版本，所以也提高了boost版本


