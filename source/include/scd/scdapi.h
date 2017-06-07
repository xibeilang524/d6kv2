#ifndef   _SCDAPI_H
#define   _SCDAPI_H

#include  "net/netapi.h"
#include  "scd/scddef.h"
#include "fes/fesscdshm.h"

#define SCADA_OK       0


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////节点权限校核begin////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////操作配置表/////////////////////////////////

typedef  struct _DISPATCHER_STRU{
	uchar  type;                            //操作员类别
	char   typedescribe[OBJDESCRIBE_LEN];   //类别描述
	char   username[USERNAME_LEN];		    //用户名
	char   describe[OBJDESCRIBE_LEN];       //描述
	char   password[USERNAME_LEN];		    //密码
	char   groupname[USERNAME_LEN];		    //组名
	uchar  ismonitor;                       //组长否

/*	_DISPATCHER_STRU()
	{
		memset(this, 0, sizeof(_DISPATCHER_STRU));
	}

	//重载操作符 ">", ">=", "<".
	BOOL operator > (_DISPATCHER_STRU dsp) 
	{ 
		if(type>dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator >= (_DISPATCHER_STRU dsp) 
	{ 
		if(type>dsp.type || type == dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator < (_DISPATCHER_STRU dsp) 
	{ 
		if(type<dsp.type)
			return TRUE;
		return FALSE;
	}
	BOOL operator <= (_DISPATCHER_STRU dsp) 
	{ 
		if(type<dsp.type || type == dsp.type)
			return TRUE;
		return FALSE;
	}*/

}DISPATCHER_STRU;

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Dispatcher{
//		static  int  openf;
//		static  Rdb_Tab dispatchertab;
		int rcdno;
	public:
		Dispatcher(void);
		~Dispatcher(void);
		DISPATCHER_STRU  *dispatcherret;
		void GetDispatcher(void);
		int  GetDispatcherNum(void);
};

