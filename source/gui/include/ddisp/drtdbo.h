#ifndef _DRTDBO_H
#define _DRTDBO_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include "ddbi/ddbi_def.h"
#include "ddbo/ddbo.h"

class GDE_EXPORT_DRTDBO GykQueue:public Ddbo
{
public:
	GykQueue();
	~GykQueue();

	int32 exist( const char* name);
};

//实时库ykpara表
typedef struct __tagYKPARA
{
	char  name[ GDB_CODE_LEN  ];		//代码
	char  desc[ GDB_DESC_LEN ];			//描述
	uint16	terminalno;					//终端序号
	char	termdesc[ GDB_DESC_LEN ];	//终端描述
	uint16	yxno;						//遥信序号
	uint16	ykno;						//遥控序号
	char	monflag;					//调度模式
	uint8	requesttime;				//遥控请求时间限
	uint8	waitchktime;				//遥控返校时间限
	uint8	sendcmdtime;				//遥控发令时间限
	uint8	execcmdtime;				//遥控执行时间限
	uint8	lockykf;					//封锁遥控标志
	float	value;						//值
	char	ykcode[ GDB_CODE_LEN * 2 ];		//遥控校验码
	char	tablename[ GDB_CODE_LEN ];	//遥信所在实时表名
	uint8	exykflag;					//紧急遥控标志
	char	ykmode;
	int32   securegrade;				//安全级别
}YKPARA,*PYKPARA;

class GDE_EXPORT_DRTDBO GykPara:public Ddbo
{
public:
	
	GykPara();
	~GykPara();

	uint8  readData( const char* name, const char* tableName, char* errormsg );
	uint8  readHData( const char* name, Ddbi *hDbi );
	PYKPARA data();
protected:
	
	YKPARA		m_ykPara ;
};

//
typedef struct __tagYTPARA
{
	char  name[ GDB_CODE_LEN ];			//代码
	char  desc[ GDB_DESC_LEN ];			//描述
	uint16	terminalno;					//终端序号
	char  termdesc[ GDB_DESC_LEN ];		//终端描述
	uint16	upykno;						//升遥控号
	uint16	downykno;					//降遥控号
	uint16	stopykno;					//急停遥控号
	uint16	ytno;						//遥调号
	char	monflag;					//调度模式
	uint8	requesttime;				//遥控请求时间限
	uint8	waitchktime;				//遥控返校时间限
	uint8	sendcmdtime;				//遥控发令时间限
	uint8	execcmdtime;				//遥控执行时间限
	uint8	lockytf;					//封锁标志
	float	value;						//值
	char	ytcode[ GDB_CODE_LEN * 2 ];		//遥调校验码
	char	tablename[ GDB_CODE_LEN ];	//参数所在表名：analog or updownpara
	uint8	yttype;						//遥调类型：升降方式还是设点方式
}YTPARA,*PYTPARA;

class GDE_EXPORT_DRTDBO GytPara:public Ddbo
{
public:
	
	GytPara();
	~GytPara();

	uint8  readData( const char* name, char* errormsg  );
	uint8  readHData( const char* name, Ddbi *hDbi );
	PYTPARA data();
protected:
	
	YTPARA		m_ytPara ;
};

typedef struct _tagCardHisInfo
{
	int16	sort;
	int16	type;
	char	desc[ GDB_DESC_LEN ];
	char	symbolpath[ 255 ];
	int16	symbolid;
}CARDHISINFO, *PCARDHISINFO;

typedef struct _tagCardRtInfo
{
	int16	objecttype;
	char	objectname[ GDB_CODE_LEN ];
	int16	cardtype;
	int16	cardno;
	FLOAT8	x;
	FLOAT8	y;
	int16	symbolid;
	char	hostname[ GDB_CODE_LEN ];
	char	username[ GDB_CODE_LEN ];
	char	cardtime[ GDB_DESC_LEN * 2 ];
	char	graphpath[ GDB_PATH_LEN ];
	char	symbolpath[ GDB_PATH_LEN ];
	char	note[ GDB_PATH_LEN ];
	char	devdesc[ GDB_DESC_LEN ];
}CARDRTINFO, *PCARDRTINFO;


class GDE_EXPORT_DRTDBO GcardHisPara:public Ddbo
{
public:
	
	GcardHisPara();
	~GcardHisPara();

	uint8  readData(); 
	PCARDHISINFO data();
	uint16 count();
protected:

	PCARDHISINFO	m_cardHisInfo;
	int32			m_cardHisCount;
};

class GDE_EXPORT_DRTDBO GcardRtPara:public Ddbo
{
public:
	
	GcardRtPara();
	~GcardRtPara();

	uint8  readData(); 
	PCARDRTINFO data();
	uint16 count();
protected:
	void clear();

	PCARDRTINFO		m_cardRtInfo ;
	int32			m_cardRtCount;
};

typedef struct _tagBreakRepInfo
{
	char	breakrep[ GDB_CODE_LEN ];
	char	breakrepdesc[ GDB_DESC_LEN ];
	char	breakname[ GDB_CODE_LEN ];
	char	breakdesc[ GDB_DESC_LEN ];
	uint8	replacetype;
	char	plswitch[ GDB_CODE_LEN ];
	uint8	replacemode;
}BREAKREPINFO, *PBREAKREPINFO;

class GDE_EXPORT_DRTDBO GbreakRepPara:public Ddbo
{
public:
	
	GbreakRepPara();
	~GbreakRepPara();

	uint8  readData( const char * breakrepname ); 
	PBREAKREPINFO data();
	uint16 count();
	uint8	m_replacedFlag; //被替代标志
	char	m_replacedBreak[ GDB_CODE_LEN ];//被替代开关
	char	m_replacedBreakDesc[ GDB_DESC_LEN ];
protected:
	PBREAKREPINFO		m_breakRepInfo ;
	int32			m_breakRepCount;
};

class GDE_EXPORT_DRTDBO GName2DescMap:public Ddbo
{
public:
	GName2DescMap(){};
	~GName2DescMap(){};

	uint8  readData();
	
	uint8 getDesc( const char * name, char * desc );

	uint16 count();

protected:
	QMap< QString, QString > m_name2DescMap;
};

#endif
