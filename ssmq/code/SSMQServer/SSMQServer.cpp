// SSMQServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SSMQServer.h"
#include "SSMQServerFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

//CWinApp theApp;
//
//using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	/*
	// ȥ��MFC����
	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		CSSMQServerFrame Frame;
		Frame.Run();
	}
	*/
	CSSMQServerFrame Frame;
	Frame.Run();

	return 0;
}
