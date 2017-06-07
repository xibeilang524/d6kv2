#ifndef GSCADAIMP_FUNC_H_HEADER_INCLUDED_BB046556
#define GSCADAIMP_FUNC_H_HEADER_INCLUDED_BB046556
//################################################
//############命令码参数说明######################
//################################################

#include "ddef/ddes_comm.h"
//***********************************控制请求**************************************
#define	GSCDM_CTRLREQ		0x100
//遥控

//遥控、升降、下发定值、下发定值区号等所有须遵循下述过程的命令码定义：
#define  GYKCMD_NULL			      0   //无效
#define  GYKCMD_PRESET		      1	  //预置
#define  GYKCMD_EXEC               2   //执行
#define  GYKCMD_CANCEL		      3   //撤消
#define  GYKCMD_CONFIRM            4   //确认

//遥控、升降的方式定义：
#define  GYKFANGSHI_STANDARD	      0	//标准
#define  GYKFANGSHI_UPDOWNCMD      1	//用升降令


//调度员界面发给SCADA服务器的遥控动作码定义：
#define  GACTTYPE_OFF              0   //控分
#define  GACTTYPE_ON               1   //控合

//调度员界面发给SCADA服务器的遥调动作码定义：
#define  GACTTYPE_DOWN             0   //降
#define  GACTTYPE_UP               1   //升
#define  GACTTYPE_PAUSE            2   //急停

typedef struct __tagGScdCTRLReqStruct
{ 
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	objectname[ GDB_CODE_LEN ];
	char	tablename[ GDB_CODE_LEN ];
	int32	cmdcode;
	int32	actcode;
	int32	eflag;
	float	trgvalue;//用于遥调设点方式的目标值
}GSCDCTRLREQ, *PGSCDCTRLREQ;

typedef struct 
{
   char  objectname[ GDB_CODE_LEN ];
   uchar resulttype;
   char  operatordes[ GDB_DESC_LEN ];
   char  confirmdes[ GDB_DESC_LEN ];
   int   errorcode;
   char  errorinf[ GDB_DESC_LEN*2 ];
} GSCADA_YKRESULT_STRU, *PGSCADA_YKRESULT_STRU;

//GSCDCTRLREQ scdctrl;
//success=sendCMD(GSCDCTRLREQ,(uint32)&scdctrl,0);
#define	GSCDM_YK			( GSCDM_CTRLREQ + 1 )
//遥调
//GSCDCTRLREQ scdctrl;
//success=sendCMD(GSCDM_YT,(uint32)&scdctrl,0);
#define	GSCDM_YT			( GSCDM_YK + 1 )
//***********************************序列遥控**************************************

typedef struct 
{
    char  objname[GDB_CODE_LEN];
    char  tablename[GDB_CODE_LEN];
    char  acttype; //合分、升降
	char  res[3];
}GSCADA_YKOBJNAME_STRU,*PGSCADA_YKOBJNAME_STRU;

typedef struct 
{
	int		grpno;
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	serialname[ GDB_CODE_LEN ];
	int		serialtype,mode,objnums;
	PGSCADA_YKOBJNAME_STRU serobjp;
	int		ctrlcmd;
}GSCADA_SERIAL_YK_STRU,*PGSCADA_SERIAL_YK_STRU;
//SendSerialYkCmd
#define GSCDM_SERIAL_YK			(GSCDM_YT+1)
//ctrl_serialyk
#define GSCDM_SERIAL_YK_CTRL	(GSCDM_SERIAL_YK+1)

typedef struct 
{
	int	grpno;
	char	username[ GDB_CODE_LEN ];
	char	password[ GDB_CODE_LEN ];
	char	groupname[ GDB_CODE_LEN ];
	int	objnums;
	PGSCADA_YKOBJNAME_STRU groupobjp;
	int	ctrlcmd;
}GSCADA_GROUP_YK_STRU,*PGSCADA_GROUP_YK_STRU;
//SendGroupYkCmd
#define GSCDM_GROUP_YK			(GSCDM_SERIAL_YK_CTRL+1)
//ctrl_groupyk
#define GSCDM_GROUP_YK_CTRL	(GSCDM_GROUP_YK+1)

//***********************************置位请求**************************************

