/**
@file iec101.cpp
@brief 101规约处理源文件

@author chenkai 
@version 1.0.0
@date 2015-04-08
*/

#include "iec101.h"

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
#include <math.h>
using namespace std;

const char* const version_string = "1.0.0";
const char* const desc_string = "the very beginning 101";

//连接不成功后，等待2秒再尝试连接
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC101配置项
*/
static IEC101_CONFIG iec101_config[MAX_CHAN_NUM];

Base_protocol* get_protocol(const int& channel)
{
	return new Iec101(channel);
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

FETIME T101Time_To_Realtime(CP24Time2a& SoeTime);	//CP24time计算为实际时间

Iec101::Iec101(const int& channel)
:Base_protocol(channel), m_config_ptr(iec101_config + channel)
{
	m_common_addr = m_chanpara->common_addr;	//公共地址
}

Iec101::~Iec101()
{

}

void Iec101::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "iec101 init\n"));
	m_mainstation_FCB = 0 ;
	m_substation_FCB = 0 ;
	m_substation_ACD = 0 ;
	m_resend_count = 0 ;
	m_resend_limit = 3 ;
	m_calltwodata_cnt = 0 ;

	m_t0_timer.restart();

	m_call_all_data_flag = NEED_CALL;	
	m_call_kwh_data_flag = NEED_CALL;	
	m_sync_time_flag = NEED_CALL;
	m_rtu_init_flag = NO_NEED;
	m_request_link_flag = true ;
	m_reset_link_flag = false ;
	m_t0_timeout = false;
	m_islink_busy = false ;
	m_link_flag = REQUESTLINKSTATUS;

	m_call_all_data_timer.restart();	
	m_call_kwh_timer.restart();		
	m_sync_time_timer.restart();

	m_current_ykno = -1;
	m_current_ykoder = 0;

	m_yx_all_num = Fes_shm::get().chan_para_ptr->yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr->yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr->kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);

}

void Iec101::protocol_proc(void)
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;
	while (!m_nEnd)
	{
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
		else		//新的连接,重置101规约数据
		{
			init_protocol();
		}

		//报文循环处理
		while (!m_nEnd)		//运行条件为该通道值班机为当前前置
		{
			int write_ret = 0;
			int read_ret = 0;
			int proc_ret = 0;
			m_write_lenth = 0;  //发送长度清零

			//检查邮件，并对邮件进行相关处理
			check_event();
			write_ret = Iec101_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
				break;
			}
			else if (0 == write_ret)	//发送成功后才进行端口读写
			{
				ACE_OS::sleep(ACE_Time_Value(0,100*1000));
				read_ret  = recv_msg();	
			}

			//接收报文，并判断各种超时
			if (-1 == read_ret)			//读端口失败会导致本层循环退出
			{
				ACE_DEBUG((LM_DEBUG, "读端口失败\n"));
				break;
			}
			else if (0 == read_ret)		//未接收到数据，判断超时
			{
				if (m_t0_timer.elapsed() > m_config_ptr->t0 )
				{
					if ( m_link_flag == REQUESTLINKSTATUS || m_link_flag == RESET )
//					if ( m_request_link_flag == true || m_reset_link_flag == true )
					{
						ACE_DEBUG((LM_DEBUG, "t0超时\n"));
						close_dev();
						break;
					}
					else
					{
						m_resend_count++ ;
						if ( m_resend_count > m_resend_limit )
						{
							ACE_DEBUG((LM_DEBUG, "t0超时\n"));
							close_dev();
							break;
						}
					}
				}
			}
			else		//接收正常，重置t0定时器
			{
				m_resend_count = 0 ;
				m_t0_timer.restart();
				m_t0_timeout = false;
				m_islink_busy = false ;		//链路空闲
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
				//处理数据
				int frame_type = Iec101_receive_proc() ;
				switch ( frame_type )
				{
					case FIXED_FRAME:
						{
							proc_ret = deal_fixed_frame();
							break;
						}
					case UNFIXED_FRAME:
						{
							proc_ret = deal_unfixed_frame();
							break;
						}
					case YKCONFIRM_FRAME:
						{
							proc_ret = DEAL_SUCCESS ;
							call_classtwo_data() ;
							break;
						}
					case INVALID_FRAME:
						{
							proc_ret = deal_error_frame();
							break;
						}
					default:
						break;
				}
				//
				if ( DEAL_SUCCESS != proc_ret)  //出错跳出循环 
				{
					break;
				}
			}

			if (m_chandata->curr_attend_host != curr_host_no)		//判断归还负荷
			{
				close_dev();
				break;
			}
			ACE_OS::sleep(ACE_Time_Value(0,100*1000));

		}//while (true) 二层循环
	}//while (true)  一层循环
}

