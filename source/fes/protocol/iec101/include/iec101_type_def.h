/**
@file iec101_type_def.h
@brief 101规约数据类型定义头文件

@author chenkai 
@version 1.0.0
@date 2015-04-07
*/

#ifndef IEC101_TYPE_DEF_H_
#define IEC101_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"

/**
@brief 报文常用字节
*/
enum DATA_CON
{
	FIXED_FRAME_HEAD = 0x10,  //固定帧头
	FRAME_TAIL = 0x16, //结束符
	UNFIXED_FRAME_HEAD = 0x68 , //可变帧头
	YK_CONFIRM_HEAD = 0xE5 ,//遥控链路层确认帧
	STATE_SP_ON = 1,  //单点合位
	STATE_SP_OFF = 0,  //单点分位
	STATE_DP_ON = 0X02,  //双点合位
	STATE_DP_OFF = 0X01,  //双点分位
};
/**
@brief IEC101类型标识

只列出了常用的，可根据需要和规约文档增加
*/
enum TYPE_IDENTIFICATION
{
	M_SP_NA_1 = 0x01,	///<单点信息
	M_SP_TA_1 = 0x02,	///<带时标的单点信息(短时标SOE)
	M_DP_NA_1 = 0x03,	///<双点信息
	M_DP_TA_1 = 0x04,	///<带时标的双点信息(短时标SOE)

	M_ST_NA_1 = 0x05,	///<步位置信息（变压器分接头）
	M_ST_TA_1 = 0x06,	///<带时标的步位置信息（变压器分接头）
	M_BO_NA_1 = 0x07,	///<32比特串
	M_BO_TA_1 = 0x08 , ///<带时标的32比特串

	M_ME_NA_1 = 0x09,	///<测量值，规一化值
	M_ME_TA_1 = 0x0a,	///<带时标的测量值
	M_ME_NB_1 = 0x0b,	///<测量值，标度化值
	M_ME_TB_1 = 0x0c, ///<测量值, 带时标的标度化值
	M_ME_NC_1 = 0x0d,	///<测量值，短浮点值
	M_ME_TC_1 = 0x0e, ///<测量值, 带时标的短浮点数
	M_IT_NA_1 = 0x0f,	///<累计量
	M_IT_TA_1 = 0x10,	///<带时标的累计量

	M_EP_TA_1 = 0x11, ///<带时标的继电保护设备事件
	M_EP_TB_1 = 0x12, ///<带时标的继电保护设备成组启动事件
	M_EP_TC_1 = 0x13, ///<带时标的继电保护设备成组输出电路信息
	M_PS_NA_1 = 0x14,	///<带变位检出的成组单点信息
	M_ME_ND_1 = 0x15,	///<不带品质描述的规一化测量值

	M_SP_TB_1 = 0x1e,	///<带CP56Time2a 时标的单点信息
	M_DP_TB_1 = 0x1f,	///<带时标CP56Time2a的双点信息
	M_ST_TB_1 = 0x20,   ///<带CP56Time2a时标的步位置信息
	M_BO_TB_1  = 0x21, ///<带CP56Time2a时标的32比特串
	M_ME_TD_1 = 0x22,	///<带时标CP56Time2a的的测量值，规一化值
	M_ME_TE_1 = 0x23,	///<带时标CP56Time2a的的测量值，标度化值
	M_ME_TF_1 = 0x24,	///<带时标CP56Time2a的的测量值，短浮点数
	M_IT_TB_1 = 0x25,	    ///<带时标CP56Time2a的累计量
	M_EP_TD_1 = 0x26,	///<带CP56Time2a时标的继电保护设备事件
	M_EP_TE_1 = 0x27,	///<带CP56Time2a时标的继电保护设备成组启动事件
	M_EP_TF_1 = 0x28,	///<带CP56Time2a时标的继电保护设备成组输出电路信息

	C_SC_NA_1 = 0x2d,	///<单点命令
	C_DC_NA_1 = 0x2e,	///<双点命令
	C_RC_NA_1 = 0x2f,	///<升降命令
	C_SE_NA_1 = 0x30,	///<设定值命令, 规一化值
	C_SE_NB_1 = 0x31,	///<设定值命令, 标度化值
	C_SE_NC_1 = 0x32,	///<设定值命令, 短浮点数
	C_BO_NA_1 = 0x33,	///<32比特串

	M_EI_NA_1 = 0x46,	///<初始化结束

