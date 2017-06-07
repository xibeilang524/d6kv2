/**
@file iec104_type_def.h
@brief 加密104规约数据类型定义头文件

@author chenkai 
@version 1.0.0
@date 2015-11-29
*/

#ifndef ENCRY_IEC104_TYPE_DEF_H_
#define ENCRY_IEC104_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief 加密IEC104类型标识

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
	M_BO_NA_1 = 0x07,	///<子站远动终端状态

	M_ME_NA_1 = 0x09,	///<测量值，规一化值
	M_ME_TA_1 = 0x0a,	///<带时标的测量值
	M_ME_NB_1 = 0x0b,	///<测量值，标度化值
	M_ME_NC_1 = 0x0d,	///<测量值，短浮点值
	M_IT_NA_1 = 0x0f,	///<累积量
	M_IT_TA_1 = 0x10,	///<带时标的电能脉冲

	M_PS_NA_1 = 0x14,	///<带状态检出的成组单点信息
	M_ME_ND_1 = 0x15,	///<不带品质描述的规一化测量值

	M_SP_TB_1 = 0x1e,	///<带CP56Time2a 时标的单点信息
	M_DP_TB_1 = 0x1f,	///<带时标CP56Time2a的双点信息
	M_ME_TD_1 = 0x22,	///<带时标CP56Time2a的的测量值，规一化值
	M_ME_TE_1 = 0x23,	///<带时标CP56Time2a的的测量值，标度化值
	M_ME_TF_1 = 0x24,	///<带时标CP56Time2a的的测量值，短浮点数
	M_IT_TB_1 = 0x25,	///<带时标CP56Time2a的累计量

	C_SC_NA_1 = 0x2d,	///<单点命令
	C_DC_NA_1 = 0x2e,	///<双点命令
	C_RC_NA_1 = 0x2f,	///<升降命令

	C_IC_NA_1 = 0x64,	///<总召唤命令
	C_CI_NA_1 =	0x65,	///<累计量召唤命令
	C_CS_NA_1 = 0x67	///<时钟同步命令
};

/**
@brief 各种数据在报文中的位置
*/
enum BYTE_POS_DEF
{
	START_ID = 0,

	NS       = 2,
	NS_L     = 2,
	NS_H     = 3,
	NR       = 4,
	NR_L     = 4,
	NR_H     = 5,

	APDU_ID  = 6,
	NUM      = 7,

	REASON   = 8,
	REASON_L = 8,
	REASON_H = 9,
	ADDR     = 10,
	ADDR_L   = 10,
	ADDR_H   = 11,

	INDEX_NO = 12,
	INDEX_L  = 12,
	INDEX_M  = 13,
	INDEX_H  = 14,
	

	YK_FRAME_LENTH = 0x0e,

	ACTIVE_CON = 14,
	VALUE    = 14,

	SYNC_VALUE = 20		
};

/**
@brief 数据长度
*/
enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 2,///<公共地址长度
	COT_LEN = 2,		///<传送原因长度
	INDEX_LEN = 3,		///<信息体地址长度
	CTL_FLD = 4,		///<控制域长度
	SINGLE_CHGYX_APDU_LEN = 14,  ///<单遥信变位帧长度
	HEAD_LEN = 10,		///<报文开始到信息体的长度
	ASDU_HEAD_LEN = 12, ///<从报文头到ASDU的长度
	APDU_HEAD_LEN = 13, ///<APDU报文从控制域到信息体数值部分长度
	INFO_BEGIN_LEN = 15, //<从报文头到信息体数值部分长度
	ASDU_DATA_LEN = 256	///<ASDU信息体长度，无特别意义，信息体会交由分析函数处理
};

/**
@brief 报文常用字节
*/
enum DATA_CON
{
	FIRST = 0X68,   //<IEC104报文头
	ENCRY_FIRST = 0X16,   //<加密IEC104报文头
	U_LEN = 0X04,   //<U帧固定长度
	S_LEN = 0X04,    //<S帧固定长度
	VSQ_SQ = 0x80,   //<VSQ
	VSQ_SINGLE = 1,
	NEGATE = 1,    //取反标志
	STATE_SP_ON = 1,  //单点合位
	STATE_SP_OFF = 0,  //单点分位
	STATE_DP_ON = 0X02,  //双点合位
	STATE_DP_OFF = 0X01,  //双点合位
	QDI_GENERAL_CALL = 0x14  //总召唤限定词QDI
};

/**
@brief S帧
*/
enum U_TYPE
{
	STARTDT_ACT   = 0x04|0x03,    // 0x07
	STARTDT_CON   = 0x08|0x03,    // 0x0b
	STOPDT_ACT    = 0x10|0x03,    // 0x13
	STOPDT_CON    = 0x20|0x03,    // 0x23
	TESTFR_ACT    = 0x40|0x03,    // 0x43
	TESTFR_CON    = 0x80|0x03     // 0x83
};

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

