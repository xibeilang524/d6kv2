/*
*/

#ifndef MYSQL_API_H
#define MYSQL_API_H
#pragma once

#include "dbdef.h"

// #include <my_global.h>
// #include <my_sys.h>
// #if defined (_MYSQL)
#include <mysql.h>
#include "netapi.h"

class DBComm: public NetDbg 
{
public:
	enum NET_NUM
	{
		e_netNum = 2,//A、B网
	};

public:
	DBComm(void);
	virtual ~DBComm(void);

	virtual CS_RETCODE Open (char netflag = FALSE,LOG_INFO*  login=0,int blkflag=FALSE,char webflag=FALSE);
	virtual CS_RETCODE Close ();

	virtual CS_RETCODE init_db ();
	virtual CS_RETCODE connect_use (LOG_INFO *loginfo,char* dbname);
	virtual CS_RETCODE cleanup_db (CS_RETCODE status);
/*
	virtual CS_RETCODE AllocStmtHandle();
	virtual CS_RETCODE FreeStmtHandle();
*/

	virtual int FillData(CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,char* colname,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,int columno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);
	virtual double GetData(int rcdno,FUNC_STR* func,CS_DATAFMT* datafmt,void* pt,int& ptcnt,char* buf);

	virtual CS_RETCODE GetTableType(char* dbname,char* table,char* type);
	virtual CS_RETCODE GetTableInfo (FUNC_STR *func, CS_DATAFMT **datafmt, char* type=0);

	virtual CS_RETCODE ExecSQL(char *sqlbuf);
	virtual CS_RETCODE exec_cmd(char *sqlbuf);
	virtual CS_RETCODE TransRollback();
	virtual CS_RETCODE TransCommit(long* rowcount);

	virtual CS_RETCODE BlkCopyIn (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt,char convertf=0);
	virtual CS_RETCODE BlkCopyOut (FUNC_STR *func, CS_DATAFMT *datafmt1, CS_VOID **ptt);

	virtual CS_RETCODE InsertHisdata(FUNC_STR *func, HIS_DATA *data);
	virtual CS_RETCODE InsertOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE InsertRcds(FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt,char convertf=0);

	virtual CS_RETCODE exec_cmd_count (CS_CHAR *cmdbuf, FUNC_STR *func);
	virtual CS_RETCODE SelectRcdCount (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE SelectRcds(FUNC_STR* func,CS_DATAFMT *datafmt,void** datapt);
	virtual CS_RETCODE SelectIsqlResults (FUNC_STR *func, CS_DATAFMT **datafmt, CS_VOID **pt);
	virtual CS_RETCODE SelectHisdata(FUNC_STR *func, HIS_DATA** data);
	virtual CS_RETCODE SelectEvent(SYS_TIME& fromTime,SYS_TIME& toTime,char* condition,int* nEvent,HIS_EVENT **pEvent,int OEventf);

	virtual CS_RETCODE UpdateOneRcd (FUNC_STR *func, CS_DATAFMT *datafmt);
	virtual CS_RETCODE UpdateRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);
	virtual CS_RETCODE UpdateHisdata(FUNC_STR *func, HIS_DATA* data);

	virtual CS_RETCODE DeleteData (FUNC_STR *func);
	virtual CS_RETCODE DeleteRcds (FUNC_STR *func, CS_DATAFMT *datafmt, CS_VOID *pt);

	virtual CS_RETCODE IsqlCommand (FUNC_STR *func, CS_CHAR *sqlcmd);
	virtual CS_RETCODE DumpLog (FUNC_STR *func);

	virtual CS_RETCODE GetUsage(char* server,char* dbname,float& usage);

	virtual int		   GetLogNum(){	return lognum;	}
	virtual LOG_INFO*  GetLogPtr(){	return logp;	}

	virtual CS_RETCODE UseDB (char* dbname);
	virtual CS_RETCODE GetWebDBSrv(char* dbname);
	virtual CS_RETCODE SetBlkOutProcFunc(int  (STDCALL *proc)(int* row,void* pt,void* client),void* client,int step);

public:
	// 字符转大写
	void StrUpr(char *strs);
	void PrintError(const char* error);

protected:

	CS_RETCODE GetTableInfo(char *lib,char* table,short *colnum,short *recordlen,CS_DATAFMT **fmt);
	CS_INT ToSybType(enum_field_types type,unsigned int nPrecision,unsigned short * Len);
	// 数据转字符型输出
	void FromDataToStr(CS_DATAFMT *fmt,char *str,void *data);

	CS_RETCODE FetchData(FUNC_STR	*func, CS_DATAFMT *datafmt,CS_VOID **pt,char type = 0);
	CS_RETCODE FetchResultFmt(int* colnum,CS_DATAFMT** datafmt,long* rcdlen);
	CS_RETCODE GetMainServer(char* dbname);
	CS_RETCODE IsValidServer(char* dbname);
	CS_RETCODE CreateConnect();
	CS_RETCODE CreateConnect(char *srv,char *usr,char *pwd);
	CS_RETCODE CloseEnv();

protected:
	void  Loop();
	void  TrimSpace(char *str,int len);
	void  TrimRight(char *p);
	void  TrimLeft(char *p);
	void  GetOffset (CS_DATAFMT *fmt, int *ptcnt);
/*
	int  GetTypeLen(ub2 nType,ub2 nPrecision);
*/
	short int CalRcdLen (FUNC_STR *func, CS_DATAFMT *datafmt);

	CS_RETCODE con_use (LOG_INFO *loginfo,char* dbname);
	CS_RETCODE con_use_blk (LOG_INFO *loginfo,char* dbname);
	CS_RETCODE IsqlOneCommand (FUNC_STR *func, CS_CHAR *sqlcmd);

	char	netf,blkf,webf;

	int  blkstep;
	void* pclient;
	int  (STDCALL *BlkOutProcFunc)(int* row,void* pt,void* client);


public:
	int   wrf;//读写标志
	LOG_INFO	localLog;

protected:
	static int	lognum;
	static short		m_obj;
	static LOG_INFO*	logp;

	char	m_srvname[DESC_LEN];//"nbdb" "nbdbb"
	char	m_usr[DESC_LEN];
	char	m_pwd[DESC_LEN];
	char	m_link;

	char m_host[e_netNum][DESC_LEN];//A、B网地址
	int m_port;//端口

	char* m_lastError;//记录错误信息

	int   colno;

protected:
	MYSQL* m_mysql_conn; //
	MYSQL_STMT* m_mysql_stmt;//
	MYSQL_RES* m_mysql_res;//
	MYSQL_ROW m_mysql_row;//

private:
	void *m_pResBuf;
	CS_DATAFMT *m_pResDataFmt;

	bool m_bAutoFreeBuf;//自动回收内存标志

	void ReleaseDataFmtBuf();
	void ReleaseResBuf();
};
// #endif
#endif

