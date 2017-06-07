#ifndef _DDBO_H
#define _DDBO_H

//#include <qstring.h>
#include <QtCore/QString>
#include <QtCore/QObject>

#include "ddbi/ddbi_def.h"

#define	G_MSLEN		32
#define G_USRRIGHT	42
#define G_IPL		16

class Ddbi;

class GDE_EXPORT_DBO Ddbo
{
public:
	Ddbo();
	~Ddbo();

	Ddbi* getDBI();
	void  setDBI(Ddbi* dbi);

	uint8 isReady() const;

	uint8 readValue(const char* tblname,\
		const char* keyfield,\
        const char* keyword,\
		const char* field,\
		GdbField& value);

	uint8 readData(const char* tblname,\
		const char* field,\
		const char* name,\
		GdbRecords& recs);

	void  fillData(GdbDataFmt& colfmt,GdbRecord& rec);
	uint8 insertRow(const char* tblname,GdbRecord& rec);

private:

	Ddbi*		m_ddbi;
};

enum 
{
	UR_MJHC,//修改计划曲线
	UR_MRD ,//修改实时数据
	UR_MHC ,//修改历史曲线
	UR_MHE ,//修改历史事项
	UR_MGF ,//修改图形文件
	UR_MNP ,//修改网络参数
	UR_MDB ,//修改数据库参数
	UR_MTB ,//修改报表

	UR_VJHC,//查看计划曲线
	UR_VRD ,//查看实时数据
	UR_VHC ,//查看历史曲线
	UR_VHE ,//查看历史事项
	UR_VGF ,//查看图形文件
	UR_VNP ,//查看网络参数
	UR_VDB ,//查看数据库参数
	UR_VTB ,//查看报表
	UR_VXBD,//查看谐波数据
	UR_VLBD,//查看录波数据
	UR_VDI ,//查看调试信息
	UR_VRC ,//查看通讯原码

	UR_YK  ,//遥控
	UR_YT  ,//遥调
	UR_SYK ,//超级遥控
	UR_SF  ,//遥信停闪
	UR_PC  ,//挂牌
	UR_EC  ,//事项确认
	UR_NCT ,//网络对时
	UR_SH  ,//切换主机
	UR_SC  ,//切换通道
	UR_CP  ,//微机保护
	UR_MPV ,//修改定值
	UR_PRS ,//保护投退
	UR_MSVN,//修改定值区号
	UR_PRS1,//微机保护预留1
	UR_PRS2,//微机保护预留2
	UR_PRS3,//微机保护预留3
	UR_SRTU,//停运RTU
	UR_CMNP,//控制模拟盘
	UR_LFD ,//封锁前置数据
	UR_PDL ,//参数下载
	UR_PUL ,//参数上载
	UR_GZP  //光字牌
};

typedef struct _tagUserRight
{
	uint8	type;
	char    userdesc[GDB_DESC_LEN];
	char	password[GDB_CODE_LEN];
	char	hostname[GDB_CODE_LEN];
	char	username[GDB_CODE_LEN];
	uint8	right[G_USRRIGHT];
	uint8	grade[G_MSLEN];

} USERRIGHT,*PUSERRIGHT;

class GDE_EXPORT_DBO GUserRightPara:public Ddbo
{
public:

	GUserRightPara();
	~GUserRightPara();

	uint8 readData();

	uint32 count() const ;
	PUSERRIGHT operator[](uint32 index) const;
	PUSERRIGHT at(uint32 index) const;
	PUSERRIGHT find(const char* hostname,const char* username) const;
	uint32     filter(PUSERRIGHT* dp ,const char* hostname) const;
	PUSERRIGHT data() const;

	static PUSERRIGHT find( PUSERRIGHT dp, uint32 ncount, const char* username);
	static PUSERRIGHT findDesc( PUSERRIGHT dp, uint32 ncount, const char* userdesc);
protected:
	void    clear();
protected:
	PUSERRIGHT    m_urPara ;
	uint32		  m_count  ;
};

class GDE_EXPORT_DBO GUserRightTell
{
	PUSERRIGHT	m_pUR;

public:

	GUserRightTell(PUSERRIGHT pUR);
	void setUR(PUSERRIGHT pUR);

	uint8 tellRight( int32 right ) const;
	uint8 haveGrade( int32 grade ) const;
};


typedef  struct _taddispatcher
{
	uint8  type;
	char   typedesc[GDB_DESC_LEN];
	char   username[GDB_CODE_LEN];
	char   userdesc[GDB_DESC_LEN];
	char   password[GDB_CODE_LEN];
	char   groupname[GDB_CODE_LEN];
	uint8  ismonitor;
}DISPATCHER,*PDISPATCHER;

class GDE_EXPORT_DBO DdispatcherPara:public Ddbo
{
public:
	DdispatcherPara();
	~DdispatcherPara();