int Iec101::Iec101_send_proc(void)
{
	int ret = 0xffff ;
//	if ( m_link_flag == REQUESTLINKSTATUS )
	if ( m_request_link_flag == true )
	{
		ret = request_link_status() ;
		m_request_link_flag = false ;
		m_islink_busy = true ;
		m_t0_timeout = true ;
	}
//	else if ( m_link_flag == RESET )
	else if ( m_reset_link_flag == true )
	{
		ret = reset_link() ;
		m_reset_link_flag = false ;
		m_islink_busy = true ;
		m_t0_timeout = true ;
	}
	else if( m_link_flag == REQUESTONEDATA )
	{
		ret = call_classone_data() ;
		m_islink_busy = true ;
		m_t0_timeout = true ;
	}
	else if (m_call_all_data_flag == NEED_CALL)		//邮件判据的报文发送
	{
		if (m_call_kwh_data_flag == ON_PROCESS)	//召唤电度的过程中，产生总召命令
		{
			//todo
		}
		ret = call_all_data();
		m_islink_busy = true ;
		m_t0_timeout = true ;
	}
	else if (m_sync_time_flag == NEED_CALL)
	{
		if (m_call_all_data_flag != ON_PROCESS)		//总召过程中不进行对时
		{
			ret = sync_time();
			m_islink_busy = true ;
			m_t0_timeout = true ;
		}
	}
	else if ( m_pi_all_num && (m_call_kwh_data_flag == NEED_CALL))	//若有电度量且需要召唤
	{
		if (m_call_all_data_flag != ON_PROCESS)		//总召过程中不进行电度召唤
		{
			ret = call_kwh_data();
			m_islink_busy = true ;
			m_t0_timeout = true ;
		}
	}
	else if( m_islink_busy == false && m_calltwodata_cnt%10 == 0 && m_link_flag == REQUESTTWODATA )
	{
		ret = call_classtwo_data() ;
		m_t0_timeout = true ;
	}
	m_calltwodata_cnt++ ;

	return ret;
}

