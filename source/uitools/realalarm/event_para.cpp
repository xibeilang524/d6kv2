// QEventPara.cpp: implementation of the QEventPara class.
//
//////////////////////////////////////////////////////////////////////

#include "event_para.h"
#include "globaldef.h"

#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BUFF_LEN 1024
char *EventSortName[] = 
{
	"未定义",
	"系统中的设备及程序运行事件",
	"SCADA事件",
	"综合自动化保护事件",
	"配电事件",
	"DA应用事件",
	"DA研究态应用事件",
	"EMS应用事件",
	"操作事件",
	"通知事件",
	"WEB应用事件",
	"维护类应用事件",
};


Event_para::Event_para()
{
// 	m_groupPara.setAutoDelete( true );
// 	m_rtuPara.setAutoDelete( true );
// 	m_ycPara.setAutoDelete( true );
// 	m_yxPara.setAutoDelete( true );
// 	m_eventTypePara.setAutoDelete( true );
// 	m_stationPara.setAutoDelete( true );
// 	m_devPara.setAutoDelete( true );
// 	m_devTypePara.setAutoDelete( true );

	m_rfile = FALSE;
}

Event_para::~Event_para()
{

}

bool Event_para::read_para()
{
	bool ret;

	printf( "readPara start\n" );
	fflush( stdout );

	if( !m_db.Open( DB_OP_R, FALSE, FALSE ) )
	{
		m_rfile = TRUE;
		//return FALSE;
	}

	printf( "readPara end\n" );
	fflush( stdout );

	if( !read_event_type() ||
		!read_group_para() ||
		!read_station_para() ||
		!read_area_para() ||
		!read_rtu_para() ||
		!read_yc_para() ||
		!read_yx_para() ||
		!read_dev_para() ||
		!read_dev_type_para() ||
		!read_report_name() ) 
	{
		ret = false;
	}
	else
	{
		ret = true;
	}

#ifndef _HISEVENT_VERSION
	if( !read_node_table() ||
		!read_confirm_event() )
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
#endif

	if( !m_rfile )
		m_db.Close();
	else
	{
		m_rfile = FALSE;
	}

	

	return ret;
}

uint Event_para::get_group_count()
{
	uint count;

	count = m_group_para.count();

	return count;
}

bool Event_para::get_group_desc( uint id, QString &desc )
{
	bool ret;


	PGROUPPARA pGroupPara = m_group_para.at( id );
	if( pGroupPara == NULL )
	{
		ret = false;
	}
	else
	{
		ret = true;
		desc = pGroupPara->desc;
	}


	return ret;
}

bool Event_para::get_group_no( uint id, uint &no )
{
	bool ret;


	PGROUPPARA pGroupPara = m_group_para.at( id );
	if( pGroupPara == NULL )
	{
		ret = false;
	}
	else
	{
		ret = true;
		no = pGroupPara->no;
	}


	return ret;
}

bool Event_para::get_station_code( uint groupno, QStringList &stations )
{
	bool ret = false;
	uint i;


	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_station_para.count(); i ++ )
	{
		pStaPara = m_station_para.at( i );
		if( groupno != pStaPara->groupno )
			continue;

		stations.append( pStaPara->code );
	}

	if( stations.count() > 0 )
		ret = true;

	return ret;
}


bool Event_para::get_station_desc( const QString &code, QString &desc )
{
	bool ret = false;
	uint i;


	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_station_para.count(); i ++ )
	{
		pStaPara = m_station_para.at( i );
		if( code == pStaPara->code )
		{
			desc = pStaPara->desc;
			ret = true;
			break;
		}
	}


	return ret;
}

bool Event_para::get_yc_code( const QString &rtucode, QStringList &yccode )
{
	bool ret = false;
	uint i;


	PYCPARA pYcPara;

	for( i = 0 ; i < m_yc_para.count(); i ++ )
	{
		pYcPara = m_yc_para.at( i );

		if( rtucode == pYcPara->stationcode )
		{
			yccode.append( pYcPara->code );
			ret = true;
		}
	}


	return ret;
}

