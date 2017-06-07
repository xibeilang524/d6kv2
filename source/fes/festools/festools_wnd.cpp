#include "festools_wnd.h"
#include "festools_globaldef.h"
#include <time.h>
#include <QVBoxLayout>

festools_wnd::festools_wnd( QWidget* parent /*= NULL */ ) : QMainWindow( parent ) , m_fes_shm( Fes_shm::get() )
{
	ini_stylesheet();

	ini_festools_widget() ;
	ini_festools_menubar() ;
	ini_festools_statusbar() ;

	setWindowTitle( tr( "前置机工具" ) ) ;
	setWindowIcon(  QPixmap( ":/res/ironman.png" ) ) ;
	m_timerTime = startTimer( 1000 ) ;

	connect( m_pChannel_tree_widget , SIGNAL( on_channel_select_changed( QTreeWidgetItem* , int ) ) , this, SLOT( on_current_channel_changed( QTreeWidgetItem* , int ) ) ) ;
	connect( m_pChannel_data_widget->m_pMsg_widget , SIGNAL( signal_show_raw_table(bool) ) , this , SLOT( on_show_raw_table(bool) ) ) ;
}

void festools_wnd::ini_festools_widget()
{
	m_pChannel_tree_widget = new channel_tree_widget( this ) ;
	m_pChannel_data_widget = new channel_data_widget( this ) ;
	m_pRaw_data_widget = new raw_data_widget( this ) ;
	m_pVsplitter = new QSplitter( Qt::Vertical ) ;
	m_pVsplitter->addWidget( m_pChannel_data_widget ) ;
	m_pVsplitter->addWidget( m_pRaw_data_widget ) ;
	m_pVsplitter->setStretchFactor( 0 , 5 ) ;
	m_pVsplitter->setStretchFactor( 1 , 3 ) ;

	m_pHsplitter = new QSplitter( Qt::Horizontal ) ;
	m_pHsplitter->addWidget( m_pChannel_tree_widget ) ;
	m_pHsplitter->addWidget( m_pVsplitter ) ;
	m_pHsplitter->setStretchFactor( 0 , 1 ) ;
	m_pHsplitter->setStretchFactor( 1 , 5 ) ;

	setCentralWidget( m_pHsplitter ) ;
}

void festools_wnd::ini_festools_menubar()
{

}

void festools_wnd::ini_festools_statusbar()
{
	m_pStatus_bar = new QStatusBar( this ) ;
	setStatusBar( m_pStatus_bar ) ;
	m_pStatus_bar_widget = new status_bar_widget( this ) ;
	m_pStatus_bar->installEventFilter( m_pStatus_bar_widget ) ;
	m_pData_label = new QLabel( m_pStatus_bar );
	m_pData_label->setFrameStyle( QFrame::Panel | QFrame::Sunken ) ;
	m_pData_label->setMaximumWidth( 150 ) ;
	m_pStatus_bar_widget->add_status_widget( m_pData_label , 150 ) ;

	m_pTime_label = new QLabel( m_pStatus_bar );
	m_pTime_label->setFrameStyle( QFrame::Panel | QFrame::Sunken ) ;
	m_pTime_label->setMaximumWidth( 120 ) ;
	m_pStatus_bar_widget->add_status_widget( m_pTime_label , 120 ) ;
}

void festools_wnd::timerEvent( QTimerEvent* event )
{
	if( event->timerId() == m_timerTime )
	{
		update_time() ;
//		m_pChannel_tree_widget->refresh_channel_tree() ;
	}
}

void festools_wnd::update_time()
{
	char str1[ 128 ] , str2[ 128 ] ;

	time_t t ;
	time( &t ) ;

	tm * ltm = localtime( &t ) ;

	sprintf( str1 , "%04d年%02d月%02d日" , ltm->tm_year + 1900 , ltm->tm_mon + 1 , ltm->tm_mday ) ;
	sprintf( str2 ,  "%02d:%02d:%02d" , ltm->tm_hour , ltm->tm_min , ltm->tm_sec ) ;

	m_pData_label->setText( _C2Q( str1 ) ) ;
	m_pTime_label->setText( _C2Q( str2 ) ) ;
}

