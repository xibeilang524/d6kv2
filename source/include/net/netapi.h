#ifndef	NET_API_H
#define	NET_API_H
#include "sysstr.h"
#include "netstr.h"
#include <signal.h>
#if defined(__unix) && defined(__alpha)
#include <mach/machine.h>
#endif

#if defined(WIN32)
	#include <pthread/pthread.h>
	#include <stdio.h>
	#ifndef TRACE
		void IMEXPORT TRACE(char *format, ...);
	#endif
#else
//		void IMEXPORT TRACE(char *format, ...);
	#define TRACE printf
#endif

void  IMEXPORT	_delay_time(int n);
void  IMEXPORT	_byte_swap(int ilen,void *addr);
int	  IMEXPORT 	_get_last_error();
void  IMEXPORT	_get_host_name(char *);							//取主机名

#ifdef WIN32
// 进程互斥类
class IMEXPORT dsMutex 
{
protected:
    void * m;

public:
	dsMutex(char const* name);	
	~dsMutex();

	void lock();
	void unlock();
	bool locked();
	bool trylock();
};

class IMEXPORT dsMutexLock
{
	dsMutex * mtx;

public:
	dsMutexLock(dsMutex & m);
	~dsMutexLock();

    dsMutex *mutex() const;
};

#define DECLARE_MUTEX(a) static dsMutex _mutex##a;
#define IMPLEMENT_MUTEX(a) dsMutex a::_mutex##a(#a);
#define TRYLOCK(a) 

//#define TRYLOCK(a) dsMutexLock _lock##a(_mutex##a)
#endif
//调试信息类
class IMEXPORT NetDbg
{
public:
	NetDbg(void);
	virtual ~NetDbg(void);

	virtual	int RegProc(char* proc_asc);
	virtual	int UnRegProc();
	virtual	int UnRegProc(int idx);
	virtual	int RegType(int type,char* desc);
	virtual	char* GetDesc(int type);

	virtual	int DbgProc(char *proc_asc);					//for debug  proc
	virtual	int DbgProc(int idx);							//for debug  proc
	virtual	int GetProcEnm(char proc_asc[][DESC_LEN]);
	virtual	int* GetProcPidEnm(){ return pid; }
	virtual int* GetRegIndex(){	return index;	}			//for debug  proc
	virtual	int GetTypeEnm(int procInex,char type_asc[][DESC_LEN]);
	virtual int* GetTypeIndex(){	return typeno;	}

	virtual char *GetDbg(int* typeno,char* type,char* desc);
	virtual void PutDbg(int type,char *format,...);
	virtual void PutDbgHex(int type,void *buf,int len);

	virtual void Clear();
private:
	void	RegSysType();
	static DBG_DB *DebugDB;
	int readp;	
	static int reg_index;		//注册号
	int	 m_regIdx;				//for dbg
	static int objnum;
	static int reg_pid;
//	int dbgf;

	//for show info
	int pid[DEBUG_MAX_PROC];
	int index[DEBUG_MAX_PROC];
	int typeno[DEBUG_MAX_TYPE];
#ifdef WIN32
	DECLARE_MUTEX(NetDbg)
#endif
};


class IMEXPORT ZipData : public NetDbg
{
	//char decode_stack[ZIP_TABLE_SIZE];
	char *decode_stack;
	unsigned short int next_code;
	unsigned char  current_code_bits;
	unsigned short int next_bump_code;
	DICTIONARY *dictp;
	BIT_STREAM *tbit_out,*tbit_in;
	int   tbitout_maxsize;
	char  *tbitout_stream;
	int   toutput_maxsize;
	char  *toutput;

	void InitializeDictionary(void);
	BIT_STREAM *InitializeCompress(int outputlen);
	unsigned short int find_child_node(unsigned short int parent_code, unsigned short int child_character);

	int OutputBits(BIT_STREAM *bit_out,unsigned  int code,unsigned char bit_count,
				int inputLength,int *memsizep,char **output_firstp);
	BIT_STREAM *InitializeExpand(char* input);
	unsigned short int decode_string(unsigned short int count,unsigned short int code);
	unsigned  int InputBits(BIT_STREAM *bit_in,unsigned char bit_count);
	char *ziprealloc_bitoutstream(int cursize)
	{
		if(cursize<=tbitout_maxsize)return tbitout_stream;
		tbitout_maxsize=cursize+(4096-cursize%4096);
		if(tbitout_stream)
			tbitout_stream=(char *)realloc(tbitout_stream,tbitout_maxsize);
		else
			tbitout_stream=(char *)malloc(tbitout_maxsize);
		return(tbitout_stream);
	}
	char *ziprealloc_output(int cursize)
	{
		if(cursize<=toutput_maxsize)return toutput;
		toutput_maxsize=cursize+(5120-cursize%5120);
		if(toutput)
			toutput=(char *)realloc(toutput,toutput_maxsize);
		else
			toutput=(char *)malloc(toutput_maxsize);
		return(toutput);
	}
public:
	ZipData(void);
	~ZipData(void);
	int CompressData(char *input,int inputLength,char **outputp,int *outputLengthp);
	int IsCompressData(char *input,int inputLength);
	int ExpandData(char *exinput,int exinputLength,char **outputp,int *outputLengthp);
};


