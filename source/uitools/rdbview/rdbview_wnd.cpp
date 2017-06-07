#include "rdbview_wnd.h"
#include "rdb_scada_dialog.h"
#include "rdb_ycyx_dialog.h"

#include "rwxml.h"
#include "db/dbapi.h"
#include "net/netapi.h"
#include "rdb/ldb.h"
#include "uitools/dmessagebox.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QTimer>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include <sys/stat.h>
#include <string.h>

DataInfo Info ;
DBApi db ;
NetDbg dbg ;
NetStatus netcfg ;
SHOWCFG* ShowCfg; //参数显示设置的结构

int cur_computer_group_no = 1 ;//当前机器组号
bool bFlag = false ;

char *RootType[] = {
	"实时遥测数据", "实时遥信数据","实时电度数据","SCADA实时数据",NULL
};
char *RealYcColName[] ={
	"代码","遥测描述","终端序号","遥测序号","实时值","是否锁定","是否计算","遥测系数","是否可疑",NULL
};
char *RealYxColName[] ={
	"代码","遥信描述","终端序号","遥信序号","实时状态","是否锁定","是否计算",NULL
};
char *RealKwhColName[] ={
	"代码","电度描述","终端序号","电度序号","日电度值","是否锁定","是否计算",NULL
};

Rdbview_wnd::Rdbview_wnd( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	m_bIs_connected = false ;
	m_bGet_dict = false ;
	m_rsql_username[ 0 ] = '\0' ;
	m_rsql_password[ 0 ] = '\0' ;
	m_rdb_type = SERVER ;
	m_pRsql_server = NULL ;
	m_pSer_dict = NULL ;
	m_pRsql_local = NULL ;
	m_pLocal_dict = NULL ;
	m_bRd_file = false ;
	m_yc_num = 0 ;
	m_yx_num = 0 ;
	m_yk_num = 0 ;
	m_cal_num = 0 ;
	m_bButton_type = false ;

	m_pSql_statement = NULL ;
	m_err_code = OPERATION_OK ;
	m_rdb_ret_info.ColumnNum = 0 ;
	m_rdb_ret_info.OldColumnNum = 0 ;
	m_rdb_ret_info.RcdNum = 0 ;

	for ( int i = 0 ; i < MAXCOLUMNNUM ; i++ )
	{
		m_rdb_ret_info.ColumnLen[ i ] = 0 ;
	}
	for ( int i = 0 ; i < MAXCOLUMNNUM ; i++ )
	{
		m_rdb_ret_info.ColumnName[ i ][ 0 ] = '\0' ;
	}

	m_rdb_ret_info.pretinfo = NULL ;
	//sql查询用到的

	QString inifilename = getenv( "NBENV" ) ;
	dbg.RegProc("实时数据查看") ;
	if( !db.Open( DB_OP_R, FALSE, FALSE ) )
	{
		printf("读取参数库失败");
		m_bRd_file = true ;
	}

	ini_rdbview_wnd() ;

	read_stationarea_data() ;
	refresh_rdb_tree() ;
	m_pFresh_timer = new QTimer( this ) ;
	connect( m_pFresh_timer , SIGNAL( timeout() ) , this , SLOT( slot_timer() ) ) ;
	m_pFresh_timer->start( ( ShowCfg->FreshTime ) * 1000 ) ;
	slot_sql_checkbox( 0 ) ;

	resize( 800 , 650 ) ;
	slot_change_button_type() ;
}

Rdbview_wnd::~Rdbview_wnd()
{
	if ( m_pRsql_server ) delete m_pRsql_server ;

	if ( m_pSer_dict ) delete m_pSer_dict ;

	if ( m_pLocal_dict ) delete m_pLocal_dict ;

	if ( m_pRsql_local )  delete m_pRsql_local ;

	if ( m_rdb_ret_info.pretinfo )  free( m_rdb_ret_info.pretinfo ) ;

	dbg.UnRegProc() ;
	db.Close() ;

	if( ShowCfg )
	{
		delete( ShowCfg ) ;
		ShowCfg = NULL ;
	}
}

void Rdbview_wnd::ini_rdbview_wnd( void )
{
	setWindowTitle( tr( "D6000实时库工具" ) ) ;
	setWindowIcon( QIcon( ":/res/rdbview.png" ) ) ;
	QFont font ;
	font.setFamily( tr( "微软雅黑" ) ) ;
	font.setPointSize( 10 ) ;

	m_pCentral_widget = new QWidget( this ) ;
	m_pToolbar_widget = new QFrame( m_pCentral_widget ) ;
	m_pToolbar_widget->setObjectName( tr( "toolbarwidget" ) ) ;
	m_pNormal_frame = new QFrame( m_pCentral_widget ) ;
	m_pSql_frame = new QFrame( m_pCentral_widget ) ;
	QVBoxLayout* pVbox_layout = new QVBoxLayout( m_pCentral_widget ) ;
	pVbox_layout->addWidget( m_pToolbar_widget ) ;
	pVbox_layout->addWidget( m_pNormal_frame ) ;
	pVbox_layout->addWidget( m_pSql_frame ) ;
	pVbox_layout->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	setCentralWidget( m_pCentral_widget ) ;
	m_pCentral_widget->setLayout( pVbox_layout ) ;

	m_pRdb_tree = new QTreeWidget( m_pNormal_frame ) ;
	m_pRdb_tree->setHeaderLabel( tr( "实时库" ) ) ;
	m_pRdb_tree->setFont( font ) ;
	connect( m_pRdb_tree , SIGNAL( itemClicked( QTreeWidgetItem* , int ) ), this, SLOT( slot_select_change( QTreeWidgetItem* ) ) ) ;
	m_pNormal_table = new Rdb_table( m_pNormal_frame ) ;
	QSplitter* pSplitter = new QSplitter( m_pNormal_frame ) ;

	QVBoxLayout* pVbox_layout_1 = new QVBoxLayout( m_pNormal_frame ) ;
	m_pNormal_frame->setLayout( pVbox_layout_1 ) ;
	pVbox_layout_1->addWidget( pSplitter ) ;
	pVbox_layout_1->setContentsMargins( 0 , 0 , 0 , 0 ) ;
	pSplitter->addWidget( m_pRdb_tree ) ;
	pSplitter->addWidget( m_pNormal_table ) ;
	m_pNormal_table->setMinimumSize( QSize( 0, 0 ) );
	m_pNormal_table->setRowCount( 0 );
	m_pNormal_table->setColumnCount( 0 );
	m_pNormal_table->setSelectionBehavior( QAbstractItemView::SelectRows ) ;	//单行选中
	m_pNormal_table->setAlternatingRowColors( true ) ;	//隔行变色
	m_pNormal_table->setSortingEnabled( false ) ;	//禁止选中光标自动刷新时乱动 mod by lbh 2014年3月5日 15:55:21
	pSplitter->setStretchFactor( 0 , 1 ) ;
	pSplitter->setStretchFactor( 1 , 3 ) ;

	m_pSql_textedit = new QTextEdit( m_pSql_frame ) ;
	m_pSql_table = new Rdb_table( m_pSql_frame ) ;
	connect( m_pSql_textedit , SIGNAL( textChanged() ) , this , SLOT( slot_sql_change() ) );
	QSplitter* pSplitter_1 = new QSplitter( Qt::Vertical , m_pSql_frame ) ;
	pSplitter_1->addWidget( m_pSql_textedit ) ;
	pSplitter_1->addWidget( m_pSql_table ) ;
	pSplitter_1->setStretchFactor( 0 , 1 ) ;
	pSplitter_1->setStretchFactor( 1 , 4 ) ;
	QVBoxLayout* pVbox_layout_2 = new QVBoxLayout( m_pSql_frame ) ;
	m_pSql_frame->setLayout( pVbox_layout_2 ) ;
	pVbox_layout_2->addWidget( pSplitter_1 ) ;
	pVbox_layout_2->setContentsMargins( 0 , 0 , 0 , 0 ) ;

	ini_rdbview_toolbar() ;
	ini_status_bar() ;
	ini_show_cfg() ;

	slot_change_button_type() ;
}

