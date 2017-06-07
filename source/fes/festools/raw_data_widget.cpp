#include "raw_data_widget.h"
#include "fes/fesdatashm_accessor.h"

#include <QVBoxLayout>
#include <QHeaderView>

raw_data_widget::raw_data_widget( QWidget* parent /*= NULL */ ) : QFrame( parent ) , m_fes_shm( Fes_shm::get() )
{
	ini_raw_data_widget() ;

	m_terminal_no = -1 ;
	m_channel_no = -1 ;
	m_table_type = -1 ;
	m_yc_list = NULL ;
	m_yx_list = NULL ;
	m_kwh_list = NULL ;
	m_yc_cnt = 0 ;
	m_yx_cnt = 0 ;
	m_kwh_cnt = 0 ;
	on_tab_changed( 0 ) ;

	m_pTimer = new QTimer();
	connect( m_pTimer , SIGNAL( timeout() ) , this , SLOT( on_timer() ) ) ;
	m_pTimer->start( 1000 ) ;
	hide_all_widgets( true ) ;

	setFrameStyle( QFrame::Panel | QFrame::Sunken ) ;
	connect( m_pTab_bar , SIGNAL( currentChanged( int ) ) , this , SLOT( on_tab_changed( int ) ) ) ;
}

void raw_data_widget::ini_raw_data_widget()
{
	m_pTab_bar = new QTabBar( this ) ;
	m_pTab_bar->addTab( tr( "遥测" ) ) ;
	m_pTab_bar->addTab( tr( "遥信" ) ) ;
	m_pTab_bar->addTab( tr( "电度" ) ) ;
	m_pData_table = new QTableWidget( this ) ;
	m_pData_table->setEditTriggers ( QAbstractItemView::NoEditTriggers );

	QWidget* pWidget = new QWidget( this ) ;
	QHBoxLayout* pHBox_layout = new QHBoxLayout( pWidget );
	pHBox_layout->addWidget( m_pTab_bar ) ;
	QSpacerItem* pHorizontal_spacer = new QSpacerItem( 40 , 20 , QSizePolicy::Expanding , QSizePolicy::Minimum ) ;
	pHBox_layout->addItem( pHorizontal_spacer ) ;

	QVBoxLayout* pVerticalLayout = new QVBoxLayout( this );
	pVerticalLayout->addWidget( pWidget ) ;
	pVerticalLayout->addWidget( m_pData_table ) ;
	pVerticalLayout->setSpacing(0);
	setLayout( pVerticalLayout ) ;
}

void raw_data_widget::hide_all_widgets( bool flag )
{
	if ( flag )
	{
		m_pTab_bar->hide() ;
		m_pData_table->hide() ;
		m_pTimer->stop() ;
		hide() ;
	}
	else
	{
		m_pTab_bar->show() ;
		m_pData_table->show() ;
		m_pTimer->start( 1000 ) ;
		show() ;
	}
}

void raw_data_widget::change_channel_no( int channel_no )
{
	m_channel_no = channel_no ;
	on_tab_changed( m_pTab_bar->currentIndex() ) ;
}

void raw_data_widget::change_termianl_no( int terminal_no )
{
	m_terminal_no = terminal_no ;
}

void raw_data_widget::on_tab_changed( int current_index )
{
	m_table_type = current_index ;

	m_pData_table->clear() ;
	ini_raw_data_table( current_index ) ;
	if ( m_terminal_no < 0 )
	{
		return ;
	}
	if ( current_index == 0 )
	{
		read_yc_data() ;
		m_pData_table->setRowCount( m_fes_shm.rtu_para_ptr[ m_terminal_no ].yc_num );
	}
	else if ( current_index == 1 )
	{
		read_yx_data() ;
		m_pData_table->setRowCount( m_fes_shm.rtu_para_ptr[ m_terminal_no ].yx_num );
	}
	else if ( current_index == 2 )
	{
		read_kwh_data() ;
		m_pData_table->setRowCount( m_fes_shm.rtu_para_ptr[ m_terminal_no ].kwh_num );
	}
	fill_raw_desc() ;
}

