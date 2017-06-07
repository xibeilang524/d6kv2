// msg.h: interface for the MsgNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSG_H__70B2C3DE_A76D_4744_8B3A_B99E1A2877F8__INCLUDED_)
#define AFX_MSG_H__70B2C3DE_A76D_4744_8B3A_B99E1A2877F8__INCLUDED_

#include "net/netapi.h"
#include "rdb/rdbapi.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(WIN32)
#if defined(MSG_EXPORTDLL)
#define	MSG_IMEXPORT				__declspec(dllexport) 
#else
#define	MSG_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	MSG_IMEXPORT
#endif

struct sock_str
{
	int sock;
	char listenf;
//	char proxyf;
	int (*OnReceive)(void*);
	int (*OnConnect)(void*);
};
/*
typedef struct _sock_man
{
	int num;
	struct sock_str* psock;
}SockMan;

class MSG_IMEXPORT MsgNet : public Tcp
{

	static char lockf;
#if defined(WIN32)
	static CWinThread* thread;
#elif defined(__unix)
	static pthread_t thread;
#endif

protected:

public:
	MsgNet();
	MsgNet(int socketid);
	virtual ~MsgNet();
	
	virtual int  create(int (*conn)(void*),int (*recv)(void*));
	virtual int  connect(struct in_addr,int);
	virtual void close(void);
	virtual int  send(void* buf,int len);
	virtual int  recv(void* buf,int* len);
	virtual int  listen( int port);
	virtual int  islisten(int sock);
	virtual int  addsock(int sock,int listen,int (*p1)(void*),int (*p2)(void*));
	virtual int  delsock(int sock);
//	virtual int  setproxy(int sock,int flag);
//	virtual int  isproxy(int sock);
	virtual int  lock();
	virtual int  unlock();

	static SockMan m_sockman;
};

class MSG_IMEXPORT Pipe
{
	int handle[2];
public:
	Pipe();
	virtual ~Pipe();

	int Create();
	int Write(void* buf,int len);
	int Read(void* buf,int len);
	int Close();
};
*/

#if defined (__unix)

#define		TIME_MSG_ID		2003
#define		SyncTimeMsg		1

class MSG_IMEXPORT MsgIPC
{
private:
	int m_msgid;
public:
	MsgIPC(int key);
	virtual ~MsgIPC();

	int send(void* buf,int len);
	int send(int type,void* buf,int len);
	int recv(int type,void* buf,int len);
	int close();
};
#endif

struct _retdata
{
	int order;
	char type;
	VTVALUE_UNION val;
};
class MSG_IMEXPORT HisMultiTypeValue
{


public:
	HisMultiTypeValue();
	virtual ~HisMultiTypeValue();

	int GetValNum() { return rcdnum; }
	int GetVal (int datano, int* parao,char* type,
		int *date, int *time,int *secms,
		VTVALUE_UNION& val);
public:

	struct _retdata *data;
	unsigned short rcdnum;
	int sdate,stime,ms;
};
/*
class MSG_IMEXPORT HisMultiTypeSrv
{
	int wino;
	HisMultiTypeValue val;
public:
	HisMultiTypeSrv();
	~HisMultiTypeSrv();
};
*/

struct _his_data
{
	char name[24];
	char field[24];
	int date;
	int time;
	int ms;
	float val;
	char  type;
};

class MSG_IMEXPORT HisDataStream : public NetDbg
{
	pthread_t dbThread;
	int dbThreadf;

	DBApi m_db;
	int m_cnt;
	int pdrf;

#define YX_YC		0
#define YC			1
#define YX			2

	HisMultiTypeValue* oldval;
	struct _his_win_buf
	{
		SYS_TIME ftime,ttime;
		int paranums,freshperiod,flag,data_ok,fetchflag,data_type,first;
		RDB_FIELD_STRU *fldaddr;

		struct _his_data *yx;
		struct _his_data *yc;
		int yxnum,ycnum;

		int yxidx,ycidx;
		int curdate,curtime,curms;
		int nxtdate,nxttime;
		char pauseflag,endflag;

//		int numcurdata;
		struct _his_data *pcurdata;
#ifdef WIN32
		rdbcalleridtype winhandle;
#endif
	} his_win_buf[WIN_BUF_MAX];
/*	int Check();*/
	int CompareTime(SYS_TIME& time1,SYS_TIME& time2);
	int GetHisData(int winno,DBApi& db,char* lib,SYS_TIME ftime,SYS_TIME ttime,
					struct _his_data **yx,int* yxnum,
					struct _his_data **yc,int* ycnum);
	int MakeHisYcData(int winno);
	int MakeHisYxData(int winno);
	int BackYcData(int winno);
	int BackYxData(int winno);
	int NextYcData(int winno);
	int NextYxData(int winno);
/*	int SortByTime(struct _his_data* pt,int rcdnum);

	void MakeStream(int grpno,char *srv,int wino);
*/

public:
	HisDataStream();
	virtual ~HisDataStream();

	void MakeHisData(int winno);
	int  SetTimePos(int winno,SYS_TIME& timepos);
	void BackData(int winno);
	void NextData(int winno);

	void GetHisData();
	void GetHisData(int winno);
	int SetStreamPara(int pdrflag, SYS_TIME ftime,SYS_TIME ttime,
					  int freshperiod,int paranums,RDB_FIELD_STRU *fldaddr,int winno
#ifdef WIN32
			,rdbcalleridtype winhandle
#endif
		,void (STDCALL *func)(void *buf,int ord,int len,int num) = NULL	);//设置刷新参数
	int StopStream(int winno);
	int PauseStream(int winno);
	int ResumeStream(int winno);

	int GetFieldValue(int winno,HisMultiTypeValue* val);
};

#endif // !defined(AFX_MSG_H__70B2C3DE_A76D_4744_8B3A_B99E1A2877F8__INCLUDED_)