//网络信息查询类
class IMEXPORT NetInfo  
{
public:
	virtual char *GetProcessStatus(int);
	virtual int GetHostTypeNum();
	virtual char * GetHostStatus(int n);
	virtual char * GetNetStatus(int n);
	virtual char * GetHostType(int n);
	virtual char * GetErrType(int n);
	virtual char * GetGramType(int n);
	virtual char * GetNetType(int n);
	virtual char * GetMailType(int n);
	virtual char * GetEventType(int n);
	NetInfo();
	virtual ~NetInfo();
private:
	char asc[100];
};

//内存管理类
class IMEXPORT MemMan  
{
public:
	MemMan(int);
	MemMan(void);
	virtual ~MemMan(void);

	virtual int		GetMemLen(void) { return mem_len; }
	virtual void *	GetMemPtr(void) { return ptr; }
	virtual void *	ResizeMem(int);

	operator int () { return mem_len; }
	operator void * () { return ptr; }
	void operator = (int n) { ResizeMem(n); }
private:
	void * ptr;
	int mem_len;
};

//多个数据区管理类
class IMEXPORT MakeBuf : public MemMan
{
public:
	virtual void ClearBuf(void);
	virtual int AddBuf(int,void *);
	virtual int GetBuf(int,void *);
	virtual void * GetBuf(int,int *);
	virtual int64u GetBufNum(void) { memmove(&buf_num,GetMemPtr(),sizeof(buf_num)); return buf_num; }
	MakeBuf();
	virtual ~MakeBuf();

private:
	int64u buf_num;
};

//文件类
class IMEXPORT FileLocal : public NetDbg
{
public:
	FileLocal(char *);
	virtual ~FileLocal();
	virtual int Write(void *,int);
	virtual int Read(void *,int);
	virtual int GetLen(void);
	virtual void Delete();
	void operator = (char *);
	virtual void Append(char *asc,int len);

private:
	void SetName(char *name);
	char filename[100];
	char file1[100],name2[100];
	char *homedir;
};

class IMEXPORT ShareMem {
public:
        int CreateShm(int  shmkey,int  shmlen);
        void *MappingShm(int shmkey,int  shmlen);
        int UnmapShm(void *shmaddr);
        int DeleteShm(int shmkey);
        void *CreateAndMapping(int shmkey,int  shmlen,int *createf=NULL);
};

//网络应用共享内存管理类
class IMEXPORT ShmNetDB
{
public:
	ShmNetDB();
	virtual ~ShmNetDB();
	virtual int MakeMem();
	virtual int FreeMem();
	virtual NET_SHM_DB *ShmDB() { return shm_ptr1; }
	virtual PORT_DB* PortDB() {	return &shm_ptr1->portdb; }
	virtual HISEVT_DB* HisEvtDB() { return &shm_ptr1->hisevtdb; }
	virtual NET_DB *NetDB() { return &shm_ptr1->netdb; }
	virtual MAIL_DB *MailDB() { return &shm_ptr1->maildb; }
	virtual PROC_DB *ProcDB() { return &shm_ptr1->procdb; }
	virtual AUTO_DB *AutoDB() { return &shm_ptr1->autodb; }
	virtual RTDB_DB *RtdbDB() { return &shm_ptr1->rtdbdb; }
	virtual EVENT_DB *EventDB() { return &shm_ptr1->eventdb; }
	virtual LINK_DB *LinkDB() { return &shm_ptr1->linkdb; }
	virtual DBG_DB *DbgDB() { return &shm_ptr1->dbgdb; }
	virtual TIME_DB *TimeDB() { return &shm_ptr1->timedb; }
	virtual DEAD_DB *DeadDB() { return &shm_ptr1->deaddb; }
	virtual GRPVRT_DB *GrpVrtDB() { return &shm_ptr1->grpvrtdb; }
	virtual char *HostName() { return shm_ptr1->hostname; }
private:
	NET_SHM_DB *shm_ptr1;
#if defined(__unix)
private:
	int shmID;
#elif defined(WIN32)
private:
	HANDLE handle;
#endif
};

//网络状态管理类
class IMEXPORT NetStatus : public NetDbg
{
public:
	NetStatus();
	virtual ~NetStatus();

	int IsCenter(int groupno);								//self
	int IsCenter(int groupno,int n);						
	int IsMainCenter(int groupno);	
//	int IsServer(int n,char *srv_name);
	int IsServer(int groupno,int ord,char *srv_name);
	int IsServer(int groupno,char *srv_name);				//self
	int IsMirrorServer(int groupno,int ord,char *srv_name);
	int IsMirrorServer(int groupno,char *srv_name);			//self
	int IsMainServer(int groupno,char *srv_name);			//self
	int IsMainServer(int groupno,int ord,char *srv_name);
	int IsMainFileServer(int groupno);						//self
	int IsMainFileServer(int groupno,int n);
	int IsServerOk(int groupno,char *srv_name);

