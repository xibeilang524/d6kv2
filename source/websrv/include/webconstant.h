/*
 * --------------------------------------------------
 *   文件名：webconstant.h
 *   软件包：DS3210配网监控系统Web服务器   
 *   文件主要内容：WEB服务器守护线程
 *
 *   Copyright 国电南自配网事业部, 2002
 *   版本号：v1.0.0,	2002/06/03
 *					 
 *   设计：蒋宏图
 *   说明：
 * --------------------------------------------------
 */


#if !defined(WEBCONSTANT)
#define WEBCONSTANT

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "db/dbapi.h"

#define		WEBPORT			7200	//服务器端口号
#define		MAXBUFSIZE		80*1024   /*规模限制在64K debug*/
#define		WEBNODENAMELEN	40		/*计算机节点名称长度*/
#define		INITCLIENTNUMS	100		//初始的客户端数量
#define		CLIENTSTOPTIME	90		//50		//客户端退出时间，秒
#define		CODESIZE		256		//代码长度
#define		MSGHEADLEN		12		/*MESSAGE头的长度*/
#define     DEVTYPENUM		32     //设备类型数
#define		INVALIDF		-1	//THREADMANAGE的validf无效

/*Win32 begin*/
#ifdef WIN32
	#define		EWOULDBLOCK		WSAEWOULDBLOCK
	#define		EISCONN			WSAEISCONN
	#define		EINPROGRESS		WSAEINPROGRESS

	#ifndef		EINTR
		#define		EINTR		WSAEINTR
	#endif

#endif
/*Win32 end*/

//定义历史表是否为日表
//#define		USE_YC_DAYTABLE  


//C/S通信报文类型定义
#define		S_SUCCESS		8			//注册成功
#define		S_FAILLOG		9			//客户登录失败
#define		MSGTYPE			10			//帧类型
#define     S_STREAM		MSGTYPE+0   //流处理
#define		S_HISEVENT		MSGTYPE+2	//历史事项
#define		S_REALEVENT		MSGTYPE+3	//实时事项报文
#define		S_DEVPARA		MSGTYPE+4	//设备参数
#define     S_TOPOLOGY      MSGTYPE+8   //拓扑结构表（实时库）


#define     S_GROUPTABLE    20			//组参数表
#define		S_RTUNAME		MSGTYPE+11	//终端参数表
#define		S_YCCODE		22			//遥测代码
#define     S_CURVE         23			//曲线数据(曲线查看器)
#define		S_DDCODE        24			//电度代码
#define		S_CARDRCD		25			//标志牌记录表
#define 	S_REALCURVE		26			//实时曲线类型数据
#define		S_FILE			MSGTYPE+20	//传文件
#define		S_ALLFILE		MSGTYPE+21	//传全部文件
#define		S_USER			50			//传用户表
#define		S_GRAPHGROUP    51			//web图形分组表
#define     S_USERRIGHT		52			//web权限参数表
#define     S_TOPOCOLOR     53			//topocolor表
#define     S_STATION	    54          //站所线系参数表
#define     S_ANALOGVIEW    55          //模拟量视图

#define		S_TEST			66			//测试报文
#define		S_ACK			99			//客户端(socket)平安报文
#define		S_TYPE			100			//客户端类型
#define		S_SRVEXIT		199			//服务器关闭
#define     S_ERROR			200       	//网络错误


#define		DAYYCCURVE		1			//日遥测曲线
#define		MONTHYCCURVE	2			//月遥测曲线
#define		DAYKWHCURVE		10			//日电度曲线
#define		MONTHKWHCURVE	11			//月电度曲线
#define     REALYCCURVE		0			//实时遥测曲线
#define     REALKWHCURVE	1			//实时电度曲线



#define		USERNAME		"600268"//"scada"		//实时库访问用户名
#define		PASSWORD		""//"scada"		//实时库访问用户名
#define		BASEWINNO		INITCLIENTNUMS  //构造流时，winno = BASEWINNO*threadno +winno,=INITCLIENTNUMS
#define     WAIT_TIME       40		 	//停止流后，等待时间。等待流数据处理线程，以免内存冲突,可以改成20