void raw_data_widget::ini_raw_data_table( int current_index )
{
	if ( current_index == 0 )
	{
		m_pData_table->setColumnCount( 7 );
		QStringList ycHeader;
		ycHeader<<tr( "遥测号" )
			<<tr( "描述" )
			<<tr( "解析码" )
			<<tr( "有效" )
			<<tr( "变化时间秒" )
			<<tr( "变化时间毫秒" )
			<<tr( "计算量" );
		m_pData_table->setHorizontalHeaderLabels( ycHeader ); 
		m_pData_table->verticalHeader()->setVisible( false );

		m_pData_table->setColumnWidth( 0, 70 );
		m_pData_table->setColumnWidth( 1, 400 );
		m_pData_table->setColumnWidth( 2, 150 );
		m_pData_table->setColumnWidth( 3, 70 );
		m_pData_table->setColumnWidth( 4, 200 );
		m_pData_table->setColumnWidth( 5, 200 );
		m_pData_table->setColumnWidth( 6, 70 );
	}
	else if ( current_index == 1 )
	{
		m_pData_table->setColumnCount( 8 );
		QStringList yxHeader;
		yxHeader<<tr( "遥信号" )
			<<tr( "描述" )
			<<tr( "遥信值" )
			<<tr( "取反" )
			<<tr( "有效" )
			<<tr( "变化时间秒" )
			<<tr( "变化时间毫秒" )
			<<tr( "计算量" );
		m_pData_table->setHorizontalHeaderLabels( yxHeader );
		m_pData_table->verticalHeader()->setVisible( false );

		m_pData_table->setColumnWidth( 0, 70 );
		m_pData_table->setColumnWidth( 1, 400 );
		m_pData_table->setColumnWidth( 2, 200 );
		m_pData_table->setColumnWidth( 3, 200 );
		m_pData_table->setColumnWidth( 4, 70 );
		m_pData_table->setColumnWidth( 5, 200 );
		m_pData_table->setColumnWidth( 6, 200 );
		m_pData_table->setColumnWidth( 7, 70 );
	}
	else if ( current_index == 2 )
	{
		m_pData_table->setColumnCount( 7 );
		QStringList kwhHeader;
		kwhHeader<<tr( "电度号" )
			<<tr( "描述" )
			<<tr( "电度值" )
			<<tr( "有效" )
			<<tr( "变化时间秒" )
			<<tr( "变化时间毫秒" )
			<<tr( "计算量" );	
		m_pData_table->setHorizontalHeaderLabels( kwhHeader );
		m_pData_table->verticalHeader()->setVisible( false );

		m_pData_table->setColumnWidth( 0, 70 );
		m_pData_table->setColumnWidth( 1, 400 );
		m_pData_table->setColumnWidth( 2, 200 );
		m_pData_table->setColumnWidth( 3, 200 );
		m_pData_table->setColumnWidth( 4, 200 );
		m_pData_table->setColumnWidth( 5, 200 );
		m_pData_table->setColumnWidth( 6, 70 );
	}
}

