#ifndef  _CMDDEF_H
#define  _CMDDEF_H

#include "dev.h"

#ifdef WIN32
	#if !defined(SCD_DLL)
	#define _SCD_DLLPORT _declspec(dllimport)
	#else
	#define _SCD_DLLPORT _declspec(dllexport)
	#endif
#endif


//类名、对象名、域名长度定义
#define	TABNAME_LEN		    CODE_LEN		//表名长度
#define	FLDNAME_LEN		    CODE_LEN		//域名长度

#define	ENTDESC_LEN		    128		        //事项描述
#define ENTNAME_LEN         CODE_LEN		//事项名长度
#define PROCNAME_LEN        CODE_LEN		//用户过程名长度


#define ERRORINFO_LEN       80		        //错误信息长度	
#define PROTINFO_LEN	    DESC_LEN		//保护信息长度
#define PROT_UNIT_LEN		5		        //保护单位长度

#define HOSTNAME_LEN        HOSTNAMELEN
#define USERNAME_LEN        CODE_LEN

#define MAXSECUREGRADE      32

//#define SCADA_MAX_TERMINALNUMS   128
#define SCADA_MAX_TERMINALNUMS   5120

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


#define STATUS_CHGON          1
#define STATUS_CHGOFF         0

#define STATUS_VAL_ON         0x01
#define STATUS_VAL_OFF        0x00

#define  ACT_TO_VAL(a)   (((a)==STATUS_CHGON) ? STATUS_VAL_ON:STATUS_VAL_OFF)


////////////////////////////////////////////////////////////////
//////////////// 遥控队列及遥控过程、结果定义 //////////////////
////////////////////////////////////////////////////////////////


//遥控模式定义：
#define  YKMODE_WAIT_CHECK		0	//等待返校
#define  YKMODE_AUTO_EXEC	    1	//自动执行


//下发给前置机的遥控动作码：
#define  YKACT_OFF			0x33	// 控分
#define  YKACT_ON			0xCC	// 控合

//遥控类型定义：
#define  YKTYPE_YK            0		// 遥控
#define  YKTYPE_UPDOWN        1		// 升降

//前置机返校信息定义:
#define  YKCHECK_OK           1		// 遥控返校正确
#define  YKCHECK_ERR          0		// 遥控返校错误

//遥控过程码定义：
#define  YK_PRESET            0		// 遥控预置
#define  YK_CHECKKOK          1		// 遥控返校正确
#define  YK_CHECKERR          2		// 遥控返校错误
#define	 YK_CHECKTIMEOUT	  3		// 遥控返校超时
#define  YK_CONFIRM           4		// 遥控确认
#define	 YK_CONFIRMTIMEOUT	  5		// 遥控确认发令超时
#define  YK_EXEC              6		// 遥控执行
#define  YK_EXECTIMEOUT		  7		// 遥控执行发令超时
#define  YK_CANCEL            8		// 遥控撤消
#define  YK_SUCCESS           9		// 遥控成功
#define  YK_REFUSE            10	// 拒动
#define  YK_NULL              127	// 空：过程已终结

inline int  YkSerClassify(int yksercode)
{
switch(yksercode) {
	case YK_CHECKKOK:
	case YK_PRESET:
	case YK_EXEC:
	case YK_CONFIRM:
		return(0);
	case YK_SUCCESS:
		return(1);
	default :
		return(-1);
 }
}


//群遥控下发方式定义:
#define	   GRPYKMODE_ONEBYONE	  0
#define	   GRPYKMODE_ALLATONCE	  1

//遥控、升降、下发定值、下发定值区号等所有须遵循下述过程的命令码定义：
#define  YKCMD_NULL			      0   //无效
#define  YKCMD_PRESET		      1	  //预置
#define  YKCMD_EXEC               2   //执行
#define  YKCMD_CANCEL		      3   //撤消
#define  YKCMD_CONFIRM            4   //确认

//遥控、升降的方式定义：
#define  YKFANGSHI_STANDARD	      0	//标准
#define  YKFANGSHI_UPDOWNCMD      1	//用升降令


//调度员界面发给SCADA服务器的遥控动作码定义：
#define  ACTTYPE_OFF              0   //控分
#define  ACTTYPE_ON               1   //控合

//调度员界面发给SCADA服务器的遥调动作码定义：
#define  ACTTYPE_DOWN             0   //降
#define  ACTTYPE_UP               1   //升
#define  ACTTYPE_PAUSE            2   //急停


#define	 YK_COND_FAILRETURN	      0
#define	 YK_COND_FAILCONTINUE     1

