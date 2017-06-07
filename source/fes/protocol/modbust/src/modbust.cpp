/**
@file modbust.cpp
@brief 带断点续传的modbus规约处理源文件

@author lujiashun 
@version 1.0.0
@date 2016-3-23
*/

#include "modbust.h"

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
const char* const desc_string = "the very beginning";

//连接不成功后，等待2秒再尝试连接
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

/**
@brief IEC104配置项
*/
static MODBUST_CONFIG modbust_config[MAX_CHAN_NUM];

//预置的控制帧报文
unsigned char CONF_LK_VAL[4]={0x00,0x00,0xff,0xff};


FETIME ModbustTime_To_Realtime(MODBUST_TIME* mTime);

Base_protocol* get_protocol(const int& channel)
{
	return new Modbust(channel);
}

void get_config(void)
{
	//todo
	//从数据库读取104配置信息，写入iec104_config
	struct MODBUST_CONFIG_
	{
		unsigned short channel_no; //通道号

		unsigned short yx_begin;	///<遥信起始地址
		unsigned short yc_begin;	///<遥测起始地址
		unsigned short kwh_begin;	///<电度起始地址

		unsigned char reply_data_flag;		///<收到遥测、遥信、遥脉数据后回复确认帧，1为回复，0为不回复
	};
	MODBUST_CONFIG_ * special_config = NULL;

	int ret = Db_connector::select("select * from modbust_special_config order by  channel_no asc", (void **)&special_config);
	int rcd_cnt = ret;		//规约特殊配置元素数量

	if(ret < 0)	//访问数据库出错
	{
		return;
	}

// 	ACE_DEBUG((LM_DEBUG, "read iec104 special \n"));

	for (int nnum = 0; nnum < rcd_cnt; ++nnum)
	{
		unsigned short channel_no = special_config->channel_no;

		modbust_config[channel_no].yx_begin = special_config->yx_begin;
		modbust_config[channel_no].yc_begin = special_config->yc_begin;
		modbust_config[channel_no].kwh_begin = special_config->kwh_begin;

		modbust_config[channel_no].reply_data_flag = special_config->reply_data_flag;
		
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


Modbust::Modbust(const int& channel)
:Base_protocol(channel), m_config_ptr(modbust_config + channel)
{
// 	ACE_DEBUG((LM_DEBUG, "iec104 constructor\n"));

	m_common_addr = m_chanpara->common_addr;	//公共地址

}

Modbust::~Modbust()
{

}

void Modbust::init_protocol(void)
{
// 	ACE_DEBUG((LM_DEBUG, "modbust init\n"));
	m_confirm_link_flag = false;

	m_yx_all_num = Fes_shm::get().chan_para_ptr->yx_num;
	m_yc_all_num = Fes_shm::get().chan_para_ptr->yc_num;
	m_pi_all_num = Fes_shm::get().chan_para_ptr->kwh_num;

	ACE_OS::memset(&m_read_buf, 0, REC_LEN);
	ACE_OS::memset(&m_write_buf, 0, TRN_LEN);
}

void Modbust::protocol_proc(void)
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
			write_ret = modbust_send_proc();
			if (-1 == write_ret)
			{
				ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
// 				break;
			}

			//接收报文，并判断各种超时
			read_ret  = recv_msg();	
			if (-1 == read_ret)			//读端口失败会导致本层循环退出
			{
				ACE_DEBUG((LM_DEBUG, "读端口失败\n"));
				break;
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
					}
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
			}

			if (m_chandata->curr_attend_host != curr_host_no)		//判断归还负荷
			{
				close_dev();
				if( m_chanpara->dev_type == TCP_SERVER_DEV )
				{
					Tcp_server * pSrv = dynamic_cast<Tcp_server*>(m_dev_ptr);
					pSrv->close_listen();
				}

				break;
			}
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));

		}//while (true) 二层循环
 	}//while (true)  一层循环
}

int Modbust::modbust_send_proc(void)
{
	int ret = 0xffff;
	if (m_confirm_link_flag && m_config_ptr->reply_data_flag)
	{
		ret = confirm_link();
		m_confirm_link_flag = false;
	}
	if (m_call_yc_data_flag == NEED_CALL)
	{
		call_yc_data();
	}
	if (m_call_yx_data_flag == NEED_CALL)
	{
		call_yx_data();
	}
	if (m_call_ym_data_flag == NEED_CALL)
	{
		call_ym_data();
	}
	return ret;
}


Modbust::FRAME_TYPE Modbust::judge_frame()
{
	int cur_position;
	int frame_Len = 0;

	FRAME_TYPE  judge;
	judge = INVALID_FRAME; //初始化为无效帧
	ACE_OS::memset(m_apdu_buf, 0, MAX_BUF_SIZE);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == m_common_addr && (m_read_buf[cur_position+1]==0x10 || m_read_buf[cur_position+1]==0x0f))	//第一个字节是装置地址,第二位是0x10、0x0f
		{
			frame_Len = m_read_buf[cur_position + 8]*256+m_read_buf[cur_position+9] + 18;//数据字节数+18

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
	if ((m_apdu_buf[0] != m_common_addr))	//无效帧处理
	{
		return judge;
	}
	unsigned short crc = CRC16(m_apdu_buf,frame_Len-2);
	if (LOBYTE(crc)!=m_apdu_buf[frame_Len-2] || HIBYTE(crc)!=m_apdu_buf[frame_Len-1])	//校验码错误
	{
		return judge;
	}

	switch (m_apdu_buf[1])
	{
	case 0x10:
	case 0x0f:
		{
			m_v_recv++;		//接收到I帧后，接收计数器累加
			judge = I_FRAME;
		}
		break;
	default:
		break;
	} 
	return judge;
}

