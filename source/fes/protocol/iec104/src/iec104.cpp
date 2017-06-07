/**
@file iec104.cpp
@brief 104规约处理源文件

@author lujiashun 
@version 1.0.0
@date 2016-08-30
*/

#include "iec104.h"

#include "fes/timer.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"
#include "fes/fertfile.h"

#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

const char* const version_string = "1.0.0";
const char* const desc_string = "the very beginning";

//连接不成功后，等待2秒再尝试连接
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC104配置项
*/
static IEC104_CONFIG iec104_config[MAX_CHAN_NUM];

//预置的控制帧报文
unsigned char TEST_FR_VAL[6]={0x68,0x04,0x43,0x00,0x00,0x00};
unsigned char TEST_FR_ACK[6]={0x68,0x04,0x83,0x00,0x00,0x00};
unsigned char STOP_DT_VAL[6]={0x68,0x04,0x13,0x00,0x00,0x00};
unsigned char START_DT_VAL[6]={0x68,0x04,0x07,0x00,0x00,0x00};
unsigned char S_FRAME_ACK[6]={0x68,0x04,0x01,0x00,0x00,0x00};

FETIME T101Time_To_Realtime(CP24Time2a& SoeTime);	//CP24time计算为实际时间


Base_protocol* get_protocol(const int& channel)
{
	return new Iec104(channel);
}

void get_config(void)
{
	//todo
	//从数据库读取104配置信息，写入iec104_config
	struct IEC104_CONFIG_
	{
		unsigned short channel_no; //通道号
		unsigned short  yx_type;	//遥信类型
		unsigned short soe_type;	//SOE类型
		unsigned short  yc_type;	//遥测类型
		unsigned short  kwh_type; //电度类型

		unsigned short  yk_type;	//遥控类型，单点或双点


		short call_all_data_gap;///<总召间隔，为-1时不总召
		short call_kwh_gap;		///<召唤电度间隔，为-1时不召唤
		short send_time_gap;	///<对时间隔，为-1时不对时

		unsigned short yx_begin;	///<遥信起始地址
		unsigned short yc_begin;	///<遥测起始地址
		unsigned short yk_begin;	///<遥控起始地址
		unsigned short kwh_begin;	///<电度起始地址

		unsigned short t0;		///<t0超时，建立连接超时
		unsigned short t1;		///<t1超时时间，发送一个I帧或U帧后，必须在t1时间内接收到对方的确认，否则认为连接有问题，需要关闭重新连接
		unsigned short t2;		///<t2超时时间，接收一个I帧后，在t2时间内未接收到新的I帧，发送S帧进行确认
		unsigned short t3;		///<t3超时时间，接收到对方一帧报文后，经过t3时间没有接收到新的报文，需要向对方发送U帧测试

		unsigned short test_fr_cnt;	//未回复的测试帧限值
		unsigned short k;		///k值，有k个I帧未得到对方S帧确认时，停止传输，取值范围1-32767
		unsigned short w;		///<w值，最迟接收到对方w个I帧时，向对方发送S帧，取值范围1-32767

		unsigned char check_ns_nr;		///<检查发送接收序号，序号错乱需要关闭连接，1为检查，0为不检查
		unsigned char chg_to_soe;		///<遥信转soe，1为SOE转遥控，0为不转 
		unsigned char soe_to_chg;	///<soe转遥信，1为SOE转遥控，0为不转
		unsigned char grp_yc_check_chg_flag;	///<成组单点遥信检查变位标志，1为检查，0为不检查
	};
	IEC104_CONFIG_ * special_config = NULL;

	int ret = Db_connector::select("select * from iec104_special_config order by  channel_no asc", (void **)&special_config);
	int rcd_cnt = ret;		//规约特殊配置元素数量

	if(ret < 0)	//访问数据库出错
	{
		return;
	}

// 	ACE_DEBUG((LM_DEBUG, "read iec104 special \n"));

	for (int nnum = 0; nnum < rcd_cnt; ++nnum)
	{
		unsigned short channel_no = special_config->channel_no;

		iec104_config[channel_no].yx_type = special_config->yx_type;
		iec104_config[channel_no].soe_type = special_config->soe_type;
		iec104_config[channel_no].kwh_type = special_config->kwh_type;

		iec104_config[channel_no].call_all_data_gap = special_config->call_all_data_gap;
		iec104_config[channel_no].call_kwh_gap = special_config->call_kwh_gap;
		iec104_config[channel_no].send_time_gap = special_config->send_time_gap;

		iec104_config[channel_no].yx_begin = special_config->yx_begin;
		iec104_config[channel_no].yc_begin = special_config->yc_begin;
		iec104_config[channel_no].yk_begin = special_config->yk_begin;
		iec104_config[channel_no].kwh_begin = special_config->kwh_begin;

		iec104_config[channel_no].t0 = special_config->t0;
		iec104_config[channel_no].t1 = special_config->t1;
		iec104_config[channel_no].t2 = special_config->t2;
		iec104_config[channel_no].t3 = special_config->t3;

		iec104_config[channel_no].test_fr_cnt = special_config->test_fr_cnt;
		iec104_config[channel_no].k = special_config->k;
		iec104_config[channel_no].w = special_config->w;

		iec104_config[channel_no].check_ns_nr = special_config->check_ns_nr;
		iec104_config[channel_no].chg_to_soe = special_config->chg_to_soe;
		iec104_config[channel_no].soe_to_chg = special_config->soe_to_chg;
		iec104_config[channel_no].grp_yc_check_chg_flag = special_config->grp_yc_check_chg_flag;
		
		special_config++;
	}
}

char* get_version(void)
{
	return const_cast<char*>(version_string);
}

char* get_desc(void)
{
	return const_cast<char*>(desc_string);
}


Iec104::Iec104(const int& channel)
:Base_protocol(channel), m_config_ptr(iec104_config + channel)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 constructor\n"));

	m_common_addr = m_chanpara->common_addr;	//公共地址
}

Iec104::~Iec104()
{

}

void Iec104::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 init\n"));

	m_t0_timer.restart();
	m_t1_timer.restart();
	m_t2_timer.restart();
	m_t3_timer.restart();

	m_call_all_data_flag = NO_NEED;	
	m_call_kwh_data_flag = NO_NEED;	
	m_sync_time_flag = NO_NEED;
	m_rtu_init_flag = NO_NEED;

	m_bt1_timeout = false;
	m_bt2_timeout = false;
	m_bstartdt_flag = true;
	m_btestfr_flag = false;
	m_bSFrame_flag = false;

	m_call_all_data_timer.restart();	
	m_call_kwh_timer.restart();		
	m_sync_time_timer.restart();

	m_v_recv = 0;
	m_v_send = 0;
	m_v_ack = 0;

	m_current_ykno = -1;
	m_current_ykoder = 0;

	m_yx_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr[m_channel_no].kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

	ACE_OS::memset(m_wave_file_list,0,sizeof(WAVE_LIST)*WAVE_FILE_MAX);
	ACE_OS::memset(&m_cur_wave_file,0,sizeof(WAVE_LIST_RUN));
	m_Manual_wave = 0;
	m_wave_num_pos = 0;
}

