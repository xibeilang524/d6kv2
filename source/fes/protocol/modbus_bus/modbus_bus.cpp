#include "modbus_bus.h"
#include "fes/fesscdshm.h"
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
#include <QDebug>

const char* const version_string = "1.0.0";
const char* const desc_string = "modbus_bus";

//连接不成功后，等待2秒再尝试连接
const int CONN_RETRY_TIME = 2;
const int SECOND_LOOP_TIME = 1;

static CModbusParaConfig modbus_bus_config[MAX_CHAN_NUM];

Base_protocol* get_protocol(const int& channel)
{
	return new CMbBusProto(channel);
}

void get_config(void)
{
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

		modbus_bus_config[channel_no].yx_func_code = special_config->yx_func_code;
		modbus_bus_config[channel_no].yx_start_no = special_config->yx_start_no;
		modbus_bus_config[channel_no].yx_call_time = special_config->yx_call_time;
		modbus_bus_config[channel_no].yc_func_code = special_config->yc_func_code;
		modbus_bus_config[channel_no].yc_start_no = special_config->yc_start_no;
		modbus_bus_config[channel_no].yc_call_time = special_config->yc_call_time;
		modbus_bus_config[channel_no].yk_func_code = special_config->yk_func_code;
		modbus_bus_config[channel_no].yk_start_no = special_config->yk_start_no;
		modbus_bus_config[channel_no].t0 = special_config->t0;

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


CMbBusProto::CMbBusProto(const int& channel):Base_protocol(channel),m_pConfig(modbus_bus_config+channel)
{
	m_sCommAddr=m_chanpara->common_addr;
}

CMbBusProto::~CMbBusProto()
{

}

/*! \fn int modbus_bus::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
********************************************************************************************************* 
** \brief modbus_bus::send_yt_frame 
** \details  发送遥调事项，因系统原因只能发送一个遥调数据，不能发送多个遥调事项
** \param ytno 
** \param set_value 
** \param yt_flag 
** \return int 
** \author xingzhibing
** \date 2017年4月12日 
** \note 
********************************************************************************************************/
int CMbBusProto::send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag)
{
	if ( yt_flag == 0 ) 
	{
		return 0 ;
	}
	if ( yt_flag == 2 )
	{
		return 0 ;
	}

	int nCurPos = 0 ;
	ACE_OS::memset( m_write_buf ,0 ,REC_LEN ) ;

	int yk_type = m_pConfig->yt_func_code ;
	m_current_ykno = ytno + m_pConfig->yt_start_no ;				//存储当前下发遥控序号，以备校验

	m_write_buf[ nCurPos++ ] = m_sCommAddr ;

	m_write_buf[ nCurPos++ ] = CODE_SETVAL ;
	m_write_buf[ nCurPos++ ] = MYHI8(m_current_ykno) ;
	m_write_buf[ nCurPos++ ] = MYLO8(m_current_ykno) ;

	m_write_buf[ nCurPos++ ] =  MYHI8(1);
	m_write_buf[ nCurPos++ ] =  MYLO8(1);

	unsigned short usCRC=CRC16(m_write_buf,nCurPos);

	m_write_buf[ nCurPos++ ] =  MYLO8(usCRC) ;
	m_write_buf[ nCurPos++ ] =  MYHI8(usCRC);

	m_write_lenth=nCurPos;

	return send_msg();

}