	uint8 readData();

	uint32 count() const ;
	PDISPATCHER operator[](uint32 index) const;
	PDISPATCHER at(uint32 index) const;
	PDISPATCHER find(const char* username) const;
	PDISPATCHER findDesc(const char* userdesc) const;
	uint32      filter(PDISPATCHER* dp ,uint8 type) const;
	PDISPATCHER data() const;

	static PDISPATCHER find( PDISPATCHER dp, uint32 ncount, const char* username) ;
	static PDISPATCHER findDesc(PDISPATCHER dp, uint32 ncount,const char* userdesc);
protected:
	void    clear();
protected:
	PDISPATCHER   m_dpPara ;
	uint32		  m_count  ;
};

typedef struct __tagTypeDesc
{
	uint8 type;
	char  desc[GDB_DESC_LEN];

}TYPE2DESC,*PTYPE2DESC;

class GDE_EXPORT_DBO DdispatcherStatistic
{
public:

	DdispatcherStatistic(DdispatcherPara* dpPara);
	~DdispatcherStatistic();

	uint32	 count() const;
	PTYPE2DESC operator[](uint32 index) const;
	PTYPE2DESC at(uint32 index) const;
	PTYPE2DESC data() const;
	PTYPE2DESC find(const char* typedesc) const;

	void   statistic(GUserRightPara* urPara);
	void   statistic(PUSERRIGHT dp, uint32 ncount);
	uint32 filter( PUSERRIGHT* rp , PUSERRIGHT dp, uint32 ncount,uint8 type);
	uint32 filter( PUSERRIGHT* rp , PUSERRIGHT dp, uint32 ncount,const char* typedesc);

protected:

	void clear();
protected:

	PTYPE2DESC			m_typeDesc;
	uint32				m_count;
	DdispatcherPara*	m_dpPara;
};

typedef struct __tagNetInfo
{
	char	nodename[GDB_CODE_LEN];
	char	hostname[GDB_CODE_LEN];
	uint16	groupno;
	uint8	hostno;
	char	a_ip[G_IPL];
	char	b_ip[G_IPL];
	char	a_v_ip[G_IPL];
	char	b_v_ip[G_IPL];
	uint8	is_area3node;
	uint8	is_control;
	uint8	safe_time;
	uint8	stop_time;
	uint8	is_dialup;
}NETINFO,*PNETINFO;

class GDE_EXPORT_DBO GNetInfoPara:public Ddbo
{
public:

	GNetInfoPara();
	~GNetInfoPara();

	uint8 readData();

	uint32 count() const ;
	PNETINFO operator[](uint32 index) const;
	PNETINFO at(uint32 index) const;
	PNETINFO find(const char* hostname) const;
	PNETINFO find( uint8 hostno) const;
	uint32   filter(PNETINFO* ni ,uint16 groupno) const;
	PNETINFO data() const;
protected:
	void    clear();
protected:
	PNETINFO	m_niPara ;
	uint32		m_count  ;
};

typedef struct __tagSTATIONPARA
{
	char  name[GDB_CODE_LEN];
	char  desc[GDB_DESC_LEN];
	char  gname[GDB_PATH_LEN];
	uint32 the_stat;		//所属态	
	uint8  station_type;	//站类型
	uint8  retreat_stat;					//投退状态
}STATIONPARA,*PSTATIONPARA;

class GDE_EXPORT_DBO GStationPara:public Ddbo
{
public:
	GStationPara();
	~GStationPara();

	uint8  readData();
	uint32   count() const ;
	PSTATIONPARA operator[](uint32 index) const;
	PSTATIONPARA at(uint32 index) const;
	PSTATIONPARA find(const char* name) const;
	PSTATIONPARA findDesc(const char* desc) const;
	PSTATIONPARA data() const;
	uint8 updateName(const char* name,const char* gname);
	uint8 updateDesc(const char* desc,const char* gname);
protected:
	void    clear();
protected:
	PSTATIONPARA  m_stPara ;
	uint32		  m_count   ;
};

typedef struct __tagRTUPARA
{
	int16 no;
	char  name[GDB_CODE_LEN];
	char  desc[GDB_DESC_LEN];
	char  stname[GDB_CODE_LEN];
	uint8 used;
}RTUPARA,*PRTUPARA;

class GDE_EXPORT_DBO GRtuPara:public Ddbo
{
public:
	GRtuPara();
	~GRtuPara();

	uint8  readData();
	uint32   count() const ;
	PRTUPARA operator[](uint32 index) const;
	PRTUPARA at(uint32 index) const;
	PRTUPARA find(int16 sno) const;
	PRTUPARA find(const char* name) const;
	PRTUPARA findDesc(const char* desc) const;
	PRTUPARA stfind(const char* stname) const;
	PRTUPARA data() const;
protected:
	void    clear();
protected:
	PRTUPARA  m_rtuPara ;
	uint32	  m_count   ;
};

