

#ifndef _GLOBLALDEF_H_XWQ20030519_
#define _GLOBLALDEF_H_XWQ20030519_
#include "sysstr.h"
#include "netapi.h"
#include "dadef.h"
#define __PROGRAM_DEBUG_  //程序编写阶段调试信息输出定义
//#define __PROGRAM_DEBUG_XXX   ///网络底层不通知
#define NODE_MAX_SHAREBRKNUM 11
//#define DA_YK_USERNAME SYS_PWD
//#define DA_YK_PASSWORD SYS_PWD
#define DA_YK_USERNAME "sa"
#define DA_YK_PASSWORD "supernb"
#define DASERVER_NAME "DA"

#define EVENT_VALID_GAP			15 ////收到事项的时间与事项的发生时间的有效间隔
#define DINAMIC_BUF_BLOCK_SIZE	100 ///以100个为一块内存单位进行动态分配内存
#define REALDATA_BUF_SIZE		60 //按一秒的刷新周期计算，可以满足
#define BACK_REALDATA_TIME		2000 //在事故跳闸前备份实时数据的时间（ms）
#define MAX_ONEBRK_EVTNUM		8 //在一次事故中，一个开关的最大报事项容量
#define MAX_SUB_REPORT_SIZE		8 //子站报文中，定位故障和恢复的最大开关容量
#define WAIT_SUBREPORT_TIME		3 //等待子站报文超时时间限值

#define MAX_WAIT_SECONDS		30 //下发遥控命令后，等待时间的限值（s）
///////节点类型定义
#define NODETYPE_POWER			0 //电源点
#define NODETYPE_LINE			1	//架空线
#define NODETYPE_CABLE			2 //电缆
#define NODETYPE_LINK			3	//连接线
#define NODETYPE_TRANSFORMER	4 //变压器

///////节点故障处理模式
#define CENTERISOLATE_CENTERRESTORE 0
#define SUBISOLATE_CENTERRESTORE	1
#define SUBISOLATE_SUBRESTORE		2
////////
///////////主站DA模式
#define DONT_BACKUP		0
#define ISOLATE_BACKUP	1
#define RESTORE_BACKUP	2
#define ALL_BACKUP		3
//////////////////////////////////////////
typedef struct
{
	SYS_TIME recvTime;//DA程序收到事项的时间！
	SYS_EVENT evt;
} EX_SYS_EVENT;

//子站报文
typedef struct
{
	unsigned int Minitues;
	unsigned short Seconds;
} TIME_STRU;
typedef struct
{
	unsigned char DAType;//1 表示集中式控制；2 表示分布式控制；
	unsigned char ReportType;
	//报文类型：0x01表示馈线故障处理报文，0x02表示开闭所备自投报文 ，0x04 表示开闭所故障处理报文 ，
	//0x08 表示备自投恢复报文，0x10 DA方式转换报文
	unsigned short ReportFun;
	//报文数据有效标志：D0：使能0x01 , 故障识别0x02，D1：故障隔离0x04  D2：恢复0x08 。
	//1 表示有效，0表示无效。
	unsigned short FailProc;   //0：无故障,0x01：定位阶段,0x02：隔离阶段,0x04：恢复阶段
} NORMAL_STRU;
typedef struct
{
	unsigned short StartSW ;
	//故障区域起始开关编号
	unsigned short EndSW ;
	//故障区域末端开关编号
} FAULT_POS;
typedef struct
{
	unsigned short SWNo;	/*开关号*/
	unsigned short GZCSwWNo; /*故障侧开关编号*/
	unsigned char Phases;	/*所处步骤0x01：遥控预置，0x02：遥控执行,0x04：通讯检查，0x08：遥控确认*/
	unsigned char Status;	/*失败原因,0：无故障,0x01：点号非法,0x02：该点正在被操作,
	0x03：控制硬件问题，0x05：通讯故障,0x06：开关故障*/
} ACT_BRKINFO;
typedef struct
{
	unsigned short swnum;
	//DA方式发生变化的出口断路器开关号
	unsigned short datype;
	//1 工作在分布式控制方式下，3工作在集中控制方式下
} DAMODE_CONVERT;
typedef struct
{
	TIME_STRU time;
	NORMAL_STRU normal_info;
	FAULT_POS fault_pos;
//	unsigned char isolate_num;
	ACT_BRKINFO isolate_info[MAX_SUB_REPORT_SIZE];
//	unsigned char restore_num;
	ACT_BRKINFO restore_info[MAX_SUB_REPORT_SIZE];
	DAMODE_CONVERT mode_convert[MAX_SUB_REPORT_SIZE];
} REPORT_STRU;//报文结构

typedef struct _ex_report_stru_
{
	SYS_TIME recvTime;//接收时间
	REPORT_STRU Report;
	_ex_report_stru_* pNext;
} EX_REPORT_STRU;


typedef struct
{
	int FaultPos;
	unsigned char FaultType;//故障类型
} FAULT;
typedef struct _fault_info
{
	FAULT fault;
	int bIsolate;//被隔离操作过标志,多次隔离时用
	_fault_info* pNext;
} FAULTINFO;


typedef struct _isolate_brk_
{
	int brkaddr;
	int faultNodeAddr;
	BOOL allfault;//两侧都有故障标志
	BOOL isosuccess;//隔离成功标志
	_isolate_brk_* pNext;
} ISOLATE_BRK;

#ifndef _WINDOWS
void ReadStreamData(int);
void DaQuit(int);
void* 
#else
unsigned int 
#endif
#if defined(_WINDOWS)
DaProc(LPVOID pParam);///DA线程函数
#else
DaProc(void* pVoid);
#endif
#ifndef _WINDOWS
void* 
#else
unsigned int 
#endif
#if defined(_WINDOWS)
EvtProc(LPVOID pParam);///事项线程函数
#else
EvtProc(void* pVoid);
#endif
#endif
