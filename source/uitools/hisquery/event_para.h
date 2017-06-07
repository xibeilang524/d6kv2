/**
@file event_para.h
@brief 

@author chenkai
@version 1.2.0
@date 2016-11-7
*/
#ifndef EVENT_PARA_H
#define EVENT_PARA_H

#include <QString>
#include <QList>
#include <QMap>
#include <QStringList>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12
#define MAX_EVENT_COUNT 1500 

#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

typedef struct _GroupPara
{
	uint	no;
	QString code;
	QString desc;
}GROUPPARA,*PGROUPPARA;

typedef struct _AreaPara
{
	int	areano;			//id
	QString	desc;		//name
	int masterareano;	//masterid
}AREAPARA,*PAREAPARA;

typedef struct _StationPara
{
	QString	code;		//代码
	QString	desc;		//描述
	uint	groupno;	//组号
	uint	areano;		//区域序号
}STATIONPARA,*PSTATIONPARA;

typedef struct _RtuPara
{
	uint	no;			//终端序号
	QString	code;		//代码
	QString	desc;		//描述
	QString	rtucode;	//站所线系代码
}RTUPARA,*PRTUPARA;		

typedef struct _YcPara
{
	QString	stationcode;//站所线系代码
	QString	code;		//代码
	QString	desc;		//描述
	QString devcode;	//设备代码
	uint	type;		//类型
}YCPARA,*PYCPARA;

typedef struct _YxPara
{
	QString	stationcode;//站所线系代码
	QString	code;		//代码
	QString	desc;		//描述
	QString devcode;	//设备代码
	uint	type;		//类型
}YXPARA,*PYXPARA;

typedef struct _EventTypeDef
{
	uint	sort;		//事项分类号
	uint	type;		//事项类型号
}EVENTTYPEDEF,*PEVENTTYPEDEF;

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


class Event_para : public QObject
{
public:
	Event_para();
	virtual ~Event_para();

	bool readPara();

	uint getGroupCount();
	bool getGroupDesc( uint id, QString &desc );
	bool getGroupNo( uint id, uint &no );

	bool readAreaPara() ;
	QList<QStringList> getAreaPara() ;

	//厂站分类 涉及到站所线系参数表(区域序号)和区域参数表
	bool readStationPara();
	QList<QStringList> getStationPara() ;

	bool getStaCode( uint groupno, QStringList &stations );
	bool getStaDesc( const QString &code, QString &desc );

	bool getYcCode( const QString &rtucode, QStringList &yccode );
	bool getYcDesc( const QString &code, QString &desc );

	bool getYxCode( const QString &rtucode, QStringList &yxcode );
	bool getYxDesc( const QString &code, QString &desc );
	bool getYxType( const QString &code, uint &type );

	uint getEventSortCount();
	bool getEventSortDesc( uint sort, QString &desc );

	uint getEventTypeCount();
	PEVENTTYPEDEF getEventType( uint id );
	bool getEventTypeDesc( uint type, QString &desc );
	bool getEventTypes( uint sort, uint *types, uint &num );

	uint getEventTypeByDesc( QString &desc );

	bool getDevCode( const QString &rtucode, QStringList &devcode );
	bool getDevCode( const QString &rtucode, uint devtype, QStringList &devcode );
	bool getDevDesc( const QString &devcode, QString &devdesc );
	PDEVPARA getDev( const QString &devcode );

	uint getDevTypeNum();
	bool getDevType( uint id, PDEVTYPEPARA devtype );
	//得到和设备相关的实时量
	bool getDevRealCode( const QString &devcode, QStringList &codes );

	uint getIPNum();
	bool getIPByID( uint id, QString &ip );

	bool isConfirmEventType( uint type );

private:
	bool readGroupPara();
	bool readRtuPara();
	bool readYcPara();
	bool readYxPara();
	bool readDevPara();
	bool readEventType();
	bool readDevTypePara();
	bool readNodeTable();
	bool readConfirmEvent();

	bool readFile( char *filename, void ** buf );
	bool readFile( char *filename, void * buf, int len );

private:
	QList<GROUPPARA *>		m_groupPara;
	QList<STATIONPARA *>	m_stationPara;
	QList<RTUPARA *>		m_rtuPara;
	QList<YCPARA *>			m_ycPara;
	QList<YXPARA *>			m_yxPara;
	QList<DEVPARA *>		m_devPara;
	QList<DEVTYPEPARA *>	m_devTypePara;
	QList<EVENTTYPEDEF *>	m_eventTypePara;

	//终端序号到站所线系代码索引
	QMap<uint,QString>		m_rtuno2StaCode;

	//事项类型到描述索引
	QMap<uint,QString>		m_typeDescMap;

	//事项描述到类型索引
	QMap<QString,uint>		m_descTypeMap;

	//事项描述到类型索引
	QMap<QString,QStringList*>	m_staCode2ObjCodesMap;

	//遥测代码到遥测结构索引
	QMap<QString, YCPARA*>	m_ycCode2YcParaMap;

	//遥信代码到遥信结构索引
	QMap<QString, YXPARA*>	m_yxCode2YxParaMap;

	//设备代码到设备结构索引
	QMap<QString, DEVPARA*>	m_devCode2DevParaMap;

	QStringList				m_hostIPList;

	DBApi					m_db;

	QList<uint>		m_confirmEventType;
	int						m_rfile;

	QList<QStringList> m_stationStringList;
	QList<QStringList>	m_areaStringList; 
};

#endif
