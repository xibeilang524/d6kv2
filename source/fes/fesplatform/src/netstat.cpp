/**
@file netstat.cpp
@brief 前置平台包装实现之网络状态

@author chenkai 
@version 1.0.0
@date 2016-07-15
*/
#include "fes/platformwrapper.h"
#include "net/netapi.h"
#include <algorithm>
using namespace std;

static NetStatus s_net_stat;
const int DEFAULT_NET_GROUP = 1;

int Net_stat::get_host_no(void)
{
	return s_net_stat.GetHostNo();
}


int Net_stat::get_host_no(char* node_name)
{
	char* host = s_net_stat.GetHostName(DEFAULT_NET_GROUP, node_name);
	return s_net_stat.GetHostNo(DEFAULT_NET_GROUP, host);
}

char* Net_stat::get_host_name(char* node_name)
{
	return s_net_stat.GetHostName(DEFAULT_NET_GROUP, node_name);
}


int Net_stat::get_fe_nodes(int* nodes, const int&lenth)//todo  三区前置的计算方法有所不同
{
	if (nodes == NULL)
	{
		return -1;
	}

	char dbname[RTDB_NAME_LEN];
	if (get_PUBFEP_flag())
	{
		strcpy(dbname,PUB_FEP_SERVER);
	}
	else
	{
		strcpy(dbname,FEP_SERVER);
	}

	int ret = -1;
	ShmNetDB  shm_net_db;
	RTDB_DB* rtdb_db = shm_net_db.RtdbDB();
	for (int i = 0; i < rtdb_db->grp[DEFAULT_NET_GROUP].dbnum; i++)
	{
		if (strcmp(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].dbname, dbname))
		{
			continue;
		}

		int min_size = min((int)rtdb_db->grp[DEFAULT_NET_GROUP].db[i].nodenum, lenth);		//modify by chenkai linux下min宏无法转换需强转
		ret = min_size;

		for (int j = 0; j < min_size; j++)
		{
			int temp = 0;
			nodes[j] = get_host_no(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].node[j]);
		}
	}//for (int i = 0; i < rtdb_db->grp[DEFAULT_NET_GROUP].dbnum; i++)

	return ret;
}



int Net_stat::get_fe_hosts(char** hosts, const int&lenth)//todo  三区前置的计算方法有所不同
{
	if (hosts == NULL)
	{
		return -1;
	}

	char dbname[RTDB_NAME_LEN];
	strcpy(dbname,FEP_SERVER);

	int ret = -1;
	ShmNetDB  shm_net_db;
	RTDB_DB* rtdb_db = shm_net_db.RtdbDB();
	for (int i = 0; i < rtdb_db->grp[DEFAULT_NET_GROUP].dbnum; i++)
	{
		if (strcmp(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].dbname, dbname))
		{
			continue;
		}

		int min_size = min((int)rtdb_db->grp[DEFAULT_NET_GROUP].db[i].nodenum, lenth);		//modify by chenkai linux下min宏无法转换需强转
		ret = min_size;

		for (int j = 0; j < min_size; j++)
		{
			int temp = 0;
			strcpy(hosts[j],get_host_name(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].node[j]));
		}
	}

	return ret;
}


int Net_stat::get_fe_host_index(int host_no)
{
	ShmNetDB  shm_net_db;
	RTDB_DB* rtdb_db = shm_net_db.RtdbDB();
	int ret = -1;

	char dbname[RTDB_NAME_LEN];
	if (get_PUBFEP_flag())
	{
		strcpy(dbname,PUB_FEP_SERVER);
	}
	else
	{
		strcpy(dbname,FEP_SERVER);

	}

	for (int i = 0; i < rtdb_db->grp[DEFAULT_NET_GROUP].dbnum; i++)
	{
		if (strcmp(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].dbname, dbname))
		{
			continue;
		}


		for (int j = 0; j < rtdb_db->grp[DEFAULT_NET_GROUP].db[i].nodenum; j++)
		{
			if (host_no == s_net_stat.GetHostNo(rtdb_db->grp[DEFAULT_NET_GROUP].db[i].node[j]))
			{
				ret = j;
			}
		}
	}//for (int i = 0; i < rtdb_db->grp[DEFAULT_NET_GROUP].dbnum; i++)

	return ret;
}

int Net_stat::get_host_stat(int host_no)
{
	return s_net_stat.GetHostRstate(host_no) == HOST_STATUS_STOP ? -1 : 0;
}

char* Net_stat::get_node_name(void)
{
	return s_net_stat.GetNodeName(1, s_net_stat.GetHostName(s_net_stat.GetHostNo()));
}

char* Net_stat::get_node_name(int host_no)
{
	return s_net_stat.GetNodeName(DEFAULT_NET_GROUP, s_net_stat.GetHostName(host_no));
	 
	return NULL;
}

int Net_stat::get_host_master_flag(int host_no)
{
	return s_net_stat.IsMainServer(1, host_no, FEP_SERVER) == HOST_STATUS_MAIN ? 0 : -1;
}

void Net_stat::get_host_addr(int host_no, struct in_addr* addr)
{
	s_net_stat.GetHostNetA_Addr(host_no, addr++);
	s_net_stat.GetHostNetB_Addr(host_no, addr);
}

int Net_stat::get_PUBFEP_flag( )
{
	return s_net_stat.IsServer(1, PUB_FEP_SERVER);
}

