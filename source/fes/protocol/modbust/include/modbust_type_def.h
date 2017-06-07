/**
@file modbust_type_def.h
@brief 带断点续传的modbus规约数据类型定义头文件

@author lujiashun
@version 1.0.0
@date 2016-03-23
*/

#ifndef MODBUST_TYPE_DEF_H_
#define MODBUST_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief modbust类型标识

只列出了常用的，可根据需要和规约文档增加
*/
enum TYPE_IDENTIFICATION
{
	C_CG_TA_1 = 0x01,	///<确认信息（带时间戳）

	M_SP_TA_1 = 0x0f,	///<遥信（带时间戳）
	M_ME_TA_1 = 0x10	///<遥测和遥脉（带时间戳）
};

/**
@brief 数据长度
*/
enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 1,///<公共地址长度

	CALL_DATA_LEN = 8,		///<召唤yc、yx、ym数据帧长度
	CONFIRM_LINK_LEN = 16,  ///<确认报文长度

// 	ASDU_DATA_LEN = 256	///<ASDU信息体长度，无特别意义，信息体会交由分析函数处理
	ASDU_DATA_LEN = 512	///<ASDU信息体长度，无特别意义，信息体会交由分析函数处理
};

#if defined (__unix)
#define         LOBYTE(w)       ( (unsigned char) (w) )
#define         HIBYTE(w)       ( (unsigned char) ( (unsigned short) (w)>>8) )
#define         LOWORD(l)       ( (unsigned short) (l) )
#define         HIWORD(l)	( (unsigned short) ( (unsigned int) (l)>>16) ) 
#endif

#pragma pack(1)

/**
@brief 报文中的时间格式
*/
struct MODBUST_TIME
{
	unsigned char year;
	unsigned char month;
	unsigned char day;	
	unsigned char hour;
	unsigned char minite;
	unsigned char secend;
};

/**
@brief 报文中的电度量,高字节在最前，低字节在后
*/
struct MODBUST_KWH
{
// 	int32u	data_kwh;	//4字节
	unsigned char data_kwh_hh;		//高字节
	unsigned char data_kwh_hl;		//
	unsigned char data_kwh_lh;		//
	unsigned char data_kwh_ll;		//低字节
};
/**
@brief 报文中的遥测量
*/
struct MODBUST_YC
{
// 	int16u	data_yc;	//2字节
	unsigned char data_yc_h;	//高字节
	unsigned char data_yc_l;	//低字节
};
/**
@brief APCI : Application Protocol Control Information 应用规约控制信息
*/
struct MODBUST_APCI  
{
	unsigned char   addr;    // buf[0] 装置地址
	unsigned char   type;    // buf[1] 数据类型
	unsigned char	send_num_h;			//发送序号高字节
	unsigned char	send_num_l;			//发送序号低字节
	unsigned char	data_begin_addr_h;	//起始地址高字节
	unsigned char	data_begin_addr_l;	//起始地址低字节
	unsigned char	data_num_h;			//遥测（遥信、遥脉）个数高字节
	unsigned char	data_num_l;			//遥测（遥信、遥脉）个数低字节
	unsigned char	data_byte_num_h;		//数据部分字节数高字节
	unsigned char	data_byte_num_l;		//数据部分字节数低字节
};

/**
@brief APDU : Application Protocol Data Unit 应用规约数据单元
*/
typedef struct MODBUST_APDU  
{
	MODBUST_APCI    apci;  //8字节
	unsigned char	buf[ASDU_DATA_LEN];
};

#pragma pack()


/**
@brief IEC104配置项结构
*/
struct MODBUST_CONFIG
{
	unsigned short yx_begin;	///<遥信起始地址
	unsigned short yc_begin;	///<遥测起始地址
	unsigned short kwh_begin;	///<电度起始地址

	unsigned char reply_data_flag;	///<收到遥测、遥信、遥脉数据后回复确认帧，1为回复，0为不回复

	/**
	@brief 参数置默认值
	*/
	void set_default(void)
	{
		yx_begin = 0x0000;
		yc_begin = 0x0000;
		kwh_begin = 0x0bb8;

		reply_data_flag = 0;	//默认不回复
	};

	/**
	@brief IEC104配置项结构构造函数
	*/
	MODBUST_CONFIG()
	{
		set_default();
	};
};

#endif