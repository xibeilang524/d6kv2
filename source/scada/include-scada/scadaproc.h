#ifndef   _SCADA_PROC_H
#define   _SCADA_PROC_H
#include  <string.h>

#include    "net/netdef.h"
#include	"rdb/rdbapi.h"
#include	"scd/cmddef.h"
#include    "scd/comnlink.h"
#include	"scadatime.h"
#include    "scadaservice.h"
#include "fes/femailstruct.h"


#define MyUserName	"scada" 
#define MyPassWord	"scada" 

#define	MAX_CHGDATANUM		8192		//变化数据最大长度


//////////////////////////////////////////////////////////////////////////////////////
Rdb_Tab  *GetSubstationRdbTab(void);
Rdb_Tab  *GetAnalogRdbTab(void);
Rdb_Tab  *GetBreakRdbTab(void);
Rdb_Tab  *GetSwitchRdbTab(void);
Rdb_Tab  *GetProtsigRdbTab(void);
Rdb_Tab  *GetStatusRdbTab(void);
Rdb_Tab  *GetDmsStatusRdbTab(void);
Rdb_Tab  *GetKwhRdbTab(void);

///////////////////////////////  定时器    ///////////////////////////////////////////

#define  SCDTIMER_INTERV            1
#define  SCDTIMER_ACCURATE          2

// 定时器函数指针定义
typedef void (*SCDTIMER_CALLBACKPROC)(void);

typedef struct {
	int     time;
	uchar   mode;
	SCDTIMER_CALLBACKPROC   callfunc;
}  SCDTIMER_STRU;


class 
#ifdef WIN32
 _SCD_DLLPORT 
#endif
ScdTimer  {
	static ComnLinkTable<SCDTIMER_STRU>		timertask;
public:
	void SetScdTimer(uchar mode,int time,SCDTIMER_CALLBACKPROC  callfunc);
	void ProcTimerTask(void);
};

/////////////////////////////////////




#define  DAY_TIME_ADD(totaltime,inctime)   ((totaltime+inctime)>=(24*3600-8) ? (24*3600):(totaltime+inctime))


#ifdef WIN32
_SCD_DLLPORT 
#endif
int DataAccord(int groupno,char *tabname);


////////////////////////////////////////////////////////////
//////////// 程序注册表  ///////////////////////////////////
////////////////////////////////////////////////////////////

typedef struct {
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	int		intval;
	float   floatval;
	char    stringval[OBJECTNAME_LEN];
}  PROGAMREG_STRU;

#ifdef WIN32
_SCD_DLLPORT 
#endif
PROGAMREG_STRU	*GetProgramRunPara(char *name);


#ifdef WIN32
_SCD_DLLPORT 
#endif
int					GetProgramRunPara(char *name,int defaultval);

#ifdef WIN32
_SCD_DLLPORT 
#endif
float				GetProgramRunPara(char *name,float defaultval);

//////////////////////////////////////////////////////////
/////////// 时段处理 /////////////////////////////////////
//////////////////////////////////////////////////////////

#define  SCDSDTYPE_ALL		   0
#define  SCDSDTYPE_MONDAY      1
#define  SCDSDTYPE_TUESDAY     2
#define  SCDSDTYPE_WEDNESDAY   3
#define  SCDSDTYPE_THURSDAY    4
#define  SCDSDTYPE_FRIDAY      5
#define  SCDSDTYPE_SATURDAY    6
#define  SCDSDTYPE_SUNDAY      7
#define  SCDSDTYPE_WORKDAY     8
#define  SCDSDTYPE_HOLIDAY     9

/// 时段类型定义结构 
typedef struct {
   ushort   district;		// 时段组(区域)
   ushort   order;			// 序号
   uchar    stmonth;		// 起始月
   uchar    stday;			// 起始日
   uchar    endmonth;		// 终止月
   uchar    endday;			// 终止日
   uchar    type;			// 类型
   ushort   daygroupno;	    // 日时段组
} SCDSDTYPE_STRU;

typedef struct {
   ushort   daygroupno;	// 日时段组
   uchar    sthour;		// 起始时
   uchar    stminute;	// 起始分
   uchar    endhour;	// 终止时
   uchar    endminute;	// 终止分
   uchar    timetype;	// 时段类型
} SCDDAYSDTYPE_STRU;
 



class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdTimePeriodType {
   static  int      opentabf;
   static  Rdb_Tab  scdtimetab;
   static  Rdb_Tab  dayscdtimetab;
   static  Rdb_Tab  timeparayeartab;
   static  Rdb_Tab  timeparaminutetab;
   static  ushort  *timeparayearp;
   static  uchar   *timeparaminutep;
   static  int		maxdistrictnums;
   static  int		maxdaygroupnums;
public:
   ScdTimePeriodType(void);
   int  UpdateTimePeriodParaYearTab(void);
   int  UpdateTimePeriodParaMinuteTab(void);

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  UpdateTimePeriodParaTab(void);
#else
	friend  int  UpdateTimePeriodParaTab(void);
#endif

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
#else
	friend  int  GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
#endif

#if defined(WIN32)
	friend  int  _SCD_DLLPORT  GetCurrentTimePeriodType(ushort district=0);
#else
	friend  int  GetCurrentTimePeriodType(ushort district=0);
#endif

#if defined(WIN32)
	friend  void  _SCD_DLLPORT  FreshCurrentTimePeriodType(void);
#else
	friend  void  FreshCurrentTimePeriodType(void);
#endif
};


//////////////////////////////////////////////////////// 
///////////// 事件控制结构  ////////////////////////////
////////////////////////////////////////////////////////

typedef struct {
		uchar  puteventf;		// 
		int    lasteventno;		//
		uchar  putapeventf;		//
		int    lastapeventno;	//
		uchar  putykquef;		//
		uchar  putrlycmdf;		//
		uchar  addstudentf;		//
} PROG_SHARECTRL_STRU;  

PROG_SHARECTRL_STRU  *GetShareCtrlAddr(void);


//////////////////////////////////////////////////
// FEP服务器通道事件 
typedef struct {
	SYS_CLOCK	entclock;
	uchar		type;
	int		    channo;
	uchar		status;
	uchar       relclasstype;
	char		relname[OBJECTNAME_LEN];
	char		reldescribe[OBJDESCRIBE_LEN];
}  ENT_FORECHANENT_STRU;

// 请求装载数据库表  
typedef struct {
	char	hostname[HOSTNAME_LEN];
	char	username[USERNAME_LEN];
	char	mtabname[TABNAME_LEN];
} ENT_REQLOADTAB_STRU;

// 数据库域修改
typedef struct {
	char			hostname[HOSTNAME_LEN];
	char			username[USERNAME_LEN];
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
	char			fieldname[FLDNAME_LEN];
	uchar			datatype;
	VTVALUE_UNION	putvalue;
} ENT_DBPUTFLD_STRU;

// 数据库域变化    
typedef struct {
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
	char			fieldname[FLDNAME_LEN];
	uchar			datatype;
	VTVALUE_UNION	putvalue;
} ENT_FLDCHG_STRU;

// 人工置数    
typedef struct {
	char			hostname[HOSTNAME_LEN];
	char			username[USERNAME_LEN];
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
    uchar           securegrade;
	uchar			setflag;
	uchar			datatype;
	double			setvalue;
} ENT_MANSET_STRU;

// 五防    
typedef struct {
	char			tabname[TABNAME_LEN];
	char			objectname[OBJECTNAME_LEN];
    uchar           securegrade;
	uchar			setflag;
} ENT_WFBS_STRU;

// 越限 
typedef struct {
	char	tabname[TABNAME_LEN];
	char	objectname[OBJECTNAME_LEN];
	uchar	olgrade;
	float	olvalue;
} ENT_OVERLIMIT_STRU;


#define  SATATUSCHG_BY_FORE			0	// 由前置生成,可产生事项
#define  SATATUSCHG_BY_SCADA		1	// 由SCADA生成,可产生事项，如双遥信、慢遥信等

#define  SATATUSCHG_MAKEENT_YN		10	// 产生事项与不产生事项类型的分界

#define  SATATUSCHG_SCADA_INSIDE	10	// 由SCADA通过全数据比较生成,作为事件补充，不生成事项
#define  SATATUSCHG_SCADA_MANSET	11	// 由SCADA通过全数据比较生成,作为事件补充，不生成事项

// 状态量变化    
typedef struct {
	int         groupno;
	char		tabname[TABNAME_LEN];
	char		objectname[OBJECTNAME_LEN];
	char		substation[SUBSTNAME_LEN];
	SYS_CLOCK	chgclock;
	uchar		oddflag;
	uchar		sgzflag;
	short		staval;
	float       pdata;
	float       qdata;
	float       idata;
	short		zygroupno;
	uchar       maketype;
	uchar       testflag;
} ENT_STATUSCHG_STRU;

// 遥控
typedef struct {
	int         groupno;
	char		tabname[TABNAME_LEN];
	char		objectname[OBJECTNAME_LEN];
	int			securegrade;
	uchar		actype;
	uchar		programykf;
	uchar		procsercode;
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	uhlong		optaskid;
	char		confirmhost[HOSTNAME_LEN];
	char		confirmname[USERNAME_LEN];
	uhlong		confirmtaskid;
	char		entname[ENTNAME_LEN];
} ENT_YK_STRU;

// 遥控命令
typedef struct {
	uchar		ykcmd;
	uchar		ykfangshi;
	int         ykgroupno;            //add
	int			ykrtuno;
	int			ykno;
	int			ykaction;
} ENT_YKCMD_STRU;


// 遥调
typedef struct {
	short       groupno;
	char		objectname[OBJECTNAME_LEN];
	int			securegrade;
	uchar		outtype;
	short		terminalno;
	short		ytno;
	int			ytsercode;
	int			failcode;
	float		setvalue;
	float		curvalue;
	int			outvalue;
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	uhlong		optaskid;
	char		entname[ENTNAME_LEN];
} ENT_YT_STRU;

    
// 开关
typedef struct {
	SYS_CLOCK	entclock;
	char		breakname[OBJECTNAME_LEN];
	uchar		staval;
	uchar		sgf;
	float       pdata;
	float       qdata;
	float       idata;
	uchar       relclasstype;
	char		relname[OBJECTNAME_LEN];
	char		reldescribe[OBJDESCRIBE_LEN];
	uchar       testflag;
} ENT_BREAK_STRU;

// 设置符号标志
typedef struct {
	char		hostname[HOSTNAME_LEN];
	char		username[USERNAME_LEN];
	uchar		setflag;
	CARD_RCD_STRU	cardrcd;
} ENT_SETSPECIALFLAG_STRU;		// 设置符号标志（置/清接地符、危险符、检修符）

typedef struct {
	char	hostname[HOSTNAME_LEN];
    char	username[USERNAME_LEN];
	char	breakname[OBJECTNAME_LEN];
	char	break_replaced[OBJECTNAME_LEN];
	char	mode;
}ENT_LINEREPLACE_STRU;