void Iec104::protocol_proc(void)
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;
	while (!m_nEnd)
 	{
// 		ACE_DEBUG((LM_DEBUG, "第一重循环\n"));
		short curr_attend_host = m_chandata->curr_attend_host;
		if (curr_attend_host != curr_host_no || curr_attend_host == -1)	//非本前置接管通道,只进行外层循环
		{
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));
			continue;
		}
		
		if (m_dev_ptr)
		{
			m_dev_ptr->close_dev();
		}
 		if (open_dev() < 0 )		//网络端口未打开
 		{
			ACE_OS::sleep(CONN_RETRY_TIME);
			ACE_DEBUG((LM_DEBUG, "链路打开失败!\n"));
 			continue;
 		}
		else		//新的网络连接,重置104规约数据
		{
			init_protocol();
		}
 
		//报文循环处理
 		while (!m_nEnd)		//运行条件为该通道值班机为当前前置
 		{
// 			ACE_DEBUG((LM_DEBUG, "第二重循环\n"));
			int write_ret = 0;
			int read_ret = 0;
			int proc_ret = 0;
	  		m_write_lenth = 0;  //发送长度清零

			//检查邮件，并对邮件进行相关处理
			check_event();
			write_ret = Iec104_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
				break;
			}

			//接收报文，并判断各种超时
			read_ret  = recv_msg();	
			if (-1 == read_ret)			//读端口失败会导致本层循环退出
			{
				ACE_DEBUG((LM_DEBUG, "读端口失败\n"));
				break;
			}
			else if (0 == read_ret)		//未接收到数据，判断超时
			{
				if (m_t1_timer.elapsed() > m_config_ptr->t1 && m_bt1_timeout  )//发送I帧或U帧后无确认，t1超时关闭连接
				{
					ACE_DEBUG((LM_DEBUG, "接收数据超时\n"));
					close_dev();
					break;
				}

				if (m_t3_timer.elapsed() > m_config_ptr->t3)				//t3超时

				{
					m_btestfr_flag = true;
					m_t3_timer.restart();
				}
			}
			else	//接收正常，重置t3定时器
			{
				m_t1_timer.restart();
				m_bt1_timeout = false;
				m_t3_timer.restart();
/*				printf("m_v_recv = %d  w=%d \n",m_v_recv,m_config_ptr->w);
				fflush(stdout);
				if (!(m_v_recv % m_config_ptr->w))	//接收到对方w个I帧时，向对方发送S帧
				{
					if (m_v_recv)		//已接收到I帧
					{
						m_bSFrame_flag = true;
// 						send_s_frame();
					}
				}
*/
			}
			if (m_t2_timer.elapsed() > m_config_ptr->t2 && m_bt2_timeout )//接收I帧后 t2时间后发送s帧
			{
				m_bSFrame_flag = true;
				m_t2_timer.restart();
				m_bt2_timeout = false;
//				send_s_frame();
			}
			//计算总召、召电度及对时的时间
			if (m_config_ptr->call_all_data_gap != -1 && m_call_all_data_timer.elapsed() >= m_config_ptr->call_all_data_gap)
			{
				m_call_all_data_flag = NEED_CALL;
				m_call_all_data_timer.restart();
			}

			if (m_config_ptr->call_kwh_gap != -1 && m_call_kwh_timer.elapsed() >= m_config_ptr->call_kwh_gap)
			{
				m_call_kwh_data_flag = NEED_CALL;
				m_call_kwh_timer.restart();
			}

			if (m_config_ptr->send_time_gap != -1 && m_sync_time_timer.elapsed() >= m_config_ptr->send_time_gap)
			{
				m_sync_time_flag = NEED_CALL;
				m_sync_time_timer.restart();
			}

			//报文确认及回复
			if (m_read_lenth > 0)	//接收缓冲区有报文再进行处理
			{
// 				ACE_DEBUG((LM_DEBUG, "处理数据,缓冲区长度%d\n", m_read_lenth));
				FRAME_TYPE frame_type = judge_frame();
				switch (frame_type)
				{
				case I_FRAME:
					{
						proc_ret = deal_i_frame();
						if (!m_bt2_timeout)
						{
							m_t2_timer.restart();
							m_bt2_timeout = true;
						}
				//		m_t2_timer.restart();
				//		m_bt2_timeout = true;
					}
					break;
				case S_FRAME:
					proc_ret = deal_s_frame();
					break;
				case U_FRAME_TESTFR:
					proc_ret = deal_test_fr();
					break;
				case U_FRAME_TESTFR_ACK:
					proc_ret = deal_test_ack();
					break;
				case U_FRAME_STOPDT:
					proc_ret = deal_stop_dt();
					break;
				case U_FRAME_STARTDT:
					proc_ret = deal_start_dt();
					break;
				case INVALID_FRAME:
					proc_ret = deal_error_frame();
					break;
				default:
					break;
				}
				if (-1 == proc_ret)  //出错跳出循环 
				{
					break;
				}
				if (!(m_v_recv % m_config_ptr->w))	//接收到对方w个I帧时，向对方发送S帧
				{
					if (m_v_recv)		//已接收到I帧
					{
						m_bSFrame_flag = true;
					// 						send_s_frame();
					}
				}
			}


			if (m_chandata->curr_attend_host != curr_host_no)		//判断归还负荷
			{
				close_dev();
				break;
			}
			ACE_OS::sleep(ACE_Time_Value(0,5*1000));

		}//while (true) 二层循环
 	}//while (true)  一层循环
}

int Iec104::Iec104_send_proc(void)
{
	int ret = 0xff;
	if (m_bstartdt_flag)
	{
		ret = send_start_DT();
		m_bstartdt_flag = false;
	}
	else if (m_rtu_init_flag)		//处理终端初始化
	{
		ACE_DEBUG((LM_DEBUG, "rtu init\n"));
		m_rtu_init_flag = NO_NEED;
	}
	else if (m_btestfr_flag)
	{
// 		ACE_DEBUG((LM_DEBUG, "t3超时,发送测试帧\n"));
		ret = send_test_fr();
		m_btestfr_flag = false;
	}
	else if(m_bSFrame_flag)
	{
		ret = send_s_frame();
		m_bSFrame_flag = false;
	}
	else if (m_call_all_data_flag == NEED_CALL)		//邮件判据的报文发送
	{
		if (m_call_kwh_data_flag == ON_PROCESS)	//召唤电度的过程中，产生总召命令
		{
			//todo
		}
		ret = call_all_data();
	}
	else if (m_sync_time_flag == NEED_CALL)
	{
		if (m_call_all_data_flag != ON_PROCESS)		//总召过程中不进行对时
		{
			ret = sync_time();
		}
	}
	else if ( m_pi_all_num && (m_call_kwh_data_flag == NEED_CALL))	//若有电度量且需要召唤
	{
		if (m_call_all_data_flag != ON_PROCESS)		//总召过程中不进行电度召唤
		{
			ret = call_pi_data();
		}
	}


	if (((m_write_buf[2]&0x03) != 0x01) && ret== 0)		//发送I帧或U帧
	{
		m_t1_timer.restart();
		m_bt1_timeout = true;
	}
	return ret;
}


Iec104::FRAME_TYPE Iec104::judge_frame()
{
	int cur_position;
	int frame_Len = 0;

	FRAME_TYPE  judge;
	judge = INVALID_FRAME; //初始化为无效帧
	ACE_OS::memset(m_apdu_buf, 0, MAX_BUF_SIZE);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == FIRST)
		{
			frame_Len = m_read_buf[cur_position + 1] + 2;

			if((cur_position + frame_Len) > m_read_lenth)				//不完整报文保留至下次处理	
			{
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
				m_read_lenth -= cur_position;
				frame_Len = 0;
			}
			else
			{
				ACE_OS::memcpy(m_apdu_buf, &m_read_buf[cur_position], frame_Len);   //拷贝一帧完整报文待处理

				//将m_read_buf中未处理的报文移位
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position + frame_Len], m_read_lenth - frame_Len - cur_position);
				m_read_lenth = m_read_lenth - cur_position - frame_Len;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//报文缓冲区超限，全部清空
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset(&m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth);	//移位后的数据区需初始化
			break;
		}
	}
	if (cur_position == m_read_lenth)	//整个报文区数据都不对，全部清空
	{
		m_read_lenth = 0;
		ACE_OS::memset(m_read_buf, 0, REC_LEN);	
	}
	
	if (frame_Len == 0)		//报文保留至下次处理
	{
		return judge;
	}
	if ((m_apdu_buf[0] != FIRST) || (m_apdu_buf[1] < 4))	//无效帧处理
	{
		return judge;
	}

	P104_APDU *apdu_104;
	apdu_104=(P104_APDU*)&m_apdu_buf[0];
	cur_position += 2;   //移到控制域部分

	switch (m_apdu_buf[2]&0x03)
	{
	case 0x00:
	case 0x02:
		{
			m_v_recv++;		//接收到I帧后，接收计数器累加
			judge = I_FRAME;
		}
		break;
	case 0x01:
		judge = S_FRAME;
		break;
	case 0X03:		//U_FRAME
		switch(apdu_104->apci.type_u.u_type) //控制域类型判断
		{
		case STARTDT_CON:  //子站启动激活响应
			judge = U_FRAME_STARTDT;
			break;
		case STOPDT_CON:
			judge = U_FRAME_STOPDT; //子站停止激活相应
			break;
		case TESTFR_ACT:  //子站发送测试帧
			judge = U_FRAME_TESTFR;
			break;
		case TESTFR_CON:  //子站收到主站测试帧并确认
			judge = U_FRAME_TESTFR_ACK;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	} 
	return judge;
}

