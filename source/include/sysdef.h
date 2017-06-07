#ifndef	SYS_DEF_H
#define	SYS_DEF_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if defined(WIN32)
	#pragma warning(disable: 4251)
	#pragma warning(disable: 4275)

	#include <winsock.h>
	#include <pdh.h>

	#define		EWOULDBLOCK			WSAEWOULDBLOCK
	#define		EISCONN				WSAEISCONN
	#define		EINPROGRESS			WSAEINPROGRESS
	#define		EALREADY			WSAEALREADY
#elif defined(__unix)
	#include	<netinet/in.h>
	#include	<unistd.h>
	#include	<pthread.h>
	#include	<string.h>
	#include	<sys/time.h>
	#include	<sys/types.h>
	#include	<sys/socket.h>
	#include	<arpa/inet.h>
	#include	<fcntl.h>
	#include	<stdarg.h>
	#include	<errno.h>
	#include	<stdio.h>
	#ifndef		INADDR_NONE
		#define		INADDR_NONE			-1
	#endif
	#if !defined(WSAEINTR)
		#define		WSAEINTR				EINTR
	#endif
	#define  SOCKET					int
	#define  SOCKET_ERROR			-1
#if (defined(__sun) || defined(__hpux))
		#include <sys/errno.h>
	#endif
#endif

#include	<stdlib.h>

#ifdef WIN32
#ifndef strcasecmp
#define strcasecmp stricmp
#endif
#endif

/*-------------------------------------------------------------------------*/

#if !defined(TRUE)
#define	TRUE					1
#endif

#if !defined(FALSE)
#define	FALSE					0
#endif

/*-------------------------------------------------------------------------*/

#if defined(WIN32)
#define	STDCALL					__stdcall
#define WaitForRest(n)			Sleep(n*1000)
#if defined(EXPORTDLL)
#define	IMEXPORT				__declspec(dllexport) 
#else
#define	IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	WaitForRest(n)			sleep(n)
#define	IMEXPORT
#define	STDCALL
#endif

//工作站系统使用的消息定义
#if defined(__unix)
#define		TIMEMSGID			1000
#define		SyncTimeMsg			1
#endif

/*-------------------------------------------------------------------------*/

#define	NBENV					"NBENV"
#define	SYS_USER				"superadmin"		/*系统用户名*/
#define	SYS_PWD					"superpwd"		/*系统用户密码*/

#define	TERM_SERVER				"TERM"			/*通讯终端服务器*/
#define	FEP_SERVER				"FEP"			/*前置机 front end processor*/
#define	ZF_SERVER				"ZF"			/*转发服务器*/
#define	MNP_SERVER				"MNP"			/*模拟屏服务器*/
#define	SCD_SERVER				"SCADA"			/*SCADA服务器*/
#define	EMS_SERVER				"EMS"			/*EMS服务器*/
#define DAS_SERVER				"DAS"			/*DAS服务器*/
#define	WEB_SERVER				"WEB"			/*EMS服务器*/
#define	HIS_SERVER				"DB"			/*历史数据库服务器*/
#define	PRINT_SERVER			"PRINT"			/*打印服务器*/
#define	FILE_SERVER				"FILE"			/*文件服务器*/
/*-------------------------------------------------------------------------*/

#define CODE_LEN				24
#define DESC_LEN				40
#define GRAP_LEN				40
#define	PASSWORD_LEN			24
#define	USER_NAME_LEN			CODE_LEN
#define	RTDB_NAME_LEN			CODE_LEN
#define HOSTNAMELEN				CODE_LEN
#define SHORTNAME_LEN			CODE_LEN
#define FULLDESCRIBE_LEN		128
#define REASONINFO_LEN			64

/*-------------------------------------------------------------------------*/

#ifndef EVENTDESCRIBE_LEN
#define EVENTDESCRIBE_LEN		40
#endif

/*
//输出级定义
#define		PrintOut			output.bit0
#define		DiskOut				output.bit1
#define		ToneOut				output.bit2
#define		GraphOut			output.bit3
#define		DispOut				output.bit4
*/

#ifndef BYTE_SWAP
#define BYTE_SWAP(sn,on) {	sp=src[sn];	src[sn]=src[on];src[on]=sp;}
#endif

