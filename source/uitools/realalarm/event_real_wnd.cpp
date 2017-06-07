// QEventRealWnd.cpp: implementation of the QEventRealWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "event_real_wnd.h"
#include "uitools/dmessagebox.h"


#include <QTableWidget>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QToolButton>
#include <QMessageBox>
#include <QFile>
#include <QShortcut>
#include <QCheckBox>
#include <QSplitter>
#include <QAction>
#include <QProcess>
#include <QToolBar>
#include <QStringList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtXml/QDomProcessingInstruction>
#include <QTextStream>
#include <QMouseEvent>

#include "globaldef.h"
#include "event_ui_set_dlg.h"
#include "event_sys_set_dlg.h"


// #ifdef (_WIN32)
// #include <io.h>
// #else
// #include <sys/socket.h>
// #endif

#ifdef _WIN32
	#include <mmsystem.h>
	#include "windows.h"
#include "atlbase.h"
#include "atlwin.h"
#else
	#include "QMySound.h"
#endif


#define LISTENPORT	5555

extern void saveFileName( SYS_EVENT *eve );


Event_real_wnd::Event_real_wnd( QWidget * parent, const char * name, Qt::WindowFlags f ):QMainWindow( parent, f ), m_read_event_proc(this), m_print(&m_event_cfg, &m_db_para)
{
	m_cur_font = QApplication::desktop()->font();
	m_cur_font.setPointSize(9);
 	m_cur_font.setFamily( tr( "微软雅黑") ) ;
//	m_cur_font.setFamily( tr( "黑体") ) ;
	setFont( m_cur_font );

	setWindowTitle( tr( "智能告警" ) );
	m_centre_widget = new QWidget(this);
	m_centre_widget->setObjectName(tr("centreWidget"));
	m_centre_widget->setMouseTracking(true);

// 	setIcon( qPixmapFromMimeSource( "realalarm.png" ) );
	setWindowIcon( QIcon(":/img/images/realalarm1.png" ) );
	setWindowFlags(Qt::FramelessWindowHint);
	m_new_event_icon = QIcon(":/img/images/newevent.png" );
	m_no_new_event_icon = QIcon( ":/img/images/nonewevent.png" );
	m_tab_widget = NULL;


	setCentralWidget( m_centre_widget );
	init_title_and_toolbtn_frame();
//  m_wndList.setAutoDelete( true );
	m_tab_frame = new QFrame(m_centre_widget);
	m_tab_frame->setObjectName(tr("tabFrame"));
	m_tab_frame->setMouseTracking(true);
	QString styleSheet;
	styleSheet = tr( "#tabFrame\
					 {\
					 background-color: rgb(250, 250, 250);\
					 }") ;
	m_tab_frame->setStyleSheet( styleSheet ) ;

	m_splitter = new QSplitter( Qt::Vertical, m_tab_frame );
	m_splitter->setMouseTracking(true);
	m_vtab_layout = new QVBoxLayout(m_tab_frame);
	m_vtab_layout->addWidget(m_splitter);

	m_vbox_layout = new QVBoxLayout(m_centre_widget);
	m_vbox_layout->addWidget(m_title_frame);
	m_vbox_layout->addWidget(m_tool_button_frame);
	m_vbox_layout->addWidget(m_tab_frame);
	m_vbox_layout->setContentsMargins(0,0,0,0);
	m_vbox_layout->setSpacing(0);

	m_status_label = new QLabel(statusBar());
	m_status_label->setObjectName("status_label");
	m_status_label->setMouseTracking(true);
	statusBar()->addWidget(m_status_label);
	QString status_style_sheet = tr("#status_label{ font-size: 14px;}");
	m_status_label->setStyleSheet(status_style_sheet);

	setContextMenuPolicy(Qt::NoContextMenu) ;
	setMouseTracking(true);
	m_bleft_press_down = false;
	dir = NONE;

	memset( &m_cur_user, 0, sizeof( USEROPERRIGHT_STRU ) );
	memset( &m_cur_oper_man, 0, sizeof( DISPATCHER_STRU ) );

 	m_bpause = false;
	m_page_count = 0;
	m_current_table_index = 0;
	connect(&m_read_event_proc, SIGNAL(signal_send_event(Rsys_event * ,int)), this, SLOT(slot_custom_event(Rsys_event * ,int)));

	statusBar()->setFont( m_cur_font );
	statusBar()->setMouseTracking(true);
	printf( "0000000000000\n" );

	if( !m_db_para.read_para() )
	{
// 		QMessageBox::information( this , tr("警告"), tr( "读数据库失败!" ) );
		DMessageBox::information( this , tr("警告"), tr( "读数据库失败!" ) );
	}

	m_breg_successed = reg_proc();
	if( !m_breg_successed )
	{
// 		QMessageBox::information( this ,tr("警告"), tr( "程序注册失败!" ) );
		DMessageBox::information( this ,tr("警告"), tr( "程序注册失败!" ) );
	}	
	m_check_timer = startTimer( 2000 );
	read_ini();
	
	if( m_page_list.isEmpty() )
	{
		Event_table	* pTable;
// 		if( m_tabWidget == NULL )
// 		{
// 			pTable = new QEventTable( &m_eventCfg, &m_dbPara, m_splitter, "QEventTable" );
// 		}
// 		else
// 		{
// 			pTable = new QEventTable( &m_eventCfg, &m_dbPara, m_tabWidget, "QEventTable" );
// 		}
		pTable = new Event_table( &m_event_cfg, &m_db_para, this, "QEventTable" );
		pTable->setFont( m_cur_font );
		pTable->set_page_name( tr( "所有事项" ) );
		m_page_count++;
		pTable->set_page_index(m_page_count);
		add_page( pTable );
	}

	//从实时库中读未确认事项
	//readUnConfirmEvent();

	//启动事项侦听线程
	m_read_event_proc.start();

	//启动语音报警线程
	if( m_event_cfg.get_tone() )
	{
		printf( "语音线程启动\n" );
		m_event_sound.set_tone_dev( m_event_cfg.get_tone_dev() );
		m_event_sound.start( );
	}

	//启动短信报警线程
//#ifdef _WIN32
	if( m_event_cfg.get_send_msg() )
	{
		printf( "短信线程启动\n" );
		m_event_msg.set_serial_no( m_event_cfg.get_serial_no() );
		m_event_msg.start();
	}
	if ( m_event_cfg.get_rcv_msg())
	{
		m_event_msg.m_brcv_msg = true;
		m_event_msg.set_msg_confirm_time(m_event_cfg.get_msg_confirm_time());
	} 

//#endif

	printf( "44444444\n" );

	//启动打印线程
	if( m_event_cfg.get_print() )
	{
		m_print.set_printer( m_event_cfg.get_printer() );
	}
	
	create_socket();

	m_del_event_timer = startTimer( 800 );
	m_recv_data_timer = startTimer( 500 );
	m_print_event_timer = startTimer( 1000 );

	m_write_point = m_read_point = 0;

// 	m_realEventList.setAutoDelete( true );

	//把窗口显示在桌面上方
	QDesktopWidget *pDesktop = QApplication::desktop();

	const QRect &desktopRect = pDesktop->screenGeometry();

	QRect newRect;

	newRect.setTopLeft( QPoint( 0, 25 ) );

	newRect.setWidth( desktopRect.width() );
	setMaximumWidth( desktopRect.width() );
	newRect.setHeight( desktopRect.height() / 4 );
	setMaximumHeight( desktopRect.height() );

	setGeometry( newRect );	

	newRect.setTopLeft( QPoint( newRect.left() , 
		newRect.bottom() + 30 )  );

	newRect.setWidth( desktopRect.width() );
	newRect.setHeight( desktopRect.height() / 4 );

	for( uint i = 0; i < m_wnd_list.count(); i ++ )
	{
		Event_table	* pTable = m_wnd_list.at( i );
		pTable->setGeometry( newRect );
	}
	
	m_bclose_wnd = false;

	if( m_event_cfg.get_dgb_out() )
		load_event();

	QShortcut *helpAction = new QShortcut( QKeySequence(Qt::Key_F1), this );
	connect( helpAction, SIGNAL(activated()), this, SLOT(slot_show_help()) );
	move(0,0);

}

Event_real_wnd::~Event_real_wnd()
{

}

void Event_real_wnd::init_title_and_toolbtn_frame()
{

	m_title_frame = new Title_frame(m_centre_widget);
	connect( m_title_frame->m_psetting_button, SIGNAL( clicked() ),this, SLOT( slot_sys_set() ) );
	m_tool_button_frame = new Tool_button_frame(m_centre_widget);
	connect( m_tool_button_frame->m_pwin_set_button, SIGNAL( clicked() ),this, SLOT( slot_ui_set() ) );
	connect( m_tool_button_frame->m_pwin_top_button, SIGNAL( clicked() ),this, SLOT( slot_top_most() ) );
	connect( m_tool_button_frame->m_ppause_button, SIGNAL( clicked() ),this, SLOT( slot_pause_btn() ) );
	connect( m_tool_button_frame->m_pdelete_button, SIGNAL( clicked() ),this, SLOT( slot_delete_event() ) );
	connect( m_tool_button_frame->m_pconfirm_sound_button, SIGNAL( clicked() ),this, SLOT( slot_confirm_snd() ) );
	connect( m_tool_button_frame->m_pconfirm_all_button, SIGNAL( clicked() ),this, SLOT( slot_confirm_all_event() ) );
	connect( m_tool_button_frame->m_pconfirm_event_button, SIGNAL( clicked() ),this, SLOT( slot_confirm_event() ) );
	m_title_frame->setMouseTracking(true);
	m_tool_button_frame->setMouseTracking(true);
}

void Event_real_wnd::add_page( Event_table *table )
{
	if( m_page_list.count() > 0 )
	{
		if( m_tab_widget == NULL )
		{
			m_tab_widget = new QTabWidget( m_splitter );
			m_tab_widget->setFont( m_cur_font );
			m_tab_widget->setMouseTracking(true);
			//设置QtabWidget头样式
			m_tab_widget->setStyleSheet("QTabWidget::tab-bar{alignment:left;}"
				"QTabBar::tab{color:#0F1923; font-size: 16px;min-width:120px; min-height:30px;border:0px;}"
				"QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
				"QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}");

			connect( m_tab_widget, SIGNAL( currentChanged ( int ) ), this, SLOT( slot_show_page( int ) ) );
		}

		if( m_page_list.count() == 1 )
		{
			m_tab_widget->addTab( (QWidget*)m_page_list.at( 0 ), 
				m_no_new_event_icon, 
				m_page_list.at( 0 )->get_page_name() );
			m_splitter->insertWidget( 0, m_tab_widget );
			m_tab_widget->setMouseTracking(true);
		}
		
		m_tab_widget->addTab( (QWidget*)table, m_no_new_event_icon, table->get_page_name() );
		m_tab_widget->show();
	}

	m_page_list.append( table );
}

void Event_real_wnd::add_wnd( Event_table *table )
{
	QRect newRect;

	newRect.setTopLeft( QPoint( geometry().left() , 
		geometry().bottom() + 30 ) );

	newRect.setWidth( width() );
	newRect.setHeight( height() );
	table->setGeometry( newRect );
	table->show();
	m_wnd_list.append( table );
	m_splitter->addWidget( table );
}

void Event_real_wnd::save_event()
{
	char filename[ 256 ];
	sprintf( filename , "%s/log/dat/revent.dat" , getenv("NBENV"));

	FILE *fp = fopen( filename , "wb" );
	if( fp == NULL ) return ;

	uint max = 3000;
	uint count = m_real_event_list.count();
	uint c = count<max?count:max;

	fwrite( &c , sizeof(c) , 1 , fp );
	if( c == 0 )
	{
		fclose( fp );
		return ;
	}

	for(uint i=count-c;i<count;i++)
	{
		Rsys_event* sysEvt = m_real_event_list.at (i);
		if( !sysEvt->m_bneed_confirm || sysEvt->m_event_status == eConfirmed )
			continue;
		fwrite( (SYS_EVENT*)sysEvt , sizeof(SYS_EVENT) , 1 , fp );
	}

	fclose( fp );
}

