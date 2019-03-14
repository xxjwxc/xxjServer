/********************************************************************
创建时间：    2016/03/24  20:48:44
文件名：      IP17MON.h
作者：        谢小军

功能：        1.主要用于解析ip地址
			 2.

说明：        1.数据下载地址：http://www.ipip.net/download.html
2.

*********************************************************************/
#ifndef __IP17MON_H_
#define __IP17MON_H_
#include <string>
#include <vector>
typedef struct tapIpInfo{
	std::string Country;//国家
	std::string Region;//地区 
	std::string City; //城市
	std::string SrcIp;  //ip信息
}IpInfo;

class CMyIpMon
{
public:
	CMyIpMon();
	~CMyIpMon();

private:
public:
	bool init(const std::string strPathToDataFile = "./17monipdb.dat");
	IpInfo find(const std::string & strIP);
private:
	std::vector<char> vecDataFile;
	char *pIPIndex, *pIPData;
};

#endif // !__IP17MON_H_