int Modbust::deal_i_frame(void)
{
	int ret = 0;


	MODBUST_APDU *apdu_modbust = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_modbust->apci.data_begin_addr_h*256+apdu_modbust->apci.data_begin_addr_l;
	switch (apdu_modbust->apci.type)
	{
	case M_ME_TA_1:							
		if (data_begin_addr >= m_config_ptr->yc_begin && data_begin_addr < m_config_ptr->kwh_begin)//yc处理
		{
			ret = deal_sequence_M_ME_NA_1();
			m_confirm_link_flag = true;
		}
		else if (data_begin_addr == m_config_ptr->kwh_begin)//ym处理
		{
			ret = deal_sequence_M_IT_NA_1();
			m_confirm_link_flag = true;
		}
		break;
	case M_SP_TA_1:			//yx处理
		ret = deal_sequence_M_SP_NA_1();
		m_confirm_link_flag = true;
		break;
	default:
		break;
	} // end switch(apdu_104->asdu.type)

	if (ret == -1)
	{
		ACE_DEBUG((LM_DEBUG, "数据处理出错\n"));
	}

	return ret;
}

int Modbust::deal_error_frame(void)
{
 	ACE_DEBUG((LM_DEBUG, "recv invaild frame!\n"));
	return 0;
}

#pragma pack(1)

//以下函数为I帧所有类型的处理///////////////////////////////////////////////////////////////////////////////////////

//YX类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_SP_NA_1(void)
{
	MODBUST_APDU *pMbDataUnit = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = pMbDataUnit->apci.data_begin_addr_h*256+pMbDataUnit->apci.data_begin_addr_l;

	if(pMbDataUnit->apci.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	if(pMbDataUnit->apci.type != M_SP_TA_1)		//判断类型
	{
		return -1;
	}
	if(data_begin_addr != m_config_ptr->yx_begin)	//遥信起始地址为0x0000
	{
		return -1;
	}

	int datanum = pMbDataUnit->apci.data_num_h*256+pMbDataUnit->apci.data_num_l; //获取当前帧的遥信数据个数
	int data_bytenum = pMbDataUnit->apci.data_byte_num_h*256+pMbDataUnit->apci.data_byte_num_l;	//遥信所占字节数
	int data_no = 0;		//遥测点号
	unsigned char value = 0;

	MODBUST_TIME *data_time = (MODBUST_TIME*)(pMbDataUnit->buf+data_bytenum); 
	FETIME time;			//遥测发生时间
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<data_bytenum;i++)
	{
		unsigned char tmpdata= pMbDataUnit->buf[i];
		for (int j=0;j<8; j++)
		{
			value = (tmpdata>>j) & 0x01;
			if (data_no > datanum )	//大于上送遥信数
			{
				break;
			}
			make_yx_real_data(data_no,value,1,&time);
			data_no++;
		}
	}
	return 0;
}

//YX类型处理end///////////////////////////////////////////////////////////////////////////////////////

//YC类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_ME_NA_1()		//0x10标识YC处理
{
	MODBUST_APDU *apdu_104 = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_104->apci.data_begin_addr_h*256+apdu_104->apci.data_begin_addr_l;

	if(apdu_104->apci.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	if(apdu_104->apci.type != M_ME_TA_1)		//判断类型
	{
		return -1;
	}
// 	if(data_begin_addr != m_config_ptr->yc_begin)	//遥测起始地址为0x0000
// 	{
// 		return -1;
// 	}

	int datanum = apdu_104->apci.data_num_h*256+apdu_104->apci.data_num_l; //获取当前帧的遥测数据个数
	int data_bytenum = apdu_104->apci.data_byte_num_h*256+apdu_104->apci.data_byte_num_l;	//遥测所占字节数

	int data_no = 0;		//遥测点号

	MODBUST_TIME *data_time = (MODBUST_TIME*)(apdu_104->buf+data_bytenum); 
	FETIME time;			//遥测发生时间
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<datanum;i++)
	{
		MODBUST_YC* tmpdata= (MODBUST_YC*)(apdu_104->buf+i*2);
		unsigned short int yc_data = tmpdata->data_yc_h*256+tmpdata->data_yc_l;
		make_yc_real_data(i+data_begin_addr,yc_data,1,&time);
	}
	return 0;
}


//YC类型处理end///////////////////////////////////////////////////////////////////////////////////////