/*-------------------------------------------------------------------------*/

//事件分类号
enum EventClass
{
	ENTCLASS_NULL = 0,						//0 空事项类
	ENTCLASS_DEVICE,						//1 系统中的设备及程序运行事件
	ENTCLASS_SCADA,							//2 SCADA事件
	ENTCLASS_PROTECT,						//3 综合自动化保护事件
	ENTCLASS_DAS,							//4 DAS应用事件
	ENTCLASS_DA,							//5 DA应用事件
	ENTCLASS_DA_STUDY,						//6 DA研究态应用事件
	ENTCLASS_EMS,							//7 EMS应用事件
	ENTCLASS_OPERATION,						//8 操作事件
	ENTCLASS_NOTICE,					    //9 通知事件
	ENTCLASS_WEB,							//10 WEB应用事件
	ENTCLASS_MAINT							//11 维护类应用事件
};

enum EventVerionFlag
{
	NOMAL_EVENT_FLAG = 0,		
	EX_EVENT_FLAG 
};

//事件类型号
enum EventType
{
	ENTTYPE_NULL = 0,							//空事项

	//操作记录类型定义

	//SCADA操作
	ENTTYPE_SCADA = 100,

	//操作记录类型定义
	//SCADA操作
	ENTTYPE_SCADA_YK = 1,				//1 遥控
	ENTTYPE_SCADA_UPDOWN=5,				//5 升降
	ENTTYPE_SCADA_MANSET=9,				//9 人工置数
	ENTTYPE_SCADA_SETFLAG,				//10 设置标志
	ENTTYPE_SCADA_BYPASS,				//11 旁路替代
 	ENTTYPE_SCADA_LOGIN,				//12 用户登录和退出
 	ENTTYPE_SCADA_UPDHISDB,				//13 历史库修改
 	ENTTYPE_SCADA_YT,					//14 遥调

	//保护定值操作
	ENTTYPE_RELAY = 200,
    ENTTYPE_RELAY_PRECONFIG,					//201 预置定值
    ENTTYPE_RELAY_CONFIG_CONFIRM,				//202 监督确认修改定值
    ENTTYPE_RELAY_CONFIG_TAKE,					//203 执行修改定值
    ENTTYPE_RELAY_CONFIG_CANCEL,				//204 撤消修改定值

    ENTTYPE_RELAY_PRECONFIG_NO,					//205 预置定值区号
    ENTTYPE_RELAY_CONFIGNO_CONFIRM,				//206 监督确认修改定值区号
    ENTTYPE_RELAY_CONFIGNO_TAKE,				//207 执行修改定值区号
    ENTTYPE_RELAY_CONFIGNO_CANCEL,				//208 撤消修改定值区号

	//文件操作
	ENTTYPE_FILE = 300,
	ENTTYPE_FILE_SAVE,							//301保存文件
	ENTTYPE_FILE_DEL,							//302删除文件

	//实时库操作
	ENTTYPE_RDB = 400,
	ENTTYPE_RDB_SQL_UPDATE,						//401 实时库SQL修改
	ENTTYPE_RDB_PUT_FIELD,						//402 修改域
	ENTTYPE_RDB_LOADTABLE,						//403 加载库
	ENTTYPE_RDB_LOADMODE,						//404 加载模式

	ENTTYPE_SWITCH_HOST = 500,					//切机

	//系统运行状态事项
	ENTTYPE_SYS = 1000,
	ENTTYPE_SYS_NET,							//1001 网络
	ENTTYPE_SYS_SERVER,							//1002 服务器
	ENTTYPE_SYS_PROCESS,						//1003 进程
	ENTTYPE_SYS_RTU,							//1004 RTU
	ENTTYPE_SYS_CH,								//1005 通道
	ENTTYPE_SYS_FTU,							//1006 FTU
	ENTTYPE_SYS_MAIL,							//1007 邮件
	ENTTYPE_SYS_DISK,							//1008 DISK
	ENTTYPE_SYS_CPU,							//1009 CPU
	ENTTYPE_SYS_DBSPACE,						//1010 数据库容量
	ENTTYPE_SYS_PRINTER,						//1011 打印机事项
	ENTTYPE_SYS_GROUP_SWITCH,					//1012 组间切换
	ENTTYPE_SYS_MEM,							//1013 MEM
	ENTTYPE_SYS_SWITCH,							//1014 交换机
	ENTTYPE_SYS_NETFLOW,						//1015 网络流量事件


