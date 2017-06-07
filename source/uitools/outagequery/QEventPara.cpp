// QEventPara.cpp: implementation of the QEventPara class.
//
//////////////////////////////////////////////////////////////////////

#include "QEventPara.h"
#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BUFF_LEN 1024

#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

QEventPara::QEventPara()
{

}

QEventPara::~QEventPara()
{
	qDeleteAll(m_groupPara) ;
	qDeleteAll(m_stationPara) ;
	qDeleteAll(m_devPara) ;
	qDeleteAll(m_devTypePara) ;
	qDeleteAll(m_feederLinePara) ;
	qDeleteAll(m_planCutPara) ;
	qDeleteAll(m_belongsLoad2userInfoMap) ;
}
bool QEventPara::readPara()
{
	bool ret;

	printf( "readPara start\n" );
	fflush( stdout );

	if( !m_db.Open( DB_OP_R, FALSE, FALSE ) )
	{
		return FALSE;
	}

	printf( "readPara end\n" );
	fflush( stdout );

	if( !readGroupPara() || !readStationPara()  ||!readDevPara() ||!readDevTypePara() ||!readFeederLinePara()
		||!readPlanCutPara() || !readAreaPara() || !readUserPara() ) 
	{
		ret = false;
	}
	else
	{
		ret = true;
	}

	m_db.Close();

	return ret;
}

uint QEventPara::getGroupCount()
{
	uint count;

	count = m_groupPara.count();

	return count;
}