/*
typedef struct
{
	SYS_CLOCK eclock;
	int  faulttype;
	char startbreakname[OBJECTNAME_LEN];
	char startftuname[OBJECTNAME_LEN];
	char endbreakname[OBJECTNAME_LEN];
	char endftuname[OBJECTNAME_LEN];
}ENT_DADIAGNOSE_STRU;
*/

typedef struct
{
	SYS_CLOCK clock;
	intertime timeflag;
	char      breakname[OBJECTNAME_LEN];
	uchar     zygrpno;
	uchar     causetype;
	char      cause[OBJDESCRIBE_LEN];
    uchar	  preframenums;		// 事故前点数
    uchar	  postframenums;	// 事故及事故后点数
}ENT_STARTPDR_STRU;



typedef struct
{
	int       groupno;
	char	  ophostname[HOSTNAME_LEN];
	char	  operatorname[USERNAME_LEN];
	hlong	  optaskid;
	char	  tabname[40];
}ENT_CCFORMULA_STRU;



// 应用事件结构定义    
typedef struct {
	int			apentcode;
	SYS_CLOCK	apentclock;
	short		apentmsglen;
	char		apentmsg[MAXAPENTMSG_LEN];
} EVENT_STRU;

// 函数指针定义
typedef int (*APENT_CALLBACKPROC)(int apentcode,SYS_CLOCK *,void *,void *);

// 应用事件调度结构定义
typedef struct apentdispatch_stru {
	int							apentcode;
	APENT_CALLBACKPROC			callbackproc;
	void						*argp;
	struct apentdispatch_stru	*next;
} ENT_DISPATCH_STRU;



// 应用事件处理类定义
class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
APEvent: public NetDbg {
		static PROG_SHARECTRL_STRU	*apentquectrl;		// 事件控制结构指针
		static EVENT_STRU			*apeventaddr;		// 应用事件的首地址
		static int				     maxapentnums;		// 最大应用事件个数
		APENT_CALLBACKPROC		     noentcallback;		// 无事件时的处理函数
		void					    *noentcallbackargp;	// 无事件时的处理函数的参数指针
		ENT_DISPATCH_STRU			*apentdproot;		// 分发事件处理函数链的指针
		int		curapeventno;						// 当前的事件号
		// 抓获控制权
		int		GrapPutRight(void);				
		// 释放控制权
		void	ReleasePutRight(void);
		// 增加事件处理函数
        void	AddApEventProc(int selcode,APENT_CALLBACKPROC selproc,void *argp);
		// 调度事件处理函数
        int		DispatchApEvent(int apentcode,EVENT_STRU *apentp);
     public:
        APEvent(void);
        ~APEvent(void);
		// 增加应用事件回调函数
        int		AddApEventCallback(int selcode,APENT_CALLBACKPROC selproc,void *argp,int selcode1,...);
		// 增加一个应用事件(事件代码，事件内容，事件内容长度)
        int		AddApEvent(int apentcode,void *apentmsg,int len);
		// 单步获取事件
        EVENT_STRU	*apentstep(void);
		// 事件循环
        int		ApEventLoop(int noentretf=0);
		// 事件循环
        int		ApEventLoop(int *apentcode,SYS_CLOCK *apentclock,void *apmsg);
		// 指向最新当前事件号
        void	ResetApEvent(void);
     };
     
/////////////////////////////////////////////////////////////////
// 请求启动事故追忆

void  RequestStartPDR(SYS_CLOCK *clockp,intertime timeflag,char *breakname,
					  short groupno,uchar causetype,char *cause,uchar prefnums,uchar postfnums);


//////////////////////////////////////////////////////////////////

intertime 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
GetSysLastTime(void);


void	  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
FreshSysLastTime(void);


/////////////////////////// 主机状态表 //////////////////////////
typedef struct {
	short   groupno;      //分组序号
	char	name[24];     //代码
	char	describe[40]; //描述
	char	hoststatus;   //主机状态
	char	netastatus;   //网络A状态
	char	netbstatus;   //网络B状态
	char	datastream;   //数据流
	char	ctrlstream;   //控制流
	char	netaused;     //网络A使用
	char	netbused;     //网络B使用
	float	cpuload;      //CPU负载
	float	diskused;     //RAM使用率
	float   netload;      //NET负载
	float   ramload;      //磁盘使用
} HOSTSTATUS_STRU;

///////////////////////////  厂站表 /////////////////////////////
typedef struct {
	char	name[SUBSTNAME_LEN];       //代码
	char	describe[OBJDESCRIBE_LEN]; //描述
//	uchar	type;
//	short	stationno;
	uchar	sgzdeadband;               //事故总死区
	uchar	status;                    //状态
	short	saferunstartyear;          //安全起始年
	short   saferunstartmonth;         //安全起始月
	short   saferunstartday;           //安全起始日
	short	saferundays;               //安全运行天数
//	short   ycinconsist_entmode;
//	short   yxinconsist_entmode;
//	short   deaddata_entmode;
	float	deadscale;                 //死数比例
#ifdef QTGRAPH
	char    graphname[255];//接线图名
#else
	char    graphname[OBJDESCRIBE_LEN];//接线图名
#endif
	short   groupno;                   //分组序号

	uchar	overlimitf;             //YC越限
	uchar   ycinconsistf;           //YC可疑
	uchar   yxinconsistf;           //YX可疑
	uchar   deadflag;               //死数
	uchar   chanerror;              //通道故障
	uchar	terminalerror;          //终端故障
	uchar	invalidf;               //YC值非法

	uchar   oddstate;              //异常信号标
	uchar   sgzstatus;             //事故总信号标
	uchar   hsgstate;              //事故信号标
	uchar   hygstate;              //预告信号标
	uchar   hpsstate;              //保护信号标
	uchar   reserved;

	uchar   calycflag;               //
	uchar	calyxflag;               //
} SUBSTATION_STRU;
   

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Substation {
		static int       opentabf;
		static Rdb_Tab  substationtab;
	public:
		Substation(void);
		SUBSTATION_STRU  *GetSubStPara(char *substname);
        int  CalSafeDays(void);
		void SetCalYCFlag(char *substname);
		void SetCalYXFlag(char *substname);
		void SetSgFlag(char *substname);
		void SubStManConfirm(char *substname);
		int  CalSubStYCState(void);
		int  CalSubStYXState(void);
};

/************************   终端表   *******************************/
typedef struct {
	short		terminalno;                   //终端序号
	char		name[OBJECTNAME_LEN];         //代码
	char		describe[OBJDESCRIBE_LLEN];    //描述
	uchar		useflag;                      //使用标志
	uchar		status;                       //状态
	short		ycnums;                       //遥测个数
	short		yxnums;                       //遥信个数
	short		kwhnums;                      //电度个数
    intertime	lastproct;                    //上次处理时间
    intertime	dayruntime;                   //日运行时间
    intertime	totalruntime;                 //运行时间
	ushort		deadf;						  //死数标志
	short       runchanno;                  //实时通道号
	int     	breaktimes;	//日停运次数
} TERMINAL_STRU;
  

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Terminal {
		static	int			opentabf;
		static	Rdb_Tab	    terminaltab;
	public:
		Terminal(void);
        void       CalRunTime();
        void       DayInit();   
        TERMINAL_STRU  *GetTerminalPara(int terminalno);
	    int        GetName(short terminalno,char *name);
	    int        GetNameDes(short terminalno,char *name,char *des);
	    int        EnShareYK(int terminalno);
};

/************************ FTU 表 *******************************/
/*
typedef struct {
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	short	ftuno; 			
	short	terminalno;
	short	channo;
	uchar	status;
	short	ycnums;
	short	yxnums;
	short	kwhnums;
    intertime  lastproct;
    int     dayruntime; //运行时间 分钟
    int     totalruntime;    //累计运行时间
} FTU_STRU;
   
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ftu {
		static	int			opentabf;
		static	Rdb_Tab	ftutab;
	public:
		Ftu(void);
        void CalRunTime();
        void DayInit();
		int  GetName(int,int,char*);
};
*/


#define		FDKYCNUMS	16
typedef struct {
	char	name[OBJECTNAME_LEN];
    char	describe[OBJDESCRIBE_LEN];
	short	terminalno; 	
	short	fdkno;
	char	ycname[FDKYCNUMS][OBJECTNAME_LEN];
	} FDK_MODULE_STRU;

/************************ 通道表 *******************************/
typedef struct {
    short		channelno;      //通道序号
	char		name[OBJECTNAME_LEN];//代码
	char		describe[OBJDESCRIBE_LLEN];//描述
	uchar  	 	useflag;		//使用状态
	short   	rtuno;       //终端序号
	uchar		status;			//状态值
	char		mainbak;		//主备状态	
	float 		rateCode;		//误码率
	int		    lastswitchtime;	//上次切换时间
    int     	breaktimes;		//日故障次数
    int  	    lastproct;      //上次处理时间
    int     	dayruntime;     //日运行时间
    int     	totalruntime;   //累计运行时间
	char		hostname[OBJECTNAME_LEN];//节点名
	uchar       grouppriority;   //组内优先级
	int		  lastsynctime;			///<上次对时成功时间
	float		recvcount;		///<通道接收字节数
	float		sendcount;		///<通道发送字节数
	unsigned char		synctimesuccess;	///<对时成功标志	
} CHANNEL_STRU;
 

typedef struct {
	short       rtu_no;
	short		channel_no;		//主通道序号
    char		chname[OBJECTNAME_LEN];//主通道代码
    short      grouppriority;   //组内优先级
	short		last_channel_no;		//上次主运行通道序号
} SCDTERMINALDATA;


typedef struct
{
	SCDTERMINALDATA terminaldata[MAX_RTU_NUM];
	//SCDCHANDATA	chandata[MAX_CHAN_NUM];
}SCADACHANPARA;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Channel {
		static int       opentabf;
		static Rdb_Tab   channeltab;
	public:
		Channel(void);
		friend int     GetChanName(int channelno,char *channelname,char *chandes);
		CHANNEL_STRU  *GetChanPara(int channo);
        void           CalRunTime();
        void           DayInit();
		int            ProcChanData(CHANNEL_STRU *,float ,char );
};

/*
#define  YCOLSAVEMODE_ALLSAVE		0  // 全部存库
#define  YCOLSAVEMODE_NROLUNSAVE	1  // 普通越限不存库
#define  YCOLSAVEMODE_ALLUNSAVE		2  // 全部不存库
*/

