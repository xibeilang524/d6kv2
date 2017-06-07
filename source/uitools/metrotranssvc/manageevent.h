///负责接收到的事项缓冲区的管理
///
///		
////////////////////////////////////////////////////////////
#ifndef MANAGEDAEVT_H
#define MANAGEDAEVT_H

#include "rdb/rdbapi.h"
#include "globaldef.h"
#include "fileinfo.h"

class Mng_event
{
public:
	Mng_event();
	~Mng_event();

	//	void GetEvtNum(int& num);
	void move_buf(EX_SYS_EVENT** pEvt , int* EvtNum);
	int read_event(File_info fileInfo);
private:
	int add_event(EX_SYS_EVENT& evt);
	void release_buf();
private:
	int m_bUsing;
	EX_SYS_EVENT * m_pEvt;
	int m_iBufSize;
	int m_iEvtNum;
	EventProc m_eventproc;
public:
	int m_bNeedClearBuf;
	void broadcast_event(SYS_EVENT* pEvt,int num);
	SYS_TIME m_timeFirstFault;//多点同时故障时，收到第一个故障的时间
	SYS_TIME m_timeLastFault;//多点同时故障时，收到最后一个故障的时间
};
#endif