void Event_real_wnd::load_event()
{
	char filename[ 256 ];
	sprintf( filename , "%s/log/dat/revent.dat" , getenv("NBENV"));

	FILE *fp = fopen( filename , "rb" );
	if( fp == NULL ) return ;
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp) - sizeof(uint);
	if((len % sizeof(SYS_EVENT)) != 0) {
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "事项结构版本不一致，放弃装载未处理事项!" ), QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "事项结构版本不一致，放弃装载未处理事项!" ) );
		fclose(fp);
		return;
	}

	fseek(fp, 0, SEEK_SET);
	uint count=0;
	fread( &count , sizeof(count) , 1 , fp );
	if( count == 0 )
	{
		fclose( fp );
		return ;
	}

	SYS_EVENT e;
	for( int i = 0 ; i < count ; i++ )
	{
		memset( &e , 0 , sizeof(SYS_EVENT) );
		fread( &e , sizeof(SYS_EVENT) , 1 , fp );

		Rsys_event* pEvent = new Rsys_event(&e);
		pEvent->m_bneed_confirm = m_db_para.m_bconfirm_event_type( pEvent->type );

		// 需要显示的
		if(pEvent->DispOut)
		{
			bool bAdd = m_bnew_event( pEvent);
			if( bAdd )
			{
				TRY_LOCK;
				m_real_event_list.appendAndMatch( pEvent );
			}
		}
	}

	fclose( fp );

	{
		TRY_LOCK;
		statics_events();
	}
}

bool Event_real_wnd::m_bnew_event( Rsys_event*eve )
{
	if(m_event_cfg.get_show_unconfirm()) 
	{
		if(!eve->m_bneed_confirm || eve->m_event_status == eConfirmed )
			return false;
	}
	
	Event_table *pTable, *pCurTable = NULL;
	uint i;	
	uint pageid = -1;

	// 追加事项
	bool bAppend = false;
	// 页面模式
	for( i = 0; i < m_page_list.count(); i++ )
	{
		if( !m_event_cfg.get_show_overhaul_event() && ( eve->val3 == 1 || eve->val3 == 2 ) )
		{
			continue;
		}

		pTable = m_page_list.at( i );
		if( !pTable->is_select( eve ) )
			continue;

		if( m_tab_widget )
		{
// 			if( m_tabWidget->currentPage() != pTable && !pTable->m_haveNewEvent )
			if( m_tab_widget->currentWidget() != pTable && !pTable->m_bhave_new_event )
			{
				while( pTable->m_lock != 0 )
					;
				pTable->m_lock = 1;
#ifdef _WIN32
// 				m_tabWidget->changeTab( pTable, m_newEventIconSet,pTable->get_page_name() );
				int index = m_tab_widget->indexOf(pTable);						//modify by chenkai
				m_tab_widget->setTabText( index, pTable->get_page_name() );
				m_tab_widget->setTabIcon( index, m_new_event_icon );
#else
// 				m_tabWidget->setTabIconSet( pTable, m_newEventIconSet );
				int index = m_tab_widget->indexOf(pTable);						//modify by chenkai
				m_tab_widget->setTabText( index, pTable->get_page_name() );
				m_tab_widget->setTabIcon( index, m_new_event_icon );
#endif
				pTable->m_bhave_new_event = true;
 				_delay_time( 10 * 1000 );
 				pTable->m_lock = 0;
			}
		}

		pTable->add_event( eve );
		bAppend = true;
	}
	// 唤起主窗口
	if( bAppend  && isMinimized())
	{
#ifdef _WIN32
		ShowWindow(winId(), SW_RESTORE);
#else
		showNormal();
#endif
	}

	// 窗口模式
	for( i = 0; i < m_wnd_list.count(); i ++ )
	{
		pTable = m_wnd_list.at( i );
		if( !pTable->is_select( eve ) )
			continue;

		pTable->add_event( eve );

		// 唤起子窗口
		if( pTable->isHidden())
			pTable->showNormal();

		if( pTable->isMinimized() )
		{
#ifdef _WIN32
			ShowWindow(pTable->winId(), SW_RESTORE);
#else
			pTable->showNormal();
#endif
		}
		bAppend = true;
	}

	return bAppend;
}

void Event_real_wnd::refresh_table()
{
	Event_table *pTable;
	uint i;
	for( i = 0; i < m_page_list.count(); i++ )
	{
		pTable = m_page_list.at( i );
		pTable->refresh_table();
	}

	for( i = 0; i < m_wnd_list.count(); i++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->refresh_table();
	}
}

void Event_real_wnd::slot_confirm_event()
{
	if( !m_cur_user.entconfirm )
	{
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
		return;
	}

	//取得当前页
	Event_table *pTable = get_foucs_table();
	if( pTable == NULL )
	{
		if( m_tab_widget == NULL || 
		( m_tab_widget != NULL && m_tab_widget->count() == 0 ) )
		{
// 			pTable = (QEventTable *)centralWidget();
			pTable = (Event_table *)(m_splitter->widget( 0 ));
		}
		else
		{
// 			pTable = (QEventTable *)m_tabWidget->currentPage();
			pTable = (Event_table *)m_tab_widget->currentWidget();
		}
	}

	if( pTable == NULL )
		return;
	
	Sys_event_list eventlist;

	//取得选中事项
	if( !pTable->get_selected_event( eventlist ) )
		return;

	//向其他机器发送事项同步报文
	send_msg( eventlist );
	//确认事项
	confirm_event( eventlist );
}

void Event_real_wnd::onclose()
{
	EventProc evtproc;
	SYS_EVENT	sysevent;
	SYS_TIME	tmpTime;
	NetStatus ns;
	memset( &sysevent, 0, sizeof( SYS_EVENT ) );
	sysevent.grpno = ns.GetHostGrpNo( ns.GetHostNo() );

	sysevent.sort  = ENTCLASS_OPERATION ;
	sysevent.type  = ENTTYPE_SCADA_LOGIN ;
	sysevent.DiskOut = 1;
	sysevent.DispOut = 1;
	TimeProc tmpTimeProc;
	tmpTimeProc.GetTime( tmpTime );
	sysevent.ymd = tmpTime.year * 10000 + tmpTime.mon * 100 + tmpTime.day;
	sysevent.hmsms = tmpTime.hour * 10000000 + tmpTime.min * 100000 + 
		tmpTime.sec * 1000 + tmpTime.ms;

	strcpy( sysevent.Object , m_cur_user.username );
	sprintf(sysevent.text,"%s在%s节点退出智能告警",m_cur_oper_man.describe, 
			ns.GetNodeName( sysevent.grpno, ns.GetHostName( ns.GetHostNo() ) ) );
	evtproc.Save( &sysevent, 1 );
	
	slot_stop_thread();
	/*if( m_print.running() )
	{
		m_print.stop();
		m_print.wait();
		if( m_print.running() )
			m_print.terminate();
	}*/

	unreg_proc();

	if( m_event_cfg.get_dgb_out() )
		save_event();

	m_real_event_list.clear();

#if defined (WIN32)
		unsigned long block_flag = FALSE;
		ioctlsocket( m_event_confirm_socket, FIONBIO, &block_flag );
		closesocket( m_event_confirm_socket );
#elif defined (__unix)
		::fcntl( m_event_confirm_socket, F_SETFL, 0 );
		::close( m_event_confirm_socket );
#endif
		
	char filename[ 256 ];
	sprintf( filename , "%s/resource/xml/realalarmuser.xml" , getenv("NBENV") );
	if( QFile::exists( tr( filename ) ) )
	{
		QFile::remove( tr( filename ) );
	}

}

// netbus终止守候进程时给主窗口发送WM_CLOSE，该进程必须退出
void Event_real_wnd::closeEvent ( QCloseEvent * e )
{
//#ifdef TONG_LU
// 	if( m_bcloseWnd ) 
// 	{
		onclose();
		QMainWindow::closeEvent( e );
// 	}
// 	else 
// 		showMinimized();
/*#else
	onclose();
	QMainWindow::closeEvent( e );
#endif*/
}

void Event_real_wnd::slot_exit()
{
	m_bclose_wnd = true;
	QMainWindow::close();
}

void Event_real_wnd::showEvent( QShowEvent * e)
{
#ifdef _WIN32
	SetForegroundWindow(winId());
#endif
}

void Event_real_wnd::del_page( uint index )
{
	if( index >= m_page_list.count() )
		return;

	Event_table *pTable = m_page_list.at( index );

// 	m_pageList.remove( index );						//modify by chenkai autodelete
	delete m_page_list.takeAt(index);

	if( m_tab_widget && m_tab_widget->count() > 0 )
	{
// 		m_tabWidget->removePage( ( QWidget* )pTable );
		int tabIndex = m_tab_widget->indexOf(pTable);
		m_tab_widget->removeTab( tabIndex );
	}
// 	delete pTable;
	

	if( m_page_list.count() == 1 )
	{
// 		disconnect( m_tabWidget, SIGNAL( currentChanged ( QWidget * ) ), this, SLOT( onShowPage( QWidget * ) ) );			//add by chenkai 2013年3月14日 20:35:33
		m_tab_widget->hide();
		pTable = m_page_list.at(0);
		m_tab_widget->removeTab(m_tab_widget->indexOf(pTable)) ;
// 		m_tabWidget->removePage( (QWidget*)pTable );
// 		pTable->reparent( m_splitter, QPoint( 0, 0 ),true );
		pTable->setParent( m_splitter);
		pTable->move(QPoint( 0, 0 )) ;
		pTable->show();
		delete m_tab_widget;
		m_tab_widget = NULL;
// 		m_tabWidget->hide();
// 		pTable = m_pageList.at( 0 );
// 		m_tabWidget->removePage( (QWidget*)pTable );
// 		pTable->reparent( m_splitter, QPoint( 0, 0 ) );
// 		pTable->show();
// 		delete m_tabWidget;
// 		m_tabWidget = NULL;
	}
}

void Event_real_wnd::del_wnd( uint index )
{
	if( index >= m_wnd_list.count() )
		return;
	
// 	m_wndList.remove( index );
	delete m_wnd_list.takeAt(index);
}

void Event_real_wnd::slot_confirm_all_event()
{
	if( !m_cur_user.entconfirm )
	{
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ),QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无事项确认权限!" ) );
		return;
	}
	
	//取得要当前页
	Event_table *pTable = get_foucs_table();
	if( pTable == NULL )
	{
		if( m_tab_widget == NULL ||  
		( m_tab_widget != NULL && m_tab_widget->count() == 0 ) )
		{
			pTable = (Event_table *)(m_splitter->widget( 0 ));
		}
		else
		{
// 			pTable = (QEventTable *)m_tabWidget->currentPage();
			pTable = (Event_table *)m_tab_widget->currentWidget();
		}
	}

	if( pTable == NULL )
		return;

	Sys_event_list eventlist;

	//取得当前页没有确认事项列表
	eventlist = collect_unconfirmed(pTable->m_event_list);

	//向其他机器发送事项同步报文
	send_msg( eventlist );

	//确认事项
	confirm_event( eventlist );
//	confirmEvent( m_eventMsg.m_eventList );
}

void Event_real_wnd::slot_delete_event()
{	
	if( !m_cur_user.modifytable )
	{
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无删除事项权限!" ),QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无删除事项权限!" ) );
		return;
	}
	//取得当前页
	Event_table *pTable = get_foucs_table();
	if( pTable == NULL )
	{
		if( m_tab_widget == NULL || 
		( m_tab_widget != NULL && m_tab_widget->count() == 0 ) )
		{
// 			pTable = (QEventTable *)centralWidget();
			pTable = (Event_table *)(m_splitter->widget( 0 ));
		}
		else
		{
// 			pTable = (QEventTable *)m_tabWidget->currentPage();
			pTable = (Event_table *)m_tab_widget->currentWidget();
		}
	}

	if( pTable == NULL )
		return;
	
	Sys_event_list eventlist;

	//取得选中事项
	if( !pTable->get_selected_event( eventlist ) )
		return;

	//删除事项
	delete_event( eventlist );
}

bool Event_real_wnd::show_dock_menu( const QPoint & globalPos )
{
	return true;
}

