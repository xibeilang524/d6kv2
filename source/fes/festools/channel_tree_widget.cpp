#include "channel_tree_widget.h"
#include "festools_globaldef.h"
#include "uitools/dmessagebox.h"

#include <QGridLayout>
#include <QMessageBox>

channel_tree_widget::channel_tree_widget( QWidget* parent /*= NULL */ ) : QFrame( parent ) , m_fes_shm( Fes_shm::get() )
{
	ini_channel_tree() ;
	ini_tree_data() ;
	init_mail_buf() ;
	setFrameStyle( QFrame::Panel | QFrame::Sunken ) ;

	m_pStation_area_para = NULL ;
	m_pTerminal_view_para = NULL ;
	m_stationarea_cnt = 0 ;
	m_terminalview_cnt = 0 ;

	connect( m_pSearch_btn , SIGNAL( clicked() ) , this , SLOT( on_search_btn() ) ) ;
	connect( m_pIni_terminal_btn , SIGNAL( clicked() ) , this , SLOT( on_init_terminal() ) ) ;
	connect( m_pCall_alldata_btn , SIGNAL( clicked() ) , this , SLOT( on_call_alldata() ) ) ;
	connect( m_pCall_kwh_btn , SIGNAL( clicked() ) , this , SLOT( on_call_kwh() ) ) ;
	connect( m_pChk_time_btn , SIGNAL( clicked() ) , this , SLOT( on_check_time() ) ) ;
	connect( m_pChannel_tree , SIGNAL( itemPressed( QTreeWidgetItem* , int ) ) , this , SLOT( on_channel_changed( QTreeWidgetItem* , int ) ) ) ;
}

void channel_tree_widget::ini_channel_tree()
{
	m_pSearch_btn = new QPushButton( QPixmap( ":/res/search.png" ) , tr( "筛选" ) , this ) ;
	m_pSearch_lineedit = new QLineEdit( this ) ;

	m_pChannel_tree = new QTreeWidget( this ) ;
	m_pChannel_tree->setHeaderLabel( tr("通道列表") ) ;
	m_pIni_terminal_btn = new QPushButton( QPixmap( ":/res/reset.png" ) , tr( "终端初始化" ) , this ) ;
	m_pCall_alldata_btn = new QPushButton( QPixmap( ":/res/callall.png" ) , tr( "召唤全数据" ) , this ) ;
	m_pChk_time_btn = new QPushButton( QPixmap( ":/res/time.png" ) , tr( "对时" ) , this ) ;
	m_pCall_kwh_btn = new QPushButton( QPixmap( ":/res/callkwh.png" ) , tr( "召唤电度" ) , this ) ;

	m_pIni_terminal_btn->setEnabled( false ) ;
	m_pCall_alldata_btn->setEnabled( false ) ;
	m_pChk_time_btn->setEnabled( false ) ;
	m_pCall_kwh_btn->setEnabled( false ) ;

	QGridLayout* pGridLayout = new QGridLayout( this );
	pGridLayout->addWidget( m_pSearch_lineedit , 0 , 0 , 1 , 3 ) ;
	pGridLayout->addWidget( m_pSearch_btn , 0 , 3 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pChannel_tree , 1 , 0 , 1 , 4 ) ;
	pGridLayout->addWidget( m_pIni_terminal_btn , 2 , 0 , 1 , 2 ) ;
	pGridLayout->addWidget( m_pCall_alldata_btn , 2 , 2 , 1 , 2 ) ;
	pGridLayout->addWidget( m_pChk_time_btn , 3 , 0 , 1 , 2 ) ;
	pGridLayout->addWidget( m_pCall_kwh_btn , 3 , 2 , 1 , 2 ) ;
	pGridLayout->setContentsMargins( -1 , -1 , -1 , -1 ) ;

	setLayout( pGridLayout ) ;
}

void channel_tree_widget::init_mail_buf(void)
{
	//建立邮件内存
	m_pMail_struct = ( FE_MAIL_STRUCT* )malloc( sizeof( FE_MAIL_STRUCT ) + sizeof( FE_CALLING_MAIL ) ) ;
}