bool Event_para::get_yc_desc( const QString &code, QString &desc )
{
	bool ret = false;
	YCPARA *pYcPara;

	if( m_yccode_to_ycpara_map.find( code ) != 
		m_yccode_to_ycpara_map.end() )
	{
		pYcPara = m_yccode_to_ycpara_map[ code ];
		desc = pYcPara->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::get_yx_code( const QString &rtucode, QStringList &yxcode )
{
	bool ret = false;
	uint i;

	PYXPARA pYxPara;

	for( i = 0 ; i < m_yx_para.count(); i ++ )
	{
		pYxPara = m_yx_para.at( i );

		if( rtucode == pYxPara->stationcode )
		{
			yxcode.append( pYxPara->code );
			ret = true;
		}
	}

	return ret;
}

bool Event_para::get_yx_desc( const QString &code, QString &desc )
{
	bool ret = false;
	YXPARA *pYxPara;

	if( m_yxcode_to_yxpara_map.find( code ) != 
		m_yxcode_to_yxpara_map.end() )
	{
		pYxPara = m_yxcode_to_yxpara_map[ code ];
		desc = pYxPara->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::read_group_para()
{
	m_group_para.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "组参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy( func.table, "组参数表" );
	strcpy( func.dbname, "modeldb" );
	char * col_name[] = { "序号", "代码", "描述" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/组参数表3/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/组参数表3/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/组参数表3/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if( m_db.GetTableInfo( &func,&datafmt ) == CS_FAIL )
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if( func.colnum < 1 || !datafmt )
		{
			if( datafmt ) 
				free( datafmt );
			
			m_db.Close();	
			return FALSE;
		}
		int   col_num = sizeof( col_name ) / sizeof( char* );
		
		for( i = 0; i < func.colnum ; i ++ )
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition = NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}
	

	PGROUPPARA pGroup;
	int count ;
	char	tmpbuf[ BUFF_LEN ];

    for( i = 0; i < func.ret_roxnum; i ++ )
	{	
		pGroup = new GROUPPARA;
		count = 0;
		pGroup->no = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->desc = QObject::tr( tmpbuf );
		m_group_para.append( pGroup );
    }


    if( func.roxnum != 0 ) 
		free( pt );

    if( datafmt ) 
		free( datafmt );

    return TRUE;
}

bool Event_para::read_station_para()
{
	m_station_stringlist.clear();
	m_station_para.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "站所线系参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"站所线系参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"代码","描述","分组序号","区域序号"};

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/站所线系参数表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/站所线系参数表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/站所线系参数表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		
		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
		
		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}
	
	

	int count;
	char tmpbuf[ BUFF_LEN ];
	PSTATIONPARA pSta;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		QStringList stationPara;		
		count = 0;
		pSta = new STATIONPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->code = QObject::tr( tmpbuf );
		stationPara.append(_C2Q( tmpbuf ));
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->desc = QObject::tr( tmpbuf );
		stationPara.append(_C2Q( tmpbuf ));
		pSta->groupno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		stationPara.append(QString::number( pSta->groupno ));
		pSta->areano = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		stationPara.append(QString::number( pSta->areano ));
		m_station_stringlist.append(stationPara);
		m_station_para.append( pSta );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}

QList<QStringList> Event_para::get_station_para()
{
	return m_station_stringlist ;
}


bool Event_para::read_area_para()
{	
	m_area_stringlist.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;


	printf( "STATIONAREA" );
	fflush( stdout );
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"STATIONAREA");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"ID","NAME","MASTERID"};

	datafmt = 0;
	void *pt=0;

	if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		m_db.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		m_db.Close();	
		return FALSE;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = NO;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=NO;
		for(j=0; j<col_num; j++)
		{
			if(strcmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = YES;
			func.col[i].order_d = j;
		}
		if(strcmp(datafmt[i].name,col_name[0])==0)
			func.col[i].order = 0;
	}
	func.func = SEL_RECORDERS;
	if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	if(func.ret_roxnum ==0)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PAREAPARA pAreaPara;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		QStringList areaList;
		count = 0;
		pAreaPara = new AREAPARA ;

		pAreaPara->areano = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		areaList.append(QString::number(pAreaPara->areano));

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pAreaPara->desc = _C2Q( tmpbuf );
		areaList.append(pAreaPara->desc);

		pAreaPara->masterareano = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		areaList.append(QString::number(pAreaPara->masterareano));

		m_area_stringlist.append(areaList);
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);
	return TRUE;
}

