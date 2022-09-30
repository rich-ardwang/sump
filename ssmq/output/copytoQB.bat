echo off

set QBBIN=D:\git\QB\bin

cd /d %~dp0

rem lib
echo f | xcopy  %~dp0SSMQ\lib\win32_vc9_debug\sdbusd.lib  %QBBIN%\debug\sdbusd.lib /y
echo f | xcopy  %~dp0SSMQ\lib\win32_vc9_debug\SSMQClient.lib %QBBIN%\debug\SSMQClient.lib /y
echo f | xcopy  %~dp0QB-QM\lib\win32_vc9_debug\QBToQM.lib %QBBIN%\debug\QBToQM.lib /y

echo f | xcopy  %~dp0SSMQ\lib\win32_vc9_release\sdbus.lib %QBBIN%\release\sdbus.lib /y
echo f | xcopy  %~dp0SSMQ\lib\win32_vc9_release\SSMQClient.lib %QBBIN%\release\SSMQClient.lib /y
echo f | xcopy  %~dp0QB-QM\lib\win32_vc9_release\QBToQM.lib %QBBIN%\release\QBToQM.lib /y

rem bin
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\sdbusd.dll %QBBIN%\debug\sdbusd.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\sdbusd.pdb %QBBIN%\debug\sdbusd.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\zmq_engined.dll %QBBIN%\debug\zmq_engined.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\zmq_engined.pdb %QBBIN%\debug\zmq_engined.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\SSMQClient.dll %QBBIN%\debug\SSMQClient.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\SSMQClient.pdb %QBBIN%\debug\SSMQClient.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\SSMQServer.exe %QBBIN%\debug\SSMQServer.exe /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\SSMQServer.pdb %QBBIN%\debug\SSMQServer.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\QBToQM.dll %QBBIN%\debug\QBToQM.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\QBToQM.pdb %QBBIN%\debug\QBToQM.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_debug\config\ssmg.ini %QBBIN%\debug\config\ssmg.ini /y

echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\sdbus.dll %QBBIN%\release\sdbus.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\sdbus.pdb %QBBIN%\release\sdbus.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\zmq_engine.dll %QBBIN%\release\zmq_engine.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\zmq_engine.pdb %QBBIN%\release\zmq_engine.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\SSMQClient.dll %QBBIN%\release\SSMQClient.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\SSMQClient.pdb %QBBIN%\release\SSMQClient.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\SSMQServer.exe %QBBIN%\release\SSMQServer.exe /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\SSMQServer.pdb %QBBIN%\release\SSMQServer.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\QBToQM.dll %QBBIN%\release\QBToQM.dll /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\QBToQM.pdb %QBBIN%\release\QBToQM.pdb /y
echo f | xcopy  %~dp0QB-QM\bin\win32_vc9_release\config\ssmg.ini %QBBIN%\release\config\ssmg.ini /y


echo on