#define GSCDM_SETFLAG		0x200
//人工置数
typedef struct _tagGScdManSetStruct
{
	char	objectname[ GDB_CODE_LEN ];
	char	tablename[ GDB_CODE_LEN ];
	char	username[ GDB_CODE_LEN ];
	int32	setflag;
	uchar	datatype;
	void	*valaddr;
}GSCDMANSET,*PGSCDMANSET;
//GSCDMANSET scdmanset;
//success=sendCMD(GSCDM_MANFLAG,(uint32)&scdmanset,0);
#define GSCDM_MANFLAG		( GSCDM_SETFLAG + 1 )
//遥信停闪
typedef struct _tagScdClrChgFlagStruct
{
	char	username[ GDB_CODE_LEN ];
	char	code[ GDB_CODE_LEN ];
}GSCDCLRCHGFLAG, *PGSCDCLRCHGFLAG;
//GSCDCLRCHGFLAG scdclr;
//success=sendCMD(GSCDM_CLRCHGFLAG,(uint32)&scdclr,0);
#define GSCDM_CLRCHGFLAG	( GSCDM_MANFLAG + 1 )
//***********************************置位请求**************************************


//***********************************增加记录请求**********************************
#define	GSCDM_ADDRCD		0x300
//五防闭锁
typedef struct _tagGScdWfbsStruct
{
	char	username[ GDB_CODE_LEN ];
	uint16	no;
	uchar	wfbsflag;
	uchar	lockallf;
}GSCDWFBS,*PGSCDWFBS;

#define  GWFBS_UNLOCK   0	//解除闭锁
#define  GWFBS_LOCK     1	//闭锁
#define  GWFBS_LOCKSINGLE   0	//单条遥信闭锁
#define  GWFBS_LOCKALL      1	//全闭锁
#define	GSCDM_WFBS			( GSCDM_ADDRCD + 1 )
//GSCDWFBS	scdwfbs;
//success=sendCMD(GSCDM_WFBS,(uint32)&scdwfbs,0);

//标志牌操作
typedef struct 
{
	int16  groupno;
	int16  firststate;
	int16  secondstate;
	int16  objtype;
	char   objname[24];
	int16  cardtype;
	int16  cardno;
	int32    startx;
	int32    starty;
	int32	metaid;
	char   hostname[24];
	char   username[24];
	char   cardtime[80];
	char   graphpath[255];
	char   metapath[255];
	char   context[255];
	char   devdescribe[40];	
}GCARD_RCD_STRU;//标志牌记录表

typedef struct _tagGScdCardReqStruct
{
	GCARD_RCD_STRU	cardrcdstru;
	int				reqtype;		//0	摘牌,1 挂牌
}GSCDCRADREQ,*PGSCDCRADREQ;
//GSCDCRADREQ	scdcardreq;
//success=sendCMD(GSCDM_ADDCARD,(uint32)&scdcardreq,0);
#define	GSCDM_ADDCARD		( GSCDM_WFBS + 1 )

typedef struct _tagGScdLineReplaceStruct
{
	char username[ GDB_CODE_LEN ];
	char breakname[ GDB_CODE_LEN ]; 
	char replaced_breakname[ GDB_CODE_LEN ];
	char mode;
}GSCDLINEREP,*PGSCDLINEREP;

//GSCDLINEREP scdlinerep;
//success=sendCMD(GSCDM_LINEREPLACE,(uint32)&scdlinerep,0);
#define	GSCDM_LINEREPLACE	( GSCDM_ADDCARD + 1 )
//***********************************增加记录请求**********************************

//***********************************返回请求**************************************
#define	GSCDM_RETREQ		0x400
typedef struct _tagGScdReturnStruct
{
	void	*Result;
	int32	RetLen;
}GSCDRET,*PGSCDRET;

//GSCDRET scdret
//success=sendCMD(GSCDM_RET,(uint32)&scdret,0);
#define	GSCDM_RET			( GSCDM_RETREQ + 1 )
//***********************************返回请求**************************************

//***********************************事故追忆请求**************************************
#define	GSCDM_STARTPDR		0x500

typedef struct _tagGScdPDRStruct
{
	char username[ GDB_CODE_LEN ];
	char breakname[ GDB_CODE_LEN ]; 
	char zygroupno;
}GSCDPDR,*PGSCDPDR;

//***********************************SCADA邮件类型码定义***************************