#define  SERIALYK_CANCEL          0  // 撤消序列
#define  SERIALYK_PAUSE           1  // 暂停序列
#define  SERIALYK_CONTINUE        2  // 继续序列

//遥控扩展标志
#define	 EXEC_YK_NOTCHECKSTATUS	  1	//不检查状态

// 遥调过程
#define  YT_SENDCMD               0		// 遥调发令
#define  YT_SUCCESS               1		// 遥调成功
#define  YT_FAIL                  2		// 遥调失败
#define  YT_NULL                  127	// 遥调过程结束


///////////////////////////////////////////////////////////////
//////////////////// 定时存盘方式定义  只存瞬时值/////////////////////////////
///////////////////////////////////////////////////////////////////
/*
#define  TIMER_NOSAVE			   0
#define	 TIMER_SAVE_REAL		   1
#define	 TIMER_SAVE_AVERAGE		   2
*/
/////////////////////////////////////////////////////////////////////
///////////////////// 事故判断方式定义 //////////////////////////////
/////////////////////////////////////////////////////////////////////

#define  ACCIDENT_BY_SGZ             0	 // 事故总
#define  ACCIDENT_BY_SGZANDPROT      1	 // 事故总与保护
#define  ACCIDENT_BY_SGZORPROT       2	 // 事故总或保护
#define  ACCIDENT_BY_SGZANDYC        3	 // 事故总与遥测归零
#define  ACCIDENT_BY_LOGPROC		 4   // 逻辑过程
#define  ACCIDENT_BY_SGZANDSWITCH	 5   // 事故总与刀闸
#define  ACCIDENT_BY__NULL           127 // 不判事故

////////////////////////////////////////////////////////////////////
//////////////////// 遥信量处理方式 ////////////////////////////
////////////////////////////////////////////////////////////////////
//异常信号不处理
/*
#define  ODDCHG_NOPROC			0	//　异常不处理 
#define  ODDCHG_OFFALARM		1	//　变分处理
#define  ODDCHG_ONALARM			2	//	变合处理
*/
#define  CHGENTTYPE_NORMALCHG   1	//　正常变位
#define  CHGENTTYPE_ODDCHG      2	//	异常变位
#define  CHGENTTYPE_SOE         3	//	SOE

#define  ADDENTTYPE_NORMAL      0		// 正常
#define  ADDENTTYPE_ODD         1		// 异常
#define  ADDENTTYPE_SOE         2		// SOE
#define  ADDENTTYPE_SG          3		// 事故
#define	 ADDENTTYPE_BREAKOFF    4		// 开关分（虚事项）


//////////////////////////////////////////////////////////////////
///////////////////  其他定义  ///////////////////////////////////
//////////////////////////////////////////////////////////////////

//遥测越限类型定义：
#define  ANAOL_NORMAL           0		//正常
#define  ANAOL_HIGH             1		//越上限
#define  ANAOL_LOW              2		//越下限
#define  ANAOL_HIHI             3		//越上上限
#define  ANAOL_LOLO             4		//越下下限
/*
#define  ANACAL_COSYC		   1	    // 计算功率因数
#define  ANACAL_SPQ			   2	    // 通过P,Q计算视在功率
#define  ANACAL_SIV			   3	    // 通过I,V计算视在功率
*/

#define  MANSET_REPLACE_END    0		// 人工替代结束
#define  MANSET_REPLACE_BEGIN  1		// 人工替代开始
#define  AUTO_REPLACE_BEGIN    2		// 自动替代开始
#define  AUTO_REPLACE_ADD      3		// 自动替代叠加
#define  AUTO_REPLACE_END      4		// 自动替代结束

#define  IS_BYREPLACE_ADD(a)	(0)  //((a)==AUTO_REPLACE_ADD)
#define  IS_BYREPLACE_EQU(a)	((a)==AUTO_REPLACE_BEGIN || (a)==MANSET_REPLACE_BEGIN)

#define	 REPLACETYPE_BYPASS	   1   // 旁路替代
#define  REPLACETYPE_PAIR      2   // 对端替代

#define  TERMINALSTATUS_STOP        0     // 终端停止 (主备通道均停止)
#define  TERMINALSTATUS_MAINCHAN    1     // 终端运行从主通道收数
#define  TERMINALSTATUS_BACKCHAN    2     // 终端运行从备通道收数


#define  KWHSAMPLING_16BIT     0
#define  KWHSAMPLING_19BIT     1
#define  KWHSAMPLING_20BIT     2
#define  KWHSAMPLING_24BIT     3
#define  KWHSAMPLING_32BIT     4
#define  KWHSAMPLING_DEF	   5
#define  KWHSAMPLING_TABCODE   10
#define  KWHSAMPLING_HOURCLN   11
#define  KWHSAMPLING_SAVEINC   12    //直接存储间隔内电量