int Iec104::deal_i_frame(void)
{
	int ret = 0;


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	switch (apdu_104->asdu.type)
	{
	case M_SP_NA_1:							//yx处理
		ret = deal_sequence_M_SP_NA_1();
		break;
	case M_DP_NA_1:
		ret = deal_sequence_M_DP_NA_1();
		break;
	case M_PS_NA_1:
		ret = deal_sequence_M_PS_NA_1();			
		break;
	case M_SP_TA_1:							//yxsoe处理
		ret = deal_sequence_M_SP_TA_1();
		break;
	case M_DP_TA_1:
		ret = deal_sequence_M_DP_TA_1();
		break;
	case M_SP_TB_1:
		ret = deal_sequence_M_SP_TB_1();
		break;
	case M_DP_TB_1:
		ret = deal_sequence_M_DP_TB_1();		
		break;
	case M_ME_NA_1:							//yc处理
	case M_ME_NB_1:
		ret = deal_sequence_M_ME_NA_1();
		break;
	case M_ME_TA_1:
		ret = deal_sequence_M_ME_TA_1();
		break;
	case M_ME_ND_1:
		ret = deal_sequence_M_ME_ND_1();
		break;
	case M_ME_NC_1:
		ret = deal_sequence_M_ME_NC_1();			
		break;
	case M_ME_TE_1:
		ret = deal_sequence_M_ME_TE_1();
		break;
	case M_ME_TF_1:
		ret = deal_sequence_M_ME_TF_1();
		break;
	case M_IT_NA_1:							//kwh处理
		ret = deal_sequence_M_IT_NA_1();		
		break;
	case M_IT_TA_1:  //暂无
		break;
	case M_IT_TB_1:
		ret = deal_sequence_M_IT_TB_1();
		break;
	case M_BO_NA_1:		//7:子站远动终端状态处理 暂无
		break;
	case C_SC_NA_1:
	case C_DC_NA_1:
		{
			proc_yk_frame();			//yk处理
		}
		break;
	case C_RC_NA_1://47://升降返校
		//		A_DAResult(StaNo,ChNo,DataHead,comp);
		break;
	case C_IC_NA_1:		//总召
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)
			{
				m_call_all_data_flag = NO_NEED;		//总召唤结束,确认不做处理
			}
		}
		break;
	case C_CI_NA_1:		//电度召唤
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)
			{
				m_call_kwh_data_flag = NO_NEED;		//电度召唤结束,确认不做处理
			}
		}
		break;
	case C_CS_NA_1:		//对时确认
		{
			ret = 0;
			if ((apdu_104->asdu.cot & 0x3f) == COT_ACTCON)
			{
				m_chandata->sync_time_success = 1;
				m_sync_time_flag = NO_NEED;		//对时结束
			}
		}
		break;
	case 141:
	case 145:
		{
			deal_prot_frame();
		}
	default:
		break;
	} // end switch(apdu_104->asdu.type)

	if (ret == -1)
	{
		ACE_DEBUG((LM_DEBUG, "数据处理出错\n"));
	}

	return ret;
}

#pragma pack(1)
int Iec104::deal_s_frame(void)
{
	int ret = 0;

	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	int nrecv_count = apdu_104->apci.type_s.nr >> 1;
	
	if (nrecv_count != m_v_send)		
	{
		ACE_DEBUG((LM_DEBUG, "接收子站S帧，计数器出错\n"));		//暂不做S帧计数器出错处理，打印信息,处理方式也有待考虑
	}

	return ret;  //无返回值
}

int Iec104::send_s_frame(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(S_FRAME_ACK);
	ACE_OS::memcpy(m_write_buf, S_FRAME_ACK, m_write_lenth);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.type_s.nr =  m_v_recv << 1;  //接收序列号，2字节

	return send_msg();
}

#pragma pack()

int Iec104::send_test_fr(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(TEST_FR_VAL);
	ACE_OS::memcpy(m_write_buf, TEST_FR_VAL, m_write_lenth);
	return send_msg();

}

int Iec104::deal_test_fr(void)
{
	//接收子站测试帧，发送确认帧
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(TEST_FR_ACK);
	ACE_OS::memcpy(m_write_buf, TEST_FR_ACK, m_write_lenth);
	return send_msg();
}

int Iec104::deal_test_ack(void)
{
	return 0;	//接收子站确认帧，不做处理
}

int Iec104::deal_error_frame(void)
{
 	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

int Iec104::send_start_DT(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(START_DT_VAL);
	ACE_OS::memcpy(m_write_buf, START_DT_VAL, m_write_lenth);

// 	ACE_DEBUG((LM_DEBUG, "send start DT!\n"));
	return send_msg();
}

int Iec104::deal_start_dt(void)
{
	//激活链路后，进行总召等
	m_call_all_data_flag = NEED_CALL;	
	m_call_kwh_data_flag = NEED_CALL;	
	m_sync_time_flag = NEED_CALL;

	return 0;
}

int Iec104::send_stop_DT(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_lenth = sizeof(STOP_DT_VAL);
	ACE_OS::memcpy(m_write_buf, STOP_DT_VAL, m_write_lenth);

	ACE_DEBUG((LM_DEBUG, "send stop dt!\n"));
	return send_msg();
}

int Iec104::deal_stop_dt(void)
{
	return 0;
}

#pragma pack(1)
int Iec104::call_all_data(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节
	apdu_104->asdu.type = C_IC_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;
	pt->qdi = QDI_GENERAL_CALL;
	m_write_lenth = apdu_104->apci.apdu_len + 2;
	
	m_call_all_data_flag = ON_PROCESS;	

	m_v_send++; 
	return send_msg();
}

int Iec104::call_pi_data(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节
	apdu_104->asdu.type = C_CI_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;
	pt->qdi = 0x45;
	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_call_kwh_data_flag = ON_PROCESS;

	m_v_send++; 
	return send_msg();
}

int Iec104::sync_time(void)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = SYNC_VALUE;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节
	apdu_104->asdu.type = C_CS_NA_1;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot = COT_ACT;
	apdu_104->asdu.addr = m_common_addr;

	SYNC_TIME_STRUCT* pt = (SYNC_TIME_STRUCT*)apdu_104->asdu.data.buf;
	pt->index_no = 0;
	pt->pos_holder = 0;

	FETIME tim;
	get_time(&tim);

	pt->cp56time.minute = tim.minute;
	pt->cp56time.hour = tim.hour;
	pt->cp56time.day = tim.day;
	pt->cp56time.month = tim.month;
	pt->cp56time.year = (((tim.year + 1900)-2000)&0xff);
	pt->cp56time.millsecond = tim.milisecond + tim.second*1000;

	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_v_send++; 

	m_sync_time_flag = ON_PROCESS;

	m_chandata->last_sync_time = ACE_OS::time(NULL);
	m_chandata->sync_time_success = 0;

	return send_msg();
}