typedef struct __tagSCDDEVTYPE
{
	int16 type;
	char  name[GDB_CODE_LEN];
	char  desc[GDB_DESC_LEN];
	char  ptbl[GDB_DESC_LEN];
	char  suffix[GDB_CODE_LEN];

}SCDDEVTYPE,*PSCDDEVTYPE;

class GDE_EXPORT_DBO GScdDevType:public Ddbo
{
public:
	GScdDevType();
	~GScdDevType();
	uint8		readData();
	uint32		count() const ;
	PSCDDEVTYPE operator[](uint32 index) const;
	PSCDDEVTYPE at(uint32 index) const;
	PSCDDEVTYPE find(int16 type) const;
	PSCDDEVTYPE find(const char* name) const;
	PSCDDEVTYPE findDesc(const char* desc) const;
	PSCDDEVTYPE data() const;
	uint8		updateRow(PSCDDEVTYPE oldPara,PSCDDEVTYPE newPara);
protected:
	void    clear();
protected:
	PSCDDEVTYPE  m_scdDevType ;
	uint32		 m_count   ;
};

typedef struct __tagSCDOBJTYPE
{
	int16 type;
	char  name[GDB_CODE_LEN];
	char  desc[GDB_DESC_LEN];
	char  ptbl[GDB_DESC_LEN];
	char  color[GDB_CODE_LEN];

}SCDOBJTYPE,*PSCDOBJTYPE;

class GDE_EXPORT_DBO GScdObjType:public Ddbo
{
public:
	GScdObjType();
	~GScdObjType();
	uint8		readData();
	uint32		count() const ;
	PSCDOBJTYPE operator[](uint32 index) const;
	PSCDOBJTYPE at(uint32 index) const;
	PSCDOBJTYPE find(int16 type) const;
	PSCDOBJTYPE find(const char* name) const;
	PSCDOBJTYPE findDesc(const char* desc) const;
	PSCDOBJTYPE data() const;
	uint32      filter(PSCDOBJTYPE* scdType,const char* ptbl) const;
	uint8		updateRow(PSCDOBJTYPE oldPara,PSCDOBJTYPE newPara);

	static PSCDOBJTYPE find(PSCDOBJTYPE para,uint32 nparas,int16 type);
	static PSCDOBJTYPE find(PSCDOBJTYPE para,uint32 nparas,const char* name);
	static PSCDOBJTYPE findDesc(PSCDOBJTYPE para,uint32 nparas,const char* desc);

protected:
	void    clear();
protected:
	PSCDOBJTYPE  m_scdObjType ;
	uint32		 m_count      ;
};

typedef struct __tagSCDDEVPARA
{
	int32 subid;
	int32 dtypeid;
	int32 devid;
	int32 rcfgid;
	char  name[GDB_CODE_LEN];
	char  desc[GDB_DESC_LEN];
	char  stname[GDB_CODE_LEN];
	int16 type;
	uint8 volh;
	uint8 volm;
	uint8 voll;
	uint8 vdf ;
	char  estname[GDB_CODE_LEN];
	char  model[GDB_DESC_LEN];
	char  remark[255]   ;
	int32 grade;
	int16 bay;
	int16 context;//所属态
	char  feeder_line[GDB_CODE_LEN];	//所属馈线字段   
 	uint8  sub_type;						//子类型
 	uint8  retreat_stat;					//投退状态
}SCDDEVPARA,*PSCDDEVPARA;

class GDE_EXPORT_DBO GScdDevPara:public Ddbo
{
public:
	GScdDevPara();
	~GScdDevPara();

	uint8		readData();
	uint32		count() const ;
	PSCDDEVPARA operator[](uint32 index) const;
	PSCDDEVPARA at(uint32 index) const;
	PSCDDEVPARA find(int32 devid) const;
	PSCDDEVPARA find(const char* name) const;
	PSCDDEVPARA findDesc(const char* desc) const;
	PSCDDEVPARA data() const;

	uint32      filter(PSCDDEVPARA* para,const char* stname) const;
	uint32		filter(PSCDDEVPARA* para,const char* stname,int16 type) const;
	uint32		filter(PSCDDEVPARA* para,const char* stname,const char* estname) const;
	uint32		filter(PSCDDEVPARA* para,const char* stname,const char* estname,int16 type) const;

	int32		newDevID(const char* stname,int32 devid=0) const;
	uint8		updateRow(PSCDDEVPARA oldPara,PSCDDEVPARA newPara);
	uint8       appendRow(PSCDDEVPARA newPara);
	uint8		deleteRow(const char* name);

	static PSCDDEVPARA find(PSCDDEVPARA para,uint32 nparas,int32 devid);
	static PSCDDEVPARA find(PSCDDEVPARA para,uint32 nparas,const char* name);
	static PSCDDEVPARA findDesc(PSCDDEVPARA para,uint32 nparas,const char* desc);

