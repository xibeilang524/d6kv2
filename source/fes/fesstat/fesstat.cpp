#include "fesstat.h"
#include "fesstat_string.h"
#include "fes/platformwrapper.h"
#include "uitools/dmessagebox.h"

#include <iostream>
#include <stdio.h>
#include <QtGui/QMessageBox>
using namespace std;

QFesStat::QFesStat( QWidget* parent /* = NULL */ ) :QDialog(  parent  ) , m_fes_shm(  Fes_shm::get() )
{
	m_pChanStatTable = new QTableWidget( this );
	m_pChanStatList = new QListWidget( this );
	m_pChanStatList->setViewMode( QListView::IconMode );
	m_pChanStatList->setVisible( false );
	m_pChanStatList->setMovement( QListView::Static );
	m_pTableButton = new QPushButton( QIcon( ":/res/table.png" ), _C2Q( "表格方式" ), this );
	m_pListButton = new QPushButton( QIcon( ":/res/list.png" ), _C2Q( "列表方式" ), this );
	m_pExitButton = new QPushButton( QIcon( ":/res/application-exit.png" ), _C2Q( "退出" ), this );
	m_pVLayout = new QVBoxLayout( NULL );
	m_pHLayout = new QHBoxLayout( NULL );

	m_pHLayout->addWidget( m_pTableButton );
	m_pHLayout->addWidget( m_pListButton );
	m_pHLayout->addWidget( m_pExitButton );

	m_pVLayout->addWidget( m_pChanStatTable );
	m_pVLayout->addWidget( m_pChanStatList );
	m_pVLayout->addLayout( m_pHLayout );

	this->setLayout( m_pVLayout );
	this->setGeometry( 200, 100, 1050, 500 );
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	this->setWindowTitle( _C2Q( "通道状态查看" ) );	

	readProtocol(  );
	initStatusList(  );
	initBKList(  );
	initHostList(  );
	setChanStat(  );

	m_pTimer = new QTimer( this );
 	connect( m_pTimer, SIGNAL( timeout(  ) ), this, SLOT( onTimer(  ) ) );
	m_pTimer->start( REFLESHTIME );
	connect( m_pTableButton, SIGNAL( clicked(  ) ), this, SLOT( onTableMode(  ) ) );
	connect( m_pListButton, SIGNAL( clicked(  ) ), this, SLOT( onListMode(  ) ) );
	connect( m_pExitButton, SIGNAL( clicked(  ) ), this, SLOT( onExit(  ) ) );

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
						   min-height: 40px;\
						   max-height: 40px;\
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
						   QFrame\
						   {\
						   background: #FAFAFA;\
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
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}

QFesStat::~QFesStat(  )
{

}


bool QFesStat::readProtocol( void )
{
	struct PROTOCOL_
	{
		char protocol_name[CODE_LENTH]; //规约名
		char describe[LONG_DESC_LENTH]; //规约描述
	};

	PROTOCOL_* db_protocol = NULL;
	Db_connector::open();
	int ret = Db_connector::select("select 规约名, 规约描述 from 规约参数表", (void **)&db_protocol);
	int rcd_cnt = ret; //规约表元素数量

	//读库正常
	if (ret > 0 && rcd_cnt > 0)
	{
		//根据数据库中读出的规约参数，创建规约工厂
		for (int i = 0; i < rcd_cnt; i++)
		{
			//创建规约实例
			PROTOCOL_* protocol_in_db = db_protocol + i;
			m_protocol[ _C2Q( protocol_in_db->protocol_name ) ]  = _C2Q( protocol_in_db->describe ) ;
		}	
	}//if (ret>0)

	else if(ret == 0)
	{
		DMessageBox::warning( this , _C2Q( "警告" ) , _C2Q("数据库中无规约参数记录！" ) ) ;
		return FALSE ;
	}
	else
	{
		DMessageBox::warning( this , _C2Q( "警告" ) , _C2Q("读数据库失败！" ) ) ;
		return FALSE ;
	}

	if (db_protocol)
	{
		free(db_protocol);
	}

	Db_connector::close() ;

	return TRUE ;
}

void QFesStat::initStatusList( void )
{
	m_statusList.push_back( _C2Q( "未检测" ) );
	m_statusList.push_back( _C2Q( "搜索" ) );
	m_statusList.push_back( _C2Q( "运行" ) );
	m_statusList.push_back( _C2Q( "停止" ) );
	m_statusList.push_back( _C2Q( "误码率高" ) );
	m_statusList.push_back( _C2Q( "失步" ) );
}

void QFesStat::initBKList( void )
{
	m_bkList.push_back( _C2Q( "备运行" ) );
	m_bkList.push_back( _C2Q( "主运行" ) );
}

void QFesStat::initHostList( void )
{
// 	m_hostList.push_back( _C2Q( "非值班机" ) );
// 	m_hostList.push_back( _C2Q( "值班机" ) );
}


void QFesStat::setChanStat( void )
{
	setChanTable(  );
	setChanList(  );
}

void QFesStat::setChanTable(  )
{
	int i;
	m_pChanStatTable->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pChanStatTable->setColumnCount( 9 );
	m_pChanStatTable->setRowCount( calcRealChanNum(  ) );
	m_pChanStatTable->setHorizontalHeaderLabels( QStringList(  )<<_C2Q( "通道号" )
															 <<_C2Q( "通道名" )
															 <<_C2Q( "规约类型" )
															 <<_C2Q( "通道状态" )
															 <<_C2Q( "通道地址" )
															 <<_C2Q( "误码率" )
															 <<_C2Q( "停运时刻" )
															 <<_C2Q( "运行时刻" )
															 <<_C2Q( "当前值班机" )
												 );
	m_pChanStatTable->setColumnWidth( 0, 55 );
	m_pChanStatTable->setColumnWidth( 1, 180 );
	m_pChanStatTable->setColumnWidth( 2, 130 );
	m_pChanStatTable->setColumnWidth( 3, 70 );
	m_pChanStatTable->setColumnWidth( 4, 140 );
	m_pChanStatTable->setColumnWidth( 5, 60 );
	m_pChanStatTable->setColumnWidth( 6, 130 );
	m_pChanStatTable->setColumnWidth( 7, 130 );

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;
	CHANDATA* pChanData = m_fes_shm.chan_data_ptr ;
	int order = 0;
	for ( i = 0 ; i < MAX_CHAN_NUM ; i++ )
	{
		if ( !strcmp( pChanPara->desdribe, "" ) || pChanData->curr_attend_host == -1  )
		{
			pChanPara++;
			pChanData++;
			continue;
		}

		m_pChanStatTable->setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
		m_pChanStatTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
		m_pChanStatTable->setItem( order, 2, new QTableWidgetItem( m_protocol.value( _C2Q( pChanPara->protocol ) ) ) );
		m_pChanStatTable->setItem( order, 3, new QTableWidgetItem( m_statusList[pChanData->stat] ) );
		m_pChanStatTable->setItem( order, 4, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) ) );
		m_pChanStatTable->setItem( order, 5, new QTableWidgetItem( QString::number( pChanData->bit_err_rate ) ) );

		struct tm* p_tm = localtime( &pChanData->last_down_time );
		char tmp_time[ 1024 ] ;
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		m_pChanStatTable->setItem( order, 6, new QTableWidgetItem( _C2Q( tmp_time ) ) );

		p_tm = localtime( &pChanData->last_up_time );
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		m_pChanStatTable->setItem( order, 7, new QTableWidgetItem( _C2Q( tmp_time ) ) );

// 		if ( m_hostList[ pChanData->curr_attend_host ] < 0 )
// 		{
// 			m_pChanStatTable->setItem( order, 9, new QTableWidgetItem( m_hostList[ 0 ] ) );
// 		}
// 		else
// 		{
			char* host_name = Net_stat::get_node_name(pChanData->curr_attend_host);
			m_pChanStatTable->setItem( order, 8, new QTableWidgetItem( _C2Q( host_name ) ) );
// 		}

		switch ( pChanData->stat )
		{
		case CHANUNCHECK:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/black.png" ), _C2Q( "" ) ) );
			break;
		case CHANSEARCH:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( "" ) ) );
			break;
		case CHANRUN:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/green.png" ), _C2Q( "" ) ) );
			break;
		case CHANSTOP:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/red.png" ), _C2Q( "" ) ) );
			break;
		case CHANRATE:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( "" ) ) );
			break;
		case CHANUNSYN:
			m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( "" ) ) );
			break;
		default:
			break;
		}//switch ( pChanStatus->status )

		order++;
		pChanPara++;
		pChanData++;
	}//for ( i = 0; i < m_compara.chanpara->chanNum; i++ )
}


