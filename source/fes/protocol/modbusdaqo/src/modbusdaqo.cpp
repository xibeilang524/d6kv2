/**
@file modbusdaqo.cpp
@brief modbusdaqo规约处理源文件

@author chenkai 
@version 1.0.0
@date 2016-07-21
*/

#include "modbusdaqo.h"

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
using namespace std ;

/**
@brief modbusdaqo配置项
*/
static MODBUSDAQO_CONFIG modbusdaqo_config[MAX_CHAN_NUM];

const char* const version_string = "1.0.0" ;
const char* const desc_string = "the very beginning modbusdaqo" ;

const int CONN_RETRY_TIME = 2 ;

Base_protocol* get_protocol( const int& channel )
{
	return new modbusdaqo( channel ) ;
}

void get_config( void )
{
	//todo
	//从数据库读取modbusdaqo配置信息，写入modbusdaqo_config
	struct MODBUSDAQO_CONFIG_
	{
		unsigned short channel_no; //通道号
		unsigned short yx_func_code; //遥信功能码
		unsigned short yx_start_no; //遥信起始序号
		unsigned short yx_call_time; //遥信召唤时间
		unsigned short yc_func_code; //遥测功能码
		unsigned short yc_start_no; //遥测起始序号
		unsigned short yc_call_time; //遥测召唤时间
		unsigned short yk_func_code; //遥控功能码
		unsigned short yk_start_no; //遥控起始序号
		unsigned short t0; //t0
	};
	MODBUSDAQO_CONFIG_ * special_config = NULL;

	int ret = Db_connector::select("select * from modbusdaqo_special_config order by  channel_no asc", (void **)&special_config);
	int rcd_cnt = ret;		//规约特殊配置元素数量

	if(ret < 0)	//访问数据库出错
	{
		return;
	}

	for (int nnum = 0; nnum < rcd_cnt; ++nnum)
	{
		unsigned short channel_no = special_config->channel_no;

		modbusdaqo_config[channel_no].yx_func_code = special_config->yx_func_code;
		modbusdaqo_config[channel_no].yx_start_no = special_config->yx_start_no;
		modbusdaqo_config[channel_no].yx_call_time = special_config->yx_call_time;
		modbusdaqo_config[channel_no].yc_func_code = special_config->yc_func_code;
		modbusdaqo_config[channel_no].yc_start_no = special_config->yc_start_no;
		modbusdaqo_config[channel_no].yc_call_time = special_config->yc_call_time;
		modbusdaqo_config[channel_no].yk_func_code = special_config->yk_func_code;
		modbusdaqo_config[channel_no].yk_start_no = special_config->yk_start_no;
		modbusdaqo_config[channel_no].t0 = special_config->t0;
	
		special_config++;
	}
}

char* get_version( void )
{
	return const_cast<char*>( version_string ) ;
}

char* get_desc( void )
{
	return const_cast<char*>( desc_string ) ;
}


modbusdaqo::modbusdaqo( const int& channel ):Base_protocol( channel ), m_config_ptr(modbusdaqo_config + channel)
{
	ACE_DEBUG( ( LM_DEBUG , "CDT constructor\n" ) ) ;

	m_common_addr = m_chanpara->common_addr ;
	m_master_addr = m_chanpara->master_addr ;
}

modbusdaqo::~modbusdaqo()
{

}

void modbusdaqo::init_protocol( void )
{
	ACE_DEBUG( ( LM_DEBUG , "modbusdaqo init\n" ) ) ;

	m_call_all_data_flag = NO_NEED ;	
	m_call_kwh_data_flag = NO_NEED ;	
	m_sync_time_flag = NO_NEED ;
	m_rtu_init_flag = NO_NEED ;

	m_timeout_timer.restart() ;
	m_yc_timer.restart() ;
	m_yx_timer.restart() ;

	m_current_ykno = -1 ;
	m_current_ykoder = 0 ;
	m_acc_handle_flag = 0 ;
	m_protocol_flag = 0 ;
	m_unit_flag = m_common_addr ;
	m_call_yx_flag = true ;
	m_call_yc_flag = true ;

	m_yx_all_num = m_chanpara->yx_num ;
	m_yc_all_num = m_chanpara->yc_num ;

	ACE_OS::memset( &m_read_buf , 0 , REC_LEN ) ;
	ACE_OS::memset( &m_write_buf , 0 , TRN_LEN ) ;
}

