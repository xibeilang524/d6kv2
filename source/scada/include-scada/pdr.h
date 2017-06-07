#ifndef  _PDR_H
#define  _PDR_H

#include	"scadatime.h"
#include	"scd/cmddef.h"
#include    "scd/comnlink.h"
#include	"rdb/rdbapi.h"

#define		MAXPDRGROUPNUMS		128
#define		SNP_GROUPNO			0

///////////////////////////////////////////////////////////////////
//追忆组参数表属性：
#define  ZY_SELSUBST_NUMS		6
#define  ZY_SELVOLT_NUMS		6

struct PDRGROUP_STRU
{
    char    groupname[OBJECTNAME_LEN];//代码
    char    describe[OBJDESCRIBE_LEN];//描述
	short   groupno;   //分组序号
    uchar   zygrpno;   //追忆组号
	uchar   zymode;    //追忆方式
    uchar   sampledensity;	//采样周期
    uchar   prepoints;		//事故前点数
    uchar   postpoints;		//事故后点数
	uchar   zycondition;    //追忆条件
	char    selsubst[ZY_SELSUBST_NUMS][SUBSTNAME_LEN];//站所线系
	short   selvolt[ZY_SELVOLT_NUMS];//电压等级
	uchar   usef;    //使用标志
};


//追忆描述表结构(向历史库中存储时使用，一次事故引起的追忆在历史库中只有一条追忆描述记录)
struct SPdrDescribe	{
    char		breakername[OBJECTNAME_LEN];	// 追忆开关名
    char		breakername2[OBJECTNAME_LEN]; 
    int			timeflag;			// 时间标志
	int			ymd;
	int		    hmsms;
	short		zygrpno;
    uchar		causetype;			// 追忆原因类型(事故自动追忆1或人工手动快照0)
    uchar		sampledensity;		// 采样间隔(秒)
    uchar		preframenums;		// 事故前点数
    uchar		postframenums;		// 事故及事故后点数
    char		cause[40];			// 原因描述(事项描述)	
};

//追忆存盘数据结构(向历史数据库中存储时用)
struct SPdrSaveData	{
    short	zygrpno;					// 追忆组号
    short	frameno;					// 追忆点号(事故前为负数, 事故后为0及正数)
    intertime	timeflag;				// 时间标志
    char	objname[OBJECTNAME_LEN];	// 被追忆对象名
    float	value;						// 值(遥测量为浮点数值, 遥信量为状态值)
    short	baseobjtype;				// 被追忆对象类型
    uchar	qualityf;					// 质量标志
};

//////////////////////////////////////////////////////////////////////////////////
typedef struct {
	char		triggername[OBJECTNAME_LEN];
	intertime	trigtime;
	int			ymd;
	int		    hmsms;
	uchar		zygrpno;
    uchar		causetype;			// 追忆原因类型(事故自动追忆1或人工手动快照0)
    char		cause[40];			// 原因描述(事项描述)
	int			trigframeno;
	int			preframenums;
	int			bakframenums;
	int			lastframeno;
} PDR_SAVETASK;


class PdrSaveTaskMan {
	static ComnLinkTable<PDR_SAVETASK>		savetask;
public:
	PdrSaveTaskMan(void);
	~PdrSaveTaskMan(void);
	void AddPdrSaveTask(uchar zygrpno,int trigtime,char *triggername,
					SYS_CLOCK *entclock,int causetype,char *cause,
					int prefnums,int bakfnums);
	void ProcAllSaveTask(void);
};


///////////////////////////////////////////

#define  PDROBJ_TBLTYPE_ANALOG		0
#define  PDROBJ_TBLTYPE_BREAK		1
#define  PDROBJ_TBLTYPE_SWITCH		2

typedef struct {
	char	objname[OBJECTNAME_LEN];
	short	objtype;
	short	tabtype;
	int		tabrcdno;
} OBJDATA_PARA;

class ObjectDataPdr {
	static int opentabf;
	static Rdb_Tab	anatab,brktab,swtab;
	uchar		zygrpno;
	uchar	lockflag;
	int		pdrobjnums;
	short	maxframenums;
	short	curframeno;
	OBJDATA_PARA *objdatapara;
	float		 *objdata;
	uchar		 *quality;
	void  InitObjDataPdr(void);
public:
	ObjectDataPdr(void);
	~ObjectDataPdr(void);
	short	GetCurFrameNo(void){return(curframeno);}
	int		GetMaxFrameNums(void){return(maxframenums);}
	int		SetObjDataPara(uchar zygrpno,int maxfnums,int objnums,OBJDATA_PARA *inpara);
	void	FreshObjData(void);
	void	SavePdrData(PDR_SAVETASK *pstask,int savemode);
};

/////////////////////////////////////////////

typedef struct {
	SPdrDescribe  pdrdes;
	int			datanums;
	char		savefname[200];
} PDR_DBTASK;


class PdrDBTaskMan {
	static ComnLinkTable<PDR_DBTASK>		pdrdbtask;
	void SaveDataInDB(PDR_DBTASK *dbtaskp);
public:
	PdrDBTaskMan(void);
	~PdrDBTaskMan(void);
	void AddPdrDBTask(SPdrDescribe *pdrdesp,int datanums,char *sfname);
	void ProcAllDBTask(void);
};

////////////////////////////////////////////
typedef struct {
	uchar	availflag;
	uchar	initedflag;
	uchar	lockflag;
	uchar	trigflag;
	char    groupname[OBJECTNAME_LEN]; 
	int		sampdensity;
	int		preframenums;
	int		bakframenums;
	int		maxframenums;
	int		timecount;
	ObjectDataPdr *objpdr;
} PDRGROUP_MANINFO;


class PdrGroupMan {
	void SetPdrPara(PDRGROUP_STRU *spgi,int maxfnums);
public:
	PdrGroupMan(void);
	~PdrGroupMan(void);
	void ProcAllPdrGroup(void);
	void UpdAllPdrPara(void);
};


/////////////////////////////////////////

#endif
