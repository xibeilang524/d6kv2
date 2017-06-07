/**
@file protocolhandle.h
@brief 前置规约句柄实现

@author chenkai 
@version 1.0.0
@date 2016-07-20
*/
#include "fes/protocolhandle.h"
#include "fes/baseprotocol.h"

#include <ace/Log_Msg.h>

Protocol_handle::Protocol_handle()
:m_get_instance_fp(NULL), m_get_config_fp(NULL)
{

}

Protocol_handle::Protocol_handle(void* m_get_instance_fp, void* m_get_config_fp)
{
	set_instance_fp(m_get_instance_fp);
	set_config_fp(m_get_config_fp);
}

Protocol_handle::Protocol_handle(const Protocol_handle& handle)
{
	if (this != &handle)
	{
		m_get_instance_fp = handle.m_get_instance_fp;
		m_get_config_fp = handle.m_get_config_fp;
	}
}

Protocol_handle::~Protocol_handle()
{
	m_get_instance_fp = NULL;
	m_get_config_fp = NULL;
}

Protocol_handle& Protocol_handle::operator=(const Protocol_handle& handle)
{
	if (this != &handle)
	{
		m_get_instance_fp = handle.m_get_instance_fp;
		m_get_config_fp = handle.m_get_config_fp;
	}

	return *this;
}

void Protocol_handle::set_instance_fp(void* fp)
{
	ptrdiff_t tmp = reinterpret_cast<ptrdiff_t>(fp);
	m_get_instance_fp = reinterpret_cast<GET_INSTANCE_FP>(tmp);
}

void Protocol_handle::set_config_fp(void* fp)
{
	ptrdiff_t tmp = reinterpret_cast<ptrdiff_t>(fp);
	m_get_config_fp = reinterpret_cast<GET_CONFIG_FP>(tmp);
}

Base_protocol* Protocol_handle::get_instance(const int& channel) const
{
	Base_protocol* ret = NULL;

	if (m_get_instance_fp)
	{
		ret = m_get_instance_fp(channel);
	}

	return ret;
}

int Protocol_handle::get_config(void) const
{
	int ret = 0;

	if (m_get_config_fp)
	{
		ret = m_get_config_fp();
	}

	return ret;
}