/*! \fn void modbus_bus::protocol_proc(void)
********************************************************************************************************* 
** \brief modbus_bus::protocol_proc 
** \details 处理报文
** \param  
** \return void 
** \author xingzhibing
** \date 2017年4月12日 
** \note 
********************************************************************************************************/
void CMbBusProto::protocol_proc(void)
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
		bool bRet=true;

		while ( !m_nEnd  && bRet )
		{
			int write_ret = 0;
			int read_ret = 0;
			//检查邮件，并对邮件进行相关处理
			m_write_lenth = 0 ;  //发送长度清零
			check_event( ) ;

			//按顺序发送数据 YC
			for (int nIndex=0;nIndex<m_arrAnalogStartAddr.size()  && bRet ;++nIndex)
			{
				write_ret=SendMbBuff(ANALOG_TYPE,nIndex);

				if (-1 == write_ret)
				{
					ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
					bRet=false;
					break;
				}
				read_ret = recv_msg() ;

				//读端口失败都会导致本层循环退出
				if ( -1 == read_ret )
				{
					bRet=false;
					ACE_DEBUG( ( LM_DEBUG , "读端口失败\n" ) ) ;
					break ;
				}
				else if ( 0 == read_ret )		//未接收到数据，判断超时
				{		
					if(  m_TimeOut.elapsed() >= m_pConfig->t0  )
					{
						m_TimeOut.restart() ;
						bRet=false;
						close_dev() ;
						break ;
					}
				}
				else
				{
					m_TimeOut.restart() ;
				}
				//以下是收到了报文的处理
				if ( m_read_lenth > 0  && bRet )	//接收缓冲区有报文再进行处理
				{
					int  frame_type = GetFullMsg() ;
					//Q_ASSERT(frame_type==-1);
					if (frame_type !=-1)
					{
						ProcMbBuff(ANALOG_TYPE,nIndex) ;
					}					
				}			
			}	
			ACE_OS::sleep( ACE_Time_Value( 0 ,1000*10 ) ) ;
			//按顺序发送数据 KWH 
			for (int nIndex=0;nIndex<m_arrKwhStartAddr.size() && bRet ;++nIndex)
			{
				write_ret=SendMbBuff(KWH_TYPE,nIndex);

				if (-1 == write_ret)
				{
					ACE_DEBUG((LM_DEBUG, "写端口失败\n"));
					bRet=false;
					break;
				}
				read_ret = recv_msg() ;
				//读端口失败都会导致本层循环退出
				if ( -1 == read_ret )
				{
					ACE_DEBUG( ( LM_DEBUG , "读端口失败\n" ) ) ;
					bRet=false;
					break ;
				}
				else if ( 0 == read_ret )		//未接收到数据，判断超时
				{		
					if(  m_TimeOut.elapsed() >= m_pConfig->t0  )
					{
						m_TimeOut.restart() ;
						bRet=false;
						close_dev() ;
						break ;
					}
				}
				else
				{
					m_TimeOut.restart() ;
				}
				//以下是收到了报文的处理
				if ( m_read_lenth > 0 )	//接收缓冲区有报文再进行处理
				{
					int  frame_type = GetFullMsg() ;
				//	Q_ASSERT(frame_type!=-1);
					if (frame_type!=-1)
					{
						ProcMbBuff(KWH_TYPE,nIndex) ;
					}					
				}			
			}		
			ACE_OS::sleep( ACE_Time_Value( 0 ,1000*10 ) ) ;
		}
	}
}

/*! \fn void modbus_bus::init_protocol()
********************************************************************************************************* 
** \brief modbus_bus::init_protocol 
** \details 初始化规约信息
** \return void 
** \author xingzhibing
** \date 2017年4月12日 
** \note 
********************************************************************************************************/
void CMbBusProto::init_protocol()
{
	m_TimeOut.restart();
	m_YcTimer.restart();
	m_YxTimer.restart();

	m_nTotalYXNum=m_chanpara->yx_num;
	m_nTotalYCNum=m_chanpara->yc_num;
	m_nTotalKwhNum=m_chanpara->kwh_num;

	//以下为报文基本分帧信息 

	//!<1> YC
	Q_ASSERT(m_nTotalYCNum);

	int nCount     = m_nTotalYCNum / PER_MSG_GetValNum;
	int nLeftCount = m_nTotalYCNum % PER_MSG_GetValNum;

	if (nLeftCount!=0) 
	{
		nCount+=1;
	}

	for (int i=0;i<nCount;++i)
	{
		m_arrAnalogStartAddr.push_back(i*PER_MSG_GetValNum+m_pConfig->yc_start_no);
		m_arrAnalogCountPerFrame.push_back(m_nTotalYCNum-i*PER_MSG_GetValNum);
	}

	nCount     = 0;
	nLeftCount = 0;
	//!<2> KWH
	//Q_ASSERT(m_nTotalKwhNum);

	nCount     = m_nTotalKwhNum / PER_MSG_GetValNum;
	nLeftCount = m_nTotalKwhNum % PER_MSG_GetValNum;

	if (nLeftCount!=0) 
	{
		nCount+=1;
	}

	for (int i=0;i<nCount;++i)
	{
		m_arrKwhStartAddr.push_back(i*PER_MSG_GetValNum+m_pConfig->kwh_start_no);
		m_arrKwhCountPerFrame.push_back(m_nTotalKwhNum-i*PER_MSG_GetValNum);
	}

	m_bYCCallEnable=true;
	m_bKwhCallEnable=true;

	ACE_OS::memset( &m_read_buf  , 0 , REC_LEN ) ;
	ACE_OS::memset( &m_write_buf , 0 , TRN_LEN ) ;


}

