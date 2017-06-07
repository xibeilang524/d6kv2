#include "hisquery_modify_dialog.h"
#include "hisquery_wnd.h"

#include "db/evtapi.h"

#include <QMessageBox>
#include "uitools/dmessagebox.h"

Event_modify_dialog::Event_modify_dialog( HIS_EVENT *eve , QWidget* parent /*= 0 */ ) : QDialog( parent )
{
	setupUi( this ) ;
	ini_ui() ;
	ini_data( eve ) ;

	connect( m_pEvent_type_combobox , SIGNAL( activated( int ) ) , this , SLOT( slot_event_type_changed( int ) ) ) ;
	connect( m_pEvent_sort_combobox , SIGNAL( activated( int ) ) , this , SLOT( slot_event_sort_changed( int ) ) ) ;
	connect( m_pRtu_combobox , SIGNAL( activated( int ) ) , this , SLOT( slot_rtu_changed( int ) ) ) ;
	connect( m_pGroup_combobox , SIGNAL( activated( int ) ) , this , SLOT( slot_group_changed( int ) ) ) ;
	connect( m_pModify_button , SIGNAL( clicked() ) , this , SLOT( slot_modify_event() ) ) ;
	connect( m_pDelete_button , SIGNAL( clicked() ) , this , SLOT( slot_delete_event() ) ) ;
	connect( m_pAdd_button , SIGNAL( clicked() ) , this , SLOT( slot_add_event() ) ) ;
}

void Event_modify_dialog::ini_ui( void )
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QPushButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font: 16px;\
						   color:#FFFFFF;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width: 58px ;\
						   }\
						   QPushButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QPushButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QPushButton:disabled\
						   {\
						   background: #E5E5E5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QDialog\
						   {\
						   background: #FAFAFA;\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   min-height:25px;\
						   }\
						   QComboBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QLineEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QDateTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QDateTimeEdit::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QDateTimeEdit::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}

void Event_modify_dialog::ini_data( HIS_EVENT *eve )
{
	Hisquery_wnd* pMainWnd = ( Hisquery_wnd * )getMainWnd() ;
	int year, month, day, hour, min,sec,ms;
	ulong lYmd,lHmsms;
	int curitem;
	uint no;
	QString			desc;

	m_group_no_list.clear();

	//设置组序号
	memcpy( &m_cur_event, eve, sizeof( HIS_EVENT ) );
	memcpy( &m_old_event, eve, sizeof( HIS_EVENT ) );

	for( int i = 0 ; i < pMainWnd->m_db_para.getGroupCount(); i ++ )
	{
		pMainWnd->m_db_para.getGroupDesc( i, desc );
		pMainWnd->m_db_para.getGroupNo( i , no );
		m_pGroup_combobox->addItem( desc );

		m_group_no_list.append( no );

		if( no == (uint)m_cur_event.grpno )
			curitem = i;
	}

	m_pGroup_combobox->setCurrentIndex( curitem );

	slot_group_changed( curitem );

	for( int i = 0 ; i < pMainWnd->m_db_para.getEventSortCount(); i ++ )
	{
		pMainWnd->m_db_para.getEventSortDesc( i + 1, desc );
		m_pEvent_sort_combobox->addItem( desc );
	}

	m_pEvent_sort_combobox->setCurrentIndex( m_cur_event.sort - 1 );
	slot_event_sort_changed( m_cur_event.sort - 1 );

	lYmd = ( ulong)( m_cur_event.ymd );
	year = ( int )( lYmd / ( 100l * 100l ) );
	month = ( int )( ( lYmd % ( 100l * 100l ) ) / 100 );
	day = ( int )( ( lYmd % ( 100l * 100l ) ) % 100 );

	lHmsms = ( ulong )( m_cur_event.hmsms );
	hour = ( int )( lHmsms / ( 100l * 100l * 1000l ) );
	min = ( int )( ( lHmsms % ( 100l * 100l * 1000l ) ) / ( 100l * 1000l ) );
	sec = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) / 1000l );
	ms = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) % 1000l );

	m_pDatetime_edit->setDateTime( QDateTime( QDate( year, month, day ), QTime( hour, min, sec, ms ) ) );

	m_pMs_edit->setText( tr( "%1" ).arg( ms ) );

	m_pEvent_desc_edit->setText( tr( m_cur_event.text ) );

	m_pValue_edit->setText( tr( "%1" ).arg( m_cur_event.val ) );
}


