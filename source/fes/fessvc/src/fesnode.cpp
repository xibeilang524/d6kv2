/**
@file fesnode.cpp
@brief 前置节点类实现

@author chenkai 
@version 1.0.0
@date 2016-07-18
*/
#include "fes/fesnode.h"
#include "fes/platformwrapper.h"

Fe_node::Fe_node(const int& host_no, const int& host_index)
:m_host_no(host_no), m_index(host_index), m_load_map(0),m_collect_zone(0)
{
 	int* nzone = NULL;
	char sql[500];

	char* host_name = Net_stat::get_node_name(host_no);

	sprintf(sql, "select 监控区号 from 采集分区表 where 前置节点 = '%s'", host_name);
	int rcd_cnt = Db_connector::select(sql,(void **)&nzone );
	if (rcd_cnt > 0)
	{
		m_collect_zone = *nzone;
		printf("my node_no = %d, my zone_no = %d\n",host_no,m_collect_zone);
	}

	if (nzone)
	{
		delete nzone;
		nzone = NULL;
	}

	m_live_timer.restart();
}

int Fe_node::get_srv_stat(void)
{
	return Net_stat::get_host_stat(m_host_no);
}

int Fe_node::calc_load(void) const
{
	const int LONG_BIT_SIZE = 32;

	int load = 0;
	unsigned long mask = 0x01;

	for (int i = 0; i < LONG_BIT_SIZE; i++)
	{
		if ((m_load_map & mask) != 0)
		{
			load++;
		}

		mask = mask << 1;
	}

	return load;
}

int Fe_node::get_stat(void) const
{
	int ret = -1;
	//如果平台判断出该机在线，就需要判断计时器了，否则不需要
	if (!Net_stat::get_host_stat(m_host_no))
	{
		ret = m_live_timer.elapsed() >= DEAD_TIME ? -1 : 0;
	}
	
	return ret;
}

int Fe_node::Is_Main_Server(void) const
{
	return Net_stat::get_host_master_flag(m_host_no);
}