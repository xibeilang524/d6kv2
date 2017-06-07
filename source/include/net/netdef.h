#ifndef	NET_DEF_H
#define	NET_DEF_H

#include "sysstr.h"

#define NET_DATATYPE_UCHAR          0     //无符号字符型
#define NET_DATATYPE_CHAR           1     //有符号字符型
#define NET_DATATYPE_USHORT         2     //无符号短整数型
#define NET_DATATYPE_SHORT          3     //有符号短整数型
#define NET_DATATYPE_UINT           4     //无符号整数型
#define NET_DATATYPE_INT            5     //有符号整数型
#define NET_DATATYPE_ULONG          6     //无符号长整数型
#define NET_DATATYPE_LONG           7     //有符号长整数型(8 byte long)
#define NET_DATATYPE_FLOAT          8     //单精度浮点型
#define NET_DATATYPE_DOUBLE         9     //双精度浮点型
#define NET_DATATYPE_STRING         10    //字符串型

/*-------------------------------------------------------------------------*/

//缓冲区容量定义
#define	RXTXBUFSIZE	16384								//16k
#define	GRAMLEN		4096								//4k
#define DEADDATA_TXSIZE			(1024*64)	//64K jhongtu(1024*1000)				//1M

//网络协议定义
#define	TCPIP					0						/*TCP/IP协议*/

//port default defination

#define	MONPORT					5800					//监控端口
#define	CMDPORT					5800					//命令通道
#define	DATPORT					5801					//数据通道
//#define	FEPPORT					5802					//前置通道状态
#define	DBGPORT					5803					//调试端口
#define	MIRRORPORT				5804					//镜像端口
#define	LINKPORT				5805					//LINK通道

#define TIME_MODE_DIRECT_ONLY	0x01		//仅直接对时
#define TIME_MODE_BUF_ONLY		0x02		//仅缓冲
//#define TIME_MODE_DIRECT_BUF	0x03		//直接对时并缓冲
#define TIME_MODE_STEP			0x04		//分步对时  for unix

/*-------------------------------------------------------------------------*/

//主机类型定义
#define	HOST_TYPE_BASE			0
#define	HOST_TYPE_CENTER		(HOST_TYPE_BASE+1)		/*服务器*/
#define	HOST_TYPE_NORMAL		(HOST_TYPE_BASE+2)		/*客户应用机*/

//主机状态定义
#define	HOST_STATUS_BASE		-1
#define	HOST_STATUS_STOP		(HOST_STATUS_BASE+1)	/*停机态*/
#define	HOST_STATUS_MAIN		(HOST_STATUS_BASE+2)	/*主机态*/
#define	HOST_STATUS_BACK		(HOST_STATUS_BASE+3)	/*备机态*/
#define	HOST_STATUS_WORK		(HOST_STATUS_BASE+4)	/*工作态*/

/*-------------------------------------------------------------------------*/

//系统工作时间定义
#define	HOSTTIME	1									/*检查主机状态时间*/
#define	STOPTIME	20									/*停机检测时间*/
#define	REGTIME		3									/*开机登录等待时间*/
#define	ATBTIME		4									/*平安报文发送时间*/
#define	CHECKTIME	30									/*系统校时时间*/
#define	PERIODTIME	1000000								/*系统基础定时*/
#define	SOCKTIMEOUT	8									/*网络收发超时时间*/
#define	LINK_CONN_TIME	5
#define	LINK_TX_TIME	500
#define	LINK_RX_TIME	500

/*-------------------------------------------------------------------------*/

#if defined(WIN32)					//定义WINDOWS消息
#define	MAIN_TIMER					10000
#define	HOST_STATUS_TIMER			10001
#define	GRAM_TIMER					10002
#define	MAIL_DISP_TIMER				10003
#define	SELF_INFOR_TIMER			10004

