/**
@file data_info.h
@brief 数据结构

@author chenkai
@version 1.2.0
@date 2016-11-1
*/

#ifndef DATAINFO_H
#define DATAINFO_H

#include "sysstr.h"
#include "rdb/rdbdef.h"

#define LONG_CHAR_DESCRIBE		64
#define CHAR_DESCRIBE						40
#define CHAR_NAME								24
#define FIELDMAXNUM_REALTAB	128
#define MAX_TERMINALE_NUM		10000
#define MAX_RTDB_TABLE_NUM		100
#define ROOT_NUM								4

typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned short USHORT;

typedef struct TERMINAL_STRU
{
    short	terminalno ;															//终端序号
	char		termname[ CHAR_NAME ] ;								//终端的代码
    char		termdescribe[ LONG_CHAR_DESCRIBE ] ;	//终端的描述
	short	areaid ;
    char		areaname[ LONG_CHAR_DESCRIBE ] ;
} TERMINAL ;

typedef struct STATIONAREAPARA_STRU
{
	short	id ;															//id
	char		name[ LONG_CHAR_DESCRIBE ] ;	//name
	short	masterid ;												//masterAreaId
}STATIONAREAPARA,*PSTATIONAREAPARA ;

typedef struct RTDBTABLECOL_STRU
{
	char		tablename[ CHAR_NAME ] ;						//实时表名
    char		colname[ CHAR_NAME ] ;							//列名
    char		coldescribe[ CHAR_DESCRIBE ] ;				//列的描述
	BOOL	flag ;																//显示标志
} RTDBTABLECOL ;

typedef struct RTDB_STRU
{
	char  tablename[ CHAR_NAME ] ;							//实时表名
	char  orderfield[ CHAR_NAME ] ;							//排序域名
	char  headfield[ CHAR_DESCRIBE ] ;					//表头域名
	char  tabledescribe[CHAR_DESCRIBE] ;				//实时表描述
	BOOL			sortflag ;													//分类索引标志
	RTDBTABLECOL *col ;											//该表相应的列的信息
	int	  colnum ;															//该表列的总数
} RTDB ;

typedef struct REALDATA_STRU
{
    char			name[ CHAR_NAME ] ;					//代码
    char			describe[ CHAR_DESCRIBE ] ;		//描述
	short		terminalno ;										//终端序号
	short		no ;														//序号(YC,YX,KWH)
	BOOL	mansetf ;													//置入标志
	BOOL	calcultf ;													//计算量标志
	BOOL	inconsistf ;												//可疑标志
	float		slope ;													//倍率
	float		scale ;													//斜率
	float		value ;													//实时值(YC,YX)
	double		daykwh ;												//存盘电度值
} REALDATA ;

typedef struct SHOWCFG_STRU
{	
	BOOL	FreshFlag ;									//是否刷新
	BOOL	ShowYcCalcult ;						//是否显示遥测计算标志
	BOOL	ShowYxCalcult ;						//是否显示遥信计算标志
	BOOL	ShowYcManset ;						//是否显示遥测置入标志
	BOOL	ShowYxManset ;						//是否显示遥信置入标志
	BOOL	ShowYcxs ;									//是否显示遥测系数
	BOOL	ShowYcKy ;								//是否显示遥测可疑
	int		FreshTime ;								//刷新周期
	BOOL	ShowKwhTableCode ;				//电度显示方式
	BOOL OrderByStation ;						//按终端检索标志
}SHOWCFG ;

class DataInfo
{
public:
	DataInfo();
	~DataInfo();

public:
	TERMINAL *Terminal ; //用于取终端的结构
	REALDATA *RealData ;	//实时数据(YX,YC,KWH)的结构
	REALDATA *Yx_Break,*Yx_Switch,*Yx_Prot,*Yx_Status,*Yx_Dmsdiagnose ;//用于显示YX的临时结构
	short Yx_BreakNum,Yx_SwitchNum,Yx_ProtNum,Yx_StatusNum,Yx_DmsdiagnoseNum ;//用于显示YX的临时变量
	int FreshTime ;		//刷新时间（秒）
	short RealFlag ;		//0:yc,1:yx,2:kwh 
	short RealDataNum,TerminalNum,TerminalNo ;	//读取的实时量数目,终端数目,终端序号
	char Station[ CHAR_NAME ] ;								//厂站代码
	BOOL DataOK,OrderByStation ;								//是否读取到,是否由厂站索引

public:
	short RealDataRecLen ;											//数据记录数长度
	short RealDataRecNum ;											//当前实时数据表的数据记录数(数据库表中记录的行数)
	short RealDataColNum ;											//当前实时数据表的数据列数
	void *pRealDataInfo ;												//实时数据列头信息缓冲区
	void *pRealData ;														//实时数据内容缓冲区
	short RtdbTableNum ;												//SCADA实时表的个数
	RTDB *Rtdb ;																//用户设置的关于实时表的结构
};

