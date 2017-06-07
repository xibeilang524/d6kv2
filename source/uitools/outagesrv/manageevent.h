///manageevent.h
///读取实时事项
//////////////////////////////////////////////////////////////////////////////////////	

#ifndef _MANAGEDAEVT_H_
#define _MANAGEDAEVT_H_

#include "rdb/rdbapi.h"
#include "globaldef.h"

#include <QList>

enum{
	FAULTPOWERCUT = 0	//事故停电
};
typedef struct
{
	char devName[CODE_LEN];	//停电设备
	int	 type;				//停电类型
	int	 starttime;			//停电开始时间
	int	 cutType;			//设备失电原因
} CUTEVENT;

class CManageEvent
{
public:
	CManageEvent();
	~CManageEvent();

	int readEvent();						//读取实时事项		
	int getEvent( CUTEVENT & event );		//读取一个停电事项
	int addEvent( CUTEVENT & event );		//添加停电事项
private:
	int m_bUsing;		//读写标识
	CUTEVENT m_cutEvents[ POWERCUTEVENTNUM ]; //停电事项列表
	unsigned int m_curEventRP;				  //停电事项读指针
	unsigned int m_curEventWP;				  //停电事项写指针
	EventProc m_eventproc;
};
#endif
