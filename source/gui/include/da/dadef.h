
#ifndef _DA_DEF_H__
#define _DA_DEF_H__
//
//DA程序运行信息事项类型
////////////////////////////////////////////////////////////////////
//#include "sysdef.h"

#ifdef TRACE
#undef TRACE
#endif
#define TRACE		init.PutDbg

////da communicate mail type 
#define DAMAILTYPE_APPLYRESEARCH	0
#define DAMAILTYPE_SELRESPRJ		1
#define DAMAILTYPE_CLEARRDB			2
#define DAMAILTYPE_APPLYDELAY		3 //要求延长等待方案选择的时间
#define DAMAILTYPE_SELISOLATE		4 //zl20111026, 选择手动、放弃
#define DAMAILTYPE_RESTORESTATUS	5 //zxm20140224, 恢复故障前状态
//-------------------------------
#define DAMAILOBJ_NEEDRESEARCH 		1
#define DAMAILOBJ_CANCELRESEARCH 	0
//--------------------------

//da working state defines
#define DAWORKING_REAL		0x00 //实时态	
#define DAWORKING_DEBUG		0x01 //调试态

//who locate fault 
#define CENTERLOCATE		0x01	//主站定位
#define SUBLOCATE		0x02	//子站定位
#define ALLLOCATE		0x03	//都定位的
//
//kg yk result
#define RESULT_SUCCESS		0x01	//控分或者控合成功
#define RESULT_FAIL		0x00	
//
//who send yk command
#define CENTER_EXE		0x00
#define SUB_EXE			0x01

//isolate times
#define CENTER_ISO		0x00 //主站隔离
#define SUB_ISO			0x01 //子站隔离
#define CENTER_BACKISO		0x02 //主站后备隔离
#define CENTER_EXISO		0x03 //主站扩大范围隔离
#define SUB_EXISO		0x04 //子站扩大范围隔离

//restore perform mode
#define SEL_EXE			0x00
#define AUTO_EXE		0x01

enum DAEventtype
{
	DACOMTYPE = 4200 ,              //DA与界面交互的事项起始类型号
	DACOMTYPE_DASTART,              //4201 DA处理过程开始
	DACOMTYPE_DALOCATEFAULT,        //4202 DA故障定位
	DACOMTYPE_ISOLATE,				//4203 DA主站管理区域故障隔离
	DACOMTYPE_EXPLAINFAULT,			//4204 DA解析子站报文
	DACOMTYPE_BACKISOLATE,			//4205 DA后备隔离
	DACOMTYPE_EXISOLATE,			//4206 DA扩大范围隔离
	DACOMTYPE_RESPRJOCCUR,			//4207 DA恢复方案生成
	DACOMTYPE_RESPRJSEL,			//4208 DA恢复方案选定
	DACOMTYPE_RESTORE,				//4209 DA非故障区域恢复
	DACOMTYPE_WORKSTATE,			//4210 DA工作方式转换
	DACOMTYPE_ANALYSE,				//4211 DA智能分析
	DACOMTYPE_DALOCATESUCCESS		//4212 DA故障定位成功//zl20111026
};

typedef struct
{
	unsigned char type;             //是申请研究邮件\方案选定邮件\可以清理实时表
	char	machine[40];		//申请机器名
/*	char	machine_desc[DESC_LEN];	//机器描述
	char	oper[CODE_LEN];			//调度员代码
	char	oper_desc[DESC_LEN];	//调度员描述
	char	pwd[CODE_LEN];			//密码
*/	char	objstate;				//目标请求
	char	extend[24];					
} DAMAIL;

#endif