//KWH类型处理begin///////////////////////////////////////////////////////////////////////////////////////
int Modbust::deal_sequence_M_IT_NA_1()		//0x10电度
{

	MODBUST_APDU *apdu_104 = (MODBUST_APDU*)m_apdu_buf;
	unsigned short int data_begin_addr = apdu_104->apci.data_begin_addr_h*256+apdu_104->apci.data_begin_addr_l;

	if(apdu_104->apci.addr != m_common_addr)		//判断公共地址
	{
		return -1;
	}
	if(apdu_104->apci.type != M_ME_TA_1)		//判断类型
	{
		return -1;
	}
	if(data_begin_addr != m_config_ptr->kwh_begin)	//遥测起始地址为3000
	{
		return -1;
	}

	int datanum = (apdu_104->apci.data_num_h*256+apdu_104->apci.data_num_l)/2; //获取当前帧的电度数据个数=寄存器个数/2
	int data_bytenum = apdu_104->apci.data_byte_num_h*256+apdu_104->apci.data_byte_num_l;	//电度量所占字节数
	int data_no = 0;		//遥测点号

	MODBUST_TIME *data_time = (MODBUST_TIME*)(apdu_104->buf+data_bytenum); 
	FETIME time;			//遥测发生时间
	time = ModbustTime_To_Realtime(data_time);
	for (int i=0;i<datanum;i++)
	{
		MODBUST_KWH* tmpdata= (MODBUST_KWH*)(apdu_104->buf+i*4);
		unsigned long ym_data = tmpdata->data_kwh_hh*256*256*256+tmpdata->data_kwh_hl*256*256+tmpdata->data_kwh_lh*256+tmpdata->data_kwh_ll;
		make_kwh_real_data(i,ym_data,1,&time);
	}
	return 0;
}
//KWH类型处理end///////////////////////////////////////////////////////////////////////////////////////


//将规约上送的时间转换为真实时间
FETIME ModbustTime_To_Realtime(MODBUST_TIME* mTime)
{
	FETIME Time ;
	Time.milisecond = 0;
	Time.second = mTime->secend;
	Time.minute = mTime->minite;
	Time.hour = mTime->hour;
	Time.day = mTime->day;
	Time.month = mTime->month;
	Time.year = mTime->year+100;	//规约上送的时间是0f代表2015年  FETIME中使用115代表2015

	return Time;
}

int Modbust::confirm_link()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);
	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = C_CG_TA_1 ;
	MODBUST_APDU *apdu_modbust = (MODBUST_APDU*)m_apdu_buf;
	m_write_buf[ 2 ] = apdu_modbust->apci.send_num_h;
	m_write_buf[ 3 ] = apdu_modbust->apci.send_num_l;
	ACE_OS::memcpy(m_write_buf+4,CONF_LK_VAL,sizeof(CONF_LK_VAL));//4个字节固定码

	FETIME tim;
	get_time(&tim);
	m_write_buf[8] = tim.year-100;	//FETIME.year和实际相差1900年，下发下去的年是2000年
	m_write_buf[9] = tim.month;
	m_write_buf[10] = tim.day;
	m_write_buf[11] = tim.hour;
	m_write_buf[12] = tim.minute;
	m_write_buf[13] = tim.second;

	unsigned short crc = CRC16( m_write_buf , 14 ) ;
	m_write_buf[14] = HIBYTE(crc);
	m_write_buf[15] = LOBYTE(crc);

	m_write_lenth = CONFIRM_LINK_LEN;

	return send_msg();
}

int Modbust::call_yc_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_ME_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->yc_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->yc_begin);
	m_write_buf[ 4 ] = HIBYTE(m_yc_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_yc_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();
}

int Modbust::call_yx_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_SP_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->yx_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->yx_begin);
	m_write_buf[ 4 ] = HIBYTE(m_yx_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_yx_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();

}

int Modbust::call_ym_data()
{
	ACE_OS::memset(m_write_buf,0,REC_LEN);

	m_write_buf[ 0 ] = m_common_addr ;
	m_write_buf[ 1 ] = M_ME_TA_1 ;
	m_write_buf[ 2 ] = HIBYTE(m_config_ptr->kwh_begin);
	m_write_buf[ 3 ] = LOBYTE(m_config_ptr->kwh_begin);
	m_write_buf[ 4 ] = HIBYTE(m_pi_all_num);
	m_write_buf[ 5 ] = LOBYTE(m_pi_all_num);

	unsigned short crc = CRC16( m_write_buf , 6 ) ;
	m_write_buf[6] = LOBYTE(crc);
	m_write_buf[7] = HIBYTE(crc);

	m_write_lenth = CALL_DATA_LEN;

	return send_msg();
}

unsigned short Modbust::CRC16( unsigned char *arr_buff, int len)
{
	unsigned short crc =0xFFFF;
	unsigned short i, j;
	for ( j=0; j<len;j++)
	{
		printf("j=%d\n",j);
		crc=crc ^*arr_buff++;
		for ( i=0; i<8; i++)
		{
			if( ( crc&0x0001) >0)
			{
				crc=crc>>1;
				crc=crc^ 0xa001;
			}
			else
				crc=crc>>1;
		}
	}
	return ( crc);
}

int Modbust::send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag)
{
	return 1;
}