#define  CONFIRMMODE_TIMER	   0
#define  CONFIRMMODE_MANSET	   1


#define  ALARM_TYPE_YCINCONSIST   1
#define  ALARM_TYPE_YXINCONSIST   2
#define  ALARM_TYPE_DEADDATA	  3




//////////////////////////////////////////////////////////////////////
//////////////////////////实时库表名定义//////////////////////////////
//////////////////////////////////////////////////////////////////////
#define  PROGREG_TABNAME          "progreg"        //程序注册表
#define  VOLTGRADE_TABNAME		  "voltgrade"      //电压等级表
#define  VOLTLIMIT_TABNAME		  "voltlimit"      //电压限值表

#define	 BREAKPROT_TABNAME		  "breakprot"	   //开关保护表
#define	 BREAKPROTMDL_TABNAME	  "breakprotmdl"   //开关保护模块表	
#define	 BREAKREPLACE_TABNAME	  "breakreplace"   //开关替代表	
#define	 BREAKLINKDEV_TABNAME	  "breaklinkdevice"//开关连接设备表	
#define	 BREAKLINKSWITCH_TABNAME  "breaklinkswitch"//开关连接刀闸表

#define  ANALOG_TABNAME           "analog"         //模拟量表
#define  TRENDDB_TABNAME          "trenddbpara"    //时序数据库表
#define  BREAK_TABNAME			  "break"          //开关量表
#define  SWITCH_TABNAME           "switch"         //刀闸量表
#define  PROT_TABNAME		      "prot"           //保护量表
#define  STATUS_TABNAME           "status"         //状态量表
#define  DMSSTATUS_TABNAME        "dmsdiagnose"    //配电故障检测状态表

#define  MULTISTATUS_TABNAME	  "multistatus"    //多态量表
#define  KWH_TABNAME			  "kwh"            //电度量表
#define  CALCULATE_TABNAME		  "calculate"      //计算量表
#define	 YXNOSORT_TABNAME		  "yxnosort"       //遥信号排序表
#define	 SUBYXNOSORT_TABNAME	  "subyxnosort"    //次遥信号排序表
#define	 YCNOSORT_TABNAME		  "ycnosort"       //遥测号排序表
#define	 KWHNOSORT_TABNAME		  "kwhnosort"       //电度号排序表
#define	 CHANNELNOSORT_TABNAME		  "channelnosort"       //通道号排序表
#define  STATION_TABNAME		  "station"        //厂站表
#define  CHANNEL_TABNAME		  "channel"        //通道表
#define  TERMINAL_TABNAME		  "terminal"       //终端表
#define  APEVENT_TABNAME          "apevent"        //应用程序事件表
#define  YKQUE_TABNAME            "ykqueue"        //遥控队列表
#define  YKCOND_TABNAME           "ykcondition"    //状态控制条件表
#define  YKPARA_TABNAME           "ykpara"         //遥控参数表
#define  YTPARA_TABNAME           "ytpara"         //设点控制参数表
#define  UPDOWN_TABNAME           "updownpara"     //升降参数表
#define  USERPROC_TABNAME         "userproc"       //用户过程表
#define  USEROPMODE_TABNAME       "userright"      //节点权限表
#define  DISPATCHER_TABNAME       "dispatcher"     //操作配置表
#define  HOSTSTATUS_TABNAME		  "hoststatus"     //主机状态表


#define  SDPARA_TABNAME           "sdpara"         //时段参数表
#define  SDQUERY_TABNAME          "sdquery"        //时段查询表
#define  DAYSDPARA_TABNAME        "daysdpara"      //日时段参数表
#define  DAYSDQUERY_TABNAME       "daysdquery"     //日时段查询表
#define  PROGCTRL_TABNAME         "progctrl"       //应用程序控制表
#define  STATUSMESG_TABNAME       "statusmesg"     //状态信息表

#ifdef GIS
#define  CARDRCD_TABNAME		  "giscardrcd"     //GIS标志牌记录表
#else
#define  CARDRCD_TABNAME		  "cardrcd"        //标志牌记录表
#endif

#define  NOTIFYEVTTYPE_TABNAME    "notifyevttype"  //事项确认类型表
#define  NOTIFYEVTQUERCD_TABNAME  "notifyevtrcd"   //待确认事项记录表
#define  WFBSRCD_TABNAME          "wfbspara"       //五防闭锁参数表

