//
//限定DA工作方式的变量数据结构，作为一个全局变量存储，以利于程序对要求工作模式的检测
//于程序中，启动时由数据库读取，在程序运行时，这些设置的改变
//一方面写入数据库，一方面写入全局变量
//
//                  ----Writen by xiaowq 2003-4-24       
/////////////////////////////////////////

#ifndef __DASETTINGS_H_
#define __DASETTINGS_H_
#include "rdbapi.h"
#include "GlobalDef.h"


class CDaSetting
{
	typedef struct
	{
		short DaMode;//DA工作模式：0－主站隔离，主站恢复 1－子站隔离，主站恢复 2－子站隔离，子站恢复，主站后备
		short BrkProtectStyle;//开关带何种保护 0－不带保护，2－带面保护
		unsigned char WorkingState;//当前的工作状态，0x00－实时态，0x01－调试态
		char Machine[DESC_LEN];		
		unsigned char PerformMode;//恢复方案的执行模式
		unsigned char ExtendIsolate;//是否扩大范围隔离
		unsigned char RemoveNoloadDev;//是否切除空载设备
		unsigned char CanGetFaultDir;//Ftu能否采集到故障方向
		unsigned char BeforeFaultSecs;//故障点前移时间（s）
		unsigned char AfterFaultSecs;//故障点后移时间（s）
		unsigned char ykmode;// 0-群控 1-序列控制
		unsigned char optaim;//优化目标 0-操作开关最少 1-开关不过负荷
		int ExpandFlag; ///////该域用于以后扩展
	} DASETTING;
public:
	CDaSetting();
	~CDaSetting();
public:
	BOOL ReadSetting();
	BOOL WriteSetting(char* fldname = NULL);
//private:
public:
	DASETTING* m_pDasettingAddr;
private:
	BOOL InitDaSettingPointer();
	Rdb_LocalTab rdbDaSetting;
	RDB_BTBL_TAB_STRU m_tabInfo;
};
#endif
