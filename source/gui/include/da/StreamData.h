//实时数据类，读取各开关上的实时采集量：------遥信：开关状态，是否有效
//                                        |	
//                                        |---遥测：有功，无功，电压，电流
//采集的实时数据进行系数修正和方向调整（只对功率有用），调整原则如下：
//从SCADA读取的实时数据a,该遥测的调整系数为t,方向为f(-1,右节点流向左节点;1,左节点流向右节点)
//DA程序中该遥测实际数据为val
//	val=a*t*f								

#ifndef _STREAM_DATA_XWQ_20030515_H_ 
#define _STREAM_DATA_XWQ_20030515_H_

#include "GlobalDef.h"
#include "rdbapi.h"
#include "netapi.h"

class CStreamData
{
	enum POWERDIR
	{
		NEGATIVE = -1,
		POSITIVE = 1,
	};
public:
	CStreamData();
	~CStreamData();
#ifdef _WINDOWS
	int InitStreamPara(CWnd* pWnd);
#else
	int InitStreamPara();
#endif
	void ReadStreamData();
	void StopStreamData();
	void ReadBackupDataYx(unsigned char** pState , unsigned char** pInvalidf ,int * StateNum);
	void ReadBackupDataYc(float** pP ,float** pQ ,float** pU , float** pI ,int * StateNum);
	void ReadRealYxData(unsigned char** pState , unsigned char** pInvalidf , int * StateNum);
	void ReadRealYcData(float** pP ,float** pQ ,float** pU , float** pI ,int *StateNum);
	int RefreshStreamData(
#ifdef _WINDOWS
		CWnd* pWnd
#else
		
#endif		
		);
//	int RefreshStreamData();
//private:
	int BackupingData();
public:
	float* m_pPowerU;//电源点电压。
private:
	unsigned char *m_pState[REALDATA_BUF_SIZE];
	unsigned char *m_pInvalidf[REALDATA_BUF_SIZE];
	float* m_pAnalogP[REALDATA_BUF_SIZE];
	float* m_pAnalogQ[REALDATA_BUF_SIZE];
	float* m_pAnalogU[REALDATA_BUF_SIZE];
	float* m_pAnalogI[REALDATA_BUF_SIZE];
	unsigned char *m_pStateAtFaultMoment;
	unsigned char *m_pInvalidfAtFaultMoment;
	float* m_pAnalogPAtFaultMoment;
	float* m_pAnalogQAtFaultMoment;
	float* m_pAnalogUAtFaultMoment;
	float* m_pAnalogIAtFaultMoment;
private:
	RDB_FIELD_STRU* m_pStreamData;
//	RDB_FIELD_STRU* m_pBackupData;
	Rdb_QuickStream rdb;
	//读写标志，防止两个线程同时操作
	BOOL bUsing[REALDATA_BUF_SIZE];
	BOOL bBackUsing;
	int m_curWritePointer;
	SYS_TIME inittime;
	SYS_TIME saveRdataTime[REALDATA_BUF_SIZE];
	int m_iParaBaseNum;
	POWERDIR m_dir;
};
#endif
