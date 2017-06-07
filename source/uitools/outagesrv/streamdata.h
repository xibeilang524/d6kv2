///streadata.h
///实时数据类
///读取各开关上的实时采集量：------遥信：开关状态
///读取各刀闸上的实时采集量：------遥信：刀闸状态
///读取各设备的带电状态
///保存停电开始和结束信息
//////////////////////////////////////////////////////////////////////////////////////							

#ifndef _STREAM_DATA_H_ 
#define _STREAM_DATA_H_

#include "globaldef.h"
#include "rdb/rdbapi.h"
#include "net/netapi.h"

#include <QMap>
#include <QString>
#include <QStringList>

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
	int initStreamPara();	//流数据参数化

	void readStreamData();	//数据读取
	void stopStreamData();	//停止数据流刷新
	int saveStartData();	//保存停电开始信息
	int saveEndData();		//保存停电结束信息
	
private:
	RDB_FIELD_STRU* m_pStreamData;

	//实时数据段面
	unsigned char *m_rtDataState[REALDATA_BUF_SIZE];

	//设备带电状态段面
	unsigned char *m_tpDevActive[REALDATA_BUF_SIZE]; 

	//实时功率断面
	float *m_analogPValue[REALDATA_BUF_SIZE];

	//设备停电开始时间
	QMap< QString, int > m_name2StartTime;

	//停电结束设备
	QStringList m_endDevList;

	Rdb_QuickStream m_rdbStream;

	//读写标志，防止两个线程同时操作
	int bUsing[ REALDATA_BUF_SIZE ];
	int m_curWritePointer;
	int m_saveRdataTime[ REALDATA_BUF_SIZE ];	//保存数据的时间断面
	int m_firstFlag;	//首次流数据标识
	DBApi m_dbApi;
private:
	void removeEndData();//删除已经结束的数据
};
#endif