void QFesStat::setChanList( void )
{
	int i;

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;
	CHANDATA* pChanData = m_fes_shm.chan_data_ptr ;

	for ( i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if ( !strcmp( pChanPara->desdribe, "" ) )
		{
			pChanPara++;
			pChanData++;
			continue;
		}

		QListWidgetItem* tmp_listItem = NULL ;
		switch ( pChanData->stat )
		{
		case CHANUNCHECK:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/black.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		case CHANSEARCH:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		case CHANRUN:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/green.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		case CHANSTOP:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/red.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		case CHANRATE:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		case CHANUNSYN:
			tmp_listItem = new QListWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( pChanPara->desdribe ) ) ;
			m_pChanStatList->addItem( tmp_listItem );
			break;
		default:
			break;
		}//switch (  pChanData->stat )
		tmp_listItem->setSizeHint(QSize( 80 , 80 ) ) ;
		pChanPara++ ;
		pChanData++ ;
	}//for ( i = 0; i < m_compara.chanpara->chanNum; i++ )
}



void QFesStat::refleshChanStat( void )
{
	refleshChanTable(  );
	refleshChanList(  );
}


void QFesStat::refleshChanTable( void )
{
	int lastChanNum = m_pChanStatTable->rowCount(  );
	int curChanNum = calcRealChanNum(  );

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;
	CHANDATA* pChanData = m_fes_shm.chan_data_ptr;

	int order = 0;
	int i;
	for ( i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if ( !strcmp( pChanPara->desdribe, "" ) || pChanData->curr_attend_host == -1)
		{
			pChanPara++;
			pChanData++;
			continue;
		}

		if ( order >= lastChanNum )
		{
			break;
		}

		m_pChanStatTable->item( order, 0 )->setText( QString::number( pChanPara->order ) );
		m_pChanStatTable->item( order, 1 )->setText( _C2Q( pChanPara->desdribe ) );
		m_pChanStatTable->item( order, 2 )->setText( m_protocol.value( _C2Q( pChanPara->protocol ) ) );
		m_pChanStatTable->item( order, 3 )->setText( m_statusList[pChanData->stat] );
		m_pChanStatTable->item( order, 4 )->setText( _C2Q( pChanPara->dev_name ) );
		m_pChanStatTable->item( order, 5 )->setText( QString::number( pChanData->bit_err_rate ) );

		struct tm* p_tm = localtime( &pChanData->last_down_time );
		char tmp_time[ 1024 ] ;
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		m_pChanStatTable->item( order, 6 )->setText( _C2Q( tmp_time ) );

		p_tm = localtime( &pChanData->last_up_time );
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		m_pChanStatTable->item( order, 7 )->setText( _C2Q( tmp_time ) );

// 		if ( m_hostList[ pChanData->curr_attend_host ] < 0 )
// 		{
// 			m_pChanStatTable->item( order, 9 )->setText( m_hostList[ 0 ] );
// 		}
// 		else
// 		{
			char* host_name = Net_stat::get_node_name(pChanData->curr_attend_host);

			m_pChanStatTable->item( order, 8 )->setText( _C2Q( host_name ));
// 		}
		
		switch ( pChanData->stat )
		{
		case CHANUNCHECK:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/black.png" ) );
			break;
		case CHANSEARCH:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/blue.png" ) );
			break;
		case CHANRUN:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/green.png" ) );
			break;
		case CHANSTOP:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/red.png" ) );
			break;
		case CHANRATE:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/yellow.png" ) );
			break;
		case CHANUNSYN:
			m_pChanStatTable->verticalHeaderItem( order )->setIcon( QIcon( ":/res/puple.png" ) );
			break;
		default:
			break;	
		}//switch ( pChanStatus->status )
		order++;
		pChanPara++;
		pChanData++;
	}//for ( i = 0; i < m_compara.chanpara->chanNum; i++ )

	//通道变少了
	if ( curChanNum < lastChanNum )
	{
		for ( int j = curChanNum; j < lastChanNum; j++ )
		{
			m_pChanStatTable->removeRow( j );
			m_pChanStatTable->takeVerticalHeaderItem( j );
		}

		m_pChanStatTable->setRowCount( curChanNum );
	}//if ( curChanNum < lastChanNum )

	else if ( curChanNum > lastChanNum ) //通道变多了
	{
		m_pChanStatTable->setRowCount( curChanNum );

		for ( int k = i; k < MAX_CHAN_NUM; k++ )
		{
			if ( !strcmp( pChanPara->desdribe, "" ) )
			{
				pChanPara++;
				pChanData++;
				continue;
			}
			
			m_pChanStatTable->setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
			m_pChanStatTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
			m_pChanStatTable->setItem( order, 2, new QTableWidgetItem( m_protocol.value( _C2Q( pChanPara->protocol ) ) ) );
			m_pChanStatTable->setItem( order, 3, new QTableWidgetItem( m_statusList[pChanData->stat] ) );
			m_pChanStatTable->setItem( order, 4, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) ) );
			m_pChanStatTable->setItem( order, 5, new QTableWidgetItem( QString::number( pChanData->bit_err_rate ) ) );

			struct tm* p_tm = localtime( &pChanData->last_down_time );
			char tmp_time[ 1024 ] ;
			sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
				p_tm->tm_min, p_tm->tm_sec );
			m_pChanStatTable->setItem( order, 6, new QTableWidgetItem( _C2Q( tmp_time ) ) );

			p_tm = localtime( &pChanData->last_up_time );
			sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d\n",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
				p_tm->tm_min, p_tm->tm_sec );
			m_pChanStatTable->setItem( order, 7, new QTableWidgetItem( _C2Q( tmp_time ) ) );

			if ( m_hostList[ pChanData->curr_attend_host ] < 0 )
			{
				m_pChanStatTable->setItem( order, 8, new QTableWidgetItem( m_hostList[ 0 ] ) );
			}
			else
			{
				m_pChanStatTable->setItem( order, 8, new QTableWidgetItem( m_hostList[ pChanData->curr_attend_host ] ) );
			}
			
			switch ( pChanData->stat )
			{
			case CHANUNCHECK:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/black.png" ), _C2Q( "" ) ) );
				break;
			case CHANSEARCH:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( "" ) ) );
				break;
			case CHANRUN:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/green.png" ), _C2Q( "" ) ) );
				break;
			case CHANSTOP:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/red.png" ), _C2Q( "" ) ) );
				break;
			case CHANRATE:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( "" ) ) );
				break;
			case CHANUNSYN:
				m_pChanStatTable->setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( "" ) ) );
				break;
			default:
				break;
			}//switch ( pChanData->stat )

			order++;
			pChanPara++;
			pChanData++;

		}//for ( int k = i; k < MAX_CHAN_NUM; k++  )
	}
}