void Event_real_wnd::slot_pause_btn()
{
	if( !m_read_event_proc.is_pause() )
	{
		if(DMessageBox::question( this, tr( "智能告警提示" ), tr( "确认要暂停事项刷新吗?" ),
			QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		{
			m_bpause = false;
			m_tool_button_frame->set_pause_checked(m_bpause);
			return;
		}
	}
	m_bpause = !m_bpause;
	m_read_event_proc.pause();
	m_tool_button_frame->set_pause_checked(m_bpause);
}

void Event_real_wnd::read_ini()
{
	QString filename;

	
	char * env = getenv("NBENV");
	filename.sprintf( "%s/resource/xml/realalarm.xml", (env) );

	QFile iniFile( filename );
	QDomDocument domdoc;

	if( !QFile::exists( filename ) )
		return ;

// 	if( !iniFile.open( IO_ReadOnly ) )
	if( !iniFile.open( QIODevice::ReadOnly ) )
	{
		return;
	}

	domdoc.setContent( &iniFile );
	iniFile.close();

	QDomElement root = domdoc.documentElement();

	QDomNode node = root.firstChild();
	QDomElement body;
	QDomNode subnode,subsubnode;
	QString attrivalue, attriname;
	QString desc;
// 	QDomNodeList	nodelist;
	Event_table		*pTable;
	uint i,j;

	
	while( !node.isNull() )
	{
		//系统配置
		if ( node.isElement() && node.nodeName() == 
			 tr( "SysConfig" )  ) 
		{
			for( subnode = node.firstChild(); !subnode.isNull(); 
				subnode = subnode.nextSibling() )
			{
				if( subnode.nodeName() == tr( "ToneAlarm"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_tone( true );
					}
					else
					{
						m_event_cfg.set_tone( false );
					}
				}
				else if( subnode.nodeName() == tr( "ToneDev" ) &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_tone_dev( subnode.firstChild().nodeValue() );
				}
				else if( subnode.nodeName() == tr( "KeepUnConfirmedEvent"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_keep_unconfirmed_event( true );
					}
					else
					{
						m_event_cfg.set_keep_unconfirmed_event( false );
					}
				}
				else if( subnode.nodeName() == tr( "ShowUnConfirm"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_show_unconfirm( true );
					}
					else
					{
						m_event_cfg.set_show_unconfirm( false );
					}
				}
				else if( subnode.nodeName() == tr( "AutoConfirmDel"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_auto_confirm_del( true );
					}
					else
					{
						m_event_cfg.set_auto_confirm_del( false );
					}
				}
				else if( subnode.nodeName() == tr( "AutoMatch"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						Rsys_event::g_cfg_need_match = true;
						m_event_cfg.set_auto_match( true );
					}
					else
					{
						Rsys_event::g_cfg_need_match = false;
						m_event_cfg.set_auto_match( false );
					}
				}
				else if( subnode.nodeName() == tr( "DebugOut"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_dbg_out( true );
					}
					else
					{
						m_event_cfg.set_dbg_out( false );
					}
				}
				else if( subnode.nodeName() == tr( "ShowJXEvent"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_show_overhaul_event( true );
					}
					else
					{
						m_event_cfg.set_show_overhaul_event( false );
					}
				}
				else if( subnode.nodeName() == tr( "EventNum"	) &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_max_event_num( subnode.firstChild().nodeValue().toUInt() );
				}
// #ifdef _WIN32
				else if( subnode.nodeName() == tr( "SendMsg"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_send_msg( true );
					}
					else
					{
						m_event_cfg.set_send_msg( false );
					}
				}
				// 接收短信 [7/10/2012 ljs116]
				else if( subnode.nodeName() == tr( "RcvMsg"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_rcv_msg( true );
					}
					else
					{
						m_event_cfg.set_rcv_msg( false );
					}
				}

				else if ( subnode.nodeName() == tr( "MsgConfirmTime") &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_msg_confirm_time( subnode.firstChild().nodeValue().toUInt() );
				}

				else if( subnode.nodeName() == tr( "SerialNo"	) &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_serial_no( subnode.firstChild().nodeValue());
				}
// #endif
				else if( subnode.nodeName() == tr( "Font"	) &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_font_size( subnode.firstChild().nodeValue().toUInt() );
				}
				else if( subnode.nodeName() == tr( "EventPrint"	) &&
					subnode.firstChild().isText() )
				{
					if( subnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_print( true );
					}
					else
					{
						m_event_cfg.set_print( false );
					}
				}
				else if( subnode.nodeName() == tr( "EventPrinter"	) &&
					subnode.firstChild().isText() )
				{
					m_event_cfg.set_printer( subnode.firstChild().nodeValue() );
				}
			}
		}
		//分类颜色配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "EventSortColor" ) ) 
		{
			for( i = 0; i < m_db_para.get_event_sort_count(); i ++ )
			{
				if( !m_db_para.get_event_sort_desc( i, attriname ) )
					continue;

				attriname = tr( "EventSort%1" ).arg( i );
				if( node.toElement().elementsByTagName( attriname ).count() == 0 )
					continue;

				attrivalue = node.toElement().elementsByTagName( attriname ).item( 0 ).firstChild().nodeValue();
				m_event_cfg.add_sort_color( i, QColor( attrivalue.toUInt() ) );
			}
		}
		//类型颜色配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "EventTypeColor" ) ) 
		{
			body = node.toElement();
			PEVENTTYPEDEF	pEventType;

			for( i = 0; i < m_db_para.get_event_type_count(); i ++ )
			{
				pEventType = m_db_para.get_event_type( i );

				attriname = tr( "EventType%1" ).arg( pEventType->type );

				if( node.toElement().elementsByTagName( attriname ).count() == 0 )
					continue;

				attrivalue = node.toElement().elementsByTagName( attriname ).item( 0 ).firstChild().nodeValue();
				m_event_cfg.add_type_color( pEventType->type, QColor( attrivalue.toUInt() ) );
			}
		}

		//厂站颜色配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "EventStationColor" ) ) 
		{
			body = node.toElement();
			QStringList stationnames;
			uint	groupno;

			for( i = 0; i < m_db_para.get_group_count(); i ++ )
			{
				if( !m_db_para.get_group_no( i, groupno ) )
				{
					continue;
				}

				m_db_para.get_station_code( groupno, stationnames );
				for( j = 0; j < stationnames.count(); j ++ )
				{
					if( !m_db_para.get_station_desc( stationnames[ j ], attriname ) )
					{
						continue;
					}

					if( node.toElement().elementsByTagName( attriname ).count() == 0 )
						continue;

					attrivalue = node.toElement().elementsByTagName( attriname ).item( 0 ).firstChild().nodeValue();
					m_event_cfg.add_rtu_color( stationnames[ j ], QColor( attrivalue.toUInt() ) );
				}
			}
		}

		//级别颜色配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "EventLevelColor" ) ) 
		{
			body = node.toElement();
			QStringList stationnames;
			uint	groupno;

			for( i = 0; i < 8; i ++ )
			{
				attriname = tr( "Level%1" ).arg( i );

				if( node.toElement().elementsByTagName( attriname ).count() == 0 )
					continue;

				attrivalue = node.toElement().elementsByTagName( attriname ).item( 0 ).firstChild().nodeValue();
				QRgb tmp = attrivalue.toUInt();
				QColor tmpcolor;
				tmpcolor.setRgb( tmp );
				m_event_cfg.set_level_color( i, tmpcolor );
			}
		}

		//列显示配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "ColDisplay" ) ) 
		{
			uint dispno;
			for( dispno = 0, subnode = node.firstChild(); !subnode.isNull(); 
				subnode = subnode.nextSibling(), dispno ++ )
			{
				int index = m_event_cfg.get_colid_by_colname( subnode.nodeName() );
				if( index < 0 )
					continue;

				m_event_cfg.set_col_disp_no( index, dispno );

				if( subnode.firstChild().nodeValue() == tr( "show" ) )
				{
					m_event_cfg.set_col_disp( subnode.nodeName(), true );
				}
				else
				{
					m_event_cfg.set_col_disp( subnode.nodeName(), false );
				}
			}
		}
		else if ( node.isElement() && node.nodeName() == 
			tr( "过滤事项" ) ) 
		{
			body = node.toElement();
		}
		else if ( node.isElement() && node.nodeName() == 
			tr( "过滤厂站" ) ) 
		{
			body = node.toElement();
		}
		//页配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "RealEventWnd" ) ) 
		{
			body = node.toElement();
			QDomNode wndnode,pagenode,eventnode,leafnode,subleafnode;
			EVENTTYPEDEF eventdef;

			for( pagenode = body.firstChild(); !pagenode.isNull() ;
				pagenode = pagenode.nextSibling() )
			{
				if( pagenode.nodeName() == tr( "page" ) )
				{
					if( m_tab_widget == NULL )
					{
						pTable = new Event_table( &m_event_cfg, &m_db_para, m_splitter, _Q2C(pagenode.nodeName()) );
					}
					else
					{
						pTable = new Event_table( &m_event_cfg, &m_db_para, m_tab_widget, _Q2C(pagenode.nodeName() ) );
					}
					QFont tmpFont = pTable->font();
					tmpFont.setPixelSize( m_event_cfg.get_font_size() );
					pTable->setFont( tmpFont );
					m_page_count++;
					pTable->set_page_index(m_page_count);
					add_page( pTable );
				}
				else if( pagenode.nodeName() == tr( "wnd" ) )
				{
					pTable = new Event_table( &m_event_cfg, &m_db_para, m_splitter, _Q2C(pagenode.nodeName()) );
					QFont tmpFont = pTable->font();
					tmpFont.setPixelSize( m_event_cfg.get_font_size() );
					pTable->setFont( tmpFont );
					pTable->setMouseTracking(true);
					m_page_count++;
					pTable->set_page_index(m_page_count);
					add_wnd( pTable );
				}
				else
				{
					continue;
				}
				for( eventnode = pagenode.firstChild(); !eventnode.isNull(); 
					eventnode = eventnode.nextSibling() )
				{
					if( eventnode.nodeName() == tr( "Event" ) )
					{
						for( leafnode = eventnode.firstChild(); !leafnode.isNull();
							leafnode = leafnode.nextSibling() )
						{
							if( leafnode.nodeName() == tr( "Sort" ) )
							{
								eventdef.sort = leafnode.firstChild().nodeValue().toInt();
							}
							else if( leafnode.nodeName() == tr( "Type" ) )
							{
								eventdef.type = leafnode.firstChild().nodeValue().toInt();
							}
						}

						pTable->add_event_type( &eventdef );
					}
					else if( eventnode.nodeName() == tr( "Station" ) )
					{
						if( m_db_para.get_station_desc( eventnode.firstChild().nodeValue(), desc ) )
							pTable->add_rtu( eventnode.firstChild().nodeValue() );
					}
					else if( eventnode.nodeName() == tr( "Level" ) )
					{
						pTable->add_event_level( eventnode.firstChild().nodeValue().toInt() );
					}
					else if( eventnode.nodeName() == tr( "name" ) )
					{
						pTable->set_page_name( eventnode.firstChild().nodeValue() );
						set_page_name( pTable, eventnode.firstChild().nodeValue() );
					}
				}
			}
		}
		//语音告警配置
		else if ( node.isElement() && node.nodeName() == 
			tr( "ToneAlarm" ) ) 
		{
			for( subnode = node.firstChild(); !subnode.isNull(); 
				subnode = subnode.nextSibling() )
			{
				attriname = subnode.nodeName().right( subnode.nodeName().length() - 9 );
				uint type = attriname.toUInt();
				if( type != 0 )
				{
					m_event_cfg.set_event_sound_count( type, 
						subnode.firstChild().nodeValue().toInt( ) );
				}
				
			}
		}
		//短信报警配置
//#ifdef _WIN32
		else if ( node.isElement() && node.nodeName() == 
			tr( "PhoneNo" ) ) 
		{
			for( subnode = node.firstChild(); !subnode.isNull(); 
				subnode = subnode.nextSibling() )
			{
				attriname = subnode.nodeName().right( subnode.nodeName().length() - 9 );
				uint type = attriname.toUInt();
				if( type != 0 )
				{
					for( subsubnode = subnode.firstChild(); !subsubnode.isNull(); subsubnode = subsubnode.nextSibling() )
					{
						if (subsubnode.isElement() && subsubnode.nodeName() == tr( "TypePhoneNo" ))
						{
							m_event_cfg.set_phone_no( type, subsubnode.firstChild().nodeValue() );
						}
						else
						{
							attriname = subsubnode.nodeName().right( subsubnode.nodeName().length() - 1 );
							m_event_cfg.set_phone_no_desc( type,attriname,subsubnode.firstChild().nodeValue() );
						}
					}
				}	
			}
		}
//#endif
		//报表打印配置
		//#ifdef _WIN32
		else if ( node.isElement() && node.nodeName() == 
			tr( "ReportList" ) ) 
		{
			body = node.toElement();
			QDomNode stationnode , typenode ;
			for( stationnode = body.firstChild() ; !stationnode.isNull() ; stationnode = stationnode.nextSibling() )
			{
				attriname = stationnode.nodeName() ;
				if ( attriname == _C2Q( "IsReportPrint") && stationnode.firstChild().isText() )
				{
					if( stationnode.firstChild().nodeValue() == tr( "yes" ) )
					{
						m_event_cfg.set_report_print( true );
					}
					else
					{
						m_event_cfg.set_report_print( false );
					}
				}

				else if ( attriname == _C2Q( "ReportPrintType") && stationnode.firstChild().isText() )
				{
					if( stationnode.firstChild().nodeValue() == tr( "1" ) )
					{
						m_event_cfg.set_report_print_type( true );
					}
					else
					{
						m_event_cfg.set_report_print_type( false );
					}
				}

				else
				{
					QStringList eventTypeList , reportNameList ;
					QString eventType , reportName ;
					eventTypeList.clear() ;
					reportNameList.clear() ;
					for ( typenode = stationnode.firstChild() ; !typenode.isNull() ; typenode = typenode.nextSibling() )
					{
						eventType = typenode.nodeName() ;
						reportName = typenode.firstChild().nodeValue() ;
						if( !eventType.isEmpty() )
						{
							eventType = eventType.mid( 9 ) ;
							eventTypeList << eventType ;
							reportNameList << reportName ;
						}
					}
					m_event_cfg.set_report_event_type( attriname, eventTypeList );
					m_event_cfg.set_report_name( attriname, reportNameList );
				}
			}
		}
		//#endif
		node = node.nextSibling();
	}
}

void Event_real_wnd::write_ini( )
{
	QString filename;
	QColor clr;
	QString desc;
	uint i, j, k;

	
	char * env = getenv("NBENV");
	filename.sprintf( "%s/resource/xml/realalarm.xml", (env) );

	QFile iniFile( filename );
// 	if( !iniFile.open( IO_WriteOnly ) )
	if( !iniFile.open( QIODevice::WriteOnly ) )
	{
		return;
	}

	QDomDocument domDoc;

	QDomProcessingInstruction prcesssInstruction = 
		domDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
	domDoc.appendChild(prcesssInstruction);

	/*QDomImplementation domImpl;
	QDomDocumentType docType = domImpl.createDocumentType( _C2Q( "EventAlarm" ), 
		_C2Q( "EventAlarm" ), _C2Q( "EventAlarm" ) );
	domDoc.appendChild(docType);*/

	QDomElement root = domDoc.createElement( tr( "EventAlarm" ) );
	domDoc.appendChild(root);

	QDomText text;
	QDomElement node,subnode,subsubnode;
	QString attrivalue, attriname;
	QDomNodeList	nodelist;

	//系统配置段
	node = domDoc.createElement( tr( "SysConfig" ) );
	node.setTagName( tr( "SysConfig" ) );
	root.appendChild( node );


	subnode = domDoc.createElement( tr( "ToneAlarm" ) );
	subnode.setTagName( tr( "ToneAlarm" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_tone() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "ToneDev" ) );
	subnode.setTagName( tr( "ToneDev" ) );
	node.appendChild( subnode );
	text = domDoc.createTextNode( m_event_cfg.get_tone_dev() );
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "KeepUnConfirmedEvent" ) );
	subnode.setTagName( tr( "KeepUnConfirmedEvent" ) );
	node.appendChild( subnode );
	if(  m_event_cfg.get_keep_unconfirmed_event() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "EventPrint" ) );
	subnode.setTagName( tr( "EventPrint" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_print() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "EventPrinter" ) );
	subnode.setTagName( tr( "EventPrinter" ) );
	node.appendChild( subnode );
	text = domDoc.createTextNode( m_event_cfg.get_printer() );
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "ShowUnConfirm" ) );
	subnode.setTagName( tr( "ShowUnConfirm" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_show_unconfirm() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "DebugOut" ) );
	subnode.setTagName( tr( "DebugOut" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_dgb_out() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "ShowJXEvent" ) );
	subnode.setTagName( tr( "ShowJXEvent" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_show_overhaul_event() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

//=================================
/*      事项确认自动删除		 */
//---------------------------------
	subnode = domDoc.createElement( tr( "AutoConfirmDel" ) );
	subnode.setTagName( tr( "AutoConfirmDel" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_auto_confirm_del() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );
//=================================

//=================================
/*      事项自动匹配			 */
//---------------------------------
	subnode = domDoc.createElement( tr( "AutoMatch" ) );
	subnode.setTagName( tr( "AutoMatch" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_auto_match() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );
//=================================

	subnode = domDoc.createElement( tr( "EventNum" ) );
	subnode.setTagName( tr( "EventNum" ) );
	node.appendChild( subnode );
	text = domDoc.createTextNode( tr( "%1" ).arg( m_event_cfg.get_max_event_num() ) );
	subnode.appendChild( text );

//#ifdef _WIN32
	subnode = domDoc.createElement( tr( "SendMsg" ) );
	subnode.setTagName( tr( "SendMsg" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_send_msg() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );
// 接收短信 [7/10/2012 ljs116]
	subnode = domDoc.createElement( tr( "RcvMsg" ) );
	subnode.setTagName( tr( "RcvMsg" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_rcv_msg() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "MsgConfirmTime" ) );
	subnode.setTagName( tr( "MsgConfirmTime" ) );
	node.appendChild( subnode );
	text = domDoc.createTextNode( tr( "%1" ).arg( m_event_cfg.get_msg_confirm_time() ) );
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "SerialNo" ) );
	subnode.setTagName( tr( "SerialNo" ) );
	node.appendChild( subnode );
	text = domDoc.createTextNode( tr( "%1" ).arg( m_event_cfg.get_serial_no() ) );
	subnode.appendChild( text );
//#endif

	subnode = domDoc.createElement( tr( "Font" ) );
	subnode.setTagName( tr( "Font" ) );
	node.appendChild( subnode );
	uint size = m_event_cfg.get_font_size();
	text = domDoc.createTextNode( tr( "%1").arg(size ) );
	subnode.appendChild( text );

	subnode = domDoc.createElement( tr( "EventColor" ) );
	subnode.setTagName( tr( "EventColor" ) );
	node.appendChild( subnode );
	uint clrtype = m_event_cfg.get_color_type();
	switch( clrtype )
	{
	case EVENTSORTCOLOR:
		{
			text = domDoc.createTextNode( tr( "SortColor" ) );
		}
		break;
	case EVENTTYPECOLOR:
		{
			text = domDoc.createTextNode( tr( "TypeColor" ) );
		}
		break;
	case EVENTRTUCOLOR:
		{
			text = domDoc.createTextNode( tr( "StationColor" ) );
		}
		break;
	case EVENTLEVELCOLOR:
		{
			text = domDoc.createTextNode( tr( "LevelColor" ) );
		}
		break;
	}
	subnode.appendChild( text );

	//事项分类颜色
	node = domDoc.createElement( tr( "EventSortColor" ) );
	node.setTagName( tr( "EventSortColor" ) );
	root.appendChild( node );

	for( i = 0; i < m_db_para.get_event_sort_count(); i ++ )
	{
		if( !m_event_cfg.get_sort_color( i, clr ) )
			continue;

		desc = tr( "EventSort%1" ).arg( i ); 
		subnode = domDoc.createElement( desc );
		subnode.setTagName( desc );
		node.appendChild( subnode );

		text = domDoc.createTextNode( tr( "%1" ).arg( clr.rgb() ) );
		subnode.appendChild( text );
	}

	//事项类型颜色
	node = domDoc.createElement( tr( "EventTypeColor" ) );
	node.setTagName( tr( "EventTypeColor" ) );
	root.appendChild( node );

	uint types[ 1000 ];
	uint typenum;
	for( i = 0; i < m_db_para.get_event_sort_count(); i ++ )
	{
		m_db_para.get_event_types( i, types, typenum );

		if( typenum == 0 )
			continue;

		for( j = 0 ; j < typenum; j ++ )
		{
			if( !m_event_cfg.get_type_color( types[ j ], clr ) )
				continue;
			
			desc = tr( "EventType%1" ).arg( types[ j ] ); 
			subnode = domDoc.createElement( desc );
			subnode.setTagName( desc );
			node.appendChild( subnode );
			text = domDoc.createTextNode( tr( "%1" ).arg( clr.rgb() ) );
			subnode.appendChild( text );
		}
	}

	//事项厂站颜色
	node = domDoc.createElement( tr( "EventStationColor" ) );
	node.setTagName( tr( "EventStationColor" ) );
	root.appendChild( node );

	uint no;
	QStringList rtucodes;
	for( i = 0; i < m_db_para.get_group_count(); i ++ )
	{
		if( !m_db_para.get_group_no( i, no ) )
			continue;

		rtucodes.clear();
		if( !m_db_para.get_station_code( no, rtucodes ) )
			continue;

		for( j = 0; j < rtucodes.count(); j ++ )
		{
			if( !m_db_para.get_station_desc( rtucodes[ j ], 
				desc ) )
				continue;

			if( !m_event_cfg.get_rtu_color( rtucodes[ j ], clr ) )
				continue;

			subnode = domDoc.createElement( desc );
			subnode.setTagName( desc );
			node.appendChild( subnode );
			text = domDoc.createTextNode( tr( "%1" ).arg( clr.rgb() ) );
			subnode.appendChild( text );
		}
	}

	//事项级别颜色
	node = domDoc.createElement( tr( "EventLevelColor" ) );
	node.setTagName( tr( "EventLevelColor" ) );
	root.appendChild( node );

	for( i = 0; i < 8; i ++ )
	{
		m_event_cfg.get_level_color( i, clr );

		desc = tr( "Level%1" ).arg( i );

		subnode = domDoc.createElement( desc );
		subnode.setTagName( desc );
		node.appendChild( subnode );
		text = domDoc.createTextNode( tr( "%1" ).arg( clr.rgb() ) );
		subnode.appendChild( text );
	}

	//列显示
	node = domDoc.createElement( tr( "ColDisplay" ) );
	node.setTagName( tr( "ColDisplay" ) );
	root.appendChild( node );

	uint index;
	for( i = 0; i < LIST_COL_NUM; i ++ )
	{
		if( !m_event_cfg.get_col_id( i, index ) )
			continue;

		desc = m_event_cfg.get_col_name( i );
		subnode = domDoc.createElement( desc );
		subnode.setTagName( desc );
		node.appendChild( subnode );

		if( m_event_cfg.colIs_disp( index ) )
		{
			text = domDoc.createTextNode( tr( "show" ) );
		}
		else
		{
			text = domDoc.createTextNode( tr( "hide" ) );
		}
		subnode.appendChild( text );
	}

	//实时事项窗口
	node = domDoc.createElement( tr( "RealEventWnd" ) );
	node.setTagName( tr( "RealEventWnd" ) );
	root.appendChild( node );

	QDomElement	pagenode, tmpnode, tmpnode1,tmpnode2;
	Event_table *pTable;
	EVENTTYPEDEF event;

	for( j = 0; j < m_page_list.count(); j ++ )
	{
		pagenode = domDoc.createElement( tr( "page" ) );
		node.appendChild( pagenode );
		pTable = m_page_list.at( j );

		tmpnode = domDoc.createElement( tr( "name" ) );
		tmpnode.setTagName( tr( "name" ) );
		pagenode.appendChild( tmpnode );

		desc = pTable->get_page_name();
		text = domDoc.createTextNode( desc );
		tmpnode.appendChild( text );

		for( k = 0; k < pTable->get_event_type_num(); k ++ )
		{
			if( !pTable->get_event_type( k, event ) )
				continue;

			tmpnode = domDoc.createElement( tr( "Event" ) );
			tmpnode.setTagName( tr( "Event" ) );
			pagenode.appendChild( tmpnode );

			tmpnode1 = domDoc.createElement( tr( "Sort" ) );
			tmpnode1.setTagName( tr( "Sort" ) );
			tmpnode.appendChild( tmpnode1 );

			/*if( !m_dbPara.getEventSortDesc( event.sort, desc ) )
				continue;*/
			desc.sprintf( "%d", event.sort );
			text = domDoc.createTextNode( desc );
			tmpnode1.appendChild( text );

			tmpnode1 = domDoc.createElement( tr( "Type" ) );
			tmpnode1.setTagName( tr( "Type" ) );
			tmpnode.appendChild( tmpnode1 );

			desc.sprintf( "%d", event.type );
			text = domDoc.createTextNode( desc );
			tmpnode1.appendChild( text );
		}

		for( k = 0; k < pTable->get_rtu_num(); k ++ )
		{

			tmpnode = domDoc.createElement( tr( "Station" ) );
			tmpnode.setTagName( tr( "Station" ) );
			pagenode.appendChild( tmpnode );

			text = domDoc.createTextNode( pTable->get_rtu_code( k ) );
			tmpnode.appendChild( text );
		}

		for( k = 0; k < pTable->get_event_level_num(); k ++ )
		{

			tmpnode = domDoc.createElement( tr( "Level" ) );
			tmpnode.setTagName( tr( "Level" ) );
			pagenode.appendChild( tmpnode );

			text = domDoc.createTextNode( tr("%1").arg( pTable->get_event_level( k ) ) );
			tmpnode.appendChild( text );
		}
	}

	for( j = 0; j < m_wnd_list.count(); j ++ )
	{
		pagenode = domDoc.createElement( tr( "wnd" ) );
		node.appendChild( pagenode );
		pTable = m_wnd_list.at( j );

		tmpnode = domDoc.createElement( tr( "name" ) );
		tmpnode.setTagName( tr( "name" ) );
		pagenode.appendChild( tmpnode );

		desc = pTable->get_page_name();
		text = domDoc.createTextNode( desc );
		tmpnode.appendChild( text );

		for( k = 0; k < pTable->get_event_type_num(); k ++ )
		{
			if( !pTable->get_event_type( k, event ) )
				continue;

			tmpnode = domDoc.createElement( tr( "Event" ) );
			tmpnode.setTagName( tr( "Event" ) );
			pagenode.appendChild( tmpnode );

			tmpnode1 = domDoc.createElement( tr( "Sort" ) );
			tmpnode1.setTagName( tr( "Sort" ) );
			tmpnode.appendChild( tmpnode1 );

			/*if( !m_dbPara.getEventSortDesc( event.sort, desc ) )
				continue;*/
			desc.sprintf( "%d", event.sort );
			text = domDoc.createTextNode( desc );
			tmpnode1.appendChild( text );

			tmpnode1 = domDoc.createElement( tr( "Type" ) );
			tmpnode1.setTagName( tr( "Type" ) );
			tmpnode.appendChild( tmpnode1 );

			desc.sprintf( "%d", event.type );
			text = domDoc.createTextNode( desc );
			tmpnode1.appendChild( text );
		}

		for( k = 0; k < pTable->get_rtu_num(); k ++ )
		{

			tmpnode = domDoc.createElement( tr( "Station" ) );
			tmpnode.setTagName( tr( "Station" ) );
			pagenode.appendChild( tmpnode );

			text = domDoc.createTextNode( pTable->get_rtu_code( k ) );
			tmpnode.appendChild( text );
		}

		for( k = 0; k < pTable->get_event_level_num(); k ++ )
		{

			tmpnode = domDoc.createElement( tr( "Level" ) );
			tmpnode.setTagName( tr( "Level" ) );
			pagenode.appendChild( tmpnode );

			text = domDoc.createTextNode( tr("%1").arg( pTable->get_event_level( k ) ) );
			tmpnode.appendChild( text );
		}
	}

	//循环语音报警
	node = domDoc.createElement( tr( "ToneAlarm" ) );
	node.setTagName( tr( "ToneAlarm" ) );
	root.appendChild( node );
	PEVENTTYPEDEF pEventDef;
	for( i = 0; i < m_db_para.get_event_type_count(); i ++ )
	{
		pEventDef = m_db_para.get_event_type( i );
		desc = tr( "EventType%1" ).arg( pEventDef->type );
		subnode = domDoc.createElement( desc );
		subnode.setTagName( desc );
		node.appendChild( subnode );

		desc = tr( "%1" ).arg( m_event_cfg.get_event_sound_count( pEventDef->type ) );
		
		text = domDoc.createTextNode( desc );
		subnode.appendChild( text );
	}

	//短信报警号码
	node = domDoc.createElement( tr( "PhoneNo" ) );
	node.setTagName( tr( "PhoneNo" ) );
	root.appendChild( node );
	for( i = 0; i < m_db_para.get_event_type_count(); i ++ )
	{
		pEventDef = m_db_para.get_event_type( i );
		desc = tr( "EventType%1" ).arg( pEventDef->type );
		subnode = domDoc.createElement( desc );
		subnode.setTagName( desc );
		node.appendChild( subnode );

		QString phonestr = m_event_cfg.get_type_phone_no( pEventDef->type );
		
		subsubnode = domDoc.createElement( tr( "TypePhoneNo" ) );
		subsubnode.setTagName( tr( "TypePhoneNo" ) );
		subnode.appendChild( subsubnode );
		text = domDoc.createTextNode( phonestr );
		subsubnode.appendChild( text );

		//增加子节点  各号码筛选对象名
		QStringList phonelist = phonestr.split(",");	//取该类型所有号码
		if (phonestr.isEmpty())
		{
			continue;
		}

		QMap<int,Event_cfg::PhoneDescMap> typeNomap = m_event_cfg.get_type_to_no_map();
		Event_cfg::PhoneDescMap phonedescmap = typeNomap[pEventDef->type];

		QString descstr;
		for (int phonenum = 0;phonenum < phonelist.size(); phonenum++)
		{
			descstr.clear();
			QString phonenumber = phonelist.at(phonenum);
			descstr = phonedescmap[phonenumber];

			desc = tr( "N%1" ).arg( phonenumber );
			subsubnode = domDoc.createElement( desc );
			subsubnode.setTagName( desc );
			subnode.appendChild( subsubnode );

			text = domDoc.createTextNode( descstr );
			subsubnode.appendChild( text );
		}
	}

	//报表打印列表
	node = domDoc.createElement( tr( "ReportList" ) );
	node.setTagName( tr( "ReportList" ) );
	root.appendChild( node );

	//增加是否打印
	subnode = domDoc.createElement( tr( "IsReportPrint" ) );
	subnode.setTagName( tr( "IsReportPrint" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_report_print() )
	{
		text = domDoc.createTextNode( tr( "yes" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "no" ) );
	}
	subnode.appendChild( text );

	//增加横向打印
	subnode = domDoc.createElement( tr( "ReportPrintType" ) );
	subnode.setTagName( tr( "ReportPrintType" ) );
	node.appendChild( subnode );
	if( m_event_cfg.get_report_print_type() )
	{
		text = domDoc.createTextNode( tr( "1" ) );
	}
	else
	{
		text = domDoc.createTextNode( tr( "0" ) );
	}
	subnode.appendChild( text );

	//增加报表打印列表
	for ( i = 0 ; i < m_event_cfg.get_station_count() ; i++ )
	{
		QStringList stationList = m_event_cfg.get_report_map().keys() ;
		QString station = stationList[ i ] ;
		subnode = domDoc.createElement( station );
		subnode.setTagName( station );
		node.appendChild( subnode );
		QStringList eventTypeList = m_event_cfg.get_report_event_type( station ) ;
		QStringList reportNameList = m_event_cfg.get_report_name( station ) ;
		for ( int j = 0 ; j < eventTypeList.count() ; j++)
		{
			desc = tr( "EventType%1" ).arg( eventTypeList[ j ] );
			subsubnode = domDoc.createElement( desc );
			subsubnode.setTagName( desc );
			subnode.appendChild( subsubnode );

			text = domDoc.createTextNode( reportNameList[ j ] );
			subsubnode.appendChild( text );
		}
	}
// 	for( i = 0; i < m_dbPara.getEventTypeCount(); i ++ )
// 	{
// 		pEventDef = m_dbPara.getEventType( i );
// 		desc = tr( "EventType%1" ).arg( pEventDef->type );
// 		subnode = domDoc.createElement( desc );
// 		subnode.setTagName( desc );
// 		node.appendChild( subnode );
// 
// 		text = domDoc.createTextNode( m_eventCfg.getPhoneNo( pEventDef->type ) );
// 		subnode.appendChild( text );
// 	}

	QTextStream out( &iniFile );
	domDoc.save( out, 4 );
	
	iniFile.close();
}

bool Event_real_wnd::reg_proc()
{
	if( m_proc_man.IsProcExist( "realalarm" ) ) 
		return false;
	
#if defined(WIN32)
	WId wid = winId();
	m_proc_man.StartProc( "realalarm", "智能告警", SYS_USER, wid );
#else
	m_proc_man.StartProc( "realalarm", "智能告警", SYS_USER );
#endif

	if( !m_net_dbg.RegProc( "智能告警" ) || 
		!m_net_dbg.RegType( RALARMDEBUG, "智能告警" ) )
	{
		return false;
	}

	return true;
}

void Event_real_wnd::unreg_proc()
{
	m_net_dbg.UnRegProc();
	m_proc_man.EndProc();
}

void Event_real_wnd::timerEvent ( QTimerEvent * e )
{
	if (m_event_msg.m_bget_msg)
	{
		confirm_msg_event(m_event_msg_list);
	}

	if( e->timerId() == m_check_timer )
	{
		if( !m_proc_man.CheckParentProc() )
		{
			statusBar()->showMessage(tr("进程退出中..."));
			m_bclose_wnd = true;
			close();
		}
	}
	else if( e->timerId() == m_recv_data_timer )
	{
		//	onReadData();
		TRY_LOCK;
		statics_events();
	}
	else if( e->timerId() == m_del_event_timer )
	{
		if( m_bpause )
			return;
		roll_event_list();
	}
	else if(e->timerId() == m_print_event_timer) {
		if(m_event_cfg.get_print())
		{
			m_print.print();
		}
	}
}

void Event_real_wnd::statics_events()
{
	uint max = m_event_cfg.get_max_event_num();
	uint nRemovable = count_removable_events(m_real_event_list);
	uint nUnConfirmed = count_unconfirmed(m_real_event_list);
	uint nConfirmed = count_confirmed(m_real_event_list);
	uint nCount = m_real_event_list.count();
// 	m_status_label->setText( tr( "<html><head/><body><p>目前事项共计%1条：<span style='color:#eb5945;'>已确认</span>事项%2条,<span style=' color:#67c60b;'>未确认</span>事项%3条,其他事项%4条。</p></body></html>" )
// 		.arg(nCount)
// 		.arg( nConfirmed )
// 		.arg(nUnConfirmed)
// 		.arg(nCount-nConfirmed-nUnConfirmed)
// 		);
	m_status_label->setText( tr( "目前事项共计%1条：已确认事项%2条，需要确认事项%3条，其他事项%4条。当前用户 : %5" )
		.arg(nCount)
		.arg(nConfirmed)
		.arg(nUnConfirmed)
		.arg(nCount-nConfirmed-nUnConfirmed)
		.arg( tr( m_cur_oper_man.describe ) ) );

// 	statusBar()->showMessage(tr("目前事项共计%1条：已确认事项%2条，需要确认事项%3条，其他事项%4条。当前用户 : %5")
// 		.arg(nCount)
// 		.arg(nConfirmed)
// 		.arg(nUnConfirmed)
// 		.arg(nCount-nConfirmed-nUnConfirmed)
// 		.arg( tr( m_cur_oper_man.describe ) ) );
}

void Event_real_wnd::roll_event_list()
{
	TRY_LOCK;
	uint max = m_event_cfg.get_max_event_num();
	uint c = count_removable_events(m_real_event_list);
	uint nCount = m_real_event_list.count();

	if( nCount < max )
		return;

	if( nCount - c  > max )
	{
		//uint nRemovable = CountRemovableEvents(m_realEventList);
		uint nUnConfirmed = count_unconfirmed(m_real_event_list);
		uint nConfirmed = count_confirmed(m_real_event_list);	
		statusBar()->showMessage(tr("目前事项共计%1条：已确认事项%2条，需要确认事项%3条，其他事项%4条。"
				"当前用户 : %5 警告：事项缓冲区已到上限，请立刻确认。")
				.arg(nCount)
				.arg(nConfirmed)
				.arg(nUnConfirmed)
				.arg(nCount-nConfirmed-nUnConfirmed)
				.arg( tr( m_cur_oper_man.describe ) ) );
		if( m_event_cfg.get_keep_unconfirmed_event() && c == 0 )
			return;
	}

	uint i;
	Sys_event_list	deleventlist;
	Event_table *pTable;

	Rsys_event *pEvent;

	// 
	// 收集删除集
	//
	uint nDel;// = c - max;

	if( m_event_cfg.get_keep_unconfirmed_event() )
	{
		if( nCount - c > max )
			nDel = c;
		else 
			nDel = nCount - max;

		for( i = 0; i < nCount && nDel > 0; i++ ) 
		{
			pEvent = m_real_event_list.at( i );
			if(!pEvent->m_bneed_confirm || pEvent->m_event_status==eConfirmed) 
			{
				deleventlist.append( pEvent );
				nDel--;
			}
		}
	}
	else
	{
		for( i = 0 ; i < nCount - max ; i ++ )
		{
			pEvent = m_real_event_list.at( i );
			deleventlist.append( pEvent );
		}
	}

	if( deleventlist.count() == 0 )
	{
		return;
	}


	// 页面删除
	for( i = 0; i < m_page_list.count(); i ++ )
	{
		pTable = m_page_list.at( i );
		pTable->del_event( deleventlist );
	}

	// 子窗口删除
	for( i = 0; i < m_wnd_list.count(); i ++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->del_event( deleventlist );
	}

	// 语音报警和事项源删除
	for( i = 0;i < deleventlist.count(); i ++ )
	{
		pEvent = deleventlist.at( i );
		m_event_sound.confirm_event( pEvent ); 

//#ifdef _WIN32
		m_event_msg.confirm_event( pEvent );//短讯
//#endif
		if(pEvent->m_match)
			pEvent->m_match->m_match = NULL;
		m_real_event_list.removeOne( pEvent );
// 		int j = m_realEventList.indexOf(pEvent);
// 		if (j != -1)
// 			delete m_realEventList.takeAt(j);
	}
	statics_events();
}

void Event_real_wnd::send_msg( Sys_event_list & eventlist )
{
	sockaddr_in		addr;
	char *databuf;
	Rsys_event *pEvent;
	uint msglen = sizeof( SENDEVENTDATA ) * eventlist.count();
	QString ip;
	char *pSendPoint;
	uint sendlen;
	SENDEVENTDATA sendeve;

	databuf = new char[ msglen ];
	memset( databuf, 0 , msglen );

	uint i;
	for( i = 0; i < eventlist.count(); i ++ )
	{
		pEvent = eventlist.at( i );
		sendeve.grpno = pEvent->grpno;
		sendeve.sort = pEvent->sort;
		sendeve.type = pEvent->type;
		sendeve.ymd = pEvent->ymd;
		sendeve.hmsms = pEvent->hmsms;
		memcpy( databuf + i * sizeof( SENDEVENTDATA ), 
			&sendeve, sizeof( SENDEVENTDATA ) );
	}

	addr.sin_family = AF_INET;
	
	addr.sin_port = htons( LISTENPORT );
	for( i = 0; i < m_db_para.get_ip_num(); i ++ )
	{
		
		if( !m_db_para.get_ip_by_id( i, ip ) )
			continue;

		addr.sin_addr.s_addr = inet_addr( _Q2C( ip ) );
		pSendPoint = databuf;
		sendlen = 0;
		int ret;
		while( pSendPoint < databuf + msglen )
		{
			if( pSendPoint + 1400 < databuf + msglen )
			{
				sendlen = 1400;
			}
			else
			{
				sendlen = databuf + msglen - pSendPoint;
			}
			ret = sendto( m_event_confirm_socket, pSendPoint, sendlen, 0, 
				(struct sockaddr*)&addr, sizeof( struct sockaddr_in ) );

			pSendPoint += sendlen;
		}
		

		//printf( "send msg to IP=%s, len=%d\n", _Q2C( ip ), ret );
	}

	delete [] databuf;
}

void Event_real_wnd::on_read_data()
{
	char databuf[ 4096 ];
	uint i;
	char *pChar;

	int ret;

	struct timeval	tv;
	fd_set			rset;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO( &rset );
	FD_SET( (unsigned int)m_event_confirm_socket, &rset );
	
	ret = select( m_event_confirm_socket+1, &rset, NULL, NULL, &tv );
	
	if( ret <= 0 )
	{
		return;
	}

	memset( databuf, 0 , 4096 );

	int datanum = recv( m_event_confirm_socket, databuf, 4096, 0 );

	if( datanum <= 0 )
		return;

	for( i = 0; i < (uint)datanum; i ++ )
	{
		m_recv_data_buf[ m_write_point ] = databuf[ i ];
		m_write_point = ( m_write_point + 1 ) % DATABUFLEN;
	}

	Rsys_event *pEvent;
	SENDEVENTDATA recveve;
	Sys_event_list eventlist;

	while( ( ( m_write_point - m_read_point + DATABUFLEN ) % DATABUFLEN ) >= 
		sizeof( SENDEVENTDATA ) )
	{
		pChar = (char*)( &recveve );
		for( i = 0; i < sizeof( SENDEVENTDATA ); i ++ )
		{
			pChar[ i ] = m_recv_data_buf[ m_read_point ];
			m_read_point = ( m_read_point + 1 ) % DATABUFLEN;
		}

		for( i = 0; i < m_real_event_list.count(); i ++ )
		{
			pEvent = m_real_event_list.at( i );
			if( pEvent->grpno == recveve.grpno &&
				pEvent->sort == recveve.sort &&
				pEvent->type == recveve.type &&
				pEvent->ymd == recveve.ymd &&
				pEvent->hmsms == recveve.hmsms &&
				pEvent->m_event_status == eUnConfirmed )
			{
				eventlist.append( pEvent );
				break;
			}
		}
	}

	confirm_event( eventlist );
}

void Event_real_wnd::create_socket()
{
	sockaddr_in		addr;
	int				ret;

	m_event_confirm_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	int temp = 160000;
	setsockopt(m_event_confirm_socket, SOL_SOCKET, SO_RCVBUF,
		(char *)&temp, sizeof( temp ) );
	setsockopt(m_event_confirm_socket, SOL_SOCKET, SO_SNDBUF,
		(char *)&temp, sizeof( temp ) );

	int tmp = 1;
	setsockopt(m_event_confirm_socket,SOL_SOCKET,SO_REUSEADDR,(char *)&tmp,sizeof(tmp));
#if defined(WIN32)
	unsigned long block_flag = TRUE;
	ioctlsocket(m_event_confirm_socket,FIONBIO,&block_flag);
#elif defined (__unix)
	fcntl(m_event_confirm_socket,F_SETFL,O_NDELAY);
#endif
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY ;
	addr.sin_port = htons( LISTENPORT );

	ret = bind( m_event_confirm_socket, (struct  sockaddr*)&addr, sizeof( struct  sockaddr ) );
}

void Event_real_wnd::confirm_msg_event( Sys_event_list & eventlist )
{
	Rsys_event * pEvent;
	int		year, month, day, hour, min,sec,ms;
	int     i;
	ulong	lYmd,lHmsms;	
	char    strText[ 1024 ];

	for( i  = 0 ; i < eventlist.count(); i ++ )
	{
		// 当前事项
		pEvent = eventlist.at( i );
		lYmd = ( ulong)( pEvent->ymd );
		lHmsms = ( ulong )( pEvent->hmsms );

		year = ( int )( lYmd / ( 100l * 100l ) );
		month = ( int )( ( lYmd % ( 100l * 100l ) ) / 100 );
		day = ( int )( ( lYmd % ( 100l * 100l ) ) % 100 );
		
		hour = ( int )( lHmsms / ( 100l * 100l * 1000l ) );
		min = ( int )( ( lHmsms % ( 100l * 100l * 1000l ) ) / ( 100l * 1000l ) );
		sec = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) / 1000l );
		ms = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) % 1000l );
		
		sprintf( strText, "%04d.%02d.%02d--%02d:%02d:%02d:%03d %s", year, month, day, hour, min, sec, ms, 
			pEvent->text );
		if (m_event_msg.m_rcv_text == tr(strText))
		{
			confirm_event( eventlist );
			m_event_msg.confirm_event( pEvent );
		}
	}
}