void festools_wnd::on_current_channel_changed( QTreeWidgetItem* pItem , int col )
{
	if ( pItem->data( 0 , ROLE_MODEL_TYPE ) == TYPE_CHANNAL )
	{
		m_pChannel_data_widget->hide_channel_table() ;
		int channel_no = m_pChannel_tree_widget->get_cur_channel_no() ;
		int terminal_no = m_pChannel_tree_widget->get_cur_terminal_no() ;
		m_pChannel_data_widget->change_termianl_no( terminal_no ) ;
		m_pChannel_data_widget->change_channel_no( channel_no ) ;

		m_pRaw_data_widget->change_termianl_no( terminal_no ) ;
		m_pRaw_data_widget->change_channel_no( channel_no ) ;
	}
	else
	{
		m_pChannel_data_widget->hide_msg_widget() ;
		m_pRaw_data_widget->hide_all_widgets( true ) ;
// 		m_pChannel_data_widget->change_channel_no( -1 ) ;
// 		m_pChannel_data_widget->change_termianl_no( -1 ) ;
// 
// 		m_pRaw_data_widget->change_channel_no( -1 ) ;
// 		m_pRaw_data_widget->change_termianl_no( -1 ) ;
	}
}

void festools_wnd::on_show_raw_table( bool flag )
{
	if ( flag )
	{
		m_pRaw_data_widget->hide_all_widgets( false ) ;
	}
	else
	{
		m_pRaw_data_widget->hide_all_widgets( true ) ;
	}
}

void festools_wnd::ini_stylesheet()
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QPushButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
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
						   background: #D5D5D5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QSplitter\
						   {\
						   background: #FAFAFA;\
						   }\
						   QFrame\
						   {\
						   background: #FAFAFA;\
						   }\
						   QLabel\
						   {\
						   background: #FAFAFA;\
						   }\
						   QHeaderView::section\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color: #0F1923;\
						   width:30px;\
						   height:30px;\
						   border: 0px solid #567dbc;\
						   border-left:0px;\
						   border-right:0px;\
						   background:#F1F1F1;\
						   }\
						   QSplitter\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget::pane\
						   {\
						   border:none;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: none;\
						   }\
						   QTableWidget::item\
						   {\
						   text-align:center;\
						   }\
						   QScrollBar\
						   {\
						   background:white;\
						   height:8px;\
						   }\
						   QScrollBar::handle\
						   {\
						   background:lightgray;\
						   border:2px solid transparent;\
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar:vertical\
						   {\
						   width:8px;\
						   background:white;\
						   padding-top:9px;\
						   padding-bottom:9px;\
						   }\
						   QScrollBar::handle:vertical\
						   {\
						   background:lightgray; \
						   width:8px;\
						   min-height:20;\
						   border:2px solid transparent; \
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:vertical:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
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
						   QTreeView\
						   {\
						   border: none;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QTreeView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QTreeView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QLineEdit\
						   {\
						   background: #FFFFFF;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QTabBar::tab\
						   {\
						   color:#0F1923;\
						   font-family:Microsoft YaHei;\
						   font-size: 13px;\
						   min-width:100px;\
						   min-height:30px;\
						   border:0px;\
						   }\
						   QTabBar::tab:hover\
						   {\
						   background:rgb(255,255, 255, 100);\
						   }\
						   QTabBar::tab:selected\
						   {\
						   border-color:white;\
						   background:white;\
						   color:#5D9CE5;\
						   border-bottom: 2px solid #4A90E2;\
						   }\
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px -7px 0px 0px;\
						   }\
						   QDateEdit QCalendarWidget QSpinBox#qt_calendar_yearedit,QDateTimeEdit QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   padding:0px -2px 0px 0px;\
						   }\
						   QCalendarWidget QToolButton::menu-indicator\
						   {\
						   image:None;\
						   }\
						   QCalendarWidget QTableView\
						   {\
						   border-width:0px;\
						   }\
						   QCalendarWidget QWidget#qt_calendar_navigationbar\
						   {\
						   border:1px solid #575757;\
						   border-width:1px 1px 0px 1px;\
						   background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #4D4D4D,stop:1 #292929);\
						   }\
						   QToolBar\
						   {\
						   background: #FFFFFF;\
						   border: 0px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 30px;\
						   max-height: 30px;\
						   spacing:5px;\
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
						   QDateTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QDateTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QDateTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   ") ;
	setStyleSheet( buttonStyleSheet ) ;
}
