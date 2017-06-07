#ifndef MASINFO_DBGCLNT_H_
#define MASINFO_DBGCLNT_H_


#include "net/remotedbg.h"

class CDbgClnt  
{
public:
	void Close();
	int GetDbg(char* type,char* desc);
	CDbgClnt();
	virtual ~CDbgClnt();

	int GetProcInfo(int host,REMOTE_DBG_PROC*  dbg_procinfo);
	int GetProcInfo(char* ip,REMOTE_DBG_PROC*  dbg_proc);
	int SetDbgInfo(REMOTE_DBG_SEL* sel);
private:
	int  m_host;
	Tcp  tcp;
	int m_port;
	int m_netno;
	char  dial_addr[40];
	char  dialf;
};

#endif 