void Event_real_wnd::confirm_event( Sys_event_list & eventlist )
{
	Rsys_event * pEvent;
	Event_table *pTable;

	bool showunconfirm = m_event_cfg.get_show_unconfirm();
	bool bNeedAutoDel = m_event_cfg.get_auto_confirm_del();
	Sys_event_list deleventlist, confirmeventlist;

	TRY_LOCK;
	uint i, j;

	// 事项集合
	typedef QMap<Rsys_event*, Rsys_event*> QSysEventSet;

	QSysEventSet candinates;
	
	// 确认原事项和相关事项
	for( i  = 0 ; i < eventlist.count(); i ++ )
	{
		// 当前事项
		pEvent = eventlist.at( i );
		candinates[pEvent] = pEvent;

		m_event_sound.confirm_event( pEvent );	

//#ifdef _WIN32
		m_event_msg.confirm_event( pEvent );
//#endif
		if( !pEvent->m_bneed_confirm || pEvent->m_event_status == eConfirmed )
			continue;

		pEvent->m_event_status = eConfirmed;

		// 找匹配的
		if(pEvent->m_match) 
		{
			int iMatch = m_real_event_list.indexOf(pEvent->m_match);
			if(iMatch != -1) {
				Rsys_event * pE = m_real_event_list.at(iMatch);
				pE->m_event_status = eConfirmed;
				candinates[pE] = pE;

				m_event_sound.confirm_event( pE );		

//#ifdef _WIN32
				m_event_msg.confirm_event( pE );
//#endif
			}
		}
	}

	// 搜集需要删除的事项
	for( i = 0; i < m_real_event_list.count() ; i ++ )
	{
		pEvent = m_real_event_list.at( i );
		if(showunconfirm)
		{
			if( !pEvent->m_bneed_confirm || pEvent->m_event_status == eConfirmed )
				deleventlist.append( pEvent );
		}
		else 
		{
			if(bNeedAutoDel) 
			{
				if(pEvent->m_bneed_confirm && pEvent->m_event_status == eConfirmed)
					deleventlist.append( pEvent );
			}
		}
	}

	// 界面确认(排除在要删除之外的)
	QList<Rsys_event*> confirm_list = candinates.keys();
	for( i  = 0 ; i < confirm_list.count(); i ++ )
	{
		pEvent = confirm_list[i];
		if( deleventlist.indexOf( pEvent ) == -1 )
		{
			for( j = 0; j < m_page_list.count(); j ++ )
			{
				pTable = m_page_list.at( j );
				pTable->confirm_event( pEvent );
			}
			for( j = 0; j < m_wnd_list.count(); j ++ )
			{
				pTable = m_wnd_list.at( j );
				pTable->confirm_event( pEvent );
			}
		}
	}

	// 界面删除
	for( i = 0; i < m_page_list.count(); i++ )
	{
		pTable = m_page_list.at( i );
		pTable->del_event( deleventlist );
	}
	for( i = 0; i < m_wnd_list.count(); i++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->del_event( deleventlist );
	}

	// 事项源删除
	for( i = 0;i < deleventlist.count(); i ++ )
	{
		pEvent = deleventlist.at( i );
		if(pEvent->m_match)
			pEvent->m_match->m_match = NULL;
		m_real_event_list.removeOne( pEvent );
// 		int j = m_realEventList.indexOf(pEvent);
// 		if (j != -1)
// 			delete m_realEventList.takeAt(j);
	}
	
	statics_events();
}