void Rdbview_wnd::ini_rdbview_toolbar( void )
{
	QFont font ;
	font.setFamily( tr( "微软雅黑" ) ) ;
	font.setPointSize( 10 ) ;
	m_pToolbar_widget->setFont( font ) ;

	QHBoxLayout* pHBox_layout = new QHBoxLayout( m_pToolbar_widget ) ;
	pHBox_layout->setSpacing( 5 ) ;
	m_pToolbar_widget->setLayout( pHBox_layout ) ;

	m_pFileout_button = new QToolButton( m_pToolbar_widget ) ;
	m_pFileout_button->setObjectName( tr( "fileoutbutton" ) ) ;
	m_pFileout_button->setText( tr( "导出数据" ) ) ;
	m_pFileout_button->setToolTip( tr( "导出数据" ) ) ;
	m_pFileout_button->setIcon( QIcon( ":/res/outputdata.png" ) ) ;
	m_pFileout_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pFileout_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pFileout_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pFileout_button ) ;
	connect( m_pFileout_button , SIGNAL( clicked() ) , this , SLOT( slot_file_out() ) ) ;

	m_pReaddata_button = new QToolButton( m_pToolbar_widget ) ;
	m_pReaddata_button->setObjectName( tr( "readdatabutton" ) ) ;
	m_pReaddata_button->setText( tr( "读取数据" ) ) ;
	m_pReaddata_button->setToolTip( tr( "读取数据" ) ) ;
	m_pReaddata_button->setIcon( QIcon( ":/res/readdata.png" ) ) ;
	m_pReaddata_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pReaddata_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pReaddata_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pReaddata_button ) ;
	connect( m_pReaddata_button , SIGNAL( clicked() ) , this , SLOT( slot_read_data() ) ) ;

	m_pYcyx_display_set_button = new QToolButton( m_pToolbar_widget ) ;
	m_pYcyx_display_set_button->setObjectName( tr( "viewsettingbutton" ) ) ;
	m_pYcyx_display_set_button->setText( tr( "参数设置" ) ) ;
	m_pYcyx_display_set_button->setToolTip( tr( "参数设置" ) ) ;
	m_pYcyx_display_set_button->setIcon( QIcon( ":/res/viewsetting.png" ) ) ;
	m_pYcyx_display_set_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pYcyx_display_set_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pYcyx_display_set_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pYcyx_display_set_button ) ;
	connect( m_pYcyx_display_set_button , SIGNAL( clicked() ) , this , SLOT( slot_ycyx_set() ) ) ;

	m_pScada_display_set_button = new QToolButton( m_pToolbar_widget ) ;
	m_pScada_display_set_button->setObjectName( tr( "rdbsettingbutton" ) ) ;
	m_pScada_display_set_button->setText( tr( "SCADA实时设置" ) ) ;
	m_pScada_display_set_button->setToolTip( tr( "SCADA实时设置" ) ) ;
	m_pScada_display_set_button->setIcon( QIcon( ":/res/rdbsetting.png" ) ) ;
	m_pScada_display_set_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pScada_display_set_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pScada_display_set_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pScada_display_set_button ) ;
	connect( m_pScada_display_set_button , SIGNAL( clicked() ) , this , SLOT( slot_scada_set() ) ) ;

	m_pRead_data_info_button = new QToolButton( m_pToolbar_widget ) ;
	m_pRead_data_info_button->setObjectName( tr( "readdatainfobutton" ) ) ;
	m_pRead_data_info_button->setText( tr( "数据读取信息" ) ) ;
	m_pRead_data_info_button->setToolTip( tr( "数据读取信息" ) ) ;
	m_pRead_data_info_button->setIcon( QIcon( ":/res/readdatainfo.png" ) ) ;
	m_pRead_data_info_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pRead_data_info_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pRead_data_info_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pRead_data_info_button ) ;
	connect( m_pRead_data_info_button , SIGNAL( clicked() ) , this , SLOT( slot_read_data_info() ) ) ;

	m_pIsSql_checkbox = new QCheckBox( tr( "SQL查询" ) , m_pToolbar_widget ) ;
	connect( m_pIsSql_checkbox , SIGNAL( stateChanged( int ) ) , this , SLOT( slot_sql_checkbox( int ) ) ) ;
	pHBox_layout->addWidget( m_pIsSql_checkbox ) ;

	m_pRdb_select_combobox = new QComboBox( m_pToolbar_widget ) ;
	m_pRdb_select_combobox->addItem( tr( "服务器实时库" ) ) ;
	m_pRdb_select_combobox->addItem( tr( "本地实时库" ) ) ;
	pHBox_layout->addWidget( m_pRdb_select_combobox ) ;
	connect( m_pRdb_select_combobox , SIGNAL( activated( const QString& ) ) , this , SLOT( slot_rdb_select_change() ) );

	m_pRdb_connect_button = new QToolButton( m_pToolbar_widget ) ;
	m_pRdb_connect_button->setObjectName( tr( "rdbconnectbutton" ) ) ;
	m_pRdb_connect_button->setText( tr( "实时库连接" ) ) ;
	m_pRdb_connect_button->setToolTip( tr( "实时库连接" ) ) ;
	m_pRdb_connect_button->setIcon( QIcon( ":/res/rdbconnect.png" ) ) ;
	m_pRdb_connect_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pRdb_connect_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pRdb_connect_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pRdb_connect_button ) ;
	connect( m_pRdb_connect_button , SIGNAL( clicked() ) , this , SLOT( slot_sql_connect() ) ) ;

	m_pRdb_disconnect_button = new QToolButton( m_pToolbar_widget ) ;
	m_pRdb_disconnect_button->setObjectName( tr( "rdbdisconnectbutton" ) ) ;
	m_pRdb_disconnect_button->setText( tr( "断开连接" ) ) ;
	m_pRdb_disconnect_button->setToolTip( tr( "断开连接" ) ) ;
	m_pRdb_disconnect_button->setIcon( QIcon( ":/res/disconnect.png" ) ) ;
	m_pRdb_disconnect_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pRdb_disconnect_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pRdb_disconnect_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pRdb_disconnect_button ) ;
	connect( m_pRdb_disconnect_button , SIGNAL( clicked() ) , this , SLOT( slot_sql_disconnect() ) ) ;

	m_pRdb_sqlrun_button = new QToolButton( m_pToolbar_widget ) ;
	m_pRdb_sqlrun_button->setObjectName( tr( "rdbsqlrunbutton" ) ) ;
	m_pRdb_sqlrun_button->setText( tr( "执行语句" ) ) ;
	m_pRdb_sqlrun_button->setToolTip( tr( "执行语句" ) ) ;
	m_pRdb_sqlrun_button->setIcon( QIcon( ":/res/runsql.png" ) ) ;
	m_pRdb_sqlrun_button->setIconSize( QSize( 16 , 16 ) ) ;
	m_pRdb_sqlrun_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pRdb_sqlrun_button->setFont( font ) ;
	pHBox_layout->addWidget( m_pRdb_sqlrun_button ) ;
	connect( m_pRdb_sqlrun_button , SIGNAL( clicked() ) , this , SLOT( slot_sql_run() ) ) ;

	QSpacerItem* pHorizontal_spacer = new QSpacerItem( 40 , 20 , QSizePolicy::Expanding , QSizePolicy::Minimum ) ;
	pHBox_layout->addItem( pHorizontal_spacer ) ;

	m_pButton_type_button = new QToolButton( m_pToolbar_widget ) ;
	m_pButton_type_button->setObjectName( tr( "buttontypebutton" ) ) ;
	connect( m_pButton_type_button , SIGNAL( clicked() ) , this , SLOT( slot_change_button_type() ) ) ;
	pHBox_layout->addWidget( m_pButton_type_button ) ;

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QToolButton\
						  {\
						  background-image: url(:/res/back_normal.png);\
						  opacity: 0.6;\
						  background: #6E7D8F;\
						  border: 0px solid #FFFFFF;\
						  border-radius: 3px;\
						  font: 14px;\
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
						  background: #D5D5D5;\
						  border: 0px solid #FFFFFF;\
						  border-radius: 3px;\
						  }\
						  #toolbarwidget\
						  {\
						  spacing:5px;\
						  background: #FAFAFA;\
						  min-height: 64px;\
						  max-height:64px;\
						  border: 1px solid #E5E5E5;\
						  }\
						  QTableWidget \
						  {\
						  font-family: Microsoft YaHei;\
						  font-size: 12px;\
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
						  QPushButton\
						  {\
						  background:#4A90E2;\
						  border-radius: 2px;\
						  font-family: Microsoft YaHei;\
						  font-size: 13px;\
						  color: white;\
						  min-width:75px;\
						  min-height:25px;\
						  }\
						  QPushButton:hover\
						  {\
						  background:#0C2543;\
						  }\
						  QCheckBox\
						  {\
						  font-family: Microsoft YaHei;\
						  font-size: 13px;\
						  color:#4A4A4A;\
						  }\
						  QCheckBox::indicator:unchecked\
						  {\
						  image:url(:/res/checkbox_unchecked.png);\
						  }\
						  QCheckBox::indicator:checked\
						  {\
						  image:url(:/res/checkbox_checked.png);\
						  }\
						  QCheckBox::indicator:disabled\
						  {\
						  image:url(:/res/checkbox_disable.png);\
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
						  QComboBox:disabled \
						  {\
						  background: #D5D5D5;\
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
						  QListView,QTreeView\
						  {\
						  font-family: Microsoft YaHei;\
						  font-size: 13px;\
						  color:#4A4A4A;\
						  border: 1px solid #E5E5E5;\
						  }\
						  QListView::item:hover,QTreeView::item:hover \
						  {\
						  background-color: wheat;\
						  }\
						  QListView::item:selected,QTreeView::item:selected \
						  {\
						  background-color: lightblue;\
						  }\
						  QListView::indicator:unchecked\
						  {\
						  image:url(:/res/checkbox_unchecked.png);\
						  }\
						  QListView::indicator:checked\
						  {\
						  image:url(:/res/checkbox_checked.png);\
						  }\
						  QSpinBox \
						  {\
						  font-family: Microsoft YaHei;\
						  font-size: 13px;\
						  color:#4A4A4A;\
						  }\
						  QSpinBox::up-button \
						  {\
						  subcontrol-origin: padding;\
						  subcontrol-position: right;\
						  width: 24px;\
						  height: 24px;\
						  border-width: 3px;\
						  }\
						  QSpinBox::up-arrow \
						  {\
						  image: url(:/res/add.png);\
						  width: 18px;\
						  height: 18px;\
						  }\
						  QSpinBox::down-button \
						  {\
						  subcontrol-origin: border;\
						  subcontrol-position: left;\
						  width: 24px;\
						  height: 24px;\
						  border-width: 3px;\
						  }\
						  QSpinBox::down-arrow \
						  {\
						  image: url(:/res/remove.png);\
						  width: 18px;\
						  height: 18px;\
						  }\
						  ") ;

	setStyleSheet( buttonStyleSheet ) ;

// 	QHeaderView* pHeaderView = m_pNormal_table->horizontalHeader() ;
// 	buttonStyleSheet = tr( "QHeaderView::section\
// 						   {\
// 						   font-family:Microsoft YaHei;\
// 						   height:25px;\
// 						   min-width:75px;\
// 						   padding: 4px;\
// 						   color: #0F1923;\
// 						   border: #567dbc;\
// 						   background:#F1F1F1;\
// 						   }\
// 						   ") ;
// 	pHeaderView->setStyleSheet( buttonStyleSheet ) ;

// 	pHeaderView = m_pSql_table->horizontalHeader() ;
// 	pHeaderView->setStyleSheet( buttonStyleSheet ) ;

// 	pHeaderView = m_pNormal_table->verticalHeader() ;
// 	buttonStyleSheet = tr( "QHeaderView::section\
// 						   {\
// 						   font-family:Microsoft YaHei;\
// 						   color: #0F1923;\
// 						   border: 5px #567dbc;\
// 						   background:#F1F1F1;\
// 						   }\
// 						   ") ;
// 	pHeaderView->setStyleSheet( buttonStyleSheet ) ;

// 	pHeaderView = m_pSql_table->verticalHeader() ;
// 	pHeaderView->setStyleSheet( buttonStyleSheet ) ;
}

void Rdbview_wnd::ini_status_bar( void )
{
	m_pStatus_bar = new QStatusBar( this ) ;
	this->setStatusBar( m_pStatus_bar ) ;

	m_pRead_info_label = new QLabel( m_pStatus_bar ) ;
	m_pRead_info_label->setMinimumSize( 150 , 21 ) ;
	m_pStatus_bar->addWidget( m_pRead_info_label , 1 ) ;
	m_pRead_info_label->setText( tr( "数据读取信息" ) );

	m_pStatus_label = new QLabel( m_pStatus_bar ) ;
	m_pStatus_label->setMinimumSize( 150 , 21 ) ;
	m_pStatus_bar->addWidget( m_pStatus_label , 1 ) ;
	m_pStatus_label->setText( tr( "" ) );

	m_pSelect_label = new QLabel( m_pStatus_bar ) ;
	m_pSelect_label->setMinimumSize( 150 , 21 ) ;
	m_pStatus_bar->addWidget( m_pSelect_label , 1 ) ;
	m_pSelect_label->setText( tr( "数据选取信息" ) );
}

void Rdbview_wnd::ini_show_cfg( void )
{
	int value ;
	bool check ;
	ShowCfg = NULL ;
	ShowCfg = new SHOWCFG ;
	ShowCfg->FreshTime = 5 ;
	ShowCfg->FreshFlag = true ;
	ShowCfg->ShowYcCalcult = true ;
	ShowCfg->ShowYxCalcult = true ;
	ShowCfg->ShowYxManset = true ;
	ShowCfg->ShowYcManset = true ;
	ShowCfg->ShowYcxs = true ;
	ShowCfg->ShowYcKy = true ;
	ShowCfg->ShowKwhTableCode = false ;
	ShowCfg->OrderByStation = false ;

	char file_name[ 256 ] ;
// 	sprintf( file_name ,"%s/ini/rdbview.ini" ,getenv( "NBENV" ) ) ;
// 	ini_RW* pini = new ini_RW( file_name ) ;
	sprintf( file_name ,"%s/resource/xml/rdbview.xml" ,getenv( "NBENV" ) ) ;
	Xml_RW* pini = new Xml_RW( file_name ) ;

	if( SUCCESS == pini->get_int( "系统设置" , "刷新时间" , value ) )
	{	
		ShowCfg->FreshTime = value ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否刷新" , check ) )
	{	
		ShowCfg->FreshFlag = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥测计算标志" , check ) )
	{
		ShowCfg->ShowYcCalcult = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥信计算标志" , check ) )
	{		
		ShowCfg->ShowYxCalcult = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥信锁定" , check ) )
	{		
		ShowCfg->ShowYxManset = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥测锁定" , check ) )
	{			
		ShowCfg->ShowYcManset = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥测系数" , check ) )
	{			
		ShowCfg->ShowYcxs = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示遥测可疑" , check ) )
	{		
		ShowCfg->ShowYcKy = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否显示表码" , check ) )
	{
		ShowCfg->ShowKwhTableCode = check ;
	}
	if( SUCCESS == pini->get_bool( "系统设置" , "是否按厂站检索" , check ) )
	{	
		ShowCfg->OrderByStation = FALSE ;
	}

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}
}

void Rdbview_wnd::slot_sql_checkbox( int sql_state )
{
	if ( sql_state == Qt::Unchecked )
	{
		m_pSql_frame->hide() ;
		m_pNormal_frame->show() ;
		m_pRdb_connect_button->setEnabled( false ) ;
		m_pRdb_disconnect_button->setEnabled( false ) ;
		m_pRdb_sqlrun_button->setEnabled( false ) ;
		m_pFileout_button->setEnabled( true ) ;

		char tmp_text[ 200 ] ;
		m_pRdb_tree->setCurrentItem( m_pRdb_tree->topLevelItem( 0 ) ) ;
		m_pRead_info_label->setText( tr( "数据读取信息:无有效数据" ) ) ;
		sprintf( tmp_text , "遥测:%d个,遥信:%d个,遥控:%d个,计算量:%d个" , m_yc_num , m_yx_num , m_yk_num , m_cal_num ) ;
		m_pStatus_label->setText( tr( tmp_text ) ) ;
		m_pFresh_timer->start( ( ShowCfg->FreshTime ) * 1000 ) ;
	}
	else
	{
		m_pNormal_frame->hide() ;
		m_pSql_frame->show() ;
		m_pRdb_connect_button->setEnabled( true ) ;
		m_pFileout_button->setEnabled( false ) ;

		m_pRdb_select_combobox->setEnabled( true ) ;
		m_pRead_info_label->setText( tr( "未与实时库建立连接" ) ) ;
		m_pSelect_label->setText( tr( "SQL语法查询模式" ) ) ;
		m_pStatus_label->setText( tr("") ) ;
		m_pFresh_timer->stop() ;
		slot_sql_change() ;
	}
}

void Rdbview_wnd::refresh_rdb_tree( void )
{
	bool bTerminal_data_ok ;
	bTerminal_data_ok = get_terminal_info() ;
	if ( !bTerminal_data_ok ) 
	{
		return;
	}

	bTerminal_data_ok = get_rtdb_table_info() ; //读取实时表信息
	if ( !bTerminal_data_ok ) 
	{
		return;
	}

	bTerminal_data_ok = get_table_col_info( -1 ) ; //读取实时表列信息
	if ( !bTerminal_data_ok ) 
	{
		return;
	}

	//读取列的显示标志
	char file_name[ 256 ] ;
// 	sprintf( file_name , "%s/ini/rdbview.ini" , getenv( "NBENV" ) ) ;
// 	ini_RW* pini = new ini_RW( file_name ) ;
	sprintf( file_name , "%s/resource/xml/rdbview.xml" , getenv( "NBENV" ) ) ;
	Xml_RW* pini = new Xml_RW( file_name ) ;
	bool bRtdb_col_flag ;
	char* field ;
	for ( int i = 0 ; i < Info.RtdbTableNum ; i++ )
	{
		field = NULL;

		if ( SUCCESS == pini->get_string( "排序域名设置" , Info.Rtdb[ i ].tabledescribe , field ) )
		{
			strcpy( Info.Rtdb[ i ].orderfield , field ) ;
		}
		else
		{
			strcpy( Info.Rtdb[ i ].orderfield , "" ) ;
		}

		if ( SUCCESS == pini->get_string( "表头域名" , Info.Rtdb[ i ].tabledescribe, field ) )
		{
			strcpy( Info.Rtdb[ i ].headfield , field ) ;
		}
		else
		{
			strcpy( Info.Rtdb[ i ].headfield , "" ) ;
		}

		for ( int j = 0 ; j < Info.Rtdb[ i ].colnum ; j++ )
		{
			if( SUCCESS == pini->get_bool( Info.Rtdb[ i ].tabledescribe, Info.Rtdb[ i ].col[ j ].coldescribe, bRtdb_col_flag ) )
			{	
				Info.Rtdb[ i ].col[ j ].flag = bRtdb_col_flag ;
			}
			else
			{
				Info.Rtdb[ i ].col[ j ].flag = true ;
			}
		}
	}

	if( pini )
	{
		delete pini ;
		pini = NULL ;
	}
	//读取实时库表的排序域名结束

	m_pRdb_tree->clear() ;
	m_pRdb_tree->setSortingEnabled( false ) ;
	m_pRdb_tree->setRootIsDecorated( true ) ;
	m_pRdb_tree->header()->hide() ;//隐藏表头

	QList<QTreeWidgetItem *> parent_item_list ;	
	QStringList parent_item_str_list ;
	for ( int i = 0 ; i < ROOT_NUM ; i++ )
	{
		parent_item_str_list.clear() ;
		parent_item_str_list << tr( RootType[ i ] ) ;
		parent_item_list.append( new QTreeWidgetItem( m_pRdb_tree , parent_item_str_list ) ) ;
	}

	QTreeWidgetItem* pItem = 0 ;
	int num = 0 ;
	QStringList info_str_list ;

	int list_count = parent_item_list.size();

	for ( int i = 0 ; i < list_count ; i++ , num++ )
	{
		pItem = parent_item_list[ i ] ;
		info_str_list.clear() ;

		pItem->setIcon( 0 , QIcon( ":/res/node.png" ) ) ;

		if ( num < 3 )
		{
			QMap<int ,QTreeWidgetItem*> m_group_item_map ;

			QStringList desc_group_str_list ;
			desc_group_str_list.clear() ;
			//先显示第一层
			desc_group_str_list << tr( "未分组厂站" ) ;
			QTreeWidgetItem* pFirst_station_item = new QTreeWidgetItem( pItem , desc_group_str_list ) ;
			m_group_item_map[ 0 ] = pFirst_station_item ;
			pFirst_station_item->setIcon( 0 , QIcon( ":/res/childnode.png" ) ) ;
			pFirst_station_item->setData( 0 , Qt::UserRole + 1 , 1 ) ;
			QMapIterator<int , PSTATIONAREAPARA> area( m_station_area ) ;
			while ( area.hasNext() ) 
			{
				area.next() ;
				if ( area.value()->masterid == NULL )
				{
					desc_group_str_list.clear() ;
					desc_group_str_list.append( tr( area.value()->name ) ) ;
					QTreeWidgetItem *pSub_item = new QTreeWidgetItem( pItem, desc_group_str_list ) ;
					pSub_item->setIcon( 0 , QIcon( ":/res/childnode.png" ) ) ;
					pSub_item->setData( 0 , Qt::UserRole + 1 , 1 ) ;
					m_group_item_map[ area.value()->id ] = pSub_item ;
				}
			}
			//显示第二层,区域表只可以表示到2层
			QMapIterator<int , PSTATIONAREAPARA> area2( m_station_area ) ;
			while ( area2.hasNext() ) 
			{
				area2.next();
				if ( area2.value()->masterid != NULL )
				{
					desc_group_str_list.clear() ;
					desc_group_str_list.append( tr(area2.value()->name) ) ;
					QTreeWidgetItem *pSub_item = new QTreeWidgetItem( m_group_item_map[ area2.value()->masterid ] , desc_group_str_list ) ;
					pSub_item->setIcon( 0 , QIcon( ":/res/childnode.png" ) ) ;
					pSub_item->setData( 0 , Qt::UserRole + 1 , 2 ) ;
					m_group_item_map[ area2.value()->id ] = pSub_item ;
				}
			}
			//显示厂站

			QTreeWidgetItem* pTerminal_item = 0 ;
			for ( int i = 0 ; i < Info.TerminalNum ; i++ )
			{
				info_str_list.clear() ;
				info_str_list << tr( Info.Terminal[ i ].termdescribe ) ;

				pTerminal_item = new QTreeWidgetItem( m_group_item_map[ Info.Terminal[ i ].areaid ] , info_str_list ) ;
				pTerminal_item->setIcon( 0 , QIcon( ":/res/selnode.png" ) ) ;
				pTerminal_item->setData( 0 , Qt::UserRole + 1 , 3 ) ;
			}
		}
		else
		{
			for ( int j = 0 ; j < Info.RtdbTableNum ; j++ )
			{
				info_str_list.clear() ;
				info_str_list << tr( Info.Rtdb[ j ].tabledescribe ) ;
				QTreeWidgetItem* pTable_item = new QTreeWidgetItem( pItem , info_str_list ) ;

				if ( Info.Rtdb[ j ].sortflag )
				{	
					pTable_item->setIcon( 0 , QIcon( ":/res/childnode.png" ) ) ;
					for ( int i = 0 ; i < Info.TerminalNum ; i++ )
					{		
						info_str_list.clear() ;
						info_str_list << tr( Info.Terminal[ i ].termdescribe ) ;

						QTreeWidgetItem* pTerminal_item = new QTreeWidgetItem( pTable_item , info_str_list ) ;
						pTerminal_item->setIcon( 0 , QIcon( ":/res/selnode.png" ) ) ;
					}
				}
				else
				{
					pTable_item->setIcon( 0 , QIcon( ":/res/selnode.png" ) ) ;
				}
			}
		}
	}
	m_pRdb_tree->topLevelItem( 0 )->setExpanded( true ) ;
}

bool Rdbview_wnd::get_terminal_info( void )
{
	FUNC_STR func ;
	CS_DATAFMT* datafmt = NULL ;
	func.func = GET_TABLE_INFO ;
	int j1 , j2 , j3 , j4 , j5 , j6 , j7, j8, ptcnt ;
	j1 = 0 ;
	j2 = 0 ;
	j3 = 0 ;
	j4 = 0 ;
	ptcnt = 0 ;
	strcpy( func.table , "subareastationview2" ) ;
	strcpy( func.dbname , "modeldb" ) ;
	if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
	{
		dbg.PutDbg( 1 , "打开数据库失败-get_terminal_info" ) ;
		return false ;
	}
	for ( int i = 0 ; i < func.colnum ; i++ )
	{
		func.col[ i ].condition = false ;
		func.col[ i ].select = false ;
		func.col[ i ].order = -1;
		func.col[ i ].order_d = -1 ;
		if( strcmp( datafmt[ i ].name , "终端序号" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 0 ;
			j1 = i ;
		}
		if( strcmp( datafmt[ i ].name , "终端代码" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 1 ;
			j2 = i ;
		}
		if( strcmp( datafmt[ i ].name , "终端描述" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 2 ;
			j3 = i ;
		}
		if( strcmp( datafmt[ i ].name , "分组序号" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 3 ;
			j4 = i ;
		}
		if( strcmp( datafmt[ i ].name , "分组描述" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 4 ;
			j5 = i ;
		}
	}

	void *pt = NULL ;
	func.func = SEL_ISQL_RECORDERS ;

	int groupnum = netcfg.GetHostGrpNum() ;
	int* grp = netcfg.GetHostGrpEnm() ;
	char isqlorder[ 20 ] ;
	sprintf( isqlorder , " 1=1 order by 终端序号" ) ;
	strcpy( func.isql , isqlorder ) ;
	if ( ( db.SelectRcds( &func , datafmt , &pt ) ) != CS_SUCCEED )
	{
		return false ;
	}
	Info.TerminalNum = (int)func.ret_roxnum ;
	if ( Info.TerminalNum == 0 )
	{
		return false ;
	}

	int term_num = Info.TerminalNum ;
	Info.Terminal = new TERMINAL[ term_num ] ;
	memset( Info.Terminal , 0 , sizeof( term_num * sizeof( TERMINAL ) ) ) ;

	for( int i = 0 ; i < func.ret_roxnum ; i++ )
	{
		Info.Terminal[ i ].terminalno = ( short )db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j1 ) , pt , ptcnt , NULL ) ;
		db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j2 ) , pt , ptcnt , Info.Terminal[ i ].termname ) ;
		db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j3 ) , pt , ptcnt , Info.Terminal[ i ].termdescribe ) ;
		Info.Terminal[ i ].areaid = ( short )db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j4 ) , pt , ptcnt , NULL ) ;
		db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j5 ) , pt , ptcnt , Info.Terminal[ i ].areaname ) ;
	}

	if ( pt ) 
	{
		free( pt ) ;
		pt = NULL ;
	}
	if ( datafmt ) 
	{
		free( datafmt ) ;
		datafmt = NULL ;
	}

	return true ;
}

bool Rdbview_wnd::get_station_info( void )
{
	FUNC_STR func ;
	CS_DATAFMT* datafmt = NULL ;
	func.func = GET_TABLE_INFO ;
	int j1 , j2 , j3 , ptcnt ;
	j1 = 0 ;
	j2 = 0 ;
	j3 = 0 ;
	ptcnt = 0 ;
	strcpy( func.table , "站所线系参数表" ) ;
	strcpy( func.dbname , "modeldb" ) ;
	if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
	{
		dbg.PutDbg( 1 , "打开数据库失败-get_station_info" ) ;
		return false ;
	}
	for ( int i = 0 ; i < func.colnum ; i++ )
	{
		func.col[ i ].condition = false ;
		func.col[ i ].select = false ;
		func.col[ i ].order = -1;
		func.col[ i ].order_d = -1 ;
		if( strcmp( datafmt[ i ].name , "subid" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 0 ;
			j1 = i ;
		}
		if( strcmp( datafmt[ i ].name , "代码" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 1 ;
			j2 = i ;
		}
		if( strcmp( datafmt[ i ].name , "描述" ) == 0 )
		{
			func.col[ i ].select = true ;
			func.col[ i ].order_d = 2 ;
			j3 = i ;
		}
	}

	void *pt = NULL ;
	func.func = SEL_ISQL_RECORDERS ;

	int groupnum = netcfg.GetHostGrpNum() ;
	int* grp = netcfg.GetHostGrpEnm() ;
	char isqlcat[ 255 ] , isqlorder[ 20 ] ;
	sprintf( func.isql , "分组序号=0 or 分组序号=%d" , grp[ 0 ] ) ;
	for( int i = 1 ; i < groupnum ; i++ )
	{
		sprintf(isqlcat,"or 分组序号= %d",grp[ i ]);
		strcat( func.isql , isqlcat ) ;
	}
	sprintf( isqlorder , " order by 代码" ) ;
	strcat( func.isql , isqlorder ) ;

	if ( ( db.SelectRcds( &func , datafmt , &pt ) ) != CS_SUCCEED )
	{
		return false ;
	}
	Info.TerminalNum = (int)func.ret_roxnum ;
	if ( Info.TerminalNum == 0 )
	{
		return false ;
	}

	int term_num = Info.TerminalNum ;
	Info.Terminal = new TERMINAL[ term_num ] ;
	memset( Info.Terminal , 0 , sizeof( term_num * sizeof( TERMINAL ) ) ) ;

	for( int i = 0 ; i < func.ret_roxnum ; i++ )
	{
		Info.Terminal[ i ].terminalno = ( short )db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j1 ) , pt , ptcnt , NULL ) ;
		db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j2 ) , pt , ptcnt , Info.Terminal[ i ].termname ) ;
		db.GetData( i , &func , ( CS_DATAFMT * )( datafmt + j3 ) , pt , ptcnt , Info.Terminal[ i ].termdescribe ) ;
	}

	if ( pt ) 
	{
		free( pt ) ;
		pt = NULL ;
	}
	if ( datafmt ) 
	{
		free( datafmt ) ;
		datafmt = NULL ;
	}

	return true ;
}

bool Rdbview_wnd::get_rtdb_table_info( void )
{
	FUNC_STR func ;
	CS_DATAFMT* datafmt = NULL ;
	func.func = GET_TABLE_INFO ;
	int ptcnt = 0 ;
	strcpy( func.table , "rtdb_table" ) ;
	strcpy( func.dbname , "modeldb" ) ;
	char * col_name[] = {"NAME","DESCRIBE","SORTFLAG"};
	void * pt = 0 ;
	char filename[ 256 ] ;
	if( m_bRd_file )
	{
		sprintf( filename , "%s/dbmap/rdbview_%s/datafmt" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , ( void** )( &datafmt ) ) )
		{
			return false ;
		}

		sprintf( filename , "%s/dbmap/rdbview_%s/func" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , &func , sizeof( FUNC_STR ) ) )
		{
			return false ;
		}

		db.GetTableInfo( &func , NULL , NULL ) ;

		sprintf( filename , "%s/dbmap/rdbview_%s/pt" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , &pt ) )
		{
			return false ;
		}	
	}
	else
	{
		if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
		{
			if( datafmt )	free( datafmt ) ;
			db.Close() ;	
			return false ;
		}
		if ( func.colnum < 1 || !datafmt )
		{
			if ( datafmt ) free( datafmt ) ;
			db.Close() ;
			return false ;
		}

		int col_num = sizeof( col_name ) / sizeof( char* ) ;
		for( int i = 0 ; i < func.colnum ; i++ )
		{
			func.col[ i ].select = NO ;
			func.col[ i ].order_d = -1 ;
			func.col[ i ].order = -1 ;
			func.col[ i ].condition = NO ;
			for( int j = 0 ; j < col_num ; j++ )
			{
				if( strcmp( datafmt[ i ].name , col_name[ j ] ) )	continue ;
				func.col[ i ].select = YES ;
				func.col[ i ].order_d = j ;
			}
		}
		func.func = SEL_ISQL_RECORDERS ;

		sprintf( func.isql , "sername='SCADA' and showflag>0" ) ;
		if ( ( db.SelectRcds( &func , datafmt , &pt ) ) != CS_SUCCEED )
		{
			if( datafmt )	free( datafmt ) ;
			if( pt )	free( pt ) ;
			db.Close() ;
			return false ;
		}
		if( func.ret_roxnum == 0 )
		{
			if( datafmt )	free( datafmt ) ;
			if( pt )	free( pt ) ;
			db.Close() ;	
			return false ;
		}
	}

	Info.RtdbTableNum = func.ret_roxnum ;
	if ( Info.RtdbTableNum == 0 )
	{
		return false ;
	}
	Info.Rtdb = new RTDB[ Info.RtdbTableNum ] ;
	memset( Info.Rtdb , 0 , sizeof( Info.RtdbTableNum * sizeof( RTDB ) ) ) ;
	int count ;
	for( int i = 0 ; i < Info.RtdbTableNum ; i++ )
	{
		count = 0 ;
		db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , Info.Rtdb[ i ].tablename ) ;
		db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , Info.Rtdb[ i ].tabledescribe ) ;
		Info.Rtdb[ i ].sortflag = ( unsigned char )db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , NULL ) ;
		Info.Rtdb[ i ].col = NULL ;
	}

	if ( pt ) 
	{
		free ( pt ) ;
		pt = NULL ;
	}
	if ( datafmt ) 
	{
		free ( datafmt ) ;
		datafmt = NULL ;
	}	
	return true ;
}