	uint32		getMaxDevId();
protected:
	void    clear();
protected:
	PSCDDEVPARA  m_scdDevPara ;
	uint32		 m_count      ;

};

typedef struct __tagSCDTP
{
	char  name[GDB_CODE_LEN]	;
	uint8 type					;
	char  stname[GDB_CODE_LEN]	;
	char  estname[GDB_CODE_LEN]	;
	char  nodeX[4][GDB_CODE_LEN];				// 减少了4个字段
	uint8 volX[4];
	uint8 powerf;
	char  obligate[32];
}SCDTP,*PSCDTP;

class GDE_EXPORT_DBO GBaseTP:public Ddbo
{
public:
	GBaseTP();
	~GBaseTP();

	uint8		readData();
	uint32		count() const ;
	PSCDTP		operator[](uint32 index) const;
	PSCDTP		at(uint32 index) const;
	PSCDTP		find(const char* name) const;
	PSCDTP		data() const;

	uint32      filter(PSCDTP* para,const char* stname) const;
	uint8		updateRow(PSCDTP oldPara,PSCDTP newPara);
	uint8       appendRow(PSCDTP newPara);
	uint8		appendRows( PSCDTP newPara, uint32 count );

	uint8		deleteAll();
	uint8		deleteST(const char* stname);

	static PSCDTP find(PSCDTP para,uint32 nparas,const char* name);

protected:

	void    clear();

protected:

	PSCDTP		m_scdTP ;
	uint32		m_count ;
	QString		m_strTN ;
};

class GDE_EXPORT_DBO GScdTP:public GBaseTP
{
public:
	GScdTP();
};

class GDE_EXPORT_DBO GEmsTP:public GBaseTP
{
public:
	GEmsTP();
};


typedef struct __tagDATP
{
	char  name[GDB_CODE_LEN]		;
	uint8 type				;
	char  stname[GDB_CODE_LEN]	;
	char  estname[GDB_CODE_LEN]	;
	char  nodeX[6][GDB_CODE_LEN];
	uint8 volX[6];
	uint8 powerf;
	char  obligate[32];
}DATP,*PDATP;

class GDE_EXPORT_DBO GDATP:public Ddbo
{
public:
	GDATP();
	~GDATP();

	uint8		readData();
	uint32		count() const ;
	PDATP		operator[](uint32 index) const;
	PDATP		at(uint32 index) const;
	PDATP		find(const char* name) const;
	PDATP		data() const;

	uint32      filter(PDATP* para,const char* stname) const;
	uint8		updateRow(PDATP oldPara,PDATP newPara);
	uint8       appendRow(PDATP newPara);
	uint8		appendRows( PDATP newPara, uint32 count );

	uint8		deleteAll();
	uint8		deleteST(const char* stname);

	static PDATP find(PDATP para,uint32 nparas,const char* name);

protected:

	void    clear();

protected:

	PDATP		m_daTP ;
	uint32		m_count ;
	QString		m_strTN ;
};


//add by lbh for datp
typedef struct __tagDANODEPARA
{
	int32				nodeid;
	int16				nodetype;
	char				desc[128];
	uint8				mode;
	char				volname[GDB_CODE_LEN];
	FLOAT4				volfac;
	int32				linepriority;
	int32				extensionfield;
	char				linedevname[4][GDB_CODE_LEN];
	uint8				retreat;
}DANODEPARA,*PDANODEPARA;

class GDE_EXPORT_DBO GDaNodePARA:public Ddbo
{
public:
	GDaNodePARA();
	~GDaNodePARA();

	uint8		readData();
	uint32		count() const ;
	PDANODEPARA		operator[](uint32 index) const;
	PDANODEPARA		at(uint32 index) const;
	// 	PDANODEPARA		find(const char* name) const;
	PDANODEPARA		find(int32 nodeid) const;

	PDANODEPARA		data() const;

	//	uint32      filter(PDANODEPARA* para,const char* stname) const;
	uint8		updateRow(PDANODEPARA oldPara,PDANODEPARA newPara);
	uint8       appendRow(PDANODEPARA newPara);
	uint8		appendRows( PDANODEPARA newPara, uint32 count );

	uint8		deleteAll();
	// 	uint8		deleteST(const char* stname);

	//  	static PDANODEPARA find(PDANODEPARA para,uint32 nparas,int32 nodeid);

protected:

	void    clear();

protected:

	PDANODEPARA		m_danodepara ;
	uint32		m_count ;
	QString		m_strTN ;
};


