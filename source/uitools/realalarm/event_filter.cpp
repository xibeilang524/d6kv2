
#include "event_filter.h"
#include "globaldef.h"

bool operator>(QSYSTime & t1, QSYSTime & t2)
{
	if(t1.ymd == t2.ymd)
		return t1.hmsms > t2.hmsms;
	else
		return t1.ymd > t2.ymd;
}

bool operator<(QSYSTime & t1, QSYSTime & t2)
{
	if(t1.ymd == t2.ymd)
		return t1.hmsms < t2.hmsms;
	else
		return t1.ymd < t2.ymd;
}

bool operator==(QSYSTime & t1, QSYSTime & t2)
{
	return (t1.ymd == t2.ymd && t1.hmsms == t2.hmsms);
}

bool operator>=(QSYSTime & t1, QSYSTime & t2)
{
	return (t1 > t2 || t1 == t2);
}

bool operator<=(QSYSTime & t1, QSYSTime & t2)
{
	return (t1 < t2 || t1 == t2);
}

bool Rsys_event::g_cfg_need_match=false;

bool Rsys_event::need_match() {
	return ( g_cfg_need_match && sort == ENTCLASS_SCADA && 
		( type == ENTTYPE_POWERSYS_PROTSIG ||
		type == ENTTYPE_POWERSYS_PROTSIGSOE ) );
}

bool Rsys_event::Is_one_pare(Rsys_event & e)
{
	if(this == &e || !need_match() || !e.need_match())
		return false;

	if(m_match!=NULL || e.m_match!=NULL)
		return false;

	if(type == e.type && QObject::tr( member0 ) == QObject::tr( e.member0 ) )
	{
		QSYSTime time1( ymd, hmsms );
		QSYSTime time2( e.ymd, e.hmsms );
		if(state==1 && e.state==0 && time1 <= time2 )
			return true;
		else if(state==0 && e.state==1 && time1 >= time2 )
			return true;
	}
	return false;
}

void  Sys_event_list::appendAndMatch( Rsys_event *d )
{
	if( d && d->need_match() ) 
	{
		Rsys_event * pMatch = NULL;
		int nCount = count();
		for( int i = 0; i < nCount; i ++ ) 
		{
			Rsys_event * pEvent = at(i);
			if( d->Is_one_pare( *pEvent ) && pEvent->m_match == NULL ) 
			{
				if( pMatch == NULL )
					pMatch = pEvent;
				else 
				{
					QSYSTime time1( pEvent->ymd, pEvent->hmsms );
					QSYSTime time2( pMatch->ymd, pMatch->hmsms );
					if( d->state == 1 ) 
					{
						if( time1 < time2 ) 
							pMatch = pEvent;
					}
					else 
					{
						if( time1 > time2 ) 
							pMatch = pEvent;
					}
				}
			}
		}

		if(pMatch) 
		{
			pMatch->m_match = d;
			d->m_match = pMatch;
		}
	}

	if(d)
		/*QList<Rsys_event *>::*/append(d);
}

Sys_event_list collect_unconfirmed(Sys_event_list & list) 
{
	Sys_event_list collect;
	for( uint i = 0 ; i < list.count() ; i++ )
	{
		Rsys_event * pEvent = list.at(i);
		if(pEvent->m_bneed_confirm && pEvent->m_event_status == eUnConfirmed)
			collect.append(pEvent);
	}
	return collect;
}

Sys_event_list collect_confirmed(Sys_event_list & list) 
{
	Sys_event_list collect;
	for(uint i=0;i<list.count();i++) {
		Rsys_event * pEvent = list.at(i);
		if(pEvent->m_bneed_confirm && pEvent->m_event_status == eConfirmed)
			collect.append(pEvent);
	}
	return collect;
}

int count_removable_events(Sys_event_list & list)
{
	int c=0;
	for(uint i=0;i<list.count();i++) {
		Rsys_event * pEvent = list.at(i);
		if(!pEvent->m_bneed_confirm || pEvent->m_event_status==eConfirmed)
			c++;
	}
	return c;
}

int count_unconfirmed(Sys_event_list & list)
{
	int c = 0;
	for( uint i = 0; i < list.count() ; i++) 
	{
		Rsys_event * pEvent = list.at(i);
		if( pEvent->m_bneed_confirm && pEvent->m_event_status == eUnConfirmed)
			c++;
	}
	return c;
}