	virtual int GetHostNetA_Addr(int,struct in_addr *);
	virtual int GetHostNetA_Addr(char* host,struct in_addr *addr);
	virtual int GetHostNetB_Addr(int,struct in_addr *);
	virtual int GetHostNetB_Addr(char* host,struct in_addr *addr);
	virtual int GetHostCmdNetAddr(int n, struct in_addr *addr);
	virtual int GetHostDataNetAddr(int n, struct in_addr *addr);

	char* GetMainFileServerNode(int groupno);
	char* GetMainFileServerHost(int groupno);
	int GetMainFileServerHostNo(int groupno);
	int GetMainCenter(int groupno);
	char *GetMainServerNode(int groupno,char *srv_name);
	char *GetMainServerNode(int groupno,unsigned int n);
	char *GetMainServerHost(int groupno,char *srv_name);
	char *GetMainServerHost(int groupno,unsigned int n);

	char *GetHostName(int n);
	char *GetHostName(struct in_addr);
	char *GetHostName(int groupno,char* node);
	char *GetHostName(int groupno,int n,char *srv_name);
	char *GetNodeName(int groupno,char* host);
	char *GetNodeName(int groupno,int n,char *srv_name);
	char *GetServerName(int groupno,unsigned int n);

	virtual int GetNetState(int groupno,char *host,int *flaga=NULL,int *flagb=NULL);
	virtual int GetHostNetNoAddr(int groupno,char *host,int nettype,struct in_addr *addr=NULL);
	virtual int GetHostNetNoAddr(int n,int nettype,struct in_addr *addr=NULL);

	virtual int GetRtdbNum(int groupno);
	virtual int GetGrpVritualNo(int groupno);

	virtual int GetHostInfo(char *host,char* srv_name=0);
	virtual int GetHostNoNum(){ GetHostInfo(m_hostname); return m_selfnum; }
	virtual int* GetHostNoEnm(){ GetHostInfo(m_hostname); return m_selfno; }
	virtual int GetHostGrpNum(char* srv_name=0);
	virtual int* GetHostGrpEnm(char* srv_name=0);

	virtual int GetHostGrpNo(int hostno);
	virtual int GetHostNo(int groupno);
	virtual int GetHostNo(int groupno,char *host);
	virtual int GetHostNo(char *host);
	virtual int GetHostNo();
	virtual int GetHostNo(int groupno,int n,char *srv_name);

	virtual void SetMainServer(int groupno,int n,char *host);
	virtual void SetMainServer(int groupno,char *db,char *host);
	virtual void SetMainCenter(int groupno,int newmain);

	virtual int HostNum(void) { return netdb->hostnum; }
	virtual RTDB_DB *RtdbDB() { return rtdbdb; }

	virtual void CheckRtdb();
	virtual void CheckRtdb(int groupno);
	virtual void CheckDB();
	virtual void CheckDB(int groupno);
	virtual void CheckHostState(int groupno);

	virtual int GetNodeRstate(int groupno,char *name);
	virtual int GetHostRstate(int groupno,char *name);
	virtual int GetHostRstate(int hostno);
	virtual NET_DB *NetDB() { return netdb; }
	virtual int32u GetTime(void) { return netdb->time; }
	HOST & operator [](int);
	virtual void PutNetDB(int groupno,NET_DB *db);
private:
	int m_selfno[GROUP_MAX];
	int m_selfnum;
	int m_grpno[GROUP_MAX];
	int m_grpnum;
	NET_DB *netdb;
	RTDB_DB *rtdbdb;
	GRPVRT_DB *grpvrtdb;
	char m_hostname[HOSTNAMELEN];
	virtual void MakeHostEvent(int groupno,int soetype,int n);
	virtual void MakeServerEvent(int groupno,int n);
	void MakeNetSame(int nn);
};

class IMEXPORT Tcp : public NetDbg 
{
public:
	Tcp(void);			//for tcp client
	Tcp(int socketid);	//for tcp server
	virtual ~Tcp();
	virtual int Create();
	virtual int Connect(struct in_addr,int);
	virtual int Listen( int ,int backlog=SOMAXCONN); 
#ifdef _AIX
	virtual int	Accept(struct sockaddr *, unsigned long *);
#elif __linux
	virtual int	Accept(struct sockaddr * addr, socklen_t * addrlen);
#else
	virtual int	Accept(struct sockaddr *, int *);
#endif
	virtual int Send(void *,int);
	virtual int Recv(void *,int);
	virtual void Close(void);
	virtual int IsReadable(int sec = 0,int usec = 0);
	virtual int IsWritable(int sec = 0,int usec = 0);
	virtual int	GetSocket(void) { return socket_id; }
	virtual char SocketStatus();
	virtual int  IsValidAddr();
protected:
	int	socket_id;
	struct in_addr m_addr;
	int32s m_port;
};


