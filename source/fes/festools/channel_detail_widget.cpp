#include "channel_detail_widget.h"
#include "festools_globaldef.h"

#include "fes/platformwrapper.h"
#include "uitools/dmessagebox.h"

#include <QMessageBox>

#include <stdio.h>

channel_detail_widget::channel_detail_widget( QWidget* parent /* = NULL */ ): QTableWidget( parent ) , m_fes_shm( Fes_shm::get() )
{
	setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行

	m_host_no = Net_stat::get_host_no();
	if ( read_protocol_para() == FALSE )
	{
		return ;
	}
	ini_status_list() ;
	ini_channel_stat_table() ;
}

void channel_detail_widget::ini_channel_stat_table()
{
	int i;
	setEditTriggers( QAbstractItemView::NoEditTriggers );
	setColumnCount( 9 );
	setRowCount( calc_real_channel_cnt() );
	setHorizontalHeaderLabels( QStringList(  )<<tr( "通道号" )
		<<tr( "通道名" )
		<<tr( "规约类型" )
		<<tr( "通道状态" )
		<<tr( "通道地址" )
		<<tr( "误码率" )
		<<tr( "停运时刻" )
		<<tr( "运行时刻" )
		<<tr( "当前值班机" )
		);
	setColumnWidth( 0, 55 );
	setColumnWidth( 1, 370 );
	setColumnWidth( 2, 220 );
	setColumnWidth( 3, 120 );
	setColumnWidth( 4, 220 );
	setColumnWidth( 5, 100 );
	setColumnWidth( 6, 270 );
	setColumnWidth( 7, 270 );

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;
	CHANDATA* pChanData = m_fes_shm.chan_data_ptr ;
	QTableWidgetItem* pItem ;

	int order = 0;
	for ( i = 0 ; i < MAX_CHAN_NUM ; i++ )
	{
		if ( !strcmp( pChanPara->desdribe, "" ) || pChanData->curr_attend_host == -1  )
		{
			pChanPara++;
			pChanData++;
			continue;
		}

		pItem = new QTableWidgetItem( QString::number( pChanPara->order ) ) ;
		setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
		setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
		setItem( order, 2, new QTableWidgetItem( m_protocol_map.value( _C2Q( pChanPara->protocol ) ) ) );
		setItem( order, 3, new QTableWidgetItem( m_status_list[ pChanData->stat ] ) );
		setItem( order, 4, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) + _C2Q( ":" ) + QString::number( pChanPara->port ) ) );
		setItem( order, 5, new QTableWidgetItem( QString::number( pChanData->bit_err_rate ) ) );

		struct tm* p_tm = localtime( &pChanData->last_down_time );
		char tmp_time[ 1024 ] ;
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		setItem( order, 6, new QTableWidgetItem( _C2Q( tmp_time ) ) );

		p_tm = localtime( &pChanData->last_up_time );
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		setItem( order, 7, new QTableWidgetItem( _C2Q( tmp_time ) ) );

		char* host_name = Net_stat::get_node_name( pChanData->curr_attend_host ) ;
		setItem( order, 8, new QTableWidgetItem( _C2Q( host_name ) ) );

		//非当前值班机置灰
		if ( m_host_no != pChanData->curr_attend_host )
		{
			item( order , 0 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 1 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 2 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 3 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 4 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 5 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 6 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 7 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 8 )->setBackgroundColor( Qt::lightGray ) ;
			setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/red.png" ), _C2Q( "" ) ) );
		}
		else
		{
			switch ( pChanData->stat )
			{
			case CHANUNCHECK:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/black.png" ), _C2Q( "" ) ) );
				break;
			case CHANSEARCH:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( "" ) ) );
				break;
			case CHANRUN:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/green.png" ), _C2Q( "" ) ) );
				break;
			case CHANSTOP:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/red.png" ), _C2Q( "" ) ) );
				break;
			case CHANRATE:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( "" ) ) );
				break;
			case CHANUNSYN:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( "" ) ) );
				break;
			default:
				break;
			}//switch ( pChanStatus->status )
		}
		order++;
		pChanPara++;
		pChanData++;
	}//for ( i = 0; i < m_compara.chanpara->chanNum; i++ )
}