bool Rdbview_wnd::get_table_col_info( int cur_sel )
{
	FUNC_STR func ;
	memset( &func , 0 , sizeof( FUNC_STR ) ) ;
	CS_DATAFMT *datafmt = NULL ;
	int	ptcnt = 0 ;
	void *pt = 0 ;

	printf( "rtdb_field" ) ;
	fflush( stdout ) ;

	func.func = GET_TABLE_INFO ;
	strcpy( func.table , "rtdb_field" ) ;
	strcpy( func.dbname , "modeldb" ) ;
	char * col_name[ ] = { "TABLECODE" , "NAME" , "DESCRIBE" } ;

	char filename[ 256 ] ;
	if( m_bRd_file )
	{
		sprintf( filename , "%s/dbmap/rdbview_%s/datafmt" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , ( void** )( &datafmt ) ) )
		{
			return false ;
		}

		sprintf( filename , "%s/dbmap/rdbview_%s/func" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , &func , sizeof( FUNC_STR ) ) )
		{
			return false ;
		}
		db.GetTableInfo( &func , NULL , NULL ) ;

		sprintf( filename , "%s/dbmap/rdbview_%s/pt" , getenv( NBENV ) , func.table ) ;
		if( !readFile( filename , &pt ) )
		{
			return false ;
		}
	}
	else
	{
		if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
		{
			if( datafmt )	free( datafmt ) ;
			db.Close() ;	
			return FALSE ;
		}
		if ( func.colnum < 1 || !datafmt )
		{
			if ( datafmt ) free( datafmt ) ;
			db.Close() ;	
			return FALSE ;
		}
		int col_num = sizeof( col_name )/sizeof( char* ) ;
		for( int i = 0 ; i < func.colnum ; i++ )
		{
			func.col[ i ].select = NO ;
			func.col[ i ].order_d = -1 ;
			func.col[ i ].order = -1 ;
			func.col[ i ].condition = NO ;
			for( int j = 0 ; j < col_num ; j++ )
			{
				if( strcmp( datafmt[ i ].name , col_name[ j ] ) )	continue ;
				func.col[ i ].select = YES ;
				func.col[ i ].order_d = j ;
			}
		}
		func.func = SEL_ISQL_RECORDERS ;
		if ( cur_sel == -1 )
		{
			sprintf( func.isql , "1>0 order by tablecode , no" ) ;
		}
		else
		{
			sprintf( func.isql , "tablecode = '%s' order by no" , Info.Rtdb[ cur_sel ].tablename ) ;
		}
		if ( ( db.SelectRcds( &func , datafmt , &pt ) ) != CS_SUCCEED )
		{
			if( datafmt )	free( datafmt ) ;
			if( pt )	free( pt ) ;
			db.Close() ;	
			return FALSE ;
		}
		if( func.ret_roxnum == 0 )
		{
			if( datafmt )	free( datafmt ) ;
			if( pt )	free( pt ) ;
			db.Close() ;	
			return FALSE ;
		}
	}

	int count ;
	if ( cur_sel == -1 )
	{
		//取全部的rtdb_field
		RTDBTABLECOL * tmpcol = new RTDBTABLECOL[ func.ret_roxnum ] ;
		memset( tmpcol , 0 , sizeof( func.ret_roxnum*sizeof( RTDBTABLECOL ) ) ) ;
		for( int i = 0 ; i < func.ret_roxnum ; i++ )
		{
			count = 0 ;
			db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , tmpcol[ i ].tablename ) ;
			db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , tmpcol[ i ].colname ) ;
			db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , tmpcol[ i ].coldescribe ) ;
			if ( strcmp( tmpcol[ i ].coldescribe , "" ) == 0 )
			{
				sprintf( tmpcol[ i ].coldescribe , "未定义" ) ;
			}
			tmpcol[ i ].flag = 1 ;
		}
		int kk , tempcolnum ;
		for ( kk = 0 ; kk < Info.RtdbTableNum ;kk++ )
		{
			Info.Rtdb[ kk ].colnum = 0 ;
		}

		for ( kk = 0 ; kk < Info.RtdbTableNum ; kk++ )//统计列的个数
		{
			for( int i = 0 ; i < func.ret_roxnum ; i++ )
			{
				if ( strcmp( Info.Rtdb[ kk ].tablename , tmpcol[ i ].tablename ) == 0 )
				{
					Info.Rtdb[ kk ].colnum++ ;
				}
			}
		}

		for ( kk = 0 ; kk < Info.RtdbTableNum ; kk++ )
		{
			if ( Info.Rtdb[ kk ].colnum > 0 )
			{
				Info.Rtdb[ kk ].col = new RTDBTABLECOL[ Info.Rtdb[ kk ].colnum ] ;
				memset( Info.Rtdb[ kk ].col , 0 , sizeof( Info.Rtdb[ kk ].colnum*sizeof( RTDBTABLECOL ) ) ) ;
			}
		}

		for ( kk = 0 ; kk < Info.RtdbTableNum ; kk++ )//把列信息放到相应的表结构中
		{
			tempcolnum = 0 ;
			for( int i = 0 ; i < func.ret_roxnum ; i++ )
			{
				if ( tempcolnum >= Info.Rtdb[ kk ].colnum ) break ;
				if ( strcmp( Info.Rtdb[ kk ].tablename , tmpcol[ i ].tablename ) == 0 )
				{
					strcpy( Info.Rtdb[ kk ].col[ tempcolnum ].tablename , tmpcol[ i ].tablename ) ;
					strcpy( Info.Rtdb[ kk ].col[ tempcolnum ].colname , tmpcol[ i ].colname ) ;
					strcpy( Info.Rtdb[ kk ].col[ tempcolnum ].coldescribe , tmpcol[ i ].coldescribe ) ;
					if ( strcmp( Info.Rtdb[ kk ].col[ tempcolnum ].coldescribe , "" ) == 0 )
						sprintf( Info.Rtdb[ kk ].col[ tempcolnum ].coldescribe , "未定义" ) ;
					Info.Rtdb[ kk ].col[ tempcolnum ].flag = tmpcol[ i ].flag ;
					tempcolnum ++ ;
				}
			}
		}

		if ( tmpcol )
		{
			delete tmpcol ;
		}
	}
	else
	{
		//取某个表的rtdb_field
		Info.Rtdb[ cur_sel ].colnum = func.ret_roxnum ;
		if ( func.ret_roxnum == 0 )
			return FALSE ;
		Info.Rtdb[ cur_sel ].col = new RTDBTABLECOL[ func.ret_roxnum ] ;
		memset( Info.Rtdb[ cur_sel ].col , 0 , sizeof( func.ret_roxnum*sizeof( RTDBTABLECOL ) ) ) ;

		for( int i = 0 ; i < func.ret_roxnum ; i++ )
		{
			count = 1 ;
			db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , Info.Rtdb[ cur_sel ].col[ i ].colname ) ;
			db.GetData( i , &func , datafmt , col_name[ count++ ] , pt , ptcnt , Info.Rtdb[ cur_sel ].col[ i ].coldescribe ) ;

			if ( strcmp( Info.Rtdb[ cur_sel ].col[ i ].coldescribe , "" ) == 0 )
			{
				sprintf( Info.Rtdb[ cur_sel ].col[ i ].coldescribe , "未定义" ) ;
			}
			Info.Rtdb[ cur_sel ].col[ i ].flag = 1 ;
		}
	}
	if ( pt ) 
	{
		free ( pt ) ;
		pt = NULL ;
	}
	if ( datafmt ) 
	{
		free ( datafmt ) ;
		datafmt = NULL ;
	}

	return true ;
}

bool Rdbview_wnd::readFile( char *filename, void ** buf )
{
	struct stat sf;

	if( stat( filename, &sf )!=0 )
	{
		return false;
	}

	FILE *fp;
	int ret;

	if( sf.st_size == 0 )
		return true ;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return false ;
	*buf = (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );

	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return false;
	}

	return true ;

}

bool Rdbview_wnd::readFile( char *filename, void * buf, int len )
{
	struct stat sf;

	if( stat( filename, &sf )!=0 )
	{
		return false;
	}

	FILE *fp;
	int ret;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return false;
	ret = fread( buf, len, 1, fp );

	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return false;
	}

	return true ;
}

int Rdbview_wnd::read_stationarea_data()
{
	int ret =-1;
	m_station_area.clear() ;

	CS_DATAFMT * datafmt = NULL ;
	FUNC_STR func ;
	func.func = SEL_ISQL_RESULTS ;
	strcpy( func.dbname , DB_NAME ) ;
	strcpy( func.table , "STATIONAREA" ) ;
	strcpy( func.isql , "select * from STATIONAREA") ;
	STATIONAREAPARA* pStation_area = NULL ;

	if ( CS_SUCCEED == db.SelectIsqlResults( &func , &datafmt , ( void ** )&pStation_area ) )
	{
		ret = func.ret_roxnum ;
	}

	if ( ret > 0 )
	{
		for ( int i = 0 ; i < ret ; i++ )
		{
			STATIONAREAPARA* pTmp_station_area = new STATIONAREAPARA;
			*pTmp_station_area = *( pStation_area + i ) ;
			m_station_area.insert( pTmp_station_area->id , pTmp_station_area ) ;
		}
	}

	if ( datafmt )
	{
		free( datafmt ) ;
		datafmt = NULL ;
	}

	if ( pStation_area )
	{
		free( pStation_area ) ;
	}

	return ret > 0 ? 0 : -1 ;
}

void Rdbview_wnd::slot_change_button_type()
{
	QString buttonstylesheet ;
	m_bButton_type = !m_bButton_type ;
	if ( m_bButton_type == false )
	{
		buttonstylesheet = tr( "#buttontypebutton\
							  {\
							  image: url(:/res/less_normal.png);\
							  min-height: 40px;\
							  max-height: 40px;\
							  }\
							  #buttontypebutton:hover\
							  {\
							  image: url(:/res/less_hover.png);\
							  }\
							  # buttontypebutton:pressed\
							  {\
								image: url(:/res/less_hover.png);\
							  }") ;
		m_pFileout_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pReaddata_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pYcyx_display_set_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pScada_display_set_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pRead_data_info_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pRdb_connect_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pRdb_disconnect_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		m_pRdb_sqlrun_button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
		setMinimumWidth( 1250 ) ;
	}
	else
	{
		buttonstylesheet = tr( "#buttontypebutton\
							  {\
							  image: url(:/res/more_normal.png);\
							  min-height: 40px;\
							  max-height: 40px;\
							  }\
							  #buttontypebutton:hover\
							  {\
							  image: url(:/res/more_hover.png);\
							  }\
							  #buttontypebutton:pressed\
							  {\
							  image: url(:/res/more_hover.png);\
							  }") ;
		m_pFileout_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pReaddata_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pYcyx_display_set_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pScada_display_set_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pRead_data_info_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pRdb_connect_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pRdb_disconnect_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		m_pRdb_sqlrun_button->setToolButtonStyle( Qt::ToolButtonIconOnly ) ;
		setMinimumWidth( 800 ) ;
	}
	m_pButton_type_button->setStyleSheet( buttonstylesheet ) ;
}

int Rdbview_wnd::get_min_yx_type( int brind , int swind , int prind , int stind , int dmsdiagnoseind )
{
	int tempret = 0 ;
	int minyxno_breakswitch = INT_MAX ;
	int minType_breakswitch = MINTYPE_YXEND ;
	if( brind >= Info.Yx_BreakNum && swind >= Info.Yx_SwitchNum )//都完
	{
		minyxno_breakswitch = INT_MAX ;
		minType_breakswitch = MINTYPE_YXEND ;
	}
	else if( brind >= Info.Yx_BreakNum )//break完毕
	{
		minyxno_breakswitch = Info.Yx_Switch[swind].no ;
		minType_breakswitch = MINTYPE_SWITCH ;
	}
	else if( swind >= Info.Yx_SwitchNum )//switch完毕
	{
		minyxno_breakswitch = Info.Yx_Break[brind].no ;
		minType_breakswitch = MINTYPE_BREAK ;
	}

	else//都未完，比较
	{
		if( Info.Yx_Break[brind].no >= Info.Yx_Switch[swind].no )
		{
			minyxno_breakswitch = Info.Yx_Switch[swind].no ;
			minType_breakswitch = MINTYPE_SWITCH ;			
		}
		else
		{
			minyxno_breakswitch = Info.Yx_Break[brind].no ;
			minType_breakswitch = MINTYPE_BREAK ;
		}
	}

	int minyxno_protstatus = INT_MAX ;//2147483647
	int minType_protstatus = MINTYPE_YXEND ;
	if( prind >= Info.Yx_ProtNum && stind >= Info.Yx_StatusNum )
	{
		minyxno_protstatus = INT_MAX ;
		minType_protstatus = MINTYPE_YXEND ;
	}
	else if( prind >= Info.Yx_ProtNum )//prot完毕
	{
		minyxno_protstatus = Info.Yx_Status[stind].no ;
		minType_protstatus = MINTYPE_STATUS ;
	}
	else if( stind >= Info.Yx_StatusNum )//status完毕
	{
		minyxno_protstatus = Info.Yx_Prot[prind].no ;
		minType_protstatus = MINTYPE_PROT ;
	}
	else//比较
	{
		if( Info.Yx_Prot[prind].no >= Info.Yx_Status[stind].no )
		{
			minyxno_protstatus = Info.Yx_Status[stind].no ;
			minType_protstatus = MINTYPE_STATUS ;
		}
		else
		{
			minyxno_protstatus = Info.Yx_Prot[prind].no ;
			minType_protstatus = MINTYPE_PROT ;
		}
	}

	if( minyxno_breakswitch <= minyxno_protstatus )
		tempret = minType_breakswitch ;//return minType_breakswitch ;
	else
		tempret = minType_protstatus ;//return minType_protstatus ;

	if( dmsdiagnoseind < Info.Yx_DmsdiagnoseNum )//dmsdiagnoseindh完毕
	{
		if ( minyxno_breakswitch >= Info.Yx_Dmsdiagnose[dmsdiagnoseind].no&&minyxno_protstatus >= Info.Yx_Dmsdiagnose[dmsdiagnoseind].no )
			tempret = MINTYPE_DMSDIAGNOSE ;
	}
	return tempret ;
}

void Rdbview_wnd::get_yxdata_by_table( REALDATA **Yx , short &num , char *tablename )
{
	int ret = 0 ;
	void *raddr = NULL ;
	int tmpint = 1 ;
	Rdb_SqlCli rl ;

	REALDATA* TempData = *Yx ;

	if( TempData ) delete TempData ;
	TempData = NULL ;
	*Yx = TempData ;
	num = 0 ;

	rl.RdbSelectFrom( tablename ) ;
	rl.RdbSelectField( "yxno" ) ;
	rl.RdbSelectField( "name" ) ;
	rl.RdbSelectField( "describe" ) ;
	rl.RdbSelectField( "terminalno" ) ;
	rl.RdbSelectField( "value" ) ;
	rl.RdbSelectField( "calcultf" ) ;
	rl.RdbSelectField( "mansetf" ) ;
	rl.RdbOrderBy( "yxno" ) ;
	if ( ShowCfg->OrderByStation )	
	{
		rl.RdbWhereCond( "substation" , RDB_DATATYPE_STRING , ( Info.Station ) ) ;
	}
	else
	{
		rl.RdbWhereCond( "terminalno" , RDB_DATATYPE_SHORT , &( Info.TerminalNo ) ) ;
	}
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;

	if( ret != RDB_OK ) return ;

	num = rl.rdbgetrcdcnt() ;
	TempData = new REALDATA[ num ] ;
	*Yx = TempData ;

	for( int i = 0 ; i < num ; i++ ) 
	{
		strcpy( ( TempData + i )->name , rl.RdbGetString( i , "name" ) ) ;
		strcpy( ( TempData + i )->describe , rl.RdbGetString( i , "describe" ) ) ;
		( TempData + i )->terminalno = rl.RdbGetVal_uint( i , "terminalno" ) ;
		( TempData + i )->no = rl.RdbGetVal_uint( i , "yxno" ) ;
		( TempData + i )->value = ( float )rl.RdbGetVal_uint( i , "value" ) ;
		( TempData + i )->mansetf = rl.RdbGetVal_uint( i , "mansetf" ) ;
		( TempData + i )->calcultf = rl.RdbGetVal_uint( i , "calcultf" ) ;
	}
}

