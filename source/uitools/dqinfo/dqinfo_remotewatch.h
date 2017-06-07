#ifndef MASINFO_REMOTEWATCH_H_
#define MASINFO_REMOTEWATCH_H_

#include "masinfo_dbgclnt.h"
#include "net/netapi.h"

class CRemoteDbg
{
public:
	int setRemote(bool flag, int hostNo, char* ipAddr);
	void setRemoteID(int ID);
	int getDbg(char* type, char* desc);
	int getRenmoteTask(DbgProcInfo* &procInfo);
	bool remoteFlag(void) {return m_remoteFlag;};

private:
	bool			 m_remoteFlag;
	REMOTE_DBG_PROC  m_remoteDbg;
	CDbgClnt		 m_remoteDbgClient;	
	REMOTE_DBG_SEL	 m_remoteSel;
	int				 m_remoteHost;
};
#endif