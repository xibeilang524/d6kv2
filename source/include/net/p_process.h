#ifndef __PROCESS_INCLUDE__
#define __PROCESS_INCLUDE__

#if defined(WIN32)
#if defined(PROC_EXPORTDLL)
#define	PROC_IMEXPORT				__declspec(dllexport) 
#else
#define	PROC_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	PROC_IMEXPORT
#endif


#ifdef WIN32
#include <TLHELP32.H>
#pragma warning(disable: 4786)
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __hpux
#include <sys/procfs.h>
#endif
#ifdef __hpux
#include <sys/param.h>
#include <sys/pstat.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/socket.h>

#include <dlfcn.h>

#ifdef _AIX
#include <fcntl.h>
#endif

#ifdef __sun
	#include <link.h>	
#endif
#undef FARPROC
typedef int (*FARPROC)();
#endif

#include "netapi.h"
#include <map>
using namespace std;

#if defined _AIX || defined __linux
typedef struct{
	uint	pid;
	char	cmd[ 256 ];
}AIXPROCINFO;
#endif

#ifdef __hpux
#define BURST 100
#endif

typedef struct
{
	double	 cpuLoad;		//CPU使用率
	int		 memUsage;		//内存使用量kb
#ifdef __linux
	unsigned long time;		//cpu占用时间
#elif _AIX
	double time;			//CPU占用时间
#endif
}PROCRES;

typedef map<int, PROCRES>::iterator procResIterator;

class  PROC_IMEXPORT CProcInfo
{
#ifdef WIN32
	static	PROCESSENTRY32 m_procinfo;
#elif defined _AIX || defined __linux
	static 	AIXPROCINFO 	m_procinfo;
#elif defined __hpux
	static 	pst_status 	m_procinfo;
#else
	static 	prpsinfo_t 	m_procinfo;
#endif

	static  PROCRES	m_procDetailInfo;
public:

	static  int	IsExist(char* filename);
	static  int	GetProcNum(char* filename);

#ifdef WIN32
	static  PROCESSENTRY32 *GetProcInfo(char* filename);
#elif defined _AIX || defined __linux
	static  AIXPROCINFO     *GetProcInfo(char* filename);
	static  AIXPROCINFO     *GetProcInfo(int pid);
#elif defined __hpux
	static  pst_status     *GetProcInfo(char* filename);
	static  pst_status     *GetProcInfo(int pid);
#else
	static  prpsinfo_t     *GetProcInfo(char* filename);
	static  prpsinfo_t     *GetProcInfo(int pid);
#endif

	static	int GetProcEnum(char* filename,int* pid);

private:
	map<int, PROCRES> m_procRes;
	
#if defined _AIX || defined __linux
	struct timeval lasttime;
	void readOneProcStat(int pid, PROCRES* res);
#endif


public:
#ifdef WIN32
	CProcInfo();
#endif
	
	void calcAllProcRes(void);
	PROCRES *getOneProcRes(int pid);
};

#endif