class IMEXPORT Tcpip: public Tcp
{
public:
	Tcpip(int,int,int port=CMDPORT);
	Tcpip(void);
	virtual ~Tcpip();
	virtual int Create(void);
	virtual int LinkTo(int,int,int port=CMDPORT);
	virtual int LinkTo(struct in_addr,int,int port=CMDPORT);
	virtual int LinkTo(char *szAddr,int port);
	virtual int TxStream(HEAD *head, void *buf);
	virtual int TxStream(int grpno,char *user,int cmdtype,int user1,int user2,int len,void *buf);
	virtual int TxData(void *buf, int size);
	virtual int TxHead(HEAD *head);
	virtual int RxData(void *,int);
	virtual int RxHead(HEAD *head);
	virtual void Close(void);
	int net_no;
	NetStatus cfg;
private:
	virtual int Connect(struct in_addr,int port=CMDPORT);
	char trgname[HOSTNAMELEN];
protected:
	char hostname[HOSTNAMELEN];
	struct sockaddr_in txaddr;
//	DeadMan  deadman;
};


class IMEXPORT Udp : public NetDbg
{
public:
	Udp();
	virtual ~Udp();
	virtual int Create();
	virtual int Sendto(void *buf, int len,struct in_addr *addr,int port=MONPORT,int flags=0);
	virtual int Recvfrom(void *buf, int len,int& retlen,struct sockaddr_in* rxaddr,int port=MONPORT,int flags=0);
	virtual void Close(void);
	virtual int IsReadable(int sec = 0,int usec = 0);
	virtual int IsWritable(int sec = 0,int usec = 0);
	virtual int GetSocket(void) { return socket_id; }
	virtual int Listen(int port, int backlog=SOMAXCONN);
protected:
	int socket_id;
};

class IMEXPORT Udpip: public Udp
{
public:
	Udpip();
	virtual ~Udpip();
	virtual int SendGram(void *,int,struct in_addr *);
	virtual void SendToServer(int groupno,int len,void *buf,char *servername);
private:
	virtual int Create();
	char hostname[HOSTNAMELEN];
};

//邮件管理类
class IMEXPORT MailMan : public NetDbg,private MemMan
{
public:
	virtual void Check();
	virtual int Get(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,int typenum,int type[],MAILPROCTYPE pid = 0,int mail_level=0);	//pid != 0,取本机指定进程的指定邮件 pid == 0,取本机本进程的指定邮件
	virtual int Get(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,int *type,int mail_level=0);	//取本机本进程的一个邮件
	virtual int Put(MAIL_HEAD *,MAIL_HEAD *,int,void *,int,int mail_level=0);
	virtual void Clear(void);
	MailMan();
	virtual ~MailMan();
	struct MAIL_DB::mail_t & operator [](int n) { return maildb->mail[n];}
private:
	MAIL_DB * maildb;
	virtual int Get(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,MAILPROCTYPE pid,char *host,int *type,int mail_level=0);	//取指定主机指定进程的邮件
	int readp;
protected:
	char hostname[HOSTNAMELEN];
#ifdef WIN32
	DECLARE_MUTEX(MailMan)
#endif
};

class IMEXPORT DeadMan : public NetDbg
{
public:
	DeadMan(void);
	virtual ~DeadMan();
	virtual int Put(int grpno,int port,void* deadbuf,int size);
	virtual int Put(int grpno,int srvno,int port,void* deadbuf,int size);
	virtual int Put(struct in_addr addr,int port,void* deadbuf,int size);
	virtual int StartSendThread();
	virtual void StopSendThread();

	virtual void SendDeadLoop();
private:
	virtual int Get(DEADBUF_HEAD* deadbuf_head,void** deadbuf,int* size);
	virtual int IsSpaceEnough(int len);

	virtual int  Lock();
	virtual int  Lock(int msec);
	virtual void UnLock();

	int   FindSysPos();
	void  MemCpyIn(char* des,char* src,int len);
	void  MemCpyOut(char* des,char* src,int len);
	void  TestMemCpyOut(char* des,char* src,int len);
//	void* GetMemCpyOut(int &len);

	DEADBUF_HEAD	bufhead;
	DEAD_DB*	deaddb;
	char*	buf;

	int    threadf;
	pthread_t thread;

#ifdef WIN32
	DECLARE_MUTEX(DeadMan)
#endif
};

//进程主窗口管理
class IMEXPORT WndMan
{
public:
#if defined(WIN32)
	virtual void SetMainWnd(HWND hwnd) { main_hwnd = hwnd; }
	virtual HWND GetMainWnd();
#elif defined(__unix)
	virtual void SetMainWnd(int pid) { main_id = pid; }
	virtual int GetMainWnd(void);
#endif
	WndMan();
	virtual ~WndMan();

private:
#if defined(WIN32)
	HWND main_hwnd;
#elif defined(__unix)
	int main_id;
#endif
};

