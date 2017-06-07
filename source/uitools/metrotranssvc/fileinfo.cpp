#include "fileinfo.h"


File_info::File_info()
{
	m_action_num =0;
	m_blockcond_num =0;
	m_triggercond_num =0;
}

File_info::~File_info()
{
// 	m_dbApi.Close();
}

int File_info::Init()
{
// 	if(m_dbApi.Open(DB_OP_R) != CS_SUCCEED)
// 	{
// 		printf("\n 打开数据库失败!" );
// 		return	-1;
// 	}
	return 0;
}

int File_info::read_action_para()
{
	DBApi m_dbApi;

	if(m_dbApi.Open(DB_OP_R) != CS_SUCCEED)
	{
		printf("\n 打开数据库失败!" );
		return	-1;
	}

	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode = 0;
	CS_DATAFMT *datafmt = NULL;
	strcpy(func.table,"metro_action");
	strcpy(func.dbname,"modeldb");
	func.func = GET_TABLE_INFO;
	retcode = m_dbApi.GetTableInfo(&func,&datafmt);

	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) 
			free(datafmt);
		return -1;
	}
	char *SelColName[] = {"groupno","no","opertype","yxcode","yxtablecode","yxstatus","msg"};
	int SelColNum = sizeof(SelColName)/sizeof(char*);
	int i;

	for( i=0;i<func.colnum;i++)
	{
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].select = NO;
		func.col[i].condition = NO;

		for(int j=0;j<SelColNum;j++)
		{

			if(strcasecmp(datafmt[i].name,*(SelColName+j)) !=0) 
				continue;
			func.col[i].select = YES;

			if(strcasecmp(datafmt[i].name,"no")==0 ) 
				func.col[i].order = 0;

			func.col[i].order_d = j;
		}
	}

	///
	CS_VOID *pt = NULL;
	int ptcnt = 0;

	func.func = SEL_RECORDERS;

	retcode = m_dbApi.SelectRcds(&func, datafmt, &pt);
	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) free(datafmt);
		if(pt != NULL) free(pt);
		return -1;
	}

	if(func.ret_roxnum == 0)
	{
		printf("没有动作表\n!");
		return -1;
	}

	m_action_num = func.ret_roxnum;
	m_action_list = new ActionInfo[m_action_num];

	for(i=0; i<m_action_num; i++)
	{
		(m_action_list+i)->groupno =	  (int)m_dbApi.GetData(i,&func,datafmt,SelColName[0],pt,ptcnt,NULL);
		(m_action_list+i)->No =	  (int)m_dbApi.GetData(i,&func,datafmt,SelColName[1],pt,ptcnt,NULL);
		(m_action_list+i)->oper_type =	  (int)m_dbApi.GetData(i,&func,datafmt,SelColName[2],pt,ptcnt,NULL);
		m_dbApi.GetData(i,&func,datafmt,SelColName[3],pt,ptcnt,(m_action_list+i)->code);
		m_dbApi.GetData(i,&func,datafmt,SelColName[4],pt,ptcnt,(m_action_list+i)->table);
		(m_action_list+i)->state = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[5],pt,ptcnt,NULL);
		m_dbApi.GetData(i,&func,datafmt,SelColName[6],pt,ptcnt,(m_action_list+i)->msg);
	}

	m_dbApi.Close();
	return 0;
}

int File_info::read_action_realstate()
{
	if( m_pControlNum == 0 )
		return 1;

	for(int i = 0; i<m_pControlNum; i++ )
	{
		Rdb_SqlCli rdb;
		int nRet,nRcdCount;
	
		rdb.RdbSelectFrom(m_pControlList[i].table);
		rdb.RdbSelectField("value");
		rdb.RdbWhereCond("name",RDB_DATATYPE_STRING,m_pControlList[i].code);
		nRet=rdb.RdbSelCmd(1,"scada","scada");	
		if(nRet == RDB_OK)
		{
			//唯一
			nRcdCount=rdb.RdbGetRcdCnt();
			m_pControlList[i].real_state = (int )(rdb.RdbGetVal_int(0,"value"));
		}
		else
			return 0;
	}
	return 1;
}

int File_info::read_blockcond_para(int blockcondno)
{
	DBApi m_dbApi;

	if(m_dbApi.Open(DB_OP_R) != CS_SUCCEED)
	{
		printf("\n 打开数据库失败!" );
		return	-1;
	}

	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode = 0;
	CS_DATAFMT *datafmt = NULL;
	strcpy(func.table,"metro_block_cond");
	strcpy(func.dbname,"modeldb");
	func.func = GET_TABLE_INFO;
	retcode = m_dbApi.GetTableInfo(&func,&datafmt);

	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) 
			free(datafmt);
		return -1;
	}

	char *SelColName[] = {"blockcondno","no","yxcode","tablecode","yxstatus"};
	int SelColNum = sizeof(SelColName)/sizeof(char*);
	int i;

	for( i=0;i<func.colnum;i++)
	{
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].select = NO;
		func.col[i].condition = NO;

		for(int j=0;j<SelColNum;j++)
		{

			if(strcasecmp(datafmt[i].name,*(SelColName+j))!=0 ) 
				continue;
			func.col[i].select = YES;

			if(strcasecmp(datafmt[i].name,"no")==0 ) 
				func.col[i].order = 0;

			func.col[i].order_d = j;
		}
	}

	///
	CS_VOID *pt = NULL;
	int ptcnt = 0;
	char tmpstr[64];
	memset(tmpstr,0,64);
	sprintf(tmpstr,"blockcondno= %d",blockcondno);
	strcpy(func.isql,tmpstr);

	func.func = SEL_ISQL_RECORDERS;
	retcode = m_dbApi.SelectRcds(&func, datafmt, &pt);
	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) free(datafmt);
		if(pt != NULL) free(pt);
		return -1;
	}

	if(func.ret_roxnum == 0)
	{
		printf("没有闭锁条件\n!");
		m_blockcond_num = func.ret_roxnum;
		return 0;
	}

	m_blockcond_num = func.ret_roxnum;
	m_blockcond_list = new BlockCond[m_blockcond_num];

	for(i=0; i<m_blockcond_num; i++)
	{
		(m_blockcond_list+i)->block_no = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[0],pt,ptcnt,NULL);
		(m_blockcond_list+i)->No =	  (int)m_dbApi.GetData(i,&func,datafmt,SelColName[1],pt,ptcnt,NULL);
		m_dbApi.GetData(i,&func,datafmt,SelColName[2],pt,ptcnt,(m_blockcond_list+i)->code);
		m_dbApi.GetData(i,&func,datafmt,SelColName[3],pt,ptcnt,(m_blockcond_list+i)->table);
		(m_blockcond_list+i)->state = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[4],pt,ptcnt,NULL);
	}

	m_dbApi.Close();
	return m_blockcond_num;
}