/***************** 模拟量 **************************/
typedef struct  {
        char    name[OBJECTNAME_LEN];//代码
        char    describe[OBJDESCRIBE_LEN];//描述
        int     securegrade;//安全级
        char    substation[SUBSTNAME_LEN];//站所线系代码
		char    devname[OBJECTNAME_LEN];  //设备代码         //add
		ushort  terminal;//接线端子                          //add
//		uchar	samplemode;//采集方式
//		short   ftuno;
		short   groupno;    //分组序号                       //add
        short   terminalno;//终端序号
        short   ycno;//遥测序号
		short	baseobjtype;//遥测类型
//        int     maxscale;//	
//        float   maxengval;  // 如果已知系数和基值的情况下，如下填写参数:
//        int     minscale;	// minscale=0  maxscale=1  maxengval=系数+基值  minengval=基值
//        float   minengval;//
        uchar  savedensity;//存盘周期
        uchar  polgrade; //处理越限级别 0-no process limit
        float   slope;//斜率                           //add
		float   intercept;//截距                       //add

        float   zeroval;//零漂
        float   maxinvld;//上溢值
        float   mininvld;//下溢值
        float   maxchange;//最大增量

//		short   deadtime;//死数时间                     //add
		char    entdealmode[OBJECTNAME_LEN];//事项处理方式//add
        float   highlimit;//警告上限
        float   lowlimit;//警告下限
        float   hihilimit;//警报上限
        float   lololimit;//警报下限
        float   recovcoef;      //恢复系数
		char    olentname[ENTNAME_LEN];//事项句代码
		char    almgraname[GRAP_LEN];//推图名
        uchar   calcultf;//计算量标志                          //add
		uchar   changesavef;//变化存储标志                            //add
		short   scale;    //倍率                                //add
//        uchar   delayalmtime;   //报警延时
//        char    olentname[ENTNAME_LEN];//报警句
        
        float   rawdata;//原始值
		float   maxrawdata;//最大原始值
		float   minrawdata;//最小原始值
        uchar   maxhour;//最大值小时
        uchar   maxminute;//最大值分钟
        uchar   maxsecond;//最大值秒
        uchar   minhour;//最小值小时
        uchar   minminute;//最小值分钟
        uchar   minsecond;//最小值秒
        float   value;//值
        float   maxvalue;//最大值
        float   minvalue;//最小值

        uchar   mansetf; //置入标志
        uchar   bypassflag;//旁路替代标志
        uchar   linezero;//线路零值标志
        uchar	neterror;//网络故障标志
		uchar	terminalerror;//终端故障标志
		uchar   invalidf;//非法标志
		uchar	reserved0;

        uchar   puteventf;    //已生成事项标志
        uchar   deadf;		//死数标志
        uchar   reliabilityf;	//可靠度-针对计算量：0-可靠，1-不可靠
		uchar	lockfertf;    //封锁FEP服务器数据
		uchar	ycinconsistf; //YC不一致标志
		uchar	yxinconsistf; //YX不一致标志
		uchar	bypassrevf;   //替代值取反标志
		ushort	reserved1;	

		ushort   exflag1;		//扩展标志
		ushort   olgrade;/*越限级别	
							0-- no over limit 
							1-- over high limit
							2-- over low  limit 
							3-- over high-high limit
							4-- over low-low   limit 
						*/
//		uchar	calmode;		//0--非标准计算，1--
		short	overlimitcounthh;
		short   overlimitcounth;
		short   overlimitcountl;
		short   overlimitcountll;
		int     overlimittimehh;
		int     overlimittimeh;
		int     overlimittimel;
		int     overlimittimell;
		int 	sumtimes;
		double	sumvalue;
		double	daysumvalue;
		int	    daysumtimes;
		int		deadsecs;
		int     deadtime;
		int     dstoptime;	//日停运时间(仅对电压而言)
		intertime  lastproctime;
		short   deadtimelimit;
		short	overinccnt;
		short	olcounter;
		char	bypassname[OBJECTNAME_LEN]; 
		int     procsecs;//处理时间秒
		int 	procmsecs;//处理时间毫秒
}  BASEANALOG_STRU;

//变化数据时标结构体
typedef struct
{
	int    tv_sec;         /* seconds */
	int    tv_msec;        /* and microseconds */
}TIME_VAL;

typedef struct 
{
	char    name[OBJECTNAME_LEN];//代码
	char    desc[OBJDESCRIBE_LEN];
    float   value;//值
	int     procsecs;//处理时间秒
	int 	procmsecs;//处理时间毫秒
	char    unit[16];
} CHANGE_YCDATA;


typedef struct 
{
	CHANGE_YCDATA  ycData[MAX_CHGDATANUM];
	short         ycwritep;
	short          ycreadp;
	short         ycTrendDBwp;
	short         ycTrendDBrp;
}SAVE_YCDATA;

typedef struct 
{
	char    db[OBJECTNAME_LEN];
	char    nameserver[OBJDESCRIBE_LEN];
	char    user[OBJECTNAME_LEN];
	char    passwd[SUBSTNAME_LEN];
	int    datahistdays;  
	short  datahistdepth;    
}TRENDDB_STRU;


typedef struct  {
	short   index;			                        
	char    name[OBJECTNAME_LEN];
	short   starttime;
	short   endtime;
	float   highlimit;
	float   lowlimit;
	float   hihilimit;
	float   lololimit;
} YCTIMELIMIT_STRU;


#define   IS_YC_OVER_LIMIT(a) ((a)->olgrade && (a)->puteventf)



//analog table struct
typedef struct {
        BASEANALOG_STRU   baseanalog;
        short   quotatime;
        float   quotadata;  // 当前计划值
		float   quotaup;	// 当前计划上限
		float   quotadown;  // 当前计划下限
//		uchar	voltgrade; // 电压等级 
//		short	voltlmt;   //电压限值
		int		updowntimes; // 总升降成功次数，永不清零
    } ANALOG_STRU;


//////////////////// 遥测处理基类  ////////////////////////

class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseAnalog : public NetDbg{
	Rdb_Tab*    pRdbTable;
    uchar       cbOLGradeChangef,cbOLGrade;
    uchar       cbPutEventf;

  private:
//私有成员：
    void        RawToEngineering(short  rawdata);
    void        Filter(void);			//进行数据过滤，如零值处理，越限计数，置数据非法标志，等。
    void        OverLimitCheck(void);	//越限处理。判断是否越限，及越限级别，并置适当标志，
    void        TimeOverLimit(BASEANALOG_STRU  *banap,int decsecs)	//越限统计时间
    		{
	  	switch (banap->olgrade)
	   	  {
	   	   case ANAOL_HIGH:
			banap->overlimittimeh=DAY_TIME_ADD(banap->overlimittimeh,decsecs);
			break;
	   	   case ANAOL_LOW:
			banap->overlimittimel=DAY_TIME_ADD(banap->overlimittimel,decsecs);
			break;
	   	   case ANAOL_HIHI:
			banap->overlimittimehh=DAY_TIME_ADD(banap->overlimittimehh,decsecs);
			break;
	   	   case ANAOL_LOLO:
			banap->overlimittimell=DAY_TIME_ADD(banap->overlimittimell,decsecs);
			break;
	   	  }
    		}
    void        OverLimitApEventProcessing(void);	//处理越限事项：若越限级别变化，则产生适当的越限事项，反之亦然。

virtual int     YieldOverLimitApEvent(void *rcdp)=0;          //纯虚函数==》该类为纯虚基类，不能有实例
virtual int     YieldOverMutationApEvent(void *rcdp)=0;          //纯虚函数==》该类为纯虚基类，不能有实例
	int         SetValue(float  setdata,int setflag);



   protected:
	BASEANALOG_STRU*      pBASEANALOG;
	int				nDecSecs;
	uchar			cbRawChangef,cbCookChangef;
	float			fEngData;
	//short           sRawData;
	virtual void    CalMaxMin(float value,float rawdata);	
	virtual void	Statistics(void);		//统计工程数据的最大最小值及产生时间
	
    void			CountOverLimit(BASEANALOG_STRU  *banap,uchar olgrade);	//越限次数统计

  public:
	BaseAnalog(Rdb_Tab* pTable)
	{
		pRdbTable=pTable;
		m_pYcTimeLimit = NULL;
		ycdatap = NULL;
	};
	~BaseAnalog()
	{
		if (ycdatap!= NULL)
		{
			delete ycdatap;
			ycdatap = NULL;
		}
	}
	SAVE_YCDATA*    ycdatap;
	int GetYcName(short terminalno,short order,char* name);//取遥测名
  
	int     GetAnaValue(char *name,float *data);    //取值
	int     ManSetAnaVal(char *name,float  setdata,int setflag); //人工置数
	virtual int     DayInit(void); 
	int     EngTimeDataProcessing(BASEANALOG_STRU *anap,float engdata,TIME_VAL yctime); //工程转换,带时标
	//int     EngDataProcessing(BASEANALOG_STRU *anap,float engdata); //工程转换
	//int     RawDataProcessing(BASEANALOG_STRU *anap,short  rawdata ); //处理生数据
	//int     FloatRawDataProcessing(BASEANALOG_STRU *anap,float  rawdata ); //处理浮点型生数据
	//int     FloatEngDataProcessing(BASEANALOG_STRU *anap,float  engdata,TIME_VAL yctime); //处理浮点型工程数据 zxm
	//int     PutRawDataToYcRcdNo(int rcdno,short rawdata);  //将生数据放入相应遥测记录
	int     PutTimeData(char *name,float data,TIME_VAL ycmultime);
	void	StatisticsCalVal(BASEANALOG_STRU  *banap);		//2004 05 add 统计计算数据的最大最小值及产生时间
	void    TimerProcess(void);    // 处理定时累加任务
  public:
	  YCTIMELIMIT_STRU * m_pYcTimeLimit;
	  int CheckYcInTimeLimit(float &hlimit,float &hhlimit,float &llimit,float &lllimit);
  };

/////////////////////// 遥测处理类   ////////////////////////
class  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Analog : public  BaseAnalog {
    static int        nOpenTablef;
    static Rdb_Tab    AnalogTable;
    int     YieldOverLimitApEvent(void *rcdp);
  public:
    Analog(void);
	ANALOG_STRU  *GetAnalogAddr(char *name,int *rcdno=NULL);
		int     YieldOverMutationApEvent(void *rcdp);
/*
	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void  AnaStandardCal(void);
*/
	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void   AnaBypassProcessing(void);
  };

#if defined(WIN32)
_SCD_DLLPORT 
#endif
void  SetTimerSavingFlag(int setval);

////////////////  电压等级 和 电压限值 /////////////////////////////////////////
/*
typedef struct {
	char   name[OBJECTNAME_LEN];
	uchar  voltgrade;
    char   describe[OBJDESCRIBE_LEN];
    float  ratingvolt;
    float  stdvolt;
 	uchar  units;	//单位：0--KV，1--V
	uchar  ac_dc;    //交直流: 0-- AC 1-- DC
} VOLTGRADE_STRU;
  
typedef struct {
	short  voltlmtno;
    char   describe[OBJDESCRIBE_LEN];
    float  upvolt;
    float  downvolt;
	float  almhlimit;
	float  almllimit;
	float  almhhlimit;
	float  almlllimit;
} VOLTLIMIT_STRU;
  
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
VoltLimit {
	static int  opentabf;
	static Rdb_Tab  voltlimittab;
	static Rdb_Tab  voltgradetab;
  public:
	VoltLimit(void);
	VOLTGRADE_STRU *GetVoltGrade(uchar voltgrade);
	VOLTLIMIT_STRU *GetVoltLimit(short voltlmt);
};  
*/


