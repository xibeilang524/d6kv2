// QHisEventModifyDlg.cpp: implementation of the QHisEventModifyDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "QHisEventModifyDlg.h"

#include "QEventMainWnd.h"
#include "globaldef.h"
#include "evtapi.h"
#include "sysdef.h"

#include "qlineedit.h"
#include "qapplication.h"
#include "qcombobox.h"
#include "qdatetimeedit.h"
#include "qdatetime.h"
#include "qmessagebox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QHisEventModifyDlg::QHisEventModifyDlg( HIS_EVENT *eve, QWidget* parent, 
		const char* name, bool modal, WFlags fl )
		: QHisEventModiBase( parent, name, modal, fl )
{
	uint			i;			
	QString			desc;
	QEventMainWnd	*pMainWnd = (QEventMainWnd *)qApp->mainWidget();
	int				year, month, day, hour, min,sec,ms;
	ulong			lYmd,lHmsms;
	int				curitem;
	uint			no;

	//设置组序号
	memcpy( &m_curHisEvent, eve, sizeof( HIS_EVENT ) );
	memcpy( &m_oldHisEvent, eve, sizeof( HIS_EVENT ) );
	
	for( i = 0; i < pMainWnd->m_dbPara.getGroupCount(); i ++ )
	{
		pMainWnd->m_dbPara.getGroupDesc( i, desc );
		pMainWnd->m_dbPara.getGroupNo( i , no );
		m_grpCB->insertItem( desc );

		m_grpNoList.append( no );

		if( no == (uint)m_curHisEvent.grpno )
			curitem = i;
	}

	m_grpCB->setCurrentItem( curitem );

	onGrpChanged( curitem );
	
	for( i = 0; i < pMainWnd->m_dbPara.getEventSortCount(); i ++ )
	{
		pMainWnd->m_dbPara.getEventSortDesc( i + 1, desc );
		m_eventSortCB->insertItem( desc );
	}

	m_eventSortCB->setCurrentItem( m_curHisEvent.sort - 1 );
	onEventSortChanged( m_curHisEvent.sort - 1 );

	lYmd = ( ulong)( m_curHisEvent.ymd );
	year = ( int )( lYmd / ( 100l * 100l ) );
	month = ( int )( ( lYmd % ( 100l * 100l ) ) / 100 );
	day = ( int )( ( lYmd % ( 100l * 100l ) ) % 100 );

	lHmsms = ( ulong )( m_curHisEvent.hmsms );
	hour = ( int )( lHmsms / ( 100l * 100l * 1000l ) );
	min = ( int )( ( lHmsms % ( 100l * 100l * 1000l ) ) / ( 100l * 1000l ) );
	sec = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) / 1000l );
	ms = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) % 1000l );

	m_dateTime->setDateTime( QDateTime( QDate( year, month, day ), QTime( hour, min, sec, ms ) ) );

	m_msEdit->setText( _C2Q( "%1" ).arg( ms ) );

	m_describeEdit->setText( _C2Q( m_curHisEvent.text ) );

	m_valueEdit->setText( _C2Q( "%1" ).arg( m_curHisEvent.val ) );
}

QHisEventModifyDlg::~QHisEventModifyDlg()
{
	
}

void QHisEventModifyDlg::onEventSortChanged( int item )
{
	QEventMainWnd *pMainWnd = (QEventMainWnd *)qApp->mainWidget();

	uint types[ 100 ];
	uint num,i ;
	QString desc;
	m_eventTypeCB->clear();
	int curitem = 0;

	m_curHisEvent.sort = item + 1;

	pMainWnd->m_dbPara.getEventTypes( item + 1, types, num );
	m_eventTypeCB->clear();
	m_eventTypeList.clear();
	for( i = 0; i < num; i ++ )
	{
		pMainWnd->m_dbPara.getEventTypeDesc( types[ i ], desc );
		m_eventTypeCB->insertItem( desc );

		if( (uint)m_curHisEvent.type == types[ i ] )
			curitem = i;

		m_eventTypeList.append( types[ i ] );
	}

	m_eventTypeCB->setCurrentItem( curitem );

	onEventTypeChanged( curitem );
}


void QHisEventModifyDlg::onEventTypeChanged( int item )
{
	int grpno = m_grpNoList[ m_grpCB->currentItem() ];
	QEventMainWnd	*pMainWnd = (QEventMainWnd *)qApp->mainWidget();
	uint i ;
	QString desc;
	int curitem = 0;

	m_stationList.clear();
	m_rtuCB->clear();
	if( m_curHisEvent.sort == ENTCLASS_SCADA )
	{
		pMainWnd->m_dbPara.getStaCode( grpno, m_stationList );

		for( i = 0; i < m_stationList.count() ; i ++ )
		{
			pMainWnd->m_dbPara.getStaDesc( m_stationList[ i ], desc );
			m_rtuCB->insertItem( desc );
			if( m_stationList[ i ] == _C2Q( m_curHisEvent.Object ) )
			{
				curitem = i;
			}
		}
	}

	m_curHisEvent.type = m_eventTypeList[ item ];

	m_rtuCB->setCurrentItem( curitem );
	onRtuChanged( curitem );
}

