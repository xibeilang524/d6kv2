#include "hisquery_wnd.h"
#include "hisquery_modify_dialog.h"
#include "db/evtapi.h"
#include "uitools/dmessagebox.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QHeaderView>
#include <QPrinter>
#include <QPrintDialog>
#include <QStatusBar>
#include <QCloseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>

Hisquery_wnd::Hisquery_wnd( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	ini_mainwindow() ;
}

void Hisquery_wnd::ini_mainwindow( void )
{
	setWindowTitle( tr( "D6000事项查询工具" ) ) ;
	setWindowIcon( QIcon( ":/res/queryevent.png" ) ) ;

	if( !m_db_para.readPara() )
	{
// 		QMessageBox::warning( this, tr( "警告" ) , tr( "读数据库失败!" ) ) ;
		DMessageBox::warning( this ,tr("警告"), tr( "读数据库失败!" ) );
		close();
	}
	ini_para() ;

	m_pCur_user = NULL;

	m_pCentral_frame = new QFrame( this ) ;
	m_pToolbar_frame = new QFrame( m_pCentral_frame ) ;
	m_pToolbar_frame->setObjectName( tr( "toolbarframe" ) ) ;
	m_pMain_frame = new QFrame( m_pCentral_frame ) ;
	m_pFilter_frame = new QFrame( m_pMain_frame ) ;
	m_pEvent_table = new QTableWidget( m_pMain_frame ) ;
	QStringList header_list ;
	header_list.append( tr( "事项类型" ) ) ;
	header_list.append( tr( "事项时间" ) ) ;
	header_list.append( tr( "事项描述" ) ) ;
	header_list.append( tr( "值" ) ) ;
	header_list.append( tr( "对象组名" ) ) ;
	m_pEvent_table->setColumnCount( 5 ) ;
	m_pEvent_table->setHorizontalHeaderLabels( header_list ) ;
	m_pEvent_table->setAlternatingRowColors( true ) ;
	m_pEvent_table->setEditTriggers( QAbstractItemView::NoEditTriggers ) ;
	m_pEvent_table->setSelectionBehavior( QAbstractItemView::SelectRows ) ;
	QHeaderView *pTable_headerview = m_pEvent_table->verticalHeader();
	if ( pTable_headerview )
	{
		pTable_headerview->setDefaultAlignment( Qt::AlignHCenter | Qt::AlignVCenter ) ;
		pTable_headerview->setTextElideMode(Qt::ElideRight) ;
	}
	QPalette palette = m_pEvent_table->palette() ;
	palette.setColor( QPalette::AlternateBase , QColor( "#D5D5D5" ) ) ;
	m_pEvent_table->setPalette( palette ) ;

	QVBoxLayout* pVbox_layout = new QVBoxLayout( m_pCentral_frame ) ;
	pVbox_layout->addWidget( m_pToolbar_frame ) ;
	pVbox_layout->addWidget( m_pMain_frame ) ;
	pVbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	setCentralWidget( m_pCentral_frame ) ;
	m_pCentral_frame->setLayout( pVbox_layout ) ;

	QSplitter* pSplitter = new QSplitter( m_pMain_frame ) ;

	QVBoxLayout* pVbox_layout_1 = new QVBoxLayout( m_pMain_frame ) ;
	m_pMain_frame->setLayout( pVbox_layout_1 ) ;
	pVbox_layout_1->addWidget( pSplitter ) ;
	pVbox_layout_1->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	pSplitter->addWidget( m_pFilter_frame ) ;
	pSplitter->addWidget( m_pEvent_table ) ;
	pSplitter->setStretchFactor( 0 , 1 ) ;
	pSplitter->setStretchFactor( 1 , 3 ) ;

	m_pFilter_tabwidget = new QTabWidget( m_pFilter_frame ) ;
	QVBoxLayout* pVbox_layout_2 = new QVBoxLayout( m_pFilter_frame ) ;
	m_pFilter_frame->setLayout( pVbox_layout_2 ) ;
	pVbox_layout_2->addWidget( m_pFilter_tabwidget ) ;
	pVbox_layout_2->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	m_pClassified_query_frame = new QFrame( this ) ;
	m_pAdvances_query_frame = new QFrame( this ) ;
	m_pFilter_tabwidget->addTab( m_pClassified_query_frame , tr( "分类查询" ) ) ; 
	m_pFilter_tabwidget->addTab( m_pAdvances_query_frame , tr( "高级查询" ) ) ; 
	ini_toolbar() ;
	ini_filterframe() ;
	slot_today_button() ;

	m_pPrinter = new QPrinter( QPrinter::HighResolution ) ;

	m_pStatus_bar = new QStatusBar( this ) ;
	setStatusBar( m_pStatus_bar ) ;

	connect( m_pEvent_print_button , SIGNAL( clicked() ) , this , SLOT( slot_print() ) ) ;
	connect( m_pEvent_output_button , SIGNAL( clicked() ) , this , SLOT( slot_event_output() ) ) ;
	connect( m_pEvent_delete_button , SIGNAL( clicked() ) , this , SLOT( slot_event_delete() ) ) ;
	connect( m_pEvent_exit_button , SIGNAL( clicked() ) , this , SLOT( slot_exit() ) ) ;

	connect( m_pAll_type_button , SIGNAL( clicked() ) , this , SLOT( slot_all_type() ) ) ;
	connect( m_pAll_station_button , SIGNAL( clicked() ) , this , SLOT( slot_all_station() ) ) ;
	connect( m_pClassified_query_button , SIGNAL( clicked() ) , this , SLOT( slot_event_type_query() ) ) ;
	connect( m_pDev_query_button , SIGNAL( clicked() ) , this , SLOT( slot_dev_query() ) ) ;
	connect( m_pFuzzy_query_button , SIGNAL( clicked() ) , this , SLOT( slot_fuzzy_query() ) ) ;

	connect( m_pToday_button , SIGNAL( clicked() ) , this , SLOT( slot_today_button() ) ) ;
	connect( m_pYesterday_button , SIGNAL( clicked() ) , this , SLOT( slot_yesterday_button() ) ) ;
	connect( m_pToolbar_search_button , SIGNAL( clicked() ) , this , SLOT( slot_toolbar_search_button() ) ) ;
	connect( m_pDev_select_tree , SIGNAL( itemExpanded ( QTreeWidgetItem* ) ) , this , SLOT( slot_show_station_dev( QTreeWidgetItem* ) ) ) ;
	connect( m_pEvent_type_select_tree , SIGNAL( itemClicked( QTreeWidgetItem* , int ) ) , this , SLOT( slot_event_type_tree_clicked(QTreeWidgetItem* , int ) ) ) ;
	connect( m_pDev_select_tree , SIGNAL( itemClicked( QTreeWidgetItem* , int ) ) , this , SLOT( slot_event_type_tree_clicked(QTreeWidgetItem* , int ) ) ) ;
	connect( m_pStation_select_tree , SIGNAL( itemClicked( QTreeWidgetItem* , int ) ) , this , SLOT( slot_event_type_tree_clicked(QTreeWidgetItem* , int ) ) ) ;
	connect( m_pStation_select_tree , SIGNAL( itemExpanded( QTreeWidgetItem* , int ) ) , this , SLOT( slot_event_type_tree_clicked(QTreeWidgetItem* , int ) ) ) ;
	connect( m_pEvent_table , SIGNAL( itemDoubleClicked ( QTableWidgetItem *) ), SLOT( slot_modify_event( QTableWidgetItem * ) ) );

	reg_proc() ;
}

