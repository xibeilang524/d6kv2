// QMangerPara.cpp: implementation of the QMangerPara class.
//
//////////////////////////////////////////////////////////////////////

#include "QZonePara.h"
#include "globaldef.h"
#include "sysdef.h"
#include <qdebug.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern NetDbg netdbg;


QMangerPara::QMangerPara()
{
// 	m_roleList.setAutoDelete( true );
// 	m_userList.setAutoDelete( true );
// 	m_devList.setAutoDelete( true );
// 	m_devTypeList.setAutoDelete( true );
}

QMangerPara::~QMangerPara()
{
}

bool QMangerPara::readPara()	// modify [4/10/2012 zhoujian]
{
	bool ret;
	
	printf( "readPara start\n" );
	fflush( stdout );
	
	if( m_db.Open( DB_OP_R, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	printf( "readPara end\n" );
	fflush( stdout );
	
	if( !readRolePara() ||
		!readNodeTable() ||
		!readUserPara() ||
		!readUserCfgPara())
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

bool QMangerPara::savePara()
{
	bool ret;
	
	printf( "savePara start\n" );
	fflush( stdout );
	
	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	printf( "readPara end\n" );
	fflush( stdout );

	if( !saveUserPara()  ||
		!saveRolePara() )
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

bool QMangerPara::readRolePara()	// add [4/9/2012 zhoujian]
{
	m_roleList.clear();
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	char tmpbuf[256];
	PNODEPARA pNode;
	int count = 0;


	printf( "角色权限参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"角色权限参数表");

	strcpy(func.dbname, DB_NAME );
	char * usrcolname[] = {"角色名", "操作员类型","修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};

	datafmt=0;
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

	int   col_num = sizeof(usrcolname)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,usrcolname[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
			
			if(strcasecmp(datafmt[i].name,"操作员类型") == 0 )
			{
				func.col[i].order = 0;
			}
		}
	}

	func.func = SEL_RECORDERS;
	void *pt=0;
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

	uint zone;
	
    for( i = 0; i < func.ret_roxnum; i++)
	{	
		count = 0;
		pNode = new NODEPARA;
		pNode->zone1 = 0;
		pNode->zone2 = 0;
		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
		pNode->sname = _C2Q( tmpbuf );
// 		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
// 		pNode->code = _C2Q( tmpbuf );	//int dec = str.toInt(&ok, 10);
	
		pNode->code = pNode->sname;   //libinghong 2012-04-16
		pNode->devtype = m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,NULL);

		for( j = 0; j < 32; j ++ )
		{
			zone = m_db.GetData(i,&func,datafmt,usrcolname[count+j],pt,ptcnt,NULL);
			if( zone )
			{
				pNode->zone1 |= 0x01 << j;
			}
		}		
		pNode->oldzone1 = pNode->zone1;

		for (j=32;j<42;j ++)
		{
			int nmv = j-32;
			zone = m_db.GetData(i,&func,datafmt,usrcolname[count+j],pt,ptcnt,NULL);
			if( zone )
			{
				pNode->zone2 |= 0x01 << nmv;
			}
		}
		pNode->oldzone2 = pNode->zone2;
		
// 		if( !usermap.contains( pNode->code ) )
// 		{
// 			delete pNode;
// 		}
// 		else
// 		{
// 			pNode->desc = usermap[ pNode->code ];
// 		}
		m_roleList.append(pNode);
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);
	
	return TRUE;
}

bool QMangerPara::readNodeTable()
{
	int i,j;

	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;

	NetStatus		netstatus;

	QString			hostname;

	hostname = _C2Q( netstatus.GetHostName( netstatus.GetHostNo() ) ) ;

	printf( "网络节点参数表" );
	fflush( stdout );

	func.func = GET_TABLE_INFO;
	strcpy( func.table, "网络节点参数表" );
	strcpy( func.dbname, DB_NAME );
	char * col_name[] = { "主机名", "节点名" };

	datafmt=0;
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
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if( strcasecmp(datafmt[i].name,col_name[j]) )
				continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;	
		}

		if( !strcasecmp( datafmt[i].name, "no" ) )
		{
			func.col[i].order = 0;
		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
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
	char tmpbuf[ 256 ];
	QString desc, code;

    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		code = _C2Q( tmpbuf );

		m_db.GetData( i, count ++, &func, datafmt, pt, ptcnt, tmpbuf );
		desc = _C2Q( tmpbuf );
		
		m_hostmap[ code ] = desc;
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	return TRUE;
}

// *****ljs [4/11/2012]
bool QMangerPara::readUserCfgPara()
{
	m_usercfgList.clear();
	
	int i,j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "用户口令参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"用户口令参数表");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = { "操作员","描述","密码","类别描述","操作员类别","组长"};
	
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
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
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
	char tmpbuf[ 256 ];
	PUSERPARA pUser;
	QMap< QString, QString > usermap;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;		
		pUser = new USERPARA ;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUser->code = _C2Q( tmpbuf );

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUser->desc = _C2Q( tmpbuf );


		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUser->password = _C2Q( tmpbuf );

		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		pUser->roledesc = _C2Q( tmpbuf );

		pUser->roletype = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);

		pUser->groupleader = m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		
		m_usercfgList.append(pUser);


    }
    if (func.roxnum != 0) 
	{
		free(pt);
		pt = 0;
	}
	
    if (datafmt) 
	{
		free(datafmt);
		datafmt = NULL;
	}

	printf("%d",m_usercfgList.count());
	return TRUE;
}


bool QMangerPara::readUserPara()		// modify [4/10/2012 zhoujian]
{
	m_userList.clear();
	int i,j;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt = 0;
	
	printf( "用户口令参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy(func.table,"用户口令参数表");
	strcpy(func.dbname, DB_NAME );
	char * col_name[] = { "操作员","描述" };
	
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
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
	}
	func.func = SEL_RECORDERS;
	void *pt=0;
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
	char tmpbuf[ 256 ];
	PNODEPARA pNode;
	QString code, desc;

	QMap< QString, QString > usermap;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		code = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		desc = _C2Q( tmpbuf );

		usermap[ code ] = desc;
    }
    if (func.roxnum != 0) 
	{
		free(pt);
		pt = 0;
	}

    if (datafmt) 
	{
		free(datafmt);
		datafmt = NULL;
	}

	func.func = GET_TABLE_INFO;
	strcpy(func.table,"节点权限参数表");
	strcpy(func.dbname, DB_NAME );
	char * usrcolname[] = {"主机名", "操作员","修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	
	datafmt=0;
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
	
	col_num = sizeof(usrcolname)/sizeof(char*);
	for(i=0;i<func.colnum;i++)
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition = FALSE;
		for(j=0; j<col_num; j++)
		{
			if(strcasecmp(datafmt[i].name,usrcolname[j]))	continue;
			func.col[i].select = TRUE;
			func.col[i].order_d = j;
		}
	}
	
	func.func = SEL_RECORDERS;
	pt=0;
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
	
	uint zone;
	
    for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		pNode = new NODEPARA;
		pNode->zone1 = 0;
		pNode->zone2 = 0;
		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
		pNode->sname = _C2Q( tmpbuf );
		m_db.GetData(i,&func,datafmt,usrcolname[count++],pt,ptcnt,tmpbuf);
		pNode->code = _C2Q( tmpbuf );
//		qDebug() << pNode->sname << pNode->code;
		for( j = 0; j < 32; j ++ )
		{
			zone = m_db.GetData(i,&func,datafmt,usrcolname[count+j],pt,ptcnt,NULL);
			if( zone )
			{
				pNode->zone1 |= 0x01 << j;
			}
		}		
		pNode->oldzone1 = pNode->zone1;
		
		for (j=32;j<42;j ++)
		{
			int nmv = j-32;
			zone = m_db.GetData(i,&func,datafmt,usrcolname[count+j],pt,ptcnt,NULL);
			if( zone )
			{
				pNode->zone2 |= 0x01 << nmv;
			}
		}
		pNode->oldzone2 = pNode->zone2;

		if( !usermap.contains( pNode->code ) )
		{
			delete pNode;
		}
		else
		{
			pNode->desc = usermap[ pNode->code ];
			m_userList.append( pNode );
		}
    }
    if (func.roxnum != 0) free(pt);
    if (datafmt) free(datafmt);

	int rettemp = m_userList.count();
	
	return TRUE;
}