bool Rdbview_wnd::get_yc_data( void )
{
	Rdb_SqlCli rl ;

	rl.RdbSelectFrom( "analog" ) ;
	rl.RdbSelectField( "ycno" ) ;
	rl.RdbOrderBy( "ycno" ) ;
	rl.RdbSelectField( "name" ) ;
	rl.RdbSelectField( "describe" ) ;
	rl.RdbSelectField( "terminalno" ) ;
	rl.RdbSelectField( "ycinconsistf" ) ;
	rl.RdbSelectField( "value" ) ;
	rl.RdbSelectField( "slope" ) ;
	rl.RdbSelectField( "scale" ) ;//倍率
	rl.RdbSelectField( "calcultf" ) ;
	rl.RdbSelectField( "mansetf" ) ;
	if ( ShowCfg->OrderByStation )
	{
		rl.RdbWhereCond( "substation" , RDB_DATATYPE_STRING , ( Info.Station ) ) ;
	}
	else
	{
		rl.RdbWhereCond( "terminalno" , RDB_DATATYPE_SHORT , &( Info.TerminalNo ) ) ;
	}

	int ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return false ;
	}

	int rec_number = rl.rdbgetrcdcnt() ;
	if ( rec_number == 0 ) 
	{
		return false ;
	}
	Info.RealDataNum = rec_number ;
	Info.RealData = new REALDATA[ rec_number ] ;
	memset( Info.RealData , 0 , sizeof( rec_number * sizeof( REALDATA ) ) ) ;
	for( int i = 0 ; i < rec_number ; i++ ) 
	{
		strcpy( Info.RealData[ i ].name , rl.RdbGetString( i , "name" ) ) ;
		strcpy( Info.RealData[ i ].describe , rl.RdbGetString( i , "describe" ) ) ;
		Info.RealData[ i ].no = rl.RdbGetVal_uint( i , "ycno" ) ;
		Info.RealData[ i ].terminalno = rl.RdbGetVal_uint( i , "terminalno" ) ;
		Info.RealData[ i ].value = rl.RdbGetVal_float( i , "value" ) ;
		Info.RealData[ i ].slope = rl.RdbGetVal_float( i , "slope" ) ;
		Info.RealData[ i ].scale = rl.RdbGetVal_float( i , "scale" ) ;
		Info.RealData[ i ].calcultf = rl.RdbGetVal_uint( i , "calcultf" ) ;
		Info.RealData[ i ].mansetf = rl.RdbGetVal_uint( i , "mansetf" ) ;
		Info.RealData[ i ].inconsistf = rl.RdbGetVal_uint( i , "ycinconsistf" ) ;
	}

	return true ;
}

bool Rdbview_wnd::get_yx_data( void )
{
	get_yxdata_by_table( &( Info.Yx_Break ) , Info.Yx_BreakNum , "break" ) ;
	get_yxdata_by_table( &( Info.Yx_Switch ) , Info.Yx_SwitchNum , "switch" ) ;
	get_yxdata_by_table( &( Info.Yx_Prot ) , Info.Yx_ProtNum , "prot" ) ;
	get_yxdata_by_table( &( Info.Yx_Status ) , Info.Yx_StatusNum , "status" ) ;
	get_yxdata_by_table( &( Info.Yx_Dmsdiagnose ) , Info.Yx_DmsdiagnoseNum , "dmsdiagnose" ) ;

	int rec_number = Info.Yx_BreakNum+Info.Yx_SwitchNum+Info.Yx_ProtNum+Info.Yx_StatusNum+Info.Yx_DmsdiagnoseNum ;
	Info.RealDataNum = rec_number ;
	if( rec_number <= 0 ) return false ;
	Info.RealData = new REALDATA[ rec_number ] ;

	int breakind = 0 , switchind = 0 , protind = 0 , statusind = 0 , dmsdiagnoseind = 0 ;
	int min_type = MINTYPE_YXEND ;
	int index = 0 ;
	for( int i = 0 ; i< rec_number ; i++ )
	{
		min_type = get_min_yx_type( breakind , switchind , protind , statusind , dmsdiagnoseind ) ;
		if( min_type == MINTYPE_YXEND ) break ;
		switch( min_type )
		{
		case MINTYPE_BREAK:
			index = breakind ;
			strcpy( Info.RealData[ i ].name , Info.Yx_Break[ breakind ].name ) ;
			strcpy( Info.RealData[ i ].describe , Info.Yx_Break[ breakind ].describe ) ;
			Info.RealData[ i ].terminalno = Info.Yx_Break[ breakind ].terminalno ;
			Info.RealData[ i ].no = Info.Yx_Break[ breakind ].no ;
			Info.RealData[ i ].mansetf = Info.Yx_Break[ breakind ].mansetf ;
			Info.RealData[ i ].calcultf = Info.Yx_Break[ breakind ].calcultf ;
			Info.RealData[ i ].value = Info.Yx_Break[ breakind ].value ;
			breakind++ ;
			break ;
		case MINTYPE_SWITCH:
			index = switchind ;
			strcpy( Info.RealData[ i ].name , Info.Yx_Switch[ switchind ].name ) ;
			strcpy( Info.RealData[ i ].describe , Info.Yx_Switch[ switchind ].describe ) ;
			Info.RealData[ i ].terminalno = Info.Yx_Switch[ switchind ].terminalno ;
			Info.RealData[ i ].no = Info.Yx_Switch[ switchind ].no ;
			Info.RealData[ i ].mansetf = Info.Yx_Switch[ switchind ].mansetf ;
			Info.RealData[ i ].calcultf = Info.Yx_Switch[ switchind ].calcultf ;
			Info.RealData[ i ].value = Info.Yx_Switch[ switchind ].value ;
			switchind++ ;
			break ;
		case MINTYPE_PROT:
			index = protind ;
			strcpy( Info.RealData[ i ].name , Info.Yx_Prot[ protind ].name ) ;
			strcpy( Info.RealData[ i ].describe , Info.Yx_Prot[ protind ].describe ) ;
			Info.RealData[ i ].terminalno = Info.Yx_Prot[ protind ].terminalno ;
			Info.RealData[ i ].no = Info.Yx_Prot[ protind ].no ;
			Info.RealData[ i ].mansetf = Info.Yx_Prot[ protind ].mansetf ;
			Info.RealData[ i ].calcultf = Info.Yx_Prot[ protind ].calcultf ;
			Info.RealData[ i ].value = Info.Yx_Prot[ protind ].value ;
			protind++ ;
			break ;
		case MINTYPE_STATUS:
			index = statusind ;
			strcpy( Info.RealData[ i ].name , Info.Yx_Status[ statusind ].name ) ;
			strcpy( Info.RealData[ i ].describe , Info.Yx_Status[ statusind ].describe ) ;
			Info.RealData[ i ].terminalno = Info.Yx_Status[ statusind ].terminalno ;
			Info.RealData[ i ].no = Info.Yx_Status[ statusind ].no ;
			Info.RealData[ i ].mansetf = Info.Yx_Status[ statusind ].mansetf ;
			Info.RealData[ i ].calcultf = Info.Yx_Status[ statusind ].calcultf ;
			Info.RealData[ i ].value = Info.Yx_Status[ statusind ].value ;
			statusind++ ;
			break ;
		case MINTYPE_DMSDIAGNOSE:
			index = dmsdiagnoseind ;
			strcpy( Info.RealData[ i ].name , Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].name ) ;
			strcpy( Info.RealData[ i ].describe , Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].describe ) ;
			Info.RealData[ i ].terminalno = Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].terminalno ;
			Info.RealData[ i ].no = Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].no ;
			Info.RealData[ i ].mansetf = Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].mansetf ;
			Info.RealData[ i ].calcultf = Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].calcultf ;
			Info.RealData[ i ].value = Info.Yx_Dmsdiagnose[ dmsdiagnoseind ].value ;
			dmsdiagnoseind++ ;
			break ;
		}
	}
	return true ;
}

bool Rdbview_wnd::get_kwh_data( void )
{
	Rdb_SqlCli rl ;

	rl.RdbSelectFrom( "kwh" ) ;
	rl.RdbSelectField( "kwhno" ) ;
	rl.RdbOrderBy( "kwhno" ) ;
	rl.RdbSelectField( "name" ) ;
	rl.RdbSelectField( "describe" ) ;
	rl.RdbSelectField( "terminalno" ) ;
	if ( ShowCfg->ShowKwhTableCode )
	{
		rl.RdbSelectField( "tabcode" ) ;
	}
	else
	{
		rl.RdbSelectField( "daykwh" ) ;
	}
	rl.RdbSelectField( "calcultf" ) ;
	rl.RdbSelectField( "mansetf" ) ;
	if ( ShowCfg->OrderByStation )	
	{
		rl.RdbWhereCond( "substation" , RDB_DATATYPE_STRING , ( Info.Station ) ) ;
	}
	else 
	{
		rl.RdbWhereCond( "terminalno" , RDB_DATATYPE_SHORT , &( Info.TerminalNo ) ) ;
	}
	int ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	return false ;

	int rec_number = rl.rdbgetrcdcnt() ;
	if ( rec_number == 0 ) return false ;
	Info.RealDataNum = rec_number ;
	Info.RealData = new REALDATA[ rec_number ] ;
	memset( Info.RealData , 0 , sizeof( rec_number*sizeof( REALDATA ) ) ) ;
	for( int i = 0 ; i < rec_number ; i++ ) 
	{
		strcpy( Info.RealData[ i ].name , rl.RdbGetString( i , "name" ) ) ;
		strcpy( Info.RealData[ i ].describe , rl.RdbGetString( i , "describe" ) ) ;
		Info.RealData[ i ].no = rl.RdbGetVal_uint( i , "kwhno" ) ;
		Info.RealData[ i ].terminalno = rl.RdbGetVal_uint( i , "terminalno" ) ;
		if ( ShowCfg->ShowKwhTableCode )
		{
			Info.RealData[ i ].daykwh = rl.RdbGetVal_double( i , "tabcode" ) ;
		}
		else
		{
			Info.RealData[ i ].daykwh = rl.RdbGetVal_double( i , "daykwh" ) ;
		}
		Info.RealData[ i ].calcultf = rl.RdbGetVal_uint( i , "calcultf" ) ;
		Info.RealData[ i ].mansetf = rl.RdbGetVal_uint( i , "mansetf" ) ;
	}
	return true ;
}

void Rdbview_wnd::slot_timer( void )
{
	slot_read_data() ;
}

void Rdbview_wnd::slot_read_data( void )
{
	QTreeWidgetItem* pCurrent_item = ( QTreeWidgetItem* )m_pRdb_tree->currentItem( ) ;
	QString table_name_str , rtu_describe_str ;
	short rtu_no ;
	bFlag = false ;
	if ( !pCurrent_item )
	{
		return ;
	}

	if ( pCurrent_item->data( 0 , Qt::UserRole+1 ).toInt( ) == 1\
		 || pCurrent_item->data( 0 , Qt::UserRole+1 ).toInt( ) == 2 )
	{
		return ;
	}

	int depth = 0 ;
	QTreeWidgetItem* pTmp_item = pCurrent_item ;
	while ( pTmp_item )
	{
		depth++ ;
		pTmp_item = pTmp_item->parent( ) ;
	}
	depth = depth - 1 ;

	int user_flag = pCurrent_item->data( 0 , Qt::UserRole+1 ).toInt( ) ;
	if( pCurrent_item->childCount( ) == 0 )	
	{
		if ( user_flag <= 0 )
		{
			if ( depth == 2 )
			{
				Info.RealFlag = SCADA_REAL ;
				table_name_str = pCurrent_item->parent( )->text( 0 ) ;
				rtu_describe_str = pCurrent_item->text( 0 ) ;
				rtu_no = get_terminal_no( rtu_describe_str ) ;
			}
			else
			{
				Info.RealFlag = SCADA_REAL ;
				table_name_str = pCurrent_item->text( 0 ) ;
				rtu_no = -1 ;
			}
		}
		else if( user_flag == 3 )
		{
			QTreeWidgetItem* pTemp_item ;
			QTreeWidgetItem *pParent_item = pCurrent_item->parent( ) ;
			while ( pParent_item )
			{
				pTemp_item = pParent_item ;
				pParent_item = pParent_item->parent( ) ;		 
			}
			if ( pTemp_item->text( 0 ) == tr( "实时电度数据" ) )
			{
				Info.RealFlag = KWH ;
				Info.TerminalNo = get_terminal_no( pCurrent_item->text( 0 ) ) ;
				table_name_str = tr( "kwh" ) ;
				rtu_no = get_terminal_no( pCurrent_item->text( 0 ) ) ;
			}
			else if ( pTemp_item->text( 0 ) == tr( "实时遥测数据" ) )
			{
				Info.RealFlag = YC ;
				Info.TerminalNo = get_terminal_no( pCurrent_item->text( 0 ) ) ;
				table_name_str = tr( "analog" ) ;
				rtu_no = get_terminal_no( pCurrent_item->text( 0 ) ) ;
			}
			else if ( pTemp_item->text( 0 ) == tr( "实时遥信数据" ) )
			{
				Info.RealFlag = YX ;
				Info.TerminalNo = get_terminal_no( pCurrent_item->text( 0 ) ) ;
				rtu_no = get_terminal_no( pCurrent_item->text( 0 ) ) ;
			}
		}
		if( call_real_data( table_name_str , rtu_no ) == 1 ) 
		{
			fill_data( table_name_str , rtu_no , bFlag ) ;
			Info.DataOK = true ;
		}
		else 
		{
			fill_empty_data( table_name_str , rtu_no ) ;
			m_pRead_info_label->setText( tr( "数据读取信息:无有效数据" ) ) ;
			Info.DataOK = false ;
		}
	}	
}

short Rdbview_wnd::get_terminal_no( QString table_str )
{
	for ( int i = 0 ; i < Info.TerminalNum ; i++ )
	{
		if ( tr( Info.Terminal[ i ].termdescribe ) == table_str )
		{
			return Info.Terminal[ i ].terminalno ;
		}
	}
	return 0 ;
}

bool Rdbview_wnd::call_real_data( QString table_name_str , short rtu_no )
{
	bool flag = false ;
	if ( Info.RealFlag == SCADA_REAL )//读整个表
	{	
		if ( get_real_table( table_name_str , rtu_no ) )
		{
			return true ;	
		}
		else
		{
			return false ;
		}
	}
	else//读短数据
	{
		switch ( Info.RealFlag ) 
		{
		case YC:
			flag = get_yc_data() ; 
			break ;
		case YX:
			flag = get_yx_data() ; 
			break ;
		case KWH:
			flag = get_kwh_data() ; 
			break ;
		}
	}
	return flag;
}

bool Rdbview_wnd::get_real_table( QString table_name_str , short rtu_no )
{
	RDB_BTBL_TAB_STRU tabinfo ;
	RDB_BTBL_COL_STRU colinfo ;
	void *raddr = NULL ;
	Rdb_SqlCli rl ;
	int tmp = 0 , old_rec_num = 0 , old_col_num = 0 ;
	short search_no ;
	bool sort_flag ;
	short table_name_no ;
	for ( int j = 0 ; j < Info.RtdbTableNum ; j++ )
	{
		if ( tr( Info.Rtdb[ j ].tabledescribe ) == table_name_str )
		{
			table_name_no = j ;
			break ;
		}
	}
	for ( int i = 0 ; i < Info.TerminalNum ; i++ )
	{
		if ( Info.Terminal[ i ].terminalno == rtu_no )
		{
			search_no = i ;
			break ;
		}
	}
	if ( rtu_no != -1 )
	{
		sort_flag = true ;
	}
	else 
	{
		sort_flag = false ;
	}
	char table[ CHAR_NAME ] ;
	strcpy( table , Info.Rtdb[ table_name_no ].tablename ) ;
	if ( !sort_flag&&Info.Rtdb[ table_name_no ].sortflag )
	{
		return false ;
	}
	tmp = rl.RdbSelectFrom( table ) ;

	if ( strlen( Info.Rtdb[ table_name_no ].orderfield ) != 0 )
	{
		rl.RdbSetOrdertype( 0 ) ;
		rl.RdbOrderBy( Info.Rtdb[ table_name_no ].orderfield ) ;
	}

	if( tmp != RDB_OK ) 
	{
		return false ;
	}

	if( sort_flag ) 
	{
		if ( ShowCfg->OrderByStation )
		{
			rl.RdbWhereCond( "substation" , RDB_DATATYPE_STRING , ( Info.Terminal[ search_no ].termname ) ) ;
		}
		else 
		{
			rl.RdbWhereCond( "terminalno" , RDB_DATATYPE_SHORT , &( Info.Terminal[ search_no ].terminalno ) ) ;
		}
	}
	tmp = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;			

	if ( tmp == RDB_OK )
	{
		rl.RdbGetTableInfo( &raddr , &tabinfo ) ; // 取表的首地址 , 记录个数 , 记录长度
		if( tabinfo.columnnums > 0 && tabinfo.columnnums < FIELDMAXNUM_REALTAB )
		{
			if( tabinfo.columnnums > old_col_num || !Info.pRealDataInfo )
			{
				Info.RealDataColNum = tabinfo.columnnums ;
				//取实时域信息----------------------------------------------------------------------
				tmp = Info.RealDataColNum*( int )sizeof( CField ) ; ;
				if( Info.pRealDataInfo ) 
				{
					free( Info.pRealDataInfo ) ;
				}
				Info.pRealDataInfo = NULL ;
				Info.pRealDataInfo = malloc( tmp ) ;
				if( !Info.pRealDataInfo )
				{
					return false ;
				}
				tmp = Info.RealDataColNum*( int )sizeof( RDB_BTBL_COL_STRU ) ;

				void *pcolumninfo = NULL ;
				pcolumninfo = malloc( tmp ) ;
				if( !pcolumninfo ) 
				{
					return false ;
				}
				RDB_BTBL_COL_STRU *colptr = NULL ;
				colptr = ( RDB_BTBL_COL_STRU * )( pcolumninfo ) ;
				tmp = rl.RdbGetAllColumnInfo( colptr ) ;				//一次读所有域信息
				CField *fd = ( CField * )( Info.pRealDataInfo ) ;	//借用参数定义的域结构
				for( int i = 0 ; i < Info.RealDataColNum ; i++ , fd++ )
				{
					if( tmp != RDB_OK || !colptr )	
					{
						rl.RdbGetColumnInfo( i , &colinfo ) ;
					}
					else 
					{
						colinfo = *( colptr+i ) ;
					}
					fd->SetStrv( colinfo.full_name ) ;
					fd->SetFieldValueAddr( colinfo.offset ) ;
					fd->SetFieldLen( colinfo.unitlen ) ;
					fd->SetType( colinfo.datatype ) ;
					fd->SetChangeFlag( colinfo.position ) ;		//位的位置
					fd->SetUseFlag( colinfo.bitnums ) ;			//位数
				}
				if( pcolumninfo )
				{
					free( pcolumninfo ) ;
					pcolumninfo = NULL ;
				}
			}

			Info.RealDataRecNum = tabinfo.rcdcount ;
			Info.RealDataRecNum = ( Info.RealDataRecNum )<30000?( Info.RealDataRecNum ):30000 ;		//小于30000记录
			Info.RealDataRecLen = tabinfo.recordlen ;
			//-------------------------------------------------------------------------------
			//取实时记录信息 == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == = 
			tmp = Info.RealDataRecNum*Info.RealDataRecLen ;
			if ( tmp == 0 ) 
			{
				return false ;
			}
			if( Info.pRealData ) 
			{
				free( Info.pRealData ) ;
			}
			Info.pRealData = NULL ;
			Info.pRealData = malloc( tmp ) ;
			char *lpfirst = rl.rdbgetfirstrcdaddr( ) ;
			if( lpfirst && Info.pRealData )
			{
				char * lpdata = ( char * )( Info.pRealData ) ;
				memmove( lpdata , lpfirst , tmp ) ;
			}
			else 
			{
				Info.RealDataRecNum = 0 ;
				Info.RealDataColNum = 0 ;
				TRACE( "没有该表的实时数据!" ) ;
				return false ;
			}
		}
	}
	else 
	{
		Info.RealDataRecNum = 0 ;
		Info.RealDataColNum = 0 ;
		TRACE( "没有该表的实时数据!" ) ;
		return false ;
	}
	return true ;
}