QList<QStringList> Event_para::get_area_para()
{
	return m_area_stringlist ;
}

bool Event_para::read_rtu_para()
{
	m_rtu_para.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;


	printf( "终端参数表" );
	fflush( stdout );
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"终端参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = {"代码","描述","终端序号","站所线系代码"};

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/终端参数表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/终端参数表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/终端参数表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PRTUPARA pRtu;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pRtu = new RTUPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->desc = QObject::tr( tmpbuf );
		pRtu->no = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pRtu->rtucode = QObject::tr( tmpbuf );

		m_rtuno_to_station_code[ pRtu->no ] = pRtu->rtucode;

		m_rtu_para.append( pRtu );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}


bool Event_para::read_yc_para()
{
	m_yc_para.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "遥测参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"遥测参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码","描述","类型","设备代码","终端序号" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/遥测参数表/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/遥测参数表/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/遥测参数表/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PYCPARA pYc;
	uint	rtuno;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pYc = new YCPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->desc = QObject::tr( tmpbuf );
		pYc->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYc->devcode = QObject::tr( tmpbuf );
		rtuno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		if( m_rtuno_to_station_code.find( rtuno ) != m_rtuno_to_station_code.end() )
		{
			pYc->stationcode = m_rtuno_to_station_code[ rtuno ];
		}
		m_yc_para.append( pYc );
		m_yccode_to_ycpara_map[ pYc->code ] = pYc;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool Event_para::read_yx_para()
{
	m_yx_para.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "遥信参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"遥信参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码","描述","类型","设备代码","终端序号" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/遥信参数表/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/遥信参数表/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/遥信参数表/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PYXPARA pYx;
	uint	rtuno;

	printf( "222222222222" );
	fflush( stdout );

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pYx = new YXPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->desc = QObject::tr( tmpbuf );
		pYx->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pYx->devcode = QObject::tr( tmpbuf );
		rtuno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		if( m_rtuno_to_station_code.find( rtuno ) != m_rtuno_to_station_code.end() )
		{
			pYx->stationcode = m_rtuno_to_station_code[ rtuno ];
		}

		m_yx_para.append( pYx );

		m_yxcode_to_yxpara_map[ pYx->code ] = pYx;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool Event_para::read_event_type()
{
	int i;

	printf( "事项类型表" );
	fflush( stdout );

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	char * col_name[] = { "分类号","类型号","类型名" };

	memset(&func,0,sizeof(FUNC_STR));
	for(int iOrder = 0 ; iOrder<MAX_COLNUM ;iOrder++)
	{
		func.col[iOrder].order = -1;
		func.col[iOrder].order_d = -1;
	}

	func.func=GET_TABLE_INFO;
//	func.serverno=SERVER_DEFAULT;
	strcpy(func.table,"事项类型表");
	strcpy(func.dbname,"modeldb");
	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/事项类型表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/事项类型表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/事项类型表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
	
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		if (func.colnum<1) 
		{
			free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		
		//func.recorderlen = 0;
		for ( i=0;i<func.colnum;i++)
		{
			func.col[i].select=NO;
			func.col[i].order=-1;
			func.col[i].condition=NO;
			func.col[i].order_d = -1;
		}

		for (i=0;i<func.colnum;i++)
		{
			if(strcmp(datafmt[ i ].name,"分类号")==0)
			{
				func.col[i].select=YES;
				func.col[i].order_d = 0;
			}
			else if(strcmp(datafmt[ i ].name,"类型号")==0) 
			{
				func.col[i].select=YES;
				func.col[i].order=0;
				func.col[i].order_d = 1;
			}
			else if(strcmp(datafmt[ i ].name,"类型名")==0) 
			{
				func.col[i].select=YES;
				func.col[i].order_d = 2;
			}
		}

		func.func=SEL_RECORDERS;
		
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	PEVENTTYPEDEF pEventTypeDef;
	int count;
	char tmpbuf[ BUFF_LEN ];

	for( i = 0; i < func.ret_roxnum; i ++ )
	{
		count = 0;
		pEventTypeDef = new EVENTTYPEDEF ;
		pEventTypeDef->sort = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pEventTypeDef->type = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		m_event_type_para.append( pEventTypeDef );

		m_type_desc_map[ pEventTypeDef->type ] = QObject::tr( tmpbuf );
		m_desc_type_map[ QObject::tr( tmpbuf ) ] = pEventTypeDef->type;
	}

	if(datafmt)
		free(datafmt);
	return TRUE;
}

uint Event_para::get_event_sort_count()
{
	return EVENT_SORT_NUM;
}

bool Event_para::get_event_sort_desc( uint sort, QString &desc )
{
	bool ret ;

	if( sort >= EVENT_SORT_NUM )
	{
		ret = false;
	}
	else
	{
		desc = QObject::tr( EventSortName[ sort ] );
		ret = true;
	}

	return ret;
}

uint Event_para::get_event_type_count()
{
	uint count;
	count = m_event_type_para.count();

	return count;
}

bool Event_para::get_event_type_desc( uint type, QString &desc )
{
	bool ret = false;


	if( m_type_desc_map.find( type ) != m_type_desc_map.end() )
	{
		desc = m_type_desc_map[ type ];
		ret = true;
	}

	return ret;
}

bool Event_para::get_event_types( uint sort, uint *types, uint &num )
{

	PEVENTTYPEDEF pEventTypeDef;

	num = 0;


	for( uint i = 0 ; i < m_event_type_para.count(); i ++ )
	{
		pEventTypeDef = m_event_type_para.at( i );

		if( pEventTypeDef->sort == sort )
		{
			types[ num ++ ] = pEventTypeDef->type;
		}
	}

	
	if( num == 0 )
		return false;

	return true;
}

PEVENTTYPEDEF Event_para::get_event_type( uint id )
{
	PEVENTTYPEDEF pEventType;

	pEventType = m_event_type_para.at( id );

	return pEventType;
}

uint Event_para::get_event_type_by_desc( QString &desc )
{
	if( m_desc_type_map.find( desc ) == m_desc_type_map.end() )
		return 0;

	return m_desc_type_map[ desc ];
}

bool Event_para::read_dev_para()
{
	m_dev_para.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "SCD设备参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD设备参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码","描述","站所线系代码", "类型" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/SCD设备参数表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/SCD设备参数表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/SCD设备参数表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVPARA pDev;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDev = new DEVPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->code = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->desc = QObject::tr( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->stationcode = QObject::tr( tmpbuf );
		pDev->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		
		m_devcode_to_devpara_map[ pDev->code ] = pDev;
		m_dev_para.append( pDev );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool Event_para::get_dev_code( const QString &rtucode, QStringList &devcode )
{
	bool ret = false;

	PDEVPARA pDevPara;

	for( uint i = 0 ; i < m_dev_para.count(); i ++ )
	{
		pDevPara = m_dev_para.at( i );

		if( rtucode == pDevPara->stationcode )
		{
			devcode.append( pDevPara->code );
			ret = true;
		}
	}


	return ret;
}

bool Event_para::get_dev_code( const QString &rtucode, uint devtype, QStringList &devcode )
{
	bool ret = false;

	PDEVPARA pDevPara;

	for( uint i = 0 ; i < m_dev_para.count(); i ++ )
	{
		pDevPara = m_dev_para.at( i );

		if( rtucode == pDevPara->stationcode && devtype == pDevPara->type )
		{
			devcode.append( pDevPara->code );
			ret = true;
		}
	}

	return ret;
}

bool Event_para::read_dev_type_para()
{
	m_dev_type_para.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "SCD设备类型表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"SCD设备类型表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "类型","描述" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/SCD设备类型表/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/SCD设备类型表/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/SCD设备类型表/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVTYPEPARA pDevType;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDevType = new DEVTYPEPARA ;
		pDevType->devtype = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDevType->desc = QObject::tr( tmpbuf );
		
		m_dev_type_para.append( pDevType );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

uint Event_para::get_dev_type_num()
{
	return m_dev_type_para.count();
}

bool Event_para::get_dev_type( uint id, PDEVTYPEPARA devtype )
{
	bool ret  = false;

	if( id >= m_dev_type_para.count() )
		return false;

	devtype->desc = m_dev_type_para.at( id )->desc;
	devtype->devtype = m_dev_type_para.at( id )->devtype;

	return true;
}

bool Event_para::get_dev_desc( const QString &devcode, QString &devdesc )
{
	bool ret = false;
	DEVPARA *pDev;

	if( m_devcode_to_devpara_map.find( devcode ) != 
		m_devcode_to_devpara_map.end() )
	{
		pDev = m_devcode_to_devpara_map[ devcode ];
		devdesc = pDev->desc;
		ret = true;
	}

	return ret;
}

bool Event_para::get_dev_real_code( const QString &devcode, QStringList &codes )
{
	bool ret = false;

	PYCPARA pYcPara;
	uint i;

	for( i = 0 ; i < m_yc_para.count(); i ++ )
	{
		pYcPara = m_yc_para.at( i );

		if( devcode == pYcPara->devcode )
		{
			codes.append( pYcPara->code );
			ret = true;
		}
	}

	PYXPARA pYxPara;

	for( i = 0 ; i < m_yx_para.count(); i ++ )
	{
		pYxPara = m_yx_para.at( i );

		if( devcode == pYxPara->devcode )
		{
			codes.append( pYxPara->code );
			ret = true;
		}
	}
	return ret;
}

PDEVPARA Event_para::get_dev( const QString &devcode )
{
	if( !m_devcode_to_devpara_map.contains( devcode ) )
		return NULL;

	return m_devcode_to_devpara_map[ devcode ];
}

bool Event_para::read_node_table()
{
	m_host_ip_list.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	NetStatus		netstatus;

	QString			hostname;

	hostname = QObject::tr( netstatus.GetHostName( netstatus.GetHostNo() ) ) ;

	printf( "网络节点参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"网络节点参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "主机名","A网IP地址","B网IP地址" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/网络节点参数表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/网络节点参数表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/网络节点参数表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for( i = 0; i < func.colnum; i ++ )
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcasecmp(datafmt[i].name,col_name[j]))	
					continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	char tmpbuf[ BUFF_LEN ];

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );

		if( hostname == QObject::tr( tmpbuf ) )
		{
			m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
			m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
			continue;
		}

		//printf( "hostname=%s", tmpbuf );

		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		if( !QObject::tr( tmpbuf ).isEmpty() )
		{
			m_host_ip_list.append( QObject::tr( tmpbuf ) );
			//printf( "A IP=%s", tmpbuf );
		}
		
		tmpbuf[ 0 ] = '\0';
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		if( !QObject::tr( tmpbuf ).isEmpty() )
		{
			m_host_ip_list.append( QObject::tr( tmpbuf ) );
			//printf( "B IP=%s\n", tmpbuf );
		}

		//fflush( stdout );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

uint Event_para::get_ip_num()
{
	return m_host_ip_list.count();
}

bool Event_para::get_ip_by_id( uint id, QString &ip )
{
	if( id >= m_host_ip_list.count() )
		return false;

	ip = m_host_ip_list[ id ];

	return true;
}

bool Event_para::read_confirm_event()
{
	m_confirm_event_type.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "事项确认类型表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"事项确认类型表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "类型号" };

	datafmt = 0;
	void *pt=0;
	char filename[ 256 ];
	if( m_rfile )
	{
		sprintf( filename, "%s/dbmap/事项确认类型表1/datafmt", getenv( NBENV ) );
		if( !read_file(filename, (void**)(&datafmt) ) )
			return FALSE;
		
		sprintf( filename, "%s/dbmap/事项确认类型表1/func", getenv( NBENV ) );
		if( !read_file(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;
		
		m_db.GetTableInfo( &func, NULL, NULL );
		
		sprintf( filename, "%s/dbmap/事项确认类型表1/pt", getenv( NBENV ) );
		if( !read_file(filename, &pt ) )
			return FALSE;
	}
	else
	{
		if(m_db.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			m_db.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			m_db.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
		}
		func.func = SEL_RECORDERS;
		if((retcode = m_db.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}

		if(func.ret_roxnum ==0)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			m_db.Close();	
			return FALSE;
		}
	}

	int count;
	uint	type;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		type = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_confirm_event_type.append( type );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool Event_para::m_bconfirm_event_type( uint type )
{
// 	if( m_confirmEventType.find( type ) != m_confirmEventType.end() )
	if( m_confirm_event_type.indexOf( type ) != -1 )
		return true;

	return false;
}

bool Event_para::get_yx_type( const QString &code, uint &type )
{
	bool ret = false;
	YXPARA *pYxPara;

	if( m_yxcode_to_yxpara_map.find( code ) != m_yxcode_to_yxpara_map.end() )
	{
		pYxPara = m_yxcode_to_yxpara_map[ code ];
		type = pYxPara->type;
		ret = true;
	}

	return ret;
}

bool Event_para::read_file( char *filename, void ** buf )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}
	
	FILE *fp;
	int ret;
	
	if( sf.st_size == 0 )
		return TRUE;
	
	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	*buf =  (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );
	
	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}
	
	return TRUE;
	
}

bool Event_para::read_file( char *filename, void * buf, int len )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}
	
	FILE *fp;
	int ret;
	
	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	ret = fread( buf, len, 1, fp );
	
	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}
	
	return TRUE;
}

bool Event_para::read_report_name()
{
	FUNC_STR func ;
	CS_DATAFMT *datafmt=NULL ;
	func.func = GET_TABLE_INFO ;
	int col_rptname , col_rpttype , col_webid , ptcnt ;

	strcpy( func.table,"webrptname" ) ;
	strcpy( func.dbname,"modeldb" ) ;
	if( m_db.GetTableInfo( &func,&datafmt ) == CS_FAIL )	
	{
		return false ;
	}
	m_report_name_list.clear() ;
	for ( int i=0 ; i<func.colnum ; i++ )
	{
		func.col[ i ].condition = FALSE ;
		func.col[ i ].select = FALSE ;
		func.col[ i ].order  = -1 ;
		func.col[ i ].order_d = -1 ;
		if( strcmp( datafmt[ i ].name,"RPTNAME" ) == 0  )
		{
			func.col[ i ].select = TRUE ;
			func.col[ i ].order_d = 0 ;
			col_rptname = i ;
		}
		if( strcmp( datafmt[ i ].name,"RPTTYPE" ) == 0  )
		{
			func.col[ i ].select = TRUE ;
			func.col[ i ].order_d = 1 ;
			col_rpttype = i ;
		}
		if( strcmp( datafmt[ i ].name,"WEBID" )==0  )
		{
			func.col[ i ].select = TRUE ;
			func.col[ i ].order_d = 2 ;
			col_webid = i ;
		}

	}
	void *pt = NULL ;
	func.func=SEL_RECORDERS ;

	if ( ( m_db.SelectRcds( &func,datafmt,&pt ) ) != CS_SUCCEED )
		return false ;
	int reportCount = ( int )func.ret_roxnum ;
	m_report_count = reportCount ;
	if ( reportCount == 0 )
		return false ;
	m_report_para = new REPORTNAMEPARA[ reportCount ] ;
	memset( m_report_para , 0 , sizeof( reportCount * sizeof( REPORTNAMEPARA ) ) ) ;
	for( int i = 0 ; i < reportCount ; i++ )
	{
		m_db.GetData( i , &func , datafmt+col_rptname , pt , ptcnt , m_report_para[ i ].rptname ) ;			//取报表名
		m_db.GetData( i , &func , datafmt+col_rpttype , pt , ptcnt , m_report_para[ i ].rpttype ) ;				//取报表类型
		m_report_para[ i ].webid = ( uint )m_db.GetData( i , &func , datafmt+col_webid , pt , ptcnt , NULL ) ;					//取ID
		m_reportname_to_webid.insert( _C2Q( m_report_para[ i ].rptname ) , m_report_para[ i ].webid ) ;
		m_report_name_list << _C2Q( m_report_para[ i ].rptname ) ;
	}

	if ( pt ) 
	{
		free( pt ) ;
		pt=NULL ;
	}

	if ( datafmt ) 
	{
		free( datafmt ) ;
		datafmt=NULL ;
	}

	return true ;
}

int Event_para::get_report_count()
{
	return m_report_count ;
}

QStringList Event_para::get_report_name()
{
	return m_report_name_list ;
}

bool Event_para::get_report_Id( QString name ,int& id )
{
	bool ret = false ;

	if( m_reportname_to_webid.find( name ) != 
		m_reportname_to_webid.end() )
	{
		id = m_reportname_to_webid[ name ];
		ret = true;
	}
	return ret;
}