	C_IC_NA_1 = 0x64,	///<总召唤命令
	C_CI_NA_1 =	0x65,	///<计数量召唤命令
	C_RD_NA_1 =	0x66,	///<读命令
	C_CS_NA_1 = 0x67,	///<时钟同步命令
	C_TS_NA_1 = 0x68,	///<测试命今
	C_RP_NA_1 =	0x69,	///<复位进程命令
	C_CD_NA_1 = 0x6a,	///<延时获得命今

	P_ME_NA_1 = 0x6e,	///<测量值参数, 规一化值
	P_ME_NB_1 = 0x6f,	///<测量值参数, 标度化值
	P_ME_NC_1 =	0x70,	///<测量值参数, 短浮点数
	P_AC_NA_1 = 0x71,	///<参数激活

	F_FR_NA_1 =	0x78,	///<文件淮备就绪
	F_SR_NA_1 =	0x79,	///<节淮备就绪
	F_SC_NA_1  = 0x7a,	///<召唤目录, 选择文件, 召唤文件，召唤节
	F_LS_NA_1 = 0x7b,		///<最后的节,最后的段
	F_AF_NA_1 =	0x7c,	///<认可文件,认可节
	F_SG_NA_1 = 0x7d,	///<段
	F_DR_TA_1 = 0x7e		///<目录
};

//启动方向链路功能码
enum M_LINK_FUNCCODE
{
	RESET = 0 ,	//复位远方链路
	RESETUSER = 1 ,	//复位用户进程
	TESTLINK = 2 ,	//
	SENDCONFIRM = 3 ,	//发送/确认用户数据
	SENDNOCONFIRM = 4 ,	//发送/无回答用户数据
	REQUESTDATA = 8 , //访问请求
	REQUESTLINKSTATUS = 9 ,	//召唤链路状态
	REQUESTONEDATA = 10 ,	//召唤一级数据
	REQUESTTWODATA = 11 	//召唤二级数据
};

//从动方向链路功能码
enum S_LINK_FUNCCODE
{
	ACK = 0,
	NACK = 1 ,	
	RESPONSE101 = 8 ,	
	NODATA = 9	,
	LINKNOWORKING = 14 ,	//链路服务未工作
	LINKREDAY = 11 ,	//链路完好
	LINKNOEND = 15 ,	//链路服务未完成
};

typedef	struct
{
	unsigned char		FC ;		//功能码
	unsigned char		FCV;		//计数有效位(1表示变化有效)
	unsigned char		FCB;		//帧计数位
	unsigned char		PRM;		//启动报文位(启动为1; 从动为0)
	unsigned char		DIR;		//传输方向位(主->子 0)
}ControlField;

/**
@brief 传送原因
*/
enum COT_TYPE
{
	COT_PERCYC	     = 1,   //周期/循环
	COT_BACK         = 2,  //背景扫描
	COT_SPONT        = 3,  //突发
	COT_INIT         = 4,  //初始化
	COT_REQ		     = 5,  //请求或被请求
	COT_ACT          = 6,  //激活
	COT_ACTCON       = 7,  //激活确认
	COT_DEACT        = 8,  //停止激活
	COT_DEACTCON     = 9,  //停止激活确认
	COT_ACTTERM      = 10, //激活结束
	COT_RETREM		 = 11, //远程命令引起的返送信息
	COT_RETLOC		 = 12, //当地命令引起的返送信息
	COT_FILE		 = 13, //文件传送

	COT_INTROGEN     = 20, //总召唤
	COT_PIINTROGEN   =37,  //计量总召唤

	COT_UNKOWN_TYPE  = 44,
	COT_UNKOWN_COT   = 45,
	COT_UNKOWN_ADDR  = 46,
	COT_UNKOWN_INDEX = 47
};

//召唤组定义
#define				CALLTOTAL				20			//站总召唤
#define				ALLDATAACK				10			//总召唤确认
#define				CALLKWHTOTAL			5	//			//电度总召唤
#define				BROADCASTCALL       3 //广播召唤1级数据

enum SIZE_101///< 101规约中长度定义
{
	MAX_BUF_SIZE = 255,///<缓冲区长度
	UNFIXED_DATA_LEN = 6,///<可变帧长报文需添加的长度
	FIXED_FRAME_LEN = 5, ///<固定帧长
	YK_CONFIRM_LEN = 1 ,///<遥控链路层确认帧长
};

enum FRAME_TYPE///<帧类别
{
	INVALID_FRAME = -1,///<非法帧
	VALID_FRAME = 0,///<非法帧
	FIXED_FRAME = 1 ,///<固定帧
	UNFIXED_FRAME = 2 ,///<可变帧
	YKCONFIRM_FRAME = 3 ,///<遥控确认帧
};