//进程管理类
class IMEXPORT ProcMan  : public NetDbg
{
public:
	int killAProc(char *name);
	virtual int CheckParentProc(void);
	virtual void SendSignal(char *,int);
	virtual void KillAll();
	virtual void EndProc();
	virtual void CheckProc(int groupno);
	virtual void EndProc(int groupno);
	virtual void Clear() { KillAll(); }
	virtual void StartProc(char *asc_name,char *ch_name,char *username,
#if defined(WIN32)
		HWND hwnd=NULL, 
#endif
		char *hn = NULL,int netbusflag=FALSE);

	virtual int IsProcExist(char *);
	ProcMan();
	virtual ~ProcMan();
#if defined(__unix)			//close shared server sock in child process
	static int net_mon_sock,net_cmd_sock,net_link_sock;//to be closed after fork()
	void SetNetbusSock(int mon_sock,int cmd_sock,int link_sock); 
#endif
private:
	static int ProcOrder;
	static int netbusCount;
	static int netbus;
	void ExecProc(int);
	void ExecProc(char *name,char *p1,char *p2,char *p3,char *p4,char *p5);
	virtual void CreateProcEvent(char *asc,char *ch,short status);
	virtual void CreateProcEvent(int groupno,char *asc,char *ch,short status);
	PROC_DB * procdb;
	AUTO_DB * autodb;
	RTDB_DB * rtdbdb;
	NetStatus cfg;
	char hostname[HOSTNAMELEN];
#ifdef WIN32
	DECLARE_MUTEX(ProcMan)
#endif
};

/////////////////////////////////////////////////////////////////////
//事件管理类
class IMEXPORT EventProc : public NetDbg
{
public:
	virtual int EventProcLoop(void);
	virtual int SetProcFunc(void (STDCALL *func)(SYS_EVENT *buf,int num));
	virtual int EventP(void) { return readp; }
	virtual int EventNum(void) { return eventdb->num; }
	virtual EVENT_DB *EventAddr(void) { return eventdb; }
	virtual void Clear();
	void Put(SYS_EVENT *buf,int num);
	virtual int Read(SYS_EVENT *buf);
	virtual int Save(SYS_EVENT *buf, int num);
	virtual int Save(int groupno,SYS_EVENT *buf,int num);
	virtual int Save(int groupno,SYS_EVENT_EX *buf,int num);
	virtual void SendEvent(SYS_EVENT *evt,int event_num);
	void GetEventTime(SYS_EVENT *buf);
	EventProc();
	virtual ~EventProc();

private:
	void (STDCALL *ProcFunc)(SYS_EVENT *buf,int num);
	void PutToDead(HEAD* head,void* buf);

	int readp;
	int flag;
	char hostname[HOSTNAMELEN];
	EVENT_DB *eventdb;
	NetStatus cfg;
	DeadMan   deadMan;
	pthread_t thread;
#ifdef WIN32
	DECLARE_MUTEX(EventProc)
#endif
};

class IMEXPORT DeviceMan : public NetDbg  
{
public:
	int		SendDeviceStatus(int groupno,char *name,int8u status);	//向SCADA服务器发送一个设备状态信息

	//向SCADA服务器发送一组设备状态信息
	//过程：首先使用函数AddDevice将每个设备状态放入内存，然后调用函数SendDevices发送
	//发送结束后已经放入内存的设备状态信息会自动清除
	void	ResetDevice(void);	//清除已经放入内存的设备状态信息
	void	AddDevice(char *name,int8u status);
	int		SendDevices(int groupno);

	DeviceMan();
	virtual ~DeviceMan();

private:
	char	hostname[HOSTNAMELEN];
	int		i;
	NetStatus cfg;
	MakeBuf makebuf;
};

//数据流请求类
class IMEXPORT NetStream : public Tcpip, private MakeBuf  
{
#define STREAM_SRV_MAX			256

public:
	NetStream(int groupno,char *);
	NetStream(int groupno);
	virtual ~NetStream();

	virtual int IsDataOk(int winord);
	virtual int GetData(void **buf,int winord,int *len = 0);
	virtual int GetLinkStatus(void){ return link_ok;}
	virtual int SetProcFunc(void (STDCALL *func)(void *buf,int ord,int len,int num));
	virtual void CheckLink(void);
	virtual void RecvLoop(void);
	virtual void operator = (char *);
	virtual int StopFresh(unsigned char winord=0xff);
	virtual int CallData(int,char *,char *,int,int,void *,int
#if defined(WIN32)
		,HWND win = NULL
#endif
		);	//召唤数据库中的数据

private:
	virtual int LinkSrv();
	virtual int SndWinSrc(unsigned int wirord);
	virtual void Reset();
	virtual void MakeStream(char *name=NULL);
	virtual int AddStreamObj(NetStream* obj);
	void (STDCALL *ProcFunc)(void *buf,int ord,int len,int num);
public:
	static NetStream* cur_check;
	static NetStream* streamObj[STREAM_SRV_MAX];

private:
//	static NetStream *curr_check_stream;
	int mainsrv;
	HEAD txhead,rxhead;
	int link_ok,rxnum;

