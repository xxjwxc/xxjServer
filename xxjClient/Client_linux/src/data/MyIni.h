#pragma once
#include <string>
#include <vector>
#include <map>
#include <Tools/MySingleton.h>

class CMyIni:public MySingleton<CMyIni>
{
public:
	CMyIni(void);
	~CMyIni(void);
public:
	bool ReadText();
	bool ParseJson(const std::string & str);

	std::string m_serverIP;
	int m_serverPort;
	std::map<std::string,int> m_opMap;
	std::string m_title;
	std::string m_exestart;
};

//ÄÚ´æ¼ì²â ¼àÊÓ
#define MEMORY_CHICK \
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);\
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;\
	_CrtSetDbgFlag(tmpFlag);