#include "masinfo_remotewatch.h"
#include "masinfo_string.h"


int CRemoteDbg::setRemote(bool flag, int hostNo, char* ipAddr)
{
	int ret = FALSE;
	if (flag)
	{
		m_remoteFlag = true;

		if (hostNo != -1)
		{
			ret = m_remoteDbgClient.GetProcInfo(hostNo, &m_remoteDbg);
		}
		else if ((-1 == hostNo) && (inet_addr(ipAddr) != INADDR_NONE))
		{
			ret = m_remoteDbgClient.GetProcInfo(ipAddr, &m_remoteDbg);
			m_remoteHost = -1;
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
		m_remoteDbgClient.Close();
	}

	return ret;
}


int CRemoteDbg::getRenmoteTask(DbgProcInfo* &procInfo)
{
	procInfo = m_remoteDbg.proc;
	return m_remoteDbg.proc_num;
}

void CRemoteDbg::setRemoteID(int ID)
{
	NetDbg dbg;
	REMOTE_DBG_SEL sel;
	char typedes[DEBUG_MAX_TYPE][DESC_LEN];
	sel.proc_idx = ID;

	int typeNum = dbg.GetTypeEnm(0, typedes);	//偷个懒，把本地nbsvc的类型发出
	int* index = dbg.GetTypeIndex(); 
	sel.typenum = typeNum;
	sel.type = new int16s(sel.typenum);

	for (int i = 0; i < typeNum; i++)
	{
		sel.type[i] = index[i];
	}

	m_remoteDbgClient.Close();
	m_remoteDbgClient.SetDbgInfo(&sel);	
}


int CRemoteDbg::getDbg(char* type, char* desc)
{
	return m_remoteDbgClient.GetDbg(type, desc);
}