void Event_modify_dialog::slot_event_sort_changed( int item )
{
	Hisquery_wnd* pMainWnd = ( Hisquery_wnd * )getMainWnd() ;

	uint types[ 100 ];
	uint num,i ;
	QString desc;
	m_pEvent_type_combobox->clear();
	int curitem = 0;

	m_cur_event.sort = item + 1;

	pMainWnd->m_db_para.getEventTypes( item + 1, types, num );
	m_pEvent_type_combobox->clear();
	m_event_type_list.clear();
	for( i = 0; i < num; i ++ )
	{
		pMainWnd->m_db_para.getEventTypeDesc( types[ i ], desc );
		m_pEvent_type_combobox->addItem( desc );

		if( (uint)m_cur_event.type == types[ i ] )
			curitem = i;

		m_event_type_list.append( types[ i ] );
	}

	m_pEvent_type_combobox->setCurrentIndex( curitem );

	slot_event_type_changed( curitem );
}


void Event_modify_dialog::slot_event_type_changed( int item )
{
	int grpno = m_group_no_list[ m_pGroup_combobox->currentIndex() ];
	Hisquery_wnd* pMainWnd = ( Hisquery_wnd * )getMainWnd() ;
	uint i ;
	QString desc;
	int curitem = 0;

	m_station_list.clear();
	m_pRtu_combobox->clear();
	if( m_cur_event.sort == ENTCLASS_SCADA )
	{
		pMainWnd->m_db_para.getStaCode( grpno, m_station_list );

		for( i = 0; i < m_station_list.count() ; i ++ )
		{
			pMainWnd->m_db_para.getStaDesc( m_station_list[ i ], desc );
			m_pRtu_combobox->addItem( desc );
			if( m_station_list[ i ] == tr( m_cur_event.Object ) )
			{
				curitem = i;
			}
		}
	}

	m_cur_event.type = m_event_type_list[ item ];

	m_pRtu_combobox->setCurrentIndex( curitem );
	slot_rtu_changed( curitem );
}

void Event_modify_dialog::slot_rtu_changed( int item )
{
	Hisquery_wnd* pMainWnd = ( Hisquery_wnd * )getMainWnd() ;
	QStringList	objectList;
	uint i;
	uint type;
	QString desc;
	int curitem = 0;

	if ( m_station_list.count() <= 0 )
		return ;
	strcpy( m_cur_event.Object, _Q2C( m_station_list[ item ] ) );
	m_pObject_combobox->clear();
	m_object_list.clear();
	if( m_cur_event.sort != ENTCLASS_SCADA )
		return;

	if( m_cur_event.type == ENTTYPE_POWERSYS_OVERLIMI )
	{
		pMainWnd->m_db_para.getYcCode( m_cur_event.Object, m_object_list );
		for( i = 0; i < m_object_list.count(); i ++ )
		{
			pMainWnd->m_db_para.getYcDesc( m_object_list[ i ], desc );
			m_pObject_combobox->addItem( desc );
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_STATUS || 
		m_cur_event.type == ENTTYPE_POWERSYS_STATUSODD || 
		m_cur_event.type == ENTTYPE_POWERSYS_STATUSSOE )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_STATUS )
			{
				pMainWnd->m_db_para.getYcDesc( objectList[ i ], desc );
				m_pObject_combobox->addItem( desc );
				m_object_list.append( objectList[ i ] );
			}
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_BREAK || 
		m_cur_event.type == ENTTYPE_POWERSYS_BREAKSG || 
		m_cur_event.type == ENTTYPE_POWERSYS_BREAKSOE )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );

			if( type >= OBJTYPE_BREAK && type < OBJTYPE_STATUS )
			{
				pMainWnd->m_db_para.getYxDesc( objectList[ i ], desc );
				m_pObject_combobox->addItem( desc );
				m_object_list.append( objectList[ i ] );
			}
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_SWITCH || 
		m_cur_event.type == ENTTYPE_POWERSYS_SWITCHSOE  )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );

			if( type >= OBJTYPE_SWITCH && type < OBJTYPE_PROTSIG )
			{
				pMainWnd->m_db_para.getYxDesc( objectList[ i ], desc );
				m_pObject_combobox->addItem( desc );
				m_object_list.append( objectList[ i ] );
			}
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_PROTSIG || 
		m_cur_event.type == ENTTYPE_POWERSYS_PROTSIGSOE  || 
		m_cur_event.type ==  OBJTYPE_SGZ )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_PROTSIG )
			{
				pMainWnd->m_db_para.getYxDesc( objectList[ i ], desc );
				m_pObject_combobox->addItem( desc );
				m_object_list.append( objectList[ i ] );
			}
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_YGSIG || 
		m_cur_event.type == ENTTYPE_POWERSYS_YGSIGSOE )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );

			if( type == OBJTYPE_YG )
			{
				pMainWnd->m_db_para.getYxDesc( objectList[ i ], desc );
				m_pObject_combobox->addItem( desc );
				m_object_list.append( objectList[ i ] );
			}
		}
	}

	if( m_cur_event.type == ENTTYPE_POWERSYS_SGOVERLIMIT || 
		m_cur_event.type == ENTTYPE_POWERSYS_CHGOVERLIMIT  || 
		m_cur_event.type ==  ENTTYPE_POWERSYS_YXDOUDONG )
	{
		pMainWnd->m_db_para.getYxCode( m_cur_event.Object, objectList );
		for( i = 0; i < objectList.count(); i ++ )
		{
			pMainWnd->m_db_para.getYxType( objectList[ i ], type );
			pMainWnd->m_db_para.getYxDesc( objectList[ i ], desc );
			m_pObject_combobox->addItem( desc );
			m_object_list.append( objectList[ i ] );
		}
	}

	for ( int j = 0; j < m_object_list.count() ;j++)
	{
		if( m_object_list[ j ] == tr( m_cur_event.member0 ) )
		{
			curitem = j;
		}
	}

	m_pObject_combobox->setCurrentIndex( curitem );
}