void Hisquery_wnd::ini_toolbar( void )
{
	QFont font ;
	font.setFamily( tr( "微软雅黑" ) ) ;
	font.setPointSize( 10 ) ;

	QHBoxLayout* pHBox_layout = new QHBoxLayout( m_pToolbar_frame ) ;
	pHBox_layout->setSpacing( 5 ) ;
	m_pToolbar_frame->setLayout( pHBox_layout ) ;

	m_pEvent_print_button = new QToolButton( m_pToolbar_frame ) ;
	m_pEvent_print_button->setObjectName( tr( "eventprintbutton" ) ) ;
	m_pEvent_print_button->setText( tr( "事项打印" ) ) ;
	m_pEvent_print_button->setToolTip( tr( "事项打印" ) ) ;
	m_pEvent_print_button->setIcon( QIcon( ":/res/printevent.png" ) ) ;
	m_pEvent_print_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pEvent_print_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pEvent_print_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pEvent_print_button ) ;

	m_pEvent_output_button = new QToolButton( m_pToolbar_frame ) ;
	m_pEvent_output_button->setObjectName( tr( "eventoutputbutton" ) ) ;
	m_pEvent_output_button->setText( tr( "事项输出" ) ) ;
	m_pEvent_output_button->setToolTip( tr( "事项输出" ) ) ;
	m_pEvent_output_button->setIcon( QIcon( ":/res/outputevent.png" ) ) ;
	m_pEvent_output_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pEvent_output_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pEvent_output_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pEvent_output_button ) ;

	m_pEvent_delete_button = new QToolButton( m_pToolbar_frame ) ;
	m_pEvent_delete_button->setObjectName( tr( "delevnetbutton" ) ) ;
	m_pEvent_delete_button->setText( tr( "事项删除" ) ) ;
	m_pEvent_delete_button->setToolTip( tr( "事项删除" ) ) ;
	m_pEvent_delete_button->setIcon( QIcon( ":/res/delevent.png" ) ) ;
	m_pEvent_delete_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pEvent_delete_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pEvent_delete_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pEvent_delete_button ) ;

	m_pEvent_exit_button = new QToolButton( m_pToolbar_frame ) ;
	m_pEvent_exit_button->setObjectName( tr( "exitbutton" ) ) ;
	m_pEvent_exit_button->setText( tr( "事项退出" ) ) ;
	m_pEvent_exit_button->setToolTip( tr( "事项退出" ) ) ;
	m_pEvent_exit_button->setIcon( QIcon( ":/res/exitevent.png" ) ) ;
	m_pEvent_exit_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pEvent_exit_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pEvent_exit_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pEvent_exit_button ) ;

	QSpacerItem* pHorizontal_spacer = new QSpacerItem( 40 , 20 , QSizePolicy::Expanding , QSizePolicy::Minimum ) ;
	pHBox_layout->addItem( pHorizontal_spacer ) ;

	m_pSearch_in_result_checkbox = new QCheckBox( tr( "在查询结果中查询" ) , m_pToolbar_frame ) ;
	m_pSearch_in_result_checkbox->setFont( font ) ;
	pHBox_layout->addWidget( m_pSearch_in_result_checkbox ) ;

	QLabel *pBegin_label = new QLabel( tr( "起始时间：" ));
	QLabel *pEnd_label = new QLabel( tr( "终止时间：" ));
	m_pBegin_datatime_edit = new QDateTimeEdit( m_pToolbar_frame ) ;
	m_pBegin_datatime_edit->setCalendarPopup(true);	
	pHBox_layout->addWidget( pBegin_label ) ;
	pHBox_layout->addWidget( m_pBegin_datatime_edit ) ;

	m_pEnd_datatime_edit = new QDateTimeEdit( m_pToolbar_frame ) ;
	m_pEnd_datatime_edit->setCalendarPopup(true);
	pHBox_layout->addWidget( pEnd_label ) ;
	pHBox_layout->addWidget( m_pEnd_datatime_edit ) ;

	m_pYesterday_button = new QToolButton( m_pToolbar_frame ) ;
	m_pYesterday_button->setObjectName( tr( "yesterdaybutton" ) ) ;
	m_pYesterday_button->setText( tr( "昨日" ) ) ;
	m_pYesterday_button->setToolTip( tr( "昨日" ) ) ;
// 	m_pYesterday_button->setIcon( QIcon( ":/res/delevent.png" ) ) ;
	m_pYesterday_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pYesterday_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pYesterday_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pYesterday_button ) ;	

	m_pToday_button = new QToolButton( m_pToolbar_frame ) ;
	m_pToday_button->setObjectName( tr( "todaybutton" ) ) ;
	m_pToday_button->setText( tr( "今日" ) ) ;
	m_pToday_button->setToolTip( tr( "今日" ) ) ;
// 	m_pToday_button->setIcon( QIcon( ":/res/delevent.png" ) ) ;
	m_pToday_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pToday_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pToday_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pToday_button ) ;

	m_pToolbar_search_button = new QToolButton( m_pToolbar_frame ) ;
	m_pToolbar_search_button->setObjectName( tr( "toolbarsearchbutton" ) ) ;
	m_pToolbar_search_button->setText( tr( "查询" ) ) ;
	m_pToolbar_search_button->setToolTip( tr( "查询" ) ) ;
	m_pToolbar_search_button->setIcon( QIcon( ":/res/query.png" ) ) ;
	m_pToolbar_search_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pToolbar_search_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pToolbar_search_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pToolbar_search_button ) ;	
	m_pToolbar_search_button->hide() ;

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QToolButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font: 16px;\
						   color:#FFFFFF;\
						   min-height: 35px;\
						   max-height: 35px;\
						   min-width: 58px ;\
						   }\
						   QToolButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:disabled\
						   {\
						   background: #E5E5E5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   #toolbarframe\
						   {\
						   spacing:5px;\
						   background: #FAFAFA;\
						   min-height: 64px;\
						   max-height:64px;\
						   border: 1px solid #E5E5E5;\
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
						   font-size: 14px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: 1px solid #E5E5E5;\
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
						   border: 1px solid #E5E5E5;\
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
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 40px;\
						   max-height: 40px;\
						   }\
						   QGroupBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
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
						   padding:0px 0px 0px 0px;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background-color:rgba(0,0,0,0);\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 45px;\
						   max-height: 45px;\
						   min-width:70px;\
						   max-width:70px;\
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
						   QSpinBox::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 18px;\
						   height: 18px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 18px;\
						   height: 18px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QSpinBox::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   #todaybutton,#yesterdaybutton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   min-height:25px;\
						   background:transparent;\
						   }\
						   #todaybutton:pressed,#todaybutton:hover,#yesterdaybutton:pressed,#yesterdaybutton:hover\
						   {\
						   border: 1px solid #4A90E2;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   width:70px;\
						   height:30px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}

Hisquery_wnd::~Hisquery_wnd()
{
	m_event_list.clear() ;
	delete m_pPrinter ;
	un_reg_proc() ;
}

void Hisquery_wnd::ini_filterframe( void )
{
	m_pEvent_type_select_tree = new QTreeWidget( m_pClassified_query_frame ) ;
	m_pStation_select_tree = new QTreeWidget( m_pClassified_query_frame ) ;
	QVBoxLayout* pVbox_layout = new QVBoxLayout( m_pClassified_query_frame ) ;
	m_pClassified_query_frame->setLayout( pVbox_layout ) ;
	QSplitter* pSplitter = new QSplitter( Qt::Vertical , m_pClassified_query_frame ) ;
	pVbox_layout->addWidget( pSplitter ) ;
	pVbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;

	m_pAll_type_button = new QToolButton( m_pClassified_query_frame ) ;
	m_pAll_type_button->setObjectName( tr( "alltypebutton" ) ) ;
	m_pAll_type_button->setText( tr( "所有事项" ) ) ;
	m_pAll_type_button->setToolTip( tr( "所有事项" ) ) ;
	m_pAll_type_button->setIcon( QIcon( ":/res/allevent.png" ) ) ;
	m_pAll_type_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pAll_type_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pAll_type_button->setCheckable( true ) ;

	m_pAll_station_button = new QToolButton( m_pClassified_query_frame ) ;
	m_pAll_station_button->setObjectName( tr( "allstationbutton" ) ) ;
	m_pAll_station_button->setText( tr( "所有厂站" ) ) ;
	m_pAll_station_button->setToolTip( tr( "所有厂站" ) ) ;
	m_pAll_station_button->setIcon( QIcon( ":/res/allstation.png" ) ) ;
	m_pAll_station_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pAll_station_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pAll_station_button->setCheckable( true ) ;

	m_pClassified_query_button = new QToolButton( m_pClassified_query_frame ) ;
	m_pClassified_query_button->setObjectName( tr( "classifiedbutton" ) ) ;
	m_pClassified_query_button->setText( tr( "分类查询" ) ) ;
	m_pClassified_query_button->setToolTip( tr( "分类查询" ) ) ;
	m_pClassified_query_button->setIcon( QIcon( ":/res/query.png" ) ) ;
	m_pClassified_query_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pClassified_query_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;

	QFrame* pFrame = new QFrame( m_pClassified_query_frame ) ;
	QHBoxLayout* pHbox_layout = new QHBoxLayout( pFrame ) ;
	pFrame->setLayout( pHbox_layout ) ;
	pHbox_layout->addWidget( m_pAll_type_button ) ;
	pHbox_layout->addWidget( m_pAll_station_button ) ;
	pHbox_layout->addWidget( m_pClassified_query_button ) ;

	pSplitter->addWidget( m_pEvent_type_select_tree ) ;
	pSplitter->addWidget( m_pStation_select_tree ) ;
	pSplitter->addWidget( pFrame ) ;
	pSplitter->setStretchFactor( 0 , 1 ) ;
	pSplitter->setStretchFactor( 1 , 1 ) ;

	QStringList header_list ;
	header_list.append( tr( "事项查询" ) ) ;
	header_list.append( tr( "类型号" ) ) ;
	m_pEvent_type_select_tree->setHeaderLabels( header_list ) ;

	header_list.clear() ;
	header_list.append( tr( "厂站" ) ) ;
	m_pStation_select_tree->setHeaderLabels( header_list ) ;

	m_pDev_query_frame = new QFrame( m_pAdvances_query_frame ) ;
	m_pFuzzy_query_frame = new QGroupBox( m_pAdvances_query_frame ) ;
	m_pFuzzy_query_frame->setTitle( tr( "模糊查询" ) ) ;
	pVbox_layout = new QVBoxLayout( m_pAdvances_query_frame ) ;
	m_pAdvances_query_frame->setLayout( pVbox_layout ) ;
	pSplitter = new QSplitter( Qt::Vertical , m_pAdvances_query_frame ) ;
	pVbox_layout->addWidget( pSplitter ) ;
	pVbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	pSplitter->addWidget( m_pDev_query_frame ) ;
	pSplitter->addWidget( m_pFuzzy_query_frame ) ;
	pSplitter->setStretchFactor( 0 , 1 ) ;

	m_pDev_select_tree = new QTreeWidget( m_pDev_query_frame ) ;
	header_list.clear() ;
	header_list.append( tr( "设备" ) ) ;
	m_pDev_select_tree->setHeaderLabels( header_list ) ;
	m_pDev_query_button = new QToolButton( m_pDev_query_frame ) ;
	m_pDev_query_button->setObjectName( tr( "devquerybutton" ) ) ;
	m_pDev_query_button->setText( tr( "设备查询" ) ) ;
	m_pDev_query_button->setToolTip( tr( "设备查询" ) ) ;
	m_pDev_query_button->setIcon( QIcon( ":/res/query.png" ) ) ;
	m_pDev_query_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pDev_query_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;

	pFrame = new QFrame( m_pDev_query_frame ) ;
	pHbox_layout = new QHBoxLayout( pFrame ) ;
//	pHbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	pFrame->setLayout( pHbox_layout ) ;
	QSpacerItem* pHorizontal_spacer = new QSpacerItem( 40 , 20 , QSizePolicy::Expanding , QSizePolicy::Minimum ) ;
	pHbox_layout->addItem( pHorizontal_spacer ) ;
	pHbox_layout->addWidget( m_pDev_query_button ) ;

	pVbox_layout = new QVBoxLayout( m_pDev_query_frame ) ;
	pVbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	m_pDev_query_frame->setLayout( pVbox_layout ) ;
	pVbox_layout->addWidget( m_pDev_select_tree ) ;
	pVbox_layout->addWidget( pFrame ) ;

	pHbox_layout = new QHBoxLayout( m_pFuzzy_query_frame ) ;
	m_pFuzzy_query_frame->setLayout( pHbox_layout ) ;
	m_pFuzzy_query_lineedit = new QLineEdit( m_pFuzzy_query_frame ) ;
	m_pFuzzy_query_button = new QToolButton( m_pFuzzy_query_frame ) ;
	m_pFuzzy_query_button->setObjectName( tr( "fuzzyquerybutton" ) ) ;
	m_pFuzzy_query_button->setText( tr( "模糊查询" ) ) ;
	m_pFuzzy_query_button->setToolTip( tr( "模糊查询" ) ) ;
	m_pFuzzy_query_button->setIcon( QIcon( ":/res/query.png" ) ) ;
	m_pFuzzy_query_button->setIconSize( QSize( 20 , 20 ) ) ;
	m_pFuzzy_query_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	pHbox_layout->addWidget( m_pFuzzy_query_lineedit ) ;
	pHbox_layout->addWidget( m_pFuzzy_query_button ) ;

	memset( &m_cur_oper_man , 0 , sizeof( DISPATCHER_STRU ) ) ;
//	memcpy( &m_curOperMan , &OperInfo , sizeof( DISPATCHER_STRU ) ) ;

	ini_classified_query_page() ;
	ini_advanced_query_page() ;
}