void channel_tree_widget::ini_tree_data(void)
{
	if ( read_station_area() == -1 || read_terminal_view() == -1 )
	{
		DMessageBox::warning( this , tr( "警告" ) , tr("读取数据库参数表出错!" ) ) ;
		return ;
	}

	m_station_area_map.clear() ;
	m_terminal_map.clear() ;
	m_channel_map.clear() ;

	QTreeWidgetItem* pItem , *pSubItem ;
	pItem = new QTreeWidgetItem( QStringList( tr( "未分组节点" ) ) ) ;
	pItem->setIcon( 0 ,  QIcon( ":/res/city.png" ) ) ;
	m_pChannel_tree->addTopLevelItem( pItem ) ;
	m_station_area_map.insert( 0 , pItem ) ; //id为0默认为未分组节点
	//一级菜单
	for ( int i = 0 ; i < m_stationarea_cnt ; i++ )
	{
		STATIONAREAPARA* pStation_area_data = m_pStation_area_para + i ;
		if ( pStation_area_data->masterid == 0 )
		{
			pItem = new QTreeWidgetItem( QStringList( tr( pStation_area_data->name ) ) ) ;
			pItem->setData( 0 , ROLE_MODEL_TYPE , TYPE_CITY ) ;
			pItem->setIcon( 0 ,  QIcon( ":/res/city.png" ) ) ;
			m_pChannel_tree->addTopLevelItem( pItem ) ;

			m_station_area_map.insert( pStation_area_data->id , pItem ) ;
		}
	}

	//二级菜单
	for ( int i = 0 ; i < m_stationarea_cnt ; i++ )
	{
		STATIONAREAPARA* pStation_area_data = m_pStation_area_para + i ;
		if ( pStation_area_data->masterid != 0 )
		{
			if( m_station_area_map.contains( pStation_area_data->masterid ) )
			{
				pItem = m_station_area_map[ pStation_area_data->masterid ] ;
				pSubItem = new QTreeWidgetItem( pItem , QStringList( tr( pStation_area_data->name ) ) ) ;
				pSubItem->setData( 0 , ROLE_MODEL_TYPE , TYPE_STATION ) ;
				pSubItem->setIcon( 0 ,  QIcon( ":/res/zone.png" ) ) ;
				pItem->addChild( pSubItem ) ;
				m_station_area_map.insert( pStation_area_data->id , pSubItem ) ;
			}
			else
			{
				//todo 需要处理,目前只有三级菜单
			}
		}
	}

	//增加终端描述
	for ( int i = 0 ;  i < m_terminalview_cnt ; i++ )
	{
		TERMINALVIEWPARA* pTerminal_view_data = m_pTerminal_view_para + i ;
		if ( m_station_area_map.contains( pTerminal_view_data->areaid ) )
		{
			pItem = m_station_area_map[ pTerminal_view_data->areaid ] ;
			pSubItem = new QTreeWidgetItem( pItem , QStringList( tr( pTerminal_view_data->terminaldesc ) ) ) ;
			pSubItem->setData( 0 , ROLE_MODEL_TYPE , TYPE_TERMINAL ) ;
			pSubItem->setData( 0 , TYPE_TERMINAL , pTerminal_view_data->terminalno ) ;
			pSubItem->setIcon( 0 ,  QIcon( ":/res/terminal.png" ) ) ;
			pItem->addChild( pSubItem ) ;

			m_terminal_map.insert( pTerminal_view_data->terminalno , pSubItem ) ;
		}
		else
		{
			//todo 需要处理,目前只有三级菜单
		}
	}

	//增加通道描述
	for ( int chan_no = 0 ; chan_no < MAX_CHAN_NUM ; chan_no++ )
	{
		CHANPARA* pChan_para = m_fes_shm.chan_para_ptr + chan_no ;

		if ( pChan_para->valid == 0 )
		{
			continue;
		}

		if ( m_terminal_map.contains( pChan_para->child_rtu ) )
		{
			pItem = m_terminal_map[ pChan_para->child_rtu ] ;
			pSubItem = new QTreeWidgetItem( QStringList( tr( pChan_para->desdribe ) ) ) ;
			pSubItem->setIcon( 0 ,  QIcon( ":/res/curentlnode.png" ) ) ;
			pSubItem->setData( 0 , ROLE_MODEL_TYPE , TYPE_CHANNAL ) ;
			pSubItem->setData( 0 , TYPE_CHANNAL , pChan_para->order ) ;
			pItem->addChild( pSubItem ) ;

			m_channel_map.insert( pChan_para->order , pSubItem ) ;
		}
		else
		{
			//todo 需要处理,目前只有三级菜单
		}
	}
}

