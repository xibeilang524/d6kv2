#include "fespara.h"
#include "fespara_string.h"
#include "fes/platformwrapper.h"
#include "uitools/dmessagebox.h"
#include <iostream>
#include <QtGui/QHeaderView>
#include <QtGui/QMessageBox>

using namespace std;

QFesPara::QFesPara( QWidget* parent /* = NULL */ ) : QDialog( parent ) , m_fes_shm( Fes_shm::get() )
{
	m_pChanTable = new QTableWidget( this );
	m_pRtuTable = new QTableWidget( this );
	m_pChanTable->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	m_pChanTable->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位

	m_pRtuButton = new QPushButton( QIcon( ":/res/terminal.png" ), _C2Q( "终端参数" ), this );
	m_pChanButton = new QPushButton( QIcon( ":/res/channel.png" ), _C2Q( "通道参数" ), this );
	m_pExitButton = new QPushButton( QIcon( ":/res/application-exit.png" ), _C2Q( "退出" ), this );
	m_pVLayout = new QVBoxLayout( NULL );
	m_pHLayout = new QHBoxLayout( NULL );

	m_pHLayout->addWidget( m_pChanButton );
	m_pHLayout->addWidget( m_pRtuButton );
	m_pHLayout->addWidget( m_pExitButton );

	m_pVLayout->addWidget( m_pChanTable );
	m_pVLayout->addWidget( m_pRtuTable );
	m_pVLayout->addLayout( m_pHLayout );
	m_pRtuTable->setVisible( false );

	setLayout( m_pVLayout );

	setGeometry( 200, 100, 1000, 500 );
	setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	setWindowTitle( _C2Q( "通道参数查看" ) );	

	readProtocol(  );
	initDevType(  );
	setPara(  );

	m_pTimer = new QTimer( this );
	connect( m_pTimer, SIGNAL( timeout(  ) ), this, SLOT( onTimer(  ) ) );
	m_pTimer->start( REFLESHTIME );
	connect( m_pChanButton, SIGNAL( clicked(  ) ), this, SLOT( onChanClicked(  ) ) );
	connect( m_pRtuButton, SIGNAL( clicked(  ) ), this, SLOT( onRtuClicked(  ) ) );
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

QFesPara::~QFesPara(  )
{
	
}

bool QFesPara::readProtocol( void )
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

void QFesPara::initDevType( void )
{
	m_devType.insert( 0, _C2Q( "COM" ) );
	m_devType.insert( 2, _C2Q( "MOXA" ) );
	m_devType.insert( 4, _C2Q( "UDPRTU" ) );
	m_devType.insert( 5, _C2Q( "TCPRTU" ) );
}


void QFesPara::setPara( void )
{
	setChanPara(  );
	setRtuPara(  );
}

void QFesPara::setChanPara( void )
{
 	int i;
	m_pChanTable->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pChanTable->verticalHeader()->setVisible( false );
	m_pChanTable->setColumnCount( 29 );
	m_pChanTable->setRowCount( calcRealChanNum(  ) );
	m_pChanTable->setHorizontalHeaderLabels( QStringList(  )<<_C2Q( "通道号" )
														 <<_C2Q( "通道名" )
														 <<_C2Q( "规约类型" )
														 <<_C2Q( "GPS标志" )
														 <<_C2Q( "GPS序号" )
														 <<_C2Q( "通道检验标志" )
														 <<_C2Q( "通道使用标识" )
														 <<_C2Q( "公共地址" )
														 <<_C2Q( "主站地址" )
														 <<_C2Q( "组号-RTU号" )
														 <<_C2Q( "设备类型" )
														 <<_C2Q( "本地设备地址" )
														 <<_C2Q( "本地端口号" )
														 <<_C2Q( "设备地址" )
														 <<_C2Q( "端口" )
														 <<_C2Q( "备设备地址" )
														 <<_C2Q( "备端口号" )
														 <<_C2Q( "波特率" )
														 <<_C2Q( "校验位" )
														 <<_C2Q( "停止位" )
														 <<_C2Q( "数据位" )
														 <<_C2Q( "流控制" )
														 <<_C2Q( "初始通道状态" )
														 <<_C2Q( "通道停止时限" )
														 <<_C2Q( "误码率标准" )
														 <<_C2Q( "遥测个数" )
														 <<_C2Q( "遥信个数" )
														 <<_C2Q( "电度个数" )
														 <<_C2Q( "默认值班机节点" )
											 );

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr;
	int order = 0;
	for ( i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if (!strcmp(pChanPara->desdribe, ""))
		{
			pChanPara++;
			continue;
		}
		m_pChanTable->setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
		m_pChanTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
		m_pChanTable->setItem( order, 2, new QTableWidgetItem( m_protocol.value( _C2Q( pChanPara->protocol ) ) ) );
		m_pChanTable->setItem( order, 3, new QTableWidgetItem( QString::number( pChanPara->gps ) ) );
		m_pChanTable->setItem( order, 4, new QTableWidgetItem( QString::number( pChanPara->gps_priority ) ) );
		m_pChanTable->setItem( order, 5, new QTableWidgetItem( QString::number( pChanPara->check ) ) );
		m_pChanTable->setItem( order, 6, new QTableWidgetItem( QString::number( pChanPara->valid ) ) );
		m_pChanTable->setItem( order, 7, new QTableWidgetItem( QString::number( pChanPara->common_addr ) ) );
		m_pChanTable->setItem( order, 8, new QTableWidgetItem( QString::number( pChanPara->master_addr ) ) );
		m_pChanTable->setItem( order, 9, new QTableWidgetItem( QString::number( pChanPara->child_rtu ) ) );
		m_pChanTable->setItem( order, 10, new QTableWidgetItem( QString::number( pChanPara->dev_type ) ) );
		m_pChanTable->setItem( order, 11, new QTableWidgetItem( _C2Q( pChanPara->local_dev ) ) );
		m_pChanTable->setItem( order, 12, new QTableWidgetItem( QString::number( pChanPara->local_port ) ) );
		m_pChanTable->setItem( order, 13, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) ) );
		m_pChanTable->setItem( order, 14, new QTableWidgetItem( QString::number( pChanPara->port ) ) );
		m_pChanTable->setItem( order, 15, new QTableWidgetItem( _C2Q( pChanPara->backup_dev_name ) ) );
		m_pChanTable->setItem( order, 16, new QTableWidgetItem( QString::number( pChanPara->back_up_port ) ) );
		m_pChanTable->setItem( order, 17, new QTableWidgetItem( QString::number( pChanPara->band ) ) );
		m_pChanTable->setItem( order, 18, new QTableWidgetItem( QString::number( pChanPara->check_bit ) ) );
		m_pChanTable->setItem( order, 19, new QTableWidgetItem( QString::number( pChanPara->stop_bit ) ) );
		m_pChanTable->setItem( order, 20, new QTableWidgetItem( QString::number( pChanPara->data_bit ) ) );
		m_pChanTable->setItem( order, 21, new QTableWidgetItem( QString::number( pChanPara->flow ) ) );
		m_pChanTable->setItem( order, 22, new QTableWidgetItem( QString::number( pChanPara->initial_stat ) ) );
		m_pChanTable->setItem( order, 23, new QTableWidgetItem( QString::number( pChanPara->stop_time_limit ) ) );
		m_pChanTable->setItem( order, 24, new QTableWidgetItem( QString::number( pChanPara->bit_error_rate ) ) );
		m_pChanTable->setItem( order, 25, new QTableWidgetItem( QString::number( pChanPara->yc_num ) ) );
		m_pChanTable->setItem( order, 26, new QTableWidgetItem( QString::number( pChanPara->yx_num ) ) );
		m_pChanTable->setItem( order, 27, new QTableWidgetItem( QString::number( pChanPara->kwh_num ) ) );
		m_pChanTable->setItem( order, 28, new QTableWidgetItem( QString::number( pChanPara->default_attend_host ) ) );
		order++;
		pChanPara++;
	}

	m_pChanTable->resizeColumnsToContents();
}