class CField
{
public:
	short colno;			//该域的列序号
	short fieldlen;		//域的长度
	short fieldvalueaddr;
	BYTE type;				//该域的类型double/float/long.....
	BYTE changeflag;	//该域是否修改
	BYTE useflag;
	int status;			
	union	
	{
		char str[CHAR_NAME]; 
		double dblv;
		float fv;
		hlong lv;
		int iv;
		UINT uv;
		short sv;
		USHORT wv;
		BYTE bv;
		char cv;
		BOOL ov;
	}val;
	short GetColumnNo()	{return colno;}
	BYTE GetType()	{return type;}
	short GetFieldLen()	{return fieldlen;}
	short GetFieldValueAddr()	{return fieldvalueaddr;}
	BYTE GetUseFlag()	{return useflag;}
	BYTE GetChangeFlag()	{return changeflag;}
	int GetStatus()	{return status;}
	char *GetStrv()	{return val.str;}
	double GetDoublev()	{return val.dblv;}
	float GetFloatv()	{return val.fv;}
	hlong GetInt64v()	{return val.lv;}
	int GetIntv()	{return val.iv;}
	UINT GetUintv()	{return val.uv;}
	USHORT GetUshortv()	{return val.wv;}
	short GetShortv()	{return val.sv;}
	BYTE GetBytev()	{return val.bv;}
	char GetCharv()	{return val.cv;}
	BOOL GetBoolv()	{return val.ov;}

	void SetColumnNo(short cno)	{colno = cno;}
	void SetType(BYTE t)	{type = t;}
	void SetFieldLen(short len)	{fieldlen = len;};
	void SetFieldValueAddr(short addr)	{fieldvalueaddr = addr;}
	void SetUseFlag(BYTE use)	{useflag = use;}
	void SetChangeFlag(BYTE cf)	{changeflag = cf;}
	void SetStatus(int s)	{status = s;}
	void SetStrv(char *asc)	{if(asc)	strcpy(val.str,asc);}
	void SetDoublev(double dv)	{val.dblv = dv;}
	void SetFloatv(float f)	{val.fv = f;}
	void SetInt64v(hlong i64)	{val.lv = i64;}
	void SetIntv(int i)	{val.iv = i;}
	void SetUintv(UINT u)	{val.uv = u;}
	void SetUshortv(USHORT w)	{val.wv = w;}
	void SetShortv(short s)	{val.sv = s;}
	void SetBytev(BYTE b)	{val.bv = b;}
	void SetCharv(char c)	{val.cv = c;}
	void SetBoolv(BOOL o)	{val.ov = o;}
};

enum  RDBTYPE 
{
	UNKNOW,
	LOCAL,SERVER
};

#define MAXCOLUMNNUM 100
#define MAXCOLUMNNAMELEN   50
typedef struct
{
	UINT ColumnNum,OldColumnNum,RcdNum,RcdLen;
	UINT ColumnLen[MAXCOLUMNNUM];
	UINT ColumnOffset[MAXCOLUMNNUM];	
	char ColumnName[MAXCOLUMNNUM][MAXCOLUMNNAMELEN];
	char *pretinfo;  
}RetInfo ; 

#define   CONNECT_OK         0
#define   USERNAMEEMPTY     -1
#define   UNKNOW_RDBTYPE    -2

#define  OPERATION_OK   0
#define  SYNTAXERR     -1
#define  GETDICTERR    -2
#define  ALLOCMEMERR   -3
#define  INITMEMERR    -4

#define NEW_RSQLCONTENT    100L
#define NEW_DBGRIDCONTENT    101L

#define	YC	0
#define	YX	1
#define	KWH		2
#define	SCADA_REAL 3

#define	MINTYPE_BREAK			1001
#define	MINTYPE_SWITCH			1002
#define	MINTYPE_PROT				1003
#define	MINTYPE_STATUS			1004
#define	MINTYPE_DMSDIAGNOSE		1005
#define	MINTYPE_YXEND			1010

// #define  _Q2C(str) (str.toAscii().data())
#define  _Q2C(str) (str.toLocal8Bit().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))

static  char* datatype_text[] = {"uchar","char","ushort","short","uint","int",
"ulong","long","float","double","string"
};

#endif