void channel_tree_widget::refresh_channel_tree()
{
	int order = 0 ;
	for ( int chan_no = 0 ; chan_no < MAX_CHAN_NUM ; chan_no++ )
	{
		CHANPARA* pChan_para = m_fes_shm.chan_para_ptr + chan_no ;
		CHANDATA* pChan_data = m_fes_shm.chan_data_ptr + chan_no ;

		if ( pChan_para->valid == 0 )
		{
			continue ;
		}
		
		if ( !m_channel_map.contains( pChan_para->order ) )
		{
			continue ;
		}

		QTreeWidgetItem* pItem = m_channel_map[ pChan_para->order ] ;
		switch( pChan_data->stat )
		{
		case CHANUNCHECK:
			pItem->setIcon( 0 , QIcon( ":/res/black.png" ) );
			break;
		case CHANSEARCH:
			pItem->setIcon( 0 , QIcon( ":/res/blue.png" ) );
			break;
		case CHANRUN:
			pItem->setIcon( 0 , QIcon( ":/res/green.png" ) );
			break;
		case CHANSTOP:
			pItem->setIcon( 0 , QIcon( ":/res/red.png" ) );
			break;
		case CHANRATE:
			pItem->setIcon( 0 , QIcon( ":/res/yellow.png" ) );
			break;
		case CHANUNSYN:
			pItem->setIcon( 0 , QIcon( ":/res/puple.png" ) );
			break;
		default:
			break;	
		}
	}
}

int channel_tree_widget::read_station_area()
{
	int ret = -1 ;
	if ( m_dbapi.Open( DB_OP_R , FALSE , FALSE ) != CS_SUCCEED )
	{
		printf( "打开数据库失败,返回!\n" ) ;
		return ret;
	}

	CS_DATAFMT* datafmt = NULL ;
	FUNC_STR  func ;
	func.func = SEL_ISQL_RESULTS ;
	strcpy( func.dbname , DB_NAME ) ;
	strcpy( func.table , "stationarea" ) ;
	strcpy( func.isql , "select * from stationarea" ) ;

	if ( CS_SUCCEED == m_dbapi.SelectIsqlResults( &func , &datafmt , (void **)&m_pStation_area_para ) )
	{
		ret = func.ret_roxnum ;
	}

	m_stationarea_cnt = ret ;

	if ( datafmt )
	{
		free( datafmt ) ;
		datafmt = NULL ;
	}

	m_dbapi.Close();
	return ret ;
}

int channel_tree_widget::read_terminal_view()
{
	int ret = -1 ;
	if ( m_dbapi.Open( DB_OP_R , FALSE , FALSE ) != CS_SUCCEED )
	{
		printf( "打开数据库失败,返回!\n" ) ;
		return ret;
	}

	CS_DATAFMT* datafmt = NULL ;
	FUNC_STR  func ;
	func.func = SEL_ISQL_RESULTS ;
	strcpy( func.dbname , DB_NAME ) ;
	strcpy( func.table , "subareaterminalview" ) ;
	strcpy( func.isql , "select 终端序号, 终端代码,终端描述,\
						站所线系代码, 站所线系描述,id,masterid,\
						name from subareaterminalview order by 终端序号" ) ;

	if ( CS_SUCCEED == m_dbapi.SelectIsqlResults( &func , &datafmt , (void **)&m_pTerminal_view_para ) )
	{
		ret = func.ret_roxnum ;
	}

	m_terminalview_cnt = ret ;

	if ( datafmt )
	{
		free( datafmt ) ;
		datafmt = NULL ;
	}

	m_dbapi.Close();
	return ret ;
}

