/**
@file CDT_type_def.h
@brief 104规约数据类型定义头文件

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#ifndef CDT_TYPE_DEF_H_
#define CDT_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief CDT类型标识

只列出了常用的，可根据需要和规约文档增加
*/

/**
@brief 帧类别
*/
enum TYPE_IDENTIFICATION
{
	INVALID_FRAME = -1,///<非法帧
	//上行帧类别
	IMPORTANT_YC = 0x61,///<重要遥测
	SECOND_YC = 0xC2,	///<次要遥测
	COMM_YC = 0xB3,		///<一般遥测
	STAT_YX = 0xF4,		///<遥信状态
	KWH_PI = 0x85,		///<电能脉冲计数值
	YX_SOE = 0x26,		///<事件顺序记录
	YK_RET = 0x9E,		///<遥控返校

	//下行帧类别
	SEL_YK = 0x61,		///<遥控选择
	EXCUTE_YK = 0xC2,	///<遥控执行
	CANCEL_YK = 0xB3,	///<遥控撤消
	SEL_YT = 0xF4,		///<升降选择
	EXCUT_YT = 0x85,	///<升降执行
	CANCEL_YT = 0x26,	///<升降撤消
	SET_COMMAND = 0x57,	///<设定命令

	SYNC_TIME = 0x7A,	///<设定时钟
	SET_TIME = 0x0B,	///<设定时钟校正值
	CALL_TIME = 0x4C,	///<召唤子站时钟
	RESET = 0x3D,		///<复归命令
	BROADCAST = 0x9E,	///<广播命令
	
};

/**
@brief 功能码
*/
enum CDT_FUNC_CODE
{
	FUNC_FREQUENCE	 = 0x8A,  //频率
	FUNC_RESET		 = 0x8b,  //复位
	FUNC_BORADCAST   = 0x8C,  //广播
	FUNC_SEL_YK		 = 0xE0,  //遥控选择
	FUNC_RET_YK		 = 0xE1,  //遥控返校
	FUNC_EXCUTE_YK   = 0xE2,  //遥控执行
	FUNC_CANCEL_YK   = 0xE3,  //遥控撤销
	FUNC_SEL_YT		 = 0xE4,  //升降选择
	FUNC_RET_YT		 = 0xE5,  //升降返校
	FUNC_EXCUTE_YT   = 0xE6,  //升降执行
	FUNC_CANCEL_YT	 = 0xE7,  //升降撤销
	FUNC_SET_COMMAND = 0xE8,  //设置命令
	FUNC_BACKUP0	 = 0xE9,  //备用
	FUNC_BACKUP1	 = 0xEA,  //备用
	FUNC_BACKUP2	 = 0xEB,  //备用
	FUNC_RTU_INFO	 = 0xEC,  //子站状态信息

	FUNC_SET_TIME	 = 0xED, //设置时钟校正值

};

/**
@brief 各种数据在报文中的位置
*/
enum BYTE_POS_DEF
{
	START_ID = 0,

	CONTROL_BYTE_POS	= 6,
	FRAME_TYPE_POS      = 7,
	INFO_NUM_POS	    = 8,
	SRC_ADDR_POS	    = 9,
	DEST_ADDR_POS	    = 10,
	CALC_CODE_POS	    = 11,