bool Rdbview_wnd::fill_data( QString table_name_str , short rtu_no , bool flag )
{
	QStringList headerList ;
	headerList.clear() ;

	int row = 0,col = 0 ;
	int row_count = 0 , col_count = 0 ;
	int yc_show_flag = 0 , yx_show_flag = 0 , kwh_show_flag = 2 ;
	QString tmpStr ;
	int table_name_no = 0 ; 
	char text[255] ;
	RDB_DATETIME_MS_STRU curclock ;
	Rdb_Time rdbtime ;

	row_count = Info.RealDataNum ;
	if ( ShowCfg->ShowYcCalcult )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcManset )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcxs )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcKy )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYxCalcult )
		yx_show_flag++ ;
	if ( ShowCfg->ShowYxManset )
		yx_show_flag++ ;

	QStringList ntr1,ntr2,ntr3,ntr4 ;

	if ( Info.RealFlag == SCADA_REAL )
	{
		for ( int i = 0 ; i < Info.RtdbTableNum ; i++ )
		{
			if ( tr( Info.Rtdb[ i ].tabledescribe ) == table_name_str )
			{
				table_name_no = i ;
				break ;
			}
		}
		CField *fd = ( CField * )( Info.pRealDataInfo ) ;
		char *lp = ( char* )( Info.pRealData ) ;
		row_count = Info.RealDataRecNum ;
		col_count = Info.RealDataColNum ;
		if( Info.RealDataRecNum <= 0 )
		{
			m_pRead_info_label->setText( tr( "数据读取信息:无有效数据" ) ) ;
			return false ;
		}
		else 
		{
			char text_char[ 200 ] ;
			sprintf( text_char , "数据读取信息:读取到%d条有效数据信息" , Info.RealDataRecNum ) ;
			m_pRead_info_label->setText( tr( text_char ) ) ;
		}
		m_pNormal_table->setRowCount( row_count ) ;
		if ( bFlag )
		{
			m_pNormal_table->setColumnCount( col_count ) ;
			for ( int i = 0 ; i < col_count ; i++ )
			{
				ntr1 << tr( Info.Rtdb[ table_name_no ].col[ i ].coldescribe ) ;
			}
			m_pNormal_table->setHorizontalHeaderLabels( ntr1 ) ;
		}
		for ( int i = 0 ; i < col_count ; i++ )
		{
			if ( Info.Rtdb[ table_name_no ].col[ i ].flag == false )
			{
				m_pNormal_table->hideColumn( i ) ;//隐藏不显示的列
			}
			else
			{
				m_pNormal_table->showColumn( i ) ;//隐藏不显示的列
			}
		}

		// 增加时间显示的处理，配置rdbview.ini文件中的时间显示 [2/17/2017 ljs]
		// 增加表头域名，配置rdbview.ini文件中的表头域名，add by lbh 2017年3月5日
		QList<int> int_collist ;
		QList<int> int_headlist ;
		int_collist.clear() ;
		int_headlist.clear() ;
		char file_name[ 256 ] ;
// 		sprintf( file_name,"%s/ini/rdbview.ini",getenv( "NBENV" ) ) ;
// 		ini_RW* pini = new ini_RW( file_name ) ;
		sprintf( file_name,"%s/resource/xml/rdbview.xml",getenv( "NBENV" ) ) ;
		Xml_RW* pini = new Xml_RW( file_name ) ;
		bool rtdbcolflag = false ;
		char *field, *headfield ;
		field = NULL ;
		headfield = NULL ;

		if ( SUCCESS == pini->get_string( "时间显示", _Q2C( table_name_str ), field ) )
		{
			rtdbcolflag = true ;	
			QString m_col = tr( field ) ;
			QStringList str_collist = m_col.split( ",", QString::SkipEmptyParts ) ;
			for ( int listcount = 0 ;listcount<str_collist.count() ;listcount++ )
			{
				QString colno = str_collist.at( listcount ) ;
				int_collist.append( colno.toInt() ) ;
			}
		}
		if ( SUCCESS == pini->get_string( "表头域名", _Q2C( table_name_str ), headfield ) )
		{
			for ( int j = 0 ; j < col_count ; j++ )
			{
				if ( tr( headfield ) == tr( Info.Rtdb[table_name_no].col[ j ].coldescribe ) )
				{
					int_headlist.append( j ) ;
				}
			}
		}

		for ( int row = 0 ; row<row_count ; row++ )
		{
			for ( int col = 0 ; col < col_count ; col++ )
			{
				get_one_real_data_val( ( fd+col )->GetType(),( fd+col )->GetUseFlag(),( fd+col )->GetChangeFlag(),
					( lp+( row*( Info.RealDataRecLen ) )+( fd+col )->GetFieldValueAddr() ),text ) ;
				if ( rtdbcolflag )
				{
					if ( int_collist.contains( col + 1) )
					{
						rdbtime.RdbConvertTime( atoi( text ),&curclock ) ;
						QString mdate ;
						mdate.sprintf( "%d/%d/%d %d:%d:%d",curclock.year,curclock.month,curclock.day,curclock.hour,curclock.minute,curclock.second ) ;
						m_pNormal_table->setItem( row, col,new QTableWidgetItem( mdate ) ) ;
					}
					else
					{
						m_pNormal_table->setItem( row, col, new QTableWidgetItem( tr( text ) ) ) ;		
					}
				}

				else
				{
					m_pNormal_table->setItem( row, col, new QTableWidgetItem( tr( text ) ) ) ;	
				}

				if ( int_headlist.contains( col ) )
				{
					headerList << tr( text ) ;
				}
			}
		}
		if ( headerList.count() > 0 )
		{
			m_pNormal_table->setVerticalHeaderLabels( headerList ) ;
		}
		else
		{
			headerList.clear() ;
			for ( int k = 0 ; k < row_count ; k++ )
			{
				headerList << tr( "" ) ;
			}
			m_pNormal_table->setVerticalHeaderLabels( headerList ) ;
		}
		m_pNormal_table->resizeColumnsToContents() ;
		return true ;
	}
	if( Info.RealDataNum <= 0 )
	{
		m_pRead_info_label->setText( tr( "数据读取信息:无有效数据" ) ) ;
		return false ;
	}
	else 
	{
		char text_char[200] ;
		sprintf( text_char , "数据读取信息:读取到%d条有效数据信息" , Info.RealDataNum ) ;
		m_pRead_info_label->setText( tr( text_char ) ) ;
	}
	if ( Info.RealFlag == YC )	
	{
		m_pNormal_table->setRowCount( row_count ) ;
		col_count = 9 ;
		if ( flag )
		{
			m_pNormal_table->setColumnCount( col_count ) ;

			for ( int j = 0 ; j < col_count ; j++ )
			{
				ntr4 << tr( RealYcColName[j] ) ;
				m_pNormal_table->showColumn( j ) ;
			}
			m_pNormal_table->setHorizontalHeaderLabels( ntr4 ) ;
		}
		if ( !ShowCfg->ShowYcCalcult )
			m_pNormal_table->hideColumn( 6 ) ;	
		else
			m_pNormal_table->showColumn( 6 ) ;

		if ( !ShowCfg->ShowYcKy )
			m_pNormal_table->hideColumn( 8 ) ;
		else
			m_pNormal_table->showColumn( 8 ) ;

		if ( !ShowCfg->ShowYcManset )
			m_pNormal_table->hideColumn( 5 ) ;
		else
			m_pNormal_table->showColumn( 5 ) ;
		if ( !ShowCfg->ShowYcxs )
			m_pNormal_table->hideColumn( 7 ) ;
		else
			m_pNormal_table->showColumn( 7 ) ;
	}
	else if ( Info.RealFlag == YX )
	{
		m_pNormal_table->setRowCount( row_count ) ;
		col_count = 7 ;
		if ( bFlag )
		{
			m_pNormal_table->setColumnCount( col_count ) ;
			for ( int j = 0 ; j < col_count ; j++ )
			{
				 ntr2 << tr( RealYxColName[j] ) ;	
				 m_pNormal_table->showColumn( j ) ;
			}
			m_pNormal_table->setHorizontalHeaderLabels( ntr2 ) ;
		}

		if ( !ShowCfg->ShowYxManset )
			m_pNormal_table->hideColumn( 5 ) ;	
		else
			m_pNormal_table->showColumn( 5 ) ;
		if ( !ShowCfg->ShowYxManset )
			m_pNormal_table->hideColumn( 6 ) ;	
		else
			m_pNormal_table->showColumn( 6 ) ;

	}
	else if ( Info.RealFlag == KWH )
	{			
		m_pNormal_table->setRowCount( row_count ) ;
		col_count = 5+kwh_show_flag ;
		if ( bFlag )
		{
			m_pNormal_table->setColumnCount( col_count ) ;
			for ( int j = 0 ; j < col_count ; j++ )
			{
				ntr3 << tr( RealKwhColName[j] ) ;
				m_pNormal_table->showColumn( j ) ;
			}
			m_pNormal_table->setHorizontalHeaderLabels( ntr3 ) ;
		}
		if ( ( ShowCfg->ShowKwhTableCode ) )
			m_pNormal_table->horizontalHeaderItem( 4 )->setText( tr( "表码值" ) ) ;
	}
	for ( int row = 0 ;row<row_count ;row++ )
	{
		for ( int col = 0 ;col<col_count ;col++ )
		{
			if ( col == 0 )
			{
				m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( Info.RealData[row].name ) ) ) ;
			}
			else if ( col == 1 )
			{
				m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( Info.RealData[row].describe ) ) ) ;
				headerList << tr( Info.RealData[row].describe ) ;
			}
			else if ( col == 2 )
			{				
				m_pNormal_table->setItem( row,col,new QTableWidgetItem( QString( "%1" ).arg( Info.RealData[row].terminalno ) ) ) ;
			}
			else if ( col == 3 )
			{
				m_pNormal_table->setItem( row,col,new QTableWidgetItem( QString( "%1" ).arg( Info.RealData[row].no ) ) ) ;
			}
			else if ( col == 4 )
			{
				switch( Info.RealFlag )
				{
				case YX:
					if( Info.RealData[row].value )	
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "合" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;

					}
					else 
					{
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "分" ) ) ) ;
					}
					break ;
				case KWH:
					tmpStr.sprintf( "%.4f", ( float )Info.RealData[row].daykwh ) ;
					m_pNormal_table->setItem( row,col,new QTableWidgetItem( tmpStr ) ) ;
					break ;
				case YC:
					tmpStr.sprintf( "%.2f", ( float )Info.RealData[row].value ) ;
					m_pNormal_table->setItem( row,col,new QTableWidgetItem( tmpStr ) ) ;
					break ;
				}
			}//col4结束
			else if( col == 5 ) 
			{	
				switch( Info.RealFlag )
				{
				case YC:
					if ( Info.RealData[row].mansetf == 1 )
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "锁定" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;
					}
					else if ( Info.RealData[row].mansetf == 0 ) 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未锁定" ) ) ) ;
					else
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ;
					break ;
				case YX:
					if ( Info.RealData[row].mansetf == 1 ) 
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "锁定" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;
					}
					else if ( Info.RealData[row].mansetf == 0 ) 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未锁定" ) ) ) ;
					else 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ;

					break ;
				case KWH:
					if ( Info.RealData[row].mansetf == 1 ) 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "锁定" ) ) ) ;
					else if ( Info.RealData[row].mansetf == 0 ) 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未锁定" ) ) ) ;
					else 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ;
					break ;
				}

			}
			else if( col == 6 ) 
			{	
				switch ( Info.RealFlag )
				{
				case YC:
					if ( Info.RealData[row].calcultf == 1 )
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "计算" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;		
					}
					else if ( Info.RealData[row].calcultf == 0 )
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未计算" ) ) ) ;
					else 
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ;

					break ;
				case YX:
					if ( Info.RealData[row].calcultf == 1 ) 
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "计算" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;		
					}
					else if ( Info.RealData[row].calcultf == 0 )	
					{
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未计算" ) ) ) ;
					}
					else 
					{
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ; 
					}
					break ;
				case KWH:
					if (Info.RealData[row].calcultf  ==  1)
					{
						QTableWidgetItem *item1 = new QTableWidgetItem ;
						m_pNormal_table->setItem( row, col, item1 ) ;
						item1->setText( tr( "计算" ) ) ;
						item1->setForeground( QBrush( Qt::red ) ) ;		
					}

					else if ( Info.RealData[row].calcultf == 0 )	
					{
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "未计算" ) ) ) ;
					}
					else 
					{
						m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ; 
					}
					break ;
				}
			}//col6结束
			else if ( col == 7 )
			{
				tmpStr.sprintf( "%.2f/%.2f",( float )Info.RealData[row].slope,( float )Info.RealData[row].scale ) ;
				m_pNormal_table->setItem( row,col,new QTableWidgetItem( tmpStr ) ) ;
			}//col7结束
			else if ( col == 8 )
			{
				if ( Info.RealData[row].inconsistf == 1 )	
				{
					QTableWidgetItem *item1 = new QTableWidgetItem ;
					m_pNormal_table->setItem( row, col, item1 ) ;
					item1->setText( tr( "可疑" ) ) ;
					item1->setForeground( QBrush( Qt::red ) ) ;
				}
				else if ( Info.RealData[row].inconsistf == 0 ) 
					m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "正常" ) ) ) ;
				else 
					m_pNormal_table->setItem( row,col,new QTableWidgetItem( tr( "不确定" ) ) ) ;
			}
		}
	}
	if ( headerList.count() > 0 )
	{
		m_pNormal_table->setVerticalHeaderLabels( headerList ) ;
	}
	m_pNormal_table->resizeColumnsToContents() ;
	return true ;
}

bool Rdbview_wnd::fill_empty_data( QString table_name_str , short rtu_no )
{
	int row_count , col_count ;
	int yc_show_flag = 0 , yx_show_flag = 0 , kwh_show_flag = 2 ;
	int table_name_no = 0 ; 
	int show_col_num = 0 ;
	m_pNormal_table->setColumnCount( 0 ) ;
	m_pNormal_table->setRowCount( 0 ) ;
	row_count = 0 ;

	QStringList mtr1 , mtr2 , mtr3 , mtr4 ;

	if ( ShowCfg->ShowYcCalcult )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcManset )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcxs )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYcKy )
		yc_show_flag++ ;
	if ( ShowCfg->ShowYxCalcult )
		yx_show_flag++ ;
	if ( ShowCfg->ShowYxManset )
		yx_show_flag++ ;

	if ( Info.RealFlag == SCADA_REAL )
	{
		for ( short t = 0 ;t<Info.RtdbTableNum ;t++ )
		{
			if ( tr( Info.Rtdb[t].tabledescribe ) == table_name_str )
			{
				table_name_no = t ;
				break ;
			}

		}
		for( int i = 0 ;i<Info.Rtdb[table_name_no].colnum ;i++ )
		{
			if( Info.Rtdb[table_name_no].col[i].flag == 1 )
				show_col_num++ ;
		}
		CField *fd = ( CField * )( Info.pRealDataInfo ) ;
		char *lp = ( char* )( Info.pRealData ) ;
		row_count = Info.RealDataRecNum ;
		col_count = Info.RealDataColNum ;
		m_pNormal_table->setColumnCount( col_count ) ;
		m_pNormal_table->setRowCount( row_count ) ;


		for ( int ii = 0 ;ii<col_count ;ii++ )
		{
			if ( Info.Rtdb[table_name_no].col[ii].flag )
			{
				mtr1 << tr( Info.Rtdb[table_name_no].col[ii].coldescribe ) ;
			} 
			else
			{
				m_pNormal_table->hideColumn( ii ) ;
			}
		}
		m_pNormal_table->setHorizontalHeaderLabels( mtr1 ) ;
		return true ;
	}
	if ( Info.RealFlag == YC )	
	{
		col_count = 5+yc_show_flag ;
		m_pNormal_table->setColumnCount( col_count ) ;
		m_pNormal_table->setRowCount( row_count ) ;
		for ( int j = 0 ; j < col_count ; j++ )
		{
			mtr2 << tr( RealYcColName[j] ) ;
		}
		m_pNormal_table->setHorizontalHeaderLabels( mtr2 ) ;
	}
	else if ( Info.RealFlag == YX )
	{
		col_count = 5+yx_show_flag ;
		m_pNormal_table->setColumnCount( col_count ) ;
		m_pNormal_table->setRowCount( row_count ) ;
		for ( int j = 0 ; j < col_count ; j++ )
		{
			if ( ( j >= 5 ) && ( ShowCfg->ShowYxManset == 0 ) && ( ShowCfg->ShowYxCalcult ) )
				mtr3 <<tr( RealYxColName[j+1] ) ;
			else
				mtr3 <<tr( RealYxColName[j] ) ;
		}
		m_pNormal_table->setHorizontalHeaderLabels( mtr3 ) ;
	}
	else if ( Info.RealFlag == KWH )
	{
		if ( bFlag )
		{
			col_count = 5+kwh_show_flag ;
			m_pNormal_table->setColumnCount( col_count ) ;
			m_pNormal_table->setRowCount( row_count ) ;
			for ( int j = 0 ; j < col_count ; j++ )
			{
				if( j == 4 && ShowCfg->ShowKwhTableCode )
				{
					mtr4 << _C2Q( "表码值" ) ;
				}
				else
				{
					mtr4 << tr( RealKwhColName[j] ) ;
				}
			}
			m_pNormal_table->setHorizontalHeaderLabels( mtr4 ) ;
		}
	}	
	return true ;
}

int Rdbview_wnd::get_one_real_data_val( short data_type , short bit_count , short pos , char* val , char * msg_str )
{
	if( !val || !msg_str )	
	{
		return 0 ;
	}
	char c ;
	unsigned char bitv = 0 ;
	short *spt ;
	unsigned short *uspt ;
	int *ipt ;
	unsigned int *uipt ;
	hlong *lpt ;
	float *fpt ;
	double *dpt ;
	unsigned char bitmask[9] = {0 , 1 , 3 , 7 , 0xf , 0x1f , 0x3f , 0x7f , 0xff} ;
	char * valtemp ;
	pos = pos <= ( short )7?pos:( short )7 ;
	bit_count = bit_count <= ( short )8?bit_count:( short )8 ;
	switch	( data_type )
	{
	case RDB_DATATYPE_CHAR:
		{
			bitv = *( BYTE * )val ;
			if( bit_count != 0 )
			{
				bitv >>= pos ;
				bitv &= bitmask[bit_count] ;
			}
			sprintf( msg_str , "%d" , ( char )bitv ) ;
			return true ;
		}
	case RDB_DATATYPE_UCHAR:
		{
			c = *( char * )val ;
			if( bit_count != 0 )
			{
				c = *( char * )val ;
				c >>= pos ;
				c &= bitmask[bit_count] ;
			}
			sprintf( msg_str , "%d" , c ) ;
			return true ;
		}
	case RDB_DATATYPE_USHORT:
		{
			uspt = ( USHORT * )val ;
			sprintf( msg_str , "%d" , *uspt ) ;
			return true ;
		}
	case RDB_DATATYPE_SHORT:
		{
			spt = ( short * )val ;
			sprintf( msg_str , "%d" , *spt ) ;
			return true ;
		}
	case RDB_DATATYPE_INT:
		{
			ipt = ( int * )val ;
			sprintf( msg_str , "%d" , *ipt ) ;
			return true ;
		}
	case RDB_DATATYPE_UINT:
		{
			uipt = ( UINT * )val ;
			sprintf( msg_str , "%d" , *uipt ) ;
			return true ;
		}
	case RDB_DATATYPE_LONG: 
	case RDB_DATATYPE_ULONG:
		{
			lpt = ( hlong * )val ;
			sprintf( msg_str , "%ld" , *lpt ) ;
			return true ;
		}
	case RDB_DATATYPE_FLOAT:
		{
			fpt = ( float * )val ;
			float_to_asc( ( double )*fpt , 3 , msg_str ) ;
			return true ;
		}
	case RDB_DATATYPE_DOUBLE:
		{
			dpt = ( double * )val ;
			float_to_asc( *dpt , 3 , msg_str ) ;
			return true ;
		}
	case RDB_DATATYPE_STRING:
		{
			valtemp =( char * )val ;
			strcpy( msg_str , valtemp ) ;
			return true ;
		}
	default: 
		msg_str[0] = NULL ;
		return false ;
	}
	return false ;
}