void Event_real_wnd::delete_event( Sys_event_list & eventlist )
{
	Rsys_event * pEvent;
	Event_table *pTable;

	Sys_event_list deleventlist;

	TRY_LOCK;
	uint i;

	// 搜集需要删除的事项
	for( i = 0; i < eventlist.count() ; i ++ )
	{
		pEvent = eventlist.at( i );
		if(!pEvent->m_bneed_confirm ||	(pEvent->m_bneed_confirm && pEvent->m_event_status == eConfirmed))
			deleventlist.append( pEvent );
	}

	// 界面删除
	for( i = 0; i < m_page_list.count(); i++ )
	{
		pTable = m_page_list.at( i );
		pTable->del_event( deleventlist );
	}
	for( i = 0; i < m_wnd_list.count(); i++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->del_event( deleventlist );
	}

	// 事项源删除
	for( i = 0;i < deleventlist.count(); i ++ )
	{
		pEvent = deleventlist.at( i );
		if(pEvent->m_match != NULL)
		{
 			pEvent->m_match->m_match = NULL;
		}
// 		m_realEventList.remove( pEvent );
		m_real_event_list.removeOne( pEvent ) ;
// 		int j = m_realEventList.indexOf(pEvent);
// 		if (j != -1)
// 			delete m_realEventList.takeAt(j);
	}
	
	statics_events();
}