#define  YCMULTISRC_TABNAME		  "ycmultisource"  //遥测多源表
//#define  DMSFAULTTYPE_TABNAME	  "dmsflttype"     //配电故障检测状态表

//事故追忆
#define  PDRGRP_TABNAME           "pdrgroup"       //追忆组表
#define  PDRPARA_TABNAME          "pdrpara"        //追忆点表
#define  PDRDESC_TABNAME          "pdrdescribe"
#define  PDRDATA_TABNAME          "pdrdata"   
#define  DEVSTAT_TABNAME          "设备运行统计表"

#define  ENTEXP_TABNAME           "eventexpress"      //事项句表
#define  SCDEVENT_TABNAME         "eventqueue"        //实时事项队列
#define  EVENTTYPE_TABNAME        "eventtype"         //事项类型表
#define  EVENTSTATUSDESC_TABNAME  "eventstatusdesc"   //事项状态描述表
//#define  EVENTMODE_TABNAME      "eventmode"         //事项模式表
#define  EVENTDEALMODE_TABNAME    "eventdealmode"     //事项处理方式参数表

#define  SERIALYKGRP_TABNAME      "serialykgrp"       //序列控制组表
#define  SERIALYKOBJ_TABNAME	  "serialykobj"       //序列控制对象表
#define  SCDDEVICE_TABNAME		  "scddevice"		  //SCD设备参数表 2004 05 add
#define  SCDYCTIMELIMIT_TABNAME	  "yctimelimit"		  //遥测时段限值表

#define  SCDTIMETYPE_HTABNAME     "时段参数表"
#define  YKACT_MSGNAME            "遥控动作"
#define  YKPROC_MSGNAME           "遥控过程"
#define  UPDOWNACT_MSGNAME        "升降动作"
#define  UPDOWNPROC_MSGNAME       "升降过程"
#define  HOSTSTATUS_MSGNAME       "主机状态"
#define  NETSTATUS_MSGNAME        "网络状态"
#define  CHANSTATUS_MSGNAME       "通道状态"


//////////////////////////////////////////////////////////////////////
/////////////////////////实时库域名定义///////////////////////////////
//////////////////////////////////////////////////////////////////////
#define  ANAATTR_PLNAME           "bypassname"
#define  ANAATTR_PLF              "bypassf"
#define  BRKATTR_PLF			  "bypassf"
#define  UPDOWNTIMES			  "updowntimes"
#define  YKTIMES			 	  "yktimes"

#define  SWATTR_LINENAME          "linename"

#define  COMNATTR_NAME			  "name"
#define  COMNATTR_SUBSTNNAME	  "substation"
#define  COMNATTR_LINENAME		  "devname"
#define  COMNATTR_VALUE			  "value"
#define  COMNATTR_YKTAKEF		  "yktakef"
#define  COMNATTR_DESCRIBE		  "describe"
#define  COMNATTR_SECUREGRADE	  "securegrade"
#define  COMNATTR_VOLTGRADE		  "voltgrade"

#define  YKATTR_NAME              "name"
#define  UPDOWNATTR_NAME          "name"

#define  SCDPROC_USERNAME         "scadaproc"



///////////////////////////////////////////////////////////////////////
///////////////////  SCADA邮件类型码定义 ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

#define SCADA_TYPE_NULL			    0
#define SCADA_TYPE_MANSET			1		//人工置入
#define SCADA_TYPE_YK				2		//YK
#define SCADA_TYPE_UPDOWN			3		//升降
#define SCADA_TYPE_STUDY		 	4		//研究
#define SCADA_TYPE_YKCHK			5		//YK返校
#define SCADA_TYPE_YKRESULT		    6		//YK结果
#define SCADA_TYPE_UPDOWNRESULT     7		//升降结果
#define SCADA_TYPE_FOREYKCHK		8		//前置机YK返校
#define SCADA_TYPE_SETSPECIALF   	9		//置标志牌

#define SCADA_TYPE_LINEREPLACE      10		//旁路替代
#define SCADA_TYPE_CLRCHGFLAG       11      //遥信对位
#define SCADA_TYPE_MASKCHGENT		12		//封锁变位事项
#define SCADA_TYPE_CLRALARMFLAG     13      //清警示标志
#define SCADA_TYPE_YT				14		//设点控制

#define SCADA_TYPE_CLRNOTIFYQUE   	15		//清通知事项队列
#define SCADA_TYPE_WFBS   	        16		//五防闭锁


#define SCADA_TYPE_PROT             20		//保护事件	
#define SCADA_TYPE_PROTEVENT        21 		//保护事项报告
#define SCADA_TYPE_PROTDETECT       22		//保护自检报告

