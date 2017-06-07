#ifndef  _SCD_DEF_H
#define  _SCD_DEF_H

#include "sysdef.h"
#include "sysstr.h"
#include "cmddef.h"
#include "rdb/rdbapi.h"


/***********************SCADA 邮件结构定义**************************/

#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
#define scdnetbyte(type,addr)   RdbNetbyte(type,addr,RDB_DATA_FROM_HOST_TO_NET)
#define scdhostbyte(type,addr)  RdbNetbyte(type,addr,RDB_DATA_FROM_NET_TO_HOST)
#else
#define scdnetbyte(type,addr)
#define scdhostbyte(type,addr)
#endif

typedef unsigned char uchar ;
typedef unsigned short ushort;


typedef struct 
{
   short    servicetype;
   char		password[PASSWORD_LEN];
   char     reserve_char[6];
}SCADA_SRV_HEAD_STRU;


typedef struct 
{
   short    servicetype;
   char     reserve_char[6];
}SCADA_CLI_HEAD_STRU;


typedef struct 
{
   int   errorcode;
   short servicetype;
   char  errorinf[ERRORINFO_LEN];
} SCADA_ERR_STRU;

typedef struct 
{
   int   groupno;
   char  tablename[OBJECTNAME_LEN];
   char  objectname[OBJECTNAME_LEN];
   uchar setflag;
   uchar datatype;
   double  setval;
 } SCADA_MANSET_STRU;
 
typedef struct 
{
	int groupno;
	uchar  setflag;
	CARD_RCD_STRU  cardrcd;
} SCADA_SET_SPECIALF_STRU;


typedef struct {
	int groupno;
	uchar  clearf;
	SYS_EVENT_EX notifyque;
} SCADA_CLR_NOTIFYQUE_STRU;
 
typedef struct {
	int groupno;
	short no;
	uchar wfbsflag;
	uchar lockallflag;
} SCADA_WFBS_STRU;

#define  WFBS_UNLOCK   0
#define  WFBS_LOCK     1

#define  WFBS_LOCKSINGLE   0
#define  WFBS_LOCKALL      1

typedef struct
{
	int groupno;
	int	mode;
	int nums;
}  SCADA_CLRALARMF_STRU;

typedef struct 
{
   int groupno;
   char  tablename[OBJECTNAME_LEN];
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   uchar actcode;
   uchar eflag;
 } SCADA_YK_STRU;
 

typedef struct 
{
   int groupno;
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   float setvalue;
 } SCADA_YT_STRU;


typedef struct 
{
   int groupno;
   char  objectname[OBJECTNAME_LEN];
   uchar cmdcode;
   uchar actcode;
 } SCADA_UPDOWN_STRU;


typedef struct
{
    char  objname[OBJECTNAME_LEN];
    char  tablename[OBJECTNAME_LEN];
    char  acttype; //合分、升降
	char  res[3];
}SCADA_YKOBJNAME_STRU;

typedef struct
{
	int  groupno;
	char groupname[OBJECTNAME_LEN];
	int  objnums;
}SCADA_GROUPYK_STRU; 

typedef struct
{
	int groupno;
	char serialname[OBJECTNAME_LEN];
	uchar  serialtype;  // 0 -- 预定义序列   1--- 临时序列
	char grpykmode;  // 0--连续执行    1--单步执行
	int  objnums;
}SCADA_SERIALYK_STRU;

typedef struct
{
	int groupno;
	char serialname[OBJECTNAME_LEN];
	uchar  ctrlcmd;
}SCADA_SERIALYK_CTRL_STRU; 



typedef struct
{
    int groupno;
	char breakname[OBJECTNAME_LEN];
	char break_replaced[OBJECTNAME_LEN];
	char mode;
	char res[7];
}SCADA_LINE_REPLACE_STRU;

typedef struct
{
	int groupno;
	char breakname[OBJECTNAME_LEN];
	char zygroupno;
	char res[3];
}SCADA_STARTPDR_STRU;

typedef struct
{
   	short  	terminalno;             //终端号
   	unsigned char protocoltype;     //规约类型  (决定保护信息的解释方法) 0-->LFP, 1-->ISA ,   2-->SEL
   	unsigned char gram_len;			//保护报文的长度
	unsigned char gram[256];		//保护报文
}SCADA_PROT_STRU;

