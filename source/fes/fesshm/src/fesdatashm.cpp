/**
@file fesrawshm.h
@brief 前置生数据共享内存头文件

提供前置生数据共享内存的创建、申请、访问接口

@author chenkai 
@version 1.0.0
@date 2016-07-20
*/

#include "fes/fesdatashm.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"

#include <ace/Log_Msg.h>
#include <ace/Numeric_Limits.h>

//////////////////////////////////Fes_data_shm////////////////////////////////////////前置生数据基类共享内存创建
Fes_data_shm::Fes_data_shm()
{
	m_fe_data = static_cast<FE_DATA *>(Share_memery::create_share_memory(FES_DATA_SHM_KEY, FES_DATA_SHM_LEN));

	int m_shmdata_lent = sizeof(FE_DATA);

	ACE_ASSERT(m_fe_data != NULL);
}


Fes_data_shm::~Fes_data_shm()
{
	m_fe_data = NULL;
}

FE_YC* Fes_data_shm::channel_yc(const int& channel) const
{
	FE_YC* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yc_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.yc + m_fe_data->ctrl_seg.channel_yc_begin[channel];
	}

	return ret;
}

const FE_YC* Fes_data_shm::const_channel_yc(const int& channel) const
{
	const FE_YC* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yc_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.yc + m_fe_data->ctrl_seg.channel_yc_begin[channel];
	}

	return ret;
}

FE_YX* Fes_data_shm::channel_yx(const int& channel) const
{
	FE_YX* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yx_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.yx + m_fe_data->ctrl_seg.channel_yx_begin[channel];
	}

	return ret;
}

const FE_YX* Fes_data_shm::const_channel_yx(const int& channel) const
{
	FE_YX* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_yx_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.yx + m_fe_data->ctrl_seg.channel_yx_begin[channel];
	}

	return ret;
}

FE_KWH* Fes_data_shm::channel_kwh(const int& channel) const
{
	FE_KWH* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_kwh_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.kwh + m_fe_data->ctrl_seg.channel_kwh_begin[channel];
	}

	return ret;
}

const FE_KWH* Fes_data_shm::const_channel_kwh(const int& channel) const
{
	FE_KWH* ret = NULL;

	if (channel >= 0 && channel <= Fes_shm::get().fe_sys_data_ptr->max_chan_no 
		&& m_fe_data->ctrl_seg.channel_kwh_begin[channel] != -1)
	{
		ret = m_fe_data->data_seg.kwh + m_fe_data->ctrl_seg.channel_kwh_begin[channel];
	}

	return ret;
}

int Fes_data_shm::rtu_yc(const int& rtu) const
{
	int ret = 0;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_yc_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_yc_begin[rtu];
	}

	return ret;

}
const int Fes_data_shm::const_rtu_yc(const int& rtu) const
{
	int ret = 0;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_yc_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_yc_begin[rtu];
	}

	return ret;
}

int Fes_data_shm::rtu_yx(const int& rtu) const
{
	int ret = NULL;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_yx_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_yx_begin[rtu];
	}

	return ret;
}

const int Fes_data_shm::const_rtu_yx(const int& rtu) const
{
	int ret = NULL;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_yx_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_yx_begin[rtu];
	}

	return ret;
}

int Fes_data_shm::rtu_kwh(const int& rtu) const
{
	int ret = NULL;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_kwh_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_kwh_begin[rtu];
	}

	return ret;
}

const int Fes_data_shm::const_rtu_kwh(const int& rtu) const
{
	int ret = NULL;

	if (rtu >= 0 && rtu <= Fes_shm::get().fe_sys_data_ptr->max_rtu_no 
		&& m_fe_data->ctrl_seg.rtu_kwh_begin[rtu] != -1)
	{
		ret = m_fe_data->ctrl_seg.rtu_kwh_begin[rtu];
	}

	return ret;
}

const float Fes_data_shm::const_yc_slope(const int& channel, const int& datano) const
{
	float ret = 1.0;		//默认为1？

	//内部调用，是否有必要再检验参数合格？？？
	if (m_fe_data->ctrl_seg.channel_yc_begin[channel] != -1)
	{
		ret = m_fe_data->para_seg.yc[m_fe_data->ctrl_seg.channel_yc_begin[channel] + datano].slope;
	}

	return ret;
}

const float Fes_data_shm::const_yc_ordinate(const int& channel, const int& datano) const
{
	float ret = 0.0;		//默认为0？

	//内部调用，是否有必要再检验参数合格？？？
	if (m_fe_data->ctrl_seg.channel_yc_begin[channel] != -1 )
	{
		ret = m_fe_data->para_seg.yc[m_fe_data->ctrl_seg.channel_yc_begin[channel] + datano].ordinate;
	}

	return ret;
}

const unsigned char Fes_data_shm::const_yx_negate(const int& channel, const int& datano) const
{
	unsigned char ret = false;		//默认为0？

	//内部调用，是否有必要再检验参数合格？？？
	if (m_fe_data->ctrl_seg.channel_yx_begin[channel] != -1)
	{
		ret = m_fe_data->para_seg.yx[m_fe_data->ctrl_seg.channel_yx_begin[channel] + datano].negate;
	}

	return ret;
}

const float Fes_data_shm::const_kwh_full_code(const int& channel, const int& datano) const
{
	float ret = 0.0;		//默认为0？

	//内部调用，是否有必要再检验参数合格？？？
	if (m_fe_data->ctrl_seg.channel_kwh_begin[channel] != -1 )
	{
		ret = m_fe_data->para_seg.kwh[m_fe_data->ctrl_seg.channel_kwh_begin[channel] + datano].full_code;
	}

	return ret;
}

const float Fes_data_shm::const_kwh_slope(const int& channel, const int& datano) const
{
	float ret = 0.0;		//默认为0？

	//内部调用，是否有必要再检验参数合格？？？
	if (m_fe_data->ctrl_seg.channel_kwh_begin[channel] != -1 )
	{
		ret = m_fe_data->para_seg.kwh[m_fe_data->ctrl_seg.channel_kwh_begin[channel] + datano].slope;
	}

	return ret;
}