bool raw_data_widget::read_yc_data()
{
	CS_RETCODE retcode = 0; 
	int i;

	FUNC_STR func;
	memset(  &func,0, sizeof(  func  )  );
	CS_DATAFMT *datafmt=NULL;
	func.func = GET_TABLE_INFO;
	strcpy( func.table,"模拟量视图" );
	strcpy( func.dbname,DB_NAME );

	char * col_name[] = {"遥测序号","描述","计算量标志"};

	char str[100];
	sprintf( str,"终端序号=%d", m_terminal_no );
	strcpy( func.isql,str );

	void* pt = 0;

	char filename[256];
	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
//		return false ;
		sprintf( filename, "%s/dbmap/模拟量视图1/datafmt", getenv( "NBENV" ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/模拟量视图1/func", getenv( "NBENV" ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/模拟量视图1/pt", getenv( "NBENV" ) );
		if( !read_file(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,ycNo, isComputer;
		char desc[DESC_LEN];
		m_yc_list = new RTDATA[ func.ret_roxnum ];
		int i,j;
		j = 0;
		for(i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"终端序号",pt,ptcnt,NULL );
			ycNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			if (rtuNo == m_terminal_no)
			{
				m_yc_list[ j ].no = ycNo;
				m_yc_list[ j ].desc = _C2Q(  desc  );
				m_yc_list[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_yc_cnt = j;
		printf("yc num = %d\n",m_yc_cnt);
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"遥测序号" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}
		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int ptcnt = 0;
		int ycNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];

		m_yc_list = new RTDATA[ func.ret_roxnum ];
		m_yc_cnt = func.ret_roxnum;
		for( i=0; i<func.ret_roxnum; i++ )
		{
			ycNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			m_yc_list[ i ].no = ycNo;
			m_yc_list[ i ].desc = _C2Q(  desc  );
			m_yc_list[ i ].iscomputer = isComputer;
		}
	}

	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;

	m_db.Close(  );
	return true;
}

bool raw_data_widget::read_yx_data()
{
	CS_RETCODE retcode = 0; 
	int i;

	char filename[256];
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	func.func = GET_TABLE_INFO;
	strcpy( func.table,"遥信量视图" );
	strcpy( func.dbname, DB_NAME );

	char * col_name[] = {"遥信序号", "描述", "取反标志", "计算量标志"};

	char str[100];
	sprintf( str,"终端序号=%d", m_terminal_no );
	strcpy( func.isql,str );
	void* pt = 0;


	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
//		return false;
		sprintf( filename, "%s/dbmap/遥信量视图1/datafmt", getenv( "NBENV" ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/遥信量视图1/func", getenv( "NBENV" ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/遥信量视图1/pt", getenv( "NBENV" ) );
		if( !read_file(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,yxNo, isComputer, isNegate;
		char desc[DESC_LEN];
		m_yx_list = new RTDATA[ func.ret_roxnum ];
		int i,j;
		j = 0;
		for(i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"终端序号",pt,ptcnt,NULL );
			yxNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isNegate = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[3],pt,ptcnt,NULL );

			if (rtuNo == m_terminal_no)
			{
				m_yx_list[ j ].no = yxNo;
				m_yx_list[ j ].desc = _C2Q(  desc  );
				m_yx_list[ j ].isnegate = isNegate;
				m_yx_list[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_yx_cnt = j;
		printf("yx num = %d\n",m_yx_cnt);
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"遥信序号" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int ptcnt = 0;
		int yxNo, isComputer, isNegate;
		char desc[DESC_LEN];
		char computer[DESC_LEN];
		char negate[DESC_LEN];

		m_yx_list = new RTDATA[ func.ret_roxnum ];
		m_yx_cnt = func.ret_roxnum;

		for( i=0; i<func.ret_roxnum; i++ )
		{
			yxNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isNegate = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[3],pt,ptcnt,NULL );

			m_yx_list[ i ].no = yxNo;
			m_yx_list[ i ].desc = _C2Q(  desc  );
			m_yx_list[ i ].isnegate = isNegate;
			m_yx_list[ i ].iscomputer = isComputer;
		}
	}

	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;
	m_db.Close(  );

	return true;
}

bool raw_data_widget::read_kwh_data()
{
	CS_RETCODE retcode = 0; 
	int i;
	char filename[256];
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	func.func = GET_TABLE_INFO;
	strcpy( func.table,"电度视图" );
	strcpy( func.dbname, DB_NAME );

	char * col_name[] = {"电度序号","描述","计算量标志"};

	char str[100];
	sprintf( str,"终端序号=%d", m_terminal_no );
	strcpy( func.isql,str );
	void* pt = 0;


	if( !m_db.Open( DB_OP_R,FALSE,FALSE ) )
	{
//		return false;
		sprintf( filename, "%s/dbmap/电度视图1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/电度视图1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_db.GetTableInfo( &func, NULL, NULL );

		sprintf( filename, "%s/dbmap/电度视图1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;

		int ptcnt = 0;
		int rtuNo,kwhNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];
		int j = 0;

		m_kwh_list = new RTDATA[ func.ret_roxnum ];
		for( i=0; i<func.ret_roxnum; i++ )
		{
			rtuNo = ( int )m_db.GetData( i,&func,datafmt,"终端序号",pt,ptcnt,NULL );
			kwhNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );
			if (rtuNo == m_terminal_no)
			{
				m_kwh_list[ j ].no = kwhNo;
				m_kwh_list[ j ].desc = _C2Q(  desc  );
				m_kwh_list[ j ].iscomputer = isComputer;
				j++;
			}
		}
		m_kwh_cnt = j;
		printf("m_kwhNum = %d\n",m_kwh_cnt);
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt )==CS_FAIL )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			datafmt = NULL;
			return false;
		}

		int col_num = sizeof( col_name )/sizeof( char* );

		for( int i=0;i<func.colnum;i++ )
		{
			func.col[i].select = NO;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			func.col[i].order_d = -1;
			for( int j=0; j<col_num; j++ )
			{
				if( strcasecmp( datafmt[i].name,col_name[j] ) )
				{
					continue;
				}
				if( strcasecmp( datafmt[i].name,"电度序号" )==0 )
				{
					func.col[i].order = 0;
				}
				func.col[i].select = YES;
				func.col[i].order_d = j;
				break;
			}
		}

		func.func = SEL_ISQL_RECORDERS;
		if( ( retcode = m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return false;
		}

		int lognum = func.ret_roxnum;
		if( lognum == 0  ) 
		{
			if(  datafmt  )
			{
				free(  datafmt  );
			}
			if ( pt )
			{
				free( pt );
			}
			datafmt = NULL;
			pt = NULL;
			return true;
		}
		int ptcnt = 0;
		int kwhNo, isComputer;
		char desc[DESC_LEN];
		char computer[DESC_LEN];


		m_kwh_list = new RTDATA[ func.ret_roxnum ];
		m_kwh_cnt = func.ret_roxnum;

		for( i=0; i<func.ret_roxnum; i++ )
		{
			kwhNo = ( int )m_db.GetData( i,&func,datafmt,col_name[0],pt,ptcnt,NULL );
			m_db.GetData( i,&func,datafmt,col_name[1],pt,ptcnt,desc );
			isComputer = ( int )m_db.GetData( i,&func,datafmt,col_name[2],pt,ptcnt,NULL );

			m_kwh_list[ i ].no = kwhNo;
			m_kwh_list[ i ].desc = _C2Q(  desc  );
			m_kwh_list[ i ].iscomputer = isComputer;
		}
	}

	if( datafmt )
	{
		free( datafmt );
	}
	if ( pt )
	{
		free( pt );
	}
	datafmt = NULL;
	pt = NULL;
	m_db.Close(  );

	return true;
}

void raw_data_widget::fill_raw_desc()
{
	PRTDATA pRD ;
	QString computer , negate ;
	if ( m_table_type == 0 )
	{
		for( int i = 0 ; i < m_yc_cnt ; i ++  )
		{
			pRD = &m_yc_list[ i ] ;

			if( pRD->no >= m_pData_table->rowCount() )
			{
				continue;
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "是" );
			}
			else 
			{
				computer = _C2Q( "否" );
			}

			m_pData_table->setItem(  pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) )  );
			m_pData_table->setItem(  pRD->no, 1, new QTableWidgetItem( pRD->desc )  );
			m_pData_table->setItem(  pRD->no, 6, new QTableWidgetItem( computer )  );
		}
	}
	else if ( m_table_type == 1 )
	{
		for ( int i = 0 ; i < m_yx_cnt ; i++ )
		{
			pRD = &m_yx_list[ i ];

			if(  pRD->no >= m_pData_table->rowCount(  )  )
			{
				continue;
			}

			if ( 1 == pRD->isnegate )
			{
				negate = _C2Q(  "取反"  );
			}
			else 
			{
				negate = _C2Q(  "不取反" );
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "是" );
			}
			else 
			{
				computer = _C2Q( "否" );
			}

			m_pData_table->setItem( pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) ) );
			m_pData_table->setItem( pRD->no, 1, new QTableWidgetItem( pRD->desc ) );
			m_pData_table->setItem( pRD->no, 3, new QTableWidgetItem( negate ) );
			m_pData_table->setItem( pRD->no, 7, new QTableWidgetItem( computer ) );
		}
	}
	else if ( m_table_type == 2 )
	{
		for ( int i = 0 ; i < m_kwh_cnt ; i++ )
		{
			pRD = &m_kwh_list[ i ];

			if(  pRD->no >= m_pData_table->rowCount(  )  )
			{
				continue;
			}

			if (  1 == pRD->iscomputer  )
			{
				computer = _C2Q( "是" );
			}
			else 
			{
				computer = _C2Q( "否" );
			}

			m_pData_table->setItem( pRD->no, 0, new QTableWidgetItem( QString::number( pRD->no ) ) );
			m_pData_table->setItem( pRD->no, 1, new QTableWidgetItem( pRD->desc ) );
			m_pData_table->setItem( pRD->no, 6, new QTableWidgetItem( computer ) );
		}
	}
	clear_rcd_list() ;
}