////////////////////////节点权限表////////////////////////////
typedef  struct {
	char	hostname[HOSTNAME_LEN];		//主机名
	char	username[USERNAME_LEN];		//用户名
	uchar   modifyjhcurve;              //修改计划曲线
	uchar   modifyrealdata;             //修改实时数据
	uchar   modifyhiscurve;             //修改历史曲线
	uchar   modifyhisent;               //修改历史事项
	uchar   modifygraph;                //修改图形
	uchar   modifynetpara;              //修改网络参数
	uchar   modifydbpara;               //修改数据库参数
	uchar   modifytable;                //修改报表
	uchar   viewjhcurve;                //查看计划曲线
	uchar   viewrealdata;               //查看实时数据
	uchar   viewhiscurve;               //查看历史曲线
	uchar   viewhisent;                 //查看历史事项
	uchar   viewgraph;                  //查看图形
	uchar   viewnetpara;                //查看网络参数
	uchar   viewdbpara;                 //查看数据库参数
	uchar   viewtable;                  //查看报表
	uchar   viewxbdata;                 //查看谐波数据
	uchar   viewlbdata;                 //查看录波数据
	uchar   viewdebuginfo;              //查看调试信息
	uchar   viewrawcode;                //查看通讯原码
	uchar   ykright;                    //遥控
	uchar   updownright;                //遥调
	uchar   superykright;               //超级遥控
	uchar   stopfresh;                  //停闪
	uchar   putcard;                    //挂牌
	uchar   entconfirm;                 //事项确认
	uchar   netchecktime;               //网络对时
	uchar   switchhost;                 //切换主机
	uchar   switchchannel;              //切换通道
	uchar   computerprot;               //微机保护
	uchar   modifyprotvalue;            //修改定值
	uchar   protrunstop;                //保护投退
	uchar   modifysetvalnum;            //修改定值区号
	uchar   protreserverd1;             //微机保护预留1
	uchar   protreserverd2;             //微机保护预留2
	uchar   protreserverd3;             //微机保护预留3
	uchar   stoprtu;                    //停运RTU
	uchar   controlmnp;                 //控制模拟盘
	uchar   lockforedata;               //封锁前置数据
	uchar   paradownload;               //参数下载
	uchar   paraupload;                 //参数上载
	uchar   guangzipai;                 //光字牌
	uchar	opright[MAXSECUREGRADE];	//操作权限：=1为有相应安全级操作权限
} USEROPERRIGHT_STRU;
  
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
UserOperRight{
		static  int  openf;
		static  Rdb_Tab useroprighttab;
		int recordno;
	public:
		UserOperRight(void);
		~UserOperRight(void);
		// 根据主机名、用户名检查有无指定安全级的操作权限
		int CheckSecureGrade(USEROPERRIGHT_STRU *uor, int securegrade);//校验用户安全级
		int CheckSecureGrade(char *hostname, char *username, int securegrade);
//		USEROPERRIGHT_STRU  *GetUserOperRight(char *hostname, char *username);
		USEROPERRIGHT_STRU  *useroperrightret;
		void GetOperright(void);
		int GetOperrightNum(void);
		int CheckUserPwd(char *username,char *password);//校验用户密码
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////节点权限校核 end/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////




int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetDesc(int groupno,char *tabname,char *fldname,char *desc,uchar *datatype,int *len);

int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetYkPara(char *name,int  *cmdtime);


int
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScdGetUpdownPara(char *name);

///////////////////////////////////
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ScadaServer {
     MailBox  scadamail;
public:
     ScadaServer(void);
     ~ScadaServer(void);
	 //HMI发往SCADA服务器的人工置数请求
     int  SendManSetValReq(int groupno,char *username,char *objectname,char *tablename,int setflag,uchar datatype,void *valaddr);
                 
	 //HMI发往SCADA服务器的遥控命令请求
	 //eflag为紧急遥控标志，eflag=1时不判断相关遥控闭锁条件
     int  SendYkCmdReq(int groupno,char *username,char *password,char *objectname,char *tablename,int cmdcode,int actcode,int eflag=0);
     
	 //FEP服务器发往SCADA服务器的遥控返校信息
     int  ForeSendYkCheckMesg(int groupno,char *username,int terminalno,int ykno,int chkokf);
     
	 //FEP服务器发往SCADA服务器的挂牌信息
     int  ForePutCard(int groupno,char *username,int terminalno,int yxno,int cardtype,int setflag);

	 //HMI发往SCADA服务器的升降命令请求
     int  SendUpdownCmdReq(int groupno,char *username,char *password,char *objname,int cmdcode,int actcode);
 
	 //HMI发往SCADA服务器的遥调(设点控制)命令请求
     int  SendYtCmdReq(int groupno,char *username,char *password,char *objname,int cmdcode,float setval);
	 
	 //SCADA服务器发往客户机的请求结果信息
     int  ScadaReturn(char *username,void **result,int *reslen);
	
	 //HMI发往SCADA服务器的群控命令 
	 int  SendGroupYkCmd(int groupno,char *username,char *password, char *groupname,int  objnum,  SCADA_YKOBJNAME_STRU *ykgrp);//群控
	 int  ctrl_groupyk(int groupno,char *username,char *password, char *groupname,int ctrlcmd);

	 //HMI发往SCADA服务器预定义序列控制
	 //serialtype:  0 -- 预定义序列   1--- 临时序列
	 //mode:        0 -- 连续执行     1--- 单步执行
	 //返回邮件类型: SCADA_TYPE_YKRESULT （遥控过程的信息）,  SCADA_TYPE_SERIALYKMSG （序列状态的信息）
     int  SendSerialYkCmd(int groupno,char *username,char *password, char *serialname, int serialtype,
					int mode,int objnums,SCADA_YKOBJNAME_STRU *serobjp);
	 int  ctrl_serialyk(int groupno,char *username,char *password,char *serialname, int ctrlcmd);

	 //HMI发往SCADA服务器的旁路替代命令请求  
     int  SendLineReplaceReq(int groupno,char *username, char *breakname, char *replaced_breakname,char mode);
     
	 //HMI发往SCADA服务器的启动追忆命令
     int  SendStartPdrCmd(int groupno,char *username, char *breakname, char zygroupno);
     
	 //HMI发往SCADA服务器的设置标志请求
	 int  SendSetSpecialFlagReq(int groupno,char *username, char setflag,CARD_RCD_STRU *cardrcdp);

	 //HMI发往SCADA服务器的删除确认事项列表请求
	 //clearf=1 删除单条确认事项; clearf=2 清空确认事项列表;
	 int  SendClearNotifyQueueReq(int groupno,char *username, char clearf, SYS_EVENT_EX *nqrcdp);

	 //HMI发往SCADA服务器的清除遥信变位标志请求	
	 // 入口参数:username:用户名 
	 //  substation 为站名默认为系统专用站,指整个系统的遥信变位标志均清除掉
	 //  否则只清除一个站的
	 int  SendClrChgFlagReq(int groupno,char *username, char *substation =SYS_SUBST_NAME);

	 //HMI发往SCADA服务器的清除警示变位标志请求	
	 // 入口参数:
     //		username:用户名 
	 //		mode:    0 : 清全系统,  1 : 清整个厂站  2 : 清指定对象
	 //		nums:  仅当mode=2时，表示对象个数,其它情况无效
	 //		name: 当mode=0时无效,
	 //				mode=1时表示站所线系代码,
	 //			    mode=2时为存放对象代码的字符数组,每个代码占OBJECTNAME_LEN长度
	 int SendClearAlarmFReq(int groupno,char *username,int mode, int nums,char *name);

	 	
	 //FEP服务器发往SCADA服务器的生事项
	 //int ForeSendRawEventToScd(char* username,SCADA_RAWEVENT_STRU *rawevent,int num);
       int ForeSendRawEventToScd(char* username,FE_DATA_EVENT_NODE *rawevent,int num);
	 //FEP服务器发往SCADA服务器的遥测越限生事项
	 int ForeSendOverLimitEventToScd(int groupno,char* username,SCADA_OVERLIMIT_STRU* oleventp);

	 //HMI发往SCADA服务器的封锁变位事项请求,isstation==1时,name为station的名字，
	 //isstation==0时，为特定对象的名字,setflag==1为设置封锁，==0为解除封锁
	 int SendMaskChgEventReq(int groupno,char *username,char *name,char isstation,char setf);
	 
	 //FEP服务器发往SCADA服务器的FTU故障报告
	 int ForeSendFtuFaultRpt(int groupno,char *username, SCADA_FTUFAULT_RPT_STRU *ftureport, char num);

	 //FEP服务器发往SCADA服务器的故障录波数据报告
	 int ForeSendFtuFaultRcd(int groupno,char* username,FAULT_TIMEANDNUM_STRU *rcdnum_time, FAULT_RCD_FROM_FORE_STRU *fturcd);

	 //HMI发往SCADA服务器的故障录波请求邮件 
	 int SendCallFaultRcdReq(int groupno,char *username, char *breakname, unsigned char  datatype);

	 //发往SCADA服务器的计算表达式编译请求邮件
	 int UserprocSendCompileReq(int groupno,char *username, char *tabname);

	 //FEP服务器发往SCADA服务器的微机保护信息邮件
	 int ForeSendProtInfo(int groupno,char *username, SCADA_PROT_STRU *protectp);

	 //FEP服务器或HMI发往SCADA服务器的五防闭锁请求
	 //no为五防闭锁参数表中的闭锁序号；
	 //wfbsflag为闭锁标志：WFBS_UNLOCK，解除闭锁，WFBS_LOCK，闭锁
	 //lockallflag为全闭锁标志：WFBS_LOCKSINGLE，只处理单条遥信的闭锁，WFBS_LOCKALL，根据wfbsflag闭锁或解除五防闭锁参数表中的全部遥信，用于与五防系统通讯初始化等特殊情况
	 int SendWfbsReq(int groupno, char* username, short no, unsigned char wfbsflag, unsigned char lockallf);

	 //光字牌确认
	 //tabname为光字牌确认的表名；objname为光字牌确认的对象名；clearflag为光字牌确认标志:1,清除光字牌确认标志
//	 int SendClrGzpReq(int groupno, char* username, char* tabname,char* objname, unsigned char clearflag);

	 //发往SCADA服务器的输入量
	 int NetSendYcData(int groupno,char *username,char *password,int nums,SCADA_NET_YCDATA_STRU *ycdatap);
	 int NetSendYxData(int groupno,char *username,char *password,int nums,SCADA_NET_YXDATA_STRU *ycdatap);
	 int NetSendKwhData(int groupno,char *username,char *password,int nums,SCADA_NET_KWHDATA_STRU *ycdatap);
     };


#endif