	char username[USER_NAME_LEN];
	char password[PASSWORD_LEN];
	char server[RTDB_NAME_LEN];
	int  grpno;

	struct _win_buf_str
	{
		MakeBuf data_buf;
		void *txptr;
		int data_ok,src_ok,method;
		int rxlen,txlen,period;
#if defined(WIN32)
		HWND win;
#endif
	} win_buf[WIN_BUF_MAX];

private:
	int checkf;
	pthread_t recvThread;
	static pthread_t checkThread;
	static int bThreadStart;
};

/////////////////////////////////////////////////////////////////
/*
#if defined(__unix)
//消息队列管理
class MsgQueue
{
public:
	MsgQueue(int);
	virtual int Send(struct msgbuf *,int);
	virtual int Send(int,void *,int);
	virtual int Recv(void *buf,size_t len,long type,int flag)
	{ return msgrcv(msgid,buf,len,type,flag); }
private:
	int msgid;
};
#endif
*/
class IMEXPORT NetControl : public MemMan,NetDbg
{
public:
	void FreshDataStream(void);
	NetControl();
	virtual ~NetControl();
	virtual int SwitchServer(int groupno,char *dbname,char *host);
	virtual int FreshAutoProc(int,void *);
	virtual int ProcCtrl(int groupno,char *host,char *procname,int flag=FALSE);
	virtual PROC_DB *GetProcInfo(int groupno,char *host);
	virtual int KillProc(int groupno,char *host,MAILPROCTYPE pid);
	virtual int CheckTime(int groupno,int,int,int,int,int,int,int);
	virtual void SendRawEvent(int groupno,void *txbuf, int len);
	virtual int SwitchHost(int groupno,int type,int n);
	virtual int SwitchHost(int groupno,int type,char *);
	virtual int SwitchHost(int groupno,char *);
	virtual int RestartBus(int groupno,char *host);
private:
	virtual void SendToServer(int groupno,void *buf);
	virtual void SendToAll(int groupno,void *buf);
	int hostnum;
	HEAD txhead;
	HEAD rxhead;
	NetStatus cfg;
	NET_DB *netdb;
//	Udpip udpip;
};

//邮件类
class IMEXPORT MailBox : public MailMan
{
public:
	MailBox() ;
	MailBox(int groupno,char *name);
	virtual ~MailBox();

//	virtual void operator = (char *name);
	virtual void SetSrvName(int groupno,char *name);


	//客户端发送服务请求
	virtual int SendSrvRequest(char *user,void *buf,int len,int main_only=TRUE,int mail_level=0);	//send mail to server,return TRUE if OK,called by client when request service
	virtual int SendSrvRequest(char *user,void *buf,int len,char *name,int mail_level=0);	//向指定的服务器节点发送服务请求

	//3区发送服务器请求
	virtual int SendPubSrvRequest(char *user,void *buf,int len,int main_only=TRUE,int mail_level=0);	//向指定的3区服务器节点发送服务请求


	//服务器端接收服务请求
	virtual int RecvSrvRequest(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,char* srvname,int groupnum,int* groupno,int mail_level=0);	//get mail, return TRUE if mail exists,called by server
	//服务器端发送服务结果
	virtual int SendSrvResult(MAIL_HEAD *trg,void *buf,int len,int mail_level=0);	//send mail to client,return TRUE if OK,called by server when result is to be sent
	//客户端接收服务结果
	virtual int RecvSrvResult(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,char* srvname,int groupnum,int* groupno,int mail_level=0);	//get result mail, return TRUE if mail exists,called by client
	virtual int RecvSrvResult(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,int maillevel=0);

	//向指定主机发送通用邮件，不要求该主机是某一服务器,客户端调用
	int SendSrvRequestToHost(char *user,void *buf,int len,char *name,int mail_level=0);	    //CLIENT CALL
	//取发送到本机的通用邮件，服务器端调用。
	int RecvSrvRequestFrmClient(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,int mail_level=0); //server call
	//服务器端发送通用邮件给客户端，服务器端调用。
	int SendSrvResultToClient(MAIL_HEAD *trg,void *buf,int len,int mail_level=0);	
	//客户端接收通用服务结果，客户端调用
	int RecvSrvResultFrmHost(MAIL_HEAD *src,MAIL_HEAD *trg,void **buf,int *len,int  mail_level=0);	//get result mail, return TRUE if mail exists,called by client

//邮件处理回调
	virtual void SrvProcLoop(void);
	virtual void ReqProcLoop(void);
	virtual int SetSrvProcFunc(void (STDCALL *func)(MAIL_HEAD *src,MAIL_HEAD *trg,void *buf,int len),int mail_level=0);
	virtual int SetReqProcFunc(void (STDCALL *func)(MAIL_HEAD *src,MAIL_HEAD *trg,void *buf,int len),int mail_level=0);

private:
	int mailtype;
	int grpno;
	int mail_level;
	char server_name[CODE_LEN];
	NetStatus cfg;