/**
@brief 数据质量
*/
enum QUALITY_TYPE
{
	DATA_OVERLOAD = 0x01,    // 溢出    0:未溢出    1:溢出
	DATA_BLOCKADE = 0x10,    // 封锁    0:未被封锁  1:被封锁
	DATA_REPLACE  = 0x20,    // 取代    0:未被取代  1:被取代
	DATA_CURRENT  = 0x40,    // 当前值  0:当前值    1:非当前值
	DATA_INVALID  = 0x80     // 有效    0:有效      1:无效
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
@brief ASDU中的信息体，按照字节流处理
*/
struct    DATA_STRUCT
{
	unsigned char	buf[ASDU_DATA_LEN];
};

/**
@brief 发送、接收序号结构
*/
union NSR_FORMAT
{
	int16u num;
};

/**
@brief I帧时的数据头
*/
struct I_FORMAT    // I : Information Transmit Format 编号的信息传输格式
{
	int16u ns;   //发送序列号
	int16u nr;   //接收序列号
};

/**
@brief S帧时的数据头
*/
struct S_FORMAT    // S : Numbered Supervisory Function 编号的监视功能格式
{
	unsigned char  s_1;
	unsigned char  s_2;
	int16u nr;
};

/**
@brief U帧时的数据头
*/
struct U_FORMAT    // U : Unnumbered Control Function 不编号的控制功能格式
{
	unsigned char  u_type;
	unsigned char  u_2;
	unsigned char  u_3;
	unsigned char  u_4;
};


/**
@brief 总召唤/遥控结构
*/
struct C_STRUCT
{

	int16u  index_no;	///<信息体地址
	unsigned char pos_holder;	///<104规定的
	unsigned char qdi;
};

/**
@brief 对时结构
*/
struct SYNC_TIME_STRUCT
{
	int16u  index_no;
	unsigned char	pos_holder;
	CP56TIME cp56time;
};


/**
@brief APCI : Application Protocol Control Information 应用规约控制信息
*/
struct P104_APCI  
{
	unsigned char    start_id;    // buf[0] 启动字符
	unsigned char    apdu_len;    // buf[1] APDU的长度

	union				 // buf[2-5] 控制域
	{
		I_FORMAT type_i;
		S_FORMAT type_s;
		U_FORMAT type_u;
		unsigned char    buf[CTL_FLD];
	};
};

/**
@brief ASDU : Application Service Data Unit 应用服务数据单元
*/
struct P104_ASDU
{
	unsigned char    type;        // buf[6] type identification 类型标识
	unsigned char    vsq;		 // buf[7] variable structure qualifier 可变结构限定词

	int16u    cot;       // buf[8-9] cause of transmission 传送原因
	int16u   addr;       // buf[10-11] common address 应用服务数据单元公共地址

	union
	{
		C_STRUCT cs;
		SYNC_TIME_STRUCT sts;
		DATA_STRUCT    data;
	};
};

/**
@brief 104信息体地址结构
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

/**
@brief APDU : Application Protocol Data Unit 应用规约数据单元
*/
typedef struct P104_APDU  
{
	P104_APCI    apci;  //6字节
	P104_ASDU    asdu;  //249字节最多
};

#pragma pack()


/**
@brief ENCRY_IEC104配置项结构
*/
struct ENCRY_IEC104_CONFIG
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
	unsigned short t1;		///<t1超时时间，发送一个I帧或U帧后，必须在t1时间内接收到对方的确认，否则认为连接有问题，需要关闭重新连接
	unsigned short t2;		///<t2超时时间，接收一个I帧后，在t2时间内未接收到新的I帧，发送S帧进行确认
	unsigned short t3;		///<t3超时时间，接收到对方一帧报文后，经过t3时间没有接收到新的报文，需要向对方发送U帧测试

	unsigned short test_fr_cnt;	//未回复的测试帧限值
	unsigned short k;		///k值，有k个I帧未得到对方S帧确认时，停止传输，取值范围1-32767
	unsigned short w;		///<w值，最迟接收到对方w个I帧时，向对方发送S帧，取值范围1-32767

	bool check_ns_nr;		///<检查发送接收序号，序号错乱需要关闭连接，1为检查，0为不检查
	bool chg_to_soe;		///<遥信转soe，1为SOE转遥控，0为不转 
	unsigned char soe_to_chg;	///<soe转遥信，1为SOE转遥控，0为不转
	unsigned char grp_yc_check_chg_flag;	///<成组单点遥信检查变位标志，1为检查，0为不检查

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
		t1 = 15;
		t2 = 10;
		t3 = 20;

		test_fr_cnt = 3;
		k = 12;
		w = 8;

		check_ns_nr = 0;
		chg_to_soe = 0;
		soe_to_chg = 0;
		grp_yc_check_chg_flag = 0;

	};

	/**
	@brief IEC104配置项结构构造函数
	*/
	ENCRY_IEC104_CONFIG()
	{
		set_default();
	};
};

#endif