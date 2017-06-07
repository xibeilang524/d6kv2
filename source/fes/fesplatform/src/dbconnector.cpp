/**
@file dbconnector.cpp
@brief 前置平台包装实现之数据库接口实现

@author chenkai 
@version 1.0.0
@date 2016-07-13
*/
#include "fes/platformwrapper.h"
#include "db/dbapi.h"

//数据库正常标志，0正常，-1异常
static int s_db_stat = 0;
static DBApi s_db;


int Db_connector::open(void)
{
	s_db_stat = CS_SUCCEED == s_db.Open() ? 0 : -1;
	return s_db_stat;
}

int Db_connector::select(const char* sql, void**result_pt)
{
	int ret = -1;

	//只有数据库正常时才真正从数据库select
	if (!s_db_stat && sql != NULL)
	{
		CS_DATAFMT *datafmt = NULL;
		FUNC_STR func;
		func.func = SEL_ISQL_RESULTS;
		strcpy(func.dbname, DB_NAME);
		strcpy(func.isql, sql);

		if (CS_SUCCEED == s_db.SelectIsqlResults(&func, &datafmt, result_pt))
		{
			ret = func.ret_roxnum;
		}

		if (datafmt)
		{
			free(datafmt);
			datafmt = NULL;
		}
	}//if (!s_db_stat && sql != NULL)

	return ret;
}

void Db_connector::close(void)
{
	s_db.Close();
	s_db_stat = -1;
}