	//一般电力事项（遥测越限、遥信变位、SOE）
	ENTTYPE_POWERSYS = 2000,
	ENTTYPE_POWERSYS_OVERLIMI,					//2001 YC越限
	ENTTYPE_POWERSYS_STATUS,					//2002 状态量正常变位
	ENTTYPE_POWERSYS_STATUSODD,					//2003 状态量异常变位
	ENTTYPE_POWERSYS_BREAK,						//2004 开关正常变位
	ENTTYPE_POWERSYS_BREAKSG,					//2005 开关事故变位
	ENTTYPE_POWERSYS_SWITCH,					//2006 刀闸正常变位
	ENTTYPE_POWERSYS_PROTSIG,					//2007 保护信号变位
	ENTTYPE_POWERSYS_STATUSSOE,					//2008 状态量SOE
	ENTTYPE_POWERSYS_BREAKSOE,					//2009 开关SOE
	ENTTYPE_POWERSYS_SWITCHSOE,					//2010 刀闸SOE
	ENTTYPE_POWERSYS_PROTSIGSOE,				//2011 保护信号SOE
	ENTTYPE_POWERSYS_SGOVERLIMIT,				//2012 事故跳闸次数越限
	ENTTYPE_POWERSYS_MULTSTA,					//2013 多态量状态变化
	ENTTYPE_POWERSYS_YGSIG,						//2014 预告信号变位
	ENTTYPE_POWERSYS_YGSIGSOE,					//2015 预告信号SOE
	ENTTYPE_POWERSYS_CHGOVERLIMIT,				//2016 变位次数越限
    ENTTYPE_POWERSYS_SUBSTATION,				//2017 厂站警告事项
	ENTTYPE_POWERSYS_YXDOUDONG,					//2018 遥信抖动次数越限
	ENTTYPE_POWERSYS_WFBS,					    //2019 五防
	ENTTYPE_POWERSYS_CVT,					    //2020 CVT
	ENTTYPE_POWERSYS_PROTCNT,				    //2021 保护信号变位次数越限报警

	//综合自动化保护事件
	ENTTYPE_PROTECT = 3000,
	ENTTYPE_PROTECT_EVENT,						//3001 综合自动化保护信息
	ENTTYPE_PROTECT_DIAGNOSE,					//3002 综合自动化保护诊断信息 		
	ENTTYPE_RELAYFAULT,							//3003 保护故障
    ENTTYPE_RELAYCHKERR,						//3004 保护自检错误
	ENTTYPE_PROTECT_DETECT,						//3005 保护检测

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////DMS事项类型定义/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//RTU/FTU/DTU故障检测类型

	//配电子站定义的诊断结果
	ENTTYPE_DIAG=4000,                         //配电事项
	ENTTYPE_DIAG_SGZ,			               //4001 配网事故总信号/有故障
	ENTTYPE_DIAG_NOFAULT,			           //4002 无故障
	ENTTYPE_DIAG_INST,			               //4003 瞬间故障
	ENTTYPE_DIAG_UNKNOWN_FAULT,			       //4004 故障类型不明确
	ENTTYPE_DIAG_FAULT,						   //4005 有故障

	ENTTYPE_DIAG_GND=4010,                     //接地
	ENTTYPE_DIAG_S_GND,		                   //4011 单相接地
	ENTTYPE_DIAG_S_GND_INST,		           //4012 单相瞬时接地
	ENTTYPE_DIAG_A_GND,		                   //4013 A相接地
	ENTTYPE_DIAG_B_GND,			               //4014 B相接地
	ENTTYPE_DIAG_C_GND,			               //4015 C相接地

	ENTTYPE_DIAG_D_GND,			               //4016 相间接地
	ENTTYPE_DIAG_D_GND_INST,		  	       //4017 相间瞬时接地
	ENTTYPE_DIAG_AB_GND,			           //4018 AB相间接地
	ENTTYPE_DIAG_BC_GND,		               //4019 BC相间接地
	ENTTYPE_DIAG_CA_GND,			           //4020 CA相间接地

