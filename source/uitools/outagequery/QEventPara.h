// QEventPara.h: interface for the QEventPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
#define AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QString>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QStringList>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12

typedef struct _GroupPara
{
	uint	no;
	QString code;
	QString desc;
}GROUPPARA,*PGROUPPARA;

typedef struct _StationPara
{
	uint	groupno;	//组号
	QString	code;		//代码
	QString	desc;		//描述
}STATIONPARA,*PSTATIONPARA;

typedef struct _DevPara
{
	QString	stationcode;//站所线系代码
	QString	code;		//代码
	QString	desc;		//描述
	uint	type;		//设备类型
}DEVPARA,*PDEVPARA;

typedef struct _DevTypePara
{
	uint	devtype;	//设备类型号
	QString	desc;		//描述
}DEVTYPEPARA,*PDEVTYPEPARA;

typedef struct _FeederLinePara
{
	uint	no;			//序号
	QString	name;		//代码
	QString	desc;		//描述
}FEEDERLINEPARA,*PFEEDERLINEPARA;

typedef struct _PowerCutPara	//停电管理参数
{
	QString	code;			//代码
	uint	type;			//设备类型
	QString	stationcode;	//站所线系代码
	uint	feeder;			//所属馈线号
	uint	starttime;		//停电开始时间
	uint	endtime;		//停电结束时间
	QString	breakname;		//事故开关
	uint	cutType;		//停电类型
	float	loadValue;		//丢失负荷容量
}POWERCUTPARA,*PPOWERCUTPARA;

typedef struct _PlanCutPara	//计划停电
{
	QString	code;		//代码
	QString	desc;		//描述
	uint	beginYmd;	//停电开始日期
	uint	beginHms;	//停电开始时分秒
	uint	endYmd;		//停电结束日期
	uint	endHms;		//停电结束时分秒
}PLANCUTPARA,*PPLANCUTPARA;

typedef struct _AreaInfoPara	//配电区域信息
{
	QString	code;		//代码
	QString	desc;		//描述
}AREAINFOPARA,*PAREAINFOPARA;

typedef struct _UserInfoPara	//配电客户信息
{
	QString	code;			//代码
	QString	desc;			//描述
	uint	type;			//客户类型
	QString	belongsLoad;	//所属负荷
	QString	belongsArea;	//所属区域
}USERINFOPARA,*PUSERINFOPARA;

class QEventPara
{
public:
	QEventPara();
	~QEventPara();

	bool readPara();

	bool fineDevByFeederLine( QStringList& devList , QStringList& feederLineList ) ;

	uint getGroupCount();
	bool getGroupDesc( uint id, QString &desc );
	bool getGroupNo( uint id, uint &no );

	bool getStaCode( uint groupno, QStringList &stations );
	bool getStaDesc( const QString &code, QString &desc );

	bool getDevDesc( const QString &devcode, QString &devdesc );

	uint getDevTypeNum();
	bool getDevType( uint id, PDEVTYPEPARA devtype );

	bool getDevTypeNo( uint& devType, QString desc );
	bool getDevTypeDesc( uint devType, QString &desc );
	bool getDevTypeDescs( QStringList &devtypedescs );

	bool getFeederLineDesc( uint &feederNo, QString &feederDesc );

	QList<FEEDERLINEPARA *>	m_feederLinePara;
	QList<PLANCUTPARA *>		m_planCutPara;

	//所属负荷到客户信息索引
	QMap<QString, USERINFOPARA*>	m_belongsLoad2userInfoMap;
	
	//馈线号到馈线索引
	QMap<uint, FEEDERLINEPARA*>	m_feederLineNo2FeederLineMap;

	//区域名到区域描述索引
	QMap<QString, QString>		m_areaName2descMap;


private:
	bool readGroupPara();		
	bool readStationPara();
	bool readDevPara();
	bool readDevTypePara();
	bool readFeederLinePara();
	bool readPlanCutPara();
	bool readAreaPara();
	bool readUserPara();

private:

	QList<GROUPPARA *>			m_groupPara;
	QList<STATIONPARA *>		m_stationPara;
	QList<DEVPARA *>				m_devPara;
	QList<DEVTYPEPARA *>		m_devTypePara;

	//事项类型到描述索引
	QMap<uint,QString>		m_typeDescMap;

	//事项描述到类型索引
	QMap<QString,uint>		m_descTypeMap;

	//设备代码到设备结构索引
	QMap<QString, DEVPARA*>	m_devCode2DevParaMap;

	//设备类型号到设备类型索引
	QMap<uint, DEVTYPEPARA*>	m_devTypeNo2DevTypeMap;

	//设备类型描述到设备类型索引
	QMap<QString, DEVTYPEPARA*>	m_devTypeDesc2DevTypeMap;


	DBApi	m_db;
};

#endif // !defined(AFX_QEVENTPARA_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