void modbusdaqo::protocol_proc( void )
{
	short curr_host_no = Fes_shm::get().fe_sys_data_ptr->host_no ;
	while ( !m_nEnd )
	{
		ACE_DEBUG( ( LM_DEBUG , "第一重循环\n" ) ) ;

		short curr_attend_host = m_chandata->curr_attend_host ;
		if ( curr_attend_host != curr_host_no )	//非本前置接管通道 ,只进行外层循环
		{
			ACE_OS::sleep( ACE_Time_Value( 0 ,1*1000 ) ) ;
			continue ;
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
		else		//新的网络连接,重置modbusdaqo规约数据
		{
			init_protocol();
		}

		//报文循环处理
		while ( !m_nEnd )
		{
			int write_ret = 0;
			int read_ret = 0;

			//检查邮件，并对邮件进行相关处理
			m_write_lenth = 0 ;  //发送长度清零
			check_event( ) ;
			write_ret = modbusdaqo_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
				break;
			}

			read_ret = recv_msg() ;
			//读端口失败都会导致本层循环退出
			if ( -1 == read_ret )
			{
				ACE_DEBUG( ( LM_DEBUG , "读端口失败\n" ) ) ;
				break ;
			}
			else if ( 0 == read_ret )		//未接收到数据，判断超时
			{		
// 				ACE_DEBUG( ( LM_DEBUG , "无数据\n" ) ) ;
				int aaa = m_timeout_timer.elapsed() ;
				if(  m_timeout_timer.elapsed() >= m_config_ptr->t0  )
				{
					m_timeout_timer.restart() ;
					close_dev() ;
					break ;
				}
			}
			else
			{
				m_timeout_timer.restart() ;
			}
			if ( m_yc_timer.elapsed() > m_config_ptr->yc_call_time && m_yc_all_num > 0 )
			{
				m_call_yc_flag = true ;
				m_yc_timer.restart() ;
			}
			if ( m_yx_timer.elapsed() > m_config_ptr->yx_call_time && m_yx_all_num > 0 )
			{
				m_call_yx_flag = true ;
				m_yx_timer.restart() ;
			}
			//以下是收到了报文的处理
			if ( m_read_lenth > 0 )	//接收缓冲区有报文再进行处理
			{
				TYPE_IDENTIFICATION frame_type = calc_frame() ;
				proc_modbusdaqo_frame( frame_type ) ;
			}
			ACE_OS::sleep( ACE_Time_Value( 0 ,10*1000 ) ) ;
		}//while ( true ) 二层循环
	}//while ( true )  一层循环
}

int modbusdaqo::modbusdaqo_send_proc(void)
{
	int ret = 0 ;
	if ( m_call_yx_flag == true && m_yx_all_num > 0 )
	{
		int yx_func_code = m_config_ptr->yx_func_code ;
		switch ( yx_func_code )
		{
		case YX_CODE1:
			{
				ret = call_modbusdaqo_code1() ;
				m_call_yx_flag = false ;
			}
			break;
		default:
			break ;
		}
	}
	if ( m_call_yc_flag == true && m_yc_all_num > 0 )
	{
		int yc_func_code = m_config_ptr->yc_func_code ;
		switch ( yc_func_code )
		{
		case YC_CODE3:
			{
				ret = call_modbusdaqo_code3() ;
				m_call_yc_flag = false ;
			}
			break;
		default:
			break ;
		}
	}

	return ret;
}

int modbusdaqo::call_modbusdaqo_code1()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	int cur_pos = 0 ;

	m_write_buf[ cur_pos++ ] = FIRST ;
	m_write_buf[ cur_pos++ ] = SECOND ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag / 256  ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag % 256 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = m_unit_flag ;
	m_write_buf[ cur_pos++ ] = YX_CODE1 ;
	m_write_buf[ cur_pos++ ] = m_config_ptr->yx_start_no / 256 ;
	m_write_buf[ cur_pos++ ] = m_config_ptr->yx_start_no % 256 ;
	m_write_buf[ cur_pos++ ] = m_yx_all_num / 256 ;
	m_write_buf[ cur_pos++ ] = m_yx_all_num % 256 ;

	m_write_buf[ 4 ] = ( cur_pos - 6 )/256 ;
	m_write_buf[ 5 ] = ( cur_pos - 6 )%256 ;

	m_write_lenth = cur_pos ;
	return send_msg();
}

int modbusdaqo::call_modbusdaqo_code3()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	int cur_pos = 0 ;

	m_write_buf[ cur_pos++ ] = FIRST ;
	m_write_buf[ cur_pos++ ] = SECOND ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag / 256  ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag % 256 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = m_unit_flag ;
	m_write_buf[ cur_pos++ ] = YC_CODE3 ;
	m_write_buf[ cur_pos++ ] = m_config_ptr->yc_start_no / 256 ;
	m_write_buf[ cur_pos++ ] = m_config_ptr->yc_start_no % 256 ;
	m_write_buf[ cur_pos++ ] = m_yc_all_num / 256 ;
	m_write_buf[ cur_pos++ ] = m_yc_all_num % 256 ;

	m_write_buf[ 4 ] = ( cur_pos - 6 )/256 ;
	m_write_buf[ 5 ] = ( cur_pos - 6 )%256 ;

	m_write_lenth = cur_pos ;

	return send_msg();
}

