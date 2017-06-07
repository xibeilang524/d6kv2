/**
@file CDT.cpp
@brief 104规约处理源文件

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#include "CDT.h"

#include "fes/timer.h"
#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"

#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <iostream>
#include <time.h>
using namespace std;

const char* const version_string = "1.0.0";
const char* const desc_string = "the very beginning CDT";

/**
@brief CDT配置项
*/
static unsigned char tbch0[256];					//BCH 校验码
static CDT_CONFIG cdt_config[MAX_CHAN_NUM];
const int CONN_RETRY_TIME = 2;


Base_protocol* get_protocol(const int& channel)
{
	return new CDT(channel);
}

void get_config(void)
{
	//todo
	//从数据库读取CDT配置信息，写入CDT_config
	
}

char* get_version(void)
{
	return const_cast<char*>(version_string);
}

char* get_desc(void)
{
	return const_cast<char*>(desc_string);
}


CDT::CDT(const int& channel)
:Base_protocol(channel), m_config_ptr(cdt_config + channel)
{
	ACE_DEBUG((LM_DEBUG, "CDT constructor\n"));

	MakeBCHList();  //算出校验队列

	m_common_addr = m_chanpara->common_addr;
	m_master_addr = m_chanpara->master_addr;
	ACE_OS::memset(&m_soe_time, 0, sizeof(FETIME));

}

CDT::~CDT()
{
	ACE_OS::memset(&m_soe_time, 0, sizeof(FETIME));

}

void CDT::init_protocol(void)
{
	ACE_DEBUG((LM_DEBUG, "CDT init\n"));

	m_call_all_data_flag = NO_NEED;	
	m_call_kwh_data_flag = NO_NEED;	
	m_sync_time_flag = NO_NEED;
	m_rtu_init_flag = NO_NEED;

	m_call_all_data_timer.restart();	
	m_call_kwh_timer.restart();		
	m_sync_time_timer.restart();

	m_timeout_timer.restart();


	m_current_ykno = -1;
	m_current_ykoder = 0;

	m_yx_all_num = m_chanpara->yx_num;
	m_yc_all_num = m_chanpara->yc_num;
	m_pi_all_num = m_chanpara->kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

}

void CDT::protocol_proc(void)
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;
	while (!m_nEnd)
 	{
		ACE_DEBUG((LM_DEBUG, "第一重循环\n"));

		short curr_attend_host = m_chandata->curr_attend_host;
		if (curr_attend_host != curr_host_no)	//非本前置接管通道,只进行外层循环
		{
			ACE_OS::sleep(ACE_Time_Value(0,1*1000));
			continue;
		}

		open_dev();
		
 		if (!is_open())//端口未打开
 		{
			ACE_OS::sleep(CONN_RETRY_TIME);
			ACE_DEBUG((LM_DEBUG, "网络未连接\n"));
 			continue;
 		}
 		
		//新的网络连接,重置104规约数据
 		init_protocol();

		//报文循环处理
 		while (!m_nEnd)
 		{
//  			ACE_DEBUG((LM_DEBUG, "第二重循环\n"));
			int ret= 0;
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

			//检查邮件，并对邮件进行相关处理
			m_write_lenth = 0;  //发送长度清零
			check_event();
			{
// 				CDT_send_proc();
			}

			ret = recv_msg();

			//读端口失败都会导致本层循环退出
			if (-1 == ret)
			{
				ACE_DEBUG((LM_DEBUG, "读端口失败\n"));
				break;
			}
			else if (0 == ret)		//未接收到数据，判断超时
			{		
// 				ACE_DEBUG((LM_DEBUG, "无数据\n"));
				if(m_timeout_timer.elapsed() >= m_config_ptr->t0)
				{
					close_dev();
					break;
				}
			}
			else
			{
				m_timeout_timer.restart();
			}

			//以下是收到了报文的处理
			if (m_read_lenth > 0)	//接收缓冲区有报文再进行处理
			{
				TYPE_IDENTIFICATION frame_type = calc_frame();
				proc_cdt_frame(frame_type);
			}
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));
		}//while (true) 二层循环
 	}//while (true)  一层循环
}