void Event_real_wnd::read_unconfirm_event()
{
	Rdb_SqlCli  rsqlcli;

	rsqlcli.RdbSelectFrom( "notifyevtrcd" );

	rsqlcli.RdbSelectField( "groupno" );
	rsqlcli.RdbSelectField( "eventsort" );
	rsqlcli.RdbSelectField( "eventtype" );
	rsqlcli.RdbSelectField( "ymd" );
	rsqlcli.RdbSelectField( "hmsms" );
	rsqlcli.RdbSelectField( "security" );
	rsqlcli.RdbSelectField( "voltgrade" );
	rsqlcli.RdbSelectField( "testflag" );
	rsqlcli.RdbSelectField( "statusvalue" );
	rsqlcli.RdbSelectField( "statusvalue1" );
	rsqlcli.RdbSelectField( "statusvalue2" );
	rsqlcli.RdbSelectField( "floatvalue" );
	rsqlcli.RdbSelectField( "floatvalue1" );
	rsqlcli.RdbSelectField( "floatvalue2" );
	rsqlcli.RdbSelectField( "floatvalue3" );
	rsqlcli.RdbSelectField( "groupname" );
	rsqlcli.RdbSelectField( "membername0" );
	rsqlcli.RdbSelectField( "membername1" );
	rsqlcli.RdbSelectField( "membername2" );
	rsqlcli.RdbSelectField( "membername3" );
	rsqlcli.RdbSelectField( "graphname" );
	rsqlcli.RdbSelectField( "toneinfo" );
	rsqlcli.RdbSelectField( "charinfo" );

	NetStatus netstat;

	int nRet = -1;
	uint nRcdCount;
    int *host_group_no=netstat.GetHostGrpEnm();

	nRet=rsqlcli.RdbSelCmd(host_group_no[0],"600268","" );

	if (nRet==RDB_OK)
		nRcdCount=rsqlcli.RdbGetRcdCnt();
	else
		return;

	Rsys_event *pEvent;
	char *tmpstr;

	uint i;
	for( i = 0; i < nRcdCount; i ++ )
	{
		pEvent = new Rsys_event;
		pEvent->grpno = rsqlcli.RdbGetVal_int( i, "groupno" );
		pEvent->sort = rsqlcli.RdbGetVal_int( i, "eventsort" );
		pEvent->type = rsqlcli.RdbGetVal_int( i, "eventtype" );
		pEvent->ymd = rsqlcli.RdbGetVal_int( i, "ymd" );
		pEvent->hmsms = rsqlcli.RdbGetVal_int( i, "hmsms" );
		pEvent->security = rsqlcli.RdbGetVal_int( i, "security" );
		pEvent->volgrade = rsqlcli.RdbGetVal_int( i, "voltgrade" );
		pEvent->testflag = rsqlcli.RdbGetVal_int( i, "testflag" );
		pEvent->state = rsqlcli.RdbGetVal_int( i, "statusvalue" );
		pEvent->state1 = rsqlcli.RdbGetVal_int( i, "statusvalue1" );
		pEvent->state2 = rsqlcli.RdbGetVal_int( i, "statusvalue2" );
		pEvent->val = rsqlcli.RdbGetVal_double( i, "floatvalue" );
		pEvent->val1 = rsqlcli.RdbGetVal_double( i, "floatvalue1" );
		pEvent->val2 = rsqlcli.RdbGetVal_double( i, "floatvalue2" );
		pEvent->val3 = rsqlcli.RdbGetVal_double( i, "floatvalue3" );
		tmpstr = rsqlcli.RdbGetString( i, "groupname" );
		strcpy( pEvent->Object, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "membername0" );
		strcpy( pEvent->member0, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "membername1" );
		strcpy( pEvent->member1, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "membername2" );
		strcpy( pEvent->member2, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "membername3" );
		strcpy( pEvent->member3, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "graphname" );
		strcpy( pEvent->graph, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "toneinfo" );
		strcpy( pEvent->tone, tmpstr );
		tmpstr = rsqlcli.RdbGetString( i, "charinfo" );
		strcpy( pEvent->text, tmpstr );

		pEvent->m_match = NULL;
		pEvent->m_event_status = eUnConfirmed;
		pEvent->m_bneed_confirm = m_db_para.m_bconfirm_event_type( pEvent->type );

		TRY_LOCK;
		m_real_event_list.appendAndMatch( pEvent );
		if(pEvent->DispOut)
			m_bnew_event( pEvent );
	}
}