typedef struct __tagDAKGPARA
{
	int32				kgid;
	char				kgname[GDB_CODE_LEN];
	char				kgdesc[GDB_DESC_LEN];
	int32				leftid;
	int32				rightid;
	int32				pnode;
	int16				kgtype;
	uint8				pyc;
	char				closeyx[GDB_CODE_LEN];
	char				openyx[GDB_CODE_LEN];
	int32				signaltime;
	FLOAT4				ratedKA;
	FLOAT4				ratedKAcft;
	FLOAT4				ratedKV;
	FLOAT4				ratedKVcft;
	char				KAycname[GDB_CODE_LEN];
	FLOAT4				KAcft;
	char				KVycname[GDB_CODE_LEN];
	FLOAT4				KVcft;
	char				Pycname[GDB_CODE_LEN];
	FLOAT4				Pcft;
	char				Qycname[GDB_CODE_LEN];
	FLOAT4				Qcft;
	int32				extensionfield;
}DAKGPARA,*PDAKGPARA;


class GDE_EXPORT_DBO GDaKgPARA:public Ddbo
{
public:
	GDaKgPARA();
	~GDaKgPARA();

	uint8		readData();
	uint32		count() const ;
	PDAKGPARA		operator[](uint32 index) const;
	PDAKGPARA		at(uint32 index) const;
	// 	PDANODEPARA		find(const char* name) const;
	PDAKGPARA		find(const char* name) const;

	PDAKGPARA		data() const;

	//	uint32      filter(PDANODEPARA* para,const char* stname) const;
	uint8		updateRow(PDAKGPARA oldPara,PDAKGPARA newPara);
	uint8       appendRow(PDAKGPARA newPara);
	uint8		appendRows( PDAKGPARA newPara, uint32 count );

	uint8		deleteAll();
	// 	uint8		deleteST(const char* stname);

	//  	static PDANODEPARA find(PDANODEPARA para,uint32 nparas,int32 nodeid);

protected:

	void    clear();

protected:

	PDAKGPARA		m_dakgpara ;
	uint32		m_count ;
	QString		m_strTN ;
};

//故障指示器 add by lbh for datp
typedef struct __tagDAGZPARA
{
	int32				gzid;
	char				gzname[GDB_CODE_LEN];
	char				gzdesc[GDB_DESC_LEN];
	int32				node;
	char				leftdesc[GDB_DESC_LEN];
	char				rightdesc[GDB_DESC_LEN];
	int32				extensionfield;
}DAGZPARA,*PDAGZPARA;

class GDE_EXPORT_DBO GDaGzPARA:public Ddbo
{
public:
	GDaGzPARA();
	~GDaGzPARA();

	uint8		readData();
	uint32		count() const ;
	PDAGZPARA		operator[](uint32 index) const;
	PDAGZPARA		at(uint32 index) const;
	PDAGZPARA		find(const char* name) const;
	PDAGZPARA		data() const;
	uint8		updateRow(PDAGZPARA oldPara,PDAGZPARA newPara);
	uint8       appendRow(PDAGZPARA newPara);
	uint8		appendRows( PDAGZPARA newPara, uint32 count );

	uint8		deleteAll();
protected:

	void    clear();

protected:

	PDAGZPARA	m_dagzpara ;
	uint32		m_count ;
	QString		m_strTN ;
};



typedef struct __tagVOLGRADE
{
	char	name[GDB_CODE_LEN];
	char	desc[GDB_DESC_LEN];
	uint8	grade;
	FLOAT4	rating;
	FLOAT4	base;
	uint8	unit;
	uint8	flag;
}VOLGRADE,*PVOLGRADE;

class GDE_EXPORT_DBO GVolGrade:public Ddbo
{
public:
	GVolGrade();
	~GVolGrade();
	uint8	  readData();
	uint32	  count() const ;
	PVOLGRADE operator[](uint32 index) const;
	PVOLGRADE at(uint32 index) const;
	PVOLGRADE find(char grade) const;
	PVOLGRADE find(const char* name) const;
	PVOLGRADE findDesc(const char* desc) const;
	PVOLGRADE data() const;

	QString   toDesc(uint8 grade);
protected:
	void    clear();
protected:
	PVOLGRADE	 m_volGrade ;
	uint32		 m_count    ;
};

typedef struct __tagVOLGRADERGB
{
	int16	grade;
	int16	red;
	int16	green;
	int16	blue;
}VOLGRADERGB,*PVOLGRADERGB;

class GDE_EXPORT_DBO GVolGradeRGB:public Ddbo
{
public:
	GVolGradeRGB();
	~GVolGradeRGB();
	uint8	  readData();
	uint32	  count() const ;
	PVOLGRADERGB operator[](uint32 index) const;
	PVOLGRADERGB at(uint32 index) const;
	PVOLGRADERGB find(char grade) const;
	PVOLGRADERGB data() const;
	uint8		 updateRow(int16 grade,uint32 color);
	uint32		 volRGB(char grade);
	uint32		 volRGB(PVOLGRADERGB rgb);
protected:
	void    clear();
protected:
	PVOLGRADERGB	m_volGradeRGB ;
	uint32			m_count       ;
};