	INFO_FIRS_POS		= 12,
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
@brief 控制字字节
*/
struct CONTORL_BYTE		
{
	unsigned char control:4;
	unsigned char D:1;
	unsigned char S:1;
	unsigned char L:1;
	unsigned char E:1;
};

/**
@brief 控制字结构
*/
struct CONTROL_STRUCT    // S : Numbered Supervisory Function 编号的监视功能格式
{
	CONTORL_BYTE  contor_byte;
	unsigned char  frame_type;
	unsigned char  info_num;
	unsigned char  src_addr;
	unsigned char  dest_addr;
	unsigned char  calc_byte;
};

/**
@brief 信息字结构
*/
struct INFO_STRUCT    // S : Numbered Supervisory Function 编号的监视功能格式
{
	unsigned char  func_code;
	unsigned char  info_no1;
	unsigned char  info_no2;
	unsigned char  info_no3;
	unsigned char  info_no4;
	unsigned char  bch_code;
};

/**
@brief SOE信息字结构
*/
struct SOE_STRUCT    // S : Numbered Supervisory Function 编号的监视功能格式
{
	unsigned char  func_code1;
	unsigned char  milsecond_L;
	unsigned char  milsecond_H;
	unsigned char  seconds;
	unsigned char  minute;
	unsigned char  bch_code1;
	unsigned char  func_code2;
	unsigned char  hour;
	unsigned char  day;
	unsigned char  data_no_L;
	unsigned char  data_no_H:4;
	unsigned char  back_info:3;
	unsigned char  stat:1;
	unsigned char  bch_code2;
};


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
@brief 对时结构
*/
struct SYNC_TIME_STRUCT
{
	int16u  index_no;
	unsigned char	pos_holder;
	CP56TIME cp56time;
};

/**
@brief CDT遥控报文结构
*/
struct CDT_YK_STRUCT
{
	CONTROL_STRUCT  control_struct;;
	INFO_STRUCT		info_strcut1;
	INFO_STRUCT		info_strcut2;
	INFO_STRUCT		info_strcut3;
};

#pragma pack()

/**
@brief CDT配置项结构
*/
struct CDT_CONFIG
{
	short call_all_data_gap;///<总召间隔，为-1时不总召
	short call_kwh_gap;		///<召唤电度间隔，为-1时不召唤
	short send_time_gap;	///<对时间隔，为-1时不对时

	unsigned short yx_begin;		///<遥信起始地址
	unsigned short yx_end;			///<遥信结束地址
	
	unsigned short yc_begin;		///<遥测起始地址
	unsigned short yc_end;			///<遥测结束地址
	
	unsigned short yx_soe_begin;	///<事项顺序记录起始地址
	unsigned short yx_soe_end;		///<事项顺序记录结束地址
	

	unsigned short pi_data_begin;	///<电能脉冲起始地址
	unsigned short pi_data_end;		///<电能脉冲结束地址

	unsigned short add_yc_begin;	///<总加遥测起始地址
	unsigned short add_yc_end;		///<总加遥测结束地址


	unsigned short t0;		///<t0超时，建立连接超时
	unsigned short t1;		///<t1超时时间，发送一个I帧或U帧后，必须在t1时间内接收到对方的确认，否则认为连接有问题，需要关闭重新连接
	unsigned short t2;		///<t2超时时间，接收一个I帧后，在t2时间内未接收到新的I帧，发送S帧进行确认
	unsigned short t3;		///<t3超时时间，接收到对方一帧报文后，经过t3时间没有接收到新的报文，需要向对方发送U帧测试

	/**
	@brief 参数置默认值
	*/
	void set_default(void)
	{
		call_all_data_gap = 900;//总召间隔默认值900秒
		call_kwh_gap = 1200;	//召唤电度间隔默认值1200秒
		send_time_gap = 300;	//对时间隔300秒

		yx_begin = 0xF0;
		yx_end = 0xFF;
		yc_begin = 0x00;
		yc_end = 0x7F;
		yx_soe_begin = 0x80;
		yx_soe_end = 0x81;
		pi_data_begin = 0xA0;
		pi_data_end = 0xDF;
		add_yc_begin = 0x86;
		add_yc_end = 0x89;

		//以下超时为南网要求，秒级单位
		t0 = 30;
		t1 = 15;
		t2 = 10;
		t3 = 20;

	};

	/**
	@brief CDT配置项结构构造函数
	*/
	CDT_CONFIG()
	{
		set_default();
	};
};

#endif