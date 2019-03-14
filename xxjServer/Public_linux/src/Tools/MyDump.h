#ifndef __MYDUMP_H_
#define __MYDUMP_H_
#include  <dbghelp.h> 
#include <winnt.h>
#include <shellapi.h>
#include <time.h>
#include <stdio.h>
#define _T 

#pragma comment(lib,  "dbghelp.lib")


class MyDmp
{
public:

	//设置异常处理回调函数
	//SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	//如果无效 			
	//调用  在设置自己的异常处理函数后，调用DisableSetUnhandledExceptionFilter 禁止CRT 设置即可。

	//异常处理代码
	//EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了 
	//EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束 
	//EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行 
	static long   __stdcall   MyUnhandledExceptionFilter(_EXCEPTION_POINTERS*   ExceptionInfo)
	{
		//std::string str = (boost::format("%d.dmp") % time_t(0)).str().c_str();
		printf("------dump!\n");
		char _str[1024];
		memset(_str, 0, 1024 * sizeof(char));
		time_t t;
		time(&t);
		wsprintf(_str, "%d.dmp", t);

		HANDLE   hFile = CreateFile(_str, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION   ExInfo;

			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = ExceptionInfo;
			ExInfo.ClientPointers = NULL;

			//   write   the   dump
			BOOL   bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
			CloseHandle(hFile);
		}

		Sleep(60000);
		ShellExecute(NULL, "open", "AlarmMail.exe", NULL, NULL, SW_SHOWNORMAL);

		// 	WCHAR szAppPath[MAX_PATH];
		// 	GetModuleFileName(NULL, szAppPath, MAX_PATH);
		// 	(wcsrchr(szAppPath, '\\'))[1] = 0;
		// 	std::wstring strpath = L"/select, " + std::wstring(szAppPath) + L"0.dmp";

		//MessageBox(NULL, L"程序出现错误，请将dmp文件发给开发人员！谢谢", L"提示", MB_OK);
		//ShellExecute(NULL, L"open", L"explorer.exe", strpath.c_str(), NULL, SW_SHOWNORMAL);
		//theApp.PostThreadMessage(WM_QUIT, 0, 0);
		OnQuit();
		return EXCEPTION_EXECUTE_HANDLER;
	}

	static long  __stdcall MyUnhandledExceptionFilterNull(_EXCEPTION_POINTERS*   ExceptionInfo)
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	static void DisableSetUnhandledExceptionFilter()
	{
		void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")),
			"SetUnhandledExceptionFilter");
		if (addr)
		{
			unsigned char code[16];
			int size = 0;
			code[size++] = 0x33;
			code[size++] = 0xC0;
			code[size++] = 0xC2;
			code[size++] = 0x04;
			code[size++] = 0x00;

			DWORD dwOldFlag, dwTempFlag;
			VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
			WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
			VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
		}
	}

	static void OnQuit()
	{

	}

	static void OnExitProcess()
	{
		OnQuit();
		::ExitProcess(0);
	}
};
//内存检测 监视

#define MEMORY_CHICK \
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);\
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;\
	_CrtSetDbgFlag(tmpFlag);


// void detect_memory_leaks( bool on_off )  
// {  
// 	int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);  
// 	if(!on_off)  
// 		flags &= ~_CRTDBG_LEAK_CHECK_DF;  
// 	else {  
// 		flags |= _CRTDBG_LEAK_CHECK_DF;  
// 		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);  
// 		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);  
// 	}  
// 	_CrtSetDbgFlag( flags );  
// } 
////_CrtSetBreakAlloc(0x022C50A8);
#endif