#define SCADA_TYPE_RAWEVENT	    	30		//生事项
#define SCADA_TYPE_OLEVENT	    	31		//遥测越限生事项


#define SCADA_TYPE_STARTPDR         90		 //启动事故追忆

#define	SCADA_TYPE_FTUFAULTRPT		91	     //FTU故障报告	
#define SCADA_TYPE_CALLFAULTRPT		92	     //mmi召唤故障录波数据	
#define	SCADA_TYPE_FOREFAULTRPT		93   	 //前置机发往SCADA服务器的故障录波数据
#define SCADA_TYPE_FAULTRCD			94	     //SCADA服务器返回MMI的故障录波结果 

#define SCADA_TYPE_SERIALYK			100      // 预定义序列控制
#define SCADA_TYPE_GROUPYK			101      // 多路群控
#define SCADA_TYPE_SERIALYKCTRL		102		 // 控制序列 
#define SCADA_TYPE_GROUPYKCTRL		103		 // 控制多路群控 
#define SCADA_TYPE_SERIALYKMSG		104		 // 序列控制返回信息 

#define SCADA_TYPE_CCFORMULA		110		

#define SCADA_TYPE_NETYCDATA		111		
#define SCADA_TYPE_NETYXDATA		112		
#define SCADA_TYPE_NETKWHDATA		113	


#define SCADA_TYPE_TONGQICMD		150	    //调度员下发的同期命令操作
#define SCADA_TYPE_TONGQIANS		151     //前置机发往SCADA服务器的同期应答
#define SCADA_TYPE_TONGQIRES		152     //SCADA服务器发给调度员的同期结果应答
#define SCADA_TYPE_ERRRET			255     //参数返回

#define MAXSTRINGVALLEN				256
#ifdef QTGRAPH
#define MAXAPENTMSG_LEN				256*8  
#else
#define MAXAPENTMSG_LEN				256*2  
#endif

///////////////////////////////////////////////////////////////////
///////////  应用事件类型定义及其它  //////////////////////////////
///////////////////////////////////////////////////////////////////

#define  APEVENT_NULL               0	    // 无事件
#define  APEVENT_NOEVENT            1	    // 无事件（主要用于设置空闲时处理函数）
#define  APEVENT_ALL                2	    // 全部事件

#define  APEVENT_OVERLIMIT          20		// 越限
#define  APEVENT_MSTATUSCHG         21		// 多态量变化
#define  APEVENT_STACHG             22		// 状态量变化	
#define  APEVENT_STASOE             23		// 状态量SOE
#define  APEVENT_YK                 24		// 遥控
#define  APEVENT_UPDOWN             25		// 升降
#define  APEVENT_YKCMD              26		// 遥控命令
#define  APEVENT_BREAK              27		// 开关	
#define  APEVENT_DBUPD              28		// 数据库域修改
#define  APEVENT_REQLOADTAB         29		// 请求装载数据库表
#define  APEVENT_LOADTABOK          30		// 装载数据库表成功
#define  APEVENT_MANSET             31		// 人工置数
#define	 APEVENT_SETSPECIALF	    32		// 置特殊标志
#define  APEVENT_FLDCHG             33		// 数据库域变化
#define  APEVENT_FORECHANNELENT     34		// 前置机通道事件
#define	 APEVENT_PROT       	    35		// 保护事件
#define  APEVENT_REPLACE		    36		// 旁路替代
#define  APEVENT_DASDIAGNOSE	    37		// 配电诊断事件
#define	 APEVENT_STARTPDR		    38		// 启动追忆
#define	 APEVENT_FTUFAULT           39		// FTU故障
#define	 APEVENT_CCFORMULA	        40	    // 编译计算表达式表

#define  APEVENT_PROT_DETECT        41      // 保护自检
#define  APEVENT_PROT_TROUBLE       42      // 保护故障
#define  APEVENT_YT			        43      // YT

#define  APEVENT_WFBS               44		// 五防

