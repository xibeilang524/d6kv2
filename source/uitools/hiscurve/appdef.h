#ifndef APPDEF_H
#define APPDEF_H

#include "scd/saveflag.h"
#include "sysdef.h"

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtCore/QMetaType>

#define EPSILON  (1e-5)

#define C2G(str) QString::fromLocal8Bit(str)
#define G2C(qs)  (const char*)(qs).toLocal8Bit()

#define MANUSER  C2G("superdoor")
#define MANPWD   C2G("superdoor")
#define APPNAME  C2G("hiscurve")
#define ORGNAME  C2G("daqo")

#define ONE_CURVE_TABLE_COLUMN 2

#define YCCURVETABLEPREFIX C2G("YC")
#define DDCURVETABLEPREFIX C2G("KWH")
#define YCDAYTABLEPREFIX C2G("DAYYC")
#define DDDAYTABLEPREFIX C2G("DAYKWH")
#define YCMONTABLEPREFIX C2G("MONTHYC")
#define DDMONTABLEPREFIX C2G("MONTHKWH")
#define YCYEARTABLEPREFIX C2G("YEARYC")
#define DDYEARTABLEPREFIX C2G("DYEARKWH")
#define VARIANTCURVETABLEPREFIX C2G("YCCHANGE")
#define TRENDCURVETABLEPREFIX C2G("YCTREND")

// #define YCTYPEDEFINED        C2G("/dbinfo/遥测类型.txt")
// #define KWHTYPEDEFINED       C2G("/dbinfo/电度_类型值.txt")
#define DayCycleBeginMinStaticMinute



enum CurveTreeNodeType{RootNode,TerminalNode,YCParaNode,KWHParaNode};
enum CurveTreeType{StationMeasType=1,EnergyMeasType,EmsKindType};

enum CurveDataSrc{YcDataSrc,DdDataSrc};
enum CurveCycleType{DayCycle=1,MonthCycle=2,YearCycle=3,VariantDayCycle = 4,TrendDayCycle = 5};
enum CurveSegmentType{GeneralDate,SegmentDate};
enum CurveSelectorType{SingleSelector=1,MutilSelector=2,CompSelector=3};

//enum CurveDataKindDef{CurrentKind,VoltageKind,WPowerKind,RPowerKind,WKwhKind,RKwpKind,OtherTypeKind};

// #define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
// 
// static const QRgb DataCurveColorTable[]=
// {
// 		qRgb(0xFF,0x00,0x00),
// 		qRgb(0x00,0xFF,0x00),
// 		qRgb(0x00,0x00,0xFF),
// 		qRgb(0xFF,0xFF,0x00),
// 		qRgb(0xFF,0x00,0xFF),
// 		qRgb(0x80,0x80,0x80),
// 		qRgb(0x80,0x80,0x00),
// 		qRgb(0x80,0x00,0x80),
// 		qRgb(0x80,0x00,0x00)
// };

struct curvetypepara
{
private:
	int		_type;
	QString _desc;
	QColor  _color;
	QString _unit;
public:
	curvetypepara():_type(0),_desc(""),_color(QColor(Qt::black)),_unit(""){;}
	curvetypepara(int Type,QString desc,QColor color,QString unit)
		:_type(Type),_desc(desc),_color(color),_unit(unit){;}
	int Type() const{return _type;}
	QString Desc() const{return _desc;}
	QColor  Color() const{return _color;}
	QString Unit() const{return _unit;}
};

struct terminalpara
{
	short	no;
	QString	name;
	QString	describe;
	short   zone;
	QString sname;
	bool    beginstatus;
	bool    isstat;
	bool    isused;
};

struct stationviewpara
{
	short	no;
	QString	name;
	short   areano;
};

 struct StationAreaPara
{
	short	id;						//id
	QString	name;	//name
	short	masterid;				//masterAreaId
};

struct ycpara
{
	QString	 name;					//代码
	QString	 describe;				//描述
	short    zone;					//监控区
	QString  devname;				//设备代码
	short    type;					//类型
	short	 terminalno;			//终端号
	short    ycno;					//遥测序号
	
	float    warnupperlimit;
	float	 warnlowerlimit;
	float    alertupperlimit;
	float    alertlowerlimit;
	
	bool     iscomputer;			//计算量标志
	short    savetime;				//存盘周期
	short    plantime;              //计划间隔
	bool     isshow;				//是否显示
	//
	QString  typeDescribe;
};

struct kwhpara
{
	QString	 name;					//代码
	QString	 describe;				//描述
	short    zone;					//监控区
	QString  devname;				//设备代码
	short    type;					//类型
	short	 terminalno;			//终端号
	short    kwhno;					//电度序号
	float    basecode;				//基码
    float    scale;					//标度
	float	 maxcode;				//满码
	float    times;					//倍率
	short    savetime;				//存盘周期
	short    collectortype;			//采集类型
	ulong    primitivemaxcode;		//原始满码;
	bool     iscomputer;			//计算量标志
	bool     isintegral;			//积分标志
	bool     issave;				//存盘标志
	bool     isdel;                 //删除标志
	short    starttime;			//起始运行时间
	bool     startstatus;			//起始状态
	bool     isstat;				//是否统计
	bool     isshow;                //是否显示
	//
	QString  typeDescribe;
};


struct hisDataValue
{
    QString		name;
    ushort      date;
    ushort      time;
	ushort      ms;
    uchar   	flag;
	union
	{
		uchar		yxdata;
        float       ycdata;
		float		kwhdata;
	} val;
	union
	{
		uchar		 yxdata;
        short		 ycdata;
		uint 		 kwhdata;
	} rawval;
    short	      odate;
    short         otime;
    uchar         oflag;
};

Q_DECLARE_METATYPE(terminalpara*);
Q_DECLARE_METATYPE(StationAreaPara*);
Q_DECLARE_METATYPE(ycpara*);
Q_DECLARE_METATYPE(kwhpara*);

#endif