void QHisEventModifyDlg::onRtuChanged( int item )
{
	QEventMainWnd	*pMainWnd = (QEventMainWnd *)qApp->mainWidget();
	QStringList	objectList;
	uint i;
	uint type;
	QString desc;
	int curitem = 0;

	strcpy( m_curHisEvent.Object, _Q2C( m_stationList[ item ] ) );
	m_objectCB->clear();
	m_objectList.clear();
	if( m_curHisEvent.sort != ENTCLASS_SCADA )
		return;

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_OVERLIMI )
	{
		pMainWnd->m_dbPara.getYcCode( m_curHisEvent.Object, m_objectList );
		for( i = 0; i < m_objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYcDesc( m_objectList[ i ], desc );
			m_objectCB->insertItem( desc );
			if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
			{
				curitem = i;
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_STATUS || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_STATUSODD || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_STATUSSOE )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_STATUS )
			{
				pMainWnd->m_dbPara.getYcDesc( objectList[ i ], desc );
				m_objectCB->insertItem( desc );
				m_objectList.append( objectList[ i ] );
				if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
				{
					curitem = i;
				}
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_BREAK || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_BREAKSG || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_BREAKSOE )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );

			if( type >= OBJTYPE_BREAK && type < OBJTYPE_STATUS )
			{
				pMainWnd->m_dbPara.getYxDesc( objectList[ i ], desc );
				m_objectCB->insertItem( desc );
				m_objectList.append( objectList[ i ] );
				if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
				{
					curitem = i;
				}
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_SWITCH || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_SWITCHSOE  )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );

			if( type >= OBJTYPE_SWITCH && type < OBJTYPE_PROTSIG )
			{
				pMainWnd->m_dbPara.getYxDesc( objectList[ i ], desc );
				m_objectCB->insertItem( desc );
				m_objectList.append( objectList[ i ] );
				if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
				{
					curitem = i;
				}
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_PROTSIG || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_PROTSIGSOE  || 
		m_curHisEvent.type ==  OBJTYPE_SGZ )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_PROTSIG )
			{
				pMainWnd->m_dbPara.getYxDesc( objectList[ i ], desc );
				m_objectCB->insertItem( desc );
				m_objectList.append( objectList[ i ] );
				if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
				{
					curitem = i;
				}
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_YGSIG || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_YGSIGSOE )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_YG )
			{
				pMainWnd->m_dbPara.getYxDesc( objectList[ i ], desc );
				m_objectCB->insertItem( desc );
				m_objectList.append( objectList[ i ] );
				if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
				{
					curitem = i;
				}
			}
		}
	}

	if( m_curHisEvent.type == ENTTYPE_POWERSYS_SGOVERLIMIT || 
		m_curHisEvent.type == ENTTYPE_POWERSYS_CHGOVERLIMIT  || 
		m_curHisEvent.type ==  ENTTYPE_POWERSYS_YXDOUDONG )
	{
		pMainWnd->m_dbPara.getYxCode( m_curHisEvent.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_dbPara.getYxType( objectList[ i ], type );
			pMainWnd->m_dbPara.getYxDesc( objectList[ i ], desc );
			m_objectCB->insertItem( desc );
			m_objectList.append( objectList[ i ] );
			if( m_objectList[ i ] == _C2Q( m_curHisEvent.member0 ) )
			{
				curitem = i;
			}
		}
	}

	m_objectCB->setCurrentItem( curitem );
}

void QHisEventModifyDlg::onGrpChanged( int item )
{
	int grpno = m_grpNoList[ item ];
	QEventMainWnd	*pMainWnd = (QEventMainWnd *)qApp->mainWidget();
	uint i ;
	QString desc;
	int curitem = 0;

	m_stationList.clear();
	m_rtuCB->clear();
	if( m_curHisEvent.sort == ENTCLASS_SCADA )
	{
		pMainWnd->m_dbPara.getStaCode( grpno, m_stationList );

		for( i = 0; i < m_stationList.count() ; i ++ )
		{
			pMainWnd->m_dbPara.getStaDesc( m_stationList[ i ], desc );
			m_rtuCB->insertItem( desc );
			if( m_stationList[ i ] == _C2Q( m_curHisEvent.Object ) )
			{
				curitem = i;
			}
		}
	}

	m_rtuCB->setCurrentItem( curitem );
	onRtuChanged( curitem );
}

