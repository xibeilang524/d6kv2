#include "dqinfo_dbgpara.h"

CDbgPara::CDbgPara()
{
	m_remoteFlag = false;
	m_dbgID = -1;
	m_remoteStopFlag = false;
}

void CDbgPara::setDbgID(int ID)
{
	if (!m_remoteFlag)
	{
		m_dbgID = ID;
		m_netDbg.DbgProc(m_dbgID);
		cout<<"set local dbg to "<<ID<<endl;
	}
	else
	{
		REMOTE_DBG_SEL sel;
		//char typedes[DEBUG_MAX_TYPE][DESC_LEN];
		sel.proc_idx = ID;
		
		//int typeNum = m_netDbg.GetTypeEnm(0, typedes);	//偷个懒，把本地nbsvc的类型发出
		sel.typenum = m_remoteDbg.proc[ID].typenum;
		sel.type = new int16s[(int)sel.typenum];
		
		for (int i = 0; i < sel.typenum; i++)
		{
			sel.type[i] = m_remoteDbg.proc[ID].type[ i ].type;
		}
		
		m_dbgID = ID;
		//m_remoteLock.lock();
		//m_remoteDbgClient.Close();
		m_remoteDbgClient.SetDbgInfo(&sel);	
		//m_remoteLock.unlock();
		delete[] sel.type;
	}
}

int CDbgPara::setRemote(bool flag, int hostNo, char* ipAddr)
{
	int ret = FALSE;
	if (flag)
	{
		m_remoteFlag = true;
		
		if (hostNo != -1)
		{
			//m_remoteLock.lock();
			ret = m_remoteDbgClient.GetProcInfo(hostNo, &m_remoteDbg);
			//m_remoteLock.unlock();
		}
		else if ((-1 == hostNo) && (inet_addr(ipAddr) != INADDR_NONE))
		{
			//m_remoteLock.lock();
			ret = m_remoteDbgClient.GetProcInfo(ipAddr, &m_remoteDbg);
			m_remoteHost = -1;
			//m_remoteLock.unlock();
		}
		else
		{
			m_remoteFlag = false;
			m_remoteHost = -1;
		}
	}//if (flag)
	else
	{
		m_remoteFlag = false;
		m_remoteHost = -1;
		//m_remoteLock.lock();
		//m_remoteDbgClient.Close();
		m_remoteStopFlag = true;
		//m_remoteLock.unlock();
	}
	
	return ret;
}


int CDbgPara::getRenmoteTask(DbgProcInfo* &procInfo)
{
	procInfo = m_remoteDbg.proc;
	return m_remoteDbg.proc_num;
}

int CDbgPara::getDbg(int* typeNo, char* type, char* desc)
{
	int ret;

	if (-1 == m_dbgID)
	{
		return FALSE;
	}
	
	if (!m_remoteFlag)
	{
		if (m_netDbg.GetDbg(typeNo, type, desc) == FALSE)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
		//return m_netDbg.GetDbg(type, desc);
	}
	else
	{
		ret = m_remoteDbgClient.GetDbg(type, desc);

		if (m_remoteStopFlag)
		{
			m_remoteDbgClient.Close();
			m_remoteStopFlag = false;
		}
		return ret;
	}
	
}