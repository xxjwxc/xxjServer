/********************************************************************
    创建时间：    2016/11/01  17:49:58 
    文件名：      consoleHandler.h
    作者：        谢小军
    
    功能：        1.控制台退出操作
                2.
                
				说明：
				CTRL_C_EVENT //Ctrl+C
				CTRL_BREAK_EVENT //Ctrl+Break
				CTRL_CLOSE_EVENT: // Closing the consolewindow
				CTRL_LOGOFF_EVENT:
				CTRL_SHUTDOWN_EVENT: // System isshutting down. Passed only to services!
                2. 

example:
	static bool s_bTerminal = false;
	void OnTerminalCallBack()
	{
		s_bTerminal = true;
	}
	
	MyConsoleHandler::OnStart(OnTerminalCallBack);
	
	while (!s_bTerminal)
	{
	 	sleep(100);
	 }

*********************************************************************/
#ifndef __CONSOLE_HANDLER_H_
#define __CONSOLE_HANDLER_H_

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <signal.h>
#endif
#include <boost/function.hpp>

namespace MyConsoleHandler
{

	std::function<void()> funCall = nullptr;

#ifdef _WIN32
	BOOL WINAPI console_ctrl_handler(DWORD dwCtrlType)
	{
		switch (dwCtrlType)
		{
		case CTRL_C_EVENT: // Ctrl+C
		case CTRL_BREAK_EVENT: // Ctrl+Break
		case CTRL_CLOSE_EVENT: // Closing the consolewindow
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT: // System isshutting down. Passed only to services!
			if (funCall)
				funCall();
			return TRUE;
		}
		return FALSE;
	}
#else
	void kill_signal_handler(int iSignal)
	{
		switch (iSignal)
		{
		case SIGINT:
		case SIGQUIT:
		case SIGTERM: // System isshutting down. Passed only to services!
		{
			if (funCall)
				funCall();
			std::cout<<"Recieve kill signal!"<<std::endl;
		}break;
		default:
			break;
		}
	}
#endif // _WIN32
	void OnStart(std::function<void()> func)
	{
		MyConsoleHandler::funCall = func;
#ifdef _WIN32
		SetConsoleCtrlHandler(MyConsoleHandler::console_ctrl_handler, TRUE);
#else
		signal(SIGTERM, MyConsoleHandler::kill_signal_handler);
		signal(SIGINT, MyConsoleHandler::kill_signal_handler);
		signal(SIGQUIT, MyConsoleHandler::kill_signal_handler);
		signal(SIGPIPE, SIG_IGN);
#endif
	}
}


#endif