typedef struct __tagCARDPARA
{
	int16	sort;
	int16	type;
	char	desc[ GDB_DESC_LEN ];
	char	path[ GDB_PATH_LEN ];
	int16	id;
	uint8	valid;
}CARDPARA,*PCARDPARA;

class GDE_EXPORT_DBO GcardPara:public Ddbo
{
public:
	GcardPara();
	~GcardPara();
	uint8	  readData();
	uint32	  count() const ;
	PCARDPARA operator[](uint32 index) const;
	PCARDPARA at(uint32 index) const;

	PCARDPARA data() const;

	PCARDPARA find(int16 sort,int16 type) const;
	uint8   deleteRow(PCARDPARA newPara);
	uint8	updateRow(PCARDPARA oldPara,PCARDPARA newPara);
	uint8   appendRow(PCARDPARA newPara);
protected:
	void    clear();
protected:
	PCARDPARA	m_cardPara ;
	uint32		m_count    ;
};

//add  for topocolor
typedef struct __tagTOPOCOLOR
{
	int16				no;
	char				desc[GDB_DESC_LEN];
	int16				red;
	int16				green;
	int16				blue;
}TOPOCOLOR,*PTOPOCOLOR;

class GDE_EXPORT_DBO GTopoColor:public Ddbo
{
public:
	GTopoColor();
	~GTopoColor();
	uint32			count() const ;
	PTOPOCOLOR		operator[](uint32 index) const;
	PTOPOCOLOR		at(int16 index) const;
	PTOPOCOLOR		data() const;
	PTOPOCOLOR find(int16 no) const;
	PTOPOCOLOR findDesc(const char* desc) const;
	uint8			readData();
	uint8			updateRow(uint16 colorindex,uint32 color);
	uint8			updateRow(PTOPOCOLOR oldpara,PTOPOCOLOR newpara);
	//  [12/4/2013 ljs]
	uint8			appendRow(PTOPOCOLOR newPara);
	uint8			appendRows( PTOPOCOLOR newPara, uint32 count );

	static PTOPOCOLOR find(PTOPOCOLOR para,uint32 nparas,int16 no);
	static PTOPOCOLOR findDesc(PTOPOCOLOR para,uint32 nparas,const char* desc);

	/*
	PTOPOCOLOR find(char grade) const;
	uint32		 volRGB(char grade);
	uint32		 volRGB(PVOLGRADERGB rgb);*/
protected:
	void    clear();
protected:
	PTOPOCOLOR		m_topoColor;
	uint32			m_count;
};

//add  for bay
typedef struct __tagBAYPARA
{
	int16				devid;
	char				desc[GDB_DESC_LEN];
	char				name[GDB_DESC_LEN];
	char				keydev[GDB_DESC_LEN];
	char				station[GDB_DESC_LEN];
}BAYPARA,*PBAYPARA;


class GDE_EXPORT_DBO GBayPara:public Ddbo
{
public:
	GBayPara();
	~GBayPara();

	uint8		readData();
	uint32		count() const ;
	PBAYPARA operator[](uint32 index) const;
	PBAYPARA at(uint32 index) const;
	PBAYPARA find(const char* name) const;
	PBAYPARA find(int32 devid) const;

	PBAYPARA findDesc(const char* desc) const;
	PBAYPARA data() const;

	uint32      filter(PBAYPARA* para,const char* stname) const;

	int32		newDevID(const char* stname,int32 devid=0) const;
	uint8		updateRow(PBAYPARA oldPara,PBAYPARA newPara);
	uint8       appendRow(PBAYPARA newPara);
	uint8		deleteRow(const char* name);

	static PBAYPARA find(PBAYPARA para,uint32 nparas,int32 devid);
	static PBAYPARA find(PBAYPARA para,uint32 nparas,const char* name);
	static PBAYPARA findDesc(PBAYPARA para,uint32 nparas,const char* desc);
protected:
	void    clear();
protected:
	PBAYPARA	 m_baypara ;
	uint32		 m_count      ;
};

//add  for feeder line
typedef struct __tagFEEDLINEPARA
{
	int16				devid;
	char				name[GDB_DESC_LEN];
	char				desc[GDB_DESC_LEN];
	char				stname[GDB_DESC_LEN];
	char				powername[GDB_DESC_LEN];
	char				out_break[GDB_DESC_LEN];
	int8				retreat_stat;					//投退状态
}FEEDLINEPARA,*PFEEDLINEPARA;

class GDE_EXPORT_DBO GFeederLine:public Ddbo
{
public:
	GFeederLine();
	~GFeederLine();

	uint8		readData();
	uint32		count() const ;
	PFEEDLINEPARA operator[](uint32 index) const;
	PFEEDLINEPARA at(uint32 index) const;
	PFEEDLINEPARA find(const char* name) const;
	PFEEDLINEPARA find(int32 devid) const;

