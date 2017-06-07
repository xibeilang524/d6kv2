/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: sqlapi.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: SqlServer接口头文件，数据结构体定义
Others: 
Function List: SqlDbBase、DBComm
History: 
*************************************************/

#if !defined(SQL_SERVER_HEAD_H)
#define SQL_SERVER_HEAD_H

#define DBNTWIN32
#include "stdio.h"
#include "windows.h"

#include "sqlfront.h"
#include "sqldb.h"

#include "sysdef.h"
#include "dbdef.h"
#include "netapi.h"

#define MAXRECORDBUF	20480000	/* 记录集分配最大内存 20M */

class SqlDbBase : public NetDbg
{
public:
	static char			initdbf;
//	static PLOGINREC	Login;
	PDBPROCESS		pdbprocess;
	
	CS_SMALLINT     DEBUG_FLAG; 
	CS_SMALLINT     DISPISQLCOMMAND_FLAG; 
	CS_SMALLINT     IsNetUser; 

	char ComputerName[COMPUTERNAMELEN]; 
	char strAppName[APPNAMELEN]; 
	char    strMsg[EX_BUFSIZE]; 
	LOG_INFO   localLog;

public:
	SqlDbBase();
	virtual ~SqlDbBase();

	CS_VOID db_trimright(char *p);
	CS_VOID db_trimleft(char *p);
	CS_INT datatype(short nType);
    CS_RETCODE CalRcdLen (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_VOID   cleanup_db ();
	RETCODE   exec_cmd (DBCHAR *cmdbuf,DBINT *rowcount=NULL);
	CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void** pt),int step);

protected:
	RETCODE   init_db ();
	RETCODE   connect (LOG_INFO *loginfo,long blk_flag);
	RETCODE   disconnect ();
	RETCODE   con_use (LOG_INFO *loginfo,char* dbname);	
	CS_RETCODE use_db (char *dbname);
	CS_VOID   close ();
//	RETCODE   ctx_cleanup (RETCODE status);
	RETCODE   exec_one_cmd (DBCHAR *cmdbuf,DBINT *rowcount=0);
	RETCODE   con_use_blk (LOG_INFO *loginfo,char* dbname);
	RETCODE   send_cmd(DBCHAR *cmdbuf);
	RETCODE	  fetch_recorders(FUNC_STR	*func,DBCHAR *cmdbuf,CS_DATAFMT **datafmt,CS_VOID **pt);
//	RETCODE   exec_cmd_count (CS_CHAR *cmdbuf, FUNC_STR *func);
//	RETCODE   create_db (char *dbname);
//	RETCODE	 remove_db (char *dbname);
//	CS_RETCODE	 use_db (char *dbname);
//	CS_RETCODE   disp_header (CS_INT numcols, CS_DATAFMT *columns);
//	CS_RETCODE   disp_column (CS_DATAFMT *colfmt, CS_VOID *data, CS_INT datalength, CS_SMALLINT indicator);
/*	CS_RETCODE   fetch_data_any_recorder (CS_COMMAND *cmd, FUNC_STR *func);
	CS_RETCODE   fetch_data_count (CS_COMMAND *cmd, FUNC_STR *func);
	CS_RETCODE   fetch_data_any_recorders (CS_COMMAND *cmd, FUNC_STR *func, CS_VOID *pt);
	CS_RETCODE   fetch_any_results (CS_COMMAND *cmd, FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);
	*/
/*	CS_RETCODE   fetch_data_table_info (CS_COMMAND *cmd, FUNC_STR *func, CS_DATAFMT **datafmt);

	CS_INT   column_datalen (CS_DATAFMT *column);
	int printf_db (FILE *stream, const char *format, ...);
	void disp_error (char *msg);
	void talk_msg (char *msg);
	void isql_msg (char *msg);
	void panic (char *msg);*/
	void TrimSpace(char *str,int len);
protected:
	int  (STDCALL *BlkOutProcFunc)(int* row,void** pt);
	int  blkstep;
};

class DBComm : public SqlDbBase  
{
public:
	DBComm();
	virtual ~DBComm();

	RETCODE Open (char netflag = FALSE,LOG_INFO*  login=0,int blkflag=FALSE,char webflag=FALSE);
	RETCODE Close ();

	CS_RETCODE connect_use (LOG_INFO *loginfo,char* dbname);

	CS_RETCODE BlkCopyIn (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	CS_RETCODE BlkCopyOut (FUNC_STR *func, CS_DATAFMT *datafmt1, CS_VOID **ptt);

/*	short GetSrvNum (int groupno);*/
	CS_RETCODE GetTableInfo (FUNC_STR *func, CS_DATAFMT **datafmt, char* type=0);

	CS_RETCODE InsertOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE InsertRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	CS_RETCODE InsertHisdata(FUNC_STR *func, HIS_DATA *data);

	CS_RETCODE UpdateOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE UpdateRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	CS_RETCODE UpdateHisdata(FUNC_STR *func, HIS_DATA* data);	

	CS_RETCODE DeleteOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);		//for hisdata
	CS_RETCODE DeleteRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);

	CS_RETCODE DeleteData (FUNC_STR *func);

	CS_RETCODE SelectOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE SelectRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID **pt);
	CS_RETCODE SelectHisdata(FUNC_STR *func, HIS_DATA** data);
	CS_RETCODE SelectEvent(SYS_TIME& fromTime,SYS_TIME& toTime,char* condition,int* nEvent,HIS_EVENT **pEvent,int OEventf);
	CS_RETCODE SelectRcdCount (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE SelectIsqlResults (FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);

//	CS_RETCODE   FetchDataData (CS_COMMAND *cmd, FUNC_STR *func, DATA_STR *data);
//	CS_RETCODE   fetch_dbsrv_info (CS_COMMAND *cmd, LOG_INFO *loginfo);

	CS_RETCODE DumpLog (FUNC_STR *func);
/*	CS_RETCODE UseDB (char* dbname);*/
	CS_RETCODE IsqlCommand (FUNC_STR *func, CS_CHAR *sqlcmd=NULL);

	CS_RETCODE GetTableType(char* dbname,char* table,char* type);

	CS_RETCODE GetUsage(char* server,char* dbname,float& usage);

	CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void** pt),int step);


	void  Loop();
	double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,char* colname,void* pt,int& ptcnt,char* buf);
	double GetData(int rcdno,int columno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);

	int FillData(CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	int			GetLogNum(){	return lognum;	}
	LOG_INFO*	GetLogPtr(){	return logp;	}

	CS_RETCODE	GetWebDBSrv(char* dbname);

public:
	int   wrf;

protected:
	int	  GetMainDBSrvLogInfo(LOG_INFO* log,char* dbname=0);
	void FromDataToStr (CS_DATAFMT *datafmt, char *cmdbuf1, void *datapt);
	void GetOffset (CS_DATAFMT *fmt, int *ptcnt);

	char		netf,threadf,blkf,exitf,webf;
	NetStatus	cfg;
	NetControl  netctl;
	static LOG_INFO*	logp;
	static int			lognum;

private:

#if defined(WIN32)
	CWinThread *thread;
#elif defined(__unix)
	pthread_t thread;
#endif	

	char	dbname[CODE_LEN];
	static int	objnum;
	static char	opennum;
	int   colno;
};


#endif