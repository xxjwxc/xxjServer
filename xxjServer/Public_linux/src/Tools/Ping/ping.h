#ifndef _PING_H
#define _PING_H

#define DEF_PACKET_SIZE 32

#ifdef WIN32
#include <windows.h>
#include <WinSock2.h>

//这里需要导入库 Ws2_32.lib，在不同的IDE下可能不太一样 
//#pragma comment(lib, "Ws2_32.lib")

#define ECHO_REQUEST 8
#define ECHO_REPLY 0

struct IPHeader
{
	BYTE m_byVerHLen; //4位版本+4位首部长度
	BYTE m_byTOS; //服务类型
	USHORT m_usTotalLen; //总长度
	USHORT m_usID; //标识
	USHORT m_usFlagFragOffset; //3位标志+13位片偏移
	BYTE m_byTTL; //TTL
	BYTE m_byProtocol; //协议
	USHORT m_usHChecksum; //首部检验和
	ULONG m_ulSrcIP; //源IP地址
	ULONG m_ulDestIP; //目的IP地址
};

struct ICMPHeader
{
	BYTE m_byType; //类型
	BYTE m_byCode; //代码
	USHORT m_usChecksum; //检验和 
	USHORT m_usID; //标识符
	USHORT m_usSeq; //序号
	ULONG m_ulTimeStamp; //时间戳（非标准ICMP头部）
};

struct PingReply
{
	USHORT m_usSeq;
	DWORD m_dwRoundTripTime;
	DWORD m_dwBytes;
	DWORD m_dwTTL;
};

class CPing
{
public:
	CPing();
	~CPing();
	BOOL Ping(DWORD dwDestIP, PingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
	BOOL Ping(const char *szDestIP, PingReply *pPingReply = NULL, DWORD dwTimeout = 2000);
private:
	BOOL PingCore(DWORD dwDestIP, PingReply *pPingReply, DWORD dwTimeout);
	USHORT CalCheckSum(USHORT *pBuffer, int nSize);
	ULONG GetTickCountCalibrate();
private:
	SOCKET m_sockRaw;
	WSAEVENT m_event;
	USHORT m_usCurrentProcID;
	char *m_szICMPData;
	BOOL m_bIsInitSucc;
private:
	static USHORT s_usPacketSeq;
};

/*
main
std::vector<std::string> ver;
CPing objPing;
std::string szDestIP = "127.0.0.1";
std::string str;
PingReply reply;

//printf("Pinging %s with %d bytes of data:\n", szDestIP, DEF_PACKET_SIZE);
str = (boost::format("Pinging %s with %d bytes of data:") % szDestIP % DEF_PACKET_SIZE).str();
ver.push_back(str);
for(size_t i=0;i<5;i++)
{
ZeroVar(reply);
objPing.Ping(szDestIP.c_str(), &reply);
str = (boost::format("Reply from %s: bytes=%ld time=%ldms TTL=%ld") % szDestIP % reply.m_dwBytes%reply.m_dwRoundTripTime%reply.m_dwTTL).str();
ver.push_back(str);
Sleep(500);
}

std::string pFile = "log/ping_" + CMyTimes::GetDayStr(CMyTimes::get_utc()) + ".log";
MyTools::PrintToFile(pFile, ver, false);
*/
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>

#include <string>
#include <vector>
#define PACKET_SIZE     4096
#define MAX_WAIT_TIME   5
#define MAX_NO_PACKETS  3

struct IcmpEchoReply {
	int icmpSeq;
	int icmpLen;
	int ipTtl;
	double rtt;
	std::string fromAddr;
	bool isReply;
};
struct PingResult {
	int dataLen;
	int nsend;
	int nreceived;
	std::string ip;
	std::string error;
	std::vector<IcmpEchoReply> icmpEchoReplys;
};

class CPing {
public:
	CPing();
	bool Ping(std::string host, PingResult &pingResult);
	bool Ping(std::string host, int count, PingResult& pingResult);
private:
	unsigned short getChksum(unsigned short *addr, int len);
	int packIcmp(int pack_no, struct icmp* icmp);
	bool unpackIcmp(char *buf, int len, struct IcmpEchoReply *icmpEchoReply);
	struct timeval tvSub(struct timeval timeval1, struct timeval timval2);
	bool getsockaddr(const char * hostOrIp, sockaddr_in* sockaddr);
	bool sendPacket();
	bool recvPacket(PingResult &pingResult);
private:
	char m_sendpacket[PACKET_SIZE];
	char m_recvpacket[PACKET_SIZE];
	int m_maxPacketSize;
	int m_sockfd;
	int m_datalen;
	int m_nsend;
	int m_nreceived;
	int m_icmp_seq;
	struct sockaddr_in m_dest_addr;
	struct sockaddr_in m_from_addr;
	pid_t m_pid;
};
#endif

#endif