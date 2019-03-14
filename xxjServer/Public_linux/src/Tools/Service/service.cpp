//#include "stdafx.h"
#include "service.h"
#include <iostream>
#include <stdarg.h>
#include <functional>
#include <algorithm>

char CMyService::m_szServiceName[MaxServiceNAME] = {'\0'};
SERVICE_STATUS_HANDLE CMyService::hServiceStatus;
SERVICE_STATUS CMyService::status;
unsigned long CMyService::dwThreadID;
CMyService::CallBack CMyService::_callback_start = []() { std::cout << "no service callback start"; };
CMyService::CallBack CMyService::_callback_Stop = []() { std::cout << "no service callback stop "; };


//********************************************************
//执行一个命令
#include <Tlhelp32.h>
#include <Userenv.h>
#include <tchar.h>
#pragma comment(lib, "Userenv.lib")

DWORD FindSessionPid(LPSTR lpProcessName, DWORD dwSessionId)
{
	DWORD res = 0;

	PROCESSENTRY32 procEntry;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return res;
	}

	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnap, &procEntry))
	{
		goto _end;
	}

	do
	{
		if (_stricmp(procEntry.szExeFile, lpProcessName) == 0)
		{
			DWORD winlogonSessId = 0;
			if (ProcessIdToSessionId(procEntry.th32ProcessID, &winlogonSessId) && winlogonSessId == dwSessionId)
			{
				res = procEntry.th32ProcessID;
				break;
			}
		}

	} while (Process32Next(hSnap, &procEntry));

_end:
	CloseHandle(hSnap);
	return res;
}

bool CMyService::CmdLineExecuteDifferentSession(std::string lpCmdLine)
{
	if (lpCmdLine.length() == 0)
		return false;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bResult = FALSE;
	DWORD dwSessionId = 0, winlogonPid = 0;
	HANDLE hUserToken = nullptr, hUserTokenDup = nullptr, hPToken = nullptr, hProcess = nullptr;
	DWORD dwCreationFlags = 0;

	// Log the client on to the local computer.  

	typedef DWORD(WINAPI *__pfnWTSGetActiveConsoleSessionId)();
	typedef BOOL(WINAPI *__pfnWTSQueryUserToken)(ULONG SessionId, PHANDLE phToken);

	__pfnWTSGetActiveConsoleSessionId pfnWTSGetActiveConsoleSessionId =
		(__pfnWTSGetActiveConsoleSessionId)GetProcAddress(LoadLibraryA("kernel32.dll"), "WTSGetActiveConsoleSessionId");

	__pfnWTSQueryUserToken pfnWTSQueryUserToken =
		(__pfnWTSQueryUserToken)GetProcAddress(LoadLibraryA("Wtsapi32.dll"), "WTSQueryUserToken");

	if (pfnWTSGetActiveConsoleSessionId == NULL)
	{
		//WriteLog("Not found api: WTSGetActiveConsoleSessionId\n");
		return 0;
	}
	if (pfnWTSQueryUserToken == NULL)
	{
		//	WriteLog("Not found api: WTSQueryUserToken\n");
		return 0;
	}

	dwSessionId = pfnWTSGetActiveConsoleSessionId();

	winlogonPid = FindSessionPid((LPSTR)"explorer.exe", dwSessionId);

	if (winlogonPid == 0)
	{
		winlogonPid = FindSessionPid((LPSTR)"winlogon.exe", dwSessionId);
	}

	if (winlogonPid == 0)
	{
		//WriteLog("Can't Find Explorer\n");
		return 0;
	}

	////////////////////////////////////////////////////////////////////////  

	dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = (LPSTR)("winsta0\\default");
	ZeroMemory(&pi, sizeof(pi));

	TOKEN_PRIVILEGES tp;
	LUID luid;
	LPVOID TokenInformation;
	DWORD RetLen = 0;

	if (!pfnWTSQueryUserToken(dwSessionId, &hUserToken))
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, winlogonPid);

		if (!OpenProcessToken(hProcess, TOKEN_ALL_ACCESS_P, &hPToken))
		{
			char pTemp[121];
			sprintf(pTemp, "Process token open Error: %u\n", GetLastError());
			//	WriteLog(pTemp);
		}

		if (hPToken == NULL)
		{
			//	WriteLog("Process tokenError: \n");
		}
	}
	else
	{
		hPToken = hUserToken;
	}

	if (GetTokenInformation(hPToken, TokenLinkedToken, &TokenInformation, 4, &RetLen))
	{
		hUserTokenDup = TokenInformation;
	}
	else
	{
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			char pTemp[121];
			sprintf(pTemp, "Lookup Privilege value Error: %u\n", GetLastError());
			//	WriteLog(pTemp);
		}

		if (!DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup))
		{
			char pTemp[121];
			sprintf(pTemp, "DuplicateTokenEx Error: %u\n", GetLastError());
			//	WriteLog(pTemp);
		}
	}

	LPVOID pEnv = NULL;

	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
	{
		//	WriteLog("CreateEnvironmentBlock Failed\n");
		pEnv = NULL;
	}

	// Launch the process in the client's logon session.  

	bResult = CreateProcessAsUser(
		hUserTokenDup,            // client's access token  
		NULL,       // file to execute  
		(LPSTR)lpCmdLine.c_str(),         // command line       
		NULL,              // pointer to process SECURITY_ATTRIBUTES  
		NULL,              // pointer to thread SECURITY_ATTRIBUTES  
		FALSE,             // handles are not inheritable  
		dwCreationFlags,   // creation flags  
		pEnv,              // pointer to new environment block  
		NULL,              // name of current directory  
		&si,               // pointer to STARTUPINFO structure  
		&pi                // receives information about new process  
	);
	// End impersonation of client.  

	//GetLastError Shud be 0  

	int iResultOfCreateProcessAsUser = GetLastError();

	if (bResult == FALSE && iResultOfCreateProcessAsUser != 0)
	{
		char pTemp[121];
		sprintf(pTemp, "CreateProcessAsUser Error: %u\n", GetLastError());
		//	WriteLog(pTemp);
	}

	if (pi.hProcess)
	{
		CloseHandle(pi.hProcess);
	}
	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
	}

	//Perform All the Close Handles task  

	if (hProcess)
	{
		CloseHandle(hProcess);
	}
	if (hUserToken)
	{
		CloseHandle(hUserToken);
	}
	if (hUserTokenDup)
	{
		CloseHandle(hUserTokenDup);
	}
	if (hPToken)
	{
		CloseHandle(hPToken);
	}
	if (pEnv)
	{
		DestroyEnvironmentBlock(pEnv);
	}

	return bResult;
}

