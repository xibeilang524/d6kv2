//信号控制类
//改类主要解决不可控开关的隔离和恢复操作过程中，提供给上一级系统的要求进行遥控的信号
//控制方式
///////////////////////////////////////////////////
#ifndef _SIGNALCTL_H_
#define _SIGNALCTL_H_
#include "rdbapi.h"
#include "ChainTab.h"

class CSignalCtl
{
	struct EX_SIG
	{
		SYS_TIME starttime;
		int lastingsecs;
		char yxcode[CODE_LEN];
	};
	CSignalCtl();
	~CSignalCtl();
public:
	static int StartThread();
	static int Add(char* yxcode,int lastsecs);
	static void EndThread();
private:
	static int SetYx(char* yxcode,char val = 1);

#ifdef WIN32
	static unsigned int ThreadFunc(void*);
#else
    static void  * ThreadFunc(void*);
#endif

private:
	static CChainTab<EX_SIG> m_sigcluster;
#ifdef WIN32
	static CWinThread* m_thread;
#else
	static pthread_t m_thread;
#endif
	static int m_iQuit;
};

#endif