/*! \fn void modbus_bus::CRC16(unsigned char* pBuff,unsigned long len)
********************************************************************************************************* 
** \brief modbus_bus::CRC16 
** \details crc16校验函数
** \param pBuff 
** \param len 
** \return void 
** \author xingzhibing
** \date 2017年4月12日 
** \note 
********************************************************************************************************/
unsigned short CMbBusProto::CRC16(unsigned char* buf,unsigned long len)
{
	unsigned short crc;
	unsigned short i,j,k;

	crc=0xffff;

	for(i=0;i<len;i++)
	{
		crc =crc ^ buf[i];
		for(j=0;j<8;j++)
		{
			k=crc & 01;
			crc=crc >> 1;
			if (k==0) continue;
			crc =crc ^ 0xA001;
		}
	}
	return crc;
}

/*! \fn int CMbBus::SendMbBuff()
********************************************************************************************************* 
** \brief CMbBus::SendMbBuff 
** \details 
** \return int 
** \author xingzhibing
** \date 2017年4月17日 
** \note 处理modbus 发送缓冲区
********************************************************************************************************/
int CMbBusProto::SendMbBuff(int nType,int nIndex)
{
	unsigned short nStartRegAddr=0;     //寄存器起始地址
	unsigned short nCount=0;            //查询数目

	if (nType==ANALOG_TYPE)
	{
		nStartRegAddr=m_arrAnalogStartAddr[nIndex];
		nCount=m_arrAnalogCountPerFrame[nIndex];
	}
	else if (nType==KWH_TYPE)
	{
		nStartRegAddr=m_arrKwhStartAddr[nIndex];
		nCount=m_arrKwhCountPerFrame[nIndex];
	}	

	ACE_OS::memset(m_write_buf,0,REC_LEN);

	int nCurPos = 0 ;

	m_write_buf[ nCurPos++ ] =  m_sCommAddr;
	m_write_buf[ nCurPos++ ] =  GetFunCode(nType);
	m_write_buf[ nCurPos++ ] =  MYHI8(nStartRegAddr);
	m_write_buf[ nCurPos++ ] =  MYLO8(nStartRegAddr);
	m_write_buf[ nCurPos++ ] =  MYHI8(nCount);
	m_write_buf[ nCurPos++ ] =  MYLO8(nCount);

	unsigned short usCRC=CRC16(m_write_buf,nCurPos);

	m_write_buf[ nCurPos++ ] =  MYLO8(usCRC) ;
	m_write_buf[ nCurPos++ ] =  MYHI8(usCRC);

	m_write_lenth=nCurPos;

	return send_msg();
}

int CMbBusProto::GetFunCode(int nType)
{
	return (nType==ANALOG_TYPE)?(CODE_VAL):((nType==KWH_TYPE)?(CODE_GetSetVAL):CODE_SETVAL);
}

/*! \fn int CMbBus::RecvMbBuff(int nType,int nIndex)
********************************************************************************************************* 
** \brief CMbBus::RecvMbBuff 
** \details 接收处理每一帧报文
** \param nType 
** \param nIndex 
** \return int 
** \author xingzhibing
** \date 2017年4月18日 
** \note 
********************************************************************************************************/
int CMbBusProto::ProcMbBuff(int nType,int nIndex)
{
	unsigned char  funCode; //功能码

	funCode = m_Buff[BIT_FUNCODE];

	switch (funCode)
	{
	case CODE_VAL:
		{
			ParaseAnalog(nIndex);
			break;
		}	
	case CODE_GetSetVAL:
		{
			ParaseKwh(nIndex);
			break;
		}		
	}
	return 0;
}