bool QMangerPara::saveUserPara()	// modify [4/10/2012 zhoujian]
{
	int i,j;
	char * usrcolname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PNODEPARA		pNode;
	QList<NODEPARA *> nodeList;
	void *pt = NULL;
	int keyid1, keyid2;
	int valueid[ 42 ];
	int nctl=0;

//	nodeList.setAutoDelete( false );
	
	ptcnt = 0;
	nodeList.clear();
	

	for( j = 0; j < m_userList.count(); j ++ )
	{
		pNode = m_userList.at( j );
		if( pNode->oldzone1 != pNode->zone1 || pNode->oldzone2 != pNode->zone2)
		{
			nodeList.append( pNode );
			pNode->oldzone1 = pNode->zone1;
			pNode->oldzone2 = pNode->zone2;
		}
	}

	if( nodeList.count() == 0 )
		return TRUE;

	printf( "%s", "节点权限参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "节点权限参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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

	for( i = 0; i < 42; i ++ )
	{
		valueid[ i ] = 0;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "主机名" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid1 = j;
		}

		if( !strcasecmp( datafmt[j].name, "操作员" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid2 = j;
		}
		if (nctl <42)
		{
			if( strstr( datafmt[j].name, usrcolname[nctl] ) )
			{
				func.col[j].select = TRUE;
				func.col[j].order_d = 1;
				valueid[ nctl ] = j;
				nctl++;
			}
		}
	}
	func.func = UPD_RECORDERS;
	func.roxnum = nodeList.count();
	udword zone;
	char isset;
	
	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0, func.recorderlen * func.roxnum);
	for( j = 0; j < nodeList.count(); j ++ )
	{
		pNode = nodeList.at( j );
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(_Q2C( pNode->sname ) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(_Q2C( pNode->code ) ) );
		for( i = 0; i < 32; i ++ )
		{
			zone = ( pNode->zone1 >> i ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		for (i = 32; i < 42; i++)
		{
			int nmv = i - 32;
			zone = ( pNode->zone2 >> nmv ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
	}
	
	if((retcode = m_db.UpdateRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	return TRUE;
}

bool QMangerPara::saveRolePara()
{
	int i,j;

	char * usrcolname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PNODEPARA		pNode;
	QList<NODEPARA *> nodeList;
	void *pt = NULL;
	int keyid1, keyid2;
	int valueid[ 42 ];
	int nctl=0;

//	nodeList.setAutoDelete( false );
	
	ptcnt = 0;
	nodeList.clear();
	

	for(int j = 0; j < m_roleList.count(); j ++ )
	{
		pNode = m_roleList.at( j );
			qDebug() << pNode->sname <<pNode->code;
		if( pNode->oldzone1 != pNode->zone1 || pNode->oldzone2 != pNode->zone2 || pNode->code !=pNode->sname)
		{
			pNode->code = pNode->sname;
//			qDebug() << pNode->sname <<pNode->code;
			nodeList.append( pNode );
			pNode->oldzone1 = pNode->zone1;
			pNode->oldzone2 = pNode->zone2;
		}

	}

	if( nodeList.count() == 0 )
		return TRUE;

	printf( "%s", "角色权限参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "角色权限参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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

	for( i = 0; i < 42; i ++ )
	{
		valueid[ i ] = 0;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "角色名" ) )
		{
//			func.col[j].condition = FALSE;
			func.col[j].select = TRUE;
			func.col[j].order_d = 1;
			keyid1 = j;
		}

		if( !strcasecmp( datafmt[j].name, "操作员类型" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid2 = j;
		}
		if (nctl <42)
		{
			if( strstr( datafmt[j].name, usrcolname[nctl] ) )
			{
				func.col[j].select = TRUE;
				func.col[j].order_d = 1;
				valueid[ nctl ] = j;
				nctl++;
			}
		}
	}

	func.func = UPD_RECORDERS;
	func.roxnum = nodeList.count();
	udword zone;
	char isset;
	
	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0, func.recorderlen * func.roxnum );

	for( j = 0; j < nodeList.count(); j ++ )
	{
		pNode = nodeList.at( j );
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(_Q2C( pNode->sname ) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)( &(pNode->devtype)  ) );

		for( i = 0; i < 32; i ++ )
		{
			zone = ( pNode->zone1 >> i ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		for (i=32;i<42;i++)
		{
			int nmv = i-32;
			zone = ( pNode->zone2 >> nmv ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
	}
 	
	if((retcode = m_db.UpdateRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	return TRUE;
}


bool QMangerPara::AddRolePara( QList<PNODEPARA> *paddrole )
{
	int i,j;
	char * usrcolname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	void *pt = NULL;
	int keyid1, keyid2;
	int valueid[ 42 ];
	int nctl=0;


	PNODEPARA role;
	for(int j = 0; j < paddrole->count(); j ++ )
	{
		role = paddrole->at( j );
		qDebug() << role->sname <<role->code;
		if( role->oldzone1 != role->zone1 || role->oldzone2 != role->zone2 || role->code !=role->sname)
		{
			role->code = role->sname;
//			qDebug() << role->sname <<role->code;
			role->oldzone1 = role->zone1;
			role->oldzone2 = role->zone2;
		}
		
	}	

	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}

	printf( "%s", "角色权限参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "角色权限参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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

	for( i = 0; i < 42; i ++ )
	{
		valueid[ i ] = 0;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "角色名" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid1 = j;
		}

		if( !strcasecmp( datafmt[j].name, "操作员类型" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid2 = j;
		}
		if (nctl <42)
		{
			if( strstr( datafmt[j].name, usrcolname[nctl] ) )
			{
				func.col[j].select = TRUE;
				func.col[j].order_d = 1;
				valueid[ nctl ] = j;
				nctl++;
			}
		}
	}

	func.func = INS_RECORDERS;
	func.roxnum = paddrole->count();
	udword zone;
	char isset;
	
	pt= malloc( func.recorderlen*func.roxnum);

	memset( pt, 0, func.recorderlen*func.roxnum );

	ptcnt = 0;

	for( j = 0; j < paddrole->count(); j ++ )
	{
		role = paddrole->at(j);
//		qDebug() << role->devtype <<role->sname;
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(_Q2C( role->sname ) ) );

		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)( &(role->devtype) ) );
		for( i = 0; i < 32; i ++ )
		{
			zone = ( role->zone1 >> i ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		for (i = 32; i < 42; i++)
		{
			int nmv = i - 32;
			zone = ( role->zone2 >> nmv ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		ptcnt +=32;		//剩余安全级别指针走过
	}

	if((retcode = m_db.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	m_db.Close();
	return TRUE;
}

bool QMangerPara::AddNodePara(QList<PNODEPARA> *paddlist)
{
	int i,j;
	char * usrcolname[] = {"修改计划曲线","修改实时数据","修改历史曲线","修改历史事项","修改图形","修改网络参数","修改数据库参数", \
		"修改报表","查看计划曲线","查看实时数据","查看历史曲线","查看历史事项","查看图形","查看网络参数","查看数据库参数","查看报表","查看谐波数据", \
		"查看录波数据","查看调试信息","查看通讯原码","遥控","遥调","超级遥控","停闪","挂牌","事项确认", "网络对时","切换主机","切换通道","微机保护",\
		"修改定值","保护投退","修改定值区号","微机保护预留1","微机保护预留2","微机保护预留3","停运RTU","控制模拟盘","封锁前置数据","参数下载",\
		"参数上载","光字牌"};	
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	void *pt = NULL;
	int keyid1, keyid2;
	int valueid[ 42 ];
	int nctl=0;


	PNODEPARA padd;

//	nodeList.setAutoDelete( false );
	

	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}

	printf( "%s", "节点权限参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "节点权限参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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

	for( i = 0; i < 42; i ++ )
	{
		valueid[ i ] = 0;
	}

	for( j = 0; j < func.colnum; j ++ )
	{
		func.col[j].select = FALSE;
		func.col[j].order_d = -1;
		func.col[j].order = -1;
		func.col[j].condition=FALSE;
		
		if( !strcasecmp( datafmt[j].name, "主机名" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid1 = j;
		}

		if( !strcasecmp( datafmt[j].name, "操作员" ) )
		{
			func.col[j].condition = TRUE;
			func.col[j].order_d = 1;
			keyid2 = j;
		}
		if (nctl <42)
		{
			if( strstr( datafmt[j].name, usrcolname[nctl] ) )
			{
				func.col[j].select = TRUE;
				func.col[j].order_d = 1;
				valueid[ nctl ] = j;
				nctl++;
			}
		}
	}

	func.func = INS_RECORDERS;
	func.roxnum = paddlist->count();
	udword zone;
	char isset;
	
	pt= malloc( func.recorderlen*func.roxnum);

	memset( pt, 0, func.recorderlen*func.roxnum );

	ptcnt = 0;

	for( j = 0; j < paddlist->count(); j ++ )
	{
		padd = paddlist->at(j);
		qDebug() << padd->code <<padd->sname;
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(_Q2C( padd->sname ) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(_Q2C( padd->code ) ) );
		for( i = 0; i < 32; i ++ )
		{
			zone = ( padd->zone1 >> i ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		for (i = 32; i < 42; i++)
		{
			int nmv = i - 32;
			zone = ( padd->zone2 >> nmv ) & 0x1;
			isset = (char)zone;
			m_db.FillData( &datafmt[ valueid[ i ] ], pt, ptcnt, &(isset) );
		}
		ptcnt +=32;		//剩余安全级别指针走过
	}

	if((retcode = m_db.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	m_db.Close();

	return TRUE;

}

bool QMangerPara::DelRolePara(QList<PNODEPARA> *pdellist)

{	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	PNODEPARA		pNode;
	
	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	memset(&func,0,sizeof(FUNC_STR));
	char *pcmd;
	pcmd=(char *)malloc(EX_BUFSIZE*2);
	func.func = ISQL_COMMAND;
	
	for (int nlist = 0; nlist < pdellist->count(); nlist++)
	{
		pNode = pdellist->at(nlist);
		sprintf(pcmd,"delete  from 角色权限参数表 where 角色名='%s' and 操作员类型='%s'",_Q2C(pNode->sname),_Q2C(QString::number( pNode->devtype )));
		retcode = m_db.IsqlCommand(&func,pcmd);
		if (retcode != CS_SUCCEED)
			return FALSE;
	}
	
	m_db.Close();
	
	return TRUE ; 
}

bool QMangerPara::DelNodePara(QList<PNODEPARA> *pdellist)

{	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	PNODEPARA		pNode;

	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	memset(&func,0,sizeof(FUNC_STR));
	char *pcmd;
	pcmd=(char *)malloc(EX_BUFSIZE*2);
	func.func = ISQL_COMMAND;

	for (int nlist = 0; nlist < pdellist->count(); nlist++)
	{
		pNode = pdellist->at(nlist);
		sprintf(pcmd,"delete  from 节点权限参数表 where 主机名='%s' and 操作员='%s'",_Q2C(pNode->sname),_Q2C(pNode->code));
		retcode = m_db.IsqlCommand(&func,pcmd);
		if (retcode != CS_SUCCEED)
			return FALSE;
	}
		
	m_db.Close();
	
	return TRUE ; 
}


bool QMangerPara::saveUserCfgPara(QList<PUSERPARA> *pchaglist)
{
	int i,j;
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PUSERPARA		pChageUser;
	void *pt = NULL;
	int keyid1, keyid2, keyid3, keyid4, keyid5, keyid6,keyid7;
	ptcnt = 0;


	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}


	
	if( pchaglist->count() == 0 )
		return TRUE;

	printf( "%s", "用户口令参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "用户口令参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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


	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = i;
		func.col[i].condition=FALSE;
		
		if( !strcasecmp( datafmt[i].name, "操作员类别" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid1 = i;
		}

		if( !strcasecmp( datafmt[i].name, "类别描述" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid2 = i;
		}

		if( !strcasecmp( datafmt[i].name, "操作员" ) )
		{
			func.col[i].condition = TRUE;
			//func.col[i].order_d = 1;
			keyid3 = i;
		}
		if( !strcasecmp( datafmt[i].name, "描述" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid4 = i;
		}
		if( !strcasecmp( datafmt[i].name, "密码" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid5 = i;
		}
		if( !strcasecmp( datafmt[i].name, "组名" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid6 = i;
		}
		if( !strcasecmp( datafmt[i].name, "组长" ) )
		{
			func.col[i].select = TRUE;
			//func.col[i].order_d = 1;
			keyid7 = i;
		}		
	}
	func.func = UPD_RECORDERS;
	func.roxnum = pchaglist->count();
	
	pt= malloc( func.recorderlen * func.roxnum );
	
	ptcnt = 0;
	memset( pt, 0, func.recorderlen * func.roxnum );
	for( j = 0; j < pchaglist->count(); j ++ )
	{
		pChageUser = pchaglist->at( j );

		char iszet = 0;
			if(pChageUser->groupleader) iszet = 1;

		char tmpchar = pChageUser->roletype;
		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(&(tmpchar) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(_Q2C( pChageUser->roledesc ) ) );
		m_db.FillData( &datafmt[ keyid3 ], pt, ptcnt, (void*)(_Q2C( pChageUser->code) ) );
		m_db.FillData( &datafmt[ keyid4 ], pt, ptcnt, (void*)(_Q2C( pChageUser->desc )) );
		m_db.FillData( &datafmt[ keyid5 ], pt, ptcnt, (void*)(_Q2C( pChageUser->password ) ) );
		m_db.FillData( &datafmt[ keyid6 ], pt, ptcnt, (void*)(_Q2C( pChageUser->roledesc ) ) );
		m_db.FillData( &datafmt[ keyid7 ], pt, ptcnt, (void*)(&(iszet)) );
	}
	
	if((retcode = m_db.UpdateRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	return TRUE;
}


bool QMangerPara::AddUserPara(QList<PUSERPARA> *paddlist)
{
	int i;
	
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	CS_DATAFMT      *datafmt=NULL;
	int				ptcnt;
	PUSERPARA		pAdduser;
	void *pt = NULL;
	int keyid1, keyid2, keyid3, keyid4, keyid5, keyid6,keyid7;
	
	ptcnt = 0;

	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}

	
	if( paddlist->count() == 0 )
		return TRUE;

	printf( "%s", "用户口令参数表" );
	fflush( stdout );
	
	func.func = GET_TABLE_INFO;
	strcpy( func.table, "用户口令参数表" );
	strcpy( func.dbname, DB_NAME );
	
	datafmt=0;
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

	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;
		
		if( !strcasecmp( datafmt[i].name, "操作员类别" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid1 = i;
		}

		if( !strcasecmp( datafmt[i].name, "类别描述" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid2 = i;
		}

		if( !strcasecmp( datafmt[i].name, "操作员" ) )
		{
			func.col[i].condition = TRUE;
			func.col[i].order_d = 1;
			keyid3 = i;
		}
		if( !strcasecmp( datafmt[i].name, "描述" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid4 = i;
		}
		if( !strcasecmp( datafmt[i].name, "密码" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid5 = i;
		}
		if( !strcasecmp( datafmt[i].name, "组名" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid6 = i;
		}
		if( !strcasecmp( datafmt[i].name, "组长" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid7 = i;
		}

	}
	func.func = INS_RECORDERS;
	func.roxnum = paddlist->count();
	
	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0,  func.recorderlen * func.roxnum  );
	for( int j = 0; j < paddlist->count(); j ++ )
	{
		pAdduser = paddlist->at( j );
		qDebug() <<pAdduser->roletype;
		qDebug() <<pAdduser->roledesc;
		qDebug() <<pAdduser->code;
		qDebug() <<pAdduser->desc;
		qDebug() <<pAdduser->password;
		qDebug() <<pAdduser->groupleader;

		char iszet = 0;
			if(pAdduser->groupleader) iszet = 1;

		m_db.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(&(pAdduser->roletype) ) );
		m_db.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(_Q2C( pAdduser->roledesc ) ) );
		m_db.FillData( &datafmt[ keyid3 ], pt, ptcnt, (void*)(_Q2C(pAdduser->code) ) );
		m_db.FillData( &datafmt[ keyid4 ], pt, ptcnt, (void*)(_Q2C( pAdduser->desc )) );
		m_db.FillData( &datafmt[ keyid5 ], pt, ptcnt, (void*)(_Q2C( pAdduser->password ) ) );
		m_db.FillData( &datafmt[ keyid6 ], pt, ptcnt, (void*)(_Q2C( pAdduser->roledesc ) ) );
		m_db.FillData( &datafmt[ keyid7 ], pt, ptcnt, (void*)(&(iszet)) );
	}
	
	if((retcode = m_db.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		m_db.Close();	
		return FALSE;
	}
	
	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}
	
	m_db.Close();

	return TRUE;
	
}

bool QMangerPara::DelUserPara(QList<PUSERPARA> *pdellist)
{
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	PUSERPARA		pUser;

	if( m_db.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
	memset(&func,0,sizeof(FUNC_STR));
	char *pcmd;
	pcmd=(char *)malloc(EX_BUFSIZE*2);
	func.func = ISQL_COMMAND;

	for (int nlist = 0; nlist < pdellist->count(); nlist++)
	{
		pUser = pdellist->at(nlist);
		sprintf(pcmd,"delete  from 用户口令参数表 where 操作员='%s'",_Q2C(pUser->code));
		retcode = m_db.IsqlCommand(&func,pcmd);
		if (retcode != CS_SUCCEED)
			return FALSE;
	}
		
	m_db.Close();
	
	return TRUE ; 
}