/*********************** 计算变量表 ************************/     

//     此表的定义主要为了保存一些计算结果或中间变量，它不象
//	模拟量或者状态量那样需要许多参数，只需保存一些值。
/*
// 计算变量结构
typedef struct {
		char    name[OBJECTNAME_LEN];			// 名字
		int     describe[OBJDESCRIBE_LEN];		// 描述
		int     securegrade;					// 安全级
		float   value;							// 值
		uchar	state;							// 状态
} CALCULATE_STRU;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Calculate {
		static int			opentabf;
		static Rdb_Tab		calculttab;
	public:
		Calculate(void);  
		int GetCalValue(char *name, float *data);  
  };
*/  

/*********************** 状态信息表 ************************/
// 事项输出方式
typedef struct {
		uchar  printf;	// 打印
		uchar  logf;		// 登录	
		uchar  alarmf;	// 音响报警
		uchar  popgraphf;	// 调图
} ENTOUTMODE_STRU;


typedef struct {
		char		name[OBJECTNAME_LEN];		// 名字
		char		describe[OBJDESCRIBE_LEN];	// 描述
		short		staval;						// 状态值
		ENTOUTMODE_STRU	entmode;				// 事项输出方式
} STATUSMESG_STRU;
          
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
StatusMesg {
		static	int			opentabf;
		static	Rdb_Tab	    stamsgtab;
	public:
		StatusMesg(void); 
		// 根据对象名、状态值取得状态信息表结构地址
		STATUSMESG_STRU	*getmsgaddr(char *,short);
};




typedef	struct {
		char	name[OBJECTNAME_LEN];			// 名字
		char	describe[OBJDESCRIBE_LEN];		// 描述
		int	    securegrade;				    // 安全级
        char    substation[SUBSTNAME_LEN];
		ushort	objtype;				        // 设备类型
		uchar   sampletype;
		short   groupno;
		short   terminalno;
		short   orderno;
		short	maxinvld;						// 最大无效限值
		short	mininvld;						// 最小无效限值
		char	chgentname[ENTNAME_LEN];		// 事项句名
		char	stamsgname[OBJECTNAME_LEN];     // 状态信息名
		char	almgraname[GRAP_LEN];		    // 图名
		short	value;							// 状态值
		uchar	mansetf;						//　变位
		uchar	invalidf;						// 无效标志
		uchar   calcultf;
} MULTISTATUS_STRU;        


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
MultiStatus {
		static	int			opentabf;
		static	Rdb_Tab	    multstatab;
		//  产生多态量变化应用事件
		int		YieldChgApEvent(MULTISTATUS_STRU *,short);
		//	处理多态值
		int		MultiStadataProcessing(MULTISTATUS_STRU *,short);
	public:
		MultiStatus(void);
		//  根据对象名放入状态值
		int		SetMultiStadataByName(char *name,short staval);
		//  根据对象名取状态值
		int		GetData(char *name,short *stadata);
		void    ProcRawData(MULTISTATUS_STRU *mp,short mstaval)  {
			MultiStadataProcessing(mp,mstaval);
		}
};

////////////////////////////////////////////////////////////////
typedef struct {
        SYS_CLOCK   eclock;		//	时间
        uchar     source;		//	事项来源 
        ushort    terminalno;	//	
        ushort    yxno;			//
        uchar     status;		//
		uchar     testflag;
}  RAWENT_STRU;



// 结构
typedef struct	{
		short terminalno;
		short yxno;
}YXSORT_STRU;

typedef	struct   {
	    short   groupno;
	    short	terminalno;					// 终端号
		short	yxno;						// 遥信序号
		char	objname[OBJECTNAME_LEN];	// 对象名
		short	objtype;					// 对象类型
} TERMINALYXSORT_STRU;

typedef struct	{
	short terminalno;
	short ycno;
}YCSORT_STRU;

typedef	struct   {
	short   groupno;
	short	terminalno;					// 终端号
	short	ycno;						// 遥信序号
	char	objname[OBJECTNAME_LEN];	// 对象名
	short	objtype;					// 对象类型
} TERMINALYCSORT_STRU;

typedef struct	{
	short channelno;
	char	objname[OBJECTNAME_LEN];	// 对象名
}CHANNELSORT_STRU;

typedef struct	{
	short terminalno;
	short kwhno;
}KWHSORT_STRU;

typedef	struct   {
	short   groupno;
	short	terminalno;					// 终端号
	short	kwhno;						// 遥信序号
	char	objname[OBJECTNAME_LEN];	// 对象名
	short	objtype;					// 对象类型
} TERMINALKWHSORT_STRU;


typedef	struct   {
		short	no;						    // 序号
		char	objname[OBJECTNAME_LEN];	// 对象名
} WFBS_STRU;

//状态量基础结构
typedef struct {
        char    name[OBJECTNAME_LEN];		//  对象名
        char    describe[OBJDESCRIBE_LEN];	//	描述
        int     securegrade;				//	安全级
//		uchar   voltgrade;					//  电压等级
        char    substation[SUBSTNAME_LEN];	//	站所线系代码
		char    devcode[OBJECTNAME_LEN];	//  设备代码                     //add
		ushort	baseobjtype;				//  遥信类型
//		uchar	samplemode;					//	采集方式
//		short	ftuno;						//	ftu号
		short	groupno;					//	分组序号                     //add
		short	terminalno;					//	终端序号
		short	yxno;						//  遥信序号
		short	yxno1;						//  次遥信序号
		uchar	twinyxflag;					//  双遥信标志（双位标志）
		uchar	mainyxflag;					//  主遥信标志                   //add
		uchar   twinyxcalmode;              //  双遥信计算方式               //add
        char    entdealmode[OBJECTNAME_LEN];//  事项处理方式                 //add
		uchar   slowyx;						//  慢遥信
//		uchar   confirmmode;				//  确认方式
//      uchar	oddproctype;				// 0--- no process     
											// 1--- 变分报警  
		uchar	zyf;						//	追忆标志
		uchar	ykf;						//	可否遥控
		uchar	calcultf;					//　计算量标志
		ushort	reserved0;
		uchar	changesavef;					//　变化存储标志

		uchar	maskchgentf;				//  封锁变位事项标志
		uchar	mansetf;					//　人工设置
		uchar	lockfertf;					//  封锁FEP服务器数据
		uchar	putdangerf;					//  挂牌标志
		uchar	putgroundf;					//  母线侧接地标志
		uchar	sndgroundf;					//  线路侧接地标志
		uchar   hsgflag;
		ushort	reserved1;

		uchar	invalidf;					//　无效
		uchar	changef;					//　变位
		uchar	oddstate;					//　异常
		uchar	yktakef;					//	遥控执行标志
		uchar   linerunf;					//  线路带电标志
		uchar	revval0;
		uchar	revval1;
		uchar	initedf;					//  初始化标志

		uchar	neterror;					//　网络故障
		uchar	terminalerror;				//　TERMINAL故障
		uchar   alarmf;						//  警示标志
		uchar   yxinconsistf;				//  遥信不匹配
		uchar	reserved3;

        uchar   value;						//　当前值
        uchar   revval;						//　保留值(yxno)
		uchar	fiveflag;					//  五防标志

		ushort clearchgtime;				//  变位标志为1时，在指定时刻清标志（仅用作内存列）
		ushort clearalmtime;				//  警示标志为1时，在指定时刻清标志（仅用作内存列）
        char    soeentname[ENTNAME_LEN];	//  SOE句
        char    chgentname[ENTNAME_LEN];	//  事项句
        char    almgraname[GRAP_LEN];	    //  推图名
		intertime lastproct;				//	上次处理时间
		int		yktimes;					//  遥控成功次数累加值, 永不清零
		int     procsecs;                    //处理时间秒
		int     procmsecs;                    //处理时间毫秒
} BASESTATUS_STRU;
   

typedef struct 
{
	char    name[OBJECTNAME_LEN];		//  对象名
	char    desc[OBJDESCRIBE_LEN];
    uchar   value;						//　值
	int     procsecs;                    //处理时间秒
	int     procmsecs;                    //处理时间毫秒
} CHANGE_YXDATA;


typedef struct 
{
	CHANGE_YXDATA  yxData[MAX_CHGDATANUM];
	short         yxwritep;
	short          yxreadp;
	short         yxTrendDBwp;
	short         yxTrendDBrp;       
}SAVE_YXDATA;

// 状态量基类（status process base class）

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseStatus : public NetDbg{
		char		     objtabname[RDB_COLNAME_LEN];
		Rdb_Tab	        *objtable;
		BASESTATUS_STRU	*statusp;
		uchar            changeflag;
		// 根据终端序号和TERMINAL内序号，取得记录号，此记录不应是计算值。
//		int			getrcdno(short terminalno, short order); 
		// 置警示标志
		virtual int		SetAlarmFlag(BASESTATUS_STRU *p); 
	public:
		SAVE_YXDATA *yxdatap;
	
		
		BaseStatus(char *tabname,Rdb_Tab *tabp)
		{
			strcpy(objtabname,tabname);
		    objtable = tabp;
			yxdatap = NULL;
		};
		~BaseStatus()
		{
			if (yxdatap != NULL)
			{
				delete yxdatap;
				yxdatap = NULL;
			}
		}
		int GetYxName(short terminalno,short order,char* name);//取遥信名
		int GetSubYxName(short terminalno,short order,char* name);//取次遥信号遥信名	
		//变位时求小车开关的状态
		int  CarbreakStatusProcessing(BASESTATUS_STRU  *p,uchar &uCarBreakStat);

		virtual int      RawStatusdataProcessing(BASESTATUS_STRU  *stap,int yxno,uchar  statusval,int proc_slowyxf,uchar makeentf=1);
		virtual int		 StatusdataObjectProcessing(BASESTATUS_STRU  *stap,uchar  statusval,uchar makeentf=1);//处理生数据
		// 将变位事件放入应用事件队列
		virtual void     PutStaChgEvent(SYS_CLOCK *entclockp,int chgtype,BASESTATUS_STRU *stap,uchar staval,uchar maketype,uchar testflag=0);
		// 根据名字将状态值存入表中
		virtual  int     SetStadataByName(char *name,uchar stadata);
		// 根据名字取状态值
		int				 GetYxData(char *name,uchar *stadata);//取值
		// 根据名字取得记录结构地址
		BASESTATUS_STRU	*GetAddrByName(char *name,int *rcdno=NULL);
		// 处理生事项
		int				 RaweventProcessing(char *name, RAWENT_STRU *raweventp,int proc_slowyxf); 
		// 根据名字将记录设为人工置入方式并将记录状态置为给定值 或者 清人工置入标志并将记录状态置为实时值
		virtual  int     MansetYxVal(char *name,uchar setdata,uchar setflag); //人工置数
};