void Event_real_wnd::slot_ui_set()
{
	if( !m_cur_user.modifytable )
	{
// 		QMessageBox::message( tr( "提示" ), _C2Q( "当前用户无修改事项配置权限!" ),QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "当前用户无修改事项配置权限!" ) );
		return;
	}

// 	QEventUISetDlg dlg( this, "QEventUISetDlg", TRUE, Qt::WType_TopLevel );
	Event_ui_set_dlg dlg( this, "QEventUISetDlg", TRUE, Qt::Window );

	dlg.exec();

	uint i;
	Event_table *pTable;
	Rsys_event *pEvent;

	for( i = 0; i < m_page_list.count(); i ++ )
	{
		pTable = m_page_list.at( i );
		pTable->clear();
	}

	for( i = 0; i < m_wnd_list.count(); i ++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->clear();
	}

	TRY_LOCK;
	for( i = 0; i < m_real_event_list.count(); i ++ )
	{
		pEvent = m_real_event_list.at( i );
		if(pEvent->DispOut)
			m_bnew_event( pEvent );
	}

	write_ini();
}

void Event_real_wnd::slot_sys_set()
{
	if( !m_cur_user.modifytable )
	{
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "当前用户无修改事项配置权限!" ),QString::null, this );
		DMessageBox::information( this , tr( "提示" ), tr( "当前用户无修改事项配置权限!" ) );
		return;
	}

	QEventSysSetDlg dlg( this, "QEventSysSetDlg", TRUE, Qt::Window );

	if( dlg.exec() != QDialog::Accepted )
		return;

	if( !m_event_cfg.get_tone() && m_event_sound.isRunning() )
	{
		m_event_sound.stop();
	}
	else if( m_event_cfg.get_tone() && !m_event_sound.isRunning() )
	{
		m_event_sound.set_tone_dev( m_event_cfg.get_tone_dev() );
		m_event_sound.start();
	}

//#ifdef _WIN32
	if( !m_event_cfg.get_send_msg() && m_event_msg.isRunning() )
	{
		m_event_msg.stop();
	}
	else if( m_event_cfg.get_send_msg() && !m_event_msg.isRunning() )
	{
		m_event_msg.set_serial_no( m_event_cfg.get_serial_no() );
		m_event_msg.start();
	}
	if ( m_event_cfg.get_rcv_msg())
	{
		m_event_msg.m_brcv_msg = true;
		m_event_msg.set_msg_confirm_time(m_event_cfg.get_msg_confirm_time());
	}
	else 
	{
		m_event_msg.m_brcv_msg = false;		
	}
//#endif

/*	if( !m_eventCfg.getPrint())
	{
		if(m_print.running())
			m_print.stop();
	}
	else
	{
		if(m_print.running()) {
			m_print.stop();
			m_print.wait(1000);
			if(m_print.running())
				m_print.terminate();
		}

		if(m_print.SetPrinter( m_eventCfg.getPrinter()))
			m_print.start();
	}*/
	//m_print.stop();
	if(m_event_cfg.get_print())
		m_print.set_printer( m_event_cfg.get_printer());

	uint i;
	Event_table *pTable;
	Rsys_event *pEvent;

	for( i = 0; i < m_page_list.count(); i ++ )
	{
		pTable = m_page_list.at( i );
		pTable->clear();
	}

	for( i = 0; i < m_wnd_list.count(); i ++ )
	{
		pTable = m_wnd_list.at( i );
		pTable->clear();
	}

	bool showunconfirm = m_event_cfg.get_show_unconfirm();
	bool bNeedAutoDel = m_event_cfg.get_auto_confirm_del();
	Sys_event_list deleventlist;

	TRY_LOCK;
	// 搜集需要删除的事项
	for( i = 0; i < m_real_event_list.count() ; i ++ )
	{
		pEvent = m_real_event_list.at( i );
		if(showunconfirm) {
			if( !pEvent->m_bneed_confirm || pEvent->m_event_status == eConfirmed )
				deleventlist.append( pEvent );
		}
		else {
			if(bNeedAutoDel) {
				if(pEvent->m_bneed_confirm && pEvent->m_event_status == eConfirmed)
					deleventlist.append( pEvent );
			}
		}
	}

	// 事项源删除
	for( i = 0;i < deleventlist.count(); i ++ )
	{
		pEvent = deleventlist.at( i );
		m_event_sound.confirm_event( pEvent ); 

//#ifdef _WIN32
		m_event_msg.confirm_event( pEvent );//短讯
//#endif
		if(pEvent->m_match)
			pEvent->m_match->m_match = NULL;

		m_real_event_list.removeOne( pEvent );
// 		int j = m_realEventList.indexOf(pEvent);
// 		if (j != -1)
// 			delete m_realEventList.takeAt(j);
	}
	
	for( i = 0; i < m_real_event_list.count(); i ++ )
	{
		pEvent = m_real_event_list.at( i );
		if(pEvent->DispOut)
			m_bnew_event( pEvent);
	}
	statics_events();

	write_ini();
}

void Event_real_wnd::set_page_name( Event_table *table, const QString &title )
{
	if( m_tab_widget != NULL &&
		m_tab_widget->count() > 0 )
	{
// 		m_tabWidget->setTabLabel( table, title );
		m_tab_widget->setTabText( m_tab_widget->indexOf(table), title );
	}
	
	if( m_wnd_list.indexOf( table ) != -1 )
	{
		table->setWindowTitle( title );
	}
}

void Event_real_wnd::slot_show_page( int i )
{
	if (i >= 0)		//add by chenkai
	{
		QWidget *w = m_tab_widget->widget(i);
		if( ( ( Event_table* )w )->m_bhave_new_event )
		{
			while( ( ( Event_table* )w )->m_lock != 0 )
				;
			( ( Event_table* )w )->m_lock = 1;
		#ifdef _WIN32
		// 		m_tabWidget->changeTab( w, m_noNewEventIconSet,( ( QEventTable* )w )->get_page_name() );
			int index = m_tab_widget->indexOf(w);						//modify by chenkai
			m_tab_widget->setTabText( index, ( ( Event_table* )w )->get_page_name() );
			m_tab_widget->setTabIcon( index, m_no_new_event_icon );
		#else
// 			m_tabWidget->setTabIconSet( w, m_noNewEventIconSet );
			int index = m_tab_widget->indexOf(w);						//modify by chenkai
			m_tab_widget->setTabText( index, ( ( Event_table* )w )->get_page_name() );
			m_tab_widget->setTabIcon( index, m_no_new_event_icon );
		#endif
			( ( Event_table* )w )->m_bhave_new_event = false;
			_delay_time( 10 * 1000 );
			( ( Event_table* )w )->m_lock = 0;
		}
	}
}