TYPE_IDENTIFICATION modbusdaqo::calc_frame()
{
	TYPE_IDENTIFICATION frame_type = INVALID_FRAME ;
	int cur_position ;
	int frame_len = 0 ;
	ACE_DEBUG( ( LM_DEBUG , "calc_frame\n" ) ) ;

	ACE_OS::memset( m_modbusdaqo_buf , 0 , MAX_BUF_SIZE ) ;

	for ( cur_position = 0 ;cur_position < m_read_lenth ; cur_position++ )
	{
		if (  m_read_buf[ cur_position ] == FIRST  && m_read_buf[ cur_position + 1 ] == SECOND )		//移动置报文头
		{
			frame_len  = m_read_buf[ cur_position + 4 ] * 256 + m_read_buf[ cur_position + 5 ] + 6 ;
			frame_type = (TYPE_IDENTIFICATION)m_read_buf[ cur_position + 7 ] ;
			if( ( cur_position + frame_len ) > m_read_lenth )				//不完整报文保留至下次处理	
			{
				ACE_OS::memmove( m_read_buf , &m_read_buf[ cur_position ] , m_read_lenth - cur_position ) ;
				m_read_lenth -= cur_position ;
				frame_len = 0 ;
			}
			else
			{
				ACE_OS::memcpy( m_modbusdaqo_buf , &m_read_buf[ cur_position ] , frame_len ) ;   //拷贝一帧完整报文待处理
				//将m_read_buf中未处理的报文移位
				ACE_OS::memmove( m_read_buf , &m_read_buf[ cur_position + frame_len ] , m_read_lenth - frame_len - cur_position ) ;
				m_read_lenth = m_read_lenth - cur_position - frame_len ;
			}
			if(  m_read_lenth < 0 || m_read_lenth > REC_LEN  )	//报文缓冲区超限，全部清空
			{
				m_read_lenth = 0 ;
				ACE_OS::memset( m_read_buf , 0 , REC_LEN ) ;	
			}
			ACE_OS::memset( &m_read_buf[ m_read_lenth ] , 0 , REC_LEN - m_read_lenth ) ;	//移位后的数据区需初始化
			break ;
		}// 报文头判断
	}

	if ( cur_position == m_read_lenth )	//整个报文区数据都不对，全部清空
	{
		m_read_lenth = 0 ;
		ACE_OS::memset( m_read_buf , 0 , REC_LEN ) ;	
	}
	return frame_type ;
}

int modbusdaqo::proc_modbusdaqo_frame( TYPE_IDENTIFICATION frametype )
{
	int ret = -1 ;
	ACE_DEBUG( ( LM_DEBUG , "proc_modbusdaqo_frame\n" ) ) ;

	if ( frametype > ERR_CODE )
	{
		ACE_DEBUG( ( LM_DEBUG , "data error !!! \n" ) ) ;
		return 0 ;
	}
	switch ( frametype )
	{
	case YX_CODE1:
		ret = deal_modbusdaqo_code1() ;
		break;
	case YC_CODE3:
		ret = deal_modbusdaqo_code3() ;
		break;
	case YK_CODE6 :
		ret = deal_yk_frame() ;
		break;
	default:
		break;
	}

	return ret ;
}

int modbusdaqo::deal_modbusdaqo_code3()
{
	int cur_pos = 8 ;
	int yc_num = m_modbusdaqo_buf[ cur_pos ] / 2 ;
	if ( yc_num != m_yc_all_num )
	{
		return -1 ;
	}

	cur_pos++ ;
	short ycval ;
	for ( int i = 0 ; i < m_yc_all_num ; i++ )
	{
		ycval = m_modbusdaqo_buf[ cur_pos ] *256 + m_modbusdaqo_buf[ cur_pos + 1 ] ;
//		make_yc_real_data( m_config_ptr->yc_start_no + i , ycval , 1 ) ;
		make_yc_real_data( i , ycval , 1 ) ;
		cur_pos += 2 ;
	}
	
	return 0 ;
}

int modbusdaqo::deal_modbusdaqo_code1()
{
	int cur_pos = 8 ;
	int tmp_num = ( m_yx_all_num + 7 ) / 8 ;
	if ( tmp_num != m_modbusdaqo_buf[ cur_pos ] )
	{
		return -1 ;
	}

//	cur_pos++ ;
	unsigned char yxval ;
	for ( int i = 0 ; i < m_yx_all_num ; i++ )
	{
		int bit_pos = i % 8 ;
		if ( bit_pos == 0 )
		{
			cur_pos++ ;
		}
		yxval = ( m_modbusdaqo_buf[ cur_pos ] >> bit_pos ) & 0x01 ;
//		make_yx_real_data( m_config_ptr->yx_start_no + i , yxval , 1 ) ;
		make_yx_real_data( i , yxval , 1 ) ;
	}

	return 0 ;
}

