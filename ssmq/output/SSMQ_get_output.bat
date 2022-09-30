rem %1 SubDir

echo off

cd /d %~dp0

rem lib
echo f | xcopy  %~dp0..\lib\win32_vc9_notset_debug\sdbusd.lib %~dp0%1\lib\win32_vc9_debug\sdbusd.lib /y
echo f | xcopy  %~dp0..\lib\win32_vc9_notset_debug\SSMQClient.lib %~dp0%1\lib\win32_vc9_debug\SSMQClient.lib /y

echo f | xcopy  %~dp0..\lib\win32_vc9_notset_release\sdbus.lib %~dp0%1\lib\win32_vc9_release\sdbus.lib /y
echo f | xcopy  %~dp0..\lib\win32_vc9_notset_release\SSMQClient.lib %~dp0%1\lib\win32_vc9_release\SSMQClient.lib /y

echo f | xcopy  %~dp0..\lib\win32_vc10_notset_debug\sdbusd.lib %~dp0%1\lib\win32_vc10_debug\sdbusd.lib /y
echo f | xcopy  %~dp0..\lib\win32_vc10_notset_debug\SSMQClient.lib %~dp0%1\lib\win32_vc10_debug\SSMQClient.lib /y

echo f | xcopy  %~dp0..\lib\win32_vc10_notset_release\sdbus.lib %~dp0%1\lib\win32_vc10_release\sdbus.lib /y
echo f | xcopy  %~dp0..\lib\win32_vc10_notset_release\SSMQClient.lib %~dp0%1\lib\win32_vc10_release\SSMQClient.lib /y

rem inc
echo f | xcopy  %~dp0..\inc\SSMQ\ISSMQMng.h %~dp0%1\inc\SSMQ\ISSMQMng.h /y
echo f | xcopy  %~dp0..\inc\SSMQ\ISSMQObs.h %~dp0%1\inc\SSMQ\ISSMQObs.h /y
rem svn export svn://192.168.1.201/manager/VC/SSMQ/trunk/inc/sdbus %~dp0%1/inc/sdbus
xcopy  %~dp0..\inc\sdbus %~dp0%1\inc\sdbus\

rem bin
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_debug\sdbusd.dll %~dp0%1\bin\win32_vc9_debug\sdbusd.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_debug\zmq_engined.dll %~dp0%1\bin\win32_vc9_debug\zmq_engined.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_debug\SSMQClient.dll %~dp0%1\bin\win32_vc9_debug\SSMQClient.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_debug\SSMQServer.exe %~dp0%1\bin\win32_vc9_debug\SSMQServer.exe /y
echo f | xcopy  %~dp0..\static\config\ssmg.ini %~dp0%1\bin\win32_vc9_debug\config\ssmg.ini /y
echo f | xcopy  %~dp0..\static\config\logconfig.ini %~dp0%1\bin\win32_vc9_debug\config\logconfig.ini /y

echo f | xcopy  %~dp0..\bin\win32_vc9_notset_release\sdbus.dll %~dp0%1\bin\win32_vc9_release\sdbus.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_release\zmq_engine.dll %~dp0%1\bin\win32_vc9_release\zmq_engine.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_release\SSMQClient.dll %~dp0%1\bin\win32_vc9_release\SSMQClient.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc9_notset_release\SSMQServer.exe %~dp0%1\bin\win32_vc9_release\SSMQServer.exe /y
echo f | xcopy  %~dp0..\static\config\ssmg.ini %~dp0%1\bin\win32_vc9_release\config\ssmg.ini /y
echo f | xcopy  %~dp0..\static\config\logconfig.ini %~dp0%1\bin\win32_vc9_release\config\logconfig.ini /y

echo f | xcopy  %~dp0..\bin\win32_vc10_notset_debug\sdbusd.dll %~dp0%1\bin\win32_vc10_debug\sdbusd.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_debug\zmq_engined.dll %~dp0%1\bin\win32_vc10_debug\zmq_engined.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_debug\SSMQClient.dll %~dp0%1\bin\win32_vc10_debug\SSMQClient.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_debug\SSMQServer.exe %~dp0%1\bin\win32_vc10_debug\SSMQServer.exe /y
echo f | xcopy  %~dp0..\static\config\ssmg.ini %~dp0%1\bin\win32_vc10_debug\config\ssmg.ini /y
echo f | xcopy  %~dp0..\static\config\logconfig.ini %~dp0%1\bin\win32_vc10_debug\config\logconfig.ini /y

echo f | xcopy  %~dp0..\bin\win32_vc10_notset_release\sdbus.dll %~dp0%1\bin\win32_vc10_release\sdbus.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_release\zmq_engine.dll %~dp0%1\bin\win32_vc10_release\zmq_engine.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_release\SSMQClient.dll %~dp0%1\bin\win32_vc10_release\SSMQClient.dll /y
echo f | xcopy  %~dp0..\bin\win32_vc10_notset_release\SSMQServer.exe %~dp0%1\bin\win32_vc10_release\SSMQServer.exe /y
echo f | xcopy  %~dp0..\static\config\ssmg.ini %~dp0%1\bin\win32_vc10_release\config\ssmg.ini /y
echo f | xcopy  %~dp0..\static\config\logconfig.ini %~dp0%1\bin\win32_vc10_release\config\logconfig.ini /y

echo on