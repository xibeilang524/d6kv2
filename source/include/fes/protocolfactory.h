/**
@file protocolfactory.h
@brief 前置规约工厂类

提供规约插件的打开、调用功能

@author zhangyun 
@version 1.0.0
@date 2013-11-07
*/
#ifndef FES_PROTOCOL_FACTORY_H_
#define FES_PROTOCOL_FACTORY_H_

#include <map>
#include <string>
using std::map;
using std::string;

#include <ace/Singleton.h>
#include <ace/Synch.h>

class Protocol_handle;
class Base_protocol;
class Protocol_factory_i;

//适用一下ACE提供的单例接口
typedef ACE_Singleton<Protocol_factory_i, ACE_SYNCH_MUTEX> Protocol_factory;

/**
@brief 规约工厂类
*/
class Protocol_factory_i
{
	friend class ACE_Singleton<Protocol_factory_i, ACE_SYNCH_MUTEX>;
	typedef map<string, Protocol_handle*>::iterator protocol_iterator;
public:
	~Protocol_factory_i();
	/**
	@brief 增加一种规约
	@param in const char* protocol_name 规约名
	@return 成功返回0，失败返回-1
	*/
	int add_protocol(const char* protocol_name);
	/**
	@brief 获取规约实例
	@param in const char* protocol_name 规约名
	@param in const int& channel 通道序号
	@return 规约实例指针
	*/
	Base_protocol* get_instance(const char* protocol_name, const int& channel_no);
	/**
	@brief 读取所有规约配置
	*/
	void get_all_config(void);
	/**
	@brief 规约工厂是否为空
	@return 空返回ture，不为空返回false
	*/
	bool empty(void);

private:

	Protocol_factory_i();

private:
	map<string, Protocol_handle*> m_handle_map;
};



#endif