void QFesPara::setRtuPara( void )
{
	int i;
	m_pRtuTable->setEditTriggers( QAbstractItemView::NoEditTriggers );
	m_pRtuTable->verticalHeader(  )->setVisible( false );
	m_pRtuTable->setColumnCount( 7 );
	m_pRtuTable->setRowCount( calcRealRtuNum(  ) );
	m_pRtuTable->setHorizontalHeaderLabels( QStringList(  )<<_C2Q( "终端号" )
														<<_C2Q( "终端名" )
														<<_C2Q( "终端代码" )
														<<_C2Q( "遥测个数" )
														<<_C2Q( "遥信个数" )
														<<_C2Q( "电度个数" )
														<<_C2Q( "使用标志" )
											 );

	RTUPARA* pRtuPara = m_fes_shm.rtu_para_ptr ;
	int order = 0;
	for ( i = 0; i < MAX_RTU_NUM ; i++ )
	{
		if (!strcmp(pRtuPara->desdribe, ""))
		{
			pRtuPara++;
			continue;
		}

		m_pRtuTable->setItem( order, 0, new QTableWidgetItem( QString::number( pRtuPara->order ) ) );
		m_pRtuTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pRtuPara->desdribe ) ) );
		m_pRtuTable->setItem( order, 2, new QTableWidgetItem( _C2Q( pRtuPara->rtu_code ) ) );
		m_pRtuTable->setItem( order, 3, new QTableWidgetItem( QString::number( pRtuPara->yc_num ) ) );
		m_pRtuTable->setItem( order, 4, new QTableWidgetItem( QString::number( pRtuPara->yx_num ) ) );
		m_pRtuTable->setItem( order, 5, new QTableWidgetItem( QString::number( pRtuPara->kwh_num ) ) );
		m_pRtuTable->setItem( order, 6, new QTableWidgetItem( QString::number( pRtuPara->valid ) ) );

		order++;
		pRtuPara++;
	}

	m_pRtuTable->resizeColumnsToContents();

}