int Iec104::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.apdu_len = YK_FRAME_LENTH;
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节
	apdu_104->asdu.type = m_config_ptr->yk_type;
	apdu_104->asdu.vsq = VSQ_SINGLE;
	apdu_104->asdu.cot =  (yk_flag == 2) ? COT_DEACT : COT_ACT;		//是否取消

	apdu_104->asdu.addr = m_common_addr;

	C_STRUCT* pt = (C_STRUCT*)apdu_104->asdu.data.buf;
	//todo 是否需要判断遥控号
	pt->index_no = m_config_ptr->yk_begin + ykno;
	pt->pos_holder = 0;
	
	//控合控分,由单点或双点遥控确认
	if (m_config_ptr->yk_type == C_SC_NA_1)		//单点遥控
	{
		if(bclosed)
			pt->qdi = STATE_SP_ON;
		else
			pt->qdi = STATE_SP_OFF;
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//双点遥控
	{
		if(bclosed)
			pt->qdi = STATE_DP_ON;
		else
			pt->qdi = STATE_DP_OFF;
	}

	//选择或执行
	if (yk_flag == 0)
	{
		pt->qdi |= 0x80;
	}

	m_current_ykno = ykno;				//存储当前下发遥控序号，以备校验
	m_current_ykoder = pt->qdi;			//存储当前下发遥控命令，以备校验

	m_write_lenth = apdu_104->apci.apdu_len + 2;

	m_v_send++;
	return send_msg();
}

int Iec104::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

 	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
 	apdu_104->apci.start_id = FIRST;  
 	apdu_104->apci.apdu_len = SETPOINT_FRAME_LENTH;
 	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
 	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节
 	apdu_104->asdu.type = C_SE_NC_1;
 	apdu_104->asdu.vsq = VSQ_SINGLE;
 	apdu_104->asdu.cot =  (yt_flag == 3) ? COT_DEACT : COT_ACT;		//是否取消
	apdu_104->asdu.addr = m_common_addr;

	SETPOINT_STRUCT* pt = (SETPOINT_STRUCT*)apdu_104->asdu.data.buf;
	//todo 是否需要判断遥控号
	pt->index_no = m_config_ptr->setpoint_begin + ytno;
	pt->pos_holder = 0;
	pt->index_value = set_value ;
	//选择或执行
// 	if (yt_flag == 0)
// 	{
// 		pt->qdi = 0x80;
// 	}
// 	else
// 	{
		pt->qdi = 0x00;
/*	}*/

	m_current_ykno = ytno;				//存储当前下发遥控序号，以备校验
	m_current_ykoder = pt->qdi;			//存储当前下发遥控命令，以备校验

 	m_write_lenth = apdu_104->apci.apdu_len + 2;

 	m_v_send++;
	return send_msg();
}

int Iec104::send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节

	apdu_104->apci.apdu_len = cmd_len + 6 + 4;	//4:接收发送序号长度，6：ASDU部分固定长度 cmd_len:prot组织报文长度

	apdu_104->asdu.addr = m_common_addr;

	unsigned char *Buff = apdu_104->asdu.data.buf;

	switch(protocol_type)
	{
	case FE_PROTTYPE_IEC103://恒星,光芒104嵌套标准103保护
		{
			apdu_104->asdu.type =  145;//类型标识
			apdu_104->asdu.vsq = VSQ_SINGLE; //可变结构限定词(VSQ)
			apdu_104->asdu.cot =  COT_REQ;		//传送原因

			apdu_104->apci.apdu_len += 3;//信息体对象地址长度
			*Buff++ = 0;//信息体对象地址 3字节
			*Buff++ = 0;
			*Buff++ = 0;

			for( int i=0; i<cmd_len ; i++)
			{
				*Buff++ = cmd[i];
			}
		}	//add end
		break;
	default:
		{
			for( int i=0; i<cmd_len ; i++)
			{
				*Buff++ = cmd[i];
			}
		}
		break;
	}

	m_write_lenth = apdu_104->apci.apdu_len + 2;
	m_v_send++;
	return send_msg();
}

int Iec104::send_WaveCommand(unsigned char Type, unsigned char* cmd_buf)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	P104_APDU *apdu_104 = (P104_APDU*)m_write_buf;
	apdu_104->apci.start_id = FIRST;  
	apdu_104->apci.type_i.ns = m_v_send << 1;   //发送序列号，2字节
	apdu_104->apci.type_i.nr =  m_v_recv << 1;  //接收序列号，2字节

	apdu_104->apci.apdu_len = 6 + 4;	//4:接收发送序号长度，6：ASDU部分固定长度

	apdu_104->asdu.addr = m_common_addr;

	unsigned char *buf = apdu_104->asdu.data.buf;

	char tempstr[32];
	memset(tempstr,0,32);
	unsigned int index = 0;

	

	if (Type == CONFIRM_NODE || Type == CONFIRM_FILE)
	{
		apdu_104->asdu.type = F_AF_NA_1;
	}
	else
	{
		apdu_104->asdu.type = F_SC_NA_1;
	}
	apdu_104->asdu.vsq = 1;
	apdu_104->asdu.cot = COT_FILE;	//文件传输原因
	if(Type == REQUTST_DIR)
		apdu_104->asdu.cot = COT_REQ;	//召唤目录原因

	// 	t104.DSD[ChNo].wave_file.file_type = 1;
	*buf++ = m_cur_wave_file.file_type;	//type

	//信息体地址  6902H~7000H
	if (Type == REQUTST_DIR)
	{
		*buf++ = 0x00;
		*buf++ = 0x00;
		*buf++ = 0x00;
		*buf++ = 0x00;
	}
	else
	{
		*buf++ = m_cur_wave_file.file_name[0];
		*buf++ = m_cur_wave_file.file_name[1];

		if (Type == SEL_FILE || Type == REQUTST_FILE)
		{
			*buf++ = 0x00;
		}
		else
			*buf++ = m_cur_wave_file.cur_nodename[0];
	}

	switch(Type)
	{
	case REQUTST_DIR:	//请求目录
		*buf++ = 0x00;
		break;
	case SEL_FILE:	//选择文件
		*buf++ = 0x01;
		break;
	case REQUTST_FILE:	//请求文件
		*buf++ = 0x02;
		break;
	case REQUTST_NODE:	//请求节
		*buf++ = 0x06;
		break;
	case CONFIRM_NODE:	//节认可
		{
			if (m_cur_wave_file.cur_node_lenth == m_cur_wave_file.sum_node_lenth)
			{
				*buf++ = 0x03;//认可
			}
			else
				*buf++ = 0x04;//否定
		}
		break;
	case CONFIRM_FILE:	//文件认可
		{
			if (m_cur_wave_file.file_lenth == m_cur_wave_file.sum_file_lenth)
			{
				*buf++ = 0x01;//认可

				if (m_cur_wave_file.file_type == 0x02 || m_cur_wave_file.file_type == 0x04)
				{
					break;
				}
				else		//连续召唤
				{
					m_Manual_wave = 2;
					m_cur_wave_file.file_type ++;	//召下一类型文件
					send_WaveCommand(Type,NULL);
				}

			}
			else
				*buf++ = 0x02;//否定
		}
		break;
	default:
		break;
	}


	return 0;
}

#pragma pack()

#pragma pack(1)

//以下函数为I帧所有类型的处理///////////////////////////////////////////////////////////////////////////////////////

//YX类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_SP_NA_1(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥信点号
	unsigned char yx_quelity = 1;		//yx质量位
	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{

		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		int i;
		for (i = 0; i < info_num; i++)
		{
			//质量位为1 不处理
			if ((*pt) & 0x80)
			{
				yx_quelity = 0;
			}
			else
			{
				yx_quelity = 1;
			}
			make_yx_real_data(data_no, *pt, yx_quelity);
			data_no++;
			pt++;
		}		
	}
	else			//每帧含信息体地址
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)(apdu_104->asdu.data.buf);
		int i;
		for (i = 0; i < info_num; i++)
		{
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;
			if ((yx_struct->yx_value) & 0x80)	//质量位为1 不处理
			{
				yx_quelity = 0;
			}
			else
			{
				yx_quelity = 1;
			}
			make_yx_real_data(data_no, yx_struct->yx_value, yx_quelity);
			yx_struct++;
		}		
	}
	
	return 0;
}

