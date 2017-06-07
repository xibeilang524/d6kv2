#ifndef _DATPDATA_H_
#define _DATPDATA_H_

#include "sysdef.h"
#include "model/scdtp.h"

#include <QMap>
#include <QString>

#define  BASEOBJTYPE_PWOER 20
//拓扑中带有实时量设备,以后如果需要记录停电设备电量可以在本结构扩展
typedef struct
{
	char devName[CODE_LEN];	//设备代码
	int	 devType;			//设备类型
	char name[DESC_LEN];	//实时量代码
	int	 tpid;//对应拓扑结构的数组下标
} RTDATA;

//停电管理参数结构
typedef struct
{
	char	name[CODE_LEN];	//设备代码
	char	desc[DESC_LEN];	//
	char	devName[CODE_LEN]; //
	short	baseobjtype;
	float	value;
} ANALOG_P_DATA;

class CTpData
{
public:
	CTpData();
	~CTpData();

public:
	int readTpData();	//读取scdtp表信息

public:
	RTDATA *m_rtData;
	int m_rtDataNum;//实时数据个数
	
	TPRCD* m_topoRcd;
	int m_topoRcdNum;	//拓扑设备个数

	ANALOG_P_DATA* m_analogPData;	
	int m_analogPNum;	//实时功率个数


	//scdtp中代码到记录下标
	QMap< QString, int > m_name2ID;
};
#endif