void QFesPara::onTimer( void )
{
	refleshChanPara(  );
	refleshRtuPara(  );
}


void QFesPara::refleshChanPara( void )
{
	int i;
	int oldChanNum = m_pChanTable->rowCount(  );
	int curChanNum = calcRealChanNum(  );

	CHANPARA*	pChanPara = m_fes_shm.chan_para_ptr;
	int order = 0;
	int findcount = 0 ;
	for ( i = 0; i < MAX_CHAN_NUM; i++ )
	{
		if ( !strcmp( pChanPara->desdribe, "" ) )
		{
			pChanPara++;
			continue;
		}

		if ( order >= oldChanNum )
		{
			findcount = i ;
			break;
		}

		m_pChanTable->item( order, 0 )->setText( QString::number( pChanPara->order ) ) ;
		m_pChanTable->item( order, 1 )->setText(  _C2Q( pChanPara->desdribe ) ) ;
		m_pChanTable->item( order, 2 )->setText( m_protocol.value( _C2Q( pChanPara->protocol ) ) ) ;
		m_pChanTable->item( order, 3 )->setText( QString::number( pChanPara->gps ) );
		m_pChanTable->item( order, 4 )->setText( QString::number( pChanPara->gps_priority ) );
		m_pChanTable->item( order, 5 )->setText( QString::number( pChanPara->check ) );
		m_pChanTable->item( order, 6 )->setText( QString::number( pChanPara->valid ) );
		m_pChanTable->item( order, 7 )->setText( QString::number( pChanPara->common_addr ) );
		m_pChanTable->item( order, 8 )->setText( QString::number( pChanPara->master_addr ) );
		m_pChanTable->item( order, 9 )->setText( QString::number( pChanPara->child_rtu ) );
		m_pChanTable->item( order, 10 )->setText( QString::number( pChanPara->dev_type ) );
		m_pChanTable->item( order, 11 )->setText( _C2Q( pChanPara->local_dev ) );
		m_pChanTable->item( order, 12 )->setText( QString::number( pChanPara->local_port ) );
		m_pChanTable->item( order, 13 )->setText( _C2Q( pChanPara->dev_name ) );
		m_pChanTable->item( order, 14 )->setText( QString::number( pChanPara->port ) );
		m_pChanTable->item( order, 15 )->setText( _C2Q( pChanPara->backup_dev_name ) );
		m_pChanTable->item( order, 16 )->setText( QString::number( pChanPara->back_up_port ) );
		m_pChanTable->item( order, 17 )->setText( QString::number( pChanPara->band ) );
		m_pChanTable->item( order, 18 )->setText( QString::number( pChanPara->check_bit ) );
		m_pChanTable->item( order, 19 )->setText( QString::number( pChanPara->stop_bit ) );
		m_pChanTable->item( order, 20 )->setText( QString::number( pChanPara->data_bit ) );
		m_pChanTable->item( order, 21 )->setText( QString::number( pChanPara->flow ) );
		m_pChanTable->item( order, 22 )->setText( QString::number( pChanPara->initial_stat ) );
		m_pChanTable->item( order, 23 )->setText( QString::number( pChanPara->stop_time_limit ) );
		m_pChanTable->item( order, 24 )->setText( QString::number( pChanPara->bit_error_rate ) );
		m_pChanTable->item( order, 25 )->setText( QString::number( pChanPara->yc_num ) );
		m_pChanTable->item( order, 26 )->setText( QString::number( pChanPara->yx_num ) );
		m_pChanTable->item( order, 27 )->setText( QString::number( pChanPara->kwh_num ) );
		m_pChanTable->item( order, 28 )->setText( QString::number( pChanPara->default_attend_host ) );

		order++;
		pChanPara++;
	}//for ( int i = 0; i < m_compara.chanpara->chanNum; i++ )

	//通道变少了
	if ( oldChanNum > curChanNum )
	{
		for ( int j = curChanNum; j < oldChanNum; j++ )
		{
			m_pChanTable->removeRow( j );
		}
		m_pChanTable->setRowCount( order );
	}

	//通道变多了
	else if ( oldChanNum < curChanNum )
	{
		m_pChanTable->setRowCount( curChanNum );

		for ( int k = findcount ; k < MAX_CHAN_NUM ; k++ )
		{
			if ( !strcmp( pChanPara->desdribe, "" ) )
			{
				pChanPara++;
				continue;
			}
			m_pChanTable->setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
			m_pChanTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
			m_pChanTable->setItem( order, 2, new QTableWidgetItem( m_protocol.value( _C2Q( pChanPara->protocol ) ) ) );
			m_pChanTable->setItem( order, 3, new QTableWidgetItem( QString::number( pChanPara->gps ) ) );
			m_pChanTable->setItem( order, 4, new QTableWidgetItem( QString::number( pChanPara->gps_priority ) ) );
			m_pChanTable->setItem( order, 5, new QTableWidgetItem( QString::number( pChanPara->check ) ) );
			m_pChanTable->setItem( order, 6, new QTableWidgetItem( QString::number( pChanPara->valid ) ) );
			m_pChanTable->setItem( order, 7, new QTableWidgetItem( QString::number( pChanPara->common_addr ) ) );
			m_pChanTable->setItem( order, 8, new QTableWidgetItem( QString::number( pChanPara->master_addr ) ) );
			m_pChanTable->setItem( order, 9, new QTableWidgetItem( QString::number( pChanPara->child_rtu ) ) );
			m_pChanTable->setItem( order, 10, new QTableWidgetItem( QString::number( pChanPara->dev_type ) ) );
			m_pChanTable->setItem( order, 11, new QTableWidgetItem( _C2Q( pChanPara->local_dev ) ) );
			m_pChanTable->setItem( order, 12, new QTableWidgetItem( QString::number( pChanPara->local_port ) ) );
			m_pChanTable->setItem( order, 13, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) ) );
			m_pChanTable->setItem( order, 14, new QTableWidgetItem( QString::number( pChanPara->port ) ) );
			m_pChanTable->setItem( order, 15, new QTableWidgetItem( _C2Q( pChanPara->backup_dev_name ) ) );
			m_pChanTable->setItem( order, 16, new QTableWidgetItem( QString::number( pChanPara->back_up_port ) ) );
			m_pChanTable->setItem( order, 17, new QTableWidgetItem( QString::number( pChanPara->band ) ) );
			m_pChanTable->setItem( order, 18, new QTableWidgetItem( QString::number( pChanPara->check_bit ) ) );
			m_pChanTable->setItem( order, 19, new QTableWidgetItem( QString::number( pChanPara->stop_bit ) ) );
			m_pChanTable->setItem( order, 20, new QTableWidgetItem( QString::number( pChanPara->data_bit ) ) );
			m_pChanTable->setItem( order, 21, new QTableWidgetItem( QString::number( pChanPara->flow ) ) );
			m_pChanTable->setItem( order, 22, new QTableWidgetItem( QString::number( pChanPara->initial_stat ) ) );
			m_pChanTable->setItem( order, 23, new QTableWidgetItem( QString::number( pChanPara->stop_time_limit ) ) );
			m_pChanTable->setItem( order, 24, new QTableWidgetItem( QString::number( pChanPara->bit_error_rate ) ) );
			m_pChanTable->setItem( order, 25, new QTableWidgetItem( QString::number( pChanPara->yc_num ) ) );
			m_pChanTable->setItem( order, 26, new QTableWidgetItem( QString::number( pChanPara->yx_num ) ) );
			m_pChanTable->setItem( order, 27, new QTableWidgetItem( QString::number( pChanPara->kwh_num ) ) );
			m_pChanTable->setItem( order, 28, new QTableWidgetItem( QString::number( pChanPara->default_attend_host ) ) );
			
			order++;
			pChanPara++;
		}//for ( int k = order; k < m_compara.chanpara->chanNum; k++ )
	}
}