	PFEEDLINEPARA findDesc(const char* desc) const;
	PFEEDLINEPARA data() const;

	uint32      filter(PFEEDLINEPARA* para,const char* stname) const;

	int32		newDevID(const char* stname,int32 devid=0) const;
	uint8		updateRow(PFEEDLINEPARA oldPara,PFEEDLINEPARA newPara);
	uint8       appendRow(PFEEDLINEPARA newPara);
	uint8		deleteRow(const char* name);

	static PFEEDLINEPARA find(PFEEDLINEPARA para,uint32 nparas,int32 devid);
	static PFEEDLINEPARA find(PFEEDLINEPARA para,uint32 nparas,const char* name);
	static PFEEDLINEPARA findDesc(PFEEDLINEPARA para,uint32 nparas,const char* desc);
protected:
	void    clear();
protected:
	PFEEDLINEPARA	 m_feeder_line_para ;
	uint32		 m_count      ;
};


//add by chenkai for state_def
typedef struct __tagSTATEDEFPARA
{
	int16			ID;
	char				name[GDB_CODE_LEN];
	char				describe[GDB_CODE_LEN];
	int16			byapp;
}STATEDEFPARA,*PSTATEDEFPARA;

class GDE_EXPORT_DBO GStateDefPara:public Ddbo
{
public:
	GStateDefPara();
	~GStateDefPara();

	uint8		readData();
	uint32		count() const ;
	PSTATEDEFPARA operator[](uint32 index) const;
	PSTATEDEFPARA at(uint32 index) const;
	PSTATEDEFPARA find(const char* name) const;
	PSTATEDEFPARA find(int16 ID) const;

	PSTATEDEFPARA findDesc(const char* desc) const;
	PSTATEDEFPARA data() const;

	static PSTATEDEFPARA find(PSTATEDEFPARA para,uint32 nparas,int32 devid);
	static PSTATEDEFPARA find(PSTATEDEFPARA para,uint32 nparas,const char* name);
	static PSTATEDEFPARA findDesc(PSTATEDEFPARA para,uint32 nparas,const char* desc);

protected:
	void    clear();
protected:
	PSTATEDEFPARA	 m_statedefPara ;
	uint32		 m_count      ;
};
////////////////////////////////////////////////

class GDE_EXPORT_DBO DdboComm
{
public:

	GStationPara	m_stPara;
	GRtuPara		m_rtuPara;
	GScdDevType		m_devType;
	GScdObjType		m_objType;
	GScdDevPara		m_devPara;
	GVolGrade		m_volGrade;
	GVolGradeRGB	m_volGradeRGB;
	Ddbo			m_hisPara;
	GScdTP			m_scdTP;
	GEmsTP			m_emsTP;
	GNetInfoPara	m_niPara;
	GUserRightPara  m_urPara;
	DdispatcherPara m_dpPara;
	GcardPara		m_cardPara;
	//add  for topo color
	GTopoColor		m_topoColor;
	//add  for bay
	GBayPara		m_baypara;

	//add  for feederline
	GFeederLine		m_feeder_line;

	//add by chenkai for state_def
	GStateDefPara m_state_def ;

	//add by lbh for datp
	GDATP			m_daTP;
	GDATP			m_dagzTP;
	GDaNodePARA		m_daNodeTP;
	GDaKgPARA		m_daKgTP;
	GDaGzPARA		m_gzzsqTP;

public:
	DdboComm();
	~DdboComm();

	void setDBI(Ddbi* dbi);
	void readData();
};

typedef struct __tagSRVTYPE
{
	char	name[GDB_DESC_LEN];
	char	desc[GDB_DESC_LEN];
	char	srvname[GDB_DESC_LEN];
	char	dbname[GDB_DESC_LEN];
	char	rdbname[GDB_DESC_LEN];
	int16	value;
}SRVTYPE,*PSRVTYPE;

class GDE_EXPORT_DBO GSrvType:public Ddbo
{
public:
	GSrvType();
	~GSrvType();
	uint8	 readData();
	uint32	 count() const ;
	PSRVTYPE operator[](uint32 index) const;
	PSRVTYPE at(uint32 index) const;
	PSRVTYPE find(int16 value) const;
	PSRVTYPE findRDB(const char* rdbname) const;
	PSRVTYPE findDB(const char* dbname) const;
	PSRVTYPE find(const char* name) const;
	PSRVTYPE findDesc(const char* desc) const;
	PSRVTYPE data() const;
protected:
	void    clear();
protected:
	PSRVTYPE	 m_srvType ;
	uint32		 m_count    ;
};

