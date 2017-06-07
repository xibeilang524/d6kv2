/**
@file fesscdshm.h
@brief 前置及scada生数据共享内存结构体定义头文件

提供前置生数据共享内存的创建、申请、访问接口

@author zhoujian 
@version 1.0.0
@date 2014-01-02
*/
#ifndef FES_SCADA_SHM_H_
#define FES_SCADA_SHM_H_

#include <limits>

const unsigned char UCHAR_DEFAULT = (std::numeric_limits<unsigned char>::max)();
const float FLOAT_DEFAULT = (std::numeric_limits<float>::max)();
const unsigned long ULONG_DEFAULT = (std::numeric_limits<unsigned long>::max)();

#define		YK_CHECK_MSG_LEN		128

/**
@brief 前置毫秒级时间结构
*/
typedef struct
{
	short   		year;	///<年，从1900年开始
	unsigned char   month;	///<月
	unsigned char	day;	///<日
	unsigned char	hour;	///<小时
	unsigned char 	minute;	///<分
	unsigned char   second;	///<秒
	short			milisecond;	///<毫秒
}FETIME;

/**
@brief 数据修饰符
*/
typedef struct
{
	FETIME time;
	unsigned char quality;			//质量位
	unsigned char self_time_flag;	//是否自带时标

	void set_default(void)
	{
		quality = UCHAR_DEFAULT;
		self_time_flag = UCHAR_DEFAULT;
	};
}FE_DATA_MODIFIER;

/**
@brief 遥测带时标的生数据
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	float raw_yc;
	float processed_yc;

	void set_default(void)
	{
		modifier.set_default();
		raw_yc = FLOAT_DEFAULT;			//生数据
		processed_yc = FLOAT_DEFAULT;	//工程转换数据
	};
}FE_YC;

/**
@brief 遥信带时标的生数据
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	unsigned char raw_yx;			//生数据 0:分位 1:合位 2:无效
	unsigned char processed_yx;		//工程转换数据

	void set_default(void)
	{
		modifier.set_default();
		raw_yx = UCHAR_DEFAULT;
		processed_yx = UCHAR_DEFAULT;
	};
}FE_YX;

/**
@brief 电度带时标的生数据
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	unsigned long raw_kwh;			//生数据
	unsigned long processed_kwh;	//工程转换数据

	void set_default(void)
	{
		modifier.set_default();
		raw_kwh = ULONG_DEFAULT;
		processed_kwh = ULONG_DEFAULT;
	};
}FE_KWH;

/**
@brief 发送给SCADA的实时数据结构
*/
typedef struct
{
	enum
	{
		YX_REALDATA,
		YC_REALDATA,
		KWH_REALDATA
	};
	unsigned char	type;	///<数据类型，YX/YC/KWH
	unsigned char	master_flag;	///<是否主通道数据
	short	channel_no;		///<发出数据的通道号
	short	rtu_no;			///<终端号
	short	order;			///<数据点号
	union
	{
		FE_YX	yx_value;	///<遥信值
		FE_YC	yc_value;	///<遥测值
		FE_KWH	kwh_value;	///<电度值
	}value;					///<数据值
}FE_REAL_DATA_NODE;

/**
@brief 事项类型
*/
enum EVENT_TYPE
{
	RTU_SOE = 0x01,		///<普通SOE
	HOST_SOE = 0x02,	///<主机SOE，即普通变位
	STAION_EVENT = 0x05,
	DAS_DIAGNOSE = 0x15,
};


/**
@brief 发送给SCADA的实时电力事项结构
*/
typedef struct
{

	FETIME	time;	///<事项的时间

	unsigned char	source;	///<事项源
	unsigned char	type;	///<事项类型，目前仅DNP规约使用
	unsigned char	master_flag;
	short	postion_holder1;	///<占位符
	short	channel;		///<事项通道
	short   rtuno;			///<rtu号
	union
	{
		struct
		{
			unsigned char	value;	///<遥信值
			unsigned char	postion_holder2;	///<占位符
			short	order;	///<遥信序号

		}yx;				///<遥信
		float		ycval;	///<遥测值
	}val;					///<值

	////仅当type为故障诊断时用到下面的内容
// 	BYTE	accident_type; //事故类型:0--无法区分 1-单相接地 2-相间短路
// 	BYTE	accident_line; //0-保留 1-A相 2-B相 3-C相 4-8保留 9-AB 10-BC 11-AC 12-ABC
// 	WORD	postion_holder3;
// 	WORD	yx_order1;
// 	WORD	ftu_no1;
// 	WORD	yx_order2;
// 	WORD	ftu_no2;
}FE_DATA_EVENT_NODE;


/**
@brief 发送给SCADA的遥控返校结构
*/
typedef struct
{
	unsigned char	master_flag;	///<是否主通道数据
	short	channel_no;		///<发出数据的通道号
	short	rtu_no;			///<终端号
	short	yk_no;			///<遥控号
	int		yk_ret;			///<遥控返校值
	char	msg[YK_CHECK_MSG_LEN]; ///<返校描述
}FE_YK_CHECK_NODE;
#endif