void Rdbview_wnd::float_to_asc( double dv , unsigned char point_count , char* asc )
{
	if( point_count == 0 ) sprintf( asc , "%-8.1f" , dv ) ;
	else if( point_count == 1 ) sprintf( asc , "%-8.1f" , dv ) ;
	else if( point_count == 2 ) sprintf( asc , "%-8.2f" , dv ) ;
	else if( point_count == 3 ) sprintf( asc , "%-10.3f" , dv ) ;
	else if( point_count == 4 ) sprintf( asc , "%-11.4f" , dv ) ;
	else if( point_count == 5 ) sprintf( asc , "%-12.5f" , dv ) ;
	else if( point_count == 6 ) sprintf( asc , "%-12.6f" , dv ) ;
	else if( point_count == 7 ) sprintf( asc , "%-12.7f" , dv ) ;
	else if( point_count == 8 ) sprintf( asc , "%-14.8f" , dv ) ;
	else if( point_count == 9 ) sprintf( asc , "%-15.9f" , dv ) ;
	else if( point_count == 10 ) sprintf( asc , "%-16.10f" , dv ) ;
	else sprintf( asc , "%-12.1f" , dv ) ;
}

bool Rdbview_wnd::get_rtdb_terminal_info( void )
{
	Rdb_SqlCli rl ;

	rl.RdbSelectFrom( "terminal" ) ;
	rl.RdbOrderBy( "terminalno" ) ;
	rl.RdbSelectField( "name" ) ;
	rl.RdbSelectField( "describe" ) ;
	rl.RdbSelectField( "terminalno" ) ;

	int	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return false ;
	}

	int rec_number = rl.rdbgetrcdcnt() ;
	if ( rec_number == 0 ) 
	{
		return false ;
	}

	Info.TerminalNum = rec_number ;
	int termnum = Info.TerminalNum ;
	Info.Terminal = new TERMINAL[ termnum ] ;
	memset( Info.Terminal , 0 , sizeof( termnum*sizeof( TERMINAL ) ) ) ;
	for( int i = 0 ; i < rec_number ; i++ )
	{
		strcpy( Info.Terminal[ i ].termname , rl.RdbGetString( i , "name" ) ) ;
		strcpy( Info.Terminal[ i ].termdescribe , rl.RdbGetString( i , "describe" ) ) ;
		Info.Terminal[ i ].terminalno = rl.RdbGetVal_uint( i , "terminalno" ) ;
	}
	return true ;
}

QString Rdbview_wnd::get_rtdb_table_name( QString rtdb_table_describe_str )
{
	for ( int i = 0 ; i < Info.RealDataNum ; i++ )
	{
		if ( tr( Info.Rtdb[ i ].tabledescribe ) == rtdb_table_describe_str )
		{
			return tr( Info.Rtdb[ i ].tablename ) ;
		}
	}
	return rtdb_table_describe_str;
}

void Rdbview_wnd::set_total_row_col_num( int total_row , int total_col )
{
	m_pNormal_table->setRowCount( total_row ) ;
	m_pNormal_table->setColumnCount( total_col + 1 ) ;
	m_pNormal_table->clear();
}

bool Rdbview_wnd::get_cur_computer_no( void )
{
	FUNC_STR func ;
	CS_DATAFMT* datafmt ;
	func.func  =  GET_TABLE_INFO ;
	int j1 , ptcnt ;

	char host_name[ CHAR_NAME ] ;
	gethostname( host_name , CHAR_NAME - 1 ) ;

	strcpy( func.table , "NODEPARA" ) ;
	strcpy( func.dbname , "modeldb" ) ;

	if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
	{
		return false ;
	}
	for ( int i = 0 ; i < func.colnum ; i++ )
	{
		func.col[ i ].condition  =  false ;
		func.col[ i ].select  =  false ;
		func.col[ i ].order   =  -1 ;
		func.col[ i ].order_d  =  -1 ;
	}
	void *pt ;
	func.func = SEL_ISQL_RECORDERS ;
	sprintf( func.isql , "hostname = '%s'" , host_name ) ;
	if ( ( db.SelectRcds( &func , datafmt , &pt ) ) != CS_SUCCEED )
	{
		return false ;
	}

	if ( func.ret_roxnum  ==  0 )
	{
		return false ;
	}
	cur_computer_group_no = ( int )db.GetData( 0 , &func , datafmt + j1 , pt , ptcnt , NULL ) ;

	if ( pt ) 
	{
		free ( pt ) ;
		pt = NULL ;
	}
	if ( datafmt ) 
	{
		free ( datafmt ) ;
		datafmt = NULL ;
	}
	return true ;
}

inline   char*   strlwr(   char*   str   ) 
{ 
	char*   orig   =   str; 
	for   (   ;   *str   !=   '\0 ';   str++   ) 
		*str   =   tolower(*str); 
	return   orig; 
} 

void Rdbview_wnd::rsql_handling( void )
{
	int retcode = m_gettoken.GetWord() ;

	if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
	}

	else
	{
		if ( strcmp( strlwr( m_gettoken.word ) , "select" ) == 0 ) // identical to "select"
		{
			analyze_select( ) ;
		}
		else if ( strcmp( strlwr( m_gettoken.word ) , "help" ) == 0 ) // identical to "help"
		{
			analyze_help( ) ;
		}
		else if ( strcmp( strlwr( m_gettoken.word ) , "update" ) == 0 ) // identical to "update"
		{
			analyze_update( ) ;
		}
		else if ( strcmp( strlwr( m_gettoken.word ) , "delete" ) == 0 ) // identical to "delete"
		{
			analyze_delete( ) ;
		}
		else if ( strcmp( strlwr( m_gettoken.word ) , "insert" ) == 0 ) // identical to "insert"
		{
			analyze_insert( ) ;
		}
		else
		{
			m_err_code = SYNTAXERR ;
		}
	}

	if ( m_err_code != OPERATION_OK )
	{
		error_handling() ; //display error message into dbgrid
	}
}

void Rdbview_wnd::error_handling( void )
{
	QString tmpstr ;

	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	switch ( m_err_code )
	{
	case SYNTAXERR:
		tmpstr = tr( "语法错误!" ) ;
		break ;

	case RDB_ERR_NOTAB:
		tmpstr = tr( "表不存在 , 可能表名错误" ) ;
		break ;

	case INITMEMERR:
		tmpstr = tr( "初始化内存错误" ) ;
		break ;

	case RDB_ERR_NOCOL:
		tmpstr = tr( "表中无此列 , 可能列名错误" ) ;
		break ;

	case RDB_ERR_NORCD:
		tmpstr = tr( "表中无记录或无此记录" ) ;
		break ;

	case RDB_ERR_NORIGHT:
		tmpstr = tr( "权限不够 , 无权操作" ) ;
		break ;

	case RDB_ERR_MALLOC:
		tmpstr = tr( "共享内存错误" ) ;
		break ;

	case RDB_ERR_ADDTAB:
		tmpstr = tr( "增加表失败" ) ;
		break ;

	case RDB_ERR_ADDUSER:
		tmpstr = tr( "增加用户失败" ) ;
		break ;

	case RDB_ERR_NOUSER:
		tmpstr = tr( "无此用户 , 请查询" ) ;
		break ;

	case RDB_ERR_OPENTAB:
		tmpstr = tr( "打开表失败" ) ;
		break ;

	case RDB_ERR_ADDRCD:
		tmpstr = tr( "增加记录失败" ) ;
		break ;

	case RDB_ERR_DELRCD:
		tmpstr = tr( "删除记录失败" ) ;
		break ;

	case RDB_ERR_LOGIN:
		tmpstr = tr( "登录失败" ) ;
		break ;

	case RDB_ERR_SQLWHR:
		tmpstr = tr( "SQL条件错误" ) ;
		break ;

	case RDB_ERR_NET:
		tmpstr = tr( "网络错误" ) ;
		break ;

	case RDB_ERR_BKUP:
		tmpstr = tr( "备份错误" ) ;
		break ;

	case RDB_ERR_COLMATCH:
		tmpstr = tr( "列类型或个数错误 , 不匹配" ) ;
		break ;

	case RDB_ERR_HISSQL:
		tmpstr = tr( "历史库SQL命令错误" ) ;
		break ;

	case RDB_ERR_EVENT:
		tmpstr = tr( "事件处理错误" ) ;
		break ;

	case RDB_ERR_PARA:
		tmpstr = tr( "参数错误" ) ;
		break ;

	case RDB_ERR_DATATYPE:
		tmpstr = tr( "错误的数据类型" ) ;
		break ;

	case RDB_ERR_VERSION:
		tmpstr = tr( "程序版本不对" ) ;
		break ;

	case RDB_ERR_CMD:
		tmpstr = tr( "不支持的命令" ) ;
		break ;

	default :
		{
			tmpstr = tr( "内部错误:%1" ).arg( m_err_code ) ;
			break ;
		}
	} //end of switch

	//next store error information to perinfo pointer
	m_rdb_ret_info.RcdNum = 1 ;
	m_rdb_ret_info.ColumnNum = 1 ;
	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "返回错误信息" ) ;

	int tmpint = max( strlen( m_rdb_ret_info.ColumnName[ 0 ] ) , strlen( _Q2C( tmpstr ) ) )+1 ;

	m_rdb_ret_info.ColumnLen[ 0 ] = tmpint ;

	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;

	m_rdb_ret_info.RcdLen = tmpint ;

	m_rdb_ret_info.pretinfo = ( char * )malloc( tmpint ) ;

	strcpy( m_rdb_ret_info.pretinfo , _Q2C( tmpstr ) ) ;
}

void Rdbview_wnd::update_table( void )
{
	char text_char[ 200 ] ;

	m_pSql_table->setColumnCount( 0 ) ;
	m_pSql_table->setRowCount( 0 ) ;

	unsigned int col_count = m_rdb_ret_info.ColumnNum ;
	unsigned int rec_count = m_rdb_ret_info.RcdNum ;
	if ( rec_count > 0 )
	{	
		sprintf( text_char , "表格列数为:%d" , rec_count ) ;
		m_pSelect_label->setText( tr( text_char ) ) ;
	}

	unsigned int rec_len = m_rdb_ret_info.RcdLen ;
	m_pSql_table->setRowCount( rec_count ) ;
	m_pSql_table->setColumnCount( col_count ) ;

	QStringList tmp_str_list ;
	for ( int i = 0 ; i < col_count ; i++ )
	{
		tmp_str_list << tr( m_rdb_ret_info.ColumnName[ i ] ) ;
	}
	m_pSql_table->setHorizontalHeaderLabels( tmp_str_list ) ;

	for ( int i = 0 ; i < rec_count ; i++ )
	{
		for ( int j = 0 ; j < col_count ; j++ )
		{
			m_pSql_table->setItem( i , j , new QTableWidgetItem( tr( m_rdb_ret_info.pretinfo + i*rec_len + m_rdb_ret_info.ColumnOffset[ j ] ) ) ) ;
		}
	}
	m_pSql_table->resizeColumnsToContents( ) ;
}

void Rdbview_wnd::analyze_select( void )
{
	char selecttblname[ 100 ] ;
	int ret = 0 ;

	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	m_bGet_dict = false ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbClearSQLCmd() ;
	}
	else
	{
		m_pRsql_server->RdbClearSQLCmd() ;
	}
	int retcode = m_gettoken.GetWord() ;

	if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( strcmp( strlwr( m_gettoken.word ) , "*" ) != 0 )
	{
		for ( ; ; )
		{
			if ( m_rdb_type == LOCAL )
			{
				m_pRsql_local->RdbSelectField( m_gettoken.word ) ;
			}
			else
			{
				m_pRsql_server->RdbSelectField( m_gettoken.word ) ;
			}

			retcode = m_gettoken.GetWord( ) ;

			if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
			{
				m_err_code = SYNTAXERR ;
				return ;
			}

			if ( strcmp( strlwr( m_gettoken.word ) , "," ) != 0 )
				break ;

			retcode = m_gettoken.GetWord( ) ;

			if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
			{
				m_err_code = SYNTAXERR ;
				return ;
			}
		}
	}
	else // select columns is *
	{
		retcode = m_gettoken.GetWord( ) ;

		if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}
	}

	if ( strcmp( strlwr( m_gettoken.word ) , "from" ) )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	retcode = m_gettoken.GetWord( ) ;

	if ( retcode == RDB_LINE_EOLTYPE || retcode == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	strcpy( selecttblname ,  m_gettoken.word ) ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbSelectFrom( m_gettoken.word ) ;
	}
	else
	{
		m_pRsql_server->RdbSelectFrom( m_gettoken.word ) ;
	}

	retcode = m_gettoken.GetWord( ) ;

	if ( strcmp( strlwr( m_gettoken.word ) , "where" ) == 0 )
	{
		if ( analyze_where( selecttblname ) != 0 ) return ;
	}
	else if ( strcmp( ( m_gettoken.word ) , "order" ) == 0 )
	{
		if ( analyze_orderby( ) != 0 ) return ;
	}
	else if ( m_gettoken.word[ 0 ] != '\0' )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( m_err_code != OPERATION_OK )
		return ;

	//syntax is ok ,  now can select records from real time database
	if ( m_rdb_type == LOCAL )
	{
		ret = m_pRsql_local->RdbSelCmd( m_rsql_username , m_rsql_password ) ;
	}
	else
	{
		ret = m_pRsql_server->RdbSelCmd( 1 , m_rsql_username , m_rsql_password ) ;
	}

	if ( ret != RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	if ( gen_selected_rcds() != 0 )
		return ;
}

void Rdbview_wnd::analyze_help( void )
{
	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	m_bGet_dict = false ;
	int retcode = m_gettoken.GetWord() ;

	if ( retcode == RDB_LINE_EOLTYPE )  //user want to know which tables in realtime database
	{
		gen_table_info() ;
	}
	else  //user want to display real table struct
	{
		int ret ;
		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbClearSQLCmd() ;
			m_pRsql_local->RdbSelectFrom( m_gettoken.word ) ;
			m_pRsql_local->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
			ret = m_pRsql_local->RdbSelCmd( m_rsql_username , m_rsql_password ) ;
		}
		else
		{
			m_pRsql_server->RdbClearSQLCmd() ;
			m_pRsql_server->RdbSelectFrom( m_gettoken.word ) ;
			m_pRsql_server->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
			ret = m_pRsql_server->RdbSelCmd( 1 , m_rsql_username , m_rsql_password ) ;
		}

		if ( ret != RDB_OK )
		{
			m_err_code = ret ;
			return ;
		}
		gen_table_struct() ;
	}
}

void Rdbview_wnd::analyze_update( void )
{
	int wordtype ;
	char attr_name[ 40 ] ;
	char select_table_name[ 40 ] ;
	RDB_BTBL_COL_STRU tmp_attr_info ;
	RDB_VALUETYPE_STRU vtval ;

	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	m_bGet_dict = false ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbClearSQLCmd() ;
	}
	else
	{
		m_pRsql_server->RdbClearSQLCmd() ;
	}

	int ret = m_gettoken.GetWord() ;

	if ( ret != RDB_LINE_NAMETYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	strcpy( select_table_name , m_gettoken.word ) ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbUpdTab( m_gettoken.word ) ;
		m_pLocal_dict->RdbSelectFrom( select_table_name ) ;
		m_pLocal_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
		ret = m_pLocal_dict->RdbSelCmd( m_rsql_username , m_rsql_password ) ;
	}
	else
	{
		m_pRsql_server->RdbUpdTab( m_gettoken.word ) ;
		m_pSer_dict->RdbSelectFrom( select_table_name ) ;
		m_pSer_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
		ret = m_pSer_dict->RdbSelCmd( 1 , m_rsql_username , m_rsql_password ) ;
	}

	if ( ret!= RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	ret = m_gettoken.GetWord( ) ;

	if ( ret != RDB_LINE_NAMETYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( strcmp( m_gettoken.word , "set" ) != 0 )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	for ( ; ; )
	{
		ret = m_gettoken.GetWord( ) ;

		if ( ret!= RDB_LINE_NAMETYPE )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		strcpy( attr_name , m_gettoken.word ) ;

		ret = m_gettoken.GetWord( ) ;

		if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		if ( strcmp( m_gettoken.word , "=" ) )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		wordtype = m_gettoken.GetWord( ) ;

		if ( ret == RDB_LINE_EOLTYPE || wordtype == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		if ( m_rdb_type == LOCAL )
		{
			ret = m_pLocal_dict->RdbGetColumnInfo( attr_name , &tmp_attr_info ) ;
		}
		else
		{
			ret = m_pSer_dict->RdbGetColumnInfo( attr_name , &tmp_attr_info ) ;
		}

		if ( ret != RDB_OK )
		{
			m_err_code = ret ;
			return ;
		}

		if ( tmp_attr_info.datatype <= RDB_DATATYPE_LONG && wordtype != RDB_LINE_IDIGITTYPE )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}
		else if ( tmp_attr_info.datatype >= RDB_DATATYPE_FLOAT && tmp_attr_info.datatype <= RDB_DATATYPE_DOUBLE
			&& ( wordtype != RDB_LINE_FDIGITTYPE && wordtype != RDB_LINE_IDIGITTYPE ) )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		Valtype_SwapinDecVal( &vtval , tmp_attr_info.datatype , m_gettoken.word ) ;

		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbUpdFld( attr_name , &vtval ) ;
		}
		else
		{
			m_pRsql_server->RdbUpdFld( attr_name , &vtval ) ;
		}

		ret = m_gettoken.GetWord( ) ;

		if ( strcmp( m_gettoken.word , "," ) )break ;
	}

	if ( strcmp( m_gettoken.word , "where" ) == 0 )
	{
		m_bGet_dict = true ;
		ret = analyze_where( select_table_name ) ;

		if ( ret != RDB_OK )
		{
			m_err_code = ret ;
			return ;
		}
	}
	else if ( m_gettoken.word[ 0 ] != '\0' )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( m_rdb_type == LOCAL )
	{
		ret = m_pRsql_local->RdbUpdCmd( m_rsql_username , m_rsql_password ) ;
	}
	else
	{
		ret = m_pRsql_server->RdbUpdCmd( 1 , m_rsql_username , m_rsql_password ) ;
	}

	if ( ret!= RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	m_rdb_ret_info.RcdNum = 1 ;

	m_rdb_ret_info.ColumnNum = 1 ;

	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "返回结果" ) ;

	char tmpChar[10];
	strcpy(tmpChar,"成功修改记录!");
// 	QString tmpstr = tr( "成功修改记录!" ) ;

	int tmpint = max( strlen( m_rdb_ret_info.ColumnName[ 0 ] ) , strlen( /*_Q2C( tmpstr /*)*/ tmpChar) )+1 ;

	m_rdb_ret_info.ColumnLen[ 0 ] = tmpint ;

	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;

	m_rdb_ret_info.RcdLen = tmpint ;

	m_rdb_ret_info.pretinfo = ( char * )malloc( tmpint ) ;

	strcpy( m_rdb_ret_info.pretinfo , tmpChar ) ;

	return ;
}