//保护规约类型定义
#define PROTTYPE_LFP                0       //南瑞LFP保护
#define PROTTYPE_ISA                1       //南瑞ISA保护
#define PROTTYPE_SEL                2       //SEL保护
#define PROTTYPE_DFP                3       //DFP保护
#define PROTTYPE_LSA_P              4       //LSA_P保护
#define PROTTYPE_SBC                5       //SBC保护
#define PROTTYPE_DF3003             6       //东方DF3003保护
#define PROTTYPE_FWB		     	7       //南自FWB保护
#define PROTTYPE_CAN2000	    	8       //CAN2000保护
#define	PROTTYPE_TIGER			    9	    //Tiger保护
#define	PROTTYPE_XJS99			    10      //许继保护
#define PROTTYPE_WBX			    11	    //WBX保护
#define PROTTYPE_DFP500             12      //DFP500保护
#define PROTTYPE_DSA_301		    13	    //LSA_P1.0 规约扩展
#define PROTTYPE_ISA300			    14	    //ISA v3.0 南瑞城乡所(025)3429900-2562
#define PROTTYPE_CSC2000		    15	    //四方保护
#define PROTTYPE_HUANENG		    16	    //华能保护 0731-8906888-8312
#define PROTTYPE_ZH				    18      //ZH保护	  
#define PROTTYPE_FBZ			    19		//FBZ保护 --tel:13505171372 刘
#define PROTTYPE_DWK			    20		//DWK保护
#define PROTTYPE_LFP3			    21		//南瑞LFP3.0保护
#define PROTTYPE_KGNSA			    22		//电科院保护朝阳用
#define PROTTYPE_HNSD			    23		//河南思达保护
#define PROTTYPE_XAKY			    24		//西安开元
#define PROTTYPE_PA100			    25		//南京因泰莱
#define PROTTYPE_DFP_301		    26		//南瑞DFP_301保护
#define PROTTYPE_ZDB			    27		//ZDB
#define PROTTYPE_DF3003_NEW         28      //新东方DF3003保护
#define PROTTYPE_RCS		        29      //南瑞RCS保护
#define PROTTYPE_PS					30      //南自PS6000保护
#define PROTTYPE_DEP				31		//申瑞DEP保护
#define PROTTYPE_XWJK				32		//许继保护
#define PROTTYPE_NEWZDB				33		//大连中电新保护
#define	PROTTYPE_NSR				34		//南瑞NSR
#define	PROTTYPE_NEPCAN				35		//南自CAN
#define	PROTTYPE_IEC103				36		//标准103
#define	PROTTYPE_SACDW				37		//南自电网
#define	PROTTYPE_KT90				38		//KT90
#define	PROTTYPE_QHZG				39		//清华紫光
#define	PROTTYPE_SACJD				40		//南自机电
#define	PROTTYPE_FORTYONE			41		//41号保护规约
#define	PROTTYPE_FORTYTWO			42		//42号保护规约
#define	PROTTYPE_FORTYTHREE			43		//43号保护规约
#define	PROTTYPE_FORTYFOUR			44		//44号保护规约
#define	PROTTYPE_FORTYFIVE			45		//45号保护规约
#define	PROTTYPE_FORTYSIX			46		//46号保护规约
#define	PROTTYPE_FORTYSEVEN			47		//47号保护规约
#define	PROTTYPE_FORTYEIGHT			48		//48号保护规约
#define	PROTTYPE_FORTYNINE			49		//49号保护规约
#define	PROTTYPE_FIFTYZERO			50		//50号保护规约
#define	PROTTYPE_FIFTYONE			51		//51号保护规约
#define	PROTTYPE_FIFTYTWO			52		//52号保护规约
#define	PROTTYPE_FIFTYTHREE			53		//53号保护规约
#define	PROTTYPE_FIFTYFOUR			54		//54号保护规约
#define	PROTTYPE_FIFTYFIVE			55		//55号保护规约
#define	PROTTYPE_FIFTYSIX			56		//56号保护规约
#define	PROTTYPE_FIFTYSEVEN			57		//57号保护规约
#define	PROTTYPE_FIFTYEIGHT			58		//58号保护规约
#define	PROTTYPE_FIFTYNINE			59		//59号保护规约
#define	PROTTYPE_SIXTYZERO			60		//60号保护规约

/////////////////////////////////////////////////////////////////////
//////////////////  系统错误定义 ////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#ifndef  OK
#define  OK      0
#endif

// interface layer error
#define  SCADA_ERR_OPENMEMLIB        -200
#define  SCADA_ERR_OPENTAB           -201
#define  SCADA_ERR_NOFLD             -202
#define  SCADA_ERR_DBADDR            -203
#define  SCADA_ERR_NORCD             -204
#define  SCADA_ERR_DELRCD            -207
#define  SCADA_ERR_ADDRCD            -208
#define  SCADA_ERR_NOTAB             -209
#define  SCADA_ERR_SQLSELECT         -210
#define  SCADA_ERR_SQLWHERE          -211
#define  SCADA_ERR_UPDRCD            -212
#define  SCADA_ERR_MEMDIC            -213
#define  SCADA_ERR_MALLOC            -214
#define  SCADA_ERR_GETRCD            -215

// system layer error

