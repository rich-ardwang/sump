// SSMQServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SSMQServer.h"
#include "SSMQServerFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

//CWinApp theApp;
//
//using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	/*
	// 去除MFC依赖
	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		CSSMQServerFrame Frame;
		Frame.Run();
	}
	*/
	CSSMQServerFrame Frame;
	Frame.Run();

	return 0;
}
