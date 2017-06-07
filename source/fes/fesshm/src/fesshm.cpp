/**
@file fesshm.cpp
@brief 创建前置共享内存实现文件

@author chenkai 
@version 1.0.0
@date 2016-08-20
*/

#include "fes/fesshm.h"
#include "fes/platformwrapper.h"

#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/Assert.h"


/**************************************************************************
构造函数，初始化内部成员
**************************************************************************/
Fes_shm::Fes_shm()
{
	shm_addr_ptr = NULL;
	fe_sys_data_ptr = NULL;
	chan_para_ptr = NULL;
	rtu_para_ptr = NULL;
	chan_data_ptr = NULL;
	gps_info_ptr = NULL;

	create_fes_shm();

	fe_sys_data_ptr->host_no = Net_stat::get_host_no();
	//add by zjq
	fe_sys_data_ptr->max_chan_no = MAX_CHAN_NUM;
	fe_sys_data_ptr->max_rtu_no = MAX_RTU_NUM;
	fe_sys_data_ptr->gps_num = MAX_GPS_NUM;
}


/**************************************************************************
获取单实例
**************************************************************************/
Fes_shm& Fes_shm::get()
{
	static Fes_shm instance;
	return instance;
}


/**************************************************************************
创建共享内存
**************************************************************************/
void Fes_shm::create_fes_shm(void)
{
	int		used_len = 0;

	//创建失败直接退出
	shm_addr_ptr = static_cast<char *>(Share_memery::create_share_memory(FES_SHM_KEY, FES_SHM_LEN));
	
	ACE_ASSERT(shm_addr_ptr != NULL);

	if (NULL == shm_addr_ptr)
	{
		ACE_DEBUG((LM_ERROR, "create fes shm failed\n"));
		ACE_OS::exit(1);
	}


	fe_sys_data_ptr = (FE_SYS_DATA*)(shm_addr_ptr + used_len);
	used_len += sizeof(FE_SYS_DATA);

	//字节对齐，增加访问的效率
	if((used_len % SHM_ALIGN) != 0)
	{
		used_len += (SHM_ALIGN - ( used_len % SHM_ALIGN));
	}

	chan_para_ptr = (CHANPARA*)(shm_addr_ptr + used_len);
	used_len += sizeof(CHANPARA) * MAX_CHAN_NUM;

	if((used_len % SHM_ALIGN) != 0)
	{
		used_len += (SHM_ALIGN - ( used_len % SHM_ALIGN));
	}

	rtu_para_ptr = (RTUPARA*)(shm_addr_ptr + used_len);
	used_len += sizeof(RTUPARA) * MAX_RTU_NUM;

	if((used_len % SHM_ALIGN) != 0)
	{
		used_len += (SHM_ALIGN - ( used_len % SHM_ALIGN));
	}

	chan_data_ptr = (CHANDATA*)(shm_addr_ptr + used_len);
	used_len += sizeof(CHANDATA*) * MAX_CHAN_NUM;

	if((used_len % SHM_ALIGN) != 0)
	{
		used_len += (SHM_ALIGN - ( used_len % SHM_ALIGN));
	}

	gps_info_ptr = (GPS_INFO*)(shm_addr_ptr + used_len);
	used_len += sizeof(GPS_INFO*) * MAX_GPS_NUM;

	if((used_len % SHM_ALIGN) != 0)
	{
		used_len += (SHM_ALIGN - ( used_len % SHM_ALIGN));
	}

	ACE_DEBUG((LM_DEBUG, ("create fes shm done, total lenth = %d bytes, already used %d bytes\n"), FES_SHM_LEN, used_len));
}

void Fes_shm::attend_host_switch(const int& default_host, const int& new_host)
{
	for (int i = 0; i < fe_sys_data_ptr->max_chan_no; i++)
	{
		//modify by libinghong 若未使用通道未被接管，在其接管前置启动前，即使使用该通道，仍无法运行
		if (/*chan_para_ptr[i].valid && */chan_para_ptr[i].default_attend_host == default_host)
		{
			chan_data_ptr[i].curr_attend_host = new_host;
		}
	}
}

void Fes_shm::attend_self(const int& host_no)
{
// 	int local_host = fe_sys_data_ptr->host_no;

	for (int i = 0; i < fe_sys_data_ptr->max_chan_no; i++)
	{
		if (chan_para_ptr[i].default_attend_host == host_no)
		{
			chan_data_ptr[i].curr_attend_host = host_no;
		}
	}
}