#define  SCADA_ERR_PARA              -250
#define  SCADA_ERR_NOOBJECT          -251   // no this object
#define  SCADA_ERR_SETVALINVALID     -252
#define  SCADA_ERR_QUEUE             -253
#define  SCADA_ERR_YKCMD             -254
#define  SCADA_ERR_NORIGHT           -255
#define  SCADA_ERR_SAMESTUDY         -256
#define  SCADA_ERR_STUDENTFULL       -257
#define  SCADA_ERR_YKCOND            -258
#define  SCADA_ERR_YKACTION          -259
#define  SCADA_ERR_YKPROC            -260
#define  SCADA_ERR_YKOTHEROP         -261
#define  SCADA_ERR_YKCONFIRM         -262
#define	 SCADA_ERR_YKQUEUEFULL	     -263
#define	 SCADA_ERR_OPERATION		 -264
#define  SCADA_ERR_NOFAULTFTU		 -265   // 没有对应的FTU
#define  SCADA_ERR_NOFAULTLINE	     -266   // 无故障线路
#define  SCADA_ERR_NOFAULTRCD		 -267   // FTU没有录波数据
#define  SCADA_ERR_UPDOWNCOND		 -268
#define	 SCADA_ERR_YKMONITOR		 -269
#define  SCADA_ERR_YKMANSET		     -270
#define  SCADA_ERR_NOYKCONFIRM	     -271	// 不用监督
#define  SCADA_ERR_LOCKED			 -272	// 闭锁-不能遥控
#define  SCADA_ERR_NOTSHAREYK		 -273
#define  SCADA_ERR_GROUNDCOND		 -274	
#define  SCADA_ERR_GROUNDED		     -275	// 已接地
#define  SCADA_ERR_DWINVALID		 -276	// 档位值处于非法状态
#define  SCADA_ERR_WFLOCKED			 -277	// 五防闭锁-不能遥控


/////////////////////////////////////////////////////////////////////
////////////////////// 标志牌记录表 /////////////////////////////////
/////////////////////////////////////////////////////////////////////

//标志牌类型定义
#define CARDTYPE_JIANXIU		1		// 检修
#define CARDTYPE_LINJIAN		2		// 临检
#define CARDTYPE_XIANDIAN		3		// 限电
#define CARDTYPE_LAZHA			4		// 拉闸 
#define CARDTYPE_BAODIAN		5		// 保电
#define CARDTYPE_LOCK		    6		// 锁住
#define CARDTYPE_KEEPOFF	    7		// 保持分闸
#define CARDTYPE_KEEPON		    8		// 保持合闸

#define CARDTYPE_JIEDI			10		// 接地
#define CARDTYPE_GUAPAI			11		// 挂牌
#define CARDTYPE_JUMPER			12		// 跳接


#define CARDTYPE_GAOYA			50		// 高压危险
#define CARDTYPE_YOUREN			51		// 有人操作
#define CARDTYPE_FAULT			52		// 缺陷
#define CARDTYPE_PROTDEVCHG		53		// 继电保护变更
#define CARDTYPE_BUSBREAK		54		// 母线拆断
#define CARDTYPE_KEYUSER		55		// 重要用户
#define CARDTYPE_WORKENABLE		56		// 工作许可
#define CARDTYPE_ZQ				57		// 自切

#define CARDTYPE_DAIDIANZUOYE	60		// 带电作业
#define CARDTYPE_LINEYOUREN		61		// 线路有人操作
#define CARDTYPE_DUZHANG		62		// 故障
#define CARDTYPE_DIDIAOYK		63		// 地调遥控
#define CARDTYPE_WARNING		65		// 警告

#define CARDTYPE_MAXNO          100

#define IS_LINE_JIANXIU(a)     ((a)==CARDTYPE_JIANXIU || (a)==CARDTYPE_LINJIAN)

//对象类型定义

#define  CARDOBJTYPE_GRAPH			255

//////////////////////////

typedef struct {
	short  groupno;
	short  firststate;
	short  secondstate;
	short  objtype;
	char   objname[24];
	short  cardtype;
	short  cardno;
#ifdef GIS
	double startx;
	double starty;
	double width;
	double height;
#else
	int    startx;
	int    starty;
#ifndef QTGRAPH
	int    width;
	int    height;
#endif
#endif
#ifndef QTGRAPH
	short  imagetype;
	char   imagename[40];
#endif
#ifdef QTGRAPH
	int	metaid;
#endif
	char   hostname[24];
	char   username[24];
	char   cardtime[80];
#ifndef QTGRAPH
	char   bitmap[24];
	char   userdefgr[24];
#endif
#ifdef QTGRAPH
	char   graphpath[255];
	char   metapath[255];
#endif
	char   context[255];
	char	  devdescribe[40];
} CARD_RCD_STRU;//标志牌记录表



