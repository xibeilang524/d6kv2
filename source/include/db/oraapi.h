/*************************************************
Copyright (C),SHENZHEN SNR TECH.CO.,LTD
File name: oraapi.h
Author: zhaolong
Version: 1.0.0.0
Date: 2011.2.15
Description: oracle接口头文件，数据结构体定义
Others: 
Function List: DBComm
History: 
*************************************************/
#if !defined(AFX_DBAPI_H__DBAPI_ORACLE_H)
#define AFX_DBAPI_H__DBAPI_ORACLE_H

#ifdef __hpux
#pragma pack 8
#endif

#include "dbdef.h"

#if defined (_ORACLE)

#include "oci.h"

#define NUMBER_PRECIS_TINYINT		3
#define NUMBER_PRECIS_SMALLINT		5
#define NUMBER_PRECIS_INT			10
#define NUMBER_PRECIS_REAL			63
#define NUMBER_PRECIS_FLOAT2		0
#define NUMBER_PRECIS_FLOAT			126

// 动态回调
struct dyn_ctx_struct                          
{
   char str1[256];                             
};

typedef struct dyn_ctx_struct  dyn_ctx_struct;

//通用数据库结构体
class DBComm : public NetDbg 
{
	int   colno;
	// 字符转大写
	void StrUpr(char *strs);
	// 出错信息输出
	void CheckErr(OCIError *errhp, sword status);
	// 获得记录的长度
	void GetRecorderLen(short int colnum,CS_DATAFMT *fmt,int *rcdlen);
	// 数据转字符型输出
	void FromDataToStr(CS_DATAFMT *fmt,char *str,void *data);
	// 自定义数据类型转oracle数据类型
	void SybToOraType(short int type,int *ctype);
	// oracle数据类型转自定义数据类型
	CS_INT OraToSybType(ub2 nType,ub2 nPrecision,ub2 * Len);

	//获取表信息
	virtual CS_RETCODE GetTableInfo(char *lib,char* table,short *colnum,short *recordlen,CS_DATAFMT **fmt);
	virtual CS_RETCODE FetchData(FUNC_STR	*func, CS_DATAFMT *datafmt,CS_VOID **pt,char type = 0);
	virtual CS_RETCODE FetchResultFmt(int* colnum,CS_DATAFMT** datafmt,long* rcdlen);
	virtual CS_RETCODE GetMainServer(char* dbname);
	virtual CS_RETCODE IsValidServer(char* dbname);
	virtual CS_RETCODE CreateConnect();
	virtual CS_RETCODE CreateConnect(char *srv,char *usr,char *pwd);
	virtual CS_RETCODE CloseEnv();

	static OCIEnv* m_envhp;
	static OCIError* m_errhp;
	OCIServer* m_srvhp;
	OCISvcCtx* m_svchp;
	OCISession* m_authp;
	OCIStmt* m_stmthp;
	static dyn_ctx_struct  *dynamic_context;

	static short		m_obj;
	static LOG_INFO*	logp;

	char				m_srvname[DESC_LEN],m_usr[DESC_LEN],m_pwd[DESC_LEN],m_link;

public:
	static int			lognum;
	DBComm();
	virtual ~DBComm();

	virtual CS_RETCODE Open (char netflag = FALSE,LOG_INFO*  login=0,int blkflag=FALSE,char webflag=FALSE);
	virtual CS_RETCODE Close ();

	virtual CS_RETCODE init_db ();
	virtual CS_RETCODE connect_use (LOG_INFO *loginfo,char* dbname);
	virtual CS_RETCODE cleanup_db (CS_RETCODE status);
	virtual CS_RETCODE AllocStmtHandle();
	virtual CS_RETCODE FreeStmtHandle();

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
	int tcpConnect();
	int   wrf;
	LOG_INFO	localLog;

protected:
	virtual void  Loop();
	virtual void  TrimSpace(char *str,int len);
	virtual void  TrimRight(char *p);
	virtual void  TrimLeft(char *p);
	virtual void  GetOffset (CS_DATAFMT *fmt, int *ptcnt);
	virtual int  GetTypeLen(ub2 nType,ub2 nPrecision);
	virtual short int CalRcdLen (FUNC_STR *func, CS_DATAFMT *datafmt);

	virtual CS_RETCODE con_use (LOG_INFO *loginfo,char* dbname);
	virtual CS_RETCODE con_use_blk (LOG_INFO *loginfo,char* dbname);
	virtual CS_RETCODE IsqlOneCommand (FUNC_STR *func, CS_CHAR *sqlcmd);

	char	netf,blkf,webf;

	int  blkstep;
	void* pclient;
	int  (STDCALL *BlkOutProcFunc)(int* row,void* pt,void* client);
};

#endif

#endif