int Iec101::reset_link( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 0;
	SFC.FCB = 0;
	SFC.FC  = 0;

	m_mainstation_FCB = 1 ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 3 , CRC_WRITE ) ;
	m_write_buf[ 4 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101::request_link_status( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;
	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 0;
	SFC.FCB = 0;
	SFC.FC = REQUESTLINKSTATUS;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end
	m_write_buf[ 1 ] = tmpFC ;
	m_write_buf[ 2 ] = m_common_addr ;
	CRC_test( m_write_buf , 3 , CRC_WRITE ) ;
	m_write_buf[ 4 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101::call_all_data( void )
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x09;
	m_write_buf[ 2 ] = 0x09;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ] = tmpFC;
	m_write_buf[ 5 ] = m_common_addr ;
	m_write_buf[ 6 ] = C_IC_NA_1; //类型标识
	m_write_buf[ 7 ] = 0x01; //可变结构限定词
	m_write_buf[ 8 ] = COT_ACT; //传送原因
	m_write_buf[ 9 ] = m_common_addr ;
	m_write_buf[ 10 ] = 0x00;	//信息体地址
	m_write_buf[ 11 ] = 0x00;
	m_write_buf[ 12 ] = CALLTOTAL ;	

	CRC_test( m_write_buf + 3 , 10 , CRC_WRITE ) ;
	m_write_buf[ 14 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 2 ] + UNFIXED_DATA_LEN ;

	m_call_all_data_flag = ON_PROCESS;	

	return send_msg();
}

int Iec101::call_classone_data( void )
{
	m_substation_ACD = 0 ;
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC  = REQUESTONEDATA;

	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end
	m_write_buf[ 1 ] = tmpFC;
	m_write_buf[ 2 ] = m_common_addr ;

	CRC_test( m_write_buf , 3 , CRC_WRITE ) ;
	m_write_buf[ 4 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101::call_classtwo_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = FIXED_FRAME_HEAD ;

	ControlField SFC;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC  = REQUESTTWODATA;

	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end	
	m_write_buf[ 1 ] = tmpFC;
	m_write_buf[ 2 ] = m_common_addr ;

	CRC_test( m_write_buf , 3 , CRC_WRITE ) ;
	m_write_buf[ 4 ] = FRAME_TAIL ;

	m_write_lenth = FIXED_FRAME_LEN;

	return send_msg();
}

int Iec101::sync_time()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x0F;
	m_write_buf[ 2 ] = 0x0F;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;
	m_write_buf[ 6 ]  = C_CS_NA_1; //类型标识
	m_write_buf[ 7 ]  = 0x01; //可变结构限定词
	m_write_buf[ 8 ]  = COT_ACT; //传送原因
	m_write_buf[ 9 ]  = m_common_addr ;
	m_write_buf[ 10 ]  = 0x00 ; //信息体地址
	m_write_buf[ 11 ]  = 0x00 ;

	SYNC_TIME_STRUCT* pt = (SYNC_TIME_STRUCT*)( m_write_buf + 12 );

	FETIME tim;
	get_time(&tim);

	pt->cp56time.minute = tim.minute;
	pt->cp56time.hour = tim.hour;
	pt->cp56time.day = tim.day;
	pt->cp56time.month = tim.month;
	pt->cp56time.year = (((tim.year + 1900)-2000)&0xff);
	pt->cp56time.millsecond = tim.milisecond + tim.second*1000;

	CRC_test( m_write_buf + 3 , 16 , CRC_WRITE ) ;
	m_write_buf[ 20 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_sync_time_flag = ON_PROCESS;

	m_chandata->last_sync_time = ACE_OS::time(NULL);
	m_chandata->sync_time_success = 0;

	return send_msg();

}

int Iec101::call_kwh_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x09;
	m_write_buf[ 2 ] = 0x09;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;
	m_write_buf[ 6 ]  = C_CI_NA_1; //类型标识,101电能脉冲召唤
	m_write_buf[ 7 ]  = 0x01; //可变结构限定词
	m_write_buf[ 8 ]  = COT_ACT; //传送原因
	m_write_buf[ 9 ]  = m_common_addr ;
	m_write_buf[ 10 ]  = 0x00 ; //信息体地址
	m_write_buf[ 11 ]  = 0x00 ;

	m_write_buf[ 12 ] = CALLKWHTOTAL ;

	CRC_test( m_write_buf + 3 , 10 , CRC_WRITE ) ;
	m_write_buf[ 14 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	m_call_kwh_data_flag = ON_PROCESS;

	return send_msg();
}

int Iec101::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x09;
	m_write_buf[ 2 ] = 0x09;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;
	m_write_buf[ 6 ]  = m_config_ptr->yk_type; //类型标识,101电能脉冲召唤
	m_write_buf[ 7 ]  = 0x01; //可变结构限定词
	m_write_buf[ 8 ]  = (yk_flag == 2) ? COT_DEACT : COT_ACT;		//是否取消
	m_write_buf[ 9 ]  = m_common_addr ;
	m_write_buf[ 10 ] = ( m_config_ptr->yk_begin + ykno ) % 256 ;
	m_write_buf[ 11 ] = ( m_config_ptr->yk_begin + ykno ) / 256 ;

	//YKDCO 控合控分,由单点或双点遥控确认
	YKDCO ykdco ;
	if ( yk_flag == 1 )
	{
		ykdco.dco_se = 0 ;
	}
	else
	{
		ykdco.dco_se = 1 ;
	}
	if (m_config_ptr->yk_type == C_SC_NA_1)		//单点遥控
	{
		if(bclosed)
		{
			ykdco.dco_dcs = STATE_SP_ON ;
		}	
		else
		{
			ykdco.dco_dcs = STATE_SP_OFF ;
		}
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//双点遥控
	{
		if(bclosed)
		{
			ykdco.dco_dcs = STATE_DP_ON ;
		}	
		else
		{
			ykdco.dco_dcs = STATE_DP_OFF ;
		}
	}
	ykdco.dco_qu = 0 ;

	//modify for IBM bit begin
	unsigned char tmpbuf; 
	unsigned char tmpbuf1; // 0001 1111
	unsigned char tmpbuf2; 

	tmpbuf = (ykdco.dco_se<<7);
	tmpbuf1 = (ykdco.dco_qu)&0x1f;
	tmpbuf1 = tmpbuf1<<2;
	tmpbuf2 = ( ykdco.dco_dcs )&0x03;

	m_write_buf[ 12 ] = tmpbuf|tmpbuf1|tmpbuf2;

	m_current_ykno = ykno;				//存储当前下发遥控序号，以备校验
	m_current_ykoder = ykdco.dco_dcs;			//存储当前下发遥控命令，以备校验

	CRC_test( m_write_buf + 3 , 10 , CRC_WRITE ) ;
	m_write_buf[ 14 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	return send_msg();
}

int Iec101::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0x09;
	m_write_buf[ 2 ] = 0x09;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;
	m_write_buf[ 6 ]  = C_RC_NA_1; //类型标识,101电能脉冲召唤
	m_write_buf[ 7 ]  = 0x01; //可变结构限定词
	m_write_buf[ 8 ]  = (yt_flag == 2) ? COT_DEACT : COT_ACT;		//是否取消
	m_write_buf[ 9 ]  = m_common_addr ;
	m_write_buf[ 10 ] = ( m_config_ptr->yk_begin + ytno ) % 256 ;
	m_write_buf[ 11 ] = ( m_config_ptr->yk_begin + ytno ) / 256 ;

	//YKDCO 控合控分,由单点或双点遥控确认
	YKDCO ytdco ;
	if ( yt_flag == 1 )
	{
		ytdco.dco_se = 0 ;
	}
	else
	{
		ytdco.dco_se = 1 ;
	}
	if (m_config_ptr->yk_type == C_SC_NA_1)		//单点遥控
	{
		if(1)
		{
			ytdco.dco_dcs = STATE_SP_ON ;
		}	
		else
		{
			ytdco.dco_dcs = STATE_SP_OFF ;
		}
	}
	else if (m_config_ptr->yk_type == C_DC_NA_1)	//双点遥控
	{
		if(1)
		{
			ytdco.dco_dcs = STATE_DP_ON ;
		}	
		else
		{
			ytdco.dco_dcs = STATE_DP_OFF ;
		}
	}
	ytdco.dco_qu = 0 ;

	//modify for IBM bit begin
	unsigned char tmpbuf; 
	unsigned char tmpbuf1; // 0001 1111
	unsigned char tmpbuf2; 

	tmpbuf = (ytdco.dco_se<<7);
	tmpbuf1 = (ytdco.dco_qu)&0x1f;
	tmpbuf1 = tmpbuf1<<2;
	tmpbuf2 = ( ytdco.dco_dcs )&0x03;

	m_write_buf[ 12 ] = tmpbuf|tmpbuf1|tmpbuf2;

	m_current_ykno = ytno;				//存储当前下发遥控序号，以备校验
	m_current_ykoder = ytdco.dco_dcs;			//存储当前下发遥控命令，以备校验

	CRC_test( m_write_buf + 3 , 10 , CRC_WRITE ) ;
	m_write_buf[ 14 ] = FRAME_TAIL ;

	m_write_lenth = m_write_buf[ 1 ] + UNFIXED_DATA_LEN;

	return send_msg();
}

int Iec101::Iec101_receive_proc(void)
{
	int cur_position = 0 ;
	int frame_type = INVALID_FRAME ;
	int frame_len = 0 ;
	ACE_OS::memset( m_rec_buf , 0 , MAX_BUF_SIZE ) ;

	if ( m_read_lenth == YK_CONFIRM_LEN && m_rec_buf[ 0] == YK_CONFIRM_HEAD ) //遥控链路层确认帧
	{
		frame_type = YKCONFIRM_FRAME ;
		m_read_lenth = YK_CONFIRM_LEN ;
		ACE_OS::memcpy( m_rec_buf, &m_read_buf[ 0 ], YK_CONFIRM_LEN );
		return frame_type ;
	}

	if ( m_read_lenth < FIXED_FRAME_LEN ) //帧长小于5
	{
		return frame_type ;
	}

	for ( cur_position = 0 ; cur_position < m_read_lenth ; cur_position++ )
	{
		//固定帧处理
		if ( m_read_buf[ cur_position ] == FIXED_FRAME_HEAD && m_read_buf[ cur_position + FIXED_FRAME_LEN - 1 ] == FRAME_TAIL )
		{
			ACE_OS::memcpy( m_rec_buf, &m_read_buf[ cur_position ], FIXED_FRAME_LEN );   //拷贝一帧完整报文待处理
			frame_type = FIXED_FRAME ;
			ACE_OS::memmove( m_read_buf, &m_read_buf[cur_position + FIXED_FRAME_LEN], m_read_lenth - FIXED_FRAME_LEN - cur_position ) ;
			m_read_lenth = m_read_lenth - cur_position - FIXED_FRAME_LEN;
			ACE_OS::memset( &m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth ) ;	//移位后的数据区需初始化
			break;
		}

		//可变帧处理
		else if ( m_read_buf[ cur_position ] == UNFIXED_FRAME_HEAD && m_read_buf[ cur_position + 3 ] == UNFIXED_FRAME_HEAD )
		{
			frame_len = m_read_buf[ cur_position + 1 ] + UNFIXED_DATA_LEN ;

			if( ( cur_position + frame_len ) > m_read_lenth )				//不完整报文保留至下次处理	
			{
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
				m_read_lenth -= cur_position;
				frame_len = 0;
			}
			else
			{
				ACE_OS::memcpy( m_rec_buf, &m_read_buf[ cur_position ], frame_len);   //拷贝一帧完整报文待处理
				frame_type = UNFIXED_FRAME ;
				//将m_read_buf中未处理的报文移位
				ACE_OS::memmove( m_read_buf, &m_read_buf[cur_position + frame_len], m_read_lenth - frame_len - cur_position ) ;
				m_read_lenth = m_read_lenth - cur_position - frame_len;
			}
			if( m_read_lenth < 0 || m_read_lenth > REC_LEN )	//报文缓冲区超限，全部清空
			{
				m_read_lenth = 0;
				ACE_OS::memset(m_read_buf, 0, REC_LEN);	
			}
			ACE_OS::memset( &m_read_buf[m_read_lenth], 0, REC_LEN - m_read_lenth ) ;	//移位后的数据区需初始化
			break;
		}
	}

	if (cur_position == m_read_lenth)	//整个报文区数据都不对，全部清空
	{
		m_read_lenth = 0;
		ACE_OS::memset(m_read_buf, 0, REC_LEN);	
	}
	return frame_type ;
}

int Iec101::deal_error_frame(void)
{
	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

int Iec101::deal_fixed_frame(void)
{
	if ( !CRC_test( m_rec_buf ,  3 , CRC_READ ) )
	{
		return DEFAULT_ERROR ;
	}

	ControlField	SFC;
	//modify for IBM bit begin
	unsigned char tmpFC = m_rec_buf[ 1 ] ;

	SFC.DIR = (tmpFC>>7) & 0x01;
	SFC.PRM = (tmpFC>>6) & 0x01;
	SFC.FCB = (tmpFC>>5) & 0x01;
	SFC.FCV = (tmpFC>>4) & 0x01;
	SFC.FC = tmpFC & 0x0F;
	//end	

	if ( SFC.PRM == 1 )	// PRM=0 主站启动
	{
		return DEFAULT_ERROR ;
	}

	if( m_rec_buf[ 2 ] != m_common_addr )
	{
		return DEFAULT_ERROR ;
	}

//	if ( m_link_flag == REQUESTLINKSTATUS && SFC.FC == LINKREDAY )
	if ( m_link_flag == REQUESTLINKSTATUS && SFC.FC == LINKREDAY )
	{
		m_link_flag = RESET ;
		m_reset_link_flag = true ;
	}

	if ( m_link_flag == RESET && SFC.FC == ACK )//复位链路确认 由链路自身判断召唤
	{
// 		m_link_flag = REQUESTONEDATA ;
// 		m_call_one_data_flag = true ;
	}

	m_substation_ACD = SFC.FCB ;
	if ( m_substation_ACD == 1 ) //有一级数据
	{
		m_link_flag = REQUESTONEDATA ;
// 		m_call_one_data_flag = true ;
// 		m_call_two_data_flag = false ;
	}
 	else
 	{
		m_link_flag = REQUESTTWODATA ;
//  		m_call_two_data_flag = true ;
//  		m_call_one_data_flag = false ;
 	}
	return DEAL_SUCCESS ;
}

int Iec101::deal_unfixed_frame(void)
{
	if ( m_rec_buf[ 1 ] != m_rec_buf[ 2 ] ) //长度不一致,全部清空
	{
		return DEFAULT_ERROR ;
	}

	if ( !CRC_test( m_rec_buf + 3 ,  m_rec_buf[ 1 ] + 1 , CRC_READ ) )
	{
		return DEFAULT_ERROR ;
	}

	if( m_rec_buf[ 5 ] != m_common_addr )
	{
		return DEFAULT_ERROR ;
	}

	ControlField	SFC;
	//modify for IBM bit begin
	unsigned char tmpFC = m_rec_buf[ 4 ] ;

	SFC.DIR = (tmpFC>>7) & 0x01;
	SFC.PRM = (tmpFC>>6) & 0x01;
	SFC.FCB = (tmpFC>>5) & 0x01;
	SFC.FCV = (tmpFC>>4) & 0x01;
	SFC.FC = tmpFC & 0x0F;
	//end

	m_substation_ACD = SFC.FCB ;
	if ( m_substation_ACD == 1 ) //有一级数据
	{
		m_link_flag = REQUESTONEDATA ;
	}
	else
	{
		m_link_flag = REQUESTTWODATA ;
	}

	if ( SFC.PRM == 1 )	// PRM=0 主站启动
	{
		return DEFAULT_ERROR ;
	}


	int ret = DEFAULT_ERROR ;
	if ( SFC.FC == RESPONSE101 )//功能码=0 为确认帧
	{
		switch ( m_rec_buf[ 6 ] )
		{
		case C_CS_NA_1://对时确认
			{
				if ( m_rec_buf[ 8 ] == COT_ACTCON)
				{
					m_chandata->sync_time_success = 1;
					m_sync_time_flag = NO_NEED;		//对时结束
					ret = DEAL_SUCCESS ;
				}
			}
			break;
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
		case M_IT_NA_1:							//kwh处理
			ret = deal_sequence_M_IT_NA_1();		
			break;
		case M_IT_TA_1:  //暂无
		case M_IT_TB_1:
			break;
		case M_BO_NA_1:		//7:子站远动终端状态处理 暂无
			break;
		case C_SC_NA_1:
		case C_DC_NA_1:
			{
				ret = proc_yk_frame();			//yk处理
			}
			break;
		case C_RC_NA_1://47://升降返校
			//		A_DAResult(StaNo,ChNo,DataHead,comp);
			break;
		case M_EI_NA_1:
			{
				m_call_all_data_flag  = NEED_CALL ;
				m_link_flag = CALLTOTAL ;
			}
			break;
		case C_IC_NA_1:		//总召
			{
				ret = 0;
				if ((m_rec_buf[ 8 ] & 0x3f) == COT_ACTTERM)
				{
					m_call_all_data_flag = NO_NEED;		//总召唤结束,确认不做处理
// 					m_link_flag = REQUESTTWODATA ;
// 					m_call_two_data_flag = true ;
				}
			}
			break;
		case C_CI_NA_1:		//电度召唤
			{
				ret = 0;
				if ((m_rec_buf[ 8 ] & 0x3f) == COT_ACTTERM)
				{
					m_call_kwh_data_flag = NO_NEED;		//电度召唤结束,确认不做处理
				}
			}
			break;
// 		case 141:
// 			
		default:
			break;
		}
	}
	if (ret == DEFAULT_ERROR)
	{
		ACE_DEBUG((LM_DEBUG, "数据处理出错\n"));
	}

	return ret;
}

//YX类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101::deal_sequence_M_SP_NA_1(void)
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//遥信点号

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)( m_rec_buf + 12 );
		int i;
		for (i = 0; i < info_num; i++)
		{
			make_yx_real_data(data_no, *pt, 1);
			data_no++;
			pt++;
		}		
	}
	else			//每帧含信息体地址
	{
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)( m_rec_buf + 10 );
		int i;
		for (i = 0; i < info_num; i++)
		{
			data_no = yx_struct->addr1 - m_config_ptr->yx_begin;
			make_yx_real_data(data_no, yx_struct->yx_value, 1);
			yx_struct++;
		}		
	}

	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_DP_NA_1(void)
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//遥信点号
	unsigned char value = 0;

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		unsigned char* pt = (unsigned char*)( m_rec_buf + 12 );
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
		SEQ_M_NA_1* yx_struct = (SEQ_M_NA_1*)( m_rec_buf + 10 );
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

	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_PS_NA_1(void)			//need for check 该处理方式仍不健全，待考证
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//遥信点号
	unsigned char value = 0;

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yx_begin;

		SEQ_M_PS_NA_1* yx_struct = (SEQ_M_PS_NA_1*)( m_rec_buf + 12 );
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

		SEQ_M_PS_NA_1_B* yx_struct = (SEQ_M_PS_NA_1_B*)( m_rec_buf + 10 );

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

	return DEAL_SUCCESS ;

}
//YX类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YXSOE类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101::deal_sequence_M_SP_TA_1()		//M_SP_TA_1
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	int data_no = 0;		//遥信点号
	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)( m_rec_buf + 10 );

	for (int i = 0; i < info_num; i++)
	{
		data_no = seq_struct->info_addr.addr1 - m_config_ptr->yx_begin;  //计算yx点号
		soe_time = T101Time_To_Realtime(seq_struct->short_time);	

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_DP_TA_1()		//M_DP_TA_1
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	int data_no = 0;		//遥信点号
	unsigned char value = 0;

	FETIME soe_time;
	SEQ_M_TA_1* seq_struct = (SEQ_M_TA_1*)( m_rec_buf + 10 );

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

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}