int modbusdaqo::deal_yk_frame()
{
	int yk_ret = -1 ;
	char str[ NORMALCHAR_LEN ] ;

// 	int cur_pos = 8 ;
// 	int ykno = m_modbusdaqo_buf[ cur_pos ] *256 + m_modbusdaqo_buf[ cur_pos + 1 ] ;
// 	if ( ykno != m_current_ykno )
// 	{
// 		ACE_ERROR( ( LM_ERROR , "下发的遥控号和返校的遥控号不一致!" ) ) ;
// 		send_yk_ret( m_rtu_no , m_current_ykno , yk_ret , str ) ;
// 		return yk_ret ;
// 	}
// 
// 	cur_pos += 2 ;
// 	int ykoder = m_modbusdaqo_buf[ cur_pos ] *256 + m_modbusdaqo_buf[ cur_pos + 1 ] ;
// 	if ( ykoder != m_current_ykoder )
// 	{
// 		ACE_OS::sprintf( str ,"遥控命令错 ,下发命令:%02x 返校命令:%02x" ,m_current_ykoder ,ykoder ) ;
// 		send_yk_ret( m_rtu_no , m_current_ykno , yk_ret , str ) ;
// 		return yk_ret ;
// 	}

	yk_ret = 0 ;
	send_yk_ret( m_rtu_no , m_current_ykno , yk_ret , str ) ;

	return 0 ;
}

int modbusdaqo::send_yk_frame( unsigned char ykno , bool bclosed , unsigned char yk_flag )
{
	if ( yk_flag == 0 ) //预置直接返回成功,modbus无需预置,该方案时效性需要验证
	{
		int yk_ret = 0 ;
		char str[ NORMALCHAR_LEN ] ;
		ACE_OS::sprintf(str,"遥控返校正确!");
		send_yk_ret( m_rtu_no , ykno , yk_ret , str ) ;
		return 0 ;
	}
	if ( yk_flag == 2 )
	{
		return 0 ;
	}
	int cur_pos = 0 ;
	ACE_OS::memset( m_write_buf ,0 ,REC_LEN ) ;

	int yk_type = m_config_ptr->yk_func_code ;
	m_current_ykno = ykno + m_config_ptr->yk_start_no ;				//存储当前下发遥控序号，以备校验

	m_write_buf[ cur_pos++ ] = FIRST ;
	m_write_buf[ cur_pos++ ] = SECOND ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag / 256  ;
	m_write_buf[ cur_pos++ ] = m_protocol_flag % 256 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = 0x00 ;
	m_write_buf[ cur_pos++ ] = m_unit_flag ;
	switch( yk_type )
	{
	case YK_CODE5:
		{
			m_write_buf[ cur_pos++ ] = YK_CODE5 ;
			m_write_buf[ cur_pos++ ] = m_current_ykno / 256 ;
			m_write_buf[ cur_pos++ ] = m_current_ykno % 256 ;
			if ( bclosed )
			{
				m_write_buf[ cur_pos++ ] = 0xFF;
				m_write_buf[ cur_pos++ ] = 0x00 ;
				m_current_ykoder = 0x00 ;			//存储当前下发遥控命令，以备校验
			}
			else
			{
				m_write_buf[ cur_pos++ ] = 0x00;
				m_write_buf[ cur_pos++ ] = 0x00 ;
				m_current_ykoder = 0xFF ;			//存储当前下发遥控命令，以备校验
			}
		}
		break;		
	case YK_CODE6:
		{
			m_write_buf[ cur_pos++ ] = YK_CODE6 ;
			m_write_buf[ cur_pos++ ] = m_current_ykno / 256 ;
			m_write_buf[ cur_pos++ ] = m_current_ykno % 256 ;
			if ( bclosed )
			{
				m_write_buf[ cur_pos++ ] = 0xFF ;
				m_write_buf[ cur_pos++ ] = 0x00 ;
				m_current_ykoder = 0x00 ;			//存储当前下发遥控命令，以备校验
			}
			else
			{
				m_write_buf[ cur_pos++ ] = 0x00 ;
				m_write_buf[ cur_pos++ ] = 0x00 ;
				m_current_ykoder = 0xFF ;			//存储当前下发遥控命令，以备校验
			}
		}
		break;
	}

	m_write_buf[ 4 ] = ( cur_pos - 6 )/256 ;
	m_write_buf[ 5 ] = ( cur_pos - 6 )%256 ;

	m_write_lenth = cur_pos ;

	return send_msg() ;
}

int modbusdaqo::send_trans_yx( unsigned char yxno , unsigned char yx_value )
{
	return 0 ;
}