TYPE_IDENTIFICATION CDT::calc_frame()
{
	TYPE_IDENTIFICATION frametype = INVALID_FRAME;
	int cur_position;
	int frame_Len = 0;
	unsigned char bch_code;
	ACE_DEBUG((LM_DEBUG, "calc_frame\n"));

	ACE_OS::memset(m_cdt_buf, 0, MAX_BUF_SIZE);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == START_CODE)		//移动置报文头
		{
			if (Scan_msg_head(&m_read_buf[cur_position]))		//同步头正确
			{
				m_chandata->frame_check_err = 0;
				m_chandata->frame_corr_cnt++;

				//控制字处理
				CONTROL_STRUCT* control_struct = (CONTROL_STRUCT*)&m_read_buf[cur_position+6];
				MakeBch(&bch_code,&m_read_buf[cur_position+6]);
				if ( bch_code == control_struct->calc_byte )		//校验正确
				{
					if (control_struct->contor_byte.E == 0)		//默认帧类别
					{
						frametype = (TYPE_IDENTIFICATION)control_struct->frame_type;		//帧类别
					}
					else
					{
						//todo
					}

					if (control_struct->contor_byte.L == 1)		//有信息字
					{
						frame_Len = control_struct->info_num * 6 + 12;	//信息字长度加控制字同步头
					}
					else		//无信息字
					{
						cur_position += 12;
						frame_Len = 0;
						break;
					}
					if (control_struct->contor_byte.S == 1)		//子站地址  todo
					{
						if (m_common_addr != control_struct->src_addr)
						{
							ACE_ERROR((LM_ERROR, "子站地址错\n"));
						}
					}
					if (control_struct->contor_byte.D == 1)	//主站地址 todo
					{
						if (m_master_addr != control_struct->dest_addr)
						{
							ACE_ERROR((LM_ERROR, "主站地址错\n"));
						}
					}

					if((cur_position + frame_Len) > m_read_lenth)				//不完整报文保留至下次处理	
					{
						ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
						m_read_lenth -= cur_position;
						frame_Len = 0;
					}
					else
					{
						ACE_OS::memcpy(m_cdt_buf, &m_read_buf[cur_position], frame_Len);   //拷贝一帧完整报文待处理
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
				}//控制字判断
				else
				{
					m_chandata->frame_check_err++;
					m_chandata->frame_err_cnt++;
				}
			}//同步头判断
		}// 报文头判断
	}
	if (cur_position == m_read_lenth)	//整个报文区数据都不对，全部清空
	{
		m_read_lenth = 0;
		ACE_OS::memset(m_read_buf, 0, REC_LEN);	
	}
	return frametype;
}



int CDT::proc_cdt_frame(TYPE_IDENTIFICATION frametype)
{
	int ret = -1;
	ACE_DEBUG((LM_DEBUG, "proc_cdt_frame\n"));

	unsigned char bch_code;
	int info_num = m_cdt_buf[INFO_NUM_POS];

	INFO_STRUCT* info_body = (INFO_STRUCT*)&m_cdt_buf[INFO_FIRS_POS];

	for (int ncnt = 0; ncnt<info_num; ++ncnt)
	{

		MakeBch(&bch_code, (unsigned char*)info_body);
		if (bch_code == info_body->bch_code)		//校验码判断
		{
			m_chandata->frame_corr_cnt++;

			//功能码判断,该信息体的信息
			if(info_body->func_code >= m_config_ptr->yc_begin && info_body->func_code <= m_config_ptr->yc_end)		//yc信息体
			{
				if (frametype == IMPORTANT_YC || frametype == SECOND_YC || frametype == COMM_YC)
				{
					deal_cdt_yc(info_body);
				}
			}
			else if (info_body->func_code >= m_config_ptr->yx_begin && info_body->func_code <= m_config_ptr->yx_end)	//yx信息体
			{
				if (frametype == STAT_YX)		//yx帧
				{
					deal_cdt_yx(info_body);
				}
				else		//插帧yx,连续处理
				{
					deal_insert_frame(info_body);
				}
			}
			else if (info_body->func_code >= m_config_ptr->pi_data_begin && info_body->func_code <= m_config_ptr->pi_data_end)	//电度信息体
			{
				if (frametype == KWH_PI)
				{
					deal_cdt_pi(info_body);
				}

			}
			else if (info_body->func_code == m_config_ptr->yx_soe_begin || info_body->func_code == m_config_ptr->yx_soe_end)	//yxsoe信息体
			{
				int frame_count;		//帧计数
				if (frametype == YX_SOE)	//yxsoe帧
				{
					if (info_body->func_code == m_config_ptr->yx_soe_begin)
					{
						frame_count = 0;
						ACE_DEBUG((LM_DEBUG, "Get Rtu Soe 0x80 Frame\n"));
					}
					else if (info_body->func_code == m_config_ptr->yx_soe_end)
					{
						frame_count = 1;
						ACE_DEBUG((LM_DEBUG, "Get Rtu Soe 0x81 Frame\n"));
					}
					deal_cdt_yxsoe(info_body, frame_count);
				}
			}
			else if (info_body->func_code == FUNC_RET_YK)		//插帧遥控返校
			{
				deal_yk_frame(info_body);
			}
			else if (info_body->func_code == FUNC_FREQUENCE)		//频率
			{
			}

		}
		else
		{
			m_chandata->frame_check_err++;
			m_chandata->frame_err_cnt++;
		}

		info_body++;
	}

	return ret;
}