void Hisquery_wnd::ini_para( void )
{
	m_station_para.clear() ;
	m_station_area_para.clear() ;
	m_station_area_map.clear() ;
	m_station_para = m_db_para.getStationPara() ;
	m_station_area_para = m_db_para.getAreaPara();

	for ( int i = 0 ; i < m_station_area_para.count() ; i++)
	{
		m_station_area_map[ m_station_area_para[i][0].toInt() ] = m_station_area_para[ i ] ;
	}
}

void Hisquery_wnd::ini_classified_query_page( void )
{
	QTreeWidgetItem *pItem ,*pSubItem;
	uint i, j;
	QString desc;
	uint types[ 1000 ];
	uint typenum;
	for( i = 1 ; i < m_db_para.getEventSortCount() ; i ++ )
	{
		if( !m_db_para.getEventSortDesc( i, desc ) )
			continue;
		QStringList desclist_fir ;
		desclist_fir << desc ;

		if( !m_db_para.getEventTypes( i, types, typenum ) )
			continue;

		pItem = new QTreeWidgetItem( m_pEvent_type_select_tree, desclist_fir );
		pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		for( j = 0; j < typenum; j ++ )
		{
			if( !m_db_para.getEventTypeDesc( types[ j ], desc ) )
				continue;
			QStringList desclist_sec ;
			desclist_sec << desc ;

			pSubItem = new QTreeWidgetItem( pItem, desclist_sec );
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			pSubItem->setText(1,QString::number(types[ j ]));

			m_item_to_event_type_map[ pSubItem ] = types[ j ];

			m_event_type_to_item_map[ types[ j ] ] = pSubItem;
		}
	}


	QMap<int ,QTreeWidgetItem*> m_areaItemMap ;	//区域-qtreewidgetitem对应关系

	QStringList desc_group;
	desc_group.clear() ;
	desc_group << tr("未分组厂站")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( m_pStation_select_tree, desc_group ) ;
	firstStaItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
	if (m_cur_oper_man.type != 0)
	{
		firstStaItem->setDisabled(true);
	}
	//显示第一层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() == 0)	//masterid=0 第一级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_pStation_select_tree, desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
			if (m_cur_oper_man.type != 0)
			{
				if (m_station_area_para[ j ][0].toInt() != m_cur_oper_man.type)
				{

					pItem->setDisabled(true);
				}
			}
		}
	}
	//显示第二层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() != 0)	//masterid!=0 第二级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_areaItemMap[m_station_area_para[j][2].toInt()], desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
		}
	}

	for( j = 0; j < m_station_para.count(); j ++ )
	{
		if( m_station_para[j][1].isEmpty())
			continue;

		QStringList desc_rtu;
		desc_rtu << m_station_para[j][1] ;

		QStringList tmp_station = m_station_para[j] ;

		if (m_station_area_map.contains(tmp_station[3].toInt()) && (tmp_station[3].toInt() !=0))
		{
			pItem = m_areaItemMap[ tmp_station.at(3).toInt()] ;
			pSubItem = new QTreeWidgetItem( pItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		else
		{
			pSubItem = new QTreeWidgetItem( /*m_rtuTableWidget->topLevelItem( 0 )*/firstStaItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		m_item_to_rtu_code_map[ pSubItem ] = tmp_station[0];
	}

	if (firstStaItem->childCount() == 0)
	{
		int index_no = m_pStation_select_tree->indexOfTopLevelItem( firstStaItem ) ;
		m_pStation_select_tree->takeTopLevelItem(index_no);
	}
	m_pEvent_type_select_tree->resizeColumnToContents( 0 ) ;
}

void Hisquery_wnd::ini_advanced_query_page( void )
{
	QTreeWidgetItem *pItem,*pSubItem, *pSubSubItem;
	uint i, j, k, p;
	QString desc;
	uint groupno;
	QStringList	stalist, devlist;
	DEVTYPEPARA	devtype;


	QMap<int ,QTreeWidgetItem*> m_areaItemMap ;	//区域-qtreewidgetitem对应关系

	QStringList desc_group;
	desc_group.clear() ;
	desc_group << tr("未分组厂站")  ;
	QTreeWidgetItem *firstStaItem = new QTreeWidgetItem( m_pDev_select_tree, desc_group ) ;
	firstStaItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
	if (m_cur_oper_man.type != 0)
	{
		firstStaItem->setDisabled(true);
	}
	//显示第一层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() == 0)	//masterid=0 第一级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_pDev_select_tree, desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
			if (m_cur_oper_man.type != 0)
			{
				if (m_station_area_para[ j ][0].toInt() != m_cur_oper_man.type)
				{

					pItem->setDisabled(true);
				}
			}
		}
	}
	//显示第二层
	for ( j = 0 ; j < m_station_area_para.count() ; j++ )
	{
		if (m_station_area_para[ j ][2].toInt() != 0)	//masterid!=0 第二级目录
		{
			desc_group.clear() ;
			desc_group.append( m_station_area_para[ j ][1] ) ;
			pItem = new QTreeWidgetItem( m_areaItemMap[m_station_area_para[j][2].toInt()], desc_group ) ;
			pItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
			m_areaItemMap[ m_station_area_para[ j ][0].toInt() ] = pItem ;
		}
	}

	for( j = 0; j < m_station_para.count(); j ++ )
	{
		if( m_station_para[j][1].isEmpty())
			continue;

		QStringList desc_rtu;
		desc_rtu << m_station_para[j][1] ;

		QStringList tmp_station = m_station_para[j] ;

		if (m_station_area_map.contains(tmp_station[3].toInt()) && (tmp_station[3].toInt() !=0))
		{
			pItem = m_areaItemMap[ tmp_station.at(3).toInt()] ;
			pSubItem = new QTreeWidgetItem( pItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		else
		{
			pSubItem = new QTreeWidgetItem( /*m_rtuTableWidget->topLevelItem( 0 )*/firstStaItem, desc_rtu);
			pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 
		}
		m_dev_page_item_to_rtu_code_map[ pSubItem ] = tmp_station[0];
		pSubItem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );

	}

	if (firstStaItem->childCount() == 0)
	{
		int index_no = m_pDev_select_tree->indexOfTopLevelItem( firstStaItem ) ;
		m_pDev_select_tree->takeTopLevelItem(index_no);
	}
}

bool Hisquery_wnd::reg_proc( void )
{
	m_net_dbg.RegProc( "事项查询" ) ;
	m_net_dbg.RegType( 10 , "事项查询" ) ;

#ifdef WIN32
	WId wid = winId();
	m_proc_man.StartProc("hisquery","事项查询","",wid);
#else
	m_proc_man.StartProc("hisquery","事项查询","");
#endif
	m_proc_time_id = startTimer( 1000 ) ;
	return true;
}

void Hisquery_wnd::un_reg_proc( void )
{
	m_net_dbg.UnRegProc() ;
	m_proc_man.EndProc() ;
}

void Hisquery_wnd::slot_today_button( void )
{
	QDate date ;
	QTime time ;
	time.setHMS( 0, 0, 0, 0 ) ;
	date = QDate::currentDate() ;
	m_pBegin_datatime_edit->setDateTime( QDateTime( date, time ) ) ;
	m_pEnd_datatime_edit->setDateTime( QDateTime( date.addDays( 1 ), time ).addSecs( -1 ) ) ;
}

void Hisquery_wnd::slot_yesterday_button( void )
{
	QDate date ;
	QTime time ;
	time.setHMS( 0, 0, 0, 0 ) ;
	date = QDate::currentDate() ;
	m_pBegin_datatime_edit->setDateTime( QDateTime( date.addDays( -1 ), time ) ) ;
	m_pEnd_datatime_edit->setDateTime( QDateTime( date, time ).addSecs( -1 ) ) ;
}

void Hisquery_wnd::slot_toolbar_search_button( void )
{
	if( m_pBegin_datatime_edit->dateTime() > m_pEnd_datatime_edit->dateTime() ) 
	{
// 		QMessageBox::information( this , tr( "错误" ) , tr( "结束时间早于起始时间!" ) ) ;
		DMessageBox::information( this ,tr("错误"), tr( "结束时间早于起始时间!" ) );
		return;
	}

	QDateTime my_time = QDateTime::currentDateTime();
	if( my_time.date().month() < m_pEnd_datatime_edit->dateTime().date().month() - 1 )
	{
// 		QMessageBox::information( this , tr( "错误" ) , tr( "结束时间最远到当前月份!" ) ) ;
		DMessageBox::information( this ,tr("错误"), tr( "结束时间最远到当前月份!" ) );
		return;
	}

	int cur_tab_index = m_pFilter_tabwidget->currentIndex();
	switch( cur_tab_index )
	{
	case 0:
		{
			slot_event_type_query() ;
		}
		break ;
	case 1:
		{
			slot_dev_query() ;
		}
		break ;
	case 2:
		{
			slot_fuzzy_query() ;
		}
		break ;
	}
}

void Hisquery_wnd::slot_event_type_query( void )
{
	QTreeWidgetItem *pItem, *pSubItem,*pSubSubItem;
	QTreeWidgetItem *pCheckItem;

	if( m_pBegin_datatime_edit->dateTime() > m_pEnd_datatime_edit->dateTime() )
		return;

	QToolButton *pBtn = (QToolButton *)sender();
	if( pBtn )
	{
		pBtn->setEnabled( false );
		pBtn->update();
		pBtn->blockSignals( true );
	}

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

	m_beginTime.year = m_pBegin_datatime_edit->dateTime().date().year();
	m_beginTime.mon = m_pBegin_datatime_edit->dateTime().date().month();
	m_beginTime.day = m_pBegin_datatime_edit->dateTime().date().day();
	m_beginTime.hour = m_pBegin_datatime_edit->dateTime().time().hour();
	m_beginTime.min = m_pBegin_datatime_edit->dateTime().time().minute();
	m_beginTime.sec = m_pBegin_datatime_edit->dateTime().time().second();
	m_beginTime.ms = 0;

	m_endTime.year = m_pEnd_datatime_edit->dateTime().date().year();
	m_endTime.mon = m_pEnd_datatime_edit->dateTime().date().month();
	m_endTime.day = m_pEnd_datatime_edit->dateTime().date().day();
	m_endTime.hour = m_pEnd_datatime_edit->dateTime().time().hour();
	m_endTime.min = m_pEnd_datatime_edit->dateTime().time().minute();
	m_endTime.sec = m_pEnd_datatime_edit->dateTime().time().second();
	m_endTime.ms = 999;

	m_event_type_list.clear();
	for ( int i = 0 ; i <  m_pEvent_type_select_tree->topLevelItemCount() ; i++ )
	{
		pItem = m_pEvent_type_select_tree->topLevelItem(i) ;
		pCheckItem = (QTreeWidgetItem*) pItem;
		if( pCheckItem->checkState(0) == Qt::Unchecked)
			continue;

		for ( int j =0 ; j < pItem->childCount() ;j++)
		{
			pSubItem = pItem -> child(j) ;
			pCheckItem = (QTreeWidgetItem*) pSubItem;
			if( pCheckItem->checkState(0) == Qt::Unchecked)
				continue;
			m_event_type_list.append( m_item_to_event_type_map[ pSubItem ] );
		}
	}

	m_rtu_code_list.clear();

	for ( int i =0 ; i< m_pStation_select_tree->topLevelItemCount() ; i++ )
	{
		pItem = m_pStation_select_tree->topLevelItem(i) ;
		pCheckItem = (QTreeWidgetItem*)pItem;
		if( pCheckItem->checkState(0) == Qt::Unchecked )
			continue;
		if (m_item_to_rtu_code_map.contains(pItem))
		{
			m_rtu_code_list.append( m_item_to_rtu_code_map[ pItem ] );
		}

		for ( int j = 0 ; j < pItem->childCount() ; j++ )
		{
			pSubItem = pItem->child(j) ;
			pCheckItem = (QTreeWidgetItem*)pSubItem;
			if( pCheckItem->checkState(0) == Qt::Unchecked )
				continue;
			if (m_item_to_rtu_code_map.contains(pSubItem))
			{
				m_rtu_code_list.append( m_item_to_rtu_code_map[ pSubItem ] );
			}
			else
			{
				for (int k=0;k<pSubItem->childCount();k++)
				{
					pSubSubItem = pSubItem->child(k) ;
					pCheckItem = (QTreeWidgetItem*)pSubSubItem;
					if( pCheckItem->checkState(0) == Qt::Unchecked )
						continue;
					if (m_item_to_rtu_code_map.contains(pSubSubItem))
					{
						m_rtu_code_list.append( m_item_to_rtu_code_map[ pSubSubItem ] );
					}
				}
			}
		}
	}
	find_event_by_eventtype();
	m_rtu_code_list.clear();
	m_event_type_list.clear();
	QApplication::restoreOverrideCursor();
	m_pEvent_table->resizeColumnsToContents();

	if( pBtn )
	{
		pBtn->setEnabled( true );
		pBtn->update();
		pBtn->blockSignals( false );
	}
}

void Hisquery_wnd::slot_dev_query( void )
{
	QTreeWidgetItem* pCheckItem;
	QMap<QTreeWidgetItem*,QString>::Iterator it;
	QStringList	object_code;

	if( m_pBegin_datatime_edit->dateTime() > m_pEnd_datatime_edit->dateTime() )
		return;

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

	m_beginTime.year = m_pBegin_datatime_edit->dateTime().date().year();
	m_beginTime.mon = m_pBegin_datatime_edit->dateTime().date().month();
	m_beginTime.day = m_pBegin_datatime_edit->dateTime().date().day();
	m_beginTime.hour = m_pBegin_datatime_edit->dateTime().time().hour();
	m_beginTime.min = m_pBegin_datatime_edit->dateTime().time().minute();
	m_beginTime.sec = m_pBegin_datatime_edit->dateTime().time().second();
	m_beginTime.ms = 0;

	m_endTime.year = m_pEnd_datatime_edit->dateTime().date().year();
	m_endTime.mon = m_pEnd_datatime_edit->dateTime().date().month();
	m_endTime.day = m_pEnd_datatime_edit->dateTime().date().day();
	m_endTime.hour = m_pEnd_datatime_edit->dateTime().time().hour();
	m_endTime.min = m_pEnd_datatime_edit->dateTime().time().minute();
	m_endTime.sec = m_pEnd_datatime_edit->dateTime().time().second();
	m_endTime.ms = 999;

	m_object_code_list.clear();

	for( it = m_item_to_dev_code_map.begin() ; it != m_item_to_dev_code_map.end() ;it ++ )
	{
		pCheckItem = (QTreeWidgetItem*)it.key() ;
		if( pCheckItem->checkState(0) == 0 )
			continue;

		object_code.clear();

		if( !m_db_para.getDevRealCode( it.value(), object_code ) )	
		{
			continue;
		}
		m_object_code_list += object_code;
	}

	uint i, j, k;
	char where[ 2048 ];
	char tmpstr[ 1024 ];

	DBApi	db;
	if( db.Open( DB_OP_R, false, false ) < 0 )
	{
// 		QMessageBox::information( this, tr( "告警" ), tr( "数据库打开失败!" ) );
		DMessageBox::warning( this ,tr("告警"), tr( "数据库打开失败!" ) );
		return;
	}

	HIS_EVENT *his_event, *pHisEvent;
	int ScaEveNum;

	ulong		lYmd,lHmsms;
	bool		found;


	if( m_pSearch_in_result_checkbox->isChecked() )
	{
		QList<HIS_EVENT *>		tmpHisEventList;
		QList<uint>				eventTypeList;
		ulong					beginYmd,beginHmsms, endYmd,endHmsms;


		beginYmd = m_beginTime.year * 10000l + m_beginTime.mon * 100l + m_beginTime.day;
		beginHmsms = m_beginTime.hour * 10000000l + m_beginTime.min * 100000l +
			m_beginTime.sec * 1000 + m_beginTime.ms;

		endYmd = m_endTime.year * 10000l + m_endTime.mon * 100l + m_endTime.day;
		endHmsms = m_endTime.hour * 10000000l + m_endTime.min * 100000l +
			m_endTime.sec * 1000 + m_endTime.ms;

		for( i = 0 ; i < m_event_list.count(); i ++ )
		{
			pHisEvent = m_event_list.at( i );

			if( pHisEvent->sort != ENTCLASS_SCADA )
			{
				continue;
			}

			if( m_object_code_list.indexOf( tr( pHisEvent->member0 ) ) == m_object_code_list.size() )
			{
				continue;
			}

			lYmd = ( ulong)( pHisEvent->ymd );

			lHmsms = ( ulong )( pHisEvent->hmsms );

			if( lYmd < beginYmd || ( lYmd == beginYmd && lHmsms < beginHmsms ) ||
				endYmd < lYmd || ( endYmd == lYmd && endHmsms < lHmsms ) )
			{
				continue;
			}

			his_event = new HIS_EVENT;

			memcpy( his_event, pHisEvent, sizeof( HIS_EVENT ) );
			tmpHisEventList.append( his_event );
		}

		m_event_list = tmpHisEventList;
	}
	else
	{
		m_event_list.clear();
		int totalcount = 0;
		if( m_object_code_list.count() > 0 )
		{
			for( i = 0; i < m_object_code_list.count() ; i ++ )
			{
				if( i % 50 == 0 )
				{
					sprintf( where, "( 成员0 like '%s%%'", _Q2C( m_object_code_list[ i ] ) );
					found = false;
				}
				else
				{
					sprintf( tmpstr, " or 成员0 like '%s%%'", _Q2C( m_object_code_list[ i ] ) );
					strcat( where, tmpstr );
				}

				if( i % 50 == 49 )
				{
					strcat( where, ") " );
					if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,false) == CS_FAIL )
					{
						if( his_event ) 
						{
							free(his_event);
						}
						continue;
					}

					for( j = 0; j < (uint)ScaEveNum; j ++ )
					{
						pHisEvent = new HIS_EVENT;
						memcpy( pHisEvent, &his_event[ j ], sizeof( HIS_EVENT ) );

						for( k = 0; k < m_event_list.count(); k ++ )
						{
							if( ( pHisEvent->ymd < m_event_list.at( k )->ymd ) || 
								( pHisEvent->ymd == m_event_list.at( k )->ymd &&
								pHisEvent->hmsms < m_event_list.at( k )->hmsms ) )
							{
								break;
							}
						}
						m_event_list.insert( k, pHisEvent );

						totalcount++;
						if(totalcount && (totalcount%MAX_EVENT_COUNT)==0) {
// 							if(QMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
// 								QMessageBox::Yes, QMessageBox::No )==QMessageBox::No)
							if(DMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
								QMessageBox::Yes|QMessageBox::No, QMessageBox::No )==QMessageBox::No)
								goto _failout;
						}
					}

					free( his_event );
					found = true;
				}
			}

			if( !found )
			{
				strcat( where, ") " );
				if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,false) == CS_FAIL )
				{
					if( his_event ) 
					{
						free(his_event);
					}
				}
				else
				{
					for( i = 0; i < (uint)ScaEveNum; i ++ )
					{
						pHisEvent = new HIS_EVENT;
						memcpy( pHisEvent, &his_event[ i ], sizeof( HIS_EVENT ) );

						for( j = 0; j < m_event_list.count(); j ++ )
						{
							if( ( pHisEvent->ymd < m_event_list.at( j )->ymd ) || 
								( pHisEvent->ymd == m_event_list.at( j )->ymd &&
								pHisEvent->hmsms < m_event_list.at( j )->hmsms ) )
							{
								break;
							}
						}
						m_event_list.insert( j, pHisEvent );

						totalcount++;
						if(totalcount && (totalcount%MAX_EVENT_COUNT)==0) {
// 							if(QMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
// 								QMessageBox::Yes, QMessageBox::No )==QMessageBox::No)
							if(DMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
								QMessageBox::Yes|QMessageBox::No, QMessageBox::No )==QMessageBox::No)
								goto _failout;
						}
					}

					free( his_event );
				}
			}
		}
	}