void Event_real_wnd::slot_top_most()
{
// 	uint f;
// 	f = getWFlags();
// 	if( testWFlags( WStyle_StaysOnTop ) )
// 	{
// 		f = f & ( ~WStyle_StaysOnTop );
// 		setWFlags(  f  );
// 		m_topmostBtn->setToggleButton( true );
// 		m_topmostBtn->setOn( false );
// 		//m_topmostBtn->setToggleButton( false );
// 	}
// 	else
// 	{
// 		f = f | WStyle_StaysOnTop;
// 		setWFlags( f );
// 		m_topmostBtn->setToggleButton( true );
// 		m_topmostBtn->setOn( true );
// 		//m_topmostBtn->setToggleButton( false );
// 	}
// 
// 	reparent( NULL, f, geometry().topLeft(), true );
// 	setIcon( QPixmap::fromMimeSource( "realalarm.png" ) );
	Qt::WindowFlags f;
	f = windowFlags();
	if(  f.testFlag(Qt::WindowStaysOnTopHint))
	{
		f = Qt::FramelessWindowHint;
		setWindowFlags(  f  );
// 		m_topmost_btn->setCheckable( true );
// 		m_topmost_btn->setChecked( false );
	}
	else
	{
		f |= Qt::WindowStaysOnTopHint ;
		setWindowFlags( f );
// 		m_topmost_btn->setCheckable( true );
// 		m_topmost_btn->setChecked( true );
	}

	setParent(NULL ,f);
	move(geometry().topLeft());
	show();
}

// void QEventRealWnd::customEvent(QCustomEvent *event) 
void Event_real_wnd::slot_custom_event( Rsys_event * event ,int eventType) 
{
	switch ( eventType ) 
	{
	case QMSG_NEW_EVENT:
		//if( !m_pause )
		{
// 			Rsys_event * pEvent = new Rsys_event((SYS_EVENT *) event->data());
			
			Rsys_event * pEvent = event;			//modify by chenkai
			
			pEvent->m_bneed_confirm = m_db_para.m_bconfirm_event_type( pEvent->type );

			printf( "用户%s,安全级pEvent->security=%d\n", m_cur_user.username, 
				m_cur_user.opright[ pEvent->security ] );
			if( !m_cur_user.opright[ pEvent->security ] )
				break;

			// 需要显示的
			if(pEvent->DispOut)
			{
				bool bAdd = m_bnew_event( pEvent );
				
				if( bAdd )
				{
					TRY_LOCK;
					m_real_event_list.appendAndMatch( pEvent );
				}	
				else
				{
					return ;
				}
			}
			else
			{
				return ;
			}

			// 需要打印的
			if(pEvent->PrintOut) 
			{
				if( m_event_cfg.get_print())
				{
					m_print.add_event( pEvent);
				}
			}

			// 需要语音报警的
			if(pEvent->ToneOut) 
			{ 
				if( m_event_cfg.get_tone() )
				{
					//printf( "event=%s\n", pEvent->text );
					m_event_sound.add_sound_event( pEvent, 
						m_event_cfg.get_event_sound_count( pEvent->type ) );
				}

			}

			// 短讯报警
//			#ifdef _WIN32
			if( m_event_cfg.get_send_msg() )
			{
				m_event_msg.add_msg_event( pEvent, m_event_cfg.get_send_phone_no( (SYS_EVENT*)pEvent ) );
				m_event_msg_list.append(pEvent);
			}
//			#endif


			//报表打印			//add by chenkai 2013年10月21日 15:16:12
			if( m_event_cfg.get_report_print() )
			{
				QString station = _C2Q( pEvent->Object ) ;
				if ( m_event_cfg.get_report_name( station ).count() != 0 )
				{
					int16u pType = pEvent->type ;
					QString ymd = QString::number( pEvent->ymd ) ;
					QString pTypeStr ;
// 					if ( m_dbPara.getEventTypeDesc( pType , pTypeStr ) )
// 					{
						QStringList typeStrList = m_event_cfg.get_report_event_type( station ) ;
						QStringList nameStrList = m_event_cfg.get_report_name( station ) ;
						int index = typeStrList.indexOf( QString::number( pType ) ) ;
						if ( index != -1 )
						{
							QString reportName = nameStrList[ index ] ; 
							QString program = _C2Q( "java" ) ;
							QString reportjar = QString( getenv( NBENV ) ) + _C2Q( "/bin/report.jar" ) ;
							int Id = 0 ;
							m_db_para.get_report_Id( reportName , Id ) ;
							QString ymdStr = QString( "%1,%2,%3" ).arg( ymd.left( 4 ) ).arg( ymd.mid( 4 , 2 ) ).arg( ymd.right( 2 ) ) ;
							QStringList arguments;
							QString psubStr = _C2Q("/%1,%2,%3,%4").arg( Id ).arg( m_event_cfg.get_report_print_type() ).arg( ymdStr ).arg( _C2Q( "autoprint ") );
							arguments << "-jar" << reportjar << psubStr ;

							QProcess *myProcess = new QProcess( this );
							myProcess->start(program, arguments);
						}
// 					}
				}
			}
		}
		break;
	default:
		{
// 			qWarning("Unknown custom event type: %d", event->type());
			qWarning("Unknown custom event type: %d", eventType);
		}
		
 	}
}

void Event_real_wnd::slot_test_wav_btn()
{
	if( m_event_cfg.get_tone_dev().isEmpty() )
	{
		printf( "语音设备为空\n" );
		char filename[ 256 ];
		char env[ 256 ];
		NetDbg dbg;
		
		sprintf( env, "%s", getenv("NBENV") );
		
		sprintf( filename, "%s/resource/wav/test.wav", env );
		
		if( !QFile::exists( tr( filename ) ) )
		{
		//	dbg.PutDbg( RALARMDEBUG, "语音文件[%s]不存在!", filename );
			return;
		}
		
		m_event_sound.play( tr( filename ) );
	}
	else
	{
		m_event_sound.play_text( tr( "大全自动化智能报警系统" ) );
	}
	/*QFile file( "COM2" );
	int ret = file.open( IO_Raw | IO_ReadWrite );
	char buf[ 13 ];
	buf[ 0 ] = 0xFD;
	buf[ 1 ] = 0;
	buf[ 2 ] = 0x0A;
	buf[ 3 ] = 0x01;
	buf[ 4 ] = 0x03;
	buf[ 5 ] = 0xD1;
	buf[ 6 ] = 0x79;
	buf[ 7 ] = 0x27;
	buf[ 8 ] = 0x59;
	buf[ 9 ] = 0xAF;
	buf[ 10 ] = 0x8B;
	buf[ 11 ] = 0xDE;
	buf[ 12 ] = 0x98;

	file.writeBlock( buf, 13 );
	file.close();*/
}

void Event_real_wnd::slot_stop_thread()
{
	if( m_read_event_proc.isRunning() )
	{
		m_read_event_proc.stop();
		m_read_event_proc.wait();
	}

	if( m_event_sound.isRunning() )
	{
		m_event_sound.stop();
		m_event_sound.wait();
	}

//#ifdef _WIN32
	if( m_event_msg.isRunning() )
	{
		m_event_msg.stop();
		m_event_msg.wait();
	}
//#endif
}

//由于“确认事项”按钮不是toolbar中按钮，点击按钮后无法获取当前focus table，这里增加了tableindex来处理这个问题
Event_table * Event_real_wnd::get_foucs_table()
{
	int i;
	Event_table *pTable;
	for( i = 0; i < m_page_list.count(); i ++ )
	{
		pTable = m_page_list.at( i );
// 		if( pTable->hasFocus() )
// 		{
// 			return pTable;
// 		}
		if (pTable->m_tabel_index == m_current_table_index)
		{
			return pTable;
		}
	}

	for( i = 0; i < m_wnd_list.count(); i ++ )
	{
		pTable = m_wnd_list.at( i );
// 		if( pTable->hasFocus() )
// 		{
// 			return pTable;
// 		}
		if (pTable->m_tabel_index == m_current_table_index)
		{
			return pTable;
		}
	}

	return NULL;
}

void Event_real_wnd::slot_confirm_snd()
{
	m_event_sound.confirm_all_event();
}

void Event_real_wnd::slot_show_help()
{
	int a =1;
#if _WIN32
// 	QProcess proc;
// 	
// 	proc.addArgument( _C2Q( "hh" ) );
// 	proc.addArgument( _C2Q( "d:/d6000/help/D6000智能配电网主站系统-使用说明书.chm::/_57.htm" ) );
	
// 	proc.start();
	QString strfilename = tr( "d:/d6000/help/D6000智能配电网主站系统-使用说明书.chm::/_57.htm" );
	QStringList processArgs = strfilename.split( tr(" ") );

	QProcess proc(this);
	proc.startDetached("hh",processArgs);
#endif
	
}

void Event_real_wnd::mousePressEvent( QMouseEvent *event )
{
/*	mousePosition = event->pos();
	//只对标题栏范围内的鼠标事件进行处理
	if (mousePosition.x()<=pos_min_x)
		return;
	if ( mousePosition.x()>=pos_max_x)
		return;
	if (mousePosition.y()<=pos_min_y )
		return;
	if (mousePosition.y()>=pos_max_y)
		return;
	isLeftPressDown = true;
*/
	switch(event->button()) {
	case Qt::LeftButton:
		m_bleft_press_down = true;
		if(dir != NONE) {
			this->mouseGrabber();
		} else {
			drag_position = event->globalPos() - this->frameGeometry().topLeft();
		}
		break;
	default:
		QMainWindow::mousePressEvent(event);
	}

}

void Event_real_wnd::mouseMoveEvent( QMouseEvent *event )
{
	QPoint gloPoint = event->globalPos();
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());

	if(!m_bleft_press_down) {
		this->region(gloPoint);
	} else {

		if(dir != NONE) {
			QRect rMove(tl, rb);

			switch(dir) {
			case LEFT:
				if(rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				break;
			case RIGHT:
				rMove.setWidth(gloPoint.x() - tl.x());
				break;
			case UP:
				if(rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case DOWN:
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case LEFTTOP:
				if(rb.x() - gloPoint.x() <= this->minimumWidth())
					rMove.setX(tl.x());
				else
					rMove.setX(gloPoint.x());
				if(rb.y() - gloPoint.y() <= this->minimumHeight())
					rMove.setY(tl.y());
				else
					rMove.setY(gloPoint.y());
				break;
			case RIGHTTOP:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setY(gloPoint.y());
				break;
			case LEFTBOTTOM:
				rMove.setX(gloPoint.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			case RIGHTBOTTOM:
				rMove.setWidth(gloPoint.x() - tl.x());
				rMove.setHeight(gloPoint.y() - tl.y());
				break;
			default:

				break;
			}
			this->setGeometry(rMove);
		} else {
			move(event->globalPos() - drag_position);
			event->accept();
		}
	}
	QMainWindow::mouseMoveEvent(event);
}

void Event_real_wnd::mouseReleaseEvent( QMouseEvent *event )
{
	if(event->button() == Qt::LeftButton) {
		m_bleft_press_down = false;
		if(dir != NONE) {
			this->releaseMouse();
			this->setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void Event_real_wnd::region(const QPoint &cursorGlobalPoint)
{
	QRect rect = this->rect();
	QPoint tl = mapToGlobal(rect.topLeft());
	QPoint rb = mapToGlobal(rect.bottomRight());
	int x = cursorGlobalPoint.x();
	int y = cursorGlobalPoint.y();

	if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
		// 左上角
		dir = LEFTTOP;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	} else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		// 右下角
		dir = RIGHTBOTTOM;
		this->setCursor(QCursor(Qt::SizeFDiagCursor));
	} else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
		//左下角
		dir = LEFTBOTTOM;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	} else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
		// 右上角
		dir = RIGHTTOP;
		this->setCursor(QCursor(Qt::SizeBDiagCursor));
	} else if(x <= tl.x() + PADDING && x >= tl.x()) {
		// 左边
		dir = LEFT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	} else if( x <= rb.x() && x >= rb.x() - PADDING) {
		// 右边
		dir = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}else if(y >= tl.y() && y <= tl.y() + PADDING){
		// 上边
		dir = UP;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	} else if(y <= rb.y() && y >= rb.y() - PADDING) {
		// 下边
		dir = DOWN;
		this->setCursor(QCursor(Qt::SizeVerCursor));
	}else {
		// 默认
		dir = NONE;
		this->setCursor(QCursor(Qt::ArrowCursor));
	}
}

bool Event_real_wnd::get_pause()
{
	return m_bpause;
}
