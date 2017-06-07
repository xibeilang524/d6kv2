/**
@file event_filter.h
@brief 事项过滤器头文件

@author lujiashun
@version 1.2.0
@date 2016-9-22
*/
#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H

#include <QList>
#include <QStringList>
#include <QMutex>

#include "net/netapi.h"
#include "event_para.h"

//事项确认状态 0 事项已确认，1	事项未确认 
enum emEventStatus {
	eConfirmed,
	eUnConfirmed
};

struct QSYSTime
{
	QSYSTime() {
		ymd = 0;
		hmsms = 0;
	}
	QSYSTime(int32u _ymd, int32u _hmsms) {
		ymd = _ymd;
		hmsms = _hmsms;
	}
	QSYSTime(const QSYSTime & t) {
		ymd = t.ymd; 
		hmsms = t.hmsms;
	}
	int32u ymd, hmsms;
};

bool operator>(QSYSTime & t1, QSYSTime & t2);
bool operator<(QSYSTime & t1, QSYSTime & t2);
bool operator==(QSYSTime & t1, QSYSTime & t2);
bool operator>=(QSYSTime & t1, QSYSTime & t2);
bool operator<=(QSYSTime & t1, QSYSTime & t2);

/**
@brief realalarm 的系统事项类，继承自SYS_EVENT
*/
class Rsys_event : public SYS_EVENT
{
public:
	Rsys_event() 
	{ 
		m_bneed_confirm = false; 
		m_event_status = eUnConfirmed;
		m_match = NULL;
	}
	Rsys_event(SYS_EVENT * e ) 
	{ 
		*((SYS_EVENT*)this) = *e;
// 		grpno = e->grpno;
// 		sort = e->sort;
// 		type = e->type;
// 		ymd = e->ymd;
// 		hmsms = e->hmsms;
// 		security = e->security;
// 		volgrade = e->volgrade;
// 		testflag = e->testflag;
// 		PrintOut = e->PrintOut;
// 		DiskOut = e->DiskOut;
// 		ToneOut = e->ToneOut;
// 		GraphOut = e->GraphOut;
// 		DispOut = e->DispOut;
// 		state = e->state;
// 		state1 = e->state1;
// 		state2 = e->state2;
// 		val = e->val;
// 		val1 = e->val1;
// 		val2 = e->val2;
// 		val3 = e->val3;
// 		strcpy( Object , e->Object );
// 		strcpy( member0 , e->member0 );
// 		strcpy( member1 , e->member1 );
// 		strcpy( member2 , e->member2 );
// 		strcpy( member3 , e->member3 );
// 		strcpy( graph , e->graph );
// 		strcpy( tone , e->tone );
// 		strcpy( text, e->text );
		m_bneed_confirm = false; 
		m_event_status = eUnConfirmed;
		m_match = NULL;
	}

	bool need_match();
	bool Is_one_pare(Rsys_event & e);

	bool m_bneed_confirm;	// 需要确认标志
	emEventStatus m_event_status;

	Rsys_event * m_match ;	// 保护信号和SOE成组出现
	static bool g_cfg_need_match;
};

class Sys_event_list : public QList<Rsys_event*>
{
public:
	void  appendAndMatch( Rsys_event *d );
};

Sys_event_list collect_unconfirmed(Sys_event_list & list) ;
Sys_event_list collect_confirmed(Sys_event_list & list) ;
int count_removable_events(Sys_event_list & list);
int count_unconfirmed(Sys_event_list & list);
int count_confirmed(Sys_event_list & list);

//事项过滤器
class Event_filter
{
public:
	Event_filter();
	virtual ~Event_filter();

	void add_event_type( PEVENTTYPEDEF pEventTypeDef );
	uint get_event_type_num();
	bool get_event_type( uint index, EVENTTYPEDEF &eventtype );

	void add_rtu( const QString &rtucode );
	uint get_rtu_num();
	QString get_rtu_code( uint index );

	void add_event_level( uint level );
	uint get_event_level_num();
	uint get_event_level( uint index );

	void reset_filter();

	virtual bool is_select( Rsys_event * eve );
	virtual bool is_select( PEVENTTYPEDEF eve );
	bool is_select( QString &rtucode );
	bool is_select( uint level );
	virtual void add_event( Rsys_event * eve );
	virtual void del_event( Rsys_event * eve );

	//事项列表
	Sys_event_list		m_event_list;
	
protected:

	QMutex					m_mutex;

private:

	//不处理厂站列表，如果该事项所属厂站在列表中则不处理，否则处理
	QStringList				m_rtu_list;

	//要处理事项类型列表，如果该事项的类别、类型、级别能在列表中能找到
	//相对应的EVENTDEF项，能处理该事项，否则不处理
	QList<EVENTTYPEDEF*>	m_event_type_list;

	QList<uint>		m_event_level_list;
};

#endif