_failout:
	fill_event_table();
	db.Close();

	QApplication::restoreOverrideCursor( );
}

void Hisquery_wnd::slot_fuzzy_query( void )
{
	if( m_pFuzzy_query_lineedit->text().isEmpty() )
		return;

	if( m_pBegin_datatime_edit->dateTime() > m_pEnd_datatime_edit->dateTime() )
		return;

	QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

	m_beginTime.year = m_pBegin_datatime_edit->dateTime().date().year();
	m_beginTime.mon = m_pBegin_datatime_edit->dateTime().date().month();
	m_beginTime.day = m_pBegin_datatime_edit->dateTime().date().day();
	m_beginTime.hour = m_pBegin_datatime_edit->dateTime().time().hour();
	m_beginTime.min = m_pBegin_datatime_edit->dateTime().time().minute();
	m_beginTime.sec = m_pBegin_datatime_edit->dateTime().time().second();
	m_beginTime.ms = 0;

	m_endTime.year = m_pEnd_datatime_edit->dateTime().date().year();
	m_endTime.mon = m_pEnd_datatime_edit->dateTime().date().month();
	m_endTime.day = m_pEnd_datatime_edit->dateTime().date().day();
	m_endTime.hour = m_pEnd_datatime_edit->dateTime().time().hour();
	m_endTime.min = m_pEnd_datatime_edit->dateTime().time().minute();
	m_endTime.sec = m_pEnd_datatime_edit->dateTime().time().second();
	m_endTime.ms = 999;

	m_fuzzy_string = m_pFuzzy_query_lineedit->text();

	uint i;
	char where[ 2048 ];

	HIS_EVENT *his_event, *pHisEvent;
	int ScaEveNum;

	QStringList codelist;

	ulong		lYmd,lHmsms;

	DBApi	db;
	if( db.Open( DB_OP_R, false, false ) < 0 )
	{
// 		QMessageBox::information( this, tr( "告警" ), tr( "数据库打开失败!" ) );
		DMessageBox::warning( this, tr( "告警" ), tr( "数据库打开失败!" ) );
		return;
	}


	if( m_pSearch_in_result_checkbox->isChecked() )
	{
		QList<HIS_EVENT *>		tmpHisEventList;
		QList<uint>				eventTypeList;
		ulong					beginYmd,beginHmsms, endYmd,endHmsms;


		beginYmd = m_beginTime.year * 10000l + m_beginTime.mon * 100l + m_beginTime.day;
		beginHmsms = m_beginTime.hour * 10000000l + m_beginTime.min * 100000l +
			m_beginTime.sec * 1000 + m_beginTime.ms;

		endYmd = m_endTime.year * 10000l + m_endTime.mon * 100l + m_endTime.day;
		endHmsms = m_endTime.hour * 10000000l + m_endTime.min * 100000l +
			m_endTime.sec * 1000 + m_endTime.ms;

		for( i = 0 ; i < m_event_list.count(); i ++ )
		{
			pHisEvent = m_event_list.at( i );

			lYmd = ( ulong)( pHisEvent->ymd );

			lHmsms = ( ulong )( pHisEvent->hmsms );

			if( lYmd < beginYmd || ( lYmd == beginYmd && lHmsms < beginHmsms ) ||
				endYmd < lYmd || ( endYmd == lYmd && endHmsms < lHmsms ) )
			{
				continue;
			}

			if( tr( pHisEvent->text ).indexOf( m_fuzzy_string ) < 0 )
				continue;

			his_event = new HIS_EVENT;

			memcpy( his_event, pHisEvent, sizeof( HIS_EVENT ) );
			tmpHisEventList.append( his_event );
		}

		m_event_list.clear();

		m_event_list = tmpHisEventList;
	}
	else
	{
		int totalcount=0;
		m_event_list.clear();

		sprintf( where, "文字 like \'%%%s%%\'", _Q2C( m_pFuzzy_query_lineedit->text() ) );

		if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,true) == CS_FAIL )
		{
			if( his_event ) 
			{
				free( his_event );
			}
		}

		for( i = 0; i < (uint)ScaEveNum; i ++ )
		{
			pHisEvent = new HIS_EVENT;
			memcpy( pHisEvent, &his_event[ i ], sizeof( HIS_EVENT ) );
			m_event_list.append( pHisEvent );

			totalcount++;
			if(totalcount && (totalcount%MAX_EVENT_COUNT)==0) {
// 				if(QMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
// 					QMessageBox::Yes, QMessageBox::No )==QMessageBox::No)
				if(DMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
					QMessageBox::Yes|QMessageBox::No, QMessageBox::No )==QMessageBox::No)
					goto _failout;
			}
		}


		if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,false) == CS_FAIL )
		{
			if( his_event ) 
			{
				free( his_event );
			}
		}

		for( i = 0; i < (uint)ScaEveNum; i ++ )
		{
			pHisEvent = new HIS_EVENT;
			memcpy( pHisEvent, &his_event[ i ], sizeof( HIS_EVENT ) );
			m_event_list.append( pHisEvent );

			totalcount++;
			if(totalcount && (totalcount%MAX_EVENT_COUNT)==0) {
// 				if(QMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
// 					QMessageBox::Yes, QMessageBox::No )==QMessageBox::No)
				if(DMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
					QMessageBox::Yes|QMessageBox::No, QMessageBox::No )==QMessageBox::No)
					goto _failout;
			}
		}
	}

