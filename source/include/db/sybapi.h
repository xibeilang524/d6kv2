/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: sybapi.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: Sybase接口头文件，数据结构体定义
Others: 
Function List: DBBase、DBComm
History: 
*************************************************/

#if !defined(AFX_DBAPI_H__3F9D8B5E_6BB3_4F3F_911D_E6B11A0B415E__INCLUDED_)
#define AFX_DBAPI_H__3F9D8B5E_6BB3_4F3F_911D_E6B11A0B415E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if defined (_SYBASE)

#include "cspublic.h"
#include "ctpublic.h"
#include "sysdef.h"
#include "net/netapi.h"
#include "dbdef.h"

void SetClientMsgProcFunc(void (STDCALL *func)(char* errmsg,int retlen));
void SetServerMsgProcFunc(void (STDCALL *func)(char* srvmsg,int retlen));

class DBBase : public NetDbg
{
public:
	static char				initdbf;
	static CS_CONTEXT*		context; 
	CS_CONNECTION*			connection;
	
	CS_SMALLINT     DEBUG_FLAG; 
	CS_SMALLINT     DISPISQLCOMMAND_FLAG; 
	CS_SMALLINT     IsNetUser; 

	char ComputerName[COMPUTERNAMELEN]; 
	char strAppName[APPNAMELEN]; 
	char    strMsg[EX_BUFSIZE]; 
	LOG_INFO   localLog;

public:
	DBBase();
	virtual ~DBBase();
	short int CalRcdLen (FUNC_STR *func, CS_DATAFMT *datafmt);
	CS_RETCODE   exec_cmd (CS_CHAR *cmdbuf, CS_INT *rowcount=NULL);
	CS_RETCODE   cleanup_db (CS_RETCODE status);
	virtual CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void* pt,void* client),void* client,int step);


protected:
	CS_RETCODE   init_db ();
//	CS_RETCODE   connect_use (LOG_INFO *loginfo,char* dbname);
	CS_RETCODE   con_use (LOG_INFO *loginfo,char* dbname);	
//	CS_RETCODE   cleanup_db (CS_RETCODE status);
	CS_RETCODE   ctx_cleanup (CS_RETCODE status);
	CS_RETCODE   exec_one_cmd (CS_CHAR *cmdbuf, CS_INT *rowcount);
	CS_RETCODE   con_use_blk (LOG_INFO *loginfo,char* dbname);
	CS_RETCODE   exec_cmd_count (CS_CHAR *cmdbuf, FUNC_STR *func);
	CS_RETCODE   create_db (char *dbname);
	CS_RETCODE	 remove_db (char *dbname);
	CS_RETCODE	 use_db (char *dbname);
	CS_RETCODE   disp_header (CS_INT numcols, CS_DATAFMT *columns);
	CS_RETCODE   disp_column (CS_DATAFMT *colfmt, CS_VOID *data, CS_INT datalength, CS_SMALLINT indicator);
//	CS_RETCODE   fetch_data (CS_COMMAND *cmd);
	CS_RETCODE   fetch_data_any_recorder (CS_COMMAND *cmd, FUNC_STR *func);
	CS_RETCODE   fetch_data_count (CS_COMMAND *cmd, FUNC_STR *func);
	CS_RETCODE   fetch_data_any_recorders (CS_COMMAND *cmd, FUNC_STR *func, CS_VOID *pt);
	CS_RETCODE   fetch_any_results (CS_COMMAND *cmd, FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);
	CS_RETCODE   select_isql_results (char *isqlstr, short *colnum, CS_INT *retrow, CS_DATAFMT **datafmt, CS_VOID **datapt);
	CS_RETCODE   fetch_data_table_info (CS_COMMAND *cmd, FUNC_STR *func, CS_DATAFMT **datafmt);

	CS_INT   column_datalen (CS_DATAFMT *column);
	int printf_db (FILE *stream, const char *format, ...);
	void disp_error (char *msg);
	void talk_msg (char *msg);
	void isql_msg (char *msg);
	void panic (char *msg);
	void TrimSpace(char *str,int len);
protected:
	static int	m_objnum;

	int  (STDCALL *BlkOutProcFunc)(int* row,void* pt,void* client);
	int  blkstep;
	void* pclient;
};

class DBComm : public DBBase  
{
public:
	DBComm();
	virtual ~DBComm();

	virtual CS_RETCODE Open (char netflag = FALSE,LOG_INFO*  login=0,int blkflag=FALSE,char webflag=FALSE);
	//virtual CS_RETCODE Open (char netflag,LOG_INFO*  login,int blkflag,char webflag);
	virtual CS_RETCODE Close ();

	virtual CS_RETCODE connect_use (LOG_INFO *loginfo,char* dbname);

	virtual CS_RETCODE BlkCopyIn (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt,char convertf=0);
	virtual CS_RETCODE BlkCopyOut (FUNC_STR *func, CS_DATAFMT *datafmt1, CS_VOID **ptt);

	virtual short GetSrvNum (int groupno);
	virtual CS_RETCODE GetTableInfo (FUNC_STR *func, CS_DATAFMT **datafmt, char* type=0);

	virtual CS_RETCODE InsertOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE InsertRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE InsertHisdata(FUNC_STR *func, HIS_DATA *data);

	virtual CS_RETCODE UpdateOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE UpdateRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE UpdateHisdata(FUNC_STR *func, HIS_DATA* data);

//	CS_RETCODE DeleteOneRcd (LOG_INFO* log,FUNC_STR *func, CS_DATAFMT *datafmt);		//for hisdata
	virtual CS_RETCODE DeleteRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE DeleteData (FUNC_STR *func);

	virtual CS_RETCODE SelectOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID **pt);
	virtual CS_RETCODE SelectHisdata(FUNC_STR *func, HIS_DATA** data);
	virtual CS_RETCODE SelectEvent(SYS_TIME& fromTime,SYS_TIME& toTime,char* condition,int* nEvent,HIS_EVENT **pEvent,int OEventf);
	virtual CS_RETCODE SelectRcdCount (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectIsqlResults (FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);

//	CS_RETCODE   FetchDataData (CS_COMMAND *cmd, FUNC_STR *func, DATA_STR *data);
//	CS_RETCODE   fetch_dbsrv_info (CS_COMMAND *cmd, LOG_INFO *loginfo);

	virtual CS_RETCODE DumpLog (FUNC_STR *func);
	virtual CS_RETCODE UseDB (char* dbname);
	virtual CS_RETCODE IsqlCommand (FUNC_STR *func, CS_CHAR *sqlcmd=NULL);
	virtual CS_RETCODE GetTableType(char* dbname,char* table,char* type);

	virtual CS_RETCODE GetUsage(char* server,char* dbname,float& usage);
	virtual CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void* pt,void* client),void* client,int step);

	virtual void  Loop();
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,char* colname,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,int columno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);

	virtual int FillData(CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual int			GetLogNum(){	return lognum;	}
	virtual LOG_INFO*	GetLogPtr(){	return logp;	}

	virtual CS_RETCODE	GetWebDBSrv(char* dbname);

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

	pthread_t thread;

	char	dbname[CODE_LEN];
	static char	opennum;
	int   colno;
};

#endif

#endif // !defined(AFX_DBAPI_H__3F9D8B5E_6BB3_4F3F_911D_E6B11A0B415E__INCLUDED_)