//********************************************************

std::string CMyService::GetModulePath()
{
	char szPathTemp[512];
	GetModuleFileName(NULL, szPathTemp, 512);
	//取出文件路径   
	for (int i = strlen(szPathTemp); i >= 0; i--)
	{
		if (szPathTemp[i] == '\\')
		{
			szPathTemp[i] = '\0';
			break;
		}
	}
	return std::string(szPathTemp);
}

//*********************************************************  
//Functiopn:            Init  
//Description:          初始化  
//Calls:                main  
//*********************************************************  
void CMyService::Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}

//*********************************************************  
//Functiopn:            IsInstalled  
//Description:          判断服务是否已经被安装 
//*********************************************************  
bool CMyService::IsInstalled()
{
	bool bResult = FALSE;

	//打开服务控制管理器  
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务  
		SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

//*********************************************************  
//Functiopn:            Install  
//Description:          安装服务函数
//********************************************************* 
bool CMyService::OnInstall()
{
	if (IsInstalled())
		return true;

	//打开服务控制管理器  
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, "Couldn't open service manager", m_szServiceName, MB_OK);
		return false;
	}

	// Get the executable file path  
	char szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务  
	SC_HANDLE hService = ::CreateService(hSCM, m_szServiceName, m_szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
		return false;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return true;
}

//*********************************************************  
//Functiopn:            Uninstall  
//Description:          删除服务函数 
//********************************************************* 
bool CMyService::OnUninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务  
	bool bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return true;

	std::cout << "Service could not be deleted";
	LogEvent("Service could not be deleted");
	return false;
}


//*********************************************************  
//Functiopn:            OnStop  
//Description:          停止服务 
//*********************************************************
bool CMyService::OnStop()
{
	if (!IsInstalled())
		return true;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
		return false;
	}

	SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
		return false;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);//停止

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	return true;
}


//*********************************************************  
//Functiopn:            LogEvent  
//Description:          记录服务事件
//*********************************************************
void CMyService::LogEvent(LPCTSTR pFormat, ...)
{
	char    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf_s(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, m_szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

//*********************************************************  
//Functiopn:            ServiceMain  
//Description:          服务主函数，这在里进行控制对服务控制的注册
//********************************************************* 
void WINAPI CMyService::ServiceMain()
{
	// Register the control request handler  
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	//注册服务控制  
	hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, &CMyService::ServiceStrl);
	if (hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	//模拟服务的运行。应用时将主要任务放于此即可  
	//可在此写上服务需要执行的代码，一般为死循环 
	_callback_start();
	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	OutputDebugString(_T("Service stopped"));
}

void CMyService::OnDealCmdLine(std::string & strCmd)
{
	std::transform(strCmd.begin(), strCmd.end(), strCmd.begin(), tolower);  //字母转小写

	Init();

	dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ m_szServiceName, (LPSERVICE_MAIN_FUNCTION)CMyService::ServiceMain },
		{ NULL, NULL }
	};

	if (strCmd.compare("install") == 0)
	{
		OnInstall();
	}
	else if (strCmd.compare("uninstall") == 0)
	{
		OnUninstall();
	}
	else if (strCmd.compare("remove") == 0)
	{
		OnUninstall();
	}
	else if (strCmd.compare("stop") == 0)
	{
		OnStop();
	}
	else if (strCmd.compare("start") == 0)
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
			//DWORD dwError = GetLastError();
			LogEvent(_T("Register Service Main Function Error!"));
		}
	}
	else
	{
		LogEvent(_T("no command can be do!"));
	}
}

//*********************************************************  
//Functiopn:            ServiceStrl  
//Description:          服务控制主函数，这里实现对服务的控制，  
//                      当在服务管理器上停止或其它操作时，将会运行此处代码  
//Calls:  
//Called By:  
//Table Accessed:  
//Table Updated:  
//Input:                dwOpcode：控制服务的状态  
//Output:  
//Return:  
//Others:  
//History:
//*********************************************************  
void WINAPI CMyService::ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
	{
		status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		_callback_Stop();
	}break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		break;
	default:
		LogEvent(_T("Bad service request"));
		OutputDebugString(_T("Bad service request"));
	}
}


void CMyService::OnInit(std::string szServiceName, CallBack callBack, CallBack callBackStop)
{
	memcpy(m_szServiceName, szServiceName.c_str(), szServiceName.size());
	_callback_start = callBack;
	_callback_Stop = callBackStop;
}