_failout:
	fill_event_table();
	db.Close();

	QApplication::restoreOverrideCursor( );
}

void Hisquery_wnd::slot_event_type_tree_clicked( QTreeWidgetItem* pItem , int column )
{
	if( pItem == NULL )
		return;

	int checked;

	uint state = pItem->checkState( 0 );
	if( state == Qt::PartiallyChecked )
		return;
	else if( state == Qt::Checked )
		checked = true;
	else
		checked = false;

	QTreeWidgetItem *pSubItem,*pSubSubItem,*pChildItem;
	for ( int j =0 ; j < pItem->childCount() ;j++)		
	{
		pSubItem = pItem -> child(j) ;
		pSubItem ->setCheckState( 0 , pItem->checkState( 0 ) );
		if( m_dev_page_item_to_rtu_code_map.contains( pSubItem ) )
		{
			slot_show_station_dev( pSubItem );
		}
		for ( int k =0 ; k < pSubItem->childCount() ;k++ )
		{
			pSubSubItem = pSubItem -> child(k) ;
			pSubSubItem->setCheckState( 0 , pItem->checkState( 0 ));
			for ( int i =0 ; i < pSubSubItem->childCount() ;i++ )
			{
				pChildItem = pSubSubItem -> child(i) ;
				pChildItem->setCheckState( 0 , pItem->checkState( 0 ));
			}
		}
	}

	if( !checked )
		return;

	QTreeWidgetItem *pParentItem = pItem->parent();
	while( pParentItem != NULL )
	{
		pParentItem->setCheckState( 0 , pItem->checkState( 0 ) );
		pParentItem = pParentItem->parent();
	}
}