int count_confirmed(Sys_event_list & list)
{
	int c=0;
	for(uint i=0;i<list.count();i++) {
		Rsys_event * pEvent = list.at(i);
		if(pEvent->m_bneed_confirm && pEvent->m_event_status == eConfirmed)
			c++;
	}
	return c;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Event_filter::Event_filter()
{
// 	m_eventTypeList.setAutoDelete( true );
}

Event_filter::~Event_filter()
{
	
}

void Event_filter::add_event_type( PEVENTTYPEDEF pEventTypeDef )
{
	PEVENTTYPEDEF	ptmpTypeDef;

	ptmpTypeDef = new EVENTTYPEDEF;

	memcpy( ptmpTypeDef, pEventTypeDef, sizeof( EVENTTYPEDEF ) );

	m_mutex.lock();
	m_event_type_list.append( ptmpTypeDef );
	m_mutex.unlock();
}

void Event_filter::add_rtu( const QString &rtucode )
{
	m_mutex.lock();
	m_rtu_list.append( rtucode );
	m_mutex.unlock();
	
}

bool Event_filter::is_select( Rsys_event * eve )
{
	bool ret = true;
	PEVENTTYPEDEF pEventDef;
	uint i;

#ifdef EventLevelL
#ifdef _WIN32
	#pragma message("Mabybe  Ver 2.2, has EventLevel! [Please view sysdef.h]")
#endif
	uint level = eve->EventLevelL + 2 * eve->EventLevelM + 4 * eve->EventLevelH;

	if( m_event_level_list.count() != 0 && 
		!m_event_level_list.contains( level ) )
	{
		ret = false;
	}
#else
#ifdef _WIN32
	#pragma message("Mabybe  Ver 3.0, no EventLevel! [Please view sysdef.h]")
#endif
	uint testflag = eve->testflag;
	if( m_event_level_list.count() != 0 && 
		!m_event_level_list.contains( testflag ) )
	{
		ret = false;
	}
#endif

// 	if( !m_rtuList.isEmpty() &&  !_C2Q( eve->Object ).isEmpty() &&( m_rtuList.find( _C2Q( eve->Object ) ) !=m_rtuList.end() ) )
	if( !m_rtu_list.isEmpty() &&  !_C2Q( eve->Object ).isEmpty() &&( m_rtu_list.indexOf( _C2Q( eve->Object ) ) != -1 ) )
	{
		ret =  false;
	}

	if( !m_event_type_list.isEmpty() && ret )
	{
		ret = false;
		for( i = 0 ; i < m_event_type_list.count(); i ++ )
		{
			pEventDef = m_event_type_list.at( i );
			if( ( pEventDef->sort == 0 || ( pEventDef->sort == eve->sort ) ) &&
				( pEventDef->type == 0 || ( pEventDef->type == eve->type ) ) )
			{
				ret =  true;
				break;
			}
		}
	}

	return ret;
}

void Event_filter::add_event( Rsys_event * eve )
{
	m_mutex.lock();
	m_event_list.append( eve );
	m_mutex.unlock();
}

void Event_filter::del_event( Rsys_event * eve )
{
	m_mutex.lock();
// 	m_eventList.remove( eve );
	int i = m_event_list.indexOf(eve);
	if (i != -1)
		delete m_event_list.takeAt(i);
// 	m_eventList.removeOne( eve );
	m_mutex.unlock();
}

void Event_filter::reset_filter()
{
	m_mutex.lock();
	
	m_event_type_list.clear();

	m_rtu_list.clear();

	m_event_level_list.clear();

	m_mutex.unlock();
}

uint Event_filter::get_event_type_num()
{
	uint num;

	num = m_event_type_list.count();

	return num;
}

bool Event_filter::get_event_type( uint index, EVENTTYPEDEF &eventtype )
{

	PEVENTTYPEDEF pEvent;

	if( index < 0 || index >= m_event_type_list.count() )
		return false;

	pEvent = m_event_type_list.at( index );
	eventtype = *pEvent;

	return true;
}

uint Event_filter::get_rtu_num()
{
	uint num;

	num = m_rtu_list.count();

	return num;
}

QString Event_filter::get_rtu_code( uint index )
{
	QString str;


	if( index < 0 || index >= m_rtu_list.count() )
	{
		return QString::null;
	}

	str = m_rtu_list[ index ];

	return str;
}

bool Event_filter::is_select( PEVENTTYPEDEF eve )
{
	bool ret ;
	PEVENTTYPEDEF pEventDef;
	uint i;

	if( !m_event_type_list.isEmpty() )
	{
		ret = false;

		for( i = 0 ; i < m_event_type_list.count(); i ++ )
		{
			pEventDef = m_event_type_list.at( i );
			if( ( pEventDef->sort == 0 || ( pEventDef->sort == eve->sort ) ) &&
				( pEventDef->type == 0 || ( pEventDef->type == eve->type ) ) )
			{
				ret = true;
				break;
			}
		}

		
	}
	else
	{
		ret = true;
	}

	return ret;
}

bool Event_filter::is_select( QString &rtucode )
{
	bool ret;

	if( m_rtu_list.isEmpty() )
		ret = true;
	else
	{
// 		if( m_rtuList.find( rtucode ) == m_rtuList.end() )
		if( m_rtu_list.indexOf( rtucode ) == -1 )
			ret = true;
		else
			ret = false;
	}


	return ret;
}

bool Event_filter::is_select( uint level )
{
	bool ret;

	if( m_event_level_list.isEmpty() )
		ret = true;
	else
	{
// 		if( m_eventLevelList.find( level ) == m_eventLevelList.end() )
		if( m_event_level_list.indexOf( level ) == -1 )
			ret = false;
		else
			ret = true;
	}


	return ret;
}

void Event_filter::add_event_level( uint level )
{
	m_mutex.lock();
	m_event_level_list.append( level );
	m_mutex.unlock();
}

uint Event_filter::get_event_level_num()
{
	uint num;
	num = m_event_level_list.count();

	return num;
}

uint Event_filter::get_event_level( uint index )
{
	uint level;
	level = m_event_level_list[ index ];
	return level;
}