#include "fes/portwatch.h"
#include "fes/fesnode.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/femailstruct.h"

#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"


Port_watch::Port_watch(ACE_Thread_Mutex& check_watch_load_mutex)
:m_check_watch_load_mutex(check_watch_load_mutex), m_fes_shm(Fes_shm::get())
{
	create_host_nodes();
	m_mail_buf = new unsigned char[sizeof(FE_MAIL_STRUCT) + PORT_WATCH_MSG::get_lenth(MAX_CHAN_NUM)];
	m_give_back_flag = 0;
	m_already_give_up = 0;
}

Port_watch::~Port_watch()
{
	delete[] m_mail_buf;
	m_mail_buf = NULL;

	while (m_fe_nodes.empty())
	{
		Fe_node* node = m_fe_nodes.back();
		delete node;
		m_fe_nodes.pop_back();
	}
}

int Port_watch::open(void *args)
{
	/*放弃主前置判断方式
	int is_main_fep;		//本机是否主前置
	unsigned long change_bit = 0;
	int tem_host_no = 0;

	is_main_fep = m_fe_nodes.at(m_index)->Is_Main_Server();
	if (is_main_fep == 0)	//主前置启动后不进行takeover等动作直接设置所有负荷
	{
		int fep_num = m_fe_nodes.size();
		for (int ncheck_fep_no = 0; ncheck_fep_no < fep_num; ncheck_fep_no++)		//轮询所有前置机的状态，并处理
		{
			tem_host_no = m_fe_nodes.at(ncheck_fep_no)->get_host_no();//获取轮询机的节点号
			change_bit = 1 << ncheck_fep_no;
			m_fe_nodes.at(ncheck_fep_no)->set_load(change_bit);
			m_fes_shm.attend_self(tem_host_no);		//设置该启动机器默认通道
		}
	}
	*/
	int tem_host_no = 0;
	unsigned long change_bit = 0;
	unsigned char main_zone_fep = 1;
	int nlocal_collect_zone = m_fe_nodes.at(m_index)->get_collect_zone();
	int n_local_host_no = m_fe_nodes.at(m_index)->get_host_no();

	int fep_num = m_fe_nodes.size();
	for (int ncheck_fep_no = 0; ncheck_fep_no < fep_num; ncheck_fep_no++)		//轮询所有前置机的状态，并处理
	{
		int n_zone = m_fe_nodes.at(ncheck_fep_no)->get_collect_zone();
		if (n_zone != nlocal_collect_zone || ncheck_fep_no == m_index)
		{
			continue;
		}

		tem_host_no = m_fe_nodes.at(ncheck_fep_no)->get_host_no();//获取轮询机的节点号
		int n_stat = m_fe_nodes.at(ncheck_fep_no)->get_stat();

		if (tem_host_no < n_local_host_no && n_stat == 0)	//有节点号更小,且运行的前置
		{
			main_zone_fep = 0;
			break;
		}

	}

	if (main_zone_fep == 1)		//本机为其采集区主机
	{
		for (int ncheck_fep_no = 0; ncheck_fep_no < fep_num; ncheck_fep_no++)		//轮询所有前置机的状态，并处理
		{
			int n_zone = m_fe_nodes.at(ncheck_fep_no)->get_collect_zone();
			if (n_zone != nlocal_collect_zone )
			{
				continue;
			}
			change_bit = 1 << ncheck_fep_no;
			m_fe_nodes.at(ncheck_fep_no)->set_load(change_bit);
			m_fes_shm.attend_self(tem_host_no);		//设置该启动机器默认通道
		}
	}



	activate();
	return 0;
}


int Port_watch::close(u_long flags)
{
	return 0;
}

int Port_watch::svc(void)
{
	while (true)
	{
		port_watch();
		ACE_OS::sleep(SLEEP_TIME);
	}
	
	return 0;
}

void Port_watch::create_host_nodes(void)
{
	int nodes[MAX_FE_SRV_NUM];
	ACE_OS::memset(nodes, 0, sizeof(nodes));

	int local_host_no = Net_stat::get_host_no();
	int node_num = Net_stat::get_fe_nodes(nodes, MAX_FE_SRV_NUM);

	for (int i = 0; i < node_num; i++)
	{
		if (local_host_no == nodes[i])
		{
			m_index = i;
		}

		Fe_node* node = new Fe_node(nodes[i], i);
		m_fe_nodes.push_back(node);
	}
}