void Rdbview_wnd::analyze_delete( void )
{
	char select_table_name[40] ;

	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	m_bGet_dict = false ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbClearSQLCmd() ;
	}
	else
	{
		m_pRsql_server->RdbClearSQLCmd() ;
	}

	int ret = m_gettoken.GetWord() ;

	if ( ret != RDB_LINE_NAMETYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	strcpy( select_table_name , m_gettoken.word ) ;

	ret = m_gettoken.GetWord() ;

	if ( ret == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}
	else if ( ret == RDB_LINE_NAMETYPE )
	{
		if ( strcmp( m_gettoken.word , "where" ) == 0 )
		{
			m_bGet_dict = false ;
			ret = analyze_where( select_table_name ) ;

			if ( ret != 0 ) //indicate that there is a error when analyzeing where substatement
				return ;
		}
		else
		{
			m_err_code = SYNTAXERR ;
			return ;
		}
	}
	else if ( ret != RDB_LINE_EOLTYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( m_rdb_type == LOCAL )
	{
		ret = m_pRsql_local->RdbDelCmd( m_rsql_username , m_rsql_password , select_table_name ) ;
	}
	else
	{
		ret = m_pRsql_server->RdbDelCmd( 1 , m_rsql_username , m_rsql_password , select_table_name ) ;
	}

	if ( ret != RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	//next will display succesful information to user
	m_rdb_ret_info.RcdNum = 1 ;

	m_rdb_ret_info.ColumnNum = 1 ;

	strcpy( m_rdb_ret_info.ColumnName[0] , "返回结果" ) ;

// 	QString tmpstr = "成功删除记录!" ;
	char tmpChar[10];
	strcpy(tmpChar,"成功删除记录!");

	int tmpint = max( strlen( m_rdb_ret_info.ColumnName[0] ) , strlen(/* _Q2C( tmpstr ) */tmpChar) )+1 ;

	m_rdb_ret_info.ColumnLen[0] = tmpint ;

	m_rdb_ret_info.ColumnOffset[0] = 0 ;

	m_rdb_ret_info.RcdLen = tmpint ;

	m_rdb_ret_info.pretinfo = ( char * )malloc( tmpint ) ;

	strcpy( m_rdb_ret_info.pretinfo , /*_Q2C( tmpstr )*/tmpChar ) ;

	return ;
}

void Rdbview_wnd::analyze_insert( void )
{
	int wordtype ;
	char attr_name[ 40 ] ;
	char select_table_name[ 40 ] ;
	RDB_BTBL_COL_STRU tmp_attr_info ;
	RDB_VALUETYPE_STRU vtval ;

	if ( m_rdb_ret_info.pretinfo )
	{
		free ( m_rdb_ret_info.pretinfo ) ;
		m_rdb_ret_info.pretinfo = NULL ;
	}

	m_bGet_dict = false ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbClearSQLCmd() ;
	}
	else
	{
		m_pRsql_server->RdbClearSQLCmd() ;
	}

	int ret = m_gettoken.GetWord() ;

	if ( ret != RDB_LINE_NAMETYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	strcpy( select_table_name , m_gettoken.word ) ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbInserTab( m_gettoken.word ) ;
	}
	else
	{
		m_pRsql_server->RdbInserTab( m_gettoken.word ) ;
	}

	if ( m_rdb_type == LOCAL )
	{
		m_pLocal_dict->RdbSelectFrom( select_table_name ) ;
		m_pLocal_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
		ret = m_pLocal_dict->RdbSelCmd( m_rsql_username , m_rsql_password ) ;
	}
	else
	{
		m_pSer_dict->RdbSelectFrom( select_table_name ) ;
		m_pSer_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
		ret = m_pSer_dict->RdbSelCmd( 1 , m_rsql_username , m_rsql_password ) ;
	}

	if ( ret != RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	ret = m_gettoken.GetWord() ;

	if ( ret != RDB_LINE_NAMETYPE )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	if ( strcmp( m_gettoken.word , "values" ) != 0 )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	ret = m_gettoken.GetWord() ;

	if ( strcmp( m_gettoken.word , "(" ) != 0 )
	{
		m_err_code = SYNTAXERR ;
		return ;
	}

	for ( ; ; )
	{
		ret = m_gettoken.GetWord() ;

		if ( ret != RDB_LINE_NAMETYPE )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		strcpy( attr_name , m_gettoken.word ) ;

		ret = m_gettoken.GetWord() ;

		if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		if ( strcmp( m_gettoken.word , "=" ) )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		wordtype = m_gettoken.GetWord() ;

		if ( wordtype == RDB_LINE_EOLTYPE || wordtype == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		if ( m_rdb_type == LOCAL )
		{
			ret = m_pLocal_dict->RdbGetColumnInfo( attr_name , &tmp_attr_info ) ;
		}
		else
		{
			ret = m_pSer_dict->RdbGetColumnInfo( attr_name , &tmp_attr_info ) ;
		}

		if ( ret != RDB_OK )
		{
			m_err_code = ret ;
			return ;
		}

		if ( tmp_attr_info.datatype <= RDB_DATATYPE_LONG && wordtype != RDB_LINE_IDIGITTYPE )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}
		else if ( tmp_attr_info.datatype >= RDB_DATATYPE_FLOAT && tmp_attr_info.datatype <= RDB_DATATYPE_DOUBLE
			&& ( wordtype != RDB_LINE_FDIGITTYPE && wordtype != RDB_LINE_IDIGITTYPE ) )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}

		Valtype_SwapinDecVal( &vtval , tmp_attr_info.datatype , m_gettoken.word ) ;

		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbSetValues( attr_name , &vtval ) ;
		}
		else
		{
			m_pRsql_server->RdbSetValues( attr_name , &vtval ) ;
		}

		ret = m_gettoken.GetWord() ;

		if ( strcmp( m_gettoken.word , ")" ) == 0 )break ;

		if ( strcmp( m_gettoken.word , "," ) )
		{
			m_err_code = SYNTAXERR ;
			return ;
		}
	}


	if ( m_rdb_type == LOCAL )
	{
		ret = m_pRsql_local->RdbInsCmd( m_rsql_username , m_rsql_password ) ;
	}
	else
	{
		ret = m_pRsql_server->RdbInsCmd( 1 , m_rsql_username , m_rsql_password ) ;
	}

	if ( ret != RDB_OK )
	{
		m_err_code = ret ;
		return ;
	}

	m_rdb_ret_info.RcdNum = 1 ;

	m_rdb_ret_info.ColumnNum = 1 ;

	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "返回结果" ) ;

	char tmpChar[10];
	strcpy(tmpChar,"插入记录成功!");

// 	QString tmpstr = "插入记录成功!" ;

	int tmpint = max( strlen( m_rdb_ret_info.ColumnName[ 0 ] ) , strlen( /*_Q2C( tmpstr )*/tmpChar ) )+1 ;

	m_rdb_ret_info.ColumnLen[ 0 ] = tmpint ;

	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;

	m_rdb_ret_info.RcdLen = tmpint ;

	m_rdb_ret_info.pretinfo = ( char * )malloc( tmpint ) ;

	strcpy( m_rdb_ret_info.pretinfo , /*_Q2C( tmpstr )*/tmpChar ) ;

	return ;
}

int Rdbview_wnd::analyze_where( char* select_table_name )
{
	RDB_COND_STRU condinfo ;
	RDB_BTBL_COL_STRU tmpattrinfo ;
	int count = 0 ;
	int ret ;

	if ( !m_bGet_dict )
	{
		if ( m_rdb_type == LOCAL )
		{
			m_pLocal_dict->RdbSelectFrom( select_table_name ) ;
			m_pLocal_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
			ret = m_pLocal_dict->RdbSelCmd( m_rsql_username , m_rsql_password ) ;
		}
		else
		{
			m_pSer_dict->RdbSelectFrom( select_table_name ) ;
			m_pSer_dict->RdbWhereCond( "" , RDB_DATATYPE_STRING , ( void* )"" ) ;
			ret = m_pSer_dict->RdbSelCmd( 1 , m_rsql_username , m_rsql_password ) ;
		}
	}
	else
	{
		ret = RDB_OK ;
	}

	if ( ret != RDB_OK )
	{
		m_err_code = ret ;
		return -1 ;
	}

	for ( ; ; )
	{
		if ( count != 0 )
		{
			ret = m_gettoken.GetWord( ) ;

			if ( ret == RDB_LINE_EOLTYPE )
			{
				return 0 ;
			}
			else if ( ret == RDB_LINE_ERR )
			{
				m_err_code = SYNTAXERR ;
				return -1 ;
			}
			else if ( strcmp( strlwr( m_gettoken.word ) , "and" ) == 0 ) condinfo.logicalop = LOGIC_OPER_AND ;
			else if ( strcmp( strlwr( m_gettoken.word ) , "or" ) == 0 ) condinfo.logicalop = LOGIC_OPER_OR ;
			else if ( strcmp( strlwr( m_gettoken.word ) , "order" ) == 0 )
			{
				ret = analyze_orderby() ;
				if ( ret == 0 ) return 0 ;
				else
				{
					m_err_code = ret ;
					return -1 ;
				}
			}
			else
			{
				m_err_code = ret ;
				return -1 ;
			}
		}

		ret = m_gettoken.GetWord( ) ;

		if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return( -1 ) ;
		} ;

		strcpy( condinfo.name , m_gettoken.word ) ;

		ret = m_gettoken.GetWord( ) ;

		if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return( -1 ) ;
		} ;

		if ( strcmp( m_gettoken.word , "=" ) == 0 )condinfo.relationop = RELATION_OPER_EQU ;
		else if ( strcmp( m_gettoken.word , "!=" ) == 0 ) condinfo.relationop = RELATION_OPER_NOTEQU ;
		else if ( strcmp( m_gettoken.word , ">" ) == 0 )condinfo.relationop = RELATION_OPER_MORETHAN ;
		else if ( strcmp( m_gettoken.word , ">=" ) == 0 )condinfo.relationop = RELATION_OPER_MOREOREQU ;
		else if ( strcmp( m_gettoken.word , "<" ) == 0 )condinfo.relationop = RELATION_OPER_LESSTHAN ;
		else if ( strcmp( m_gettoken.word , "<=" ) == 0 )condinfo.relationop = RELATION_OPER_LESSOREQU ;
		else
		{
			m_err_code = SYNTAXERR ;
			return( -1 ) ;
		} ;

		ret = m_gettoken.GetWord( ) ;

		if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return( -1 ) ;
		} ;


		if ( m_rdb_type == LOCAL )
		{
			if ( ( ret = m_pLocal_dict->RdbGetColumnInfo( condinfo.name , &tmpattrinfo ) ) != RDB_OK )
			{
				m_err_code = ret ;
				return( -1 ) ;
			} ;
		}
		else
		{
			if ( ( ret = m_pSer_dict->RdbGetColumnInfo( condinfo.name , &tmpattrinfo ) ) != RDB_OK )
			{
				m_err_code = ret ;
				return( -1 ) ;
			} ;
		}


		Valtype_SwapinDecVal( &condinfo.conditionval , tmpattrinfo.datatype , m_gettoken.word ) ;

		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbValTypeWhere( condinfo.name , &condinfo.conditionval , 
			condinfo.relationop , condinfo.logicalop ) ;
		}
		else
		{
			m_pRsql_server->RdbValTypeWhere( condinfo.name , &condinfo.conditionval , 
			condinfo.relationop , condinfo.logicalop ) ;
		}
		count++ ;
	}
}

int Rdbview_wnd::analyze_orderby( void )
{
	int ret = m_gettoken.GetWord() ;

	if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
		return -1 ;
	}

	if ( strcmp( strlwr( m_gettoken.word ) , "by" ) != 0 )
	{
		m_err_code = SYNTAXERR ;
		return -1 ;
	} ;

	ret = m_gettoken.GetWord() ;

	if ( ret == RDB_LINE_EOLTYPE || ret == RDB_LINE_ERR )
	{
		m_err_code = SYNTAXERR ;
		return -1 ;
	}

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbOrderBy( m_gettoken.word ) ;
	}
	else
	{
		m_pRsql_server->RdbOrderBy( m_gettoken.word ) ;
	}

	for ( ; ; )
	{
		ret = m_gettoken.GetWord() ;

		if ( ret == RDB_LINE_EOLTYPE )
		{
			return 0 ;
		}
		else if ( ret == RDB_LINE_ERR )
		{
			m_err_code = SYNTAXERR ;
			return -1 ;
		} ;

		if ( strcmp( strlwr( m_gettoken.word ) , " , " ) ) break ;

		ret = m_gettoken.GetWord() ;

		if ( ret != RDB_LINE_NAMETYPE )
		{
			m_err_code = SYNTAXERR ;
			return -1 ;
		} ;

		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbOrderBy( m_gettoken.word ) ;
		}
		else
		{
			m_pRsql_server->RdbOrderBy( m_gettoken.word ) ;
		}
	}

	if ( ( strcmp( strlwr( m_gettoken.word ) , "descend" ) == 0 ) || ( strcmp( strlwr( m_gettoken.word ) , "desc" ) == 0 ) )
	{
		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbSetOrdertype( RDB_SQL_DESCEND ) ;
		}
		else
		{
			m_pRsql_server->RdbSetOrdertype( RDB_SQL_DESCEND ) ;
		}
		return 0 ;
	}
	else if ( ( strcmp( strlwr( m_gettoken.word ) , "ascend" ) == 0 ) ||( strcmp( strlwr( m_gettoken.word ) , "asc" ) == 0 ) )
	{
		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbSetOrdertype( RDB_SQL_ASCEND ) ;
		}
		else
		{
			m_pRsql_server->RdbSetOrdertype( RDB_SQL_ASCEND ) ;
		}

		return 0 ;
	}
	else
	{
		m_err_code = SYNTAXERR ;
		return -1 ;
	}
}

int Rdbview_wnd::gen_selected_rcds( void )
{
	unsigned int rec_count , col_count , tmp_int ;
	QString tmp_str ;
	RDB_BTBL_TAB_STRU table_info ;
	RDB_BTBL_COL_STRU column_info ;
	Rdb_Vartype_value ret_var_value ;
	unsigned int rec_len = 0 ;

	if ( m_rdb_type == LOCAL )
	{
		rec_count = m_pRsql_local->rdbgetrcdcnt() ;
		m_pRsql_local->RdbGetTableInfo( NULL , &table_info ) ;
		col_count = table_info.columnnums ;
	}
	else
	{
		rec_count = m_pRsql_server->rdbgetrcdcnt() ;
		m_pRsql_server->RdbGetTableInfo( NULL , &table_info ) ;
		col_count = table_info.columnnums ;
	}

	if ( rec_count == 0 ) //there are zero record matched the condition
	{
		m_rdb_ret_info.RcdNum = 1 ;
		m_rdb_ret_info.ColumnNum = 1 ;
		strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "返回结果" ) ;
		tmp_str = "返回记录为0" ;
		tmp_int = max( strlen( m_rdb_ret_info.ColumnName[ 0 ] ) , strlen( _Q2C( tmp_str ) ) )+1 ;
		m_rdb_ret_info.ColumnLen[ 0 ] = tmp_int ;
		m_rdb_ret_info.RcdLen = tmp_int ;
		m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;
		m_rdb_ret_info.pretinfo = ( char * )malloc( tmp_int ) ;
		strcpy( m_rdb_ret_info.pretinfo , _Q2C( tmp_str ) ) ;
		return 0 ;
	}

	m_rdb_ret_info.ColumnNum = col_count + 1 ; //add one column to display record NO.

	m_rdb_ret_info.RcdNum = rec_count ;

	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "序号" ) ;
	char tmp[ 40 ] ;

	m_rdb_ret_info.ColumnLen[ 0 ] = strlen( "999999999" ) ;

	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;

	rec_len += m_rdb_ret_info.ColumnLen[ 0 ] ;

	for ( int i = 1 ; i < m_rdb_ret_info.ColumnNum ; i++ )
	{
		m_rdb_ret_info.ColumnOffset[ i ] = rec_len ;

		if ( m_rdb_type == LOCAL )
		{
			m_pRsql_local->RdbGetColumnInfo( i - 1 , &column_info ) ;
		}
		else
		{
			m_pRsql_server->RdbGetColumnInfo( i - 1 , &column_info ) ;
		}

		strcpy( tmp , column_info.full_name ) ;
		strcat( tmp , "(" ) ;
		strcat( tmp , column_info.name ) ;
		strcat( tmp , ")" ) ;
		strcpy( m_rdb_ret_info.ColumnName[ i ] , tmp ) ;

		if ( column_info.datatype == RDB_DATATYPE_STRING )
		{
			m_rdb_ret_info.ColumnLen[ i ] = column_info.unitlen + 1 ;
		}
		else
		{
			switch ( column_info.datatype )
			{
			case RDB_DATATYPE_UCHAR:
				m_rdb_ret_info.ColumnLen[ i ] = 4 ; //uchar maximun value is 256 , three
				break ; //digital character , then add a NULL charater
				//so the length is 4

			case RDB_DATATYPE_CHAR:
				m_rdb_ret_info.ColumnLen[ i ] = 5 ;
				break ;

			case RDB_DATATYPE_USHORT:
				m_rdb_ret_info.ColumnLen[ i ] = 6 ;
				break ;

			case RDB_DATATYPE_SHORT:
				m_rdb_ret_info.ColumnLen[ i ] = 7 ;
				break ;

			case RDB_DATATYPE_UINT:
				m_rdb_ret_info.ColumnLen[ i ] = 11 ;
				break ;

			case RDB_DATATYPE_INT:
				m_rdb_ret_info.ColumnLen[ i ] = 12 ;
				break ;

			case RDB_DATATYPE_ULONG:
				m_rdb_ret_info.ColumnLen[ i ] = 11 ;
				break ;

			case RDB_DATATYPE_LONG:
				m_rdb_ret_info.ColumnLen[ i ] = 12 ;
				break ;

			case RDB_DATATYPE_FLOAT:
				m_rdb_ret_info.ColumnLen[ i ] = 16 ; //10
				break ;

			case RDB_DATATYPE_DOUBLE:
				m_rdb_ret_info.ColumnLen[ i ] = 16 ;
				break ;

			default :
				m_rdb_ret_info.ColumnLen[ i ] = 10 ;
			}
		} //end of if
		rec_len += m_rdb_ret_info.ColumnLen[ i ] ;
	} //end of for

	m_rdb_ret_info.RcdLen = rec_len ;

	m_rdb_ret_info.pretinfo = ( char * ) malloc( m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		DMessageBox::information( this , tr( "警告" ) , tr( "分配内存错误" ) ) ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return -1 ;
	}

	m_rdb_ret_info.pretinfo = ( char * )memset( m_rdb_ret_info.pretinfo , '\0' , m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		DMessageBox::information( this , tr( "警告" ) , tr( "初始化内存错误" ) ) ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return -1 ;
	}

	for ( int i = 0 ; i < m_rdb_ret_info.RcdNum ; i++ )
	{
		sprintf( m_rdb_ret_info.pretinfo+( i*m_rdb_ret_info.RcdLen ) , "%u" , i ) ;

		for ( int j = 1 ; j < m_rdb_ret_info.ColumnNum ; j++ )
		{
			if ( m_rdb_type == LOCAL )
			{
				m_pRsql_local->RdbGetColumnInfo( j - 1 , &column_info ) ;
				m_pRsql_local->RdbGetColBaseValue( i , &column_info , &ret_var_value ) ;
			}
			else
			{
				m_pRsql_server->RdbGetColumnInfo( j - 1 , &column_info ) ;
				m_pRsql_server->RdbGetColBaseValue( i , &column_info , &ret_var_value ) ;
			}

			if ( column_info.datatype == RDB_DATATYPE_STRING )
			{
				strcpy( m_rdb_ret_info.pretinfo+i*m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ j ] , ret_var_value.RdbGetString() ) ;
			}
			else //if data is not string , then will convert each datatype to string
			{
				char tmpstr[ 30 ] ;
				VTVALUE_UNION ValueUnion ;
				ret_var_value.RdbSwapVal( &ValueUnion ) ;

				switch ( column_info.datatype )
				{
				case RDB_DATATYPE_UCHAR:
					sprintf( tmpstr , "%3u" , ValueUnion.ucharval ) ;
					break ;//2015

				case RDB_DATATYPE_USHORT:
					sprintf( tmpstr , "%3u" , ValueUnion.ushortval ) ;
					break ;//2015

				case RDB_DATATYPE_UINT:
					sprintf( tmpstr , "%3u" , ValueUnion.uintval ) ;
					break ;

				case RDB_DATATYPE_CHAR:
					sprintf( tmpstr , "%4d" , ValueUnion.charval ) ;
					break ;//2003

				case RDB_DATATYPE_SHORT:
					sprintf( tmpstr , "%4d" , ValueUnion.shortval ) ;
					break ;//2015

				case RDB_DATATYPE_INT:
					sprintf( tmpstr , "%4d" , ValueUnion.intval ) ;
					break ;

				case RDB_DATATYPE_LONG:
					sprintf( tmpstr , "%5d" , ValueUnion.longval ) ;
					break ;

				case RDB_DATATYPE_ULONG:
					sprintf( tmpstr , "%4u" , ValueUnion.ulongval ) ;
					break ;

				case RDB_DATATYPE_FLOAT:
					sprintf( tmpstr , "%8.2f" , ValueUnion.floatval ) ;//19990624
					break ;

				case RDB_DATATYPE_DOUBLE:
					sprintf( tmpstr , "%8.3f" , ValueUnion.doubleval ) ;
					break ;

				default:
					tmpstr[0] = '\0' ;
				} //end of switch
				strcpy( m_rdb_ret_info.pretinfo + ( i * m_rdb_ret_info.RcdLen ) + m_rdb_ret_info.ColumnOffset[ j ] , tmpstr ) ;
			}//end of if
		}//end of for( j )
	}//end of for( i )
	return 0 ;
}

