/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: dbapi.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: 数据库接口共用头文件，数据结构体定义
Others: 
Function List: 
History: 
*************************************************/

#if !defined(AFX_DBAPI_H__DBAPI_H)
#define AFX_DBAPI_H__DBAPI_H

#include "db/dbdef.h"
#include "sysstr.h"

#if defined (_SYBASE)
#include "db/sybapi.h"
#elif defined (_SQLSERVER)
#include "db/sqlapi.h"
#elif defined (_ODBC)
#include "db/odbcapi.h"
#elif defined (_ORACLE)
#include "db/oraapi.h"
#elif defined (_MYSQL)
#include "db/mysqlapi.h"
#elif defined (_DMSQL)
#include "db/dmapi.h"
#endif

#if defined(WIN32)
#if defined(DBSQL_EXPORTDLL)
#define	DBSQL_IMEXPORT				__declspec(dllexport) 
#else
#define	DBSQL_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	DBSQL_IMEXPORT
#endif

//单库
class DBSQL_IMEXPORT DBSingle : public DBComm 
{
	virtual CS_RETCODE OutData(char* dbname,char* table,int& nCol,CS_DATAFMT *fmt,int& nRow,VAL_UNION** data);
	virtual CS_RETCODE InData(char* dbname,char* table,int& nCol,CS_DATAFMT *fmt,int& nRow,VAL_UNION* data);
	virtual CS_RETCODE GetSpace(char* dbname,float* total,float* use);		//M
public:
	DBSingle();
	virtual ~DBSingle();
	int isDBType(int dbtype);

	virtual CS_RETCODE Open (char netflag = FALSE,LOG_INFO*  login=0,char* dbname=0,int blkflag=FALSE);
	virtual CS_RETCODE Close ();

	virtual CS_RETCODE connect_use (LOG_INFO *loginfo,char* dbname);

	virtual CS_RETCODE BlkCopyIn (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt, char convertf=0);
	virtual CS_RETCODE BlkCopyOut (FUNC_STR *func, CS_DATAFMT *datafmt1, CS_VOID **ptt);

	virtual CS_RETCODE GetTableInfo (FUNC_STR *func, CS_DATAFMT **datafmt, char* type=0);

	virtual CS_RETCODE InsertOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE InsertRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE InsertHisdata(FUNC_STR *func, HIS_DATA *data);

	virtual CS_RETCODE UpdateOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE UpdateRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE UpdateHisdata(FUNC_STR *func, HIS_DATA* data);	

	virtual CS_RETCODE DeleteRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);

	virtual CS_RETCODE DeleteData (FUNC_STR *func);

	virtual CS_RETCODE SelectOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID **pt);
	virtual CS_RETCODE SelectHisdata(FUNC_STR *func, HIS_DATA** data);
	virtual CS_RETCODE SelectEvent(SYS_TIME& fromTime,SYS_TIME& toTime,char* condition,int* nEvent,HIS_EVENT **pEvent,int OEventf);
	virtual CS_RETCODE SelectRcdCount (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectIsqlResults (FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);

	virtual CS_RETCODE DumpLog (FUNC_STR *func);
	virtual CS_RETCODE UseDB (char* dbname);
	virtual CS_RETCODE IsqlCommand (FUNC_STR *func, CS_CHAR *sqlcmd=NULL);

	virtual CS_RETCODE GetTableType(char* dbname,char* table,char* type);

	virtual CS_RETCODE GetUsage(char* server,char* dbname,float& usage);
	virtual CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void* pt,void* client),void* client,int step);

	//code=DB_OUT_FILE,倒出文件;p1为库名(in);p2为列表名(in);p3为列数(out));p4为列指针(out)；p5为行数(out));p6为行指针(out)
	//code=DB_IN_FILE,倒入文件;p1为库名(in);p2为列表名(in);p3为列数(out));p4为列指针(out)；p5为行数(in);p6为行指针(in)
	//code=DB_GET_SPACE,取数据库容量;p1为库名(in);p2为总容量(out);p3为使用容量(out)
	virtual CS_RETCODE DbTools(int code,void* p1,void* p2,void* p3,void** p4,void* p5,void** p6);

	virtual void  Loop();
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,char* colname,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,int columno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);

	virtual int FillData(CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual int			GetLogNum();
	virtual LOG_INFO*	GetLogPtr();

	virtual CS_RETCODE	GetWebDBSrv(char* dbname);

public:
	int   wrf;
	static int m_dbtype;
};

class DBSQL_IMEXPORT DBApi
{
public:
	DBApi();
	virtual ~DBApi();
	
	// 判断数据库类型
	int isDBType(int dbtype);

	// 通用数据库的访问
	CS_RETCODE Open (char wr_mode=DB_OP_R,char netflag = FALSE,int blkflag=FALSE,char webflag = FALSE);
	CS_RETCODE Close ();

	CS_RETCODE GetTableInfo (FUNC_STR *func, CS_DATAFMT **datafmt, char* type=0);

	CS_RETCODE SelectOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE SelectRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID **pt);
	CS_RETCODE SelectRcdCount (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE SelectIsqlResults (FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);

	CS_RETCODE InsertOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE InsertRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);

	CS_RETCODE UpdateOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE UpdateRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);

	CS_RETCODE DeleteRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	CS_RETCODE DeleteData (FUNC_STR *func);
	
	CS_RETCODE BlkCopyIn (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt, char convertf=0);
	CS_RETCODE BlkCopyOut (FUNC_STR *func, CS_DATAFMT *datafmt1, CS_VOID **ptt);

	CS_RETCODE DumpLog (FUNC_STR *func);
	CS_RETCODE IsqlCommand (FUNC_STR *func, CS_CHAR *sqlcmd=NULL);

	CS_RETCODE GetUsage(char* server,char* dbname,float& usage);

	double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,char* colname,void* pt,int& ptcnt,char* buf);
	double GetData(int rcdno,int columno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);

	int FillData(CS_DATAFMT* datafmt,void* pt,int& ptcnt,void* buf);

	//历史数据库的访问
	CS_RETCODE SelectEvent(SYS_TIME& fromTime,SYS_TIME& toTime,char* condition,int* nEvent,HIS_EVENT **pEvent,int OEventf=FALSE);

	CS_RETCODE InsertHisdata(FUNC_STR *func, HIS_DATA *data,int sametimef=TRUE);
	CS_RETCODE SelectHisdata(FUNC_STR *func, HIS_DATA** data);
	CS_RETCODE UpdateHisdata(FUNC_STR *func, HIS_DATA* data);

	CS_RETCODE SetCallbakProc(int  (STDCALL *proc)(int* row,void* pt,void* client),void* client = 0,int step = 0);

protected:
	static DBComm	db_w[SERVER_NODE_MAX];
	static DBComm   db_r;
	static int		m_dbtype;
	static int		dbnum;
	DBComm			db_web;
	char			m_webflag;

};

#endif




