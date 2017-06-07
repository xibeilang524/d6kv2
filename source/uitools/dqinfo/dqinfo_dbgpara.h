#ifndef MASINFO_DBG_PARA_H_
#define MASINFO_DBG_PARA_H_

#include <QMutex>
#include "dqinfo_dbgclnt.h"
#include "net/netapi.h"
#include <iostream>
using namespace std;

class CDbgPara
{
public:
	CDbgPara();
	int getDbg(int* typeNo, char* type, char* desc);
	int setRemote(bool flag, int hostNo, char* ipAddr);
	int getRenmoteTask(DbgProcInfo* &procInfo);
	void setDbgID(int ID);
	bool isRemote(void){return m_remoteFlag;};

public:
	bool			 m_remoteFlag;
	NetDbg			 m_netDbg;
	REMOTE_DBG_PROC  m_remoteDbg;
	CDbgClnt		 m_remoteDbgClient;	
	REMOTE_DBG_SEL	 m_remoteSel;
	int				 m_remoteHost;
	int				 m_dbgID;
	QMutex			 m_remoteLock;
	bool			 m_remoteStopFlag;	//此标志用于在远程切换回本地状态时，让远程读取最后一次信息后关闭，防止直接关闭会卡很长时间
};
#endif