bool QEventPara::getGroupDesc( uint id, QString &desc )
{
	bool ret;


	PGROUPPARA pGroupPara = m_groupPara.at( id );
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

bool QEventPara::getGroupNo( uint id, uint &no )
{
	bool ret;


	PGROUPPARA pGroupPara = m_groupPara.at( id );
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

bool QEventPara::getStaCode( uint groupno, QStringList &stations )
{
	bool ret = false;
	uint i;


	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_stationPara.count(); i ++ )
	{
		pStaPara = m_stationPara.at( i );
		if( groupno != pStaPara->groupno )
			continue;

		stations.append( pStaPara->code );
	}

	if( stations.count() > 0 )
		ret = true;

	return ret;
}


bool QEventPara::getStaDesc( const QString &code, QString &desc )
{
	bool ret = false;
	uint i;


	PSTATIONPARA pStaPara;

	for( i = 0 ; i < m_stationPara.count(); i ++ )
	{
		pStaPara = m_stationPara.at( i );
		if( code == pStaPara->code )
		{
			desc = pStaPara->desc;
			ret = true;
			break;
		}
	}
	return ret;
}

bool QEventPara::readGroupPara()
{
	qDeleteAll(m_groupPara) ;
	m_groupPara.clear();

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

	PGROUPPARA pGroup;
	int count ;
	char	tmpbuf[ BUFF_LEN ];

    for( i = 0; i < func.ret_roxnum; i ++ )
	{	
		pGroup = new GROUPPARA;
		count = 0;
		pGroup->no = (int)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pGroup->desc = _C2Q( tmpbuf );
		m_groupPara.append( pGroup );
    }
    if( func.roxnum != 0 ) 
		free( pt );

    if( datafmt ) 
		free( datafmt );

    return TRUE;
}

bool QEventPara::readStationPara()
{
	qDeleteAll(m_stationPara);
	m_stationPara.clear();
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
	char * col_name[] = {"代码","描述","分组序号"};

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
		if ( strcasecmp( datafmt[i].name,"代码") == 0 )
		{
			func.col[i].order = 0 ;
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
	int count;
	char tmpbuf[ BUFF_LEN ];
	PSTATIONPARA pSta;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pSta = new STATIONPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pSta->desc = _C2Q( tmpbuf );
		pSta->groupno = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_stationPara.append( pSta );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	return TRUE;
}

bool QEventPara::readDevPara()
{
	qDeleteAll(m_devPara);
	m_devPara.clear();

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

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVPARA pDev;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDev = new DEVPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->desc = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDev->stationcode = _C2Q( tmpbuf );
		pDev->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		
		m_devCode2DevParaMap[ pDev->code ] = pDev;
		m_devPara.append( pDev );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::readDevTypePara()
{
	qDeleteAll(m_devTypePara);
	m_devTypePara.clear();

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

	int count;
	char tmpbuf[ BUFF_LEN ];
	PDEVTYPEPARA pDevType;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pDevType = new DEVTYPEPARA ;
		pDevType->devtype = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pDevType->desc = _C2Q( tmpbuf );

		m_devTypeNo2DevTypeMap[ pDevType->devtype ] = pDevType;
  		m_devTypeDesc2DevTypeMap[ pDevType->desc ] = pDevType;
		m_devTypePara.append( pDevType );
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::readFeederLinePara()
{
	qDeleteAll(m_feederLinePara);
	m_feederLinePara.clear();
	m_feederLineNo2FeederLineMap.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "馈线参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"馈线参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "序号", "代码","描述" };

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
		if ( strcasecmp( datafmt[i].name,"序号") == 0 )
		{
			func.col[i].order = 0 ;
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
	int count;
	char tmpbuf[ BUFF_LEN ];
	PFEEDERLINEPARA pFeederLine;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pFeederLine = new FEEDERLINEPARA ;
		pFeederLine->no = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pFeederLine->name = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pFeederLine->desc = _C2Q( tmpbuf );

		m_feederLineNo2FeederLineMap[ pFeederLine->no ] = pFeederLine;
		m_feederLinePara.append( pFeederLine );
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::fineDevByFeederLine( QStringList& devList , QStringList& feederLineList )
{
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
	char * col_name[] = { "代码" };

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
	}

	char where[ 2048 ] ;
	char tmpstr[ 1024 ] ;
	for ( i = 0 ; i < feederLineList.count() ; i++ )
	{
		if( i == 0 )
		{
			sprintf( where , "所属馈线 = '%s'", _Q2C( feederLineList.at( i ) ) ) ;
		}
		else
		{
			sprintf( tmpstr, " or 所属馈线 = '%s'",  _Q2C( feederLineList.at( i ) ) );
			strcat( where, tmpstr );
		}
	}

	func.func = SEL_ISQL_RECORDERS;
	strcpy( func.isql , where ) ;
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
	
	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0 ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		devList.append( _C2Q( tmpbuf ) ) ;
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::readPlanCutPara()
{
	qDeleteAll(m_planCutPara);
	m_planCutPara.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "计划停电参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"计划停电参数表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码","描述","停电开始年月日","停电开始时分秒","停电结束年月日","停电结束时分秒"};

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
		if ( strcasecmp( datafmt[i].name,"代码") == 0 )
		{
			func.col[i].order = 0 ;
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
	int count;
	char tmpbuf[ BUFF_LEN ];
	PPLANCUTPARA pPlanCut;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pPlanCut = new PLANCUTPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pPlanCut->code = _C2Q(tmpbuf);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pPlanCut->desc = _C2Q( tmpbuf );
		pPlanCut->beginYmd = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pPlanCut->beginHms = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pPlanCut->endYmd = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		pPlanCut->endHms = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);

		m_planCutPara.append(pPlanCut);
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::readAreaPara()
{
	m_areaName2descMap.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "配电区域信息表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"配电区域信息表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码","描述" };

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
		if ( strcasecmp( datafmt[i].name,"代码") == 0 )
		{
			func.col[i].order = 0 ;
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
	int count;
	char tmpbuf[ BUFF_LEN ];
	PAREAINFOPARA pAreaInfo;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pAreaInfo = new AREAINFOPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pAreaInfo->code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pAreaInfo->desc = _C2Q( tmpbuf );
		
		m_areaName2descMap[pAreaInfo->code] = pAreaInfo->desc;
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);

	return TRUE;
}

bool QEventPara::readUserPara()
{

	qDeleteAll(m_belongsLoad2userInfoMap);
	m_belongsLoad2userInfoMap.clear();

	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	printf( "配电客户信息表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"配电客户信息表");
	strcpy(func.dbname,"modeldb");
	char * col_name[] = { "代码", "描述","类型","所属负荷","所属区域" };

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
		if ( strcasecmp( datafmt[i].name,"代码") == 0 )
		{
			func.col[i].order = 0 ;
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
	int count;
	char tmpbuf[ BUFF_LEN ];
	PUSERINFOPARA pUserInfo;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pUserInfo = new USERINFOPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUserInfo->code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUserInfo->desc = _C2Q( tmpbuf );
		pUserInfo->type = (uint)m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUserInfo->belongsLoad = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUserInfo->belongsArea = _C2Q( tmpbuf );
		
		m_belongsLoad2userInfoMap[pUserInfo->belongsLoad] = pUserInfo;
	}
	if (func.roxnum != 0) free(pt);
	if (datafmt) free(datafmt);

	return TRUE;
}

uint QEventPara::getDevTypeNum()
{
	return m_devTypePara.count();
}

bool QEventPara::getDevType( uint id, PDEVTYPEPARA devtype )
{
	bool ret  = false;

	if( id >= m_devTypePara.count() )
		return false;

	devtype->desc = m_devTypePara.at( id )->desc;
	devtype->devtype = m_devTypePara.at( id )->devtype;

	return true;
}

bool QEventPara::getDevDesc( const QString &devcode, QString &devdesc )
{
	bool ret = false;
	DEVPARA *pDev;

	if( m_devCode2DevParaMap.find( devcode ) != 
		m_devCode2DevParaMap.end() )
	{
		pDev = m_devCode2DevParaMap[ devcode ];
		devdesc = pDev->desc;
		ret = true;
	}

	return ret;
}

bool QEventPara::getFeederLineDesc( uint &feederNo, QString &feederDesc )
{
	bool ret = false;
	PFEEDERLINEPARA pFeederLine;
	for (int i = 0; i < m_feederLinePara.count(); i++)
	{
		pFeederLine = m_feederLinePara.at(i);
		if(feederNo == pFeederLine->no)
		{
			feederDesc = pFeederLine->desc;
			ret = true;
			break;
		}
	}
	return ret;
}
bool QEventPara::getDevTypeNo( uint& devType, QString desc )
{
	bool ret = false;
	DEVTYPEPARA *pDevTypePara;

	if( m_devTypeDesc2DevTypeMap.find( desc ) != 
		m_devTypeDesc2DevTypeMap.end() )
	{
		pDevTypePara = m_devTypeDesc2DevTypeMap[ desc ];
		devType = pDevTypePara->devtype;
		ret = true;
	}

	return ret;
}

bool QEventPara::getDevTypeDesc( uint devType, QString &desc )
{
	bool ret = false;
	DEVTYPEPARA *pDevTypePara;

	if( m_devTypeNo2DevTypeMap.find( devType ) != 
		m_devTypeNo2DevTypeMap.end() )
	{
		pDevTypePara = m_devTypeNo2DevTypeMap[ devType ];
		desc = pDevTypePara->desc;
		ret = true;
	}

	return ret;
}

bool QEventPara::getDevTypeDescs( QStringList &devtypedescs )
{
	bool ret = false;
	uint i;

	PDEVTYPEPARA pDevTypePara;
	for( i = 0 ; i < m_devTypePara.count(); i ++ )
	{
		pDevTypePara = m_devTypePara.at( i );
		if (pDevTypePara->devtype != 0)
		{
			devtypedescs.append( pDevTypePara->desc);
		}
	}
	if( devtypedescs.count() > 0 )
		ret = true;

	return ret;
}