	ENTTYPE_DIAG_T_GND,			               //4021 ABC相间接地

	ENTTYPE_DIAG_SHORT=4030,                   //短路
	ENTTYPE_DIAG_D_SHORT,				       //4031 相间短路
	ENTTYPE_DIAG_D_SHORT_INST,		  	       //4032 相间瞬时短路
	ENTTYPE_DIAG_AB_SHORT,			           //4033 AB相间短路
	ENTTYPE_DIAG_BC_SHORT,			           //4034 BC相间短路
	ENTTYPE_DIAG_CA_SHORT,			           //4035 CA相间短路

	ENTTYPE_DIAG_T_SHORT,			           //4036 ABC三相短路
	ENTTYPE_DIAG_T_SHORT_INST,		  	       //4037 相间瞬时短路

	//FTU线路故障类型
	ENTTYPE_REP=4100,                           //FTU事项
	ENTTYPE_REP_SGZ,							//4101 配网事故总信号/有故障
	ENTTYPE_REP_NOFAULT,				        //4102 无故障
	ENTTYPE_REP_INST,			                //4103 瞬间故障
	ENTTYPE_REP_UNKNOWN_FAULT,			        //4104 故障类型不明确
	ENTTYPE_REP_FAULT,				            //4105 有故障
	
	ENTTYPE_REP_GND=4110,                       //接地
	ENTTYPE_REP_S_GND,  			            //4111 单相接地
	ENTTYPE_REP_S_GND_INST,		                //4112 单相瞬时接地
	ENTTYPE_REP_A_GND,				            //4113 A相接地
	ENTTYPE_REP_B_GND,			                //4114 B相接地
	ENTTYPE_REP_C_GND,			                //4115 C相接地

	ENTTYPE_REP_SHORT=4120,                     //短路
	ENTTYPE_REP_S_SHORT,		                //4121 单相短路
	ENTTYPE_REP_A_SHORT,		                //4122 A相短路
	ENTTYPE_REP_B_SHORT,			            //4123 B相短路
	ENTTYPE_REP_C_SHORT,			            //4124 C相短路
	ENTTYPE_REP_N_SHORT,			            //4125 零相短路（限值）
	ENTTYPE_REP_D_SHORT,				        //4126 相间短路
	ENTTYPE_REP_D_SHORT_INST,			        //4127 相间瞬时短路
	ENTTYPE_REP_AB_SHORT,				        //4128 AB相间短路
	ENTTYPE_REP_BC_SHORT,				        //4129 BC相间短路
	ENTTYPE_REP_CA_SHORT,				        //4130 CA相间短路

	ENTTYPE_REP_T_SHORT,		                //4131 三相间短路
	ENTTYPE_REP_T_SHORT_INST,		            //4132 三相间瞬时短路

	ENTTYPE_REP_OVER=4140,                      //过流
	ENTTYPE_REP_S_OVER,				            //4141 单相过流
	ENTTYPE_REP_S_OVER_INST,		            //4142 单相瞬时过流
	ENTTYPE_REP_A_OVER,				            //4143 A相过流
	ENTTYPE_REP_B_OVER,				            //4144 B相过流
	ENTTYPE_REP_C_OVER,				            //4145 C相过流
	ENTTYPE_REP_N_OVER,				            //4146 零相过流

	ENTTYPE_REP_D_OVER,				            //4147 相间过流
	ENTTYPE_REP_D_OVER_INST,		            //4148 相间瞬时过流
	ENTTYPE_REP_AB_OVER,				        //4149 AB相间过流
	ENTTYPE_REP_BC_OVER,				        //4150 BC相间过流
	ENTTYPE_REP_CA_OVER,				        //4151 CA相间过流






	//参数维护类型
	ENTTYPE_MAINT = 5000,
	ENTTYPE_MAINT_UPLOAD,						//上装
	ENTTYPE_MAINT_DOWNLOAD,						//下装
    NOTICETYPE_FRESHCARD  = 20000,              //刷新牌
	//用户自定义事项
	ENTTYPE_USERDEF = 10000					//用户自定义事项
};


#endif			//all of end