void Port_watch::port_watch(void)
{
	ACE_Guard<ACE_Thread_Mutex> guard(m_check_watch_load_mutex);
	{
		send_stat();
		recv_stat();
// 		resolve_conflict();
		take_over();
	}
}

int Port_watch::send_stat(void)
{
	FE_MAIL_STRUCT* mail_struct = (FE_MAIL_STRUCT *)(m_mail_buf);

	ACE_OS::memset(mail_struct, 0, sizeof(FE_MAIL_STRUCT) + PORT_WATCH_MSG::get_lenth(MAX_CHAN_NUM));	//初始化

	PORT_WATCH_MSG* msg = (PORT_WATCH_MSG*)mail_struct->mail_body;
	mail_struct->mail_type = PORT_WATCH;

	msg->clear();

	msg->fe_index = m_index;
	msg->load_map = m_fe_nodes[m_index]->get_load();

	msg->give_back_flag = m_give_back_flag;
	m_give_back_flag = 0;

// 	time(&(msg->msg_time));

	int attend_gps_num = 0;

	//填写本机接管的GPS通道数据
	int gps_index = 0;
	for (int i = 0; i < MAX_GPS_NUM; i++)
	{
		if (m_fes_shm.gps_info_ptr[i].attend_host_no == m_fe_nodes[m_index]->get_host_no())
		{
			msg->attend_gps_num++;
			msg->gps[gps_index++] = m_fes_shm.gps_info_ptr[i];
		}
	}

	//填写本机接管的通道数据
	int channel_index = 0;
	for (int i = 0; i < m_fes_shm.fe_sys_data_ptr->max_chan_no; i++)
	{
		
		if (m_fes_shm.chan_data_ptr[i].curr_attend_host == m_fe_nodes[m_index]->get_host_no())
		{
			msg->attend_channel_num++;
			FE_CHAN_STAT* stat = msg->chan_stat + channel_index++;
			CHANDATA* chan_data = m_fes_shm.chan_data_ptr + i;			
// 			stat->rtu_no = 0;
			stat->channel_no = i;
			stat->bit_err_rate = chan_data->bit_err_rate;
			stat->stat = chan_data->stat;
			stat->master = chan_data->master_flag;
		}
	}

	int mail_lenth = sizeof(FE_MAIL_STRUCT) + msg->get_lenth();
	mail_struct->mail_lenth = mail_lenth;		//邮件总长度
	
// 	ACE_DEBUG((LM_DEBUG, "send port_watch_msg host_no = %d load = %2x channel num = %d!\n", m_fe_nodes[m_index]->get_host_no(),msg->load_map,msg->attend_channel_num));

	return Fe_mail::send_to_fe(mail_struct, mail_lenth);
// 	return 0;
}

