#ifndef _SERVICE_H_
#define _SERVICE_H_

#define  MaxServiceNAME 64
#include <string>
#include <windows.h>
#include <winsvc.h>
#include <functional>

#ifndef _T
#define _T
#endif // !_T



class CMyService
{
public:
	typedef std::function<void()> CallBack;
public:
	CMyService(){};
	~CMyService(){};
public:
	static bool CmdLineExecuteDifferentSession(std::string lpCmdLine);
	static std::string GetModulePath();
public:
	void OnInit(std::string szServiceName, CallBack callBack, CallBack callBackStop);

	//处理cmd请求
	void OnDealCmdLine(std::string & strCmd);

	//定义全局函数变量  
	void Init();
	bool IsInstalled();
	bool OnInstall();//安装
	bool OnUninstall();//卸载
	bool OnStop();
	static void LogEvent(LPCTSTR pFormat, ...);
	static void WINAPI ServiceMain();
	static void WINAPI ServiceStrl(DWORD dwOpcode);

	static char m_szServiceName[MaxServiceNAME];
	bool bInstall;
	static SERVICE_STATUS_HANDLE hServiceStatus;
	static SERVICE_STATUS status;
	static unsigned long dwThreadID;
	static CallBack _callback_start;
	static CallBack _callback_Stop;
};
#endif