void channel_detail_widget::ini_status_list( void )
{
	m_status_list.push_back( _C2Q( "未检测" ) );
	m_status_list.push_back( _C2Q( "搜索" ) );
	m_status_list.push_back( _C2Q( "运行" ) );
	m_status_list.push_back( _C2Q( "停止" ) );
	m_status_list.push_back( _C2Q( "误码率高" ) );
	m_status_list.push_back( _C2Q( "失步" ) );
}

bool channel_detail_widget::read_protocol_para( void )
{
	struct PROTOCOL_
	{
		char protocol_name[ 24 ] ; //规约名
		char describe[ 255 ] ; //规约描述
	};

	PROTOCOL_* pDb_protocol = NULL;
	Db_connector::open();
	int ret = -1 ;
	ret = Db_connector::select("select 规约名, 规约描述 from 规约参数表", (void **)&pDb_protocol) ;
	int rcd_cnt = ret; //规约表元素数量

	//读库正常
	if (ret > 0 && rcd_cnt > 0)
	{
		//根据数据库中读出的规约参数，创建规约工厂
		for (int i = 0; i < rcd_cnt; i++)
		{
			//创建规约实例
			PROTOCOL_* protocol_in_db = pDb_protocol + i;
			m_protocol_map[ _C2Q( protocol_in_db->protocol_name ) ]  = _C2Q( protocol_in_db->describe ) ;
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

	if ( pDb_protocol )
	{
		free( pDb_protocol ) ;
	}

	Db_connector::close() ;

	return TRUE ;
}

int channel_detail_widget::calc_real_channel_cnt( void )
{
	int channel_cnt = 0;

	CHANPARA* pChanPara = m_fes_shm.chan_para_ptr ;

	for ( int i = 0 ; i < MAX_CHAN_NUM ; i++ )
	{
		if ( strcmp( pChanPara->desdribe , "" ) )
		{
			channel_cnt++;
		}
		pChanPara++ ;
	}

	return channel_cnt ;
}

void channel_detail_widget::refresh_channel_table( void )
{
	int lastChanNum = rowCount(  );
	int curChanNum = calc_real_channel_cnt(  );

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

		item( order, 0 )->setText( QString::number( pChanPara->order ) );
		item( order, 1 )->setText( _C2Q( pChanPara->desdribe ) );
		item( order, 2 )->setText( m_protocol_map.value( _C2Q( pChanPara->protocol ) ) );
		item( order, 3 )->setText( m_status_list[pChanData->stat] );
		item( order, 4 )->setText( _C2Q( pChanPara->dev_name ) + _C2Q( ":" ) + QString::number( pChanPara->port ) );
		item( order, 5 )->setText( QString::number( pChanData->bit_err_rate ) );

		struct tm* p_tm = localtime( &pChanData->last_down_time );
		char tmp_time[ 1024 ] ;
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		item( order, 6 )->setText( _C2Q( tmp_time ) );

		p_tm = localtime( &pChanData->last_up_time );
		sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
			p_tm->tm_min, p_tm->tm_sec );
		item( order, 7 )->setText( _C2Q( tmp_time ) );

		char* host_name = Net_stat::get_node_name(pChanData->curr_attend_host);
		item( order, 8 )->setText( _C2Q( host_name ));

		switch ( pChanData->stat )
		{
		case CHANUNCHECK:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/black.png" ) );
			break;
		case CHANSEARCH:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/blue.png" ) );
			break;
		case CHANRUN:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/green.png" ) );
			break;
		case CHANSTOP:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/red.png" ) );
			break;
		case CHANRATE:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/yellow.png" ) );
			break;
		case CHANUNSYN:
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/puple.png" ) );
			break;
		default:
			break;	
		}//switch ( pChanStatus->status )

		if ( m_host_no != pChanData->curr_attend_host )
		{
			item( order , 0 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 1 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 2 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 3 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 4 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 5 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 6 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 7 )->setBackgroundColor( Qt::lightGray ) ;
			item( order , 8 )->setBackgroundColor( Qt::lightGray ) ;
			verticalHeaderItem( order )->setIcon( QIcon( ":/res/red.png" ) );
		}
		else
		{
			item( order , 0 )->setBackgroundColor( Qt::white ) ;
			item( order , 1 )->setBackgroundColor( Qt::white ) ;
			item( order , 2 )->setBackgroundColor( Qt::white ) ;
			item( order , 3 )->setBackgroundColor( Qt::white ) ;
			item( order , 4 )->setBackgroundColor( Qt::white ) ;
			item( order , 5 )->setBackgroundColor( Qt::white ) ;
			item( order , 6 )->setBackgroundColor( Qt::white ) ;
			item( order , 7 )->setBackgroundColor( Qt::white ) ;
			item( order , 8 )->setBackgroundColor( Qt::white ) ;
		}
		order++;
		pChanPara++;
		pChanData++;
	}//for ( i = 0; i < m_compara.chanpara->chanNum; i++ )

	//通道变少了
	if ( curChanNum < lastChanNum )
	{
		for ( int j = curChanNum; j < lastChanNum; j++ )
		{
			removeRow( j );
			takeVerticalHeaderItem( j );
		}

		setRowCount( curChanNum );
	}//if ( curChanNum < lastChanNum )

	else if ( curChanNum > lastChanNum ) //通道变多了
	{
		setRowCount( curChanNum );

		for ( int k = i; k < MAX_CHAN_NUM; k++ )
		{
			if ( !strcmp( pChanPara->desdribe, "" ) )
			{
				pChanPara++;
				pChanData++;
				continue;
			}

			setItem( order, 0, new QTableWidgetItem( QString::number( pChanPara->order ) ) );
			setItem( order, 1, new QTableWidgetItem( _C2Q( pChanPara->desdribe ) ) );
			setItem( order, 2, new QTableWidgetItem( m_protocol_map.value( _C2Q( pChanPara->protocol ) ) ) );
			setItem( order, 3, new QTableWidgetItem( m_status_list[pChanData->stat] ) );
			setItem( order, 4, new QTableWidgetItem( _C2Q( pChanPara->dev_name ) + _C2Q( ":" ) + QString::number( pChanPara->port ) ) );
			setItem( order, 5, new QTableWidgetItem( QString::number( pChanData->bit_err_rate ) ) );

			struct tm* p_tm = localtime( &pChanData->last_down_time );
			char tmp_time[ 1024 ] ;
			sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
				p_tm->tm_min, p_tm->tm_sec );
			setItem( order, 6, new QTableWidgetItem( _C2Q( tmp_time ) ) );

			p_tm = localtime( &pChanData->last_up_time );
			sprintf( tmp_time , "%d-%d-%d,%02d:%02d:%02d",p_tm->tm_year+1900, p_tm->tm_mon + 1 , p_tm->tm_mday, p_tm->tm_hour, 
				p_tm->tm_min, p_tm->tm_sec );
			setItem( order, 7, new QTableWidgetItem( _C2Q( tmp_time ) ) );

			char* host_name = Net_stat::get_node_name(pChanData->curr_attend_host);
			setItem( order, 8, new QTableWidgetItem( _C2Q( host_name ) ) );

			switch ( pChanData->stat )
			{
			case CHANUNCHECK:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/black.png" ), _C2Q( "" ) ) );
				break;
			case CHANSEARCH:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/blue.png" ), _C2Q( "" ) ) );
				break;
			case CHANRUN:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/green.png" ), _C2Q( "" ) ) );
				break;
			case CHANSTOP:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/red.png" ), _C2Q( "" ) ) );
				break;
			case CHANRATE:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/yellow.png" ), _C2Q( "" ) ) );
				break;
			case CHANUNSYN:
				setVerticalHeaderItem( order, new QTableWidgetItem( QIcon( ":/res/puple.png" ), _C2Q( "" ) ) );
				break;
			default:
				break;
			}//switch ( pChanData->stat )
			if ( m_host_no != pChanData->curr_attend_host )
			{
				item( order , 0 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 1 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 2 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 3 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 4 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 5 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 6 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 7 )->setBackgroundColor( Qt::lightGray ) ;
				item( order , 8 )->setBackgroundColor( Qt::lightGray ) ;
				verticalHeaderItem( order )->setIcon( QIcon( ":/res/red.png" ) );
			}
			order++;
			pChanPara++;
			pChanData++;
		}//for ( int k = i; k < MAX_CHAN_NUM; k++  )
	}
}