void Port_watch::recv_stat(void)
{
	FE_MAIL_STRUCT* mail_struct = NULL;
	ACE_Message_Block *p_portwatch = 0;
	PORT_WATCH_MSG *pwm = 0;
	int timecnt = 0;

	m_already_give_up = 0;
	int nlocal_collect_zone = m_fe_nodes.at(m_index)->get_collect_zone();

	ACE_Time_Value tm = ACE_OS::gettimeofday() + ACE_Time_Value(0.2);
	while (getq(p_portwatch,&tm) >= 0)
	{
		mail_struct = (FE_MAIL_STRUCT *)p_portwatch->rd_ptr();
		pwm = (PORT_WATCH_MSG *)mail_struct->mail_body;

		m_fe_nodes.at(pwm->fe_index)->got_heartbeat(); //收到心跳包，重置超时

		//时标
// 		time_t current_time;
// 		time(&current_time);
// 		if(current_time - pwm->msg_time >5)
// 		{
// 			return;
// 		}
 
		if(pwm->fe_index == m_index) //如果是本机发出的消息，直接忽略
		{
			return;
		}

		int msg_host_no = m_fe_nodes.at(pwm->fe_index)->get_host_no();	//取得节点号

		int nmsg_fep_collect_zone = m_fe_nodes.at(pwm->fe_index)->get_collect_zone();

		if (nmsg_fep_collect_zone == nlocal_collect_zone )
		{
			m_already_give_up |= pwm->give_back_flag;
		}

		m_fe_nodes.at(pwm->fe_index)->set_load(pwm->load_map);


		//其他信息写入共享内存
		CHANDATA *chan_data = NULL;
		for (int k = 0; k < pwm->attend_channel_num; k++)
		{
			chan_data = m_fes_shm.chan_data_ptr + pwm->chan_stat[k].channel_no;
			chan_data->stat = pwm->chan_stat[k].stat;
			chan_data->bit_err_rate = pwm->chan_stat[k].bit_err_rate;
			chan_data->master_flag = pwm->chan_stat[k].master;
			// 				chan_data->curr_attend_host = msg_host_no;

		}
		CHANPARA *chan_para = NULL;
		for (int k = 0; k < pwm->attend_gps_num; k++)
		{
			chan_para = m_fes_shm.chan_para_ptr + pwm->gps[k].channel;
			*(m_fes_shm.gps_info_ptr + chan_para->gps_priority) = pwm->gps[k];
		}


		timecnt++;

// 		ACE_DEBUG((LM_DEBUG, "recv port_watch_msg host_no = %d load = %2x channel num = %d!\n", msg_host_no,pwm->load_map,pwm->attend_channel_num));

// 		if (timecnt > 0)
// 		{
// 			break;
// 		}
	}//while (getq(p_portwatch,&tm) >= 0)

}

void Port_watch::resolve_conflict(void)
{
	/*多台前置接管同一台前置的处理*/
	int fep_num = m_fe_nodes.size();
	unsigned long local_load = m_fe_nodes.at(m_index)->get_load();
	int calc_local_load = m_fe_nodes.at(m_index)->calc_load();
	unsigned long conflict_out = 0;
	unsigned long conflict_index = 0;
	int one_host_no = 0;
	int ano_host_no = 0;

	for(int i = 0; i < fep_num; i++)//轮询除本机之外其他所有前置，解决冲突
	{
		if (i == m_index)		
		{
			continue;
		}
		one_host_no = m_fe_nodes.at(i)->get_host_no();
		conflict_out = local_load & m_fe_nodes.at(i)->get_load();

		if (conflict_out != 0)		//当前前置的确与其他前置冲突
		{
			bool conflict_bit[sizeof(unsigned long)];

			for (int j = 0; j < fep_num; j++)		//比较其他前置
			{
				ano_host_no = m_fe_nodes.at(j)->get_host_no();

				if (((conflict_out >> j) & 0x00000001) == 1) 
				{
					conflict_index = 0;
					conflict_index = 1 << j;

					if (j == m_index)//冲突的前置机默认由本前置接管
					{
						m_fes_shm.attend_self(m_fe_nodes.at(m_index)->get_host_no());
						
					}
					else if(j == i)//冲突的前置机默认由对方接管
					{
						m_fe_nodes.at(m_index)->set_load(local_load & (~conflict_index));
						m_fes_shm.attend_host_switch(ano_host_no,-1);

					}
					else if (m_fe_nodes.at(i)->calc_load() < calc_local_load)//对方负荷比本前置机小
					{
						m_fe_nodes.at(m_index)->set_load(local_load & (~conflict_index));
						m_fes_shm.attend_host_switch(ano_host_no,-1);

					}

					ACE_DEBUG((LM_DEBUG, "reslov confilct!\n"));
				}
		
			}//for (int j = 0; j < fep_num; j++)
	

		}//if (conflict_out != 0)
	}//for(int i = 0; i < fep_num; i++)
}