#define GSCADA_TYPE_NULL			    0
#define GSCADA_TYPE_MANSET			1		//人工置入
#define GSCADA_TYPE_YK				2		//YK
#define GSCADA_TYPE_UPDOWN			3		//升降
#define GSCADA_TYPE_STUDY		 	4		//研究
#define GSCADA_TYPE_YKCHK			5		//YK返校
#define GSCADA_TYPE_YKRESULT		    6		//YK结果
#define GSCADA_TYPE_UPDOWNRESULT     7		//升降结果
#define GSCADA_TYPE_FOREYKCHK		8		//前置机YK返校
#define GSCADA_TYPE_SETSPECIALF   	9		//置标志牌

#define GSCADA_TYPE_LINEREPLACE      10		//旁路替代
#define GSCADA_TYPE_CLRCHGFLAG       11      //遥信对位
#define GSCADA_TYPE_MASKCHGENT		12		//封锁变位事项
#define GSCADA_TYPE_CLRALARMFLAG     13      //清警示标志
#define GSCADA_TYPE_YT				14		//设点控制

#define GSCADA_TYPE_CLRNOTIFYQUE   	15		//清通知事项队列
#define GSCADA_TYPE_WFBS   	        16		//五防闭锁


#define GSCADA_TYPE_PROT             20		//保护事件	
#define GSCADA_TYPE_PROTEVENT        21 		//保护事项报告
#define GSCADA_TYPE_PROTDETECT       22		//保护自检报告

#define GSCADA_TYPE_RAWEVENT	    	30		//生事项
#define GSCADA_TYPE_OLEVENT	    	31		//遥测越限生事项


#define GSCADA_TYPE_STARTPDR         90		 //启动事故追忆

#define	GSCADA_TYPE_FTUFAULTRPT		91	     //FTU故障报告	
#define GSCADA_TYPE_CALLFAULTRPT		92	     //mmi召唤故障录波数据	
#define	GSCADA_TYPE_FOREFAULTRPT		93   	 //前置机发往GSCADA服务器的故障录波数据
#define GSCADA_TYPE_FAULTRCD			94	     //GSCADA服务器返回MMI的故障录波结果 

#define GSCADA_TYPE_SERIALYK			100      // 预定义序列控制
#define GSCADA_TYPE_GROUPYK			101      // 多路群控
#define GSCADA_TYPE_SERIALYKCTRL		102		 // 控制序列 
#define GSCADA_TYPE_GROUPYKCTRL		103		 // 控制多路群控 
#define GSCADA_TYPE_SERIALYKMSG		104		 // 序列控制返回信息 

#define GSCADA_TYPE_CCFORMULA		110		

#define GSCADA_TYPE_NETYCDATA		111		
#define GSCADA_TYPE_NETYXDATA		112		
#define GSCADA_TYPE_NETKWHDATA		113		


#define GSCADA_TYPE_TONGQICMD		150	    //调度员下发的同期命令操作
#define GSCADA_TYPE_TONGQIANS		151     //前置机发往GSCADA服务器的同期应答
#define GSCADA_TYPE_TONGQIRES		152     //GSCADA服务器发给调度员的同期结果应答
#define GSCADA_TYPE_ERRRET			255     //参数返回
//***********************************GSCADA邮件类型码定义***************************


//*******************************系统错误定义*******************************


// interface layer error
#define  GSCADA_ERR_OPENMEMLIB        -200
#define  GSCADA_ERR_OPENTAB           -201
#define  GSCADA_ERR_NOFLD             -202
#define  GSCADA_ERR_DBADDR            -203
#define  GSCADA_ERR_NORCD             -204
#define  GSCADA_ERR_DELRCD            -207
#define  GSCADA_ERR_ADDRCD            -208
#define  GSCADA_ERR_NOTAB             -209
#define  GSCADA_ERR_SQLSELECT         -210
#define  GSCADA_ERR_SQLWHERE          -211
#define  GSCADA_ERR_UPDRCD            -212
#define  GSCADA_ERR_MEMDIC            -213
#define  GSCADA_ERR_MALLOC            -214
#define  GSCADA_ERR_GETRCD            -215

// system layer error