/******************** 状态量 *********************/
// 结构
typedef struct {
        BASESTATUS_STRU	    basestatus;
	    int			        runtime;		//运行时间
		unsigned short		chgcounter;		//变位计数
		unsigned int		second_yxchg;		//  处理秒 zl20110805
		unsigned int		msecond_yxchg;		//  处理毫秒
		unsigned int		second_soe;			//  处理秒
		unsigned int		msecond_soe;		//  处理毫秒
} STATUS_STRU;

// 类（status process class）
class
#if defined(WIN32)
_SCD_DLLPORT 
#endif
StatusProcess : public  BaseStatus {
		static int         opentabf;
		static Rdb_Tab    statustab;
	public:
		StatusProcess(void) ;
};

/******************** 配电故障检测状态量 *********************/
// 结构
typedef struct {
        BASESTATUS_STRU	    basestatus;
	    int			        runtime;		//运行时间
		unsigned short		chgcounter;		//变位计数
		unsigned int		second_yxchg;		//  处理秒 zl20110805
		unsigned int		msecond_yxchg;		//  处理毫秒
		unsigned int		second_soe;			//  处理秒
		unsigned int		msecond_soe;		//  处理毫秒
} DMSSTATUS_STRU;

// 类（dmsstatus process class）
class
#if defined(WIN32)
_SCD_DLLPORT 
#endif
DmsStatusProcess : public  BaseStatus {
		static int         opentabf;
		static Rdb_Tab     dmsstatustab;
	public:
		DmsStatusProcess(void) ;
};

/*****************  保护信号 ********************/
// 结构
typedef struct   {
		BASESTATUS_STRU	basestatus;
		ushort		actcounter;
		unsigned int		second_yxchg;		//  处理秒 zl20110805
		unsigned int		msecond_yxchg;		//  处理毫秒
		unsigned int		second_soe;			//  处理秒
		unsigned int		msecond_soe;		//  处理毫秒
} PROTSIG_STRU;


// 类（Protsig process class）

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Protsig : public BaseStatus  {
		static int         opentabf;
		static Rdb_Tab    protsigtab;
	public:
		Protsig(void);
};


/***************** 断路器 开关 ********************/
// 结构
typedef	struct  {
		BASESTATUS_STRU	basestatus;
		int		        tchgcounter;	//	总变位次数
		int		        dayruntime;		//	日运行时间
		int		        totalruntime;	//	总运行时间
		unsigned short	dchgcounter;	//	日变位次数
		unsigned short	tsgcounter;		//	总事故跳闸次数
		short		    maxchgnums;     //  变位次数限
		short		    maxsgnums;		//	事故限
		short		    zygroup;		//	追忆组号
		char		    bypassbreak[OBJECTNAME_LEN];
		char		    sglogic[PROCNAME_LEN];		// 事故逻辑过程名
		uchar		    sgprocmode;		//	判事故模式
		uchar		    sgprocband;		//	判事故死区
		uchar		    linestat;		//  线路状态 包括：检修、限电等
		uchar           bypassflag;     //  旁路标志
		unsigned int		second_yxchg;		//  处理秒 zl20110805
		unsigned int		msecond_yxchg;		//  处理毫秒
		unsigned int		second_soe;			//  处理秒
		unsigned int		msecond_soe;		//  处理毫秒
} BREAK_STRU;


// 类（Break process class）
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Break : public BaseStatus  {
		static int			opentabf;
		static Rdb_Tab		breaktab;
	    //static intertime	lastcaltime;	//	上次时间
	public:
		Break(void);
		// 累加运行时间
		void	CalRunTime(void);
		// 累计值清零
		void	DayInit(void);
		// 根据名字将记录设为指定的操作标志
	    int		SetSpecialFlag(char *name,uchar setflag,short flagtype,short flagno); 
};


/**********************  刀闸  *******************/
// 结构
typedef	struct   {
		BASESTATUS_STRU	basestatus;
	    int			runtime;					// 运行时间
		uchar		linestat;		//  线路状态 包括：检修、限电等
		unsigned int		second_yxchg;		//  处理秒 zl20110805
		unsigned int		msecond_yxchg;		//  处理毫秒
		unsigned int		second_soe;			//  处理秒
		unsigned int		msecond_soe;		//  处理毫秒
} SWITCH_STRU;



// 类（switch process class）
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Switch : public BaseStatus  {
		static int         opentabf;
		static Rdb_Tab     switchtab;
	public:
		Switch(void);
		//
	    int   SetSpecialFlag(char *name,uchar setflag,short flagtype,short flagno);
};


#ifdef WIN32
_SCD_DLLPORT 
#endif
BASESTATUS_STRU   *GetYXTableNameForRdb(char *objname,char *tabname);

#ifdef WIN32
_SCD_DLLPORT 
#endif
BASESTATUS_STRU   *GetYXTableTypeForRdb(char *objname,int objtype);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int PutForeRaweventByObjtype(char *name,int objtype, RAWENT_STRU *rawentp,int proc_slowyxf);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int PutStatusDataByObjtype(char *name,int objtype,uchar staval);


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
BreakYCRelation {
	static Rdb_Tab					brktable;
	static Rdb_Tab					anatable;
	static int						breaknums;
	static intertime				brkupdtime;
	static intertime				anaupdtime;
	static BASEANALOG_STRU				**ana_addr;
public :
	BreakYCRelation(void);
	void              CheckAllBreakYCRelation(void);
	BASEANALOG_STRU **GetYCRelation(int brkno,int *relnums);
	BASEANALOG_STRU **GetYCRelation(char *breakname,int *relnums);
};



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct  {
	uchar   oldstate;
	uchar   timecount;
} BREAKSTATE_DELAY_STRU;
class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
BreakYCInconsist {
	static Rdb_Tab			brktable;
	static BREAKSTATE_DELAY_STRU  *brkstate_addr;
public :
	BreakYCInconsist(void);
	void SetLineZero(int brkno,int flag);
	void SetYXInconsist(int brkno,BASESTATUS_STRU *breakp,int flag);
	void LoopProcBreakYCInconsist(void);
};



#define  SLOWYX_TYPE_CHANG        0
#define  SLOWYX_TYPE_SOE		  1
#define  SLOWYX_TYPE_STATUS       2