enum DEAL_ERR_TYPE
{
	DEFAULT_ERROR = -1 ,///<默认错误
	DEAL_SUCCESS = 0 ,///<处理成功
};

#pragma pack(1)
/**
@brief 报文中的短时标时间格式
*/
struct CP24Time2a		
{
	int16u millsecond;
	unsigned char minutes:6;
	unsigned char res:1;
	unsigned char iv:1;
};

/**
@brief 报文中的长时标时间格式
*/
struct CP56TIME
{
	int16u millsecond;
	unsigned char minute:6;
	unsigned char RES1:1;
	unsigned char IV:1;	
	unsigned char hour:5;
	unsigned char RES2:2;
	unsigned char SU:1;
	unsigned char day:5;
	unsigned char wday:3;
	unsigned char month:4;
	unsigned char RES3:4;
	unsigned char year:7;
	unsigned char RES4:1;
};

/**
@brief 遥控命令DCO
*/
struct YKDCO
{
	unsigned char dco_se;		//选择/执行
	unsigned char dco_qu;		
	unsigned char dco_dcs;		//双点遥控状态
};

/**
@brief 对时结构
*/
struct SYNC_TIME_STRUCT
{
	CP56TIME cp56time;
};

/**
@brief 101信息体地址结构
*/
struct INFO_ADDR
{
	int16u addr1;
	int8u addr2;
};

/**
@brief YX结构
*/
struct SEQ_M_NA_1
{
	int16u addr1;
	int8u addr2;
	unsigned char yx_value;
};

/**
@brief M_PS_NA_1YX结构
*/
struct SEQ_M_PS_NA_1
{
	int16u value;
	int16u statuscheck;
	unsigned char quanity;
};

/**
@brief 短时标yxsoe结构
*/
struct	SEQ_M_TA_1
{
	INFO_ADDR	info_addr;
	unsigned char value;
	CP24Time2a short_time;
};

/**
@brief 长时标yxsoe结构
*/
struct SEQ_M_TB_1
{
	INFO_ADDR	info_addr;
	unsigned char value;
	CP56TIME long_time;
};

/**
@brief M_IT_NA_1电度结构
*/
struct SEQ_M_IT_NA_1
{
	int32s value;
	unsigned char quanity;
};

/**
@brief M_ME_NA_1遥测结构
*/
struct SEQ_M_ME_NA_1
{
	int16s value;
	unsigned char quanity;
};

/**
@brief M_ME_NC_1遥测结构
*/
struct SEQ_M_ME_NC_1
{
	floats value;
	unsigned char quanity;
};

/**
@brief M_ME_TA_1遥测结构
*/
struct SEQ_M_ME_TA_1
{
	INFO_ADDR	info_addr;
	int16s value;
	unsigned char quanity;
	CP24Time2a short_time;
};

#pragma pack()


/**
@brief IEC101配置项结构
*/
struct IEC101_CONFIG
{
	unsigned short  yx_type;	//遥信类型
	unsigned short soe_type;	//SOE类型
	unsigned short  yc_type;	//遥测类型
	unsigned short  kwh_type; //电度类型

	unsigned short  yk_type;	//遥控类型，单点或双点


	short call_all_data_gap;///<总召间隔，为-1时不总召
	short call_kwh_gap;		///<召唤电度间隔，为-1时不召唤
	short send_time_gap;	///<对时间隔，为-1时不对时

	unsigned short yx_begin;	///<遥信起始地址
	unsigned short yc_begin;	///<遥测起始地址
	unsigned short yk_begin;	///<遥控起始地址
	unsigned short kwh_begin;	///<电度起始地址

	unsigned short t0;		///<t0超时，建立连接超时
	unsigned short trp;

	/**
	@brief 参数置默认值
	*/
	void set_default(void)
	{
		yx_type = M_SP_NA_1;
		soe_type = M_SP_TB_1;
		yc_type = M_ME_NC_1;
		kwh_type = M_IT_NA_1;
		yk_type = C_DC_NA_1;

		call_all_data_gap = 900;//总召间隔默认值900秒
		call_kwh_gap = 1200;	//召唤电度间隔默认值1200秒
		send_time_gap = 300;	//对时间隔300秒

		yx_begin = 0x0001;
		yc_begin = 0x4001;
		kwh_begin = 0x6401;
		yk_begin = 0x6001;

		//以下超时为南网要求，秒级单位
		t0 = 30;

	};

	/**
	@brief IEC101配置项结构构造函数
	*/
	IEC101_CONFIG()
	{
		set_default();
	};
};

#endif