int Iec101::deal_sequence_M_SP_TB_1()	//M_SP_TB_1
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	int data_no = 0;		//遥信点号
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)( m_rec_buf + 10 );

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

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_DP_TB_1()	//M_DP_TB_1
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	int data_no = 0;		//遥信点号
	unsigned char value = 0;
	FETIME soe_time;
	SEQ_M_TB_1* seq_struct = (SEQ_M_TB_1*)( m_rec_buf + 10 );

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

		make_event(soe_time, data_no, seq_struct->value);

		seq_struct++;
	}

	return DEAL_SUCCESS ;
}
//YXSOE类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YC类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101::deal_sequence_M_ME_NA_1()		//9、11标识YC处理
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;

	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//遥测点号

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_NA_1 * seq_data = (SEQ_M_ME_NA_1*)( m_rec_buf + 12 );
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
		SEQ_M_ME_NA_1_B * yc_struct = (SEQ_M_ME_NA_1_B*)( m_rec_buf + 10 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_ME_TA_1()	//10带时标遥测值
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;

	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//遥测点号
	FETIME time;			//遥测发生时间
	SEQ_M_ME_TA_1 * yc_struct = (SEQ_M_ME_TA_1*)( m_rec_buf + 10 );
	for (int i =0; i< datanum; ++i)
	{
		time = T101Time_To_Realtime(yc_struct->short_time);
		data_no = yc_struct->info_addr.addr1 - m_config_ptr->yc_begin;
		make_yc_real_data(data_no, yc_struct->value, yc_struct->quanity, &time);
		yc_struct++;
	}
	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_ME_NC_1()		//13标识YC处理
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;

	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//遥测点号

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		SEQ_M_ME_NC_1 * seq_data = (SEQ_M_ME_NC_1*)( m_rec_buf + 12 );
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

		SEQ_M_ME_NC_1_B * yc_struct = (SEQ_M_ME_NC_1_B*)( m_rec_buf + 10 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->seq_data.value, yc_struct->seq_data.quanity);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}