void Hisquery_wnd::slot_show_station_dev( QTreeWidgetItem* pItem )
{
	if( pItem->childCount() > 0 )
		return;

	QString rtuCode;
	if( !m_dev_page_item_to_rtu_code_map.contains( pItem ) )
	{
		return;
	}

	rtuCode = m_dev_page_item_to_rtu_code_map[ pItem ];

	QStringList devlist;
	QTreeWidgetItem *pSubItem;
	QTreeWidgetItem *pSubSubItem;

	DEVTYPEPARA	devtype;
	int k, p;
	QString desc;

	for( k = 0; k < m_db_para.getDevTypeNum(); k ++ )
	{
		QStringList m_devtypelist ;
		if( !m_db_para.getDevType( k, &devtype ) )
			continue;

		devlist.clear();
		if( !m_db_para.getDevCode( rtuCode, devtype.devtype, 
			devlist ) )
			continue;
		m_devtypelist << devtype.desc ;
		pSubItem = new QTreeWidgetItem( pItem, m_devtypelist) ;
		pSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块 

		for( p = 0; p < devlist.count(); p ++ )
		{
			QStringList m_devdesclist ;
			if( !m_db_para.getDevDesc( devlist[ p ], desc ) )
				continue;
			m_devdesclist << desc ;
			pSubSubItem = new QTreeWidgetItem( pSubItem, m_devdesclist);
			pSubSubItem->setCheckState(0, Qt::Unchecked); // 设定可核取的方块

			m_item_to_dev_code_map[ pSubSubItem ] = devlist[ p ];

			m_dev_code_to_item_map[ devlist[ p ] ] = pSubSubItem;
		}
	}
}

void Hisquery_wnd::slot_print( void )
{
	if( m_pEvent_table->rowCount() <=0 )
		return ;

	m_pPrinter->setFullPage( true ) ;

	QPrintDialog printer_dlg( m_pPrinter , this ) ;

	if ( printer_dlg.exec()) // printer dialog
	{		
		m_pStatus_bar->showMessage( "Printing..." );

		QPainter p;
		if( !p.begin( m_pPrinter ) ) // paint on printer
		{               
			m_pStatus_bar->showMessage( "Printing aborted", 2000 );
			return;
		}

		QPaintDevice *metrics = p.device() ;

		int dpiy = metrics->logicalDpiY(); // 吋

		int margin = (int) ( (2/2.54)*dpiy ); // 2 cm margins, 1吋=2.54厘米

		// 有效打印范围
		QRect body( margin, margin, metrics->width() - 2 * margin, metrics->height() - 2 * margin );

		QRect view( body );

		int table_width = m_pEvent_table->columnWidth(0)+m_pEvent_table->columnWidth(1)+m_pEvent_table->columnWidth(2)+m_pEvent_table->columnWidth(3);

		QRect tblRect( 0, 0, table_width,m_pEvent_table->height() );

		double coef = body.width()/tblRect.width(); // 打印范围适配到窗口范围比例

		int page = 1;

		int i, j;
		QRect rect;
		QFont oldFont = p.font();				// 原打印机字体
		float fsize = oldFont.pointSizeF();		// 原打印机字体大小 点

		fsize = fsize*coef*96.0f/600.0f; // DPI 原来是印刷上的记量单位，意思是每个英寸上，所能印刷的网点数(Dot Per Inch)

		QFont newFont(oldFont);					// 打印字体
		newFont.setPointSizeF(fsize);			// 新打印机字体大小 点

		p.setFont( newFont );		
		QPen curPen = p.pen();
		curPen.setWidth( 2 );
		p.setPen( curPen );

		uint labelwidth = m_pEvent_table->verticalHeader()->width();

		uint offsetHeight = 0;

		for( i = 0; i < m_pEvent_table->rowCount(); i ++ )
		{
			rect = m_pEvent_table->visualItemRect( m_pEvent_table->item(i , 0) );

			if( ( rect.bottom() - offsetHeight ) * coef > body.height() )
			{
				p.drawText( view.right() - p.fontMetrics().width( QString::number( page ) ),view.bottom() + p.fontMetrics().ascent() + 5, QString::number( page ) );
				page ++;
				m_pPrinter->newPage();
				offsetHeight = rect.bottom();
			}

			QString Num = QString::number(i+1, 10) ;
			p.drawText( QRectF(margin , margin + ( rect.top() - offsetHeight ) * coef,labelwidth * coef, rect.height()*coef), Qt::AlignCenter, Num );

			p.drawRect( QRect( margin, margin + ( rect.top() - offsetHeight ) * coef,labelwidth * coef , rect.height() * coef ) );

			for( j = 0; j < m_pEvent_table->columnCount(); j ++ )
			{
				rect = m_pEvent_table->visualItemRect(m_pEvent_table->item( i , j) );
				p.drawText( QRectF(margin + rect.left() * coef + labelwidth*coef, margin + ( rect.top() - offsetHeight ) * coef,rect.width()*coef, rect.height()*coef), Qt::AlignCenter, m_pEvent_table->item( i, j )->text() );
				p.drawRect( QRect( margin + rect.left() * coef + labelwidth*coef, margin + ( rect.top() - offsetHeight ) * coef,rect.width() * coef , rect.height() * coef ) );
			}
		}

		p.setFont( oldFont );
		p.end();

		m_pStatus_bar->showMessage( "Printing completed", 2000 );
	} 
	else	 
	{
		m_pStatus_bar->showMessage( "Printing aborted", 2000 );
	}
}

