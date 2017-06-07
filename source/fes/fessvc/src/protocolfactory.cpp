/**
@file protocolhandle.h
@brief 前置规约工厂类实现

@author chenkai 
@version 1.0.0
@date 2016-07-20
*/

#include "fes/protocolfactory.h"
#include "fes/protocolhandle.h"
#include "fes/baseprotocol.h"


#include <ace/DLL_Manager.h>
using std::make_pair;

Protocol_factory_i::Protocol_factory_i()
{

}

int Protocol_factory_i::add_protocol(const char* protocol_name)
{
	int ret = -1;

	if (protocol_name && ACE_OS::strlen(protocol_name))
	{
		ACE_DLL_Handle* handle = ACE_DLL_Manager::instance()->open_dll(
								protocol_name, RTLD_LAZY, NULL);

 		if (!handle)
		{
			ACE_DEBUG((LM_ERROR, "打开规约库%s失败", protocol_name));
		}
		else
		{
			void* get_instance_fp = handle->symbol(GET_INSTANCE_INTERFACE);
			void* get_config_fp = handle->symbol(GET_CONFIG_INTERFACE);

			m_handle_map.insert(make_pair<string, Protocol_handle*>(protocol_name, new Protocol_handle(get_instance_fp, get_config_fp)));

			ret = 0;
		}
	}//if (protocol_name && ACE_OS::strlen(protocol_name))

	return ret;
}

Base_protocol* Protocol_factory_i::get_instance(const char* protocol_name, const int& channel_no)
{
	Base_protocol* ret = NULL;
	
	if (protocol_name)
	{
		protocol_iterator it = m_handle_map.find(protocol_name);

		if (it != m_handle_map.end())
		{
			ret = it->second->get_instance(channel_no);
		}
	}

	return ret;
}

void Protocol_factory_i::get_all_config(void)
{
	for (protocol_iterator it = m_handle_map.begin(); it != m_handle_map.end(); it++)
	{
		if (it->second->get_config() == -1)
		{
			ACE_DEBUG((LM_ERROR, "%s规约特殊配置读取异常,以默认配置运行规约！\n", it->first.c_str()));
		}
	}
}

bool Protocol_factory_i::empty(void)
{
	return m_handle_map.empty();
}

Protocol_factory_i::~Protocol_factory_i()
{
	for (protocol_iterator it = m_handle_map.begin(); it != m_handle_map.end(); it++)
	{
		delete (it->second);
		it->second = NULL;
	}

	m_handle_map.clear();
}
