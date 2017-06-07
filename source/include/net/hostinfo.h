#ifndef HOSTINFO_H_
#define HOSTINFO_H_

#ifdef WIN32
#pragma warning(disable : 4786)  
#endif 

#include "sysdef.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

#define OIDLEN 128
#define DISKTYPELEN 128
#define DISKINFOLEN 128
#define MAXOIDNUM	1024
#define MAXVALUENUM 1024
#define MAXSTRINGLEN 1024
#define BULK_MAX 10

enum HOSTTYPE{WINDOWS, AIX, HPUX, SOLARIS, LINUX, REDHAT, UBUNTU, CISCO, H3C, HUAWEI, OTHER};
enum VALUETYPE{GETINT, GETSTRING};

const int SUCCESS = 1;
const int FAIL = 0;
const int MAXCPUNUM = 16;
const int MAXDISKNUM = 16;
const int MAXNETNUM = 64;
const int ADDRESSLEN = 128;

#if defined(WIN32)
#if defined(HOSTINFO_EXPORTDLL)
#define	HOSTINFO_IMEXPORT				__declspec(dllexport) 
#else
#define	HOSTINFO_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	HOSTINFO_IMEXPORT
#endif


class HOSTINFO_IMEXPORT CHostInfo
{
public:
	CHostInfo();
	~CHostInfo();
public:
	int getHostType(char* ip, HOSTTYPE &hostType);
	int getHostName(char* ip, char* hostName);
	int getNetWorkFlow(char *ip, int &netNum, char **macAddr, char **ipAddr, char **mask, unsigned int *speed, unsigned int *outBytes, unsigned int *inBytes);

	int getCpuLoad(char *ip, int &cpuLoad);

	int getAverageCpuLoadForHpux(char* ip, int &oneMinuteLoad, int &fiveMinuteLoad, int &fifteenMinuteLoad);
	int getAverageCpuLoadForLinux(char* ip, int &oneMinuteLoad, int &fiveMinuteLoad, int &fifteenMinuteLoad);

	int getMemUsage(char* ip, int &memNum, int &memUsage, int &pagefileNum, int &pagefileUsage);
	int getDriveUsage(char *ip, int &diskNum, char**diskInfo, int *diskSize, int *usedDiskSize);
private:
	int getValue(char* ip, VALUETYPE valueType);
	int walkValue(char* ip, bool subTree, VALUETYPE valueType);

	int getCpuLoadForCisco(char *ip, int &cpuLoad);
	int getCpuLoadForWinLinuxAix(char *ip, int &cpuLoad);
	int getCpuLoadForHpux(char *ip, int &cpuLoad);
	int getCpuTimeForHpux(char *ip, unsigned int &userTime, unsigned int &sysTime, unsigned int &idleTime, unsigned int &niceTime);
	int getMemUsageForHpux(char* ip, int &memNum, int &memUsage, int &pagefileNum, int &pagefileUsage);
	int getMemUsageForWinLinuxAix(char* ip, int &memNum, int &memUsage, int &pagefileNum, int &pagefileUsage);
	int getMemUsageForCisco(char* ip, int &memNum, int &memUsage, int &pagefileNum, int &pagefileUsage);

	int getDriveUsageForWinLinuxAix(char* ip, int &diskNum,  char **diskInfo, int *diskSize, int *usedDiskSize);
	int getDriveUsageForHpux(char* ip, int &diskNum,  char **diskInfo, int *diskSize, int *usedDiskSize);

private:	
	vector<string>			m_oidList; 
	vector<unsigned int>	m_intValueList;
	vector<string>			m_stringValueList;
	vector<string>			m_walkOidList;
};
#endif