typedef struct __tagRTDBTABLE
{
	char	name[GDB_CODE_LEN];
	char	desc[GDB_DESC_LEN];
	int16	tag			 ;
	char	rdbname[GDB_CODE_LEN];
	char	srvname[GDB_CODE_LEN];
	char	owner[GDB_CODE_LEN];
	char	ptbl[GDB_DESC_LEN];
}RTDBTABLE,*PRTDBTABLE;

#define GFLT_DBNAME		0x00
#define GFLT_SRVNAME	0x01
#define GFLT_OWNER		0x02

class GDE_EXPORT_DBO GRtdbTable:public Ddbo
{
public:
	GRtdbTable();
	~GRtdbTable();
	uint8	 readData();
	uint32	 count() const ;
	PRTDBTABLE operator[](uint32 index) const;
	PRTDBTABLE at(uint32 index) const;
	PRTDBTABLE find(int16 tag) const;
	PRTDBTABLE find(const char* name) const;
	PRTDBTABLE findDesc(const char* desc) const;
	uint32     filter(PRTDBTABLE* para,const char* key,int32 type=GFLT_DBNAME) const;
	PRTDBTABLE data() const;
	static PRTDBTABLE find(PRTDBTABLE tbls,uint32 ntbls,int16 tag);
	static PRTDBTABLE find(PRTDBTABLE tbls,uint32 ntbls,const char* name);
	static PRTDBTABLE findDesc(PRTDBTABLE tbls,uint32 ntbls,const char* desc);
protected:
	void    clear();
protected:
	PRTDBTABLE	 m_rtdbTable;
	uint32		 m_count    ;
};

typedef struct __tagRTDBFIELD
{
	char	name[GDB_CODE_LEN];
	char	desc[GDB_DESC_LEN];
	char	table[GDB_CODE_LEN];
	int16	datalen;
	uint8	datatype;
	uint8	dotlen	;
	uint8	fieldtype;
	int16   no;
	int16	showno;
	uint8	showflag;
	uint8	eventf;
}RTDBFIELD,*PRTDBFIELD;

class GDE_EXPORT_DBO GRtdbField:public Ddbo
{
public:
	GRtdbField();
	~GRtdbField();
	uint8	 readData();
	uint32	 count() const ;
	PRTDBFIELD operator[](uint32 index) const;
	PRTDBFIELD at(uint32 index) const;
	PRTDBFIELD data() const;
	uint32     filter(PRTDBFIELD* para,const char* table) const;

	static PRTDBFIELD find(PRTDBFIELD flds,uint32 nflds,const char* name);
	static PRTDBFIELD findDesc(PRTDBFIELD flds,uint32 nflds,const char* desc);
protected:
	void    clear();
protected:
	PRTDBFIELD	 m_rtdbField;
	uint32		 m_count    ;
};

typedef struct __tagRTDBPARA
{
	char	name[GDB_CODE_LEN];
	char	desc[GDB_DESC_LEN];
	char	stname[GDB_CODE_LEN];
	int16	type;
}RTDBPARA,*PRTDBPARA;

class GDE_EXPORT_DBO GRtdbPara:public Ddbo
{
public:
	GRtdbPara();
	~GRtdbPara();

	uint8	 readData(const char* tblname);
	uint32	 count() const ;
	PRTDBPARA operator[](uint32 index) const;
	PRTDBPARA at(uint32 index) const;
	PRTDBPARA data() const;
	PRTDBPARA find(const char* name) const;
	PRTDBPARA findDesc(const char* desc) const;

	static uint32 findPattern(PRTDBPARA* para,PRTDBPARA flds,uint32 nflds,const char* name);
	static PRTDBPARA find(PRTDBPARA flds,uint32 nflds,const char* name) ;
	static PRTDBPARA findDesc(PRTDBPARA flds,uint32 nflds,const char* desc);

	uint32 filter(PRTDBPARA* para,const char* stname) const;
	uint32 filter(PRTDBPARA* para,int16 type) const;
	uint32 filter(PRTDBPARA* para,const char* stname,int16 type) const;

	uint8  haveName() const;
	uint8  haveStName() const;
	uint8  haveType() const;

protected:
	void    clear();
protected:
	PRTDBPARA	 m_rtdbPara ;
	uint32		 m_count    ;
	uint8		 m_name,m_stname,m_type;
};

class GDE_EXPORT_DBO DdboRtdbComm
{
public:

	GSrvType	m_srvType;
	GRtdbTable	m_rtdbTable;
	GRtdbField	m_rtdbField;
	GRtdbPara	m_rtdbPara ;
public:

	DdboRtdbComm();
	~DdboRtdbComm();

	void setDBI(Ddbi* dbi);
	void readData();

};

//calculate realdata device id
uint8 GDE_EXPORT_DBO calcRDID( const QString& realdata , DdboComm* dboComm, QString& devname,uint32& devid);
//calculate devname device id
uint8 GDE_EXPORT_DBO calcDDID( const QString& devname , DdboComm* dboComm,uint32& devid);

#endif