#define		WEBSCADA		1			//用户类型是webscada
#define		GISTOWEB		10			//gis用户
#define		WEBTABLE		100			//web table


#define		WEBVISITTABLE   "web图形访问记录表"
#define     WEBUPDATETABLE  "web客户更新文件表"
#define		YCPARATABLE		"遥测参数表"	//遥测参数表
#define     TOPOTABLE       "SCADA静态拓扑结构记录表"
#define     TOPOCOLORTABLE  "topocolor"


#define		CARDRCD			"cardrcd"	//标志牌记录表
#define	    STATION			"station"	//站所线系参数表
#define     TOPO            "scdtp"     //SCADA静态拓扑结构记录表

#define		MAXLOOPNUMS      100000		//最大循环数

#define MAXSQLWORDLEN		1024
#define MAXFIELDWIDTH		256//Sybase最大域宽度

 


//电力设备类型号,由调度员界面定义
#define DevNULL			0	//--非电力设备
#define DevBreak		1	//--开关
#define DevSwitch		2	//--刀闸
#define DevMelt			3	//--熔断器
#define DevHanbrk		4	//--手车
#define DevMTran		5	//--主变压器
#define DevDTran		6	//--配电变压器
#define DevCap			7	//--电容器
#define DevReact		8	//--电抗
#define DevLoop			9	//--消弧线圈
#define DevPT			10	//--PT
#define DevCT			11	//--CT
#define DevArres		12	//--避雷器
#define DevGround		13	//--接地线
#define DevPowln		14	//--架空线
#define DevCable		15	//--电缆
#define DevBus			16	//--母线
#define DevGener		17	//--发电机
#define DevWeld			18	//--焊点
#define DevLnkln		19	//--连接线
#define DevJoint		20	//--端子
#define DevMaxType		21	//--电力设备最大类型号


#define FREE(p) if(p) { \
				 free(p); \
				 p = NULL;	\
				}

#define DELETEP(p) if(p) { \
				 delete (p); \
				 p = NULL;	\
				}


#ifndef WIN32
	#include <assert.h>
	#define ASSERT assert

	#define FALSE   0
	#define TRUE    1
#ifndef _AIX
	#define NULL    0
#endif
	typedef unsigned char byte;
	typedef unsigned int  UINT;
	typedef  void *(*PTHREAD_CALLFUNC)(void *);
#endif


#define TOPU_MAX_COUNT						512                   //MAX COUNT 
#define CHAR_BUF_LEN_DEF					CODE_LEN



#define  DEV_MEAS_TABNAME                    "analog"
#define  DEV_BREAK_TABNAME                   "break"
#define  DEV_SWITCH_TABNAME                  "switch" 
	
	
	
#define DEV_FRAME_STYLE_HYPERLINK           1
#define DEV_FRAME_STYLE_DEVPROP             2
	
#define DEV_LINE_HYPERLINK                  "LineHyperlink"
	
	
#define DEV_MEAS							"measlayer"
	//开关
#define DEV_BREAK							"breakerlayer"    
#define DEV_SWITCH	    					"switchlayer"
#define DEV_LINE							"aclinelayer"
	
#define DEV_BUS								"buslayer"
#define DEV_TRANSFORMER3LAYER	            "transformer3layer"
#define DEV_SUBTRANSFORMER					"transformer2layer"
	
#define DEV_MLBREAK							"mcblayer"
#define DEV_LOAD							"loadslayer"
#define DEV_CAPACITOR						"capacitorlayer"
	
#define DEV_COIL							"tgrlayer"
#define DEV_GNDSWITCH						"tgslayer"
	
#define HYPERLINK_MAX_FILENAME              10
#define CHAR_BUF_LEN						255
	//编码后缀定义
#define BREAK_POSTFIX						"Kg"
#define SWITCH_POSTFIX						"Dz"


#endif // !defined(WEBCONSTANT)