typedef struct {
	uchar       type;
	short		objtype;					// 对象类型
	char		objname[OBJECTNAME_LEN];	// 对象名
	short       yxno;
	short		lifetime;
	short       bandtime;
	intertime	recvtime;
	unsigned short		recvms;
	uchar		fstatus;	// 首次状态
	short		nstatus;    // 最新状态
	 RAWENT_STRU    rawevent;
}  SLOW_LINK_DATA_STRU;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
SlowYXLinkTable {
	static ComnLinkTable<SLOW_LINK_DATA_STRU>  clinktable;
public:
	int AddYXEvent(short objtype,char *objname, RAWENT_STRU  *rawentp,int slowyx);
	int AddYXStatus(short objtype,char *objname,uchar status,int slowyx);
	void SlowYXProc(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------//
// 电度量

#define	SDTYPENUMS	4	// 时段类型数
// 结构
typedef	struct {
		char    name[OBJECTNAME_LEN];		// 对象名
		char    describe[OBJDESCRIBE_LEN];	// 描述
		int     securegrade;				// 安全级
		char    substation[SUBSTNAME_LEN];	// 厂站名
		char    devname[OBJECTNAME_LEN];	// 设备代码
		ushort	baseobjtype;				// 电度类型
//		uchar	samplemode;					// 采集方式
//		short	ftuno;						// FTU号
		short   groupno;                    // 分组序号
		short	terminalno;					// 终端序号	
		short	kwhno;						// 电度序号
		float	basecode;					// 基码
		float   scale;						// 标度
		float   fullcode;					// 满码
		float   factor;						// 倍率
		float	sdbasecode[SDTYPENUMS];
		float	sdfullcode[SDTYPENUMS];
									//           2--20bit,3--24bit
									//           4--32bit,5--自定义
		unsigned int     rawdatafull;       // 原码满值
		unsigned int     rawdata;			// 生数据
		float   tabcode;	
		float	sdtabcode[SDTYPENUMS];		// 时段表码	
		float   curkwh;						// 电度增量值
		short   savedensity;				// 存盘间隔
		uchar	sampletype;					// 采样类型  0--16bit,1--19bit,
		uchar    exflag1;					//扩展标志
		double	integralkwh;				// 积分电度值	
		double  savekwh;					// 存盘值
		double  daykwh;						// 日累计
		double  monthkwh;					// 月累计
		double  yearkwh;					// 年累计
		double  daysdkwh[SDTYPENUMS];       // 时段日累计(平、峰、谷、尖峰)
		double  monthsdkwh[SDTYPENUMS];		// 时段月累计
		double  yearsdkwh[SDTYPENUMS];		// 时段年累计
		ushort  district;					// 区域(时段组)

		uchar	mansetf;					// 人工置入标志
		uchar	calcultf;					// 计算标志
		uchar	integralf;					// 积分标志
		uchar	savediskf;					// 存盘标志
		uchar	initedf;					// 初始化标志
		uchar	neterror;					// 网络故障
		uchar	terminalerror;				// TERMINAL故障
		uchar	reserved0;

		uchar	lockfertf;					// 封锁FEP服务器数据			
		uchar	bypassflag;					// 旁路标志
		int	reserved1;

		char    bypassname[OBJECTNAME_LEN];	// 旁路电度代码
		char    integycname[OBJECTNAME_LEN];// 积分遥测代码
		float   lastbasecode;				// 上次基码
		double	abskwh;						// 绝对电度
		double	bykwh;						// 旁路电度
		double  daysdkwh5;                  // 脊谷时段日累计
		double  monthsdkwh5;		        // 脊谷时段月累计
		double  yearsdkwh5;		            // 脊谷时段年累计
} KWH_STRU;

// 基类（KWH process base class）
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BaseKwh : public NetDbg{
		Rdb_Tab	*objtable;
		void        ProcRawData_Counter(KWH_STRU *,unsigned int  rawdata);
		void        ProcRawData_Tabcode(KWH_STRU *,unsigned int  rawdata);
	public:
		BaseKwh(Rdb_Tab *tabp) {	objtable = tabp;	};
		//
		int GetKwhName(short terminalno,short order,char* name);//取电度名
		void        ProcIncData(KWH_STRU *,double incdata); 
        // 取出存盘电度值，并将此值清零，重新进行累计
		float       GetSaveKwhVal(KWH_STRU  *kwhp);
        // 取出积分电度值，并将此值清零，重新进行累计
		float       GetIntegralKwhVal(KWH_STRU  *kwhp);
		//
		int			GetKwhValueByName(char *name, float *value);
		// 根据记录号将生数据放入实时库中
		int         PutRawDataByRcdNo(int rcdno,unsigned int rawdata);
		// 处理生数据
		void        RawKwhProcessing(KWH_STRU *,unsigned int  rawdata);
		void        ProcRawData_HourClean(KWH_STRU *,unsigned int  rawdata);
		//
		KWH_STRU   *GetKwhAddr(const char *name,int *rcdno=NULL);
};


// 类（Kwh  process  class）
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Kwh  : public BaseKwh {
		static int         opentabf;
		static Rdb_Tab     kwhtab;
	public:
		Kwh(void);
		
		// 累计电度量清零
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         KwhDayInit(void);
		// 电度积分	
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         CalIntegralKwh(void);
		// 旁路替代	
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend void         KwhBypassProcessing(void);
};



////////////////////////////////遥测多源处理//////////////////////////////

#define		MAX_YCSRC_NUMS	4


#define		MULTSRC_SAMPLING_TERMINAL	1
#define		MULTSRC_SAMPLING_OBJ	2

typedef struct {
	uchar   samplemode;
	short   sampgrpno;
	short   samporderno;
	float   coef;
	float	base;
}  YC_SRCPARA_STRU;

typedef struct {
	float   value;
	uchar	quality;
	uchar	initedf;
	uchar	invalidf;
	uchar	deadf;
	uchar	deverror;
	uchar   reserved;
	int		deadsecs;
	int     procsecs;
	int     procmsecs;
}  YC_SRCDATA_STRU;

typedef struct {
	char	objname[OBJECTNAME_LEN];
	uchar	selectmode;
	uchar	cursrcno;
	uchar	setflag;
	uchar	reserved;
	YC_SRCPARA_STRU  ycsrcpara[MAX_YCSRC_NUMS];
	YC_SRCDATA_STRU  ycsrcdata[MAX_YCSRC_NUMS];
} YC_MULTSRC_STRU;

class YCMultiSource {
    static int         opentabf;
    static Rdb_Tab    ycmultsrctab;
  public:
    YCMultiSource(void);

	#if defined(WIN32)
	_SCD_DLLPORT 
	#endif
	friend void  YCMultiSourceDataProcessing(void);
};


////////////////////////////////////////////////////////////////////////////////////////////

// 根据开关名取P、Q、I
#if defined(WIN32)
_SCD_DLLPORT 
#endif
int  GetAnalogDataByBreak(char *breakname,float *pdata,float *qdata=NULL,float *idata=NULL);


 
//------------------------ 开关保护关系表 --------------------------//
typedef	struct {
		char   breakname[OBJECTNAME_LEN];
		char   protname[OBJECTNAME_LEN];
		uchar  sgjudgef;
} BREAK_PROT_REL_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakProtRelation {
		static int        opentabf;
		static Rdb_Tab   breakprottab;
		static Rdb_Tab   breakprotmdltab;
	public:
		BreakProtRelation(void);
		int  CheckProtRelate(char *,char *,uchar);
		int  MaskBreakProtEnt(char *breakname,int flag);
};


//------------------------ 开关替代关系表 --------------------------//

typedef	struct {
		char   breakreplaced[OBJECTNAME_LEN];
		char   breakname[OBJECTNAME_LEN];
		short  replacetype;
		char   plswitch[OBJECTNAME_LEN];
		uchar  replacemode;
} BREAK_REPLACE_REL_STRU;


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakReplace : public NetDbg{
		static int        opentabf;
		static Rdb_Tab    breakreplacetab;
	public:
		BreakReplace(void);
		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend int AutoCheckBreakReplace(void);

		#if defined(WIN32)
		_SCD_DLLPORT 
		#endif
		friend int Break_Replace(char *breakreplaced,char *breakname,uchar replacemode,char *hostname,char *username);
};

/////////////////////////  开关连接刀闸表 ////////////////////////////////
typedef	struct {
		char   breakname[POWDEVNAME_LEN];
		char   switchname[POWDEVNAME_LEN];
		short  swtype;
		short  distance;
		uchar  sgmode;
} BREAK_SWITCH_REL_STRU;


class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
BreakSwitchRelation {
		static int        opentabf;
		static Rdb_Tab    breakswitchtab;
	public:
		BreakSwitchRelation(void);
		int CheckSGCond_Switch(char *breakname);
		int CheckYKCond_Switch(char *breakname);
};


/***************************  事故处理类 ******************************/

// 事故处理
#define  ESOURCE_SGZ			1			// 事故源：事故总信号
#define  ESOURCE_PROT			2			//         保护信号
#define  ESOURCE_PROTMODULE		3			//         保护模块事件信号

// 开关等待队列 
typedef	struct {
		intertime  recvtime;					// 收到时间
		unsigned short recvms;
		uchar	   yieldentf;
		int        lifetime;					// 生命周期
		uchar      sgcondition;					// 事故判断条件
		uchar      haveprotf;					// 
		SYS_CLOCK  entclock;					// 事项发生时间
		char       substation[SUBSTNAME_LEN];	// 厂站名	
		char       breakname[OBJECTNAME_LEN];	// 开关名
		uchar      staval;						// 状态值
		int        relatecond;					// 关系条件
		float      pdata;
		float      qdata;
		float      idata;
		short	   zygroupno;
		uchar       relclasstype;
		char		relname[OBJECTNAME_LEN];
		char		reldescribe[OBJDESCRIBE_LEN];
} BRKWAITQUE_STRU;

// 关系对象等待队列 
typedef	struct {
		intertime  recvtime;					// 收到时间
		unsigned short recvms;
		int			lifetime;					// 生命周期
		uchar      classtype;					// 
		char       substation[SUBSTNAME_LEN];		// 厂站名
		char       objname[OBJECTNAME_LEN];		// 对象名
		char	   objdescribe[OBJDESCRIBE_LEN];
} RELATEWAITQUE_STRU;


// 事故处理类
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
AccidentProc : public NetDbg{
		static ComnLinkTable<BRKWAITQUE_STRU>	    brkwaittab;		// 开关等待表
		static ComnLinkTable<RELATEWAITQUE_STRU>	relatetab;		// 关系表
		// 产生<开关事项>应用事件，用来区分未判事故前的<变位>应用事件
		int		YieldBreakApEveent(SYS_CLOCK *,char *,uchar,uchar,short,float,float,
			float,uchar relclasstype=0,char *relname=NULL,char *reldes=NULL,uchar testflag=0);
	public:
		// 往开关等待队列中插入一个新的开关变位
		int		InsertBreakEvent(SYS_CLOCK *entclock,char *substation,char *breakname,
			    uchar staval,short zygroupno,float pdata,float qdata,float idata,uchar testflag);
		// 往保护关系等待队列中插入事故总信号
		int		InsertSgzEvent(char *substation,char *sgzname,uchar staval);
		// 往保护关系等待队列中插入保护动作
		int		InsertProtEvent(char *substation,char *protname,uchar staval);
		int		InsertProtModuleEvent(char *substation,char *protmodulename,uchar staval);
		// 清理队列
		int		ClearBreakEvent(void);
		// 判开关变位是否事故
		int		CheckBreakEvent(void);
};
  

/////////////////// 状态控制条件   //////////////////////////////////
#define  MAX_YKCONDITION_NUMS     10

typedef struct {
	char  name[OBJECTNAME_LEN];		// 名字
	uchar yktype;//condtype;控制类型
	uchar condgroupno;	//条件组号
	struct {
		char  condname[OBJECTNAME_LEN];
		uchar condstate;
	} ctrlcond[MAX_YKCONDITION_NUMS];
}  YKCONDITION_STRU;


int  
#if defined(WIN32)
_SCD_DLLPORT 
#endif
CheckYKCondition(char *name,uchar  state);


typedef	struct {
		char    name[OBJECTNAME_LEN];		//  名字
		char    describe[OBJDESCRIBE_LEN];	//	描述                         //add
		int     securegrade;				//  安全级
        char    substation[SUBSTNAME_LEN];	//	站所线系代码                 //add
		char    devcode[OBJECTNAME_LEN];	//  设备代码                     //add
		ushort	baseobjtype;				//  遥信类型                     //add
        short	groupno;					//	分组序号                     //add
		short   ykterminalno;				//  终端序号
		short   ykno;						//  遥控序号
        uchar   monflag;					//  调度模式 是否监督？
		uchar   ykmode;                     //  遥控方式
		char    entdealmode[OBJECTNAME_LEN];//  事项处理方式                 //add
		char    almgraname[GRAP_LEN];	    //  推图名                       //add
		char    khprocname[OBJECTNAME_LEN]; //  控合过程名                   //add
		char    kfprocname[OBJECTNAME_LEN]; //  控分过程名                   //add
//		uchar   monflag;					//  监督标志
		uchar   exykflag;					//  扩展遥控：1--不检验遥信状态
		uchar   waitchktime;				//  遥控返校时间限
		uchar   sendcmdtime;				//  遥控发令时间限
		uchar   execcmdtime;				//	遥控执行时间限
		char    ykentname[ENTNAME_LEN];	    //  遥控事项句
		uchar   lockykf;					//  封锁遥控标志
		
//		char    ykactionname[OBJECTNAME_LEN];	// 遥控动作名称
} YKPARA_STRU;


// 升降参数
typedef	struct {
		char   name[OBJECTNAME_LEN];		// 名字
		char   describe[OBJDESCRIBE_LEN];	//	描述                         //add
		int    securegrade;				    //  安全级
        char   substation[SUBSTNAME_LEN];	//	站所线系代码                 //add
		char   devcode[OBJECTNAME_LEN];	    //  设备代码                     //add
		short	groupno;					//	分组序号                     //add
		short  terminalno;			        //  终端序号
		uchar  monflag;					    //  调度模式
		short  upykno;						//  升遥控号
		uchar  upykaction;					//  升动作
		uchar  upykcmd;					    //  升命令
		short  downykno;					//  降遥控
		uchar  downykaction;				//  降动作
		uchar  downykcmd;					//  降命令
		short  pauseykno;					//  急停遥控号
		uchar  pauseykaction;				//  急停动作
		uchar  pauseykcmd;					//  急停命令
//		uchar  monflag;					    //  监督标志
		uchar  waitchktime;				    //  返校时间限
        uchar  sendcmdtime;				    //  发令时间限
		uchar  execcmdtime;				    //  执行时间限
        char    entdealmode[OBJECTNAME_LEN];//  事项处理方式                 //add
		char   entname[ENTNAME_LEN];	    //  事项句名
		char   upctrlogic[PROCNAME_LEN];	//  控升逻辑
		char   downctrlogic[PROCNAME_LEN];	//  控降逻辑
		uchar  updownmode;					//  升降方式
		uchar  lockupdownf;				    //  封锁升降标志
} UPDOWNPARA_STRU;


// 遥控队列
typedef struct {
		char      tabname[TABNAME_LEN];		    // 表名
		char      name[OBJECTNAME_LEN];			// 对象名
		char      describe[OBJDESCRIBE_LEN];	// 描述
		char	  groupname[OBJECTNAME_LEN];    //  组名
		int       securegrade;					// 安全级
		uchar     yktype;						// 遥控类型
		uchar     actype;						// 动作类型
		ushort    ykgroupno;                    // 分组序号
		ushort    ykterminalno;					// 终端号
		ushort    ykno;							// yk 号
		uchar     monflag;						// 监督标志
		uchar     exykflag;					    // 扩展遥控：1--不检验遥信状态
		short     waitchktime;					// 等待返校时间	
		short     sendcmdtime;					// 发送命令时间
		short     execcmdtime;					// 执行命令时间
		uchar     ykmode;						// yk模式
		char      entname[ENTNAME_LEN];		    // 事项句
		char      opmachine[HOSTNAME_LEN];		// 操作主机名
		char      operatorname[USERNAME_LEN];	// 操作员名
		uhlong     optaskid;					// 操作任务号
		char      confirmhost[HOSTNAME_LEN];	// 许可主机名
		char      confirmname[USERNAME_LEN];	// 许可操作员名
		uhlong    confirmtaskid;				// 许可任务号  
		uchar     firstcmdf;					// 
		uchar     programykf;					// 
		uchar     intercmdf;					// 
		uchar	  ykfangshi;					//  遥控方式 0:标准 1:用升降控制令
		intertime yktime;					    //
		uchar     yksercode;					//
		uchar     sendcmdf;						//
		uchar     ykaction;						//
		int       updownval;					//
} YKQUEUE_STRU;



typedef	struct  {
		uchar   lockf;	// 锁定标志
		uchar   getshareykf;  //  请求取并行遥控标志
		uchar   shareykf;   //  并行遥控标志
		YKQUEUE_STRU  *lockykquep;  // 有权遥控的队列
} YK_LOCK_STRU;



// 遥控队列类
class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ykqueue : public NetDbg{
		static	PROG_SHARECTRL_STRU	*ykqueuectrl;
		static	Rdb_Tab		ykqueuetab;
		static  int             initlockf;
		static	YK_LOCK_STRU		terminalyklock[SCADA_MAX_TERMINALNUMS];

		//  抓获控制权
		int		GrapPutRight(void);
		//  释放控制权
		void	ReleasePutRight(void);
		//	处理遥控命令  
		int		YkCmdProcessing(YKPARA_STRU  *ykparap,char *tablename,uchar action,int ykcmd,uchar ykmode,uchar  programykf,
					char *opmachine,char *operatorname,uhlong optaskid,uchar eflag,char *groupname=NULL);
		//  处理升降命令
		int		UpdownCmdProcessing(UPDOWNPARA_STRU  *updownp,uchar action,int ykcmd,uchar ykmode,
							uchar  programykf,char *opmachine,char *operatorname,uhlong optaskid);
		//  生成应用事件
		int		YieldYkApEvent(int yksercode,YKQUEUE_STRU *ykqueuep);		//  发送遥控命令;
		int		SendYkCmd(int ykcmd,int groupno,int ykrtuno,int ykno,uchar ykaction);
		//  撤消已做的予置
		int     CancelYkPreset(YKQUEUE_STRU *ykqueuep,uchar entf=0);
		//  处理遥控队列
		int	    YkQueueProcessing(YKQUEUE_STRU *ykqueuep);
		//	锁定TERMINAL遥控
		int		LockTerminalYk(YKQUEUE_STRU *ykqueuep);
		//  解锁TERMINAL遥控
		int		UnlockTerminalYk(YKQUEUE_STRU *ykqueuep);
		//  清理TERMINAL遥控
		void    ClearTerminalYk(void);
		// 判断遥控条件
		int     CheckYkCond(YKPARA_STRU  *ykparap,char *tabname,BASESTATUS_STRU *bp,int actype,uchar eflag);
		// 判断升降条件
		int     CheckUpdownCond(UPDOWNPARA_STRU  *updownp,int actype);
	public:
		Ykqueue(void);
		char	errorinfo[100];  
		int		YkCmd(char *name,char *tablename,uchar actype,int ykcmd,uchar ykmode,char *opmachine,
							char *operatorname,uhlong optaskid,uchar eflag);

		int		AutoYkCmd(char *name,char *tablename,uchar actype,int ykcmd,
					char *opmachine,char *operatorname,uhlong optaskid,char *groupname=NULL);

		int		ProgYk(char *name,char *tablename,uchar actype,int ykcmd,char *programname,uchar eflag=0);

		int		UpdownCmd(char *name,uchar actype, int ykcmd,uchar ykmode,char *opmachine,
							char *operatorname,uhlong optaskid);

		int		ProgUpdown(char *name,uchar actype,int ykcmd,char *programname);
		int		YkCheck(int ykrtuno,int ykno,uchar chkcode);
		int		YkResult(char  *tabname,char *objname,int action);
		int		YkQueueLoop(void);
		int     StopYk(char *name);		
		int     StopGroupYk(char *groupname);		
		int     StopAllYk(void);			
		int     CheckStopYk(char *name);
};

///////////////////////////////// 遥调 ///////////////////////////////////////////////////
// 遥调参数
typedef	struct {
		char    name[OBJECTNAME_LEN];		// 名字
		int     securegrade;				// 安全级
		short   terminalno;				// 
		short   groupno;				// 
		short   ytno;						//
		uchar   outtype;					// 输出类型
		float   maxengval;					
		int		maxoutval;
		float   minengval;
		int     minoutval;
		float   highlimit;
		float   lowlimit;
		float   tolerance;
		short   execcmdtime;				//
		char    realhlmname[OBJECTNAME_LEN];
		char    realllmname[OBJECTNAME_LEN];
		char    condname1[OBJECTNAME_LEN];
		uchar   condstate1;
		char    condname2[OBJECTNAME_LEN];
		uchar   condstate2;
		char    entname[ENTNAME_LEN];	// 事项句名
		char	ytlogic[PROCNAME_LEN];	// 控升逻辑
		uchar   lockytf;				// 封锁升降标志
} YTPARA_STRU;


// 遥调队列
typedef struct {
		char      name[OBJECTNAME_LEN];			// 对象名
		char      describe[OBJDESCRIBE_LEN];	// 描述
		int 	  securegrade;				    // 安全级
		short     groupno;
		short	  terminalno;
		short	  ytno;
		uchar     outtype;
		short     execcmdtime;					// 执行命令时间
		float     tolerance;
		char      entname[ENTNAME_LEN];		    // 事项句
		char      opmachine[HOSTNAME_LEN];		// 操作主机名
		char      operatorname[USERNAME_LEN];	// 操作员名
		uhlong    optaskid;					    // 操作任务号
		int			cmdcode;
		intertime yttime;					    //
		uchar     sendcmdf;						//
		float     setvalue;				     	//
		float	  curvalue;
		int		  outvalue;
} YTQUEUE_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Ytqueue : public NetDbg{
	static ComnLinkTable<YTQUEUE_STRU>	ytqueuetab;		// 遥调队列表
	int  SearchYtQueue(char *name);
	int  CheckYtCond(YTPARA_STRU *ytparap,int cmdcode,float setvalue);
	int  YtCmdProcessing(YTPARA_STRU *ytparap,int cmdcode,float setvalue,char *opmachine,
							char *operatorname,uhlong optaskid);
	int  YieldYtApEvent(int ytsercode,YTQUEUE_STRU *ytqueuep,int failcode);
public:
	//Ytqueue(void);
	int	 YtCmd(char *name,int cmdcode,float setvalue,char *opmachine,
							char *operatorname,uhlong optaskid);
	int  YtQueueLoop(void);
};




///////////////////////////  配电故障状态检测 /////////////////////////////////
/*
typedef struct {
	short	terminalno;
	short	yxno;
	char	name[OBJECTNAME_LEN];
	char	describe[OBJDESCRIBE_LEN];
	int		chkenttype;  //检测事件类型
	char	brkname[OBJECTNAME_LEN]; //对应开关代码
}  DMS_FAULTENTTYPE_STRU;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
DMSFaultEventType {
	static int  opentabflag;
	static Rdb_Tab  dmsfaulttable;
public:
	DMSFaultEventType(void);
	DMS_FAULTENTTYPE_STRU  *GetDMSFaultEntType(short terminalno,short yxno);
};
*/
/********************************* 事项处理 **********************************/


   
// 事项类

class	
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Event : public ScdComnEvent {
	public:
		// 越限事项（表名，对象名，时间，安全级，值）
		int		 AddOverlimitEvent(char *tabname,char *objname,SYS_CLOCK *,int olgrade,float  olvalue);
		// 多态量变化事项（表名，对象名，时间，状态）
		int		 AddMultiStaChgEvent(char *tabname,char *objname,SYS_CLOCK *,int  staval);
		// 状态量变化事项（表名，对象名，时间，事项标志，状态）
		int		 AddStateChangeEvent(char *tabname,char *objname,SYS_CLOCK  *chgclockp,int entflag,
					int  staval,float pdata=0.0,float qdata=0.0,float idata=0.0,
					uchar relclasstype=0,char *relname=NULL,char *reldes=NULL,uchar testflag=0);
		// 遥控、升降事项（遥控/升降标志，时间，表名，对象名，安全级，  ）
		int		 AddYkandUpdownEvent(int ykupdownf,SYS_CLOCK  *ykclockp,char *tabname,
									char *objname,int securegrade,char *actbasemsg,
									int		 actype,char *procbasemsg,int  procserode,char *ophostname,
									char	 *operatorname,char  *entname);
		int 	 AddYtEvent(SYS_CLOCK  *ytclockp,char *name,uchar securegrade,
				 	int ytsercode,int failcode,float setvalue,float curvalue,
					int outvalue,char *ophostname,char *operatorname,char *entname);
		//人工置数事项
		int      AddMansetEvent(char* hostname,char *username,char* tabname,char* objname,uchar securegrade,
							uchar setflag,double setval,SYS_CLOCK* clockp);
		//五防事项
		int      AddWfbsEvent(char* tabname,char* objname,uchar securegrade,uchar setflag, SYS_CLOCK* apentclock);

		int      AddSetSpecialflagEvent(char* hostname,char* username,uchar setflag,CARD_RCD_STRU *cardrcdp,SYS_CLOCK *clockp);
		//生成旁路替代事项
		int      AddBreakReplaceEvent(char* hostname,char* username,char* breakname,char * break_replaced,uchar mode,SYS_CLOCK *clockp);
		int      AddDmsDiagnoseEvent(int faulttype,char* startbreakname,char* startftuname,char* endbreakname,char* endftuname,SYS_CLOCK* clockp);
		//生成配电故障诊断事项
//		int      AddDmsFaultEvent(DMS_FAULTENTTYPE_STRU *dmsfltentypep,SYS_CLOCK *clockp);
		//int      AddTerminalEvent(char *substation,char *terminalname,char *terminaldes,int state,int security);
		int      AddTerminalEvent(char *terminalname,char *terminaldes,int state,short channelno);
		int      AddChannelEvent(char *channelname,char *channeldes,int channel_no, bool main_back);
//		int      AddSubstationEvent(char *substation,int almtype);
		int      AddYxTrembleEvent(char *objname,short objtype,int tremblenums);//生成遥信抖动事项
};

/******************************* 用户过程表 **********************************/

#define  CALSTMODE_NULL              0
#define  CALSTMODE_INTERV            1
#define  CALSTMODE_ACCURATE          2
#define  CALSTMODE_DBCHANGE          3
#define  CALSTMODE_STACHANGE         4
#define  CALSTMODE_SGALARM           5
#define  CALSTMODE_HDBACCURATE       6
#define  CALSTMODE_HISDBCHANGE       7

#define  CALSTMODE_FIELDUPD          11

#define  CALSTMODE_FOREVERRUN        127



#define  CALTRIGGERNUMS              3
#define  CALINPARANUMS               10

typedef struct {
		short       no;//序号                         //add
		short       groupno;//分组序号                //add
	    char		callprocname[PROCNAME_LEN];//用户过程名
		uchar		startmode;//执行模式
		short		everyminute;//间隔分钟
		uchar		everysecond;//间隔秒
		char		atmonth;//定时月
		char		atday;//定时日
		char		athour;//定时小时
		char		atminute;//定时分钟
		char		trigger[CALTRIGGERNUMS][48];//触发器
		char		inargv[CALINPARANUMS][30];//输入参数
		uchar       usef;//使用标志                   //add
		int			returncode;//返回码
		double		returnvalue;//返回值
		double		lastvalue[CALTRIGGERNUMS];//保存值
		intertime	lastproctime;//执行时间 
} USERPROCPARA_STRU;
    
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
User_Procedure {
		static  int       opentabf;
		static  Rdb_Tab  calcultparatab;	
		int		CheckTimeCond(USERPROCPARA_STRU *calp);
		int		CheckHdbTimeCond(USERPROCPARA_STRU *calp);
		int		CheckDbChgCond(USERPROCPARA_STRU *calp);
		int		SplitDbFldName(char *dbfieldname,char *tabname,char *objname,char *fldname);
		int		CheckFldUpdCond(USERPROCPARA_STRU *calp,char *tabname,char *objname,char *fldname);
		int		CheckStaChgCond(USERPROCPARA_STRU *calp,char *tabname,char *objname);
		int		CheckSgAlarm(USERPROCPARA_STRU *calp,char *objname);
		int		ExecOneProc(int groupno,USERPROCPARA_STRU *calp);
	public:
		User_Procedure(void);
		int		InitUserprocTab(void);//初始化
		int		ExecTimerProc(void);//执行定时过程
		int		ExecHdbTimerProc(void);
		int		ExecDbChgProc(void);//执行库改变触发过程
		int		ExecFldUpdProc(char *tabname,char *objname,char *fldname);//执行域改变触发过程
		int		ExecStaChgProc(char *tabname,char *objname);//执行状态改变触发过程
		int		ExecAccidentAlarmProc(char *objname);//执行事故告警过程
    };





/************************* 节点权限表 ********************/
int  CheckUserPassword(char *um,char *password);


typedef  struct {
	char	hostname[HOSTNAME_LEN];		//主机名
	char	username[USERNAME_LEN];		//用户名
	uchar   modifyjhcurve;              //修改计划曲线
	uchar   modifyrealdata;             //修改实时数据
	uchar   modifyhiscurve;             //修改历史曲线
	uchar   modifyhisent;               //修改历史事项
	uchar   modifygraph;                //修改图形
	uchar   modifynetpara;              //修改网络参数
	uchar   modifydbpara;               //修改数据库参数
	uchar   modifytable;                //修改报表
	uchar   viewjhcurve;                //查看计划曲线
	uchar   viewrealdata;               //查看实时数据
	uchar   viewhiscurve;               //查看历史曲线
	uchar   viewhisent;                 //查看历史事项
	uchar   viewgraph;                  //查看图形
	uchar   viewnetpara;                //查看网络参数
	uchar   viewdbpara;                 //查看数据库参数
	uchar   viewtable;                  //查看报表
	uchar   viewxbdata;                 //查看谐波数据
	uchar   viewlbdata;                 //查看录波数据
	uchar   viewdebuginfo;              //查看调试信息
	uchar   viewrawcode;                //查看通讯原码
	uchar   ykright;                    //遥控
	uchar   updownright;                //遥调
	uchar   superykright;               //超级遥控
	uchar   stopfresh;                  //停闪
	uchar   putcard;                    //挂牌
	uchar   entconfirm;                 //事项确认
	uchar   netchecktime;               //网络对时
	uchar   switchhost;                 //切换主机
	uchar   switchchannel;              //切换通道
	uchar   computerprot;               //微机保护
	uchar   modifyprotvalue;            //修改定值
	uchar   protrunstop;                //保护投退
	uchar   modifysetvalnum;            //修改定值区号
	uchar   protreserverd1;             //微机保护预留1
	uchar   protreserverd2;             //微机保护预留2
	uchar   protreserverd3;             //微机保护预留3
	uchar   stoprtu;                    //停运RTU
	uchar   controlmnp;                 //控制模拟盘
	uchar   lockforedata;               //封锁前置数据
	uchar   paradownload;               //参数下载
	uchar   paraupload;                 //参数上载
	uchar   guangzipai;                 //光字牌
	uchar	opright[MAXSECUREGRADE];	// 操作权限：=1为有相应安全级操作权限// 由securegrade 按位取得
} USEROPERMODE_STRU;
   
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
UserOperMode : public NetDbg{
		static  int  opentabf;
		static  Rdb_Tab useropmodetab;
	public:
		UserOperMode(void);
		// 根据主机名、用户名检查有无指定安全级的操作权限
		USEROPERMODE_STRU  *GetUserOperMode(char *hm, char *um);
		int CheckSecureGrade(USEROPERMODE_STRU *uopm, int securegrade);
		int CheckSecureGrade(char *hm, char *um, int securegrade);
};


//序列控制组参数表结构
typedef struct {
       char serialname[OBJECTNAME_LEN];
       char describe[OBJDESCRIBE_LEN];
	   short groupno;
       uchar ykmode;
       uchar interval;                       //add
} SERIALYKGRP_STRU;

//序列控制对象参数表结构
typedef struct {
       char  serialname[OBJECTNAME_LEN];
       char  breakname[OBJECTNAME_LEN];
       short ykorder;
	   uchar actype;
       uchar condition;
//     uchar ykstyle;
//     short interval;
}SERIALYKOBJ_STRU;



typedef struct  {
	int   groupno;
	char  tablename[OBJECTNAME_LEN];
	char  objname[OBJECTNAME_LEN];
	uchar actype;
	uchar condition;
    short interval;
	uchar sendcmdf;
}  SERIALYKOBJ_UNIT_STRU;


typedef struct  {
	char		groupname[OBJECTNAME_LEN];
	char		ophostname[HOSTNAME_LEN];
	char		operatorname[USERNAME_LEN];
	hlong		optaskid;
	uchar		stepflag;
	uchar		contflag;
	int			objnums;
	int			curserno;
	intertime	ctrltime;
	SERIALYKOBJ_UNIT_STRU	*serialykobj;
}  SERIAL_GROUP_CTRL_STRU;


#define  MAX_SERIAL_GROUP_NUMS  30

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
SerialYK {
	static SERIAL_GROUP_CTRL_STRU  ykgroup[MAX_SERIAL_GROUP_NUMS];
	static unsigned char  lockflag;
	void ClearSerialYK(SERIAL_GROUP_CTRL_STRU *,char notsendserialendmesg=0);
public:
	//SerialYK(void)
	int AddGroup(char *ophostname,char *operatorname,uhlong taskid,
			char *groupname,int serialtype,int mode,int nums,SERIALYKOBJ_UNIT_STRU *serobjp);
	int StopGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int PauseGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int ContinueGroup(char *ophostname,char *operatorname,uhlong taskid,char *groupname);
	int ProcYkResult(char *objname,int okf);
	int ExecSerialYK(void);
	int StopAllGroup();
	int SendSerialYKMsg(char *hm,char *um,uhlong tid,char *groupname,int curstate,char *pauseobjname,int errorcode=0);

};

/////////////////////////////////////////
// 处理标志牌记录表
/////////////////////////////////////////

#ifdef WIN32
_SCD_DLLPORT 
#endif
int SetBusCheckFlag(int cardtype,char *busname,int flag);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int CheckDevCardCond(int devtype,int cardtype,char *devname,int flag);


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ProcCard {
	static int  opentabflag;
	static Rdb_Tab  cardtable;
	Rdb_PreMalloc	cardmalloc;
public:
	ProcCard(void);
	~ProcCard(void) {cardmalloc.freespace();}
	int GetOneCard(short objtype,char *objname,int cardtype,CARD_RCD_STRU *cardp);
	int GetAllCard(short objtype,char *objname,int *nums,CARD_RCD_STRU **cardp);
	int InitBusCheckFlag(void);
};

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// 厂站同YC、YX的关系 ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	char substname[SUBSTNAME_LEN];
	int  ana_stno;
	int  ana_nums;
	int  brk_stno;
	int  brk_nums;
	int  sw_stno;
	int  sw_nums;
	int  prot_stno;
	int  prot_nums;
	int  sta_stno;
	int  sta_nums;
	int  dmssta_stno;
	int  dmssta_nums;
}  SUBSTATION_RELATION_STRU;

class	
#if defined(WIN32)
_SCD_DLLPORT  
#endif
SubstationRelation {
	static SUBSTATION_RELATION_STRU	*substrelidx;
	static intertime				substupdtime;
	static intertime				brkupdtime;
	static intertime				swupdtime;
	static intertime				protupdtime;
	static intertime				staupdtime;
	static intertime				anaupdtime;
	static int						substnums;
	static int						ananums;
	static int						brknums;
	static int						swnums;
	static int						protnums;
	static int						stanums;
	static BASEANALOG_STRU			**ana_addr;
	static BASESTATUS_STRU			**brk_addr;
	static BASESTATUS_STRU			**sw_addr;
	static BASESTATUS_STRU			**prot_addr;
	static BASESTATUS_STRU			**sta_addr;
	static BASESTATUS_STRU			**dmssta_addr;
public :
	SubstationRelation(void);
	void InitAllIndex(void);
	void InitBreakIndex(void);
	void InitSwitchIndex(void);
	void InitProtsigIndex(void);
	void InitStatusIndex(void);
	void InitDmsStatusIndex(void);
	void InitAnalogIndex(void);
	void CheckAllRelation(void);
	BASEANALOG_STRU **GetAnalogRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetBreakRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetSwitchRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetProtsigRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetStatusRelation(int substrcdno,int *relnums);
	BASESTATUS_STRU **GetDmsStatusRelation(int substrcdno,int *relnums);
};




#endif