#define	DATA_MESSAGE				(WM_USER+100)			/*数据流准备好消息*/
#define	EVENT_MESSAGE				(WM_USER+101)			/*事项准备好消息*/
#define	PROC_CHECK					(WM_USER+102)			/*进程检查消息*/
#define	MAIL_MESSAGE				(WM_USER+103)			/*当有邮件时向主窗口发送的消息*/
#define	RAWD_MESSAGE				(WM_USER+104)			/*生数据准备好消息*/
#define	DATA_CHANGED				(WM_USER+105)
#define END_PROC_SIGNAL				WM_CLOSE
#elif defined(__unix)
#define	DATA_CHANGED				SIGUSR1
#define	END_PROC_SIGNAL				SIGKILL
#endif

/*-------------------------------------------------------------------------*/
#define PORT_NUM_MAX				16
#define	GROUP_MAX					16				/*最大分组个数*/
#define	HOST_MAX					256				/*分组中最大主机个数*/
#define	SERVER_NODE_MAX				16
#define	RTDB_MAX					(HOST_MAX/SERVER_NODE_MAX)

#define	MAILPROCTYPE				int32s
#define	MAIL_NUM_MAX				1024				//缓冲区最大邮件个数
#define	MAIL_LEN_MAX				8192			//缓冲区最大邮件长度
#define	MAIL_TYPE_TX				0
#define	MAIL_TYPE_RX				1
#define	WIN_BUF_MAX					128

#define MAIL_LEVEL_GRADE			10
#define MAIL_BASE_LEVEL				20

/*-------------------------------------------------------------------------*/

#define EVENT_NUM_MAX				65536
#define EVENTDESCRIBE_LEN			40

#define SND_EVENT_MAX				65536
#define GEN_EVENT_MAX				65536
#define OPT_EVENT_MAX				1024

/*-------------------------------------------------------------------------*/

//进程管理数据结构
#define	PROC_MAX					16*2
#define	PROC_NAME_LEN				32*2
#define	AUTO_PROC_MAX				16*2
#define	SRV_PROC_MAX				8*2

//进程控制类
#define	PROC_CTRL_NULL				-1
#define	PROC_CTRL_START				(PROC_CTRL_NULL+1)
#define	PROC_CTRL_KILL				(PROC_CTRL_NULL+2)
#define	PROC_CTRL_GET				(PROC_CTRL_NULL+3)
#define	PROC_CTRL_FRESH				(PROC_CTRL_NULL+4)
#define	PROC_CTRL_RESTARTBUS		(PROC_CTRL_NULL+5)

//进程状态管理
#define PROC_STATUS_BASE			-1
#define	PROC_STATUS_START			(PROC_STATUS_BASE+1)			/*启动*/
#define	PROC_STATUS_STOP			(PROC_STATUS_BASE+2)			/*正常退出*/
#define	PROC_STATUS_ERROR			(PROC_STATUS_BASE+3)			/*异常退出*/
#define	PROC_STATUS_NOTEXIST		(PROC_STATUS_BASE+4)			/*不存在该可执行文件*/
#define	PROC_STATUS_TIME			(PROC_STATUS_BASE+5)			/*进程计数*/

//进程启动方式
#define	PROC_START_MODE_BASE		-1
#define	PROC_START_MODE_NOTSTART	(PROC_START_MODE_BASE+1)		//不启动
#define	PROC_START_MODE_RESPAWN		(PROC_START_MODE_BASE+2)		//守护
#define	PROC_START_MODE_AT			(PROC_START_MODE_BASE+3)		//定时启动
#define	PROC_START_MODE_CYCLE		(PROC_START_MODE_BASE+4)		//周期启动

/*-------------------------------------------------------------------------*/

//以下定义为网络连路信息部分使用的定义
#ifndef MAX_ITEM
#define								MAX_ITEM 30
#endif

//链路信息数据
#ifndef LINK_MAX
#define	LINK_MAX					256
#endif

#define	HEADLEN						sizeof(HEAD)


//共享内存数据库结构
#if defined(__unix)
#define		NET_SHM_DB_KEY			980415
#elif defined(WIN32)
#define		NET_SHM_DB_KEY			"SAC_NET_SHM_DB_KEY"
#endif

#define FileDic    "log/filedic"

/*-------------------------------------------------------------------------*/

