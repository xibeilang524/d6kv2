/**
@file protocoldef.h
@brief 规约插件中需要实现的C风格接口名

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_DEF_H_
#define FES_PROTOCOL_DEF_H_

class Base_protocol;

//以下是具体规约中需要实现的几个C风格接口的函数指针定义
typedef Base_protocol* (*GET_INSTANCE_FP)(const int& channel_no);	///<获取规约实例
typedef int (*GET_CONFIG_FP)(void);									///<获取存放在数据库中的规约特殊配置
typedef char* (*GET_VERSION_FP)(void);								///<获取规约库版本
typedef char* (*GET_DESC_FP)(void);									///<获取规约库自身描述

//以下是具体规约中需要实现的几个C风格接口的接口名
const char* const GET_INSTANCE_INTERFACE = "get_protocol";	///<获取实例接口名
const char* const GET_CONFIG_INTERFACE = "get_config";		///<获取配置接口名
const char* const GET_VERSION_INTERFACE = "get_version";	///<获取配置接口名
const char* const GET_DESC_INTERFACE = "get_desc";			///<获取配置接口名

#endif