void Port_watch::take_over(void)
{
	int fep_num = m_fe_nodes.size();
	int local_host_no = m_fe_nodes.at(m_index)->get_host_no();
	int tem_host_no = 0;
	unsigned long change_bit = 0;
	bool is_local_take = true;
	unsigned long local_load = m_fe_nodes.at(m_index)->get_load();
	int calc_local_load = m_fe_nodes.at(m_index)->calc_load();

	int local_collect_zone = m_fe_nodes.at(m_index)->get_collect_zone();

	int take_over_load;

	int fep_stat;
	unsigned long fep_load;
	int check_fep_zone;

	for (int ncheck_fep_no = 0; ncheck_fep_no < fep_num; ncheck_fep_no++)		//轮询所有前置机的状态，并处理
	{
		//获取轮询机的节点号、接管其负荷时的负荷位、当前的状态、及负荷
		tem_host_no = m_fe_nodes.at(ncheck_fep_no)->get_host_no();
		change_bit = 1 << ncheck_fep_no;
		fep_stat = m_fe_nodes.at(ncheck_fep_no)->get_stat();
		fep_load = m_fe_nodes.at(ncheck_fep_no)->get_load();
		check_fep_zone = m_fe_nodes.at(ncheck_fep_no)->get_collect_zone();

		if (check_fep_zone != local_collect_zone) //非本区前置不处理其状态
		{
			continue;
		}


		if (fep_stat == 0 && fep_load == 0)		//从停止到运行状态
		{	
			if ( ncheck_fep_no == m_index && m_already_give_up )	//启动本机默认接管通道
			{ 
				m_fe_nodes.at(m_index)->set_load(change_bit);
				m_fes_shm.attend_self(tem_host_no);		//设置该启动机器默认通道
			}
			else if ((change_bit & local_load) != 0 )		//本机接管了运行机器的通道，先需要释放
			{
				m_fe_nodes.at(m_index)->set_load(local_load & (~change_bit));
				m_fes_shm.attend_host_switch(tem_host_no,-1);		//只抛弃当前接管机器
				m_give_back_flag = 1;		//归还通道
			}

		} //end 停止到运行判断
		else if(fep_stat == -1 && fep_load != 0)		//从运行到宕机状态前置
		{
			if (ncheck_fep_no == m_index)//如果是本机,放弃接管.
			{
				continue;
			}

			is_local_take = true;

			for (int nfep_take_over_no = 0; nfep_take_over_no < fep_num; nfep_take_over_no++)				//判断本机是不是节点号最小，并确认是否接管
			{
				int stat = m_fe_nodes.at(nfep_take_over_no)->get_stat();
				int nfeo_take_over_zone = m_fe_nodes.at(nfep_take_over_no)->get_collect_zone();

				if (nfep_take_over_no == ncheck_fep_no || stat == -1 || nfeo_take_over_zone != check_fep_zone) // 若是宕机的机器或非本区前置，忽略 
				{
					continue;
				}
// 				else if (m_fe_nodes.at(nfep_take_over_no)->calc_load() < calc_local_load)
// 				{
// 					is_local_take = false;
// 					break;
// 				}
// 				else if (m_fe_nodes.at(nfep_take_over_no)->calc_load() == calc_local_load)
// 				{
					if (nfep_take_over_no < m_index )	
					{
						is_local_take = false;
						break;

					}
// 				}
			}//for (int j = 0; j < fep_num; j++)

			if(is_local_take)//本机接管,条件为节点号最小
			{
				if ((fep_load & local_load) == 0 )		
				{
					m_fe_nodes.at(m_index)->set_load(local_load | fep_load);

					local_load = m_fe_nodes.at(m_index)->get_load();
					
					int take_fep_no = 0;
					const int LONG_BIT_SIZE = 32;
					unsigned long mask = 0x01;

					for (int i = 0; i < LONG_BIT_SIZE; i++)			//将其接管的前置全部接管
					{
						if ((fep_load & mask) != 0)
						{
							take_fep_no = m_fe_nodes.at(i)->get_host_no();
							m_fes_shm.attend_host_switch(take_fep_no,local_host_no);
						}
						mask = mask << 1;
					}
				}
			}
			m_fe_nodes.at(ncheck_fep_no)->set_load(0);
		} //end 运行到停止判断
	}//for (int ncheck_fep_no = 0; ncheck_fep_no < fep_num; ncheck_fep_no++)

}

