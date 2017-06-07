#ifndef PROTOCOL_IEC104_H_
#define PROTOCOL_IEC104_H_
#include "msgview_string.h"


#pragma once
#define FIRST       0x68
#define S_LEN        4
//#define BYTE unsigned char

#ifndef BYTE
typedef unsigned char BYTE; 
#endif


/********************************************以下为IEC104规约部分********************************************************/

static QString remind_first_error =  _C2Q("(报文头不正确) ");
static QString remind_first = _C2Q("(报文头) ");
static QString remind_second_error =  _C2Q("(报文长度不正确) ");
static QString remind_len =  _C2Q("(报文长度:");
static QString remind_second_type_s =  _C2Q("(帧类型: S帧) ");
static QString remind_second_type_i =  _C2Q("(帧类型: I帧) ");
static QString remind_second_type_u =  _C2Q("(帧类型: U帧 ");
static QString remind_control_s=  _C2Q("(S帧控制域) ");
static QString remind_control_u =  _C2Q("(U帧控制域) ");
static QString remind_start_act =  _C2Q("启动激活) ");
static QString remind_start_con=  _C2Q("激活确认) ");
static QString remind_stop_act =  _C2Q("停止激活) ");
static QString remind_stop_con=  _C2Q("停止确认) ");
static QString remind_test_act =  _C2Q("测试激活) ");
static QString remind_test_con =  _C2Q("测试确认) ");

static QString remind_send_no =  _C2Q("发送序列号:");
static QString remind_recv_no =  _C2Q("接收序列号:");

static QString remind_tab =  "\r";
static QString remind_space = " ";
static QString remind_leftkuohao = "(";
static QString remind_rightkuohao = ")";

static QString remind_type = _C2Q("类型值:");
static QString remind_call_limit = _C2Q("召唤限定词(QOI):");
static QString remind_num =  _C2Q("信息数目:");
static QString remind_nonsequenced =  _C2Q("非顺序");
static QString remind_sequenced =  _C2Q("顺序");
static QString remind_transmit_reason =  _C2Q("传送原因:");
static QString remind_transmit_reason_burst_3 =  _C2Q("突发");
static QString remind_transmit_reason_activation_6 =  _C2Q("激活");
static QString remind_transmit_reason_activation_confirm_7 =  _C2Q("激活确认");

static QString remind_transmit_reason_activation_stop_8 =  _C2Q("停止激活");
static QString remind_transmit_reason_activation_stop_confirm_9 =  _C2Q("停止激活确认");
static QString remind_transmit_reason_activation_teminate_10 =  _C2Q("激活终止");
static QString remind_transmit_reason_respond_station_call_20 =  _C2Q("响应站召唤");
static QString remind_respond_station_call_20 =  _C2Q("响应站召唤");
static QString remind_transmit_addr =  _C2Q("公共地址:");
static QString remind_monitor = _C2Q("监视方向");
static QString remind_no_time_single_signal =  _C2Q("不带时标单点遥信");
static QString remind_time_single_signal =  _C2Q("带时标单点遥信");
static QString remind_siq = _C2Q("带品质描述词的单点信息(SIQ):");
static QString remind_open = _C2Q("分位");
static QString remind_close = _C2Q("合位");

static QString remind_no_time_double_signal =  _C2Q("不带时标双点遥信");
static QString remind_time_double_signal =  _C2Q("带时标双点遥信");
static QString remind_diq =  _C2Q("带品质描述词的双点信息(DIQ)：");
static QString remind_open_double = _C2Q("分位");
static QString remind_close_double = _C2Q("合位");
static QString remind_uncertainty_double = _C2Q("不确定或中间状态");

static QString remind_single_command = _C2Q("单命令");
static QString remind_sco = _C2Q("单命令(SCO):");
static QString remind_no_other = _C2Q("无另外的定义");
static QString remind_short_pulse = _C2Q("短脉冲持续时间");
static QString remind_long_pulse = _C2Q("长脉冲持续时间");
static QString remind_sustained_output = _C2Q("持续输出");
static QString remind_reserve =_C2Q("保留");

static QString remind_double_command = _C2Q("双命令");
static QString remind_dco = _C2Q("双命令(DCO) ");
static QString remind_no_allowed = _C2Q("不允许");
static QString remind_open_ctl = _C2Q("控分");
static QString remind_close_ctl = _C2Q("控合");

static QString remind_time_single_command = _C2Q("带时标的单命令");

static QString remind_time_double_command = _C2Q("带时标的双命令");


static QString remind_no_time_measuring =  _C2Q("不带时标不带品质描述的归一化测量值");

static QString remind_time_quality_measuring = _C2Q("带时标带品质描述的归一化测量值");
static QString remind_measuring =  _C2Q("测量值");
static QString remind_qds =  _C2Q("品质描述词(QDS) ");
static QString remind_no_overflow =  _C2Q("未溢出");
static QString remind_overflow =  _C2Q("溢出");
static QString remind_unblock =  _C2Q("未被封锁");
static QString remind_block =  _C2Q("被封锁");
static QString remind_no_replace =  _C2Q("未被取代");
static QString remind_replace =  _C2Q("被取代");
static QString remind_current_value =  _C2Q("当前值");
static QString remind_no_current_value =  _C2Q("非当前值");
static QString remind_valid =  _C2Q("有效");
static QString remind_invalid =  _C2Q("无效");