int Iec104::deal_sequence_M_DP_NA_1(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥信数据个数
	int data_no = 0;		//遥信点号
	unsigned char value = 0;

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{

		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;
	
		unsigned char* pt = (unsigned char*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		int i;
		for (i = 0; i < info_num; i++)
		{
			//双点遥信计算状态
			value  = *pt  & 0x03;			//三种状态存入
			if (value == 1)
			{
				value = 0;	//状态分
			}
			else if (value == 2)
			{
				value = 1;	//状态合
			}
			else
			{
				value = 2;	//状态无效
			}
			make_yx_real_data(data_no, value, 1);
			data_no++;
			pt++;
		}		
	}
	else			//每帧含信息体地址
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)(apdu_104->asdu.data.buf);
		int i;
		for (i = 0; i < info_num; i++)
		{ 
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;
		
			//双点遥信计算状态
			value  = yx_struct->yx_value  & 0x03;			//三种状态存入
			if (value == 1)
			{
				value = 0;	//状态分
			}
			else if (value == 2)
			{
				value = 1;	//状态合
			}
			else
			{
				value = 2;	//状态无效
			}
			make_yx_real_data(data_no, value, 1);

			yx_struct++;
		}		
	}

	return 0;
}

int Iec104::deal_sequence_M_PS_NA_1(void)			//need for check 该处理方式仍不健全，待考证
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;
	unsigned char value = 0;

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		SEQ_M_PS_NA_1* yx_struct = (SEQ_M_PS_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int j = 0; j< info_num; j++)		//信息体循环
		{
			for (int i=0; i<16; i++)		//16个为一组
			{
				if ( (yx_struct->statuscheck>>i) &0x01 )		//状态位至少检出到一次状态变化
				{
					data_no ++;
					if (data_no > m_chanpara->yx_num )		//yxnum超出
					{
						break;
					}
					value = (yx_struct->value>>i) & 0x01;

					make_yx_real_data(data_no, value, yx_struct->quanity);
				}
				else
				{
					ACE_DEBUG((LM_DEBUG,"yxno = %d yx has no change", data_no));
				}
			}	//处理结束一组
			yx_struct ++;	
		}

	}
	else
	{
		struct SEQ_M_PS_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_PS_NA_1 seq_data;
		};

		SEQ_M_PS_NA_1_B* yx_struct = (SEQ_M_PS_NA_1_B*)(apdu_104->asdu.data.buf);

		for (int j = 0; j< info_num; j++)
		{
			data_no = yx_struct->addr.addr1 - m_config_ptr->yx_begin;

			for (int i=0; i<16; i++)		//16个为一组
			{
				if ( (yx_struct->seq_data.statuscheck>>i) &0x01 )		//状态位至少检出到一次状态变化
				{
					data_no ++;
					if (data_no > m_chanpara->yx_num )		//yxnum超出
					{
						break;
					}
					value = (yx_struct->seq_data.value>>i) & 0x01;

					make_yx_real_data(data_no, value, yx_struct->seq_data.quanity);

				}
				else
				{
					ACE_DEBUG((LM_DEBUG,"yxno = %d yx has no change", data_no));
				}
			}		
		}
	}

	return 0;

}
//YX类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YXSOE类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_SP_TA_1()		//M_SP_TA_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥信点号
	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)(apdu_104->asdu.data.buf);

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //计算yx点号
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);
		//make event
		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return 0;
}

int Iec104::deal_sequence_M_DP_TA_1()		//M_DP_TA_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥信点号
	unsigned char value = 0;

	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)(apdu_104->asdu.data.buf);

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //计算yx点号
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	
	
		//双点遥信计算状态
		value = seq_struct->value & 0x03;
		if (value == 1)
		{
			value = 0;	//状态分
		}
		else if (value == 2)
		{
			value = 1;	//状态合
		}
		else
		{
			value = 2;	//状态无效
		}

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);
		//make event
		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}


	return 0;
}


int Iec104::deal_sequence_M_SP_TB_1()	//M_SP_TB_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥信点号
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)(apdu_104->asdu.data.buf);

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //计算yx点号

		//时间转换为FETIME
		soe_time.year = seq_struct->long_time.year + 100;
		soe_time.month = seq_struct->long_time.month;
		soe_time.day = seq_struct->long_time.day;
		soe_time.hour = seq_struct->long_time.hour;
		soe_time.minute = seq_struct->long_time.minute;
		soe_time.second = seq_struct->long_time.millsecond / 1000;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000;


		//make event
		make_event(soe_time, data_no, seq_struct->value);
// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);

		seq_struct++;
	}

	return 0;
}

int Iec104::deal_sequence_M_DP_TB_1()	//M_DP_TB_1
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int info_num = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥信点号
	unsigned char value = 0;
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)(apdu_104->asdu.data.buf);

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //计算yx点号
		//时间转换为FETIME
		soe_time.year = seq_struct->long_time.year + 100;
		soe_time.month = seq_struct->long_time.month;
		soe_time.day = seq_struct->long_time.day;
		soe_time.hour = seq_struct->long_time.hour;
		soe_time.minute = seq_struct->long_time.minute;
		soe_time.second = seq_struct->long_time.millsecond / 1000;
		soe_time.milisecond = seq_struct->long_time.millsecond %1000;

		//双点遥信计算状态
		value = seq_struct->value & 0x03;
		if (value == 1)
		{
			value = 0;	//状态分
		}
		else if (value == 2)
		{
			value = 1;	//状态合
		}
		else
		{
			value = 2;	//状态无效
		}

// 		make_yx_real_data(data_no, seq_struct->value, 1, &soe_time);

		//make event
		make_event(soe_time, data_no, seq_struct->value);


		seq_struct++;
	}

		return 0;
}
//YXSOE类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YC类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_ME_NA_1()		//9、11标识YC处理
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_NA_1 * seq_data = (SEQ_M_ME_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity);
			data_no++;		//遥测点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_ME_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_NA_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_NA_1_B);
		SEQ_M_ME_NA_1_B * yc_struct = (SEQ_M_ME_NA_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TA_1()	//10带时标遥测值
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号
	FETIME time;			//遥测发生时间
	SEQ_M_ME_TA_1 * yc_struct = (SEQ_M_ME_TA_1*)(apdu_104->asdu.data.buf);
	for (int i =0; i< datanum; ++i)
	{
		time = T101Time_To_Realtime(yc_struct->short_time);
		data_no = yc_struct->info_addr.addr1 - m_config_ptr->yc_begin;
		make_yc_real_data(data_no, yc_struct->value, yc_struct->quanity, &time);
		yc_struct++;
	}
	return 0;

}

int Iec104::deal_sequence_M_ME_NC_1()		//13标识YC处理
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_NC_1 * seq_data = (SEQ_M_ME_NC_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity);

			data_no++;		//遥测点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_ME_NC_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_NC_1 seq_data;
		};

		SEQ_M_ME_NC_1_B * yc_struct = (SEQ_M_ME_NC_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return 0;

}