typedef struct {
   CARD_RCD_STRU   cardrcd;	
//   char		res[2];								//补齐
   char    devdescribe[OBJDESCRIBE_LEN];		// 设备描述
} SCADA_NEW_SPECIALF_STRU;//2004 05 add
typedef struct {
   char   name[OBJECTNAME_LEN];	
   char    describe[OBJDESCRIBE_LEN];		// 描述
} SCDDEVICE_STRU;//2004 05 add

typedef struct
{
   	unsigned char groupno;    
   	unsigned char security;	
	unsigned char wav_flag;	
}RALARM_WAV;


/****************************************************************************************************/
/*****************************************SCADA_YK_MACRO DEFINE**************************************/
/****************************************************************************************************/
#define  SCADA_YK_PRESET            0		// 遥控预置
#define  SCADA_YK_CHECK_OK          1		// 遥控返校正确
#define  SCADA_YK_CHECK_ERR         2		// 遥控返校错误
#define	 SCADA_YK_CHECK_TIMEOUT	    3		// 遥控返校超时
#define  SCADA_YK_COMFIRM           4		// 遥控确认
#define	 SCADA_YK_COMFIRM_TIMEOUT	5		// 遥控确认发令超时
#define  SCADA_YK_EXEC              6		// 遥控执行
#define  SCADA_YK_EXEC_TIMEOUT	    7		// 遥控执行发令超时
#define  SCADA_YK_CANCEL            8		// 遥控撤消
#define  SCADA_YK_SUCCESS           9		// 遥控成功
#define  SCADA_YK_EXEC_FAIL         10		// 遥控拒动
#define  SCADA_YK_CMD_ERR		    126		// 遥控命令错误
#define  SCADA_YK_NULL              127		// 空：过程已终结




typedef struct {
   char  objectname[OBJECTNAME_LEN];
   uchar resulttype;
   char  operatordes[OBJDESCRIBE_LEN];
   char  confirmdes[OBJDESCRIBE_LEN];
   int   errorcode;
   char  errorinf[ERRORINFO_LEN];
 } SCADA_YKRESULT_STRU;


#define  SCADA_SERIALYK_MESG_PAUSE     0	 // 序列暂停
#define  SCADA_SERIALYK_MESG_CMD_ERR   126	 // 命令错误
#define  SCADA_SERIALYK_MESG_END       127	 // 序列结束

typedef struct {
	int   groupno;
	char  serialname[OBJECTNAME_LEN];
	uchar curstate;
	char  pauseobjname[OBJECTNAME_LEN];
    int   errorcode;
    char  errorinf[ERRORINFO_LEN];
} SCADA_SERIALYK_MESG_STRU;

//////////////////////////////////////////////////////////////////////////////////// 
 
typedef struct 
{
  int   groupno;
  int   terminalno;
  int   ykno;
  uchar chkokf;
} SCADA_FOREYKCHK_STRU;

typedef struct {
	int     groupno;
	short	Year;
	char	Month;
	char	Day;
	char	Hour;
	char	Minute;
	char	Second;
	char	reserve;
	short	Ms;
	short	reserve1;
	char	Source;
	char	type;
	short	TERMINALNo;
	union
	{
		float	YCVal;
		struct
		{
			unsigned char	YXVal;
			unsigned char	YXres;
			unsigned short	yxOrder;
		}yx;
		struct
		{
			unsigned char status;
		}chan;
	}Val;
		//当type 为故障诊断时用到下面的内容
    char            accidenttype; //事故类型:0--无法区分 1-单相接地 2-相间短路
    char            accidentline; //0-保留 1-A相 2-B相 3-C相 4-AB 5-BC 6-AC 7-ABC
	short			YxOrder1;
    short           FTUNo1;
	short			YxOrder2;
    short           FTUNo2;

} SCADA_RAWEVENT_STRU;

typedef struct {
	int  groupno;
	char	tabname[TABNAME_LEN];
	char	objectname[OBJECTNAME_LEN];
	uchar	olgrade;
	float	olvalue;
} SCADA_OVERLIMIT_STRU;

typedef struct
{
	int  groupno;
	char name[OBJECTNAME_LEN];
	char isstation;
	char setflag;
}SCADA_MASKCHG_STRU;


typedef struct 
{
	uchar	accidenttype;
	uchar	accidentline;
	uchar	Month;
	uchar	Day;
	uchar	Hour;
	uchar	Minute;
	uchar	Second;
	uchar	reserve;
	ushort	Ms;
	ushort	Year;
	ushort	TERMINALNo;
	ushort   FTUNo;
	ushort   LineNo;
	ushort   YXNo;
	short	ycvalue[16];
}SCADA_FTUFAULT_RPT_STRU;