void Hisquery_wnd::slot_event_output( void )
{
	QString filename = QFileDialog::getSaveFileName( this, tr("Save File"),getenv("NBENV"), QString::null);

	if( m_pEvent_table->rowCount() == 0 )
		return;

	if( filename.isEmpty() )
		return;

	QFile file( filename );

	if( !file.open( QIODevice::WriteOnly ) )
		return;

	QTextStream stream( &file );

	int i,j ;

	for( i = 0; i < m_pEvent_table->rowCount(); i ++ )
	{
		stream << i + 1;
		stream << "\t";
		for( j = 0 ; j < m_pEvent_table->columnCount(); j ++ )
		{
			stream << m_pEvent_table->item( i, j )->text();;
			stream << "\t";
		}

		stream << "\n";
	}

	file.close();
}

void Hisquery_wnd::slot_event_delete( void )
{
// 	if( QMessageBox::question( this , tr( "历史事项删除提示" ) , tr( "是否删除当前选中事项?" ) ,
// 		QMessageBox::Yes , QMessageBox::No ) == QMessageBox::No )
	if(DMessageBox::question( this, tr( "历史事项删除提示" ), tr( "是否删除当前选中事项？" ),
		QMessageBox::Yes|QMessageBox::No, QMessageBox::No )==QMessageBox::No)
	{
		return ;
	}

	DBApi db ;
	CEvtProc  evtproc ;
	FUNC_STR func ;
	CS_DATAFMT* fmt = 0 ;
	HIS_EVENT* pHisEvent ;

	bool bDel_flag = true ;
	Login_widget login_dialog ;
	if( m_pCur_user == NULL )
	{
		while( true )
		{
			if( login_dialog.exec() == QDialog::Accepted )
			{
				USEROPERRIGHT_STRU tmpRight;
				DISPATCHER_STRU    tmpOperInfo;
				login_dialog.get_user_right( tmpRight );
				login_dialog.get_oper_info( tmpOperInfo );
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

				strcpy( sysevent.Object , tmpRight.username );
				sprintf(sysevent.text,"%s在%s节点登录删除事项",tmpOperInfo.describe, 
					ns.GetNodeName( sysevent.grpno, ns.GetHostName( ns.GetHostNo() ) ) );
				evtproc.Save( &sysevent, 1 );

				if( m_pCur_user == NULL )
				{
					m_pCur_user = new USEROPERRIGHT_STRU ;
				}
				login_dialog.get_user_right( *m_pCur_user ) ;
				break ;
			}
			else
			{
				return ;
			}

			if( !m_pCur_user->modifyhisent )
			{
// 				QMessageBox::information(this, _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ) ) ;
				DMessageBox::information(this, _C2Q( "提示" ), _C2Q( "当前用户无事项确认权限!" ) ) ;
				return ;
			}
			else
			{
				break ;
			}
		}	
	}

	if( !m_pCur_user->modifyhisent )
	{
// 		QMessageBox::information( this, tr( "提示" ) , tr( "当前用户无事项编辑权限!" ) ) ;	
		DMessageBox::information( this, tr( "提示" ) , tr( "当前用户无事项编辑权限!" ) ) ;	
		return;
	}

	int OEventf;
	int year;
	int mon;
	db.Open( DB_OP_WR , false , true ) ;
	for( int i = 0 ; i <  m_pEvent_table->rowCount() ; i ++ )
	{
		if( !m_pEvent_table->item( i , 0 )->isSelected() )
			continue ;

		pHisEvent = m_event_list.at( i ) ;

		OEventf = evtproc.IsOptEvt( pHisEvent ) ;

		year = pHisEvent->ymd / 10000 ;
		mon = ( pHisEvent->ymd % 10000 ) / 100 ;
		evtproc.InitDb( db , year , mon , OEventf , &func , &fmt ) ;

		for( int j = 0 ; j < func.colnum ; j ++ )
		{
			func.col[j].select = NO;
			func.col[j].condition = NO;
			if( strcmp( fmt[ j ].name, "年月日" ) == 0 )
			{
				func.col[j].condition = YES;
				func.col[j].val_o.intvalue = pHisEvent->ymd;
			}
			else if( strcmp( fmt[j].name, "时分秒毫秒" ) == 0 )
			{
				func.col[j].condition=YES;
				func.col[j].val_o.intvalue = pHisEvent->hmsms;
			}
			else if( strcmp( fmt[j].name, "文字") == 0 )
			{
				func.col[ j ].condition = YES;
				strcpy( func.col[j].val_o.charvalue, pHisEvent->text );
			}
		}

		if( evtproc.DeleteEvent(db,&func,fmt,OEventf) )
		{
			m_event_list.removeOne( pHisEvent );
// 			QMessageBox::information( this, tr( "提示" ) , tr( "所选事项已删除!" ) ) ;	
			DMessageBox::information( this, tr( "提示" ) , tr( "所选事项已删除!" ) ) ;	
		}

		if( fmt )
		{
			free(fmt);
			fmt = NULL;
		}
	}

	db.Close();

	fill_event_table() ;

	if( bDel_flag )
	{
		delete m_pCur_user;
		m_pCur_user = NULL;
	}
}

void Hisquery_wnd::slot_exit( void )
{
	qApp->closeAllWindows() ;
}

void Hisquery_wnd::slot_all_type( void )
{
	QTreeWidgetItem *pItem;

	if( m_pAll_type_button->isChecked() )
	{
		for ( int i = 0 ; i <  m_pEvent_type_select_tree->topLevelItemCount() ; i++ )
		{
			pItem = m_pEvent_type_select_tree->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Checked );
			slot_event_type_tree_clicked( pItem ,0 );
		}
	}
	else
	{		
		for ( int i = 0 ; i <  m_pEvent_type_select_tree->topLevelItemCount() ; i++ )
		{
			pItem = m_pEvent_type_select_tree->topLevelItem(i) ;
			pItem->setCheckState( 0 , Qt::Unchecked );
			slot_event_type_tree_clicked( pItem ,0 );
		}
	}
}

void Hisquery_wnd::slot_all_station( void )
{
	QTreeWidgetItem *pItem;

	if( m_pAll_station_button->isChecked() )
	{	
		for( int i = 0 ; i < m_pStation_select_tree->topLevelItemCount() ; i++ )
		{
			pItem = m_pStation_select_tree->topLevelItem(i) ;
			if (!pItem->isDisabled())
			{
				pItem->setCheckState( 0 , Qt::Checked );
				slot_event_type_tree_clicked( pItem ,0);
			}
		}
	}
	else
	{
		for( int i = 0 ; i < m_pStation_select_tree->topLevelItemCount() ; i++ )
		{
			pItem = m_pStation_select_tree->topLevelItem(i) ;
			if (!pItem->isDisabled())
			{
				pItem->setCheckState( 0 , Qt::Unchecked );
				slot_event_type_tree_clicked( pItem ,0);
			}
		}
	}
}