void QHisEventModifyDlg::onModifyEvent()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;
	FUNC_STR func;
	CS_DATAFMT* fmt=0;

	getEvent();

	int OEventf = evtproc.IsOptEvt(&m_curHisEvent);

	int year = m_curHisEvent.ymd / 10000;
	int mon = ( m_curHisEvent.ymd % 10000 ) / 100;
	evtproc.InitDb( db, year, mon, OEventf, &func, &fmt );
	
	for(int i=0; i<func.colnum; i++)
	{
		func.col[i].select = NO;
		func.col[i].condition = NO;
		if(strcmp(fmt[i].name,"年月日")==0)
		{
			func.col[i].condition = YES;
			func.col[i].val_o.intvalue = m_oldHisEvent.ymd;
			func.col[i].select = YES;
			func.col[i].val.intvalue = m_curHisEvent.ymd;
		}
		else if(strcmp(fmt[i].name,"时分秒毫秒")==0)
		{
			func.col[i].condition=YES;
			func.col[i].val_o.intvalue = m_oldHisEvent.hmsms;
			func.col[i].select = YES;
			func.col[i].val.intvalue = m_curHisEvent.hmsms;
		}
		else if(strcmp(fmt[i].name,"文字")==0)
		{
			func.col[i].condition=YES;
			strcpy( func.col[i].val_o.charvalue, m_oldHisEvent.text );
			func.col[i].select = YES;
			strcpy( func.col[i].val.charvalue, m_curHisEvent.text );
		}
	}
	if( evtproc.UpdateEvent( db, &func, fmt, OEventf ) )
	{
		QMessageBox::message( _C2Q("提示"), _C2Q( "修改成功!" ) );
	}
	if(fmt)	free(fmt);
	db.Close();
}


void QHisEventModifyDlg::onDelEvent()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;
	FUNC_STR func;
	CS_DATAFMT* fmt=0;

	getEvent();

	int OEventf = evtproc.IsOptEvt(&m_curHisEvent);

	int year = m_curHisEvent.ymd / 10000;
	int mon = ( m_curHisEvent.ymd % 10000 ) / 100;
	evtproc.InitDb( db, year, mon, OEventf, &func, &fmt );
	
	for(int i=0; i<func.colnum; i++)
	{
		func.col[i].select = NO;
		func.col[i].condition = NO;
		if(strcmp(fmt[i].name,"年月日")==0)
		{
			func.col[i].condition = YES;
			func.col[i].val_o.intvalue = m_oldHisEvent.ymd;
		}
		else if(strcmp(fmt[i].name,"时分秒毫秒")==0)
		{
			func.col[i].condition=YES;
			func.col[i].val_o.intvalue = m_oldHisEvent.hmsms;
		}
		else if(strcmp(fmt[i].name,"文字")==0)
		{
			func.col[i].condition=YES;
			strcpy( func.col[i].val_o.charvalue, m_oldHisEvent.text );
		}
	}
	if( evtproc.DeleteEvent(db,&func,fmt,OEventf) )
	{
		QMessageBox::message( _C2Q("提示"), _C2Q( "删除成功!" ) );
	}
	if(fmt)	free(fmt);
	db.Close();
}


void QHisEventModifyDlg::onAddEvent()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;

	getEvent();
	if( evtproc.InsertEvent( db, 1, &m_curHisEvent ) )
	{
		QMessageBox::message( _C2Q("提示"), _C2Q( "增加成功!" ) );
	}
	db.Close();
}

void QHisEventModifyDlg::getEvent()
{
	m_curHisEvent.grpno = m_grpNoList[ m_grpCB->currentItem() ];
	m_curHisEvent.sort = m_eventSortCB->currentItem() + 1;
	m_curHisEvent.type = m_eventTypeList[ m_eventTypeCB->currentItem() ];
	m_curHisEvent.ymd = m_dateTime->dateTime().date().year() * 10000l + 
		m_dateTime->dateTime().date().month() * 100l+ 
		m_dateTime->dateTime().date().day();
	m_curHisEvent.hmsms = m_dateTime->dateTime().time().hour() * 10000000l +
		m_dateTime->dateTime().time().minute() * 100000l +
		m_dateTime->dateTime().time().second() * 1000 +
		m_msEdit->text().toLong();
	strcpy( m_curHisEvent.Object, _Q2C( m_stationList[ m_rtuCB->currentItem() ] ) );
	strcpy( m_curHisEvent.member0, _Q2C( m_objectList[ m_objectCB->currentItem() ] ) );
	strcpy( m_curHisEvent.text, _Q2C( m_describeEdit->text() ) );
	m_curHisEvent.val = m_valueEdit->text().toFloat();
}