typedef struct {
	unsigned char datatype; //FTU故障录波数据的类型
	char res[3];
	char breakname[OBJECTNAME_LEN];
}SCADA_CALL_FAULT_STRU;

typedef struct
{
	uchar	Month;
	uchar	Day;
	uchar	Hour;
	uchar	Minute;
	uchar	Second;
	uchar	reserve;
	ushort	Ms;
	ushort	Year;
	uchar  recordnum;
	uchar  res1;
}FAULT_TIMEANDNUM_STRU;

typedef struct
{
	char    breakname[OBJECTNAME_LEN];
	char    ftuname[OBJECTNAME_LEN];
	char	ycname[OBJECTNAME_LEN];
	uchar	datatype;
	uchar   reserve;
	float	ycvalue[128]; //四个周波128个数据
}FAULT_RCD_FROM_SCADA_STRU;

typedef struct
{
	short	TERMINALNo;
	short   FTUNo;
	short   YXNo;//遥信编号
	ushort  DataType; //D0=1 
	short	ycvalue[128]; //四个周波128个数据
}FAULT_RCD_FROM_FORE_STRU;



typedef struct
{
	char tabname[40];
} SCADA_CC_FORMULA_STRU;



typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	float value;
	uchar dataflag;
} SCADA_NET_YCDATA_STRU;

typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	short value;
	uchar dataflag;
} SCADA_NET_YXDATA_STRU;

typedef struct {
	int   groupno;
	char  name[OBJECTNAME_LEN];
	float value;
	uchar datatype;
	uchar dataflag;
} SCADA_NET_KWHDATA_STRU;

#define  scdnetbyte_scdshead(p)  { scdnetbyte(RDB_DATATYPE_SHORT,&((p)->servicetype));}
#define  scdhostbyte_scdshead(p)  { scdhostbyte(RDB_DATATYPE_SHORT,&((p)->servicetype));}

//from after.h zxm 20140506
#define		MAILYKCMD	0
#define		MAILYKEXE	1
#define		MAILYKDEL	2
//通道工况
/*#define		CHANUNCHECK	0	
#define		CHANSEARCH	1	//通道搜索
#define		CHANRUN		2 	//通道运行
#define		CHANSTOP	3	//通道停止
#define		CHANRATE	4	//通道误码
#define		CHANUNSYN	5	//通道失步*/

//事项类型
#define		RTUSOE		0x1
#define		HOSTSOE		0x2
#define		STATIONEVENT    0x5
//#define		DAS_DIAGNOSE	0x15

//后台遥控命令
#define		MAILYXNOTTAB	2
#define		MAILRTUPARA		10
#define		MAILCHANPARA	11
#define		MAILSCIPARA		12
#define		MAILN4FPARA		13
#define		MAILCOMPARA		14
#define		MAILDBYKPARA	15
#define		TJ_MAILSETPOINT 15	//天津高压设定
#define		MAILMBCHAN		20
#define     MAILSWITCHCHAN  30  //嘉善通道切换

//	#define     MAXCOLUMN	64
#define		MAILMNPCMD	107
#define		MAILYKOPERATION	100
#define		MAILYKCMD	0//zxm 20140109 
#define		MAILYKEXE	1
#define		MAILYKDEL	2
#define		MAILGROUPYKOPERATION	104
#define		MAILPROTCMD	105
#define         MAILGUAPAI  110
#define		MAILVQCCMD	120
#define 	MAILSETPOINTCMD   106
#define 	MAILSETPOINTEXE   107
#define		MAILSETPOINTDEL   108
#define		MAILYTOPERATION		109
#define     MAILGUAPAI  110
//#define		MAILMNPCMD	111
#define		MAILYXRESET		112		//上海遥控复归
#define		MAILVQCCMD	120

#define		MAILWAVE		130  //故障录波
#define     MAILWAVE_DIR    131  //故障录波目录
#define     MAILWAVE_FILE   132  //故障录波文件
#define     MAILWFYK   133  //五防邮件
#define		MAILREGISTER	  135  //寄存器 adtool->forcom
#define		MAILADTOOL   	  136  //forecom->adtool
#endif

#if defined (__unix)
#define         LOBYTE(w)       ( (unsigned char) (w) )
#define         HIBYTE(w)       ( (unsigned char) ( (unsigned short) (w)>>8) )
#define         LOWORD(l)       ( (unsigned short) (l) )
#define         HIWORD(l)	( (unsigned short) ( (unsigned int) (l)>>16) ) 
#endif