void Rdbview_wnd::gen_table_info( void )
{
	Rdb_LocateTable *pRdb_local = NULL ;
	Rdb_DictManager *pRdb_dict = NULL ;
	RDB_TAB_STRU * pRdb_tablep = NULL ;

	if ( m_rdb_type == LOCAL )
	{
		pRdb_dict = ( Rdb_DictManager * ) new Rdb_DictManager ;
		m_rdb_ret_info.RcdNum = pRdb_dict->RdbGetTabNums( ) ;
	}
	else
	{
		pRdb_local = ( Rdb_LocateTable * ) new Rdb_LocateTable ;
		m_rdb_ret_info.RcdNum = pRdb_local->RdbGetTabNums( ) ;
	}

	char table_name[ 40 ] , table_des[ 40 ] , server_name[ 40 ] ;

	m_rdb_ret_info.ColumnNum = 4 ;
	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "序号" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 1 ] , "表名" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 2 ] , "描述" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 3 ] , "服务器名" ) ;

	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;
	m_rdb_ret_info.ColumnLen[ 0 ] = 8 ;
	m_rdb_ret_info.ColumnOffset[ 1 ] = 9 ;
	m_rdb_ret_info.ColumnLen[ 1 ] = 20 ;
	m_rdb_ret_info.ColumnOffset[ 2 ] = 29 ;
	m_rdb_ret_info.ColumnLen[ 2 ] = 40 ;
	m_rdb_ret_info.ColumnOffset[ 3 ] = 69 ;
	m_rdb_ret_info.ColumnLen[ 3 ] = 40 ;

	m_rdb_ret_info.RcdLen = 9 + 20 + 40 + 20 ;

	m_rdb_ret_info.pretinfo = ( char * ) malloc( m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		m_err_code = ALLOCMEMERR ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return ;
	}

	m_rdb_ret_info.pretinfo = ( char * )memset( m_rdb_ret_info.pretinfo , '\0' , m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		m_err_code = INITMEMERR ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return ;
	}

	for ( int i = 0 ; i < m_rdb_ret_info.RcdNum ; i++ )
	{
		if ( m_rdb_type == LOCAL )
		{
			pRdb_tablep = pRdb_dict->GetTableStruct( i ) ;
			strcpy( table_name , pRdb_tablep->name ) ;
			strcpy( table_des , pRdb_tablep->full_name ) ;
			strcpy( server_name , pRdb_tablep->server_name ) ;
		}
		else
		{
			pRdb_local->RdbGetTableInfo( i , table_name , table_des , server_name ) ;
		}

		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 0 ] , "%5d" , i ) ;
		strcpy( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 1 ] , table_name ) ;
		strcpy( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 2 ] , table_des ) ;
		strcpy( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 3 ] , server_name ) ;
	}

	if ( pRdb_dict != NULL )
	{
		delete pRdb_dict ;
		pRdb_dict = NULL ;
	}

	if ( pRdb_local != NULL )
	{
		delete pRdb_local ;
		pRdb_local = NULL ;
	}
}

void Rdbview_wnd::gen_table_struct( void )
{
	RDB_BTBL_TAB_STRU tableinfo ;
	RDB_BTBL_COL_STRU columninfo ;
	int ret ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local->RdbGetTableInfo( NULL , &tableinfo ) ;
	}
	else
	{
		m_pRsql_server->RdbGetTableInfo( NULL , &tableinfo ) ;
	}

	m_rdb_ret_info.RcdNum = tableinfo.columnnums ;
	m_rdb_ret_info.ColumnNum = 8 ;
	strcpy( m_rdb_ret_info.ColumnName[ 0 ] , "序号" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 1 ] , "列名" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 2 ] , "描述" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 3 ] , "类型" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 4 ] , "长度" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 5 ] , "偏移" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 6 ] , "位置" ) ;
	strcpy( m_rdb_ret_info.ColumnName[ 7 ] , "位数" ) ;
	m_rdb_ret_info.ColumnOffset[ 0 ] = 0 ;
	m_rdb_ret_info.ColumnLen[ 0 ] = strlen( "9999999" ) ;
	m_rdb_ret_info.ColumnOffset[ 1 ] = 9 ;
	m_rdb_ret_info.ColumnLen[ 1 ] = 15 ;
	m_rdb_ret_info.ColumnOffset[ 2 ] = 25 ;
	m_rdb_ret_info.ColumnLen[ 2 ] = 20 ;
	m_rdb_ret_info.ColumnOffset[ 3 ] = 45 ;
	m_rdb_ret_info.ColumnLen[ 3 ] = 10 ;
	m_rdb_ret_info.ColumnOffset[ 4 ] = 55 ;
	m_rdb_ret_info.ColumnLen[ 4 ] = 10 ;
	m_rdb_ret_info.ColumnOffset[ 5 ] = 60 ;
	m_rdb_ret_info.ColumnLen[ 5 ] = 10 ;
	m_rdb_ret_info.ColumnOffset[ 6 ] = 65 ;
	m_rdb_ret_info.ColumnLen[ 6 ] = 10 ;
	m_rdb_ret_info.ColumnOffset[ 7 ] = 70 ;
	m_rdb_ret_info.ColumnLen[ 7 ] = 10 ;
	m_rdb_ret_info.RcdLen = 9 + 16 + 20 + 10 + 5 + 5 + 5 + 5 ;

	m_rdb_ret_info.pretinfo = ( char * ) malloc( m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		m_err_code = ALLOCMEMERR ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return ;
	}

	m_rdb_ret_info.pretinfo = ( char * )memset( m_rdb_ret_info.pretinfo , '\0' , m_rdb_ret_info.RcdLen*m_rdb_ret_info.RcdNum ) ;

	if ( m_rdb_ret_info.pretinfo == NULL )
	{
		m_err_code = INITMEMERR ;
		m_rdb_ret_info.RcdNum = 0 ;
		m_rdb_ret_info.ColumnNum = 0 ;
		m_rdb_ret_info.RcdLen = 0 ;
		return ;
	}

	for ( int i = 0 ; i < m_rdb_ret_info.RcdNum ; i++ )
	{
		if ( m_rdb_type == LOCAL )
		{
			ret = m_pRsql_local->RdbGetColumnInfo( i , &columninfo ) ;
		}
		else
		{
			ret = m_pRsql_server->RdbGetColumnInfo( i , &columninfo ) ;
		}

		if ( ret != RDB_OK )
		{
			m_err_code = ret ;
			return ;
		}

		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 0 ], "%5d" , i ) ;
		strcpy( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 1 ] , columninfo.name ) ;
		strcpy( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 2 ] , columninfo.full_name ) ;
		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 4 ] , "%4d" , columninfo.unitlen ) ;
		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 5 ] , "%4d" , columninfo.offset ) ;
		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 6 ] , "%4d" , columninfo.position ) ;
		sprintf( m_rdb_ret_info.pretinfo + i * m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 7 ] , "%4d" , columninfo.bitnums ) ;

		if ( columninfo.datatype <= RDB_DATATYPE_STRING )
		{
			strcpy( m_rdb_ret_info.pretinfo+i*m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 3 ] , datatype_text[ columninfo.datatype ] ) ;
		}
		else
		{
			strcpy( m_rdb_ret_info.pretinfo+i*m_rdb_ret_info.RcdLen+m_rdb_ret_info.ColumnOffset[ 3 ] , "错误类型" ) ;
		}
	} //end of for
}

void Rdbview_wnd::slot_file_out( void )
{
	if( m_pNormal_table->rowCount() == 0 )
	{
		return ;
	}

	QString file_name_str = QFileDialog::getSaveFileName( this , tr( "导出文件" ) , getenv( "NBENV" ) , "*.csv"  );

	if( file_name_str.isEmpty() )
	{
		return ;
	}

	QFile file( file_name_str ) ;

	if( !file.open( QIODevice::WriteOnly ) )
	{
		return ;
	}

//	QTextStream stream( &file ) ;

	for( int i = 0 ; i < m_pNormal_table->rowCount() ; i ++ )
	{
// 		stream << i + 1;
// 		stream << "\t";
		for( int j = 0 ; j < m_pNormal_table->columnCount() ; j ++ )
		{
// 			stream << m_pNormal_table->item( i , j )->text() ;
// 			stream << "\t";
			file.write( m_pNormal_table->item( i , j )->text().toLocal8Bit() ) ;
			file.write( "," ) ;
		}
// 		stream << "\n";
		file.write("\n");
	}
	file.close();
}

void Rdbview_wnd::slot_scada_set( void )
{
	Rdb_scada_dialog dlg( this ) ;
	if( dlg.exec() != QDialog::Accepted )
	{
		return;
	}
}

void Rdbview_wnd::slot_ycyx_set( void )
{
	Rdb_ycyx_dialog dlg( this ) ;
	if( dlg.exec() != QDialog::Accepted )
	{
		return;
	}
}

void Rdbview_wnd::slot_select_change( QTreeWidgetItem* pItem )
{
	if ( pItem->data( 0 , Qt::UserRole + 1 ).toInt() == 1 || pItem->data( 0 , Qt::UserRole + 1 ).toInt() == 2 )
	{
		return ;
	}
	QString tablename_str , rtudesc_str ;
	int rtu_no = 0 ;
	int nMulCount=0;
	bFlag = true ;
	int depth = 0;
	QTreeWidgetItem *pTemp_item = pItem ;
	if( !pTemp_item ) 
	{
		return ;
	}
	while ( pTemp_item )
	{
		depth++;
		pTemp_item = pTemp_item->parent();
	}
	depth = depth - 1;

	QString str = pItem->text( 0 ) ;
	QTreeWidgetItem* pParent_item = pItem->parent() ;
	while ( pParent_item )
	{
		str = pParent_item->text( 0 ) + tr( "→" ) + str ;
		pParent_item = pParent_item->parent() ;
	}
	QString describe = tr( "数据选取信息:" ) + str ;
	m_pSelect_label->setText( describe ) ;
	int user_flag = pItem->data( 0 , Qt::UserRole + 1 ).toInt() ;

	if( pItem->childCount() == 0 )	
	{
		if ( user_flag <= 0 )
		{
			if ( depth == 2 )
			{
				Info.RealFlag = SCADA_REAL ;
				tablename_str = pItem->parent()->text( 0 ) ;
				rtudesc_str = pItem->text( 0 ) ;
				rtu_no = get_terminal_no( rtudesc_str ) ;
			} 
			else
			{
				Info.RealFlag = SCADA_REAL ;
				tablename_str = pItem->text( 0 ) ;
				rtu_no = -1 ;
			}
		}
		else
		{
			pParent_item = pItem ;
			QTreeWidgetItem* pPapa_item = pItem->parent() ;
			while ( pPapa_item )
			{
				pParent_item = pPapa_item ;
				pPapa_item = pPapa_item->parent() ;		 
			}
			if ( pParent_item->text( 0 ) == tr( "实时电度数据" ) )
			{
				Info.RealFlag = KWH ;
				Info.TerminalNo = get_terminal_no( pItem->text( 0 ) ) ;
				tablename_str = tr( "kwh" ) ;
				rtu_no = get_terminal_no( pItem->text( 0 ) ) ;
			}
			if ( pParent_item->text( 0 ) == tr( "实时遥测数据" ) )
			{
				Info.RealFlag = YC ;
				Info.TerminalNo = get_terminal_no( pItem->text( 0 ) ) ;
				tablename_str = tr( "analog" ) ;
				rtu_no = get_terminal_no( pItem->text( 0 ) ) ;
			}
			if ( pParent_item->text( 0 ) == tr( "实时遥信数据" ) )
			{
				Info.RealFlag = YX ;
				Info.TerminalNo = get_terminal_no( pItem->text( 0 ) ) ;
				rtu_no = get_terminal_no( pItem->text( 0 ) ) ;
			}
		}
		if( call_real_data( tablename_str , rtu_no ) ) 
		{
			fill_data( tablename_str , rtu_no , bFlag ) ;
			Info.DataOK = true ;
		}
		else
		{
			fill_empty_data( tablename_str , rtu_no ) ;
			m_pSelect_label->setText( tr( "数据读取信息:无有效数据" ) ) ;
			Info.DataOK = false ;
		}
	}
}

void Rdbview_wnd::slot_sql_connect( void )
{
	m_bIs_connected = true ;
	m_pRdb_select_combobox->setEnabled( false ) ;
	m_pIsSql_checkbox->setEnabled( false ) ;

	strcpy( m_rsql_username , "scada" ) ;
	strcpy( m_rsql_password , "scada" ) ;

	if ( m_rdb_type == LOCAL )
	{
		m_pRsql_local = new Rdb_SQL ;
		m_pLocal_dict = new Rdb_SQL ;
	}
	else
	{
		m_pRsql_server = new Rdb_SqlCli ;
		m_pSer_dict = new Rdb_SqlCli ;
	}

	if ( m_bIs_connected )
	{
		m_pRdb_connect_button->setEnabled( false ) ;
		m_pRdb_disconnect_button->setEnabled( true ) ;
		m_pRdb_sqlrun_button->setEnabled( false ) ;
	}	
	if ( m_rdb_type == LOCAL )
	{
		m_pRead_info_label->setText( tr( "已连接到本地实时库" ) ) ;
	}
	else
	{
		m_pRead_info_label->setText( tr( "已连接到服务器实时库" ) ) ;
	}
}

void Rdbview_wnd::slot_sql_disconnect( void )
{
	if( DMessageBox::question( this , tr( "警告" ) , tr( "确定断开连接?") ,QMessageBox::Yes|QMessageBox::No , QMessageBox::No ) != QMessageBox::Yes )
	{
		return ;
	}

	m_pRdb_select_combobox->setEnabled( true ) ;
	m_pIsSql_checkbox->setEnabled( true ) ;

	m_rsql_username[ 0 ] = '\0' ;
	m_rsql_password[ 0 ] = '\0' ;
	m_bIs_connected = false ;

	if ( m_bIs_connected == false )
	{
		m_pRdb_connect_button->setEnabled( true ) ;
		m_pRdb_disconnect_button->setEnabled( false ) ;
	}
	if ( m_pRsql_server != NULL )
	{
		delete m_pRsql_server ;
		m_pRsql_server = NULL ;
	}
	if ( m_pRsql_local != NULL )
	{
		delete m_pRsql_local ;
		m_pRsql_local = NULL ;
	}
	m_pRead_info_label->setText( tr( "未与实时库建立连接" ) ) ;
}

void Rdbview_wnd::slot_sql_run( void )
{
	QString rsql_stat_mnt ;
	if ( m_bIs_connected == false )
	{
		DMessageBox::information( this , tr( "警告" ) , tr( "实时库未连接" ) ) ;
		return ;
	}

	if ( m_pSql_textedit->textCursor().hasSelection() )
	{
		rsql_stat_mnt  =  m_pSql_textedit->textCursor().selectedText() ;
	}
	else
	{
		rsql_stat_mnt  =  m_pSql_textedit->toPlainText() ;
	}

	m_pSql_statement = (char *)malloc( 2 * rsql_stat_mnt.length() + 1 ) ;

	strcpy( m_pSql_statement , _Q2C( rsql_stat_mnt ) ) ;

	m_gettoken.InLine( m_pSql_statement ) ;

	m_err_code = OPERATION_OK ;

	rsql_handling() ;

	update_table() ;

	if ( m_pSql_statement != NULL ) 
	{
		free( ( void * )m_pSql_statement ) ;
		m_pSql_statement = NULL ;
	}
}

void Rdbview_wnd::slot_rdb_select_change( void )
{
	if ( m_pRdb_select_combobox->currentIndex() == 0 )
	{
		m_rdb_type = SERVER ;
	}
	else
	{
		m_rdb_type = LOCAL ;
	}
}

void Rdbview_wnd::slot_sql_change( void )
{
	if ( m_pSql_textedit->toPlainText().isEmpty() )
	{
		m_pRdb_sqlrun_button->setEnabled( false) ;
	}
	else
	{
		m_pRdb_sqlrun_button->setEnabled( true ) ;
	}
}

void Rdbview_wnd::slot_read_data_info( void )
{
	Rdb_SqlCli rl;

	//读取遥测个数	
	rl.RdbSelectFrom( "analog" ) ;
	rl.RdbSelectField( "name" ) ;

	int ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_yc_num = rl.rdbgetrcdcnt() ;

	//读取遥信个数
	rl.RdbSelectFrom( "break" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}

	m_yx_num = rl.rdbgetrcdcnt() ;

	rl.RdbSelectFrom( "prot" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_yx_num += rl.rdbgetrcdcnt() ;

	rl.RdbSelectFrom( "switch" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_yx_num += rl.rdbgetrcdcnt() ;

	rl.RdbSelectFrom( "status" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_yx_num += rl.rdbgetrcdcnt() ;


	//读取遥控个数	
	rl.RdbSelectFrom( "ykpara" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_yk_num = rl.rdbgetrcdcnt() ;

	//读取计算量个数
	rl.RdbSelectFrom( "calculate" ) ;
	rl.RdbSelectField( "name" ) ;
	ret = rl.RdbSelCmd( cur_computer_group_no , "scada" , "scada" ) ;
	if( ret != RDB_OK )	
	{
		return ;
	}
	m_cal_num = rl.rdbgetrcdcnt() ;

	char text_char[ 200 ] ;
	sprintf( text_char , "遥测:%d个,遥信:%d个,遥控:%d个,计算量:%d个" , m_yc_num , m_yx_num , m_yk_num , m_cal_num ) ;
	m_pStatus_label->setText( tr( text_char ) );

	return ;
}
