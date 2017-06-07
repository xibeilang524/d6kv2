/**
@file protocolhandle.h
@brief 前置规约句柄类

提供规约插件接口的载入和执行功能

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_HANDLE_H_
#define FES_PROTOCOL_HANDLE_H_

#include "fes/protocoldef.h"

class Base_protocol;

class Protocol_handle
{
public:
	Protocol_handle();
	Protocol_handle(void* m_get_instance_fp, void* m_get_config_fp);
	Protocol_handle(const Protocol_handle& handle);
	Protocol_handle& operator = (const Protocol_handle&);
	~Protocol_handle();
	
	/**
	@brief 获取规约实例接口
	@param in const int& channel 通道序号
	@return 规约实例指针
	*/
	Base_protocol* get_instance(const int& channel) const;
	/**
	@brief 读取规约配置
	@param in const int& channel 通道序号
	@return 成功返回0，失败返回-1
	*/
	int get_config(void) const;
	/**
	@brief 设置规约实例接口函数是真
	@param in void* 解析出的函数指针
	@return 成功返回0，失败返回-1
	*/
	void set_instance_fp(void* fp);
	/**
	@brief 设置获取配置函数是真
	@param in void* 解析出的函数指针
	*/
	void set_config_fp(void* fp);
private:
	GET_INSTANCE_FP m_get_instance_fp;
	GET_CONFIG_FP   m_get_config_fp;
};


#endif