void QFesStat::refleshChanList( void )
{
	int lastChanNum = m_pChanStatList->count(  );
	int curChanNum = calcRealChanNum(  );
	int i;
	int order = 0;

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;
	CHANDATA* pChanData = m_fes_shm.chan_data_ptr;

	for ( i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if ( !strcmp( pChanPara->desdribe , "" ) )
		{
			continue;
		}

		if ( order >= lastChanNum )
		{
			break;
		}

		m_pChanStatList->item( order )->setText( _C2Q( pChanPara->desdribe ) );
		
		switch ( pChanData->stat )
		{
		case CHANUNCHECK:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/black.png" ) );
			break;
		case CHANSEARCH:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/blue.png" ) );
			break;
		case CHANRUN:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/green.png" ) );
			break;
		case CHANSTOP:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/red.png" ) );
			break;
		case CHANRATE:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/black.png" ) );
			break;
		case CHANUNSYN:
			m_pChanStatList->item( order )->setIcon( QIcon( ":/res/black.png" ) );
			break;
		default:
			break;
		}//switch ( pChanData->stat )
		order++;
		pChanData++ ;
		pChanPara++ ;
	}//for (i = 0; i < MAX_CHAN_NUM; i++)

	if ( curChanNum < lastChanNum )
	{
		for ( int j = curChanNum; j < lastChanNum; j++ )
		{
			m_pChanStatList->takeItem( j );
		}
	}
	else if ( curChanNum > lastChanNum )
	{
		for ( int k = i; k < MAX_CHAN_NUM; k++ )
		{
			if ( !strcmp( pChanPara->desdribe , "" ) )
			{
				continue;
			}
			
			QListWidgetItem* tmp_listItem = NULL ;
			switch ( pChanData->stat )
			{
			case CHANUNCHECK:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/black.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			case CHANSEARCH:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			case CHANRUN:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/green.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			case CHANSTOP:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/red.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			case CHANRATE:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			case CHANUNSYN:
				tmp_listItem = new QListWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( pChanPara->desdribe ) ) ;
				m_pChanStatList->addItem( tmp_listItem );
				break;
			default:
				break;
			}//switch (  pChanData->stat )
			tmp_listItem->setSizeHint( QSize( 80 , 80) ) ;
			pChanData++ ;
			pChanPara++ ;
		}//for ( int k = i; k < MAX_CHAN_NUM; k++ )

	}

}


int QFesStat::calcRealChanNum( void )
{
	int chanNum = 0;

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;

	for ( int i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if (strcmp(pChanPara->desdribe, ""))
		{
			chanNum++;
		}
		pChanPara++ ;
	}

	return chanNum;
}

void QFesStat::onTimer( void )
{
	refleshChanStat(  );
}


void QFesStat::onListMode( void )
{
	m_pChanStatList->setVisible( true );
	m_pChanStatTable->setVisible( false );
	m_pChanStatList->setViewMode( QListView::IconMode );
}

void QFesStat::onTableMode( void )
{
	m_pChanStatList->setVisible( false );
	m_pChanStatTable->setVisible( true );
}


void QFesStat::onExit( void )
{
	this->close(  );
}