void Event_modify_dialog::slot_group_changed( int item )
{
	int grpno = m_group_no_list[ item ];
	Hisquery_wnd* pMainWnd = ( Hisquery_wnd * )getMainWnd() ;
	uint i ;
	QString desc;
	int curitem = 0;

	m_station_list.clear();
	m_pRtu_combobox->clear();
	if( m_cur_event.sort == ENTCLASS_SCADA )
	{
		pMainWnd->m_db_para.getStaCode( grpno, m_station_list );

		for( i = 0; i < m_station_list.count() ; i ++ )
		{
			pMainWnd->m_db_para.getStaDesc( m_station_list[ i ], desc );
			m_pRtu_combobox->addItem( desc );
			if( m_station_list[ i ] == tr( m_cur_event.Object ) )
			{
				curitem = i;
			}
		}
	}

	m_pRtu_combobox->setCurrentIndex( curitem );
	slot_rtu_changed( curitem );
}

void Event_modify_dialog::slot_modify_event()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;
	FUNC_STR func;
	CS_DATAFMT* fmt=0;

	get_event();

	int OEventf = evtproc.IsOptEvt(&m_cur_event);

	int year = m_cur_event.ymd / 10000;
	int mon = ( m_cur_event.ymd % 10000 ) / 100;
	evtproc.InitDb( db, year, mon, OEventf, &func, &fmt );

	for(int i=0; i<func.colnum; i++)
	{
		func.col[i].select = NO;
		func.col[i].condition = NO;
		if(strcmp(fmt[i].name,"年月日")==0)
		{
			func.col[i].condition = YES;
			func.col[i].val_o.intvalue = m_old_event.ymd;
			func.col[i].select = YES;
			func.col[i].val.intvalue = m_cur_event.ymd;
		}
		else if(strcmp(fmt[i].name,"时分秒毫秒")==0)
		{
			func.col[i].condition=YES;
			func.col[i].val_o.intvalue = m_old_event.hmsms;
			func.col[i].select = YES;
			func.col[i].val.intvalue = m_cur_event.hmsms;
		}
		else if(strcmp(fmt[i].name,"值")==0)
		{
			func.col[i].condition=YES;
			func.col[i].val_o.realvalue = m_old_event.val;
			func.col[i].select = YES;
			func.col[i].val.realvalue = m_cur_event.val;
		}
		else if(strcmp(fmt[i].name,"文字")==0)
		{
			func.col[i].condition=YES;
			strcpy( func.col[i].val_o.charvalue, m_old_event.text );
			func.col[i].select = YES;
			strcpy( func.col[i].val.charvalue, m_cur_event.text );
		}
	}
	if( evtproc.UpdateEvent( db, &func, fmt, OEventf ) )
	{
// 		QMessageBox::information( this ,tr("提示"), tr( "修改成功!" ) );
		DMessageBox::information( this ,tr("提示"), tr( "修改成功!" ) );
	}
	if(fmt)	free(fmt);
	db.Close();
}