#define SOCKET_STATUS_NULL			0x00		//不可用
#define SOCKET_STATUS_READABLE		0x01		//可读
#define SOCKET_STATUS_WRITEABLE		0x02		//可写
#define SOCKET_STATUS_WRITEREAD		0x03		//可写读

//网络数据库结构定义
//权限及属性
#define	attr_switch_host			attr1.bit0
#define	attr_check_time				attr1.bit1
#define	attr_dial_node				attr1.bit2
#define	attr_config_net				attr1.bit3
#define	attr_make_event				attr1.bit4

//系统事件定义

/*-------------------------------------------------------------------------------*/

//双网络序号定义
#define	NET_NO_NULL					-1
#define	NET_NO_A					(NET_NO_NULL+1)				//A网
#define	NET_NO_B					(NET_NO_NULL+2)				//B网

//网络类型定义
#define	NET_TYPE_NULL				0
#define	NET_TYPE_DATA				(NET_TYPE_NULL+1)			/*数据网*/
#define	NET_TYPE_CMD				(NET_TYPE_NULL+2)			/*控制网*/
#define	NET_TYPE_STREAM				NET_TYPE_CMD				/*流数据网*/
#define	NET_TYPE_RAW				NET_TYPE_DATA				/*生数据网*/
#define NET_TYPE_MIRROR				NET_TYPE_DATA				/*镜像数据网*/
#define NET_TYPE_DBG				NET_TYPE_CMD				/*调试数据网*/


//网络状态定义
#define	NET_STATUS_BASE				0
#define NET_STATUS_NOTCONFIG		(NET_STATUS_BASE+1)			/*网络未配置*/
#define NET_STATUS_GOOD				(NET_STATUS_BASE+2)			/*网络正常*/
#define NET_STATUS_NET_ERROR		(NET_STATUS_BASE+3)			/*网络异常*/

/*-------------------------------------------------------------------------------*/


//报文类型
#define GramNull					0								/*空报文*/
//#define	NetConfigGram				(GramNull+1)					/*网络配置*/
//#define	ServerCfgGram				(GramNull+2)					/*服务器配置报文*/
#define	RegGram						(GramNull+3)					/*注册报文*/
#define	AtbGram						(GramNull+4)					/*平安报文*/
#define	MailGram					(GramNull+5)					/*邮件报文*/
#define	ProcCtrlGram				(GramNull+6)					/*进程控制*/
#define	SyncTimeGram				(GramNull+7)					/*校时报文*/
#define	SysInfoGram					(GramNull+8)					/*系统信息报文*/
#define	SysStateGram				(GramNull+9)					/*系统状态（网络状态，服务器信息）*/
#define	SysEventGram				(GramNull+10)					/*系统事件*/
#define	SwitchHostGram				(GramNull+11)					/*切换主机*/
#define	SwitchServerGram			(GramNull+12)					/*切换服务器报文*/
#define	FileInforGram				(GramNull+13)					/*文件信息*/
#define	CenterInfoGram				(GramNull+14)					/*控制机信息*/
#define	DetectCenterGram			(GramNull+15)					/*检测控制信息*/
#define	SrvInforGram				(GramNull+16)					/*服务器信息*/
#define	StreamGram					(GramNull+17)					/*系统流数据*/
#define	StopStreamGram				(GramNull+18)					/*停止刷新系统流数据*/
#define	RawDataGram					(GramNull+19)					/*生数据报文*/
#define	RawEventGram				(GramNull+20)					/*生事项报文*/
#define	RTDBProcGram				(GramNull+21)					/*实时数据库操作*/
#define	DeviceInforGram				(GramNull+22)					/*device information gram */
#define	DebugControlGram			(GramNull+23)					/*调试报文*/
#define	ChanDataGram				(GramNull+24)					/*通道数据报文*/
#define	MirrorDataGram				(GramNull+25)					/*镜像数据报文*/
#define	AtbGram2					(GramNull+26)					/*平安报文2*/
#define	DBGram						(GramNull+27)					/*数据库报文*/
#define	HisStreamGram				(GramNull+28)					/*系统流数据*/
#define	StopHisStreamGram			(GramNull+29)					/*停止刷新系统流数据*/
#define MAX_SELDEF_MAIL_TYPE		256

