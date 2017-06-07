#if !defined(WEBSTRUCT)
#define WEBSTRUCT

#include "rdb/rdbapi.h"

#include "webconstant.h"
#include "message.h"
#include "wfa.h"


struct USEREVENT{
	unsigned char userright;//web事件管理表中的组名
	int eventcount;//包含的事项数
	int excludeevent[256];//类型号
	int ptr;//当前事项的指针
	SYS_EVENT sysevent[EVENT_NUM_MAX];
	struct USEREVENT *puserevent;
	USEREVENT()
	{
		userright = 0;//web事件管理表中的组名
		eventcount = 0;//包含的事项数
		ptr = 0;
		memset(sysevent,0,sizeof(SYS_EVENT)*EVENT_NUM_MAX);
		puserevent = NULL;
	}
};

typedef struct _realcurve{	//实时曲线结构
	unsigned char curStructNo;		//曲线结构编号
	unsigned char cur_type;			//曲线类型
	unsigned char cur_no;			//曲线号
	char libName[CODE_LEN];//库名
	char objName[CODE_LEN];//对象名
	char fieldName[CODE_LEN];//域名
	
	int lastSendTime;//上次发送数据时间,秒数
	short savetime;//存盘间隔,是处理的一个条件。=0不处理。
	char hisTable[64];//历史库表名
	char selfname[64];//selected field name
	char orderfname[64];//order field name
	char where[128];
	_realcurve()
	{
		memset(libName,0,CODE_LEN);
		memset(objName,0,CODE_LEN);
		memset(fieldName,0,CODE_LEN);
		memset(hisTable,0,64);
		memset(selfname,0,64);
		memset(orderfname,0,64);
		memset(where,0,128);
		lastSendTime = 0;
		savetime = 0;
	}
}REALCURVE;


//线程管理
typedef struct _threadmanage{
	time_t	invalidtime;				/*平安超时*/
	time_t  starttime;					/*连接时间起点*/
	char	validf;						/*-1,网络无效;-2,网络异常*/
	char	runf;						/*线程运行标志,1-->run,0-->stop*/
	/*             runf                         validf
	*初始状态       0							0
	*运行态			1							1
	*网络中断		1							-1
	*客户异常		1							-1
	*线程开始停止	0							-1
	*线程清理资源	0							-1
	*清理资源完毕	0							0
	*置成初始态		0							0
	*/
	int	cardrcdchgnums;    /*读挂牌库的次数*/
	int sendfilenums;
	SOCKET		sockid;				/*线程对应的客户socket id*/
	MESSAGE		*psnddata;			/*网络收/发缓冲区*/
	MESSAGE		*prcvdata;
	
	unsigned char		clienttype;		//客户端类型
	unsigned char        right;			//用户权限
	USEREVENT   *puserevent;	//指向本用户的事项缓冲区
	int			eventptr;//事项的指针
	#ifdef WIN32
		CRITICAL_SECTION critThread;//临界段
	#endif
	int			streamcount;		//流的数目
	Rdb_QuickStream *prdb_quickstream;	//注册流
	MULTISTREAM *pmultistream;		//流链表
	
	int			realcurvecount;		//实时曲线的数目,指SOneCurve的数目
	REALCURVE	*prealcurve;


	char		client_name[WEBNODENAMELEN];/*客户端名称*/
	char		client_addr[WEBNODENAMELEN];/*客户端IP地址*/
	char reserved[8];//保留不用
	_threadmanage(){

		invalidtime = 0;
		starttime = 0;

		validf = 0;
		runf = 0;

		sockid = 0;
		psnddata = NULL;
		prcvdata = NULL;

		clienttype = 0;
		right = 0;
		puserevent = NULL;
		eventptr = 0;
		#ifdef WIN32
				InitializeCriticalSection(&critThread);
		#endif

		streamcount = 0;
		prealcurve = NULL;
		prdb_quickstream = NULL;
		pmultistream = NULL;

		realcurvecount = 0;
		memset(client_name,0,WEBNODENAMELEN);
		memset(client_addr,0,WEBNODENAMELEN);
	}
	~_threadmanage()
	{
		#ifdef WIN32
				DeleteCriticalSection(&critThread);
		#endif
	}

}THREADMANAGE;



typedef struct _devparatable{
	int count;//记录个数
	int devtype[DEVTYPENUM];//设备类型
	NAME paratable[DEVTYPENUM];//参数表名
	NAME showfield[DEVTYPENUM];//显示域名
	_devparatable()
	{
		memset(devtype,0,sizeof(int)*DEVTYPENUM);
		memset(paratable,0,sizeof(NAME)*DEVTYPENUM);
		memset(showfield,0,sizeof(NAME)*DEVTYPENUM);
	}
}DEVPARATABLE;

#endif // !defined(WEBSTRUCT)