#define  GSCADA_ERR_PARA              -250
#define  GSCADA_ERR_NOOBJECT          -251   // no this object
#define  GSCADA_ERR_SETVALINVALID     -252
#define  GSCADA_ERR_QUEUE             -253
#define  GSCADA_ERR_YKCMD             -254
#define  GSCADA_ERR_NORIGHT           -255
#define  GSCADA_ERR_SAMESTUDY         -256
#define  GSCADA_ERR_STUDENTFULL       -257
#define  GSCADA_ERR_YKCOND            -258
#define  GSCADA_ERR_YKACTION          -259
#define  GSCADA_ERR_YKPROC            -260
#define  GSCADA_ERR_YKOTHEROP         -261
#define  GSCADA_ERR_YKCONFIRM         -262
#define	 GSCADA_ERR_YKQUEUEFULL	     -263
#define	 GSCADA_ERR_OPERATION		 -264
#define  GSCADA_ERR_NOFAULTFTU		 -265   // 没有对应的FTU
#define  GSCADA_ERR_NOFAULTLINE	     -266   // 无故障线路
#define  GSCADA_ERR_NOFAULTRCD		 -267   // FTU没有录波数据
#define  GSCADA_ERR_UPDOWNCOND		 -268
#define	 GSCADA_ERR_YKMONITOR		 -269
#define  GSCADA_ERR_YKMANSET		     -270
#define  GSCADA_ERR_NOYKCONFIRM	     -271	// 不用监督
#define  GSCADA_ERR_LOCKED			 -272	// 闭锁-不能遥控
#define  GSCADA_ERR_NOTSHAREYK		 -273
#define  GSCADA_ERR_GROUNDCOND		 -274	
#define  GSCADA_ERR_GROUNDED		     -275	// 已接地
#define  GSCADA_ERR_DWINVALID		 -276	// 档位值处于非法状态
#define  GSCADA_ERR_WFLOCKED			 -277	// 五防闭锁-不能遥控
//*******************************系统错误定义*******************************

//******************************标志牌记录表********************************

//标志牌类型定义
#define GCARDTYPE_JIANXIU		1		// 检修
#define GCARDTYPE_LINJIAN		2		// 临检
#define GCARDTYPE_XIANDIAN		3		// 限电
#define GCARDTYPE_LAZHA			4		// 拉闸 
#define GCARDTYPE_BAODIAN		5		// 保电
#define GCARDTYPE_SUOZHU		6		// 锁住
#define GCARDTYPE_BCFENGZHA		7		// 保持分闸
#define GCARDTYPE_BCHEZHA		8		// 保持合闸

#define GCARDTYPE_JIEDI			10		// 接地
#define GCARDTYPE_GUAPAI			11		// 挂牌
#define GCARDTYPE_TIAOJIE			12		// 跳接牌

#define GCARDTYPE_GAOYA			50		// 高压危险
#define GCARDTYPE_YOUREN			51		// 有人操作
#define GCARDTYPE_FAULT			52		// 缺陷
#define GCARDTYPE_PROTDEVCHG		53		// 继电保护变更
#define GCARDTYPE_BUSBREAK		54		// 母线拆断
#define GCARDTYPE_KEYUSER		55		// 重要用户
#define GCARDTYPE_WORKENABLE		56		// 工作许可
#define GCARDTYPE_ZQ				57		// 自切

#define GCARDTYPE_DAIDIANZUOYE	60		// 带电作业
#define GCARDTYPE_LINEYOUREN		61		// 线路有人操作
#define GCARDTYPE_DUZHANG		62		// 故障
#define GCARDTYPE_DIDIAOYK		63		// 地调遥控

#define GCARDTYPE_WARNING		65		// 警告牌

#define GCARDTYPE_MAXNO          100

#define  GCARDOBJTYPE_GRAPH			255

//******************************标志牌记录表********************************

/*****************************GSCADA_YK_MACRO DEFINE**************************************/
#define  GSCADA_YK_PRESET            0		// 遥控预置
#define  GSCADA_YK_CHECK_OK          1		// 遥控返校正确
#define  GSCADA_YK_CHECK_ERR         2		// 遥控返校错误
#define	 GSCADA_YK_CHECK_TIMEOUT	    3		// 遥控返校超时
#define  GSCADA_YK_COMFIRM           4		// 遥控确认
#define	 GSCADA_YK_COMFIRM_TIMEOUT	5		// 遥控确认发令超时
#define  GSCADA_YK_EXEC              6		// 遥控执行
#define  GSCADA_YK_EXEC_TIMEOUT	    7		// 遥控执行发令超时
#define  GSCADA_YK_CANCEL            8		// 遥控撤消
#define  GSCADA_YK_SUCCESS           9		// 遥控成功
#define  GSCADA_YK_EXEC_FAIL         10		// 遥控拒动
#define  GSCADA_YK_CMD_ERR		    126		// 遥控命令错误
#define  GSCADA_YK_NULL              127		// 空：过程已终结
/*****************************GSCADA_YK_MACRO DEFINE**************************************/

#endif