	virtual int SendToAllServer(char *user,void *buf,int len,int mail_level);
	virtual int SendToMainServer(char *user,void *buf,int len,int mail_level);
	virtual int SendMail(char *user,void *buf,int len,MAIL_HEAD *trg,int mail_level=0);
	virtual	int SendPubMail(char *user,void *buf,int len,MAIL_HEAD *trg,int maillevel=0);

	virtual void SetMailType();

//邮件处理回调
	void (STDCALL *SrvProcFunc)(MAIL_HEAD *src,MAIL_HEAD *trg,void *buf,int len);
	void (STDCALL *ReqProcFunc)(MAIL_HEAD *src,MAIL_HEAD *trg,void *buf,int len);
	int SrvFlag,ReqFlag;
	virtual void ClearThread(void);
	pthread_t SrvThread,ReqThread;
};

//实时数据库操作类
//polling rtdb
enum RtdbOpt
{
	RTDB_GET = 0,			/*取数据域*/
	RTDB_PUT,				/*修改数据域*/
	RTDB_UPDATE,			/*SQL之UPDATE命令*/
	RTDB_SELECT,			/*SQL之SELECT命令*/
	RTDB_RECALL,			/*加载一个表*/
	RTDB_MRECALL,			/*加载多个表*/
	RTDB_RELOAD,			/*重新加载实时数据库*/
	RTDB_MODE,				/*装载库模式*/
	RTDB_SRV				/*对所有服务器操作*/
};

class IMEXPORT RtdbProc : public MakeBuf,NetDbg
{
public:
	int Select(int groupno,char *user,void *txbuf1,int txlen1,void *txbuf2,int txlen2,void **rxbuf1,int *rxlen1,void **rxbuf2,int *rxlen2) { return TRUE; }
	int Update(int groupno,char *user,void *tx1,int len1,void *tx2,int len2,int *ret) { return TRUE; }
	int SrvRequest(char *,char *,char *,void *,int,void **,int *,int *,int flag = TRUE);
	void * GetField(int groupno,char *,char *,char *,int,int,void *,int *,int *);
	int PutField(int groupno,char *,char *,char *,int,int,void *,int,void *,int *);
	int Update(int groupno,char *,char *,char *,void *,int,void *,int,void *,int,int *);
	int Select(int groupno,char *,char *,char *,void *,int,void *,int,void *,int,void **,int *,void **,int *,void **,int *,int *);
	int DasOperation(char *user,void *txbuf,int txlen,void **rxbuf,int *rxlen);
	int Recall(char *user,char *password,char *server,char *name);
	int ReloadRtdb(char *user,char *password,char *server);
	int ReloadMode(char *user,char *pass,int load_type);

	RtdbProc(void);
	virtual ~RtdbProc(void);
private:
	int Recall(int groupno,char *,char *,char *,char *);
	int ReloadRtdb(int groupno,char *,char *,char *);
	int ReloadMode(int groupno,char *,char *,int);

	NetStatus cfg;
	HEAD txhead;
	HEAD rxhead;
};

/////////////////////////////////////////////////////////////////////
//系统文件管理类
enum FileDef
{
	FILEMAX = 1024,
	FILENAMELEN = 256
};

enum FileOpt
{
	CallFileInformation = 0,
	SaveFileReq,
	DelFileReq,
	SaveFileSrv,
	DelFileSrv,
	RemoveFileReq,
	RemoveFileSrv,
};

enum FileStatus 
{
	FILE_STATUS_READY = 0, 
	FILE_STATUS_OK
};

typedef struct
{
	int32u num;
	struct file_db_data_t
	{
		char name[FILENAMELEN];
		char hostname[HOSTNAMELEN];
		int32u len,save_time;
	}data[FILEMAX];
}FILE_DB;

class IMEXPORT FileMan : public NetDbg,private MakeBuf
{
public:
	int GetNewFile(void);
	int Delete(char *name);
	int Save(char *name,void *buf,int len,int tableflag=0);
	FileMan();
	virtual ~FileMan();

private:
	HEAD		rxhead;
	HEAD		txhead;
	NetStatus	cfg;
};

//服务应答及信息收发类
class IMEXPORT SocketProxy : public Tcpip
{
public:
	SocketProxy();
	SocketProxy(int);
	virtual ~SocketProxy();

	int RxData(void *,int);
	int TxData(void *buf, int size);
	int RxHead(HEAD *head);
	int TxHead(HEAD *);
	int TxStream(HEAD *,void *);
	int TxStream(int grpno,char *user,int cmdtype,int user1,int user2,int len,void *buf);
	virtual void CloseSocket();
	virtual void GetNetNo();

