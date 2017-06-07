/**
@file statchecker.cpp
@brief 状态检查类的实现

循环检查本机值班的所有通道的状态

@author hongxiang 
@version 1.0.0
@date 2013-10-22
*/

#include "ace/OS.h"
#include "fes/statchecker.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "ace/Log_Msg.h"

#include "fes/femailstruct.h"
#include "fes/CMyEFile.h"
#include <ace/UUID.h>

using namespace std;

/**************************************************************************
构造函数
**************************************************************************/
Stat_checker::Stat_checker(ACE_Thread_Mutex& check_watch_load_mutex)
:m_check_watch_load_mutex(check_watch_load_mutex)
{
	m_host_no = Net_stat::get_host_no();

	m_mail_buf = new unsigned char[sizeof(FE_MAIL_STRUCT) + sizeof(FE_CHAN_STAT)*MAX_CHAN_NUM];		//预分配内存
}


/**************************************************************************
析构函数
**************************************************************************/
Stat_checker::~Stat_checker()
{
	delete[] m_mail_buf;
	m_mail_buf = NULL;

}

/**************************************************************************
启动
**************************************************************************/
int Stat_checker::open(void *args)
{
	activate();
	return 0;
}

/**************************************************************************
停止
**************************************************************************/
int Stat_checker::close (u_long flags)
{
	return 0;
}

/**************************************************************************
通道状态检查线程体
**************************************************************************/
int Stat_checker::svc(void)
{
	while(true)//add by zjq
	{
		check_stat();
		ACE_OS::sleep(CHECK_STAT_TIME_SPAN);
	}
	return 0;
}