void Event_modify_dialog::slot_delete_event()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;
	FUNC_STR func;
	CS_DATAFMT* fmt=0;

	get_event();

	int OEventf = evtproc.IsOptEvt(&m_cur_event);

	int year = m_cur_event.ymd / 10000;
	int mon = ( m_cur_event.ymd % 10000 ) / 100;
	evtproc.InitDb( db, year, mon, OEventf, &func, &fmt );

	for(int i=0; i<func.colnum; i++)
	{
		func.col[i].select = NO;
		func.col[i].condition = NO;
		if(strcmp(fmt[i].name,"年月日")==0)
		{
			func.col[i].condition = YES;
			func.col[i].val_o.intvalue = m_old_event.ymd;
		}
		else if(strcmp(fmt[i].name,"时分秒毫秒")==0)
		{
			func.col[i].condition=YES;
			func.col[i].val_o.intvalue = m_old_event.hmsms;
		}
		else if(strcmp(fmt[i].name,"文字")==0)
		{
			func.col[i].condition=YES;
			strcpy( func.col[i].val_o.charvalue, m_old_event.text );
		}
	}
	if( evtproc.DeleteEvent(db,&func,fmt,OEventf) )
	{
// 		QMessageBox::information( this , tr("提示"), tr( "删除成功!" ) );
		DMessageBox::information( this ,tr("提示"), tr( "删除成功!" ) );
	}
	if(fmt)	free(fmt);
	db.Close();
}


void Event_modify_dialog::slot_add_event()
{
	DBApi db;
	db.Open(DB_OP_WR,FALSE,TRUE);
	CEvtProc  evtproc;

	get_event();
	if( evtproc.InsertEvent( db, 1, &m_cur_event ) )
	{
// 		QMessageBox::information( this ,tr("提示"), tr( "增加成功!" ) );
		DMessageBox::information( this ,tr("提示"), tr( "增加成功!" ) );
	}
	db.Close();
}

void Event_modify_dialog::get_event()
{
	m_cur_event.grpno = m_group_no_list[ m_pGroup_combobox->currentIndex() ];
	m_cur_event.sort = m_pEvent_sort_combobox->currentIndex() + 1;
	m_cur_event.type = m_event_type_list[ m_pEvent_type_combobox->currentIndex() ];
	m_cur_event.ymd = m_pDatetime_edit->dateTime().date().year() * 10000l + 
		m_pDatetime_edit->dateTime().date().month() * 100l+ 
		m_pDatetime_edit->dateTime().date().day();
	m_cur_event.hmsms = m_pDatetime_edit->dateTime().time().hour() * 10000000l +
		m_pDatetime_edit->dateTime().time().minute() * 100000l +
		m_pDatetime_edit->dateTime().time().second() * 1000 +
		m_pMs_edit->text().toLong();
	if (m_station_list.count() <= 0)
	{
		strcpy( m_cur_event.Object, "" );
	}
	else
	{
		strcpy( m_cur_event.Object, _Q2C( m_station_list[ m_pRtu_combobox->currentIndex() ] ) );
	}
	if (m_object_list.count() <= 0)
	{
		strcpy( m_cur_event.member0, "" );
	}
	else
	{
		strcpy( m_cur_event.member0, _Q2C( m_object_list[ m_pObject_combobox->currentIndex() ] ) );
	}

	strcpy( m_cur_event.text, _Q2C( m_pEvent_desc_edit->text() ) );
	m_cur_event.val = m_pValue_edit->text().toFloat();
}