static QString remind_no_time_control_measuring =  _C2Q("设点命令，归一化值");
static QString remind_qos =  _C2Q("设定命令限定词(QOS) ");
static QString remind_execute = _C2Q("执行");
static QString remind_select = _C2Q("选择");

static QString remind_time_control_measuring =  _C2Q("带时标的设点命令，归一化值");


static QString remind_initialization = _C2Q("初始化结束");
static QString remind_coi = _C2Q("初始化原因(COI) ");
static QString remind_coi_0 = _C2Q("当地电源合上");
static QString remind_coi_1 = _C2Q("当地手动复位");
static QString remind_coi_2 = _C2Q("远方复位");
static QString remind_coi_other = _C2Q("保留");
static QString remind_coi_unchanged = _C2Q("未改变当地参数的初始化");
static QString remind_coi_changed = _C2Q("改变当地参数的初始化");


static QString remind_total_call = _C2Q("总召唤");
static QString remind_clock_sync = _C2Q("时钟同步");

static QString remind_energy_call = _C2Q("电度召唤");
static QString remind_energy_limit = _C2Q("电能计数量召唤命令限定词(QCC)");
static QString remind_energy_fre_no_reset = _C2Q("冻结不复位");
static QString remind_energy_fre_reset = _C2Q("冻结复位");
static QString remind_energy_counter_reset = _C2Q("计数器复位");
static QString remind_energy_call_1 = _C2Q("请求计数量第1组");
static QString remind_energy_call_2 = _C2Q("请求计数量第2组");
static QString remind_energy_call_3 = _C2Q("请求计数量第3组");
static QString remind_energy_call_4 = _C2Q("请求计数量第4组");
static QString remind_energy_call_5 = _C2Q("请求计数量第5组");



static QString remind_info_addr =  _C2Q("信息体地址:");

static QString remind_yx_addr =  _C2Q("遥信序号:");
static QString remind_yc_addr =  _C2Q("遥测序号:");
static QString remind_yk_addr =  _C2Q("遥控序号:");
static QString remind_yt_addr =  _C2Q("遥调序号:");
static QString remind_ym_addr =  _C2Q("遥脉序号:");


static QString remind_me_na = _C2Q("单点遥测值:");

static QString remind_exe = _C2Q("遥控执行 ");
static QString remind_sel = _C2Q("遥控选择 ");
static QString remind_ms = _C2Q("毫秒:");
static QString remind_minute = _C2Q("分钟:");
static QString remind_hour = _C2Q("时:");
static QString remind_day = _C2Q("日:");
static QString remind_month = _C2Q("月:");
static QString remind_year = _C2Q("年:");
static QString remind_control = _C2Q("控制方向 ");

/********************************************以上为IEC104规约部分********************************************************/


enum TYPE_IDENTIFICATION
{
	SP_NA_1  = 0x01,
	DP_NA_3  = 0x03,
	ME_NA_9  = 0x09,
	ME_NC_13 = 0x0d,
	DC_45    = 0x2d,
	DC_46    = 0x2e,
	PD_47    = 0x2f,
	SP_48    = 0x30,
	SP_50    = 0x32,
	CALL	= 0x64,
	SET_TIME = 0x67,
	//添加
	ST_NA_5 = 0x05,//步位置信息
	BO_NA_7 = 0x07,
	ME_NB_11 = 0x0B,
	IT_NA_15 = 0x0F,
	PS_NA_20 = 0x14,
	ME_ND_21 = 0x15,
	SP_TB_30 = 0x1E,
	DP_TB_31 = 0x1F,
	ST_TB_32 = 0x20,
	BO_TB_33 = 0x21,
	ME_TD_34 = 0x22,
	ME_TE_35 = 0x23,
	ME_TF_36 = 0x24,
	IT_TB_37 = 0x25,
	EP_TD_38 = 0x26,
	EP_TE_39 = 0x27,
	EP_TF_40 = 0x28,

	//控制方向
	SC_NA_45 = 0x2D,
	DC_NA_46 = 0x2E,
	SE_NA_48 = 0x30,
	SC_NA_58 = 0x3A,
	DC_NA_59 = 0x3B,
	RC_NA_60 = 0x3C,
	SE_TA_61 = 0x3D,
	SE_TB_62 = 0x3E,
	SE_TC_63 = 0x3F,
	BO_NA_64 = 0x40,
	//控制方向的系统信息
	IC_NA_100 = 0x64,
	CI_NA_101 = 0x65,
	RD_NA_102 = 0x66,
	CS_NA_103 = 0x67,
	RP_NA_105 = 0x69,
	TS_NA_107 = 0x71,
	M_EI_NA_70 = 0x46

};
enum U_TYPE
{
	STARTDT_ACT   = 0x04|0x03,    // 0x07
	STARTDT_CON   = 0x08|0x03,    // 0x0b
	STOPDT_ACT    = 0x10|0x03,    // 0x13
	STOPDT_CON    = 0x20|0x03,    // 0x23
	TESTFR_ACT    = 0x40|0x03,    // 0x43
	TESTFR_CON    = 0x80|0x03     // 0x83
};

enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 2,///<公共地址长度
	COT_LEN = 2,		///<传送原因长度
	INDEX_LEN     = 3,	///<信息体地址长度
	CTL_FLD = 4,		///<控制域长度
	HEAD_LEN      = 10,	///<报文开始到信息体的长度
	ASDU_DATA_LEN = 256	///<ASDU信息体长度，无特别意义，信息体会交由分析函数处理
};

typedef union
{
	unsigned short int val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
	}val2;
}BYTE2_104;

//16位无符号整数
struct int16u_104
{
	BYTE2_104 word;
	void operator = (unsigned short int n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2_104 one;
		one.val1 = n;
		word.val2.b1 = one.val2.b2;
		word.val2.b2 = one.val2.b1;
#else
		word.val1 = n;
#endif
	}

	operator unsigned short int ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2_104_104 one;
		one.val2.b1 = word.val2.b2;
		one.val2.b2 = word.val2.b1;
		return one.val1;
#else
		return word.val1;
#endif
	}
};
/**
@brief 传送原因
*/
enum COT_TYPE
{
	COT_SPONT        = 3,
	COT_ACT          = 6,
	COT_ACTCON       = 7,
	COT_DEACT        = 8,
	COT_DEACTCON     = 9,
	COT_ACTTERM     = 10,

	COT_INRO         = 20,

	COT_UNKOWN_TYPE  = 44,
	COT_UNKOWN_COT   = 45,
	COT_UNKOWN_ADDR  = 46,
	COT_UNKOWN_INDEX = 47
};

/**
@brief 报文中的时间格式
*/
typedef struct CP56TIME
{
	BYTE ms[2];
	BYTE minute;
	BYTE hour;
	BYTE day:5;
	BYTE wday:3;
	BYTE month;
	BYTE year;
};

/**
@brief ASDU中的信息体，按照字节流处理
*/
typedef    struct    DATA_STRUCT
{
	BYTE    buf[ASDU_DATA_LEN];
};

/**
@brief 发送、接收序号结构
*/
union NSR_FORMAT
{
	int16u_104 num;
	struct
	{
		BYTE lsb;
		BYTE msb;
	};
};

/**
@brief I帧时的数据头
*/
typedef struct I_FORMAT    // I : Information Transmit Format 编号的信息传输格式
{
	NSR_FORMAT ns;
	NSR_FORMAT nr;
};

/**
@brief S帧时的数据头
*/
typedef struct S_FORMAT    // S : Numbered Supervisory Function 编号的监视功能格式
{
	BYTE  s_1;
	BYTE  s_2;

	NSR_FORMAT nr;
};

/**
@brief U帧时的数据头
*/
typedef struct U_FORMAT    // U : Unnumbered Control Function 不编号的控制功能格式
{
	BYTE  u_type;
	BYTE  u_2;
	BYTE  u_3;
	BYTE  u_4;
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

/**
@brief 总召唤/遥控结构
*/
typedef struct C_STRUCT
{

	int16u_104  index_no;	///<信息体地址
	BYTE pos_holder;	///<104规定的
	BYTE qoi;
};


/**
@brief 对时结构
*/
typedef struct SYNC_TIME_STRUCT
{
	int16u_104  index_no;
	BYTE	pos_holder;
	CP56TIME cp56time;
};

/**
@brief ACPI : Application Protocol Control Information 应用规约控制信息
*/
typedef struct P104_APCI  
{
	BYTE    start_id;    // buf[0] 启动字符
	BYTE    apdu_len;    // buf[1] APDU的长度

	union				 // buf[2-5] 控制域
	{
		I_FORMAT type_i;
		S_FORMAT type_s;
		U_FORMAT type_u;
		BYTE    buf[CTL_FLD];
	};
};

/**
@brief ASDU : Application Service Data Unit 应用服务数据单元
*/
typedef struct P104_ASDU
{
	BYTE    type;        // buf[6] type identification 类型标识
	BYTE    vsq;		 // buf[7] variable structure qualifier 可变结构限定词

	int16u_104    cot;       // buf[8-9] cause of transmission 传送原因
	int16u_104   addr;       // buf[10-11] common address 应用服务数据单元公共地址

	union
	{
		C_STRUCT cs;
		SYNC_TIME_STRUCT sts;
		DATA_STRUCT    data;
	};
}P104_ASDU;

/**
@brief APDU : Application Protocol Data Unit 应用规约数据单元
*/
typedef struct P104_APDU  
{
	P104_APCI    apci;
	P104_ASDU    asdu;
};


const double base_num=256.0;


enum SEND_REASON
{
	BURST = 0x0003,  //突发
	ACTIVATION = 0x0106, //激活
	ACTIVATION_CONFIRM = 0x0107, //激活确认
	ACTIVATION_END_CONFIRM = 0x010A//激活结束确认
};

#endif