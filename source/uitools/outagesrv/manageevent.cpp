
#include "scd/devdef.h"

#include "manageevent.h"
#include "tpdata.h"
#include "streamdata.h"
#include "initialize.h"
#include "globaldef.h"

extern CInitialize g_init;
extern CStreamData g_stream;
extern CTpData g_topoData;
extern CManageEvent g_manageEvent;


CManageEvent::CManageEvent()
{
	m_bUsing = FALSE;
	m_curEventRP = 0;
	m_curEventWP = 0;
}

CManageEvent::~CManageEvent()
{
}

int CManageEvent::addEvent( CUTEVENT & event )
{
	while (m_bUsing == TRUE)
		;
	m_bUsing = TRUE;
	//添加停电事项记录
	strcpy( m_cutEvents[ m_curEventWP ].devName, event.devName );
	m_cutEvents[ m_curEventWP ].starttime = event.starttime;
	m_cutEvents[ m_curEventWP ].type = event.type;
	m_cutEvents[ m_curEventWP ].cutType = event.cutType;
	m_curEventWP = ( m_curEventWP + 1 ) % POWERCUTEVENTNUM;
	m_bUsing = FALSE;

	g_stream.saveStartData();
	return TRUE;
}

int CManageEvent::getEvent( CUTEVENT & event )
{
	if( m_curEventRP == m_curEventWP )
		return FALSE;

	while (m_bUsing == TRUE)
		;
	m_bUsing = TRUE;
	strcpy( event.devName, m_cutEvents[ m_curEventRP ].devName );
	event.starttime = m_cutEvents[ m_curEventRP ].starttime;
	event.type = m_cutEvents[ m_curEventRP ].type;
	event.cutType = m_cutEvents[ m_curEventRP ].cutType;
	m_curEventRP = ( m_curEventRP + 1 ) % POWERCUTEVENTNUM;
	m_bUsing = FALSE;

	return TRUE;
}

int CManageEvent::readEvent( )
{
	int flag;
	SYS_EVENT evt;
	for( ; ; )
	{
		flag = m_eventproc.Read(&evt);	//读取事项记录

		if(flag == 0) 
		{
			break;
		}
		int type = (int)evt.type ;
		if (type == ENTTYPE_POWERSYS_BREAKSG)
		{
#ifdef __PROGRAM_DEBUG_
			TRACE("收到事项——事故跳闸事项!\n");
#endif
			TimeProc timeProc;
			CUTEVENT event;
			struct tm time_create;
			time_create.tm_year = evt.ymd/10000-1900;
			time_create.tm_mon = (evt.ymd%10000)/100 - 1;
			time_create.tm_mday = evt.ymd%100;
			time_create.tm_hour = evt.hmsms/10000000;
			time_create.tm_min = (evt.hmsms%10000000)/100000;
			time_create.tm_sec = (evt.hmsms%100000)/1000;
			time_create.tm_isdst = 0;

			strcpy( event.devName, evt.member3 );
			event.starttime = mktime( &time_create );
			event.type = DEVTYPE_BREAKER;
			event.cutType = FAULT_POWERCUT;
			addEvent( event );
		}
		else if( evt.type == ENTTYPE_POWERSYS_BREAK)
		{
#ifdef __PROGRAM_DEBUG_
			TRACE("收到事项——正常变位事项!\n");
#endif
			TimeProc timeProc;
			CUTEVENT event;
			struct tm time_create;
			time_create.tm_year = evt.ymd/10000-1900;
			time_create.tm_mon = (evt.ymd%10000)/100 - 1;
			time_create.tm_mday = evt.ymd%100;
			time_create.tm_hour = evt.hmsms/10000000;
			time_create.tm_min = (evt.hmsms%10000000)/100000;
			time_create.tm_sec = (evt.hmsms%100000)/1000;
			time_create.tm_isdst = 0;

			strcpy( event.devName, evt.member3 );
			event.starttime = mktime( &time_create );
			event.type = DEVTYPE_BREAKER;
			event.cutType = NORMAL_POWERCUT;
			addEvent( event );
		}
	}
	return 0;
}