void raw_data_widget::clear_rcd_list()
{
	if( m_yc_list )
	{
		delete [] m_yc_list;
		m_yc_list = NULL;
	}
	m_yc_cnt = 0;

	if( m_yx_list )
	{
		delete [] m_yx_list;
		m_yx_list = NULL;
	}
	m_yx_cnt = 0;

	if( m_kwh_list )
	{
		delete [] m_yx_list;
		m_yx_list = NULL;
	}
	m_kwh_cnt = 0;
}

void raw_data_widget::fill_raw_value()
{
	Fes_data_shm_accessor&	fesdatashm = Fes_data_shm_accessor::get();

	unsigned char  yxValue ;
	unsigned char  yxValid ;  
	float  kwhValue = 0.f ;
	unsigned char  kwhValid ;
	float fYcValue = 0.f ;
	unsigned int uiYcCode = 0 ;
	tm procsec ;
	unsigned int procmsec = 0 ;
	int rtuyc = fesdatashm.rtu_yc( m_terminal_no ) ;
	int rtuyx = fesdatashm.rtu_yx( m_terminal_no ) ;
	int rtukwh = fesdatashm.rtu_kwh( m_terminal_no ) ;

	FE_YC* yc_ptr = fesdatashm.channel_yc( m_channel_no ) + rtuyc;
	FE_YX* yx_ptr = fesdatashm.channel_yx( m_channel_no ) + rtuyx;
	FE_KWH* kwh_ptr = fesdatashm.channel_kwh( m_channel_no ) + rtukwh;

	if (!yc_ptr || !yx_ptr || !kwh_ptr)
	{
		return;
	}

	if ( m_table_type == 0 )
	{
		for( int i = 0 ; i < m_fes_shm.rtu_para_ptr[ m_terminal_no ].yc_num ; i ++  )
		{
			fYcValue = yc_ptr->raw_yc ;
			m_pData_table->setItem( i, 2,  new QTableWidgetItem( QString::number( fYcValue ) ) );
			if ( FLOAT_DEFAULT == fYcValue )
			{
				m_pData_table->setItem( i, 3, new QTableWidgetItem( _C2Q( "无效" ) ) );
			}
			else 
			{
				m_pData_table->setItem( i, 3, new QTableWidgetItem( _C2Q( "有效" ) ) );
			}

			procsec.tm_sec = yc_ptr->modifier.time.second;
			procsec.tm_min = yc_ptr->modifier.time.minute ;
			procsec.tm_hour = yc_ptr->modifier.time.hour ;
			procsec.tm_mday = yc_ptr->modifier.time.day ;
			procsec.tm_mon = yc_ptr->modifier.time.month ;
			procsec.tm_year = yc_ptr->modifier.time.year ;
// 			m_pData_table->setItem( i, 4, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
			char str1[ 128 ] ;
			sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
			m_pData_table->setItem( i, 4, new QTableWidgetItem( tr( str1 ) ) ) ;
			procmsec = fesdatashm.fe_data()->data_seg.yc[ rtuyc + i ].modifier.time.milisecond ;
			m_pData_table->setItem( i, 5, new QTableWidgetItem( QString::number( procmsec ) ) );

			yc_ptr++;
		}
	}
	else if ( m_table_type == 1 )
	{
		for ( int i = 0 ; i < m_fes_shm.rtu_para_ptr[ m_terminal_no ].yx_num ; i++ )
		{
			yxValue = yx_ptr->raw_yx ;
			if ( 1 == yxValue  )
			{
				QTableWidgetItem* newitem = new QTableWidgetItem( _C2Q( "合" ) );
				newitem->setTextColor( QColor( 255, 0, 0 ) );
				m_pData_table->setItem( i, 2, newitem );
			}
			else
			{
				m_pData_table->setItem( i, 2,  new QTableWidgetItem( _C2Q( "分" ) ) );
			}

			yxValid = yx_ptr->modifier.quality ;
			if ( UCHAR_DEFAULT == yxValue )
			{
				m_pData_table->setItem( i, 4, new QTableWidgetItem( _C2Q( "无效" ) ) );
			}
			else 
			{
				m_pData_table->setItem( i, 4, new QTableWidgetItem( _C2Q( "有效" ) ) );
			}

			procsec.tm_sec = yx_ptr->modifier.time.second ;
			procsec.tm_min = yx_ptr->modifier.time.minute ;
			procsec.tm_hour = yx_ptr->modifier.time.hour ;
			procsec.tm_mday = yx_ptr->modifier.time.day ;
			procsec.tm_mon = yx_ptr->modifier.time.month ;
			procsec.tm_year = yx_ptr->modifier.time.year ;
//			m_pData_table->setItem( i, 5, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
			char str1[ 128 ] ;
			sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
			m_pData_table->setItem( i, 5, new QTableWidgetItem( tr( str1 ) ) ) ;
			procmsec = yx_ptr->modifier.time.milisecond ;
			m_pData_table->setItem( i, 6, new QTableWidgetItem( QString::number( procmsec ) ) );

			yx_ptr++;
		}
	}
	else if ( m_table_type == 2 )
	{
		for ( int i = 0 ; i < m_fes_shm.rtu_para_ptr[ m_terminal_no ].kwh_num ; i++ )
		{
			kwhValue = kwh_ptr->raw_kwh ;
			m_pData_table->setItem( i, 2,  new QTableWidgetItem( QString::number( kwhValue ) ) );

			kwhValid = kwh_ptr->modifier.quality ;
			if ( ULONG_DEFAULT == kwhValue )
			{
				m_pData_table->setItem( i, 3, new QTableWidgetItem( _C2Q( "无效" ) ) );
			}
			else 
			{
				m_pData_table->setItem( i, 3, new QTableWidgetItem( _C2Q( "有效" ) ) );
			}

			procsec.tm_sec = kwh_ptr->modifier.time.second ;
			procsec.tm_min = kwh_ptr->modifier.time.minute ;
			procsec.tm_hour = kwh_ptr->modifier.time.hour ;
			procsec.tm_mday = kwh_ptr->modifier.time.day ;
			procsec.tm_mon = kwh_ptr->modifier.time.month ;
			procsec.tm_year = kwh_ptr->modifier.time.year ;
// 			m_pData_table->setItem( i, 4, new QTableWidgetItem( QString::number( mktime( &procsec ) ) ) );
			char str1[ 128 ] ;
			sprintf( str1 , "%04d/%02d/%02d/ %02d:%02d:%02d" , procsec.tm_year + 1900 , procsec.tm_mon , procsec.tm_mday , procsec.tm_hour , procsec.tm_min , procsec.tm_sec ) ;
			m_pData_table->setItem( i, 4, new QTableWidgetItem( tr( str1 ) ) ) ;
			procmsec = kwh_ptr->modifier.time.milisecond ;
			m_pData_table->setItem( i, 5, new QTableWidgetItem( QString::number( procmsec ) ) );

			kwh_ptr++;
		}
	}
}

void raw_data_widget::on_timer()
{
	fill_raw_value() ;
}

bool raw_data_widget::read_file( char *filename, void ** buf )
{
	struct stat sf;

	if( stat( filename, &sf )!=0 )
	{
		return false;
	}

	FILE *fp;
	int ret;

	if( sf.st_size == 0 )
		return true;

	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return false;
	*buf =  (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );

	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return false;
	}

	return true;
}

bool raw_data_widget::read_file( char *filename, void * buf, int len )
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

	return true;
}