int CDT::deal_cdt_yc(INFO_STRUCT* info_body)
{
	unsigned short ycval;

	int data_no = (info_body->func_code - m_config_ptr->yc_begin)*2;

	if (data_no > m_yc_all_num)
	{
		return 0;
	}

	ycval = ( info_body->info_no2 * 256 + info_body->info_no1) & 0x0fff;
	if (ycval &0x0800)
	{
		ycval = ycval & 0x07ff;
		ycval = (~ycval + 1)&0x07ff;
		ycval = ~ycval + 1;
	} 

	make_yc_real_data(data_no, ycval, 1);

	data_no++;

	if (data_no > m_yc_all_num)
	{
		return 0;
	}
	ycval = ( info_body->info_no4 * 256 + info_body->info_no3) & 0x0fff;
	if (ycval &0x0800)
	{
		ycval = ycval & 0x07ff;
		ycval = (~ycval + 1)&0x07ff;
		ycval = ~ycval + 1;
	}

	make_yc_real_data(data_no, ycval, 1);

	return 0;
}

int CDT::deal_cdt_yx(INFO_STRUCT* info_body)
{
	unsigned char* union_yx_value;
	unsigned char yxvalue;
	int data_no = (info_body->func_code - m_config_ptr->yx_begin)*16;

	union_yx_value = &(info_body->info_no1);
	for (int i = 0; i<4; i++)
	{
		for (int nyxnum =0; nyxnum < 8; ++nyxnum)
		{
			if (data_no > m_yx_all_num) 
			{
				break;
			}
			yxvalue = ((*union_yx_value)>>nyxnum) & 0x01<<nyxnum;
			data_no++;
			make_yx_real_data(data_no, yxvalue, 1);

		}
		union_yx_value++;
	}
	return 0;
}

int CDT::deal_cdt_yxsoe(INFO_STRUCT* info_body, int frame_count)
{
	FETIME temp_time;
	int data_no;
	unsigned char yxvalue;

	if (frame_count == 0)
	{
		m_soe_time.milisecond = info_body->info_no1 + info_body->info_no2 * 256;
		m_soe_time.second = info_body->info_no3;
		m_soe_time.minute = info_body->info_no4;
	}
	else if (frame_count == 1)
	{
		m_soe_time.hour = info_body->info_no1;
		m_soe_time.day = info_body->info_no2;
		data_no = info_body->info_no3 + (info_body->info_no4 & 0x0F)*256;
		yxvalue = info_body->info_no4 & 0x80;
		get_time(&temp_time);
		m_soe_time.year = temp_time.year;
		m_soe_time.month = temp_time.month;
		make_event(m_soe_time, data_no, yxvalue);
		ACE_OS::memset(&m_soe_time, 0, sizeof(FETIME));
	}
	return 0;
}

int CDT::deal_cdt_pi(INFO_STRUCT* info_body)
{
	int16u itemp;
	int data_no = (info_body->func_code)*2 - m_config_ptr->pi_data_begin;

	if(info_body->info_no4 & 0x20)		//BCD码
	{
		itemp = info_body->info_no1 / 16 * 10 + info_body->info_no1%16 + 
			100*(info_body->info_no2 / 16 * 10 + info_body->info_no2 % 16) + 
			10000*(info_body->info_no3 / 16 * 10 + info_body->info_no3 % 16) + 
			1000000*(info_body->info_no4 & 0x0f);
	}
	else		//二进制
	{
		itemp = info_body->info_no1 + 256 * info_body->info_no2 + 
			256 * 256 * info_body->info_no3 + 256 * 256 * 256 * (info_body->info_no4 & 0x0f);
	}


	make_kwh_real_data(data_no, itemp, 1);
	return 0;
}