//NetConfigGram子类--无
//ServerCfgGram子类--无
//ServerCfgGram子类--无
//RegGram子类

//AtbGram子类
#define	ATB_GRAM_BASE			-1
#define	ATB_GRAM_STOP			(ATB_GRAM_BASE+1)
#define	ATB_GRAM_NETAB_UNKOWN	(ATB_GRAM_BASE+2)
#define	ATB_GRAM_NETA			(ATB_GRAM_BASE+3)
#define	ATB_GRAM_NETB			(ATB_GRAM_BASE+4)
#define	ATB_GRAM_NETAB			(ATB_GRAM_BASE+5)

//MailGram子类---邮件类型
//ProcCtrlGram子类--无
//SyncTimeGram子类--无
//SysInforGram子类--无
//SysStateGram子类--无

//SysEventGram子类
#define	SYS_EVENT_GRAM_BASE			-1
#define SYS_EVENT_GRAM_SAVE			(SYS_EVENT_GRAM_BASE+1)	/*全网存*/
#define SYS_EVENT_GRAM_PUT			(SYS_EVENT_GRAM_BASE+2)	/*本地存*/

//SwitchHostGram子类--无
//SwitchServerGram子类--无
//CopyCRTGram子类--无

//FileInforGram子类--无
#define FILE_INFO_GRAM_BASE			-1
#define FILE_INFO_GRAM_CALLINFO		(FILE_INFO_GRAM_BASE+1)
#define FILE_INFO_GRAM_SAVEREQ		(FILE_INFO_GRAM_BASE+2)
#define FILE_INFO_GRAM_DELREQ		(FILE_INFO_GRAM_BASE+3)
#define FILE_INFO_GRAM_SAVESRV		(FILE_INFO_GRAM_BASE+4)
#define FILE_INFO_GRAM_DELSRV		(FILE_INFO_GRAM_BASE+5)

//CtrlNodeInfoGram子类--无  -----(CtrlNodeInfoGram没有用)
//SrvInforGram子类--无 -----(SrvInforGram没有用)
//StreamGram子类--无
//StopStreamGram子类--流序号

//RawDataGram子类
#define	RAW_DATA_GRAM_BASE				-1
#define RAW_DATA_GRAM_NUMS				(RAW_DATA_GRAM_BASE+1)
#define RAW_DATA_GRAM_YC				(RAW_DATA_GRAM_BASE+2)
#define RAW_DATA_GRAM_YX				(RAW_DATA_GRAM_BASE+3)
#define RAW_DATA_GRAM_KWH				(RAW_DATA_GRAM_BASE+4)
#define RAW_DATA_GRAM_STATE				(RAW_DATA_GRAM_BASE+5)
#define RAW_DATA_GRAM_CYCLE				(RAW_DATA_GRAM_BASE+6)
#define RAW_DATA_GRAM_CHANSTATE			(RAW_DATA_GRAM_BASE+7)
#define RAW_DATA_GRAM_DEVSTATE			(RAW_DATA_GRAM_BASE+8)
#define RAW_DATA_GRAM_YC_DATAFLAG		(RAW_DATA_GRAM_BASE+9)
#define RAW_DATA_GRAM_YX_DATAFLAG		(RAW_DATA_GRAM_BASE+10)
#define RAW_DATA_GRAM_KWH_DATAFLAG		(RAW_DATA_GRAM_BASE+11)
#define RAW_DATA_GRAM_STATE_DATAFLAG	(RAW_DATA_GRAM_BASE+12)
#define RAW_DATA_GRAM_CYCLE_DATAFLAG	(RAW_DATA_GRAM_BASE+13)
#define RAW_DATA_GRAM_CHAN_DATAFLAG		(RAW_DATA_GRAM_BASE+14)
#define RAW_DATA_GRAM_DEV_DATAFLAG		(RAW_DATA_GRAM_BASE+15)