int File_info::read_blockcond_realstate()
{
	if( m_blockcond_num == 0 )
		return 0;

	for(int i = 0; i<m_blockcond_num; i++ )
	{
		Rdb_SqlCli rdb;
		int nRet,nRcdCount;
		rdb.RdbSelectFrom(m_blockcond_list[i].table);
		rdb.RdbSelectField("value");
		rdb.RdbWhereCond("name",RDB_DATATYPE_STRING,m_blockcond_list[i].code);
		nRet=rdb.RdbSelCmd(1,"scada","scada");	
		if(nRet == RDB_OK)
		{
			//唯一
			nRcdCount=rdb.RdbGetRcdCnt();
			m_blockcond_list[i].real_state = (int )(rdb.RdbGetVal_int(0,"value"));
		}
		else
			return 0;
	}
	return 1;
}

int File_info::read_triggercond_para()
{
	DBApi m_dbApi;

	if(m_dbApi.Open(DB_OP_R) != CS_SUCCEED)
	{
		printf("\n 打开数据库失败!" );
		return	-1;
	}

	FUNC_STR func;
	memset(&func,0,sizeof(FUNC_STR));
	CS_RETCODE retcode = 0;
	CS_DATAFMT *datafmt = NULL;
	strcpy(func.table,"metro_trigger_cond");
	strcpy(func.dbname,"modeldb");
	func.func = GET_TABLE_INFO;
	retcode = m_dbApi.GetTableInfo(&func,&datafmt);

	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) 
			free(datafmt);
		return -1;
	}

	char *SelColName[] = {"no","yxcode","yxstatus","blockcondno","actiongroupno"};
	int SelColNum = sizeof(SelColName)/sizeof(char*);
	int i;

	for( i=0;i<func.colnum;i++)
	{
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].select = NO;
		func.col[i].condition = NO;

		for(int j=0;j<SelColNum;j++)
		{

			if(strcasecmp(datafmt[i].name,*(SelColName+j))!=0 ) 
				continue;
			func.col[i].select = YES;

			if(strcasecmp(datafmt[i].name,"no")==0 ) 
				func.col[i].order = 0;

			func.col[i].order_d = j;
		}
	}

	///
	CS_VOID *pt = NULL;
	int ptcnt = 0;

	func.func = SEL_RECORDERS;
	retcode = m_dbApi.SelectRcds(&func, datafmt, &pt);
	if(retcode == CS_FAIL)
	{
		if(datafmt != NULL) free(datafmt);
		if(pt != NULL) free(pt);
		return -1;
	}

	if(func.ret_roxnum == 0)
	{
		printf("没有触发条件表\n!");
		return -1;
	}

	m_triggercond_num = func.ret_roxnum;
	m_faultcond_list = new TriggerCond[m_triggercond_num];

	for(i=0; i<m_triggercond_num; i++)
	{
		(m_faultcond_list+i)->No =	  (int)m_dbApi.GetData(i,&func,datafmt,SelColName[0],pt,ptcnt,NULL);
		m_dbApi.GetData(i,&func,datafmt,SelColName[1],pt,ptcnt,(m_faultcond_list+i)->code);
		(m_faultcond_list+i)->state = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[2],pt,ptcnt,NULL);
		(m_faultcond_list+i)->blockcond_no = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[3],pt,ptcnt,NULL);
		(m_faultcond_list+i)->action_no = (int)m_dbApi.GetData(i,&func,datafmt,SelColName[4],pt,ptcnt,NULL);
	}

	m_dbApi.Close();

	return 0;
}

int File_info::get_actionlist_by_actionno(int action_no)
{
	m_pControlNum = 0;
	for(int i=0;i<m_action_num;i++)
	{
		if (m_action_list[i].groupno==action_no)
		{
			m_pControlNum++;
		}
	}
	m_pControlList = new ActionInfo[m_pControlNum];
	m_pControlNum=0;
	for(int i=0;i<m_action_num;i++)
	{
		if (m_action_list[i].groupno==action_no)
		{
			m_pControlList[m_pControlNum] =m_action_list[i] ;
			m_pControlNum++;
		}
	}
	return m_pControlNum;
}