int Iec101::deal_sequence_M_ME_ND_1()		//21:无品质描述遥测值
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;

	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	int data_no = 0;		//遥测点号

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->yc_begin;//计算遥测点号

		int16s* yc_data = (int16s*)( m_rec_buf + 12 );
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

		SEQ_M_ME_ND_1_B * yc_struct = (SEQ_M_ME_ND_1_B*)( m_rec_buf + 10 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = yc_struct->addr.addr1 - m_config_ptr->yc_begin;//计算遥测点号
			make_yc_real_data(data_no, yc_struct->yc_data, 1);
			yc_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}
//YC类型处理end///////////////////////////////////////////////////////////////////////////////////////

//KWH类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101::deal_sequence_M_IT_NA_1()		//0x15电度
{
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char datanum = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;

	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}
	int data_no = 0;		//遥脉点号

	if ( sq == 0 )		//只有一个信息体地址
	{
		INFO_ADDR *pt_addr = (INFO_ADDR*)( m_rec_buf + 10 );
		data_no = pt_addr->addr1 - m_config_ptr->kwh_begin;//计算遥脉点号

		SEQ_M_IT_NA_1 * seq_data = (SEQ_M_IT_NA_1*)( m_rec_buf + 12 );
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

		SEQ_M_IT_NA_1_B * kwh_struct = (SEQ_M_IT_NA_1_B*)( m_rec_buf + 10 );
		for (int i =0; i< datanum; ++i)
		{
			data_no = kwh_struct->addr.addr1 - m_config_ptr->kwh_begin;//计算遥脉点号
			make_kwh_real_data(data_no, kwh_struct->seq_data.value, kwh_struct->seq_data.quanity);
			kwh_struct ++;
		}

	}
	return DEAL_SUCCESS ;
}
//KWH类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YK类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Iec101::proc_yk_frame()
{
	char str[NORMALCHAR_LEN];

	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	int ret_ykno = m_rec_buf[ 10 ] + m_rec_buf[ 11 ] * 256 - m_config_ptr->yk_begin;

	int yk_ret = DEFAULT_ERROR ;
	if( comm_addr != m_common_addr )		//判断公共地址
	{
		ACE_OS::sprintf(str,"遥控公共地址错,正确地址:%d,返校地址:%d",m_common_addr,comm_addr);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (ret_ykno != m_current_ykno)	//遥控号出错
	{
		ACE_ERROR((LM_ERROR, "下发的遥控号和返校的遥控号不一致!"));
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if ((cause & 0x3f) != COT_ACTCON && (cause & 0x3f) != COT_DEACTCON)	//传送原因错
	{
		ACE_OS::sprintf(str,"遥控返校的传送原因无效,cot = %02x",cause & 0x3f);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	int tmp_ykcoder = m_rec_buf[ 12 ] & 0x03 ;
	if (tmp_ykcoder != m_current_ykoder)	//遥控命令出错
	{
		ACE_OS::sprintf(str,"遥控命令错,下发命令:%02x 返校命令:%02x",m_current_ykoder,tmp_ykcoder);
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if((cause & 0x40) == 0 )		//判断返校结果
	{
		ACE_OS::sprintf(str,"遥控返校正确!");
		yk_ret  = DEAL_SUCCESS;
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
	}
	else if((cause & 0x40) == 0x40 )
	{
		ACE_OS::sprintf(str,"遥控返校不允许!");
		send_yk_ret(m_rtu_no, ret_ykno, yk_ret, str);
		return yk_ret;
	}

	if (!(tmp_ykcoder & 0x80))		//遥控执行或取消完成后
	{
		ACE_DEBUG((LM_DEBUG, "遥控动作完成!"));
		m_current_ykoder = 0;	//完成遥控后将遥控状态置位
	}

	//接收正确信息
	yk_ret = DEAL_SUCCESS;
	return yk_ret;
}
//YK类型处理end///////////////////////////////////////////////////////////////////////////////////////

int Iec101::deal_prot_frame()
{
	unsigned char sendflag = 0;
	unsigned char protocoltype = 0;
	unsigned char sq = ( m_rec_buf[ 7 ] &0x80 )>>7;
	unsigned char info_num = m_rec_buf[ 7 ] &0x7F ;
	unsigned char cause = m_rec_buf[ 8 ] ;
	unsigned char comm_addr = m_rec_buf[ 9 ] ;
	unsigned char asdu_type = m_rec_buf[10];
	if( comm_addr != m_common_addr )		//判断公共地址
	{
		return DEFAULT_ERROR ;
	}

	char* mailbuf;
	unsigned char* Buff;
	mailbuf = static_cast<char*>(ACE_OS::malloc(1+256));
	int mail_lenth = 0;

	switch(asdu_type)
	{
	case 145:	//IEC103 hex103  长度需要
		{
			*mailbuf = 36;		
			Buff = m_rec_buf + 10;
			mail_lenth = m_rec_buf[2] - 6; //从信息对象地址开始为保护报文
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

int Iec101::send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)
{

	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = UNFIXED_FRAME_HEAD ;
	m_write_buf[ 1 ] = 0;
	m_write_buf[ 2 ] = 0;
	m_write_buf[ 3 ] = UNFIXED_FRAME_HEAD ;

	ControlField SFC ;
	SFC.DIR = 0;
	SFC.PRM = 1;
	SFC.FCV = 1;
	SFC.FC = SENDCONFIRM ;
	SFC.FCB = m_mainstation_FCB ;
	m_mainstation_FCB = ~m_mainstation_FCB ;

	//modify for IBM bit begin
	unsigned char tmpFC = 0;
	SFC.DIR = SFC.DIR & 0x01;
	SFC.PRM = SFC.PRM & 0x01;
	SFC.FCV = SFC.FCV & 0x01;
	SFC.FCB = SFC.FCB & 0x01;
	SFC.FC = SFC.FC & 0x0F;
	tmpFC += SFC.DIR * 0x80;
	tmpFC += SFC.PRM * 0x40;
	tmpFC += SFC.FCB * 0x20;
	tmpFC += SFC.FCV * 0x10;
	tmpFC += SFC.FC;
	//end

	m_write_buf[ 4 ]  = tmpFC ;
	m_write_buf[ 5 ]  = m_common_addr ;

	unsigned char frame_len = 2 + cmd_len;
	unsigned char* buf = m_write_buf + 6;
	switch(protocol_type)
	{
	case FE_PROTTYPE_43:
		{
			if (cmd_len == 8)	//直控命令请求读定值
			{
				*buf++ = 0x8E;
			}
			else 	//修改定值
			{
				*buf++ = 0x80; 
			}

			*buf++ = 1;		//VSQ
			*buf++ = COT_ACT;		//COT
			*buf++ = m_common_addr;

			frame_len += 4;
		}
		break;
	default:
		break;
	}

	for( int i=0; i<cmd_len; i++ )
	{
		*buf++ = cmd[i];
	}
	CRC_test( m_write_buf + 3 , frame_len , CRC_WRITE );
	*buf++;
	*buf++ = FRAME_TAIL ;

	m_write_buf[1] = frame_len;
	m_write_buf[2] = frame_len;
	m_write_lenth = frame_len + UNFIXED_DATA_LEN;

	return send_msg();

}

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