int Iec104::deal_sequence_M_ME_ND_1()		//21:无品质描述遥测值
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		int16s* yc_data = (int16s*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, *yc_data, 1);
			data_no++;		//遥测点号依次增加
			yc_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_ME_ND_1_B
		{
			INFO_ADDR addr;
			int16s yc_data;
		};

		SEQ_M_ME_ND_1_B * yc_struct = (SEQ_M_ME_ND_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->yc_data, 1);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TE_1()//35 <带时标CP56Time2a的的测量值，标度化值
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号

	FETIME yc_time;

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_TE_1 * seq_data = (SEQ_M_ME_TE_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		//时间转换为FETIME
		yc_time.year = seq_data->long_time.year + 100;
		yc_time.month = seq_data->long_time.month;
		yc_time.day = seq_data->long_time.day;
		yc_time.hour = seq_data->long_time.hour;
		yc_time.minute = seq_data->long_time.minute;
		yc_time.second = seq_data->long_time.millsecond / 1000;
		yc_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity,&yc_time);
			data_no++;		//遥测点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_ME_TE_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_TE_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_TE_1_B);
		SEQ_M_ME_TE_1_B * yc_struct = (SEQ_M_ME_TE_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			yc_time.year = yc_struct->seq_data.long_time.year + 100;
			yc_time.month = yc_struct->seq_data.long_time.month;
			yc_time.day = yc_struct->seq_data.long_time.day;
			yc_time.hour = yc_struct->seq_data.long_time.hour;
			yc_time.minute = yc_struct->seq_data.long_time.minute;
			yc_time.second = yc_struct->seq_data.long_time.millsecond / 1000;
			yc_time.milisecond = yc_struct->seq_data.long_time.millsecond %1000;
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity,&yc_time);
			yc_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_ME_TF_1()//36 <带时标CP56Time2a的的测量值，短浮点数
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的遥测数据个数
	int data_no = 0;		//遥测点号

	FETIME yc_time;

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_TF_1 * seq_data = (SEQ_M_ME_TF_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		//时间转换为FETIME
		yc_time.year = seq_data->long_time.year + 100;
		yc_time.month = seq_data->long_time.month;
		yc_time.day = seq_data->long_time.day;
		yc_time.hour = seq_data->long_time.hour;
		yc_time.minute = seq_data->long_time.minute;
		yc_time.second = seq_data->long_time.millsecond / 1000;
		yc_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_yc_real_data(data_no, seq_data->value, seq_data->quanity,&yc_time);
			data_no++;		//遥测点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_ME_TF_1_B
		{
			INFO_ADDR addr;
			SEQ_M_ME_TF_1 seq_data;
		};

		int sizen = sizeof(SEQ_M_ME_TF_1_B);
		SEQ_M_ME_TF_1_B * yc_struct = (SEQ_M_ME_TF_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			yc_time.year = yc_struct->seq_data.long_time.year + 100;
			yc_time.month = yc_struct->seq_data.long_time.month;
			yc_time.day = yc_struct->seq_data.long_time.day;
			yc_time.hour = yc_struct->seq_data.long_time.hour;
			yc_time.minute = yc_struct->seq_data.long_time.minute;
			yc_time.second = yc_struct->seq_data.long_time.millsecond / 1000;
			yc_time.milisecond = yc_struct->seq_data.long_time.millsecond %1000;
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity,&yc_time);
			yc_struct ++;
		}

	}
	return 0;
}
//YC类型处理end///////////////////////////////////////////////////////////////////////////////////////

//KWH类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_sequence_M_IT_TB_1()		//0x25电度
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的电度数据个数
	int data_no = 0;		//遥脉点号
	FETIME kwh_time;

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//计算遥脉点号

		SEQ_M_IT_TB_1 * seq_data = (SEQ_M_IT_TB_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));

		//时间转换为FETIME
		kwh_time.year = seq_data->long_time.year + 100;
		kwh_time.month = seq_data->long_time.month;
		kwh_time.day = seq_data->long_time.day;
		kwh_time.hour = seq_data->long_time.hour;
		kwh_time.minute = seq_data->long_time.minute;
		kwh_time.second = seq_data->long_time.millsecond / 1000;
		kwh_time.milisecond = seq_data->long_time.millsecond %1000;

		for (int i =0; i< datanum; ++i)
		{
			make_kwh_real_data(data_no, seq_data->value, seq_data->quanity,&kwh_time);
			data_no++;		//遥脉点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_IT_TB_1_B
		{
			INFO_ADDR addr;
			SEQ_M_IT_TB_1 seq_data;
		};

		SEQ_M_IT_TB_1_B * kwh_struct = (SEQ_M_IT_TB_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//计算遥脉点号

			kwh_time.year = kwh_struct->seq_data.long_time.year + 100;
			kwh_time.month = kwh_struct->seq_data.long_time.month;
			kwh_time.day = kwh_struct->seq_data.long_time.day;
			kwh_time.hour = kwh_struct->seq_data.long_time.hour;
			kwh_time.minute = kwh_struct->seq_data.long_time.minute;
			kwh_time.second = kwh_struct->seq_data.long_time.millsecond / 1000;
			kwh_time.milisecond = kwh_struct->seq_data.long_time.millsecond %1000;

			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity,&kwh_time);
			kwh_struct ++;
		}

	}
	return 0;
}