void channel_tree_widget::on_search_btn()
{
	QString filter_str = m_pSearch_lineedit->text() ;
	QTreeWidgetItem* pItem ;

	for( int i = 0 ; i < m_terminal_map.count() ; i++ )
	{
		pItem = m_terminal_map.values().at( i ) ;
		int child_item_count = 0  ;
		if ( filter_str.isEmpty() )
		{
			for ( int j = 0 ; j < pItem->childCount() ; j++ )
			{
				pItem->child( j )->setHidden( false ) ;
			}
			pItem->setHidden( false ) ;
		}
		else
		{
			for ( int j = 0 ; j < pItem->childCount() ; j++ )
			{
				QString str = pItem->child( j )->text( 0 ) ;
				if ( str.indexOf( filter_str ) == -1 )
				{
					pItem->child( j )->setHidden( true ) ;
					child_item_count++ ;
				}
				else
				{
					pItem->child( j )->setHidden( false ) ;
				}
			}
			if ( child_item_count == pItem->childCount() )
			{
				pItem->setHidden( true ) ;
			}
		}
	}
}

int channel_tree_widget::get_cur_channel_no()
{
	QTreeWidgetItem* pItem = m_pChannel_tree->currentItem() ;
	return pItem->data( 0 , TYPE_CHANNAL ).toInt() ;
}

int channel_tree_widget::get_cur_terminal_no()
{
	QTreeWidgetItem* pItem = m_pChannel_tree->currentItem() ;
	QTreeWidgetItem* pUpItem = pItem->parent() ;
	if ( pUpItem )
	{
		return pUpItem->data( 0 , TYPE_TERMINAL ).toInt() ;
	}
}

void channel_tree_widget::on_call_alldata()
{
	memset(m_pMail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_pMail_struct->mail_type = CALLING;
	m_pMail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_pMail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CALL_ALL;
	call_mail->channel_no = get_cur_channel_no();
	call_mail->rtu_no = get_cur_terminal_no();
	Fe_mail::send_to_fe(m_pMail_struct, m_pMail_struct->mail_lenth);
}

void channel_tree_widget::on_call_kwh()
{
	memset(m_pMail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_pMail_struct->mail_type = CALLING;
	m_pMail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_pMail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CALL_KWH;
	call_mail->channel_no = get_cur_channel_no();
	call_mail->rtu_no = get_cur_terminal_no();
	Fe_mail::send_to_fe(m_pMail_struct, m_pMail_struct->mail_lenth);
}

void channel_tree_widget::on_check_time()
{
	memset(m_pMail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_pMail_struct->mail_type = CALLING;
	m_pMail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_pMail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::CHECK_TIME;
	call_mail->channel_no = get_cur_channel_no();
	call_mail->rtu_no = get_cur_terminal_no();
	Fe_mail::send_to_fe(m_pMail_struct, m_pMail_struct->mail_lenth);
}

void channel_tree_widget::on_init_terminal()
{
	memset(m_pMail_struct, 0, sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL));	//初始化

	m_pMail_struct->mail_type = CALLING;
	m_pMail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CALLING_MAIL);

	FE_CALLING_MAIL* call_mail = (FE_CALLING_MAIL*)m_pMail_struct->mail_body;

	call_mail->call_type = FE_CALLING_MAIL::RTU_INIT;
	call_mail->channel_no = get_cur_channel_no();
	call_mail->rtu_no = get_cur_terminal_no();
	Fe_mail::send_to_fe(m_pMail_struct, m_pMail_struct->mail_lenth);
}

void channel_tree_widget::on_channel_changed( QTreeWidgetItem* pItem , int col )
{
	if ( pItem->data( 0 , ROLE_MODEL_TYPE ) == TYPE_CHANNAL )
	{
		m_pIni_terminal_btn->setEnabled( true ) ;
		m_pCall_alldata_btn->setEnabled( true ) ;
		m_pChk_time_btn->setEnabled( true ) ;
		m_pCall_kwh_btn->setEnabled( true ) ;
	}
	else
	{
		m_pIni_terminal_btn->setEnabled( false ) ;
		m_pCall_alldata_btn->setEnabled( false ) ;
		m_pChk_time_btn->setEnabled( false ) ;
		m_pCall_kwh_btn->setEnabled( false ) ;
	}
	emit on_channel_select_changed( pItem , col ) ;
}
