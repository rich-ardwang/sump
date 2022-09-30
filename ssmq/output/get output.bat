set PATH = %PATH%;C:\Program Files\TortoiseSVN\bin

rmdir SSMQ /S /Q
call SSMQ_get_output.bat SSMQ
rmdir QB-QM /S /Q
call QB-QM_get_output.bat QB-QM