int Iec104::deal_sequence_M_IT_NA_1()		//0x15电度
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	int datanum = apdu_104->asdu.vsq & 0x7F; //获取当前帧的电度数据个数
	int data_no = 0;		//遥脉点号

	if (apdu_104->asdu.vsq & 0x80)		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)(apdu_104->asdu.data.buf);
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//计算遥脉点号

		SEQ_M_IT_NA_1 * seq_data = (SEQ_M_IT_NA_1*)(apdu_104->asdu.data.buf + sizeof(INFO_ADDR));
		for (int i =0; i< datanum; ++i)
		{
			make_kwh_real_data(data_no, seq_data->value, seq_data->quanity);
			data_no++;		//遥脉点号依次增加
			seq_data ++;
		}
	}
	else		//每帧含信息体地址
	{
		struct SEQ_M_IT_NA_1_B
		{
			INFO_ADDR addr;
			SEQ_M_IT_NA_1 seq_data;
		};

		SEQ_M_IT_NA_1_B * kwh_struct = (SEQ_M_IT_NA_1_B*)(apdu_104->asdu.data.buf);
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//计算遥脉点号
			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity);
			kwh_struct ++;
		}

	}
	return 0;
}
//KWH类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YK类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::proc_yk_frame()
{
	char str[NORMALCHAR_LEN];


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int yk_ret = -1;
	C_STRUCT * yk_struct = (C_STRUCT*)(apdu_104->asdu.data.buf);
	int ret_ykno = yk_struct->index_no - m_config_ptr->yk_begin;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		ACE_OS::sprintf(str,"遥控公共地址错,正确地址:%d,返校地址:%d",m_common_addr,apdu_104->asdu.addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//遥控号出错
	{
		ACE_ERROR((LM_ERROR, "下发的遥控号和返校的遥控号不一致!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if ((apdu_104->asdu.cot & 0x3f) != COT_ACTCON && (apdu_104->asdu.cot & 0x3f) != COT_DEACTCON)	//传送原因错
	{
		ACE_OS::sprintf(str,"遥控返校的传送原因无效,cot = %02x",apdu_104->asdu.cot & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if (yk_struct->qdi != m_current_ykoder)	//遥控命令出错
	{
		ACE_OS::sprintf(str,"遥控命令错,下发命令:%02x 返校命令:%02x",m_current_ykoder,yk_struct->qdi);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if((apdu_104->asdu.cot & 0x40) == 0 )		//判断返校结果
	{
		ACE_OS::sprintf(str,"遥控返校正确!");
		yk_ret  = 0;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((apdu_104->asdu.cot & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"遥控返校不允许!");
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (!(yk_struct->qdi & 0x80))		//遥控执行或取消完成后
	{
		ACE_DEBUG((LM_DEBUG, "遥控动作完成!"));
		m_current_ykoder = 0;	//完成遥控后将遥控状态置位
	}

	//接收正确信息
	yk_ret = 0;
	return yk_ret;
}
//YK类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YT类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec104::proc_yt_frame()
{
	char str[NORMALCHAR_LEN];


	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	int yk_ret = -1;
	C_STRUCT * yk_struct = (C_STRUCT*)(apdu_104->asdu.data.buf);
	int ret_ykno = yk_struct->index_no - m_config_ptr->setpoint_begin;

	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		ACE_OS::sprintf(str,"设点公共地址错,正确地址:%d,返校地址:%d",m_common_addr,apdu_104->asdu.addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//遥控号出错
	{
		ACE_ERROR((LM_ERROR, "下发的设点号和返校的设点号不一致!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}
	if ((apdu_104->asdu.cot & 0x3f) != COT_ACTCON && (apdu_104->asdu.cot & 0x3f) != COT_DEACTCON)	//传送原因错
	{
		ACE_OS::sprintf(str,"设点返校的传送原因无效,cot = %02x",apdu_104->asdu.cot & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}


	if((apdu_104->asdu.cot & 0x40) == 0 )		//判断返校结果
	{
		ACE_OS::sprintf(str,"设点返校正确!");
		yk_ret  = 0;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((apdu_104->asdu.cot & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"设点返校不允许!");
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}


	if ((apdu_104->asdu.cot & 0x3f) == COT_ACTTERM)		//遥控执行或取消完成后
	{
		// ACE_DEBUG((LM_DEBUG, "遥控动作完成!"));
		m_current_ykoder = 0;	//完成遥控后将遥控状态置位
	}

	//接收正确信息
	yk_ret = 0;
	return yk_ret;
}
//YT类型处理end///////////////////////////////////////////////////////////////////////////////////////

//保护帧类型处理begin/////////////////////////////////////////////////////////////////////////////////
int Iec104::deal_prot_frame(void)
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	unsigned char sendflag = 0;
	unsigned char protocoltype = 0;
	if(apdu_104->asdu.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}

	char* mailbuf;
	unsigned char* Buff;
	mailbuf = static_cast<char*>(ACE_OS::malloc(1+256));
	int mail_lenth = 0;

	switch(apdu_104->asdu.type)
	{
	case 140:
	case 141://光芒保护事项
		{
			sendflag = 1;
			protocoltype = FE_PROTTYPE_IEC103;
			Buff = apdu_104->asdu.data.buf + 6;
			mail_lenth = ASDU_DATA_LEN - 6; //从信息对象地址开始为保护报文
		}
		break;
	case 145:	//IEC103 hex103  长度需要
		{
			*mailbuf = FE_PROTTYPE_IEC103;		
			Buff = apdu_104->asdu.data.buf + 6;
			mail_lenth = ASDU_DATA_LEN - 6; //从信息对象地址开始为保护报文
		}
		break;
	default:
		break;
	}

	if(sendflag==1)//保护事项
	{
		FEP_PROT_STRU protect;
		ACE_OS::memset(&protect,0,sizeof(protect));
		protect.terminalno = m_rtu_no;
		protect.protocoltype = protocoltype;
		protect.gram_len = mail_lenth;
		ACE_OS::memcpy((char *)protect.gram,Buff,mail_lenth);

		Fe_scasrv::fe_send_prot_event(&protect);
		Proc_manager::putdbg(G101DBG,"向SCADA服务器发送保护事项邮件");
	}
	else if (sendflag == 0)//定值操作
	{

		ACE_OS::memcpy((char *)(mailbuf+1),Buff,mail_lenth);
		Fe_mail::send_prot_ack((void*)mailbuf,mail_lenth+1,m_rtu_no);
		Proc_manager::putdbg(G101DBG,"向客户机发邮件");
	}

	return 0;
}
//保护帧类型处理end/////////////////////////////////////////////////////////////////////////////////

//录波处理begin////////////////////////////////////////////////////////////////////////
int Iec104::proc_wave_frame()
{
	P104_APDU *apdu_104 = (P104_APDU*)m_apdu_buf;
	unsigned char * ptr = apdu_104->asdu.data.buf;

	unsigned char * pFileCon = NULL;
	FertFile* myfile;
	static char filepath[256];
	int sub_rec = 0;
	FILE_SOF file_sof;
	unsigned short info_addr;	//保护装置地址
	unsigned short node_length;
	unsigned char file_type = 0;//文件类型
	char NOF[IEC_101_NOF];
	unsigned char NOS;
	unsigned char	ReadyQ;

	struct tm time_span;
	time_t time_change;
	int mseconds;
	int ngroup = 0;//线路组号
	int nstart_way = 0;//启动方式

	int wave_minute = 0;	//录波文件内容时间分钟
	int wave_msec = 0;	//录波文件内容时间秒

	int file_lenth = 0;
	unsigned char order_type = 0xff;

	unsigned char frame_pos = 0;

	if (apdu_104->asdu.vsq == 1)
	{
		info_addr = ptr[frame_pos]+ptr[frame_pos + 1]*256;
		frame_pos += 2;
		file_type = ptr[frame_pos++];	
		if (ptr[frame_pos++] != m_cur_wave_file.file_name[0] || ptr[frame_pos++] != m_cur_wave_file.file_name[1])
		{
			printf("NOF error!\n");
			fflush(stdout);
		}
		if (info_addr != m_cur_wave_file.info_addr)
		{
			printf("rtu address error!\n");
			fflush(stdout);
		}
		if (file_type != m_cur_wave_file.file_type)
		{
			printf("file type error!\n");
			fflush(stdout);
		}
	}
	else	//上送目录部分
	{
		if (apdu_104->asdu.type == F_DR_TA_1)		//目录
		{
			if (m_wave_num_pos == 0)
			{
				ACE_OS::memset(&(m_wave_file_list), 0, sizeof(WAVE_LIST));
			}

			if ( !apdu_104->asdu.vsq )
			{
				m_Manual_wave = 0;//无目录，手动位置0
				return 0;
			}

			for (int i = m_wave_num_pos; i < apdu_104->asdu.vsq + m_wave_num_pos; i++)
			{
				if (frame_pos > ASDU_DATA_LEN || m_wave_num_pos > WAVE_FILE_MAX)
				{
					break;
				}
				//循环读出目录
				m_wave_file_list[i].info_addrL = (ptr[frame_pos]) + (ptr[frame_pos+1]*256);
				frame_pos += 2;
				m_wave_file_list[i].file_type = ptr[frame_pos++];	

				m_wave_file_list[i].file_name[0] = ptr[frame_pos++];
				m_wave_file_list[i].file_name[1] = ptr[frame_pos++];

				m_wave_file_list[i].len = ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;
				frame_pos += 3;

				ACE_OS::memcpy(&(m_wave_file_list[i].sof), &(ptr[frame_pos++]), sizeof(FILE_SOF));
				// 				m_wave_file_list[i].sof = (FILE_SOF)ptr[frame_pos++];

				mseconds = ptr[frame_pos]+ptr[frame_pos+1]*256;	//取得毫秒时间
				frame_pos += 2;
				time_span.tm_sec =	mseconds/1000;
				time_span.tm_min = ptr[frame_pos++];
				time_span.tm_hour = ptr[frame_pos++];
				time_span.tm_mday = ptr[frame_pos++];
				time_span.tm_mon  = ptr[frame_pos++] - 1;
				time_span.tm_year = ptr[frame_pos++] + 2000 - 1900;

				time_change =  mktime(&time_span);

				m_wave_file_list[i].time_msec = mseconds%1000;
				m_wave_file_list[i].time_sec = time_change;
			}

			if (m_wave_file_list[apdu_104->asdu.vsq - 1].sof.LFD == 0)	//本帧目录文件未传送完，还有文件
			{
				m_wave_num_pos += apdu_104->asdu.vsq;
				return 0;
			}
			else
			{
				m_wave_num_pos = 0;
			}


			if (m_Manual_wave == 0)	//如果主动上送，选择第一个录波文件
			{
				m_cur_wave_file.info_addr = m_wave_file_list[0].info_addrL;
				m_cur_wave_file.file_name[0] = m_wave_file_list[0].file_name[0];
				m_cur_wave_file.file_name[1] = m_wave_file_list[0].file_name[1];

				// 				m_cur_wave_file.file_lenth = m_wave_file_list[0].len;   文件长度在文件准备就绪确定
				m_cur_wave_file.file_type = m_wave_file_list[0].file_type;
				//选择录波文件
				order_type = SEL_FILE;
			}
			else if (m_Manual_wave == 1)	//手动召唤
			{
				//todo 将目录传送至召唤工具
				char	*mailbuf;
				mailbuf = (char *)malloc( 512 );
				if(mailbuf==NULL)
				{	
					printf("申请内存失败!\n");
					return 0;
				}
				*mailbuf = 0;
				*(mailbuf+1) = apdu_104->asdu.vsq;
				int mail_lenth = sizeof(WAVE_LIST)*apdu_104->asdu.vsq;
				ACE_OS::memcpy((char *)(mailbuf+2),(char *)m_wave_file_list,sizeof(WAVE_LIST)*apdu_104->asdu.vsq);

				int ret = Fe_mail::send_prot_ack(mailbuf,mail_lenth+2,m_rtu_no);			

				if(mailbuf) free(mailbuf);
				mailbuf = NULL;
			}
			else if (m_Manual_wave == 2)	//连续召唤其他类型
			{
				order_type = SEL_FILE;
			}

			m_Manual_wave = 0;//目录接受成功后置0
		}
	}

	switch(apdu_104->asdu.type)
	{
	case F_FR_NA_1:	//文件准备就绪
		{
			file_lenth =  ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;	//文件长度
			frame_pos += 3;

			ReadyQ = ptr[frame_pos++];


			m_cur_wave_file.file_lenth = file_lenth;

			//请求文件
			if ( (ReadyQ & 0x80) == 0)
			{
				order_type = REQUTST_FILE;
			}
		}
		break;
	case F_SR_NA_1:	//节准备就绪
		{
			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			file_lenth =  ptr[frame_pos] + ptr[frame_pos+1]*256 + ptr[frame_pos+2]*256*256;	//节长度
			frame_pos += 3;

			ReadyQ = ptr[frame_pos++];

			m_cur_wave_file.cur_node_lenth = file_lenth; 

			//请求节文件
			if ( (ReadyQ & 0x80) == 0)
			{
				order_type = REQUTST_NODE;
			}
		}
		break;
	case F_LS_NA_1:	//最后的节,最后的段
		{
			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			ReadyQ = ptr[frame_pos++];

			if (ReadyQ == 0x01 || ReadyQ == 0x02)//文件认可
			{
				order_type = CONFIRM_FILE;

				myfile->SaveToSys();

			}
			else if (ReadyQ == 0x03 || ReadyQ == 0x04)//节认可
			{
				//todo 校验
				order_type = CONFIRM_NODE;
			}
		}
		break;
	case F_SG_NA_1:	//段
		{
			ACE_OS::memset(filepath,0,256);
			ACE_OS::sprintf(filepath,"%s/log/dat/wave/",getenv("NBENV"));

			m_cur_wave_file.cur_nodename[0] =  ptr[frame_pos++];

			m_cur_wave_file.cur_phase_lenth =  ptr[frame_pos++];//当前段长度

			pFileCon = ptr + frame_pos;

			//////////////////////////////////////////////////////////////////////////l录波处理
			if (m_cur_wave_file.sum_node_lenth == 0)		//当前节长度为0，创建文件
			{

				unsigned short nfile_name = m_cur_wave_file.file_name[0] + m_cur_wave_file.file_name[1] * 256;

				ACE_OS::sprintf(m_cur_wave_file.save_name,"%02d_%02d_%03d",m_rtu_no, nfile_name/1000, nfile_name % 1000);	//站号，召唤子站rtu号，毫秒

				switch(file_type)
				{
				case 1:
					ACE_OS::strcat(m_cur_wave_file.save_name,".cfg");
					break;
				case 2:
					ACE_OS::strcat(m_cur_wave_file.save_name,".dat");
					break;
				case 3:
					ACE_OS::strcat(m_cur_wave_file.save_name,".hdr");
					break;
				case 4:
					ACE_OS::strcat(m_cur_wave_file.save_name,".cfg+dat+hdr");
					break;
				}

				ACE_OS::strcat(filepath,m_cur_wave_file.save_name);

				myfile = new FertFile(filepath);

				int write_num = myfile->Save((char*)pFileCon,m_cur_wave_file.cur_phase_lenth,m_cur_wave_file.sum_file_lenth,FILE_WRITE_NEW,MYCHAR_BIT);
			}
			else
			{
				ACE_OS::strcat(filepath,m_cur_wave_file.save_name);

				int write_num = myfile->Save((char*)pFileCon,m_cur_wave_file.cur_phase_lenth,m_cur_wave_file.sum_file_lenth,FILE_WRITE_CON,MYCHAR_BIT);
			}

			m_cur_wave_file.sum_node_lenth += m_cur_wave_file.cur_phase_lenth;//当前节长度变大
			m_cur_wave_file.sum_file_lenth += m_cur_wave_file.cur_phase_lenth;
		}
		break;

	default:
		break;
	}


	//自动召唤剩余帧
	if(order_type != 0xff)
	{
		return send_WaveCommand(order_type, NULL);
	}
	return 0;
}

//录波处理end////////////////////////////////////////////////////////////////////////

#pragma pack()

//////////////////////////////////////////////////////////////////////////
//将规约上传的短时标时间，转换为真实时间
FETIME T101Time_To_Realtime(CP24Time2a& SoeTime)
{
	//此函数将101的时标转换成实用时间，此时间只有在一小时内上送才可计算
	struct tm TimeMinus,TimeAdd,TimeNochange;//分别是小时减1，小时加1，小时不变的时候计算出来的时间
	struct tm *UseTime;
	struct tm *CurTime;
	FETIME Time ;
	time_t ltime,tmptime;
	long Diff_Minus,Diff_Add,Diff_Nochange;

	time( &ltime );
	tmptime =  ltime - 3600;
	CurTime = localtime( &tmptime );
	//将小时分别减去、加上、不变，以便比较
	TimeMinus.tm_year = CurTime->tm_year;
	TimeMinus.tm_mon = CurTime->tm_mon;
	TimeMinus.tm_mday = CurTime->tm_mday;
	TimeMinus.tm_hour = CurTime->tm_hour;
	TimeMinus.tm_min = SoeTime.minutes;
	TimeMinus.tm_sec = SoeTime.millsecond / 1000;

	tmptime = ltime + 3600;
	CurTime = localtime( &tmptime );
	TimeAdd.tm_year = CurTime->tm_year;
	TimeAdd.tm_mon = CurTime->tm_mon;
	TimeAdd.tm_mday = CurTime->tm_mday;
	TimeAdd.tm_hour = CurTime->tm_hour;
	TimeAdd.tm_min = SoeTime.minutes;
	TimeAdd.tm_sec = SoeTime.millsecond / 1000;

	CurTime = localtime( &ltime );
	TimeNochange.tm_year = CurTime->tm_year;
	TimeNochange.tm_mon = CurTime->tm_mon;
	TimeNochange.tm_mday = CurTime->tm_mday;
	TimeNochange.tm_hour = CurTime->tm_hour;
	TimeNochange.tm_min = SoeTime.minutes;
	TimeNochange.tm_sec = SoeTime.millsecond / 1000;

	Diff_Minus = (long)fabs( double(mktime( &TimeMinus ) - ltime) );
	Diff_Add = (long)fabs( double(mktime( &TimeAdd ) - ltime) );
	Diff_Nochange = (long)fabs( double(mktime( &TimeNochange ) - ltime) );

	//选取与当前时间相差最小的一个时间值作为SOE的时间
	if (Diff_Minus<Diff_Add && Diff_Minus<Diff_Nochange)
	{
		UseTime = &TimeMinus;
	}
	else if (Diff_Add<Diff_Minus && Diff_Add<Diff_Nochange)
	{
		UseTime = &TimeAdd;
	}
	else if(Diff_Nochange<Diff_Minus && Diff_Nochange<Diff_Add)
	{
		UseTime = &TimeNochange;
	}
	else
	{
		UseTime = &TimeNochange;
	}

	Time.milisecond = SoeTime.millsecond % 1000;
	Time.second = UseTime->tm_sec;
	Time.minute = UseTime->tm_min;
	Time.hour = UseTime->tm_hour;
	Time.day = UseTime->tm_mday;
	Time.month = UseTime->tm_mon+1;
	Time.year = UseTime->tm_year+1900;

	return Time;
}
