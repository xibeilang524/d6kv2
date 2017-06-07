// QZonePara.h: interface for the QZonePara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QZonePara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
#define AFX_QZonePara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QString>
// #include <qptrlist.h>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QMutex>
// #include <qvaluelist.h>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12


enum NodeType
{
	NODETYPE_USER = 0,
	NODETYPE_DEV
};

typedef struct NodePara
{
	uint		zone;
	uint		oldzone;
	QString		code;
	QString		desc;
	NodeType	type;
	uint		devtype;
	QString		sname;
}NODEPARA,*PNODEPARA;

typedef struct _StationPara
{
	QString	code;		//代码
	QString	desc;		//描述
}STATIONPARA,*PSTATIONPARA;

typedef struct _DevTypePara
{
	uint devtype;
	QString devname;
	QString tablename;
}DEVTYPEPARA, *PDEVTYPEPARA;


typedef struct _StationAreaPara
{
	short	id;						//id
	char	name[REASONINFO_LEN];	//name
	short	masterid;				//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA;

class QZonePara
{
public:
	QZonePara();
	virtual ~QZonePara();

	bool readPara();

	bool getStationDesc( const QString &sname, QString & desc );
	bool savePara();
	bool getDevCode( const QString &rtucode, uint devtype, QStringList &devcode );
	bool getDevDesc( const QString &devcode, QString &devdesc );
	bool getZoneDesc( const QString &devcode, QString &zonedesc );
	PNODEPARA getDev( const QString &devcode );

	QStringList m_zoneNames;
	QMap<int,PSTATIONAREAPARA>	m_station_area;
	QMap<QString,int>	m_stanametoid;
	QList<PSTATIONPARA>		m_stationList;
	QList<PNODEPARA>		m_userList;
	QList<PNODEPARA>		m_devList;
	QList<PDEVTYPEPARA>		m_devTypeList;

	QMap<QString, QString>	m_hostmap;

	//事项描述到类型索引
	QMap<QString,QStringList*>	m_staCode2ObjCodesMap;

	//设备代码到设备结构索引
	QMap<QString, PNODEPARA>	m_devCode2DevParaMap;

	//责任区序号到责任区描述索引
	QMap<uint, QString>	m_zoneNo2ZoneDescMap;

private:
	bool readStationPara();
	bool readDevPara();
	bool readDevTypePara();
	bool readNodeTable();
	bool readUserPara();
	bool readZonePara();
    bool readStationAreaPara();
	bool saveUserPara();
	bool saveDevPara();
	bool saveZonePara();
private:
	DBApi					m_db;
};

#endif // !defined(AFX_QZonePara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