void Hisquery_wnd::find_event_by_eventtype( void )
{
	uint i, j, sort;
	HIS_EVENT *pHisEvent, *his_event;
	ulong		lYmd,lHmsms;
	char where[ 8192 ];
	char tmpstr[ 1024 ];

	DBApi	db;

	int ScaEveNum, totalcount=0;

	if( db.Open( DB_OP_R, false, false ) < 0 )
	{
// 		QMessageBox::information( this, tr( "告警" ), tr( "数据库打开失败!" ) );
		DMessageBox::information( this, tr( "告警" ), tr( "数据库打开失败!" ) );
		return;
	}

	if( m_event_type_list.count() == 0 )
		return;

	if( m_pSearch_in_result_checkbox->isChecked() )
	{
		QList<HIS_EVENT *>		tmpHisEventList;
		ulong	beginYmd , beginHmsms, endYmd , endHmsms;


		beginYmd = m_beginTime.year * 10000l + m_beginTime.mon * 100l + m_beginTime.day;
		beginHmsms = m_beginTime.hour * 10000000l + m_beginTime.min * 100000l +m_beginTime.sec * 1000 + m_beginTime.ms;

		endYmd = m_endTime.year * 10000l + m_endTime.mon * 100l + m_endTime.day;
		endHmsms = m_endTime.hour * 10000000l + m_endTime.min * 100000l +m_endTime.sec * 1000 + m_endTime.ms;

		for( i = 0 ; i < m_event_list.count(); i ++ )
		{
			pHisEvent = m_event_list.at( i );

			if( m_rtu_code_list.count() > 0 && !tr( pHisEvent->Object ).isEmpty() &&
				m_rtu_code_list.indexOf( pHisEvent->Object ) == -1 )
			{
				continue;
			}

			if( m_event_type_list.indexOf( pHisEvent->type ) == -1 )
			{
				continue;
			}

			lYmd = ( ulong)( pHisEvent->ymd );

			lHmsms = ( ulong )( pHisEvent->hmsms );

			if( lYmd < beginYmd || ( lYmd == beginYmd && lHmsms < beginHmsms ) ||
				endYmd < lYmd || ( endYmd == lYmd && endHmsms < lHmsms ) )
			{
				continue;
			}

			his_event = new HIS_EVENT;

			memcpy( his_event, pHisEvent, sizeof( HIS_EVENT ) );
			tmpHisEventList.append( his_event );
		}

		m_event_list = tmpHisEventList;
	}
	else
	{
		uint types[ 500 ];
		uint typenum, typecount, totalcount=0;

		m_event_list.clear();

		for( sort = 1; sort < m_db_para.getEventSortCount(); sort ++ )
		{
			if( !m_db_para.getEventTypes( sort, types, typenum ) )
			{
				continue;
			}
			typecount = 0;
			for( i = 0; i < typenum; i ++ )
			{
				if( m_event_type_list.indexOf( types[ i ] ) == m_event_type_list.size() || m_event_type_list.indexOf( types[ i ] ) == -1)
					continue;
				typecount ++;
			}
			if( typecount == 0 )
				continue;

			where[ 0 ] = '\0';

			if( typecount == typenum )
			{
				if( where[ 0 ] == '\0' )
				{
					sprintf( tmpstr, "分类号=%d", sort );
				}
				else
				{
					sprintf( tmpstr, " and 分类号=%d", sort );
				}
				strcat( where, tmpstr );
			}
			else
			{
				typecount = 0;
				for( i = 0; i < typenum; i ++ )
				{
					if( m_event_type_list.indexOf( types[ i ] ) == m_event_type_list.size() || m_event_type_list.indexOf( types[ i ] ) == -1)
						continue;

					if( typecount == 0 )
					{
						if( where[ 0 ] == '\0' )
						{
							sprintf( tmpstr, "类型 in ( %d", types[ i ] );
						}
						else
						{
							sprintf( tmpstr, " and ( 类型 in ( %d", types[ i ] );
						}
					}
					else
					{
						sprintf( tmpstr, ",%d", types[ i ] );
					}

					typecount ++;

					strcat( where, tmpstr );

				}

				strcat( where, " )");
			}
			his_event = NULL;
			ScaEveNum = 0;

			int sqllen = strlen( where );

			if( sqllen > 4000 )
			{
// 				QMessageBox::warning( this, tr( "提示" ), tr( "条件过长,请分类查询!" ) );
				DMessageBox::warning( this, tr( "提示" ), tr( "条件过长,请分类查询!" ) );
				goto _failout;
			}

			if( ( sort == ENTCLASS_DEVICE ) || ( sort == ENTCLASS_MAINT ) || 
				( sort == ENTCLASS_WEB ) || ( sort == ENTCLASS_NOTICE ) ||
				( sort == ENTCLASS_OPERATION ) )
			{
				if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,true) == CS_FAIL )
				{
					if( his_event ) 
					{
						free( his_event );
					}
					continue;
				}
			}
			else
			{
				if(db.SelectEvent( m_beginTime, m_endTime,where,&ScaEveNum,&his_event,false) == CS_FAIL )
				{
					if( his_event ) 
					{
						free( his_event );
					}
					continue;
				}
			}
			for( i = 0; i < (uint)ScaEveNum; i ++ )
			{
				if( m_rtu_code_list.count() > 0 && !tr( his_event[ i ].Object ).isEmpty() &&
					!m_rtu_code_list.contains( his_event[ i ].Object ) )
				{
					continue;
				}

				pHisEvent = new HIS_EVENT;
				memcpy( pHisEvent, &his_event[ i ], sizeof( HIS_EVENT ) );

				for( j = 0; j < m_event_list.count(); j ++ )
				{
					if( ( pHisEvent->ymd < m_event_list.at( j )->ymd ) || 
						( pHisEvent->ymd == m_event_list.at( j )->ymd &&
						pHisEvent->hmsms < m_event_list.at( j )->hmsms ) )
					{
						break;
					}
				}
				m_event_list.insert( j, pHisEvent );

				totalcount++;
				if(totalcount && (totalcount % MAX_EVENT_COUNT) == 0) 
				{
// 					if(QMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
// 						QMessageBox::Yes, QMessageBox::No )==QMessageBox::No)
					if(DMessageBox::question( this, tr( "历史事项查询提示" ), tr( "查询结果太多(>%1条)，还要继续吗？" ).arg(MAX_EVENT_COUNT),
						QMessageBox::Yes|QMessageBox::No , QMessageBox::No )==QMessageBox::No)					
					{
						free( his_event );
						his_event = NULL;
						goto _failout;
					}
				}
			}

			if( his_event )
			{
				free( his_event );
				his_event = NULL;
			}
		}
	}

_failout:
	fill_event_table();
	db.Close();
}

void Hisquery_wnd::fill_event_table( void )
{
	HIS_EVENT *pHisEvent;
	QString text;
	uint i;
	m_pEvent_table->clearContents();
	m_pEvent_table->setRowCount(0);
	m_pEvent_table->setRowCount( m_event_list.count() );

	for( i = 0; i < m_event_list.count(); i ++ )
	{
		pHisEvent = m_event_list.at( i );
		if( !m_db_para.getEventTypeDesc( pHisEvent->type, text ) )
		{
			continue;
		}
		m_pEvent_table->setItem( i, 0, new QTableWidgetItem(text) );

		ulong lYmd = (ulong)(pHisEvent->ymd);
		int Year=(int)(lYmd/(100l*100l));
		int Month=(int)((lYmd%(100l*100l))/100);
		int Day=(int)((lYmd%(100l*100l))%100);

		ulong lTmp = (ulong)(pHisEvent->hmsms);
		int Hour = (int)(lTmp/(100l*100l*1000l));
		int Min = (int)((lTmp%(100l*100l*1000l))/(100l*1000l));
		int Sec = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))/1000l);
		int Ms = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))%1000l);
		text.sprintf( "%04d.%02d.%02d--%02d:%02d:%02d:%03d",Year,Month,Day,Hour, Min, Sec, Ms);
		m_pEvent_table->setItem( i, 1, new QTableWidgetItem(text) );

		text = tr( pHisEvent->text );
		m_pEvent_table->setItem( i, 2, new QTableWidgetItem(text) );

		text.sprintf( "%.2f", ( float )( pHisEvent->val) );
		m_pEvent_table->setItem( i, 3, new QTableWidgetItem(text) );

		if( !m_db_para.getStaDesc( tr( pHisEvent->Object ),text ) )
		{
			text = tr( pHisEvent->Object );
		}
		m_pEvent_table->setItem( i, 4, new QTableWidgetItem(text) );
	}
}

void Hisquery_wnd::slot_modify_event( QTableWidgetItem* pItem )
{
	int item_row = m_pEvent_table->row( pItem ) ;
	bool bDel_flag = true ;
	if( m_pCur_user == NULL )
	{
		Login_widget login_dialog ;
		if( login_dialog.exec() == QDialog::Accepted )
		{
			USEROPERRIGHT_STRU tmpRight;
			DISPATCHER_STRU    tmpOperInfo;
			login_dialog.get_user_right( tmpRight );
			login_dialog.get_oper_info( tmpOperInfo );
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

			strcpy( sysevent.Object , tmpRight.username );
			sprintf(sysevent.text,"%s在%s节点登录删除事项",tmpOperInfo.describe, 
				ns.GetNodeName( sysevent.grpno, ns.GetHostName( ns.GetHostNo() ) ) );
			evtproc.Save( &sysevent, 1 );
			if( m_pCur_user == NULL )
			{
				m_pCur_user = new USEROPERRIGHT_STRU;
			}

			login_dialog.get_user_right( *m_pCur_user ) ;
		}
		else
		{
			return ;
		}

		if( !m_pCur_user->modifyhisent )
		{
// 			QMessageBox::information( this , tr( "提示" ), tr( "当前用户无事项编辑权限!" ) ) ;
			DMessageBox::information( this , tr( "提示" ), tr( "当前用户无事项编辑权限!" ) ) ;
			delete m_pCur_user;
			m_pCur_user = NULL;
			return;
		}
	}

	if( !m_pCur_user->modifyhisent )
	{
// 		QMessageBox::information( this , tr( "提示" ) , tr( "当前用户无事项编辑权限!" ) ) ;	
		DMessageBox::information( this , tr( "提示" ) , tr( "当前用户无事项编辑权限!" ) ) ;	
		return;
	}

	Event_modify_dialog dlg( m_event_list.at( item_row ), this );

	dlg.exec() ;

	if( bDel_flag )
	{
		delete m_pCur_user ;
		m_pCur_user = NULL ;
	}
}

void Hisquery_wnd::closeEvent( QCloseEvent *event )
{
// 	if( QMessageBox::question( this , tr("提示") , tr("是否退出事项查询?") , QMessageBox::Yes , QMessageBox::No) == QMessageBox::No )
	if( DMessageBox::question( this , tr("提示") , tr("是否退出事项查询?") , QMessageBox::Yes|QMessageBox::No , QMessageBox::No) == QMessageBox::No )
	{
		event->ignore() ;
		return ;
	}
	else
	{
		event->accept() ;
	}
}

void Hisquery_wnd::timerEvent( QTimerEvent *event )
{
	if(event->timerId() == m_proc_time_id )
	{
		m_proc_man.CheckParentProc() ;
	}
}