void QFesPara::refleshRtuPara( void )
{
	int i;
	int oldRtuNum = m_pRtuTable->rowCount(  );
	int curRtuNum = calcRealRtuNum(  );
	RTUPARA* pRtuPara = m_fes_shm.rtu_para_ptr ;
	int order = 0;
	int findCount = 0 ;

	for ( i = 0 ; i < MAX_RTU_NUM ; i++ )
	{
		if ( !strcmp( pRtuPara->desdribe, "" ) )
		{
			pRtuPara++;
			continue;
		}

		if ( order >= oldRtuNum )
		{
			findCount = i ;
			break;
		}

		m_pRtuTable->item( order, 0 )->setText( QString::number( pRtuPara->order ) ) ;
		m_pRtuTable->item( order, 1 )->setText( _C2Q( pRtuPara->desdribe ) ) ;
		m_pRtuTable->item( order, 2 )->setText( _C2Q( pRtuPara->rtu_code ) ) ;
		m_pRtuTable->item( order, 3 )->setText( QString::number( pRtuPara->yc_num ) ) ;
		m_pRtuTable->item( order, 4 )->setText( QString::number( pRtuPara->yx_num ) ) ;
		m_pRtuTable->item( order, 5 )->setText( QString::number( pRtuPara->kwh_num ) ) ;
		m_pRtuTable->item( order, 6 )->setText( QString::number( pRtuPara->valid ) ) ;
		
		order++;
		pRtuPara++;
		
	}

	//终端变少了
	if ( oldRtuNum > curRtuNum )
	{
		for ( int j = order; j < oldRtuNum; j++ )
		{
			m_pRtuTable->removeRow( j );
		}
		m_pRtuTable->setRowCount( order );
	}
	//终端变多了
	else if ( oldRtuNum < curRtuNum )
	{
		cout<<"终端变多了"<<endl;
		m_pRtuTable->setRowCount( curRtuNum );
		for ( int k = findCount ; k < MAX_RTU_NUM ; k++ )
		{
			if ( !strcmp( pRtuPara->desdribe, "" ) )
			{
				pRtuPara++;
				continue;
			}
			
			m_pRtuTable->setItem( order, 0, new QTableWidgetItem( QString::number( pRtuPara->order ) ) );
			m_pRtuTable->setItem( order, 1, new QTableWidgetItem( _C2Q( pRtuPara->desdribe ) ) );
			m_pRtuTable->setItem( order, 2, new QTableWidgetItem( _C2Q( pRtuPara->rtu_code ) ) );
			m_pRtuTable->setItem( order, 3, new QTableWidgetItem( QString::number( pRtuPara->yc_num ) ) );
			m_pRtuTable->setItem( order, 4, new QTableWidgetItem( QString::number( pRtuPara->yx_num ) ) );
			m_pRtuTable->setItem( order, 5, new QTableWidgetItem( QString::number( pRtuPara->kwh_num ) ) );
			m_pRtuTable->setItem( order, 6, new QTableWidgetItem( QString::number( pRtuPara->valid ) ) );
			
			order++;
			pRtuPara++;
		}//for ( int k = order; k < m_compara.rtupara->rtuNum; k++ )
	}
}


void QFesPara::onChanClicked( void )
{
	m_pChanTable->setVisible( true );
	m_pRtuTable->setVisible( false );
}

void QFesPara::onRtuClicked( void )
{
	m_pChanTable->setVisible( false );
	m_pRtuTable->setVisible( true );
}


int QFesPara::calcRealChanNum( void )
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

int QFesPara::calcRealRtuNum( void )
{
	int rtuNum = 0;

	RTUPARA* pRtuPara = m_fes_shm.rtu_para_ptr ;

	for ( int i = 0; i < MAX_RTU_NUM; i++ )
	{
		if (strcmp(pRtuPara->desdribe, ""))
		{
			rtuNum++;
		}
		pRtuPara++ ;
	}

	return rtuNum;
}

void QFesPara::onExit( void )
{
	this->close(  );
}