int CMbBusProto::GetFullMsg()
{
	int cur_position;
	int frame_Len = 0;

	int nJudge=-1;

	ACE_OS::memset(m_Buff, 0, MAX_BUFF_LENGTH);

	for (cur_position = 0;cur_position < m_read_lenth; cur_position++)
	{
		if (m_read_buf[cur_position] == m_sCommAddr && (m_read_buf[cur_position+1]==CODE_VAL || m_read_buf[cur_position+1]==CODE_GetSetVAL))	//第一个字节是装置地址,第二位是0x10、0x0f
		{
			frame_Len = m_read_buf[cur_position+2]  + 5;//数据字节数+5

			if((cur_position + frame_Len) > m_read_lenth)				//不完整报文保留至下次处理	
			{
				ACE_OS::memmove(m_read_buf, &m_read_buf[cur_position], m_read_lenth - cur_position);
				m_read_lenth -= cur_position;
				frame_Len = 0;
			}
			else
			{
				ACE_OS::memcpy(m_Buff, &m_read_buf[cur_position], frame_Len);   //拷贝一帧完整报文待处理

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

	if (frame_Len == 0)		
	{
		return nJudge;
	}
	if ((m_Buff[0] != m_sCommAddr))
	{
		return nJudge;
	}
	unsigned short crc = CRC16(m_Buff,frame_Len-2);
	if (MYLO8(crc)!=m_Buff[frame_Len-2] || MYHI8(crc)!=m_Buff[frame_Len-1])	//校验码错误
	{
		return nJudge;
	}
	return  m_Buff[1];
}

int CMbBusProto::ParaseAnalog(int nIndex)
{
	short realData;

	unsigned char ucDataBytes = m_Buff[BIT_DATA_LEN];
	int nAnalogCount = ucDataBytes/2;//每两个字节一个analog

	if (nAnalogCount >PER_MSG_GetValNum )
	{
		nAnalogCount = PER_MSG_GetValNum;
	}

	unsigned char*pDataBuf = m_Buff+BIT_DATA_HEAD;//数据区开始

	for (int i=0; i<nAnalogCount; i++)
	{
		realData = pDataBuf[2*i]*256 + pDataBuf[2*i+1];

		int nAnalogID = m_arrAnalogStartAddr[nIndex]+i;

		int nDeviceID = m_sCommAddr;

		make_yc_real_data(nAnalogID,realData,1);

		char buf[256];
		sprintf(buf,"AnalogID:%d,RealVal:%f",nAnalogID,realData);
		ACE_DEBUG((LM_DEBUG,buf));
	//	qDebug()<<"AnalogID:"<<nAnalogID<<"RealData:"<<realData;
	}
	return 0;
}

int CMbBusProto::ParaseKwh(int nIndex)
{
	short realData;

	unsigned char ucDataBytes = m_Buff[BIT_DATA_LEN];
	int nKwhCount = ucDataBytes/2;//每两个字节一个analog

	if (nKwhCount >PER_MSG_GetValNum )
	{
		nKwhCount = PER_MSG_GetValNum;
	}

	unsigned char*pDataBuf = m_Buff+BIT_DATA_HEAD;//数据区开始

	for (int i=0; i<nKwhCount; i++)
	{
		realData = pDataBuf[2*i]*256 + pDataBuf[2*i+1];

		int nKwhID = m_arrAnalogStartAddr[nIndex]+i;

		int nDeviceID = m_sCommAddr;

		make_kwh_real_data(nKwhID,realData,1);

		char buf[256];
		sprintf(buf,"KwhID:%d,RealVal:%f",nKwhID,realData);

		ACE_DEBUG((LM_DEBUG,buf));

//		qDebug()<<"KwhID:"<<nKwhID<<"RealData:"<<realData;
	}
	return 0;
}