//RawEventGram子类--无

//RTDBProcGram子类
#define	RTDB_PROC_GRAM_BASE			-1
#define	RTDB_PROC_GRAM_GET			(RTDB_PROC_GRAM_BASE+1)	/*取数据域*/
#define	RTDB_PROC_GRAM_PUT			(RTDB_PROC_GRAM_BASE+2)	/*修改数据域*/
#define	RTDB_PROC_GRAM_UPDATE		(RTDB_PROC_GRAM_BASE+3)	/*SQL之UPDATE命令*/
#define	RTDB_PROC_GRAM_SELECT		(RTDB_PROC_GRAM_BASE+4)	/*SQL之SELECT命令*/
#define	RTDB_PROC_GRAM_RECALL		(RTDB_PROC_GRAM_BASE+5)	/*加载一个表*/
#define	RTDB_PROC_GRAM_RELOAD		(RTDB_PROC_GRAM_BASE+6)	/*重新加载实时数据库*/
#define	RTDB_PROC_GRAM_MODE			(RTDB_PROC_GRAM_BASE+7)	/*装载库模式*/
#define	RTDB_PROC_GRAM_SRV			(RTDB_PROC_GRAM_BASE+8)	/*对所有服务器操作*/

//DeviceInforGram子类--无

//DebugControlGram子类
#define	DBG_GRAM_BASE				-1
#define	DBG_GRAM_GET_PROCINFO		(DBG_GRAM_BASE+1)		/*取进程信息*/
#define	DBG_GRAM_RET_PROCINFO		(DBG_GRAM_BASE+2)		/*返回进程信息*/
#define	DBG_GRAM_CALL_INFO			(DBG_GRAM_BASE+3)		/*取进程调试信息*/
#define	DBG_GRAM_RET_INFO			(DBG_GRAM_BASE+4)		/*返回程调试信息*/

//MirrorDataGram子类
#define	MIRROR_GRAM_BASE			-1
#define	MIRROR_GRAM_REQ				(MIRROR_GRAM_BASE+1)		/*镜像请求*/
#define	MIRROR_GRAM_DATA			(MIRROR_GRAM_BASE+2)		/*镜像数据*/

/*-------------------------------------------------------------------------*/

//调试信息定义
#define DEBUG_MAX_TYPE				64							//调试每个进程最大信息种类
#define DEBUG_MAX_PROC				16*2						//调试最大进程数目
#define DEBUG_MAX_LINE				128							//调试信息行数
#define DEBUG_MAX_LEN				256							//调试信息长度

#define	DBGTYPE_BASE				-1
#define DBGTYPE_NORMAL				(DBGTYPE_BASE+1)
#define	NET_DBGTYPE_SCADA			(DBGTYPE_BASE+2)		//scada information
#define	NET_DBGTYPE_RX_GRAM			(DBGTYPE_BASE+3)		//接收监控报文
#define	NET_DBGTYPE_TX_GRAM			(DBGTYPE_BASE+4)		//发送监控报文
#define	NET_DBGTYPE_CHECK_BUS		(DBGTYPE_BASE+5)		//check process BUS
#define	NET_DBGTYPE_CHECK_PROC		(DBGTYPE_BASE+6)		//BUS check child process 
#define	NET_DBGTYPE_RX_EVENT		(DBGTYPE_BASE+7)		//received event
#define	NET_DBGTYPE_TX_EVENT		(DBGTYPE_BASE+8)		//received event
#define	NET_DBGTYPE_RX_MAIL			(DBGTYPE_BASE+9)		//receive mail
#define	NET_DBGTYPE_TX_MAIL			(DBGTYPE_BASE+10)		//receive mail
#define	NET_DBGTYPE_FILE_INFO		(DBGTYPE_BASE+11)		//file management information
#define	NET_DBGTYPE_RX_CMD			(DBGTYPE_BASE+12)		//recevie command
#define	NET_DBGTYPE_CHECK_SRV_PROC	(DBGTYPE_BASE+13)		//check server process
#define	NET_DBGTYPE_SYS_CTRL		(DBGTYPE_BASE+14)		//system control
#define	NET_DBGTYPE_STREAM			(DBGTYPE_BASE+15)		//system data
#define	NET_DBGTYPE_RTDB_PROC		(DBGTYPE_BASE+16)		//rtdb procedures
#define	NET_DBGTYPE_RAWD			(DBGTYPE_BASE+17)
#define	NET_DBGTYPE_RAWDTHREAD		(DBGTYPE_BASE+18)
#define	NET_DBGTYPE_RAWDSRV			(DBGTYPE_BASE+19)
#define	NET_DBGTYPE_STREAMSRV		(DBGTYPE_BASE+20)
#define	NET_DBGTYPE_SYNCTIME		(DBGTYPE_BASE+21)
#define	NET_DBGTYPE_COPYSCR			(DBGTYPE_BASE+22)
#define	NET_DBGTYPE_ROMOTEDBG		(DBGTYPE_BASE+23)		//remote dbg
#define	NET_DBGTYPE_SOCKET			(DBGTYPE_BASE+24)		//remote socket level info
#define	NET_DBGTYPE_MIRROR			(DBGTYPE_BASE+25)		//mirror srv/client