typedef struct {
	short  groupno;
	short  objtype;
	char   objname[24];
	short  cardtype;
	short  cardno;
} CARD_RCDKEY_STRU;


//事项确认
typedef struct {
	short  eventsortid;
	char   eventsortdesc[24];
	short  eventtypeid;
	char   eventname[24];
} NOTIFY_EVTTYPE_STRU;//事项确认类型表

typedef struct {
	short  eventtypeid;
} NOTIFYEVTTYPE_RCDKEY_STRU;


typedef struct {
	uchar  groupno;
	short  sort;
	short  type;
	int    ymd;
	int    hmsms;
} NOTIFYEVTQUE_RCDKEY_STRU;

//////////////////////////////////////////////////////////////////////////////
///////////////////////   对浮点数合法性判断    //////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <float.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=_fpclass((double)value);
	if(ret==_FPCLASS_NN || ret==_FPCLASS_PN ||ret==_FPCLASS_NZ || ret==_FPCLASS_PZ)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=_fpclass(value);
	if(ret==_FPCLASS_NN || ret==_FPCLASS_PN ||ret==_FPCLASS_NZ || ret==_FPCLASS_PZ)
		return 1;
	return 0;
}
#endif

#ifdef __unix
#ifdef __sun
#include <ieeefp.h>
inline int IS_VALID_FLOAT(float value)
{
	fpclass_t ret=fpclass((double)value);
	if(ret==FP_NZERO || ret==FP_PZERO ||ret==FP_NNORM || ret==FP_PNORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	fpclass_t ret=fpclass(value);
	if(ret==FP_NZERO || ret==FP_PZERO ||ret==FP_NNORM || ret==FP_PNORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __unix
#ifdef __hpux
#include <math.h>
inline int IS_VALID_FLOAT(float value)
{
        int ret=fpclassify(value);
        if(ret==FP_NORMAL || ret==FP_ZERO)
                return 1;
        return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
        int  ret=fpclassify(value);
        if(ret==FP_NORMAL || ret==FP_ZERO)
                return 1;
        return 0;
}
#endif
#endif

#ifdef __unix
#ifdef __alpha
#include <math.h>
#include <fp_class.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fp_classf(value);
	if(ret==FP_POS_ZERO || ret==FP_NEG_ZERO ||ret==FP_NEG_NORM || ret==FP_POS_NORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=fp_class(value);
	if(ret==FP_POS_ZERO || ret==FP_NEG_ZERO ||ret==FP_NEG_NORM || ret==FP_POS_NORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __unix
#ifdef _AIX
#include <math.h>
#include <float.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fpclassify(value);
	if(ret==FP_PLUS_ZERO || ret==FP_MINUS_ZERO ||ret==FP_PLUS_NORM || ret==FP_MINUS_NORM)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int ret=fpclassify(value);
	if(ret==FP_PLUS_ZERO || ret==FP_MINUS_ZERO ||ret==FP_PLUS_NORM || ret==FP_MINUS_NORM)
		return 1;
	return 0;
}
#endif
#endif

#ifdef __linux
#include <math.h>
inline int IS_VALID_FLOAT(float value)
{
	int ret=fpclassify(value);
	if(ret==FP_NORMAL || ret==FP_ZERO)
		return 1;
	return 0;
}
inline int IS_VALID_DOUBLE(double value)
{
	int  ret=fpclassify(value);
	if(ret==FP_NORMAL || ret==FP_ZERO)
		return 1;
	return 0;
}
#endif


////////////////////////////常用函数定义begin////////////////////////////
#define  ishanzhi(a)  (((a) & 0x80) == 0x80)

#ifndef __hpux
#ifndef   MAX
#define   MAX(a,b) (((a)>(b)) ? (a):(b))
#endif

#ifndef   MIN
#define   MIN(a,b) (((a)<(b)) ? (a):(b))
#endif
#endif


#ifndef   ABS
#define   ABS(a)      (((a)<0) ? (-(a)):(a))
#define   ABSMAX(a,b) (((ABS(a))>(ABS(b))) ? (a):(b))
#define   ABSMIN(a,b) (((ABS(a))<(ABS(b))) ? (a):(b))
#endif

# define ISZERO(a)    ((a<0.000001) && (a>-0.000001))

# define FVAL5(a) (((a)-(int)(a))>=0.5) ? ((int)(a)+1):((int)(a))
////////////////////////////常用函数定义end////////////////////////////

#endif
