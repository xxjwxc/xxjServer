#include "IP17MON.h"
#include <fstream>
#include <boost/locale.hpp>
#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>  
#include <arpa/inet.h>
#endif

CMyIpMon::CMyIpMon()
{

}

CMyIpMon::~CMyIpMon()
{

}

bool CMyIpMon::init(const std::string strPathToDataFile /*= "./17monipdb.dat"*/)
{
	std::ifstream ifDataFile(strPathToDataFile.c_str(), std::ios::binary);
	if (ifDataFile.is_open() == false) {
		return false;
	}
	vecDataFile.assign(std::istreambuf_iterator<char>(ifDataFile), std::istreambuf_iterator<char>());

	unsigned int uiIndexLen = 0;
	memcpy(&uiIndexLen, &vecDataFile[0], 4);
	uiIndexLen = ntohl(uiIndexLen);
	pIPIndex = &vecDataFile[4];
	pIPData = &vecDataFile[uiIndexLen];

	return true;
}

IpInfo CMyIpMon::find(const std::string & strIP)
{
	IpInfo info;
	info.SrcIp = strIP;

	if (strIP.empty()) {
		return info;
	}

	struct sockaddr_in stSockAddrInet = { 0 };
	stSockAddrInet.sin_addr.s_addr = inet_addr(strIP.c_str());
	
	if (stSockAddrInet.sin_addr.s_addr == INADDR_NONE) {
		return info;
	}
	info.SrcIp = inet_ntoa(stSockAddrInet.sin_addr);

	unsigned int uiIP = ntohl(stSockAddrInet.sin_addr.s_addr);

	int iFirst = atoi(strIP.c_str());

	int iStart = 0;
	memcpy(&iStart, pIPIndex + (iFirst * 4), 4);

	int iMaxComLen = pIPData - pIPIndex - 1024 - 4;

	int iIndexOffset = -1;
	unsigned char ucIndexLength = 0;
	for (iStart = iStart * 8 + 1024; iStart < iMaxComLen; iStart += 8) {
		unsigned int uiCurrIP = 0;
		memcpy(&uiCurrIP, pIPIndex + iStart, 4);
		uiCurrIP = ntohl(uiCurrIP);
		if (uiCurrIP >= uiIP) {
			iIndexOffset = 0;
			memcpy(&iIndexOffset, pIPIndex + iStart + 4, 3);
			memcpy(&ucIndexLength, pIPIndex + iStart + 7, 1);
			break;
		}
	}

	if (iIndexOffset == -1) {
		return info;
	}

	std::string strRegion;
	char *pRegion = pIPData + iIndexOffset - 1024;
	strRegion.assign(pRegion, ucIndexLength);
	strRegion = boost::locale::conv::between(strRegion, "GBK", "UTF-8");
	unsigned int loc = strRegion.find("\t", 0);
	if (loc != std::string::npos)
	{
		info.Country = strRegion.substr(0, loc);
		strRegion = strRegion.substr(loc+1);
		loc = strRegion.find("\t", 0);
		if (loc != std::string::npos)
		{
			info.Region = strRegion.substr(0, loc);
			strRegion = strRegion.substr(loc + 1);
		}

		loc = strRegion.find("\t", 0);
		if (loc != std::string::npos)
		{
			info.City = strRegion.substr(0, loc);
		}
	}

	return info;
}