	virtual int GetSocket(void) { return socket_id; }
	operator int () { return socket_id; }
	void operator = (int n);
	void SetSocketInit(int n);
	
private:
	struct sockaddr_in txaddr;
};

//服务应答及信息收发类
class IMEXPORT Proxy : public Tcpip
{
public:
	Proxy();
	Proxy(int);
	virtual ~Proxy();

	int RxData(void *,int);
	int TxData(void *buf, int size);
	int RxHead(HEAD *head);
	int TxHead(HEAD *);
	int TxStream(HEAD *,void *);
	int TxStream(int grpno,char *user,int cmdtype,int user1,int user2,int len,void *buf);
	virtual void CloseSocket();
	virtual void GetNetNo();

	virtual int GetSocket(void) { return socket_id; }
	operator int () { return socket_id; }
	void operator = (int n);
	void SetSocketInit(int n);
	
private:
	struct sockaddr_in txaddr;
};


//时间处理类
class IMEXPORT TimeProc : public NetDbg
{
public:
	virtual int GetTime(SYS_TIME &val);
	virtual int SetTime(SYS_TIME &val);
	virtual int32u ConvertTime(SYS_TIME& val);			//convert from time to 1970.1.1
	virtual SYS_TIME ConvertTime(int32u& val);			//convert  from 1970.1.1 to time
	virtual SYS_TIME ConvertTime(time_t val);

	TimeProc();
	virtual ~TimeProc();
	void	Init();
private:
	TIME_DB* time_db;
#ifdef WIN32
	DECLARE_MUTEX(TimeProc)
#endif
};

class IMEXPORT CEventMan : public NetDbg
{
public:
    CEventMan();
    ~CEventMan();

	void  StartSndEvent();
	void  StopSndEvent();

	int   AddSevent(SYS_EVENT * ,int num=1);
	int   AddGevent(HIS_EVENT * ,int num=1);
    int   AddOevent(HIS_EVENT * ,int num=1);

	int	  GetSevent(SYS_EVENT** evtp);
	int   GetGevent(HIS_EVENT** evtp);
	int   GetOevent(HIS_EVENT** evtp);

public:
	EventProc  evtProc;


protected:
	SND_EVENT_BUF* snd_evtp;
    GEN_EVENT_SAVE* gen_evtp;
    OPT_EVENT_SAVE* opt_evtp;

	//get ret evevt buf
	SYS_EVENT sEvent[SND_EVENT_MAX];
	HIS_EVENT gEvent[GEN_EVENT_MAX];
	HIS_EVENT oEvent[OPT_EVENT_MAX];
private:
	pthread_t thread;
#ifdef WIN32
	DECLARE_MUTEX(CEventMan)
#endif
};

class IMEXPORT HostSysInfo : public NetDbg
{
public:
	HostSysInfo();
	virtual ~HostSysInfo();

#if defined(WIN32)
	void Loop();
	HOST_SYS_INFO* GetData(){return &data; }
#elif defined __unix
	void GetHostSysInfo();
#endif
	HOST_SYS_INFO data;

private:
	static int runflag;

#if defined(WIN32)
	static	pthread_t	thread;
	static int			objectnum;
#elif defined(__unix)
	int kmemid,flag;
	long cpu_num;
	MemMan slot1,slot2,cpu;
#ifdef __alpha
	machine_slot *slotp1,*slotp2;
#endif
	int *cpup,machine_slot_len;
	unsigned long slot_addr;
#endif
};

class IMEXPORT ElementMan
{ 
public:
	ElementMan(void){ m_idx = 0; }
	virtual ~ElementMan(){};

	int AddElement(char* element){ 
		if(element==0)		return FALSE;
		if(element[0]=='\0')	return FALSE;
		if(IsExistElement(element)==TRUE)	return FALSE;
		if(m_idx>=(GROUP_MAX*HOST_MAX))	return FALSE;

		strcpy(m_element[m_idx],element);
		m_idx++;
		return TRUE;
	}

	int IsExistElement(char* element){
		if(element==0)		return FALSE;
		if(element[0]=='\0')	return FALSE;
		for(int i=0; i<m_idx; i++){
			if(strcmp(m_element[i],element))	continue;
			return TRUE;
		}

		return FALSE;
	}
private:

	int  m_idx;
	char m_element[GROUP_MAX*HOST_MAX][HOSTNAMELEN];
};

class Signal
{
public:
	Signal(void)
	{
#if defined(__unix)
		for(int i=0;i<32;i++) 
		{
			sighold(i);
		}
#endif
	}
	virtual ~Signal(void)
	{
#if defined(__unix)
		for(int i=0;i<32;i++) sigrelse(i);
#endif
	}
private:
#if defined(__unix)
	void (*sigfunc[32])(int);
#endif
};

#endif		//all of end