#define DB_DBGTYPE_BASE				(DBGTYPE_BASE+100)
#define DB_DBGTYPE_WR				(DBGTYPE_BASE+101)

#define DBG_TYPE_NORMAL		DBGTYPE_NORMAL

enum RemoteDebug
{
	SetDebug = 0,
	ReadDebug,
	READCONFIG,
	READRTDB,
	SWITCHHOST,
	SWITCHSERVER,
	LOADRDB,
	GETPROCINFO,
	KILLPROC,
	STARTPROC
};

/*-------------------------------------------------------------------------*/

//网络错误信息定义
#define	NET_ERR_BASE				0
#define	NET_ERR_NOT_IN_SYSTEM		(NET_ERR_BASE-1)
#define	NET_ERR_NOT_WORKING			(NET_ERR_BASE-2)
#define	NET_ERR_NO_RIGHT			(NET_ERR_BASE-3)
#define	NET_ERR_TX_ERROR			(NET_ERR_BASE-4)
#define	NET_ERR_RX_ERROR			(NET_ERR_BASE-5)
#define	NET_ERR_MEM_ERROR			(NET_ERR_BASE-6)
#define	NET_ERR_LINK_ERROR			(NET_ERR_BASE-7)
#define	NET_ERR_FERT_STOP			(NET_ERR_BASE-8)
#define	NET_ERR_SERVER_STOP			(NET_ERR_BASE-9)


/*-------------------------------------------------------------------------*/

//zip  define

//"15 Bit VBR Encoder"
#define  ZIP_BITS				15		
#define  ZIP_MAX_CODE			0x7fff
//#define  ZIP_MAX_CODE			((1<<ZIP_BITS)-1)
#define  ZIP_TABLE_SIZE			(256*1024)//35023	
#define  ZIP_TABLE_BANKS		0x89
//#define  ZIP_TABLE_BANKS		((ZIP_TABLE_SIZE>>8)+1)
#define  ZIP_END_OF_STREAM		256
#define  ZIP_BUMP_CODE			257
#define  ZIP_FLUSH_CODE			258
#define  ZIP_FIRST_CODE			259
#define  ZIP_UNUSED				-1

#define  ZIP_COMPRESSMEMSIZE_FACTOR				16				
#define  ZIP_COMPRESSMEMADDSIZE_FACTOR			16				
#define  ZIP_EXPANDMEMSIZE_FACTOR				1				
#define  ZIP_EXPANDMEMADDSIZE_FACTOR			1				

#define  COMPRESS_OK		0
#define  COMPRESS_ERR		-1
#define  EXPAND_OK			0
#define  EXPAND_ERR			-1
#define  COMPRESS_KEY		0xEB90
#define  COMPRESSED			1
#define  UNCOMPRESSED		0

/*-------------------------------------------------------------------------*/

#endif		//all of end