int CDT::deal_insert_frame(INFO_STRUCT* info_body)
{
	unsigned char* union_yx_value;
	unsigned char yxvalue;
	int data_no = (info_body->func_code - m_config_ptr->yx_begin)*16;

	INFO_STRUCT* yx_frame = info_body;
	yx_frame++;

	if (ACE_OS::memcmp(info_body, yx_frame, sizeof(INFO_STRUCT)) == 0)		//第1、2帧相同
	{
		info_body++;
		yx_frame++;
		if (ACE_OS::memcmp(info_body, yx_frame, sizeof(INFO_STRUCT)))		//第2、3帧不相同
		{
			m_chandata->frame_check_err++;
			m_chandata->frame_err_cnt++;
			return -1;	
		}
	}
	else
	{
		m_chandata->frame_check_err++;
		m_chandata->frame_err_cnt++;
		return -1;
	}
	info_body++;

	for (int nbyte = 0; nbyte < 4; ++nbyte)
	{
		union_yx_value = &(info_body->info_no1);
		for (int nyxnum =0; nyxnum < 8; ++nyxnum)
		{
			if (data_no > m_yx_all_num) 
			{
				break;
			}
			yxvalue = ((*union_yx_value)>>nyxnum) & 0x01<<nyxnum;
			data_no++;
			make_yx_real_data(data_no, yxvalue, 1);

		}
		union_yx_value++;
	}
	return 0;
}

int CDT::deal_yk_frame(INFO_STRUCT* info_body)
{
	int yk_ret = -1;
	char str[NORMALCHAR_LEN];

	INFO_STRUCT* yk_frame = info_body;
	yk_frame++;

	if (ACE_OS::memcmp(info_body, yk_frame, sizeof(INFO_STRUCT)) == 0)		//第1、2帧相同
	{
		info_body++;
		yk_frame++;
		if (ACE_OS::memcmp(info_body, yk_frame, sizeof(INFO_STRUCT)))		//第2、3帧不相同
		{
			m_chandata->frame_check_err++;
			m_chandata->frame_err_cnt++;
			return -1;	
		}
	}
	else
	{
		m_chandata->frame_check_err++;
		m_chandata->frame_err_cnt++;
		return -1;
	}
	info_body++;


	if (yk_frame->info_no1 != m_current_ykoder)		//操作返回错
	{
		ACE_OS::sprintf(str,"遥控命令错,下发命令:%02x 返校命令:%02x",m_current_ykoder,yk_frame->info_no1);
		send_yk_ret(m_rtu_no, yk_frame->info_no1, yk_ret, str);
		return yk_ret;
	}
	if(yk_frame->info_no2 != m_current_ykno)		//遥控号错
	{
		ACE_ERROR((LM_ERROR, "下发的遥控号和返校的遥控号不一致!"));
		send_yk_ret(m_rtu_no, yk_frame->info_no2, yk_ret, str);
		return yk_ret;
	}

	yk_ret = 0;
	send_yk_ret(m_rtu_no, yk_frame->info_no1, yk_ret, str);

	return 0;
}

int CDT::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	unsigned char bchcode;

	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[0] = START_CODE;
	m_write_buf[1] = SYNC_CODE;
	m_write_buf[2] = START_CODE;
	m_write_buf[3] = SYNC_CODE;
	m_write_buf[4] = START_CODE;
	m_write_buf[5] = SYNC_CODE;
	m_write_buf[6] = 0x71;
	
	CDT_YK_STRUCT *yk_frame = (CDT_YK_STRUCT*)(&m_write_buf[6]);
	switch(yk_flag)
	{
	case 0:
		{
			yk_frame->control_struct.frame_type = SEL_YK;
			yk_frame->info_strcut1.func_code = FUNC_SEL_YK;
		}
		break;
	case 1:
		{
			yk_frame->control_struct.frame_type = EXCUTE_YK;
			yk_frame->info_strcut1.func_code = FUNC_EXCUTE_YK;
		}
		break;
	case 2:
		{
			yk_frame->control_struct.frame_type = CANCEL_YK;
			yk_frame->info_strcut1.func_code = FUNC_CANCEL_YK;
		}
		break;
	default:
		break;
	}
	yk_frame->control_struct.info_num = 0x03;
	yk_frame->control_struct.src_addr = m_chanpara->master_addr;
	yk_frame->control_struct.dest_addr = m_chanpara->common_addr;
	MakeBch(&bchcode,&m_write_buf[6]);
	yk_frame->control_struct.calc_byte = bchcode;

	if (bclosed)
	{
		yk_frame->info_strcut1.info_no1 = 0xcc;
		yk_frame->info_strcut1.info_no3 = 0xcc;
	}
	else
	{
		yk_frame->info_strcut1.info_no1 = 0x33;
		yk_frame->info_strcut1.info_no3 = 0x33;
	}
	yk_frame->info_strcut1.info_no2 = ykno;
	yk_frame->info_strcut1.info_no4 = ykno;

	m_current_ykno = ykno;				//存储当前下发遥控序号，以备校验
	m_current_ykoder = yk_frame->info_strcut1.info_no1;			//存储当前下发遥控命令，以备校验


	return send_msg();
}