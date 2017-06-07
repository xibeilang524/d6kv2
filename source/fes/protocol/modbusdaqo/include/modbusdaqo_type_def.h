/**
@file modbusdaqo_type_def.h
@brief 大全TCP modbus规约

@author chenkai
@version 1.0.0
@date 2016-07-21
*/

#ifndef MODBUSDAQO_TYPE_DEF_H_
#define MODBUSDAQO_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief modbusdaqo类型标识

只列出了常用的，可根据需要和规约文档增加
*/
/**
@brief 帧类别
*/
enum TYPE_IDENTIFICATION
{
	INVALID_FRAME = -1,///<非法帧
	//上行帧类别
};

/**
@brief 报文常用字节
*/
enum DATA_CON
{
	FIRST = 0xC6,   //<报文头
	SECOND = 0xC7 ,

	YC_CODE3 = 0x03 ,
	YX_CODE1 = 0x01 ,
	YK_CODE5 = 0x05 ,
	YK_CODE6 = 0x06 ,

	ERR_CODE = 0x80 ,

	PROTOCOL_FLAG = 0x00 ,

	CALL_YC = 0x20 ,
	CALL_YX = 0x30 ,
};

/**
@brief modbusdaqo配置项结构
*/
struct MODBUSDAQO_CONFIG
{
	unsigned short yx_func_code; //遥信功能码
	unsigned short yx_start_no; //遥信起始序号
	unsigned short yx_call_time; //遥信召唤时间
	unsigned short yc_func_code; //遥测功能码
	unsigned short yc_start_no; //遥测起始序号
	unsigned short yc_call_time; //遥测召唤时间
	unsigned short yk_func_code; //遥控功能码
	unsigned short yk_start_no; //遥控起始序号
	unsigned short t0; //t0

	/**
	@brief 参数置默认值
	*/
	void set_default(void)
	{
		yx_func_code = 1;
		yx_start_no = 3000;
		yx_call_time = 300;
		yc_func_code = 0x03;
		yc_start_no = 0 ;
		yc_call_time = 1000 ;
		yk_func_code = 0x06 ;
		yk_start_no = 9 ;
		t0 = 15000 ;
	};

	/**
	@brief IEC104配置项结构构造函数
	*/
	MODBUSDAQO_CONFIG()
	{
		set_default();
	};
};

#endif