/**************************************************************************
通道状态检查过程
**************************************************************************/
int Stat_checker::check_stat(void)
{
	ACE_Guard<ACE_Thread_Mutex> guard(m_check_watch_load_mutex);
	
	//发送到scada，todo
	FE_MAIL_STRUCT *mail_struct = (FE_MAIL_STRUCT*)m_mail_buf;
	FE_CHANNEL_STAT_MAIL* chan_stat = (FE_CHANNEL_STAT_MAIL*)mail_struct->mail_body;
	mail_struct->mail_type = CHANNEL_STAT;
	mail_struct->sub_information1 = 0;

	for (int channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{
		CHANPARA* p_chan_para = Fes_shm::get().chan_para_ptr + channel_no;
		CHANDATA* p_chan_data = Fes_shm::get().chan_data_ptr + channel_no;
		//通道使用,且是本机值班
		if(p_chan_para->valid == 1 && p_chan_data->curr_attend_host == m_host_no) 
		{
			check_channel_event(channel_no);	//检查通道事项
// 			check_channel_status(channel_no);	//检查通道状态
// 			if(!is_change)
// 				channel_event(channel_no);
			chan_stat->bit_err_rate = p_chan_data->bit_err_rate;
			chan_stat->channel_no = channel_no;
			chan_stat->master = p_chan_data->master_flag;
			chan_stat->rtu_no = p_chan_para->child_rtu;
			chan_stat->stat = p_chan_data->stat;

			chan_stat->last_sync_time = p_chan_data->last_sync_time;
			chan_stat->sync_time_success = p_chan_data->sync_time_success;
			chan_stat->recv_count = p_chan_data->recv_count;
			chan_stat->send_count = p_chan_data->send_count;

			mail_struct->sub_information1++;
			chan_stat++;

		}
	}

	mail_struct->mail_lenth = sizeof(FE_MAIL_STRUCT) + sizeof(FE_CHANNEL_STAT_MAIL) * mail_struct->sub_information1;
// 	FE_CHAN_STAT* chan_stat_head = (FE_CHAN_STAT*)mail_struct->mail_body;

//	return 0;

	//todo  III区前置将通道事项存为文本
	static int time_count = 0;
	time_count++;
	if(Net_stat::get_PUBFEP_flag() && (time_count%5 == 0) )
	{
		EFILE_CHANNEL *e_file_test = new EFILE_CHANNEL( mail_struct->sub_information1);

		char file_path[512];

		ACE_Utils::UUID_Generator uuidout;
		uuidout.init();
		ACE_Utils::UUID uuid;
		uuidout.generate_UUID(uuid);///->ace5.4.0 

		char **hosts = new char*[MAX_FE_SRV_NUM];
		for(int i = 0;i<MAX_FE_SRV_NUM;i++)
		{
			hosts[i] = new char[CODE_LENTH];
			memset(hosts[i],0,sizeof(char)*CODE_LENTH);
		}
		int host_num = Net_stat::get_fe_hosts(hosts, MAX_FE_SRV_NUM);//获取I区fep主机名
		for (int j=0;j<host_num;j++)
		{
			ACE_OS::sprintf(file_path,"%s/log/dat/IIIFEP/%s/channel_%s.txt",getenv("NBENV"),hosts[j],uuid.to_string ()->c_str ());

			e_file_test->set_file_name(file_path);

			e_file_test->save_data_cross_table((FE_CHANNEL_STAT_MAIL*)mail_struct->mail_body);
		}
		for( int k = 0; k < MAX_FE_SRV_NUM; k ++ )
		{
			delete [] hosts[ k ];
		}
		delete [] hosts;
		hosts = NULL;
		return 0;
	}
	else if (!Net_stat::get_PUBFEP_flag())
	{
		return Fe_mail::send_to_scada(mail_struct, mail_struct->mail_lenth, 2);
	}
	else
		return 0;
}

/**************************************************************************
检查通道主备状态
**************************************************************************/
void Stat_checker::check_channel_status(int channel_no)
{
	//函数弃用
}

/**************************************************************************
多通道切换 
**************************************************************************/
void Stat_checker::switch_channel(const int& channel_no)
{
	//函数弃用
}

/**************************************************************************
通道事项
**************************************************************************/
void Stat_checker::channel_event(int channel_no)
{
	CHANPARA* p_chan_para = Fes_shm::get().chan_para_ptr + channel_no;
	CHANDATA* p_chan_data = Fes_shm::get().chan_data_ptr + channel_no;
	int rtu_no = p_chan_para->child_rtu;
	RTUPARA* p_rtu_para = Fes_shm::get().rtu_para_ptr + rtu_no;
//	FE_SYS_DATA* p_sys_data = Fes_shm::get().fe_sys_data_ptr;

// 	ACE_DEBUG((LM_DEBUG, "通道号：%d, 通道名：%s\n",p_chan_para->order,p_chan_para->desdribe));

	//for通道事项
	char namestr[NORMALCHAR_LEN];
//	char nstr[NORMALCHAR_LEN]; 暂时不考虑语音告警
	char* node_name = Net_stat::get_node_name();
	ACE_OS::strcpy(namestr,node_name);
	ACE_OS::strcat(namestr,"上");
	ACE_OS::strcat(namestr,p_chan_para->desdribe);

	//for终端事项
	char rtu_namestr[NORMALCHAR_LEN];
	ACE_OS::strcpy(rtu_namestr,node_name);
	ACE_OS::strcat(rtu_namestr,"上");
	ACE_OS::strcat(rtu_namestr,p_rtu_para->desdribe);

	float channel_event_val = 0;
	float rtu_event_val = 0;
	switch(p_chan_data->stat)
	{
	case CHANSTOP:				//通道停止
		{
// 			ACE_DEBUG((LM_DEBUG, "产生停止事项\n"));
			channel_event_val = rtu_event_val = CHANSTOP;
			ACE_OS::strcat(namestr,"通道停止");
			ACE_OS::strcat(rtu_namestr,"终端停运");
//			ACE_OS::strcpy(nstr,"通道停止");
			break;
		}
	case CHANUNSYN:				//通道失步
		{
			channel_event_val = rtu_event_val = CHANUNSYN;
			ACE_OS::strcat(namestr,"通道失步");
			ACE_OS::strcat(rtu_namestr,"终端停运");
//			ACE_OS::strcpy(nstr,"通道失步");
			break;
		}
	case CHANRATE:				//通道误码
		{
			channel_event_val = rtu_event_val = CHANRATE;
			ACE_OS::strcat(namestr,"误码率高");
			ACE_OS::strcat(rtu_namestr,"终端停运");
//			ACE_OS::strcpy(nstr,"误码率高");
			break;
		}
	case CHANSEARCH:			//通道搜索
		{
			channel_event_val = rtu_event_val = CHANSEARCH;
			ACE_OS::strcat(namestr,"通道搜索");
			ACE_OS::strcat(rtu_namestr,"终端停运");
//			ACE_OS::strcpy(nstr,"通道搜索");
			break;
		}
	case CHANRUN:				//通道运行
		{
// 			ACE_DEBUG((LM_DEBUG, "产生运行事项事项\n"));
			channel_event_val = rtu_event_val = CHANRUN;
			ACE_OS::strcat(namestr,"通道运行");
			ACE_OS::strcat(rtu_namestr,"终端运行");
//			ACE_OS::strcpy(nstr,"通道运行");
			break;
		}
	}
	char chanorder[24];
	sprintf(chanorder,"%d",p_chan_para->order);
	//生成通道事项
	Fe_event::make_channel_event(channel_event_val,"", chanorder, namestr);

	if(Net_stat::get_PUBFEP_flag())
	{
		EFILE_CHANNEL *e_file_test = new EFILE_CHANNEL( 1 );

		char file_path[512];

		ACE_Utils::UUID_Generator uuidout;
		uuidout.init();
		ACE_Utils::UUID uuid;
		uuidout.generate_UUID(uuid);///->ace5.4.0 

		char **hosts = new char*[MAX_FE_SRV_NUM];
		for(int i = 0;i<MAX_FE_SRV_NUM;i++)
		{
			hosts[i] = new char[CODE_LENTH];
			memset(hosts[i],0,sizeof(char)*CODE_LENTH);
		}
		int host_num = Net_stat::get_fe_hosts(hosts, MAX_FE_SRV_NUM);//获取I区fep主机名
		for (int j=0;j<host_num;j++)
		{
			ACE_OS::sprintf(file_path,"%s/log/dat/IIIFEP/%s/channel_%s.txt",getenv("NBENV"),hosts[j],uuid.to_string ()->c_str ());

			e_file_test->set_file_name(file_path);
			char chan_order[64];
			sprintf(chan_order,"%d",p_chan_para->order);
			e_file_test->save_event_cross_table(channel_event_val, chan_order, namestr);
		}
		for( int k = 0; k < MAX_FE_SRV_NUM; k ++ )
		{
			delete [] hosts[ k ];
		}
		delete [] hosts;
		hosts = NULL;
	}
	
// 	//生成终端事项 若通道主通道标志依然为1，证明其无备通道或其他备用通道也为非运行状态，故rtu此刻停运
// 	if(p_chan_data->master_flag == 1)
// 		Fe_event::make_rtu_event(rtu_event_val,"", p_rtu_para->desdribe, rtu_namestr);	通道切换功能并入scada之后,无法判断masterflag标志，rtu事项也相应转移

}

/**************************************************************************
通道切换事项
**************************************************************************/
void Stat_checker::switch_channel_event(int channel_no, bool main_back)
{
	CHANPARA* p_chan_para = Fes_shm::get().chan_para_ptr + channel_no;
// 	CHANDATA* p_chan_data = Fes_shm::get().chan_data_ptr + channel_no;
//	FE_SYS_DATA* p_sys_data = Fes_shm::get().fe_sys_data_ptr;

	char namestr[NORMALCHAR_LEN];
//	char nstr[NORMALCHAR_LEN]; 暂时不考虑语音告警
	char* node_name = Net_stat::get_node_name();
	ACE_OS::strcpy(namestr,node_name);
	ACE_OS::strcat(namestr,"上");
	ACE_OS::strcat(namestr,p_chan_para->desdribe);

	float switch_channel_val = 0;
	if ( main_back == true )
	{
		switch_channel_val = 7;
		ACE_OS::strcat( namestr, "切换为主运行");
//		ACE_OS::strcpy(nstr,"切换为主运行");
	} 
	else
	{
		switch_channel_val = 8;
		ACE_OS::strcat( namestr, "切换为备运行");
//		ACE_OS::strcpy(nstr,"切换为备运行");
	}
	char chanorder[24];
	sprintf(chanorder,"%d",p_chan_para->order);

	//生成通道切换事项
	Fe_event::make_channel_event(switch_channel_val,"", chanorder, namestr);
// 	ACE_OS::sleep(1);
}

/**************************************************************************
循环检查通道事项
**************************************************************************/
bool Stat_checker::check_channel_event(int channel_no)
{
	CHANPARA* p_chan_para = Fes_shm::get().chan_para_ptr + channel_no;
	CHANDATA* p_chan_data = Fes_shm::get().chan_data_ptr + channel_no;

	int	check_flag = 1;		//通道是否产生事项标志

	time_t current_time;

	current_time = ACE_OS::time(NULL);  //获取当前时间

	if( p_chan_para->valid == 0 || p_chan_para->check == 0)  //通道不使用或者通道不不检查
	{
		return 1;
	}

	//计算误码率
	if ( p_chan_data->frame_corr_cnt + p_chan_data->frame_err_cnt )
	{
		p_chan_data->bit_err_rate = (p_chan_data->frame_err_cnt / 
			(float)(p_chan_data->frame_corr_cnt + p_chan_data->frame_err_cnt)) * 100; 
	}
	if (current_time - p_chan_data->last_recv_time < p_chan_para->stop_time_limit)	//最后接收数据时间与当前时间差小于停止时间
	{
		if (p_chan_data->stat != CHANRUN)		//转为运行
		{
			p_chan_data->last_up_time = ACE_OS::time(NULL);
			p_chan_data->stat = CHANRUN;
			ACE_DEBUG((LM_DEBUG, "Chan %d Run\n",channel_no));
			check_flag = 0;
			channel_event(channel_no);		//发出运行事项
		}
	}
	else if (current_time - p_chan_data->last_recv_time >= p_chan_para->stop_time_limit) //最后接收数据时间与当前时间差大于停止时间
	{
		if (p_chan_data->stat != CHANSTOP)		//转为停止
		{
			p_chan_data->last_down_time = ACE_OS::time(NULL);
			p_chan_data->stat = CHANSTOP;
			ACE_DEBUG((LM_DEBUG, "Chan %d Stop\n",channel_no));
			check_flag = 0;
			channel_event(channel_no);  //发出停止事项
		}
	}
	else  if(p_chan_data->bit_err_rate > p_chan_para->bit_error_rate)//通道误码
	{

		if (p_chan_data->stat == CHANSTOP)
		{
			p_chan_data->bit_err_rate = 0;
		}
		p_chan_data->stat = CHANRATE;
		channel_event(channel_no);  //发出事项
		check_flag = 0;
		ACE_DEBUG((LM_DEBUG, "Create Chan %d  BchErr Event\n",channel_no));
	}
	//todo
	// 		else if (p_chan_data->stat == CHANSEARCH) //通道搜索
	// 		{
	// 			p_chan_data->frame_check_err = 0;
	// 			p_chan_data->frame_corr_cnt = 0;
	// 			p_chan_data->frame_err_cnt = 0;
	// 			p_chan_data->bit_err_rate = 0;
	// 			ACE_DEBUG((LM_DEBUG, "Create Chan %d Search\n",channel_no));
	// 			check_flag =1;
	// 		}
	// 		else if (p_chan_data->stat == CHANUNSYN) //通道失步
	// 		{
	// 			ACE_DEBUG((LM_DEBUG, "Create Chan %d UnSyn Event\n",channel_no));
	// 			check_flag = 1;
	// 		}

	return check_flag == 1 ? true : false;
}