/**
@file channelcluser.cpp
@brief 前置通道类的实现

提供参数的加载，通道实例的创建等功能

@author hongxiang 
@version 1.0.0
@date 2013-10-10
*/

#include "fes/channelchuser.h"
#include "fes/platformwrapper.h"
#include "fes/baseprotocol.h"
#include "fes/fesdatashm.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/femailstruct.h"
#include <ace/os.h>
#include <ace/Log_Msg.h>
#include <vector>

using namespace std;

namespace
{
	const char* CHANNEL_FILE_NAME = "log/dat/channel.dat";
	const char* RTU_FILE_NAME = "log/dat/rtu.dat";
	const char* PROTOCAL_FILE_NAME = "log/dat/protocol.dat";
}

/**************************************************************************
构造函数
**************************************************************************/
Channel_chuser::Channel_chuser(ACE_Thread_Mutex& check_watch_load_mutex)
:m_check_watch_load_mutex(check_watch_load_mutex)
{
	m_channel_para = new CHANPARA[MAX_CHAN_NUM];	//创建通道参数结构体
	m_rtu_para = new RTUPARA[MAX_RTU_NUM];			//创建终端参数结构体

 	m_protocol = new Base_protocol*[MAX_CHAN_NUM];

	ACE_OS::memset(m_protocol, 0, MAX_CHAN_NUM*sizeof(Base_protocol*));		//modify by chris

	ACE_OS::memset(m_bnew_protocol, false, MAX_CHAN_NUM);

	generate_protocol_factory();					//创建规约工厂实例
}


/**************************************************************************
析构函数
**************************************************************************/
Channel_chuser::~Channel_chuser()
{
	delete[] m_channel_para;
	m_channel_para = NULL;

	delete[] m_rtu_para;
	m_rtu_para = NULL;
}

/**************************************************************************
读取规约类型表，创建规约工厂
**************************************************************************/
void Channel_chuser::generate_protocol_factory(void)
{
	struct PROTOCOL_
	{
		char protocol_name[CODE_LENTH]; //规约名
		char describe[LONG_DESC_LENTH]; //规约描述
	};

	PROTOCOL_* db_protocol = NULL;
	Db_connector::open();
	int ret = Db_connector::select("select protocol_name, describe from protocol", (void **)&db_protocol);
	int rcd_cnt = ret; //规约表元素数量

	//读库正常
	if (ret > 0 && rcd_cnt > 0)
	{
		//根据数据库中读出的规约参数，创建规约工厂
		for (int i = 0; i < rcd_cnt; i++)
		{
			//创建规约实例
			PROTOCOL_* protocol_in_db = db_protocol + i;
			Protocol_factory::instance()->add_protocol(protocol_in_db->protocol_name);
		}	

		//保存PROTOCOL_结构体到文件
		char path[LONGCHAR_LEN];
		ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), PROTOCAL_FILE_NAME);
		FILE* fp = ACE_OS::fopen (path, "w+b");

		if (fp == NULL)
		{
			ACE_DEBUG((LM_ERROR, "打开文件失败！\n"));
		}

		else
		{
			ACE_OS::fwrite (db_protocol, sizeof(PROTOCOL_), rcd_cnt, fp);
			ACE_OS::fclose(fp);

			ACE_DEBUG((LM_DEBUG, "保存规约参数到本地文件成功！\n"));
		}

	}//if (ret>0)
	else if(ret == 0)
	{
		ACE_DEBUG((LM_DEBUG, "数据库中无规约参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读数据库失败！\n"));
	}

	if (db_protocol)
	{
		free(db_protocol);
	}

}
/**************************************************************************
加载读取参数
**************************************************************************/
int Channel_chuser::load(void)
{

	clear_para();

	read_protocol_config();	//读取规约特殊配置

	//读数据库失败
	if (read_channel_para_from_db() || read_rtu_para_from_db())
	{
		ACE_DEBUG((LM_ERROR, "从数据库读取参数失败，转从本地文件读取参数！\n"));
		
		//读文件失败
		if (read_channel_para_from_file() || read_rtu_para_from_file())
		{
			ACE_ERROR_RETURN((LM_ERROR, "读取参数失败！\n"), -1);
		}
	}

	calc_channel_priority();	//计算通道主备关系
	calc_rtu_priority();
	calc_rtu_channel();		//计算终端通道关系

	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_load_mutex);
	check_channels(); //检查通道状况

	return 0;
}

/**************************************************************************
把邮件写入规约的消息队列
**************************************************************************/
int Channel_chuser::put_mail(void* mail, const int& lenth)
{
	FE_MAIL_STRUCT* mail_msg = (FE_MAIL_STRUCT*)mail;

	FE_YK_MAIL* yk_mail = (FE_YK_MAIL*)mail_msg->mail_body;

	int yk_rtu_no = yk_mail->rtu_no;

	int yk_channel_no = (m_rtu_para+yk_rtu_no)->last_channel;		//获取对应rtuno的通道号

	if (yk_mail->action == 0xCC)		//add by chris 转换
	{
		yk_mail->action = 1;
	}
	else if (yk_mail->action == 0x33)
	{
		yk_mail->action = 0;
	}

	ACE_Message_Block *protocol_msg = 0;
	protocol_msg = new ACE_Message_Block(lenth);
	ACE_OS::memcpy(protocol_msg->wr_ptr(), mail, lenth);

	int current_channel_host = (Fes_shm::get().chan_data_ptr + yk_channel_no)->curr_attend_host; //遥控通道当前值班机序号
	int current_host = Fes_shm::get().fe_sys_data_ptr->host_no;		//本机序号
	if (current_channel_host == current_host)
	{
		if (m_protocol[yk_channel_no])
		{
			m_protocol[yk_channel_no]->putq(protocol_msg);
		}
	}
	return 0;
}

/**************************************************************************
读取规约特殊配置
**************************************************************************/
void Channel_chuser::read_protocol_config(void)
{
	Protocol_factory::instance()->get_all_config();
}

/**************************************************************************
读取通道参数
**************************************************************************/
int Channel_chuser::read_channel_para_from_db(void)
{
	struct CHANNEL_
	{
		unsigned short channel_no; //通道号
		char describe[SHORT_DESC_LENTH]; //通道描述
		char protocol_name[CODE_LENTH];  //规约名
		short parent_channel; //父通道名
		unsigned short common_address; //公共地址		//add by chris新增通道公共地址及主站地址，脚本做相应改动
		unsigned short master_address; //主站地址
		unsigned char is_check; //通道检测标志位
		unsigned char is_gps; //GPS标志位
		unsigned char initial_stat; //通道初始化状态
		unsigned short stop_gap; //通道停止时限
		unsigned short bit_rate_error; //误码率
		unsigned short default_attend_host; //<默认值班机节点，参数表中如果填写-1，则由系统自动计算默认值班机节点
		unsigned char dev_type; //设备类型
		char local_dev[DEV_NAME_LEN]; //本地设备地址
		unsigned short local_port; //本地端口号
		char peer_dev[DEV_NAME_LEN]; //网络设备地址
		unsigned short peer_port; //网络设备端口
		char back_peer_dev[DEV_NAME_LEN]; //网络备用设备地址
		unsigned short back_peer_port; //网络备用设备端口
		unsigned short band_rate; //波特率
		unsigned char check_bit; //校验位
		unsigned char stop_bit; //停止位
		unsigned char data_bit; //数据位
		unsigned char flow; //流控制
		unsigned char valid; //通道使用标志
	};

	CHANNEL_* db_channel = NULL;
	int rcd_cnt = Db_connector::select("select * from channel order by  channel_no asc", (void **)&db_channel); 

	//读取通道表成功，将参数写入本地文件
	if (rcd_cnt > 0)
	{	
		//将读出终端号的终端参数赋值给相应的终端参数结构体元素
		for (int i = 0; i < rcd_cnt; i++)
		{
			CHANNEL_* db_para = db_channel + i;
			int channel_no = db_channel[i].channel_no; //获取通道号

			if (channel_no >= 0 && channel_no < MAX_CHAN_NUM)
			{
				CHANPARA* para = m_channel_para + channel_no;

				para->order = db_para->channel_no;
				ACE_OS::strcpy(para->desdribe,db_para->describe);
				ACE_OS::strcpy(para->protocol,db_para->protocol_name);
				para->gps = db_para->is_gps;
				para->check = db_para->is_check;
				para->valid = db_para->valid;
				para->last_channel = db_para->parent_channel;
				para->common_addr = db_para->common_address;		//add by chris
				para->master_addr = db_para->master_address;
				para->dev_type = db_para->dev_type;
				ACE_OS::strcpy(para->local_dev,db_para->local_dev);
				para->local_port = db_para->local_port;
				ACE_OS::strcpy(para->dev_name,db_para->peer_dev);
				para->port = db_para->peer_port;
				ACE_OS::strcpy(para->backup_dev_name,db_para->back_peer_dev);
				para->back_up_port = db_para->back_peer_port;
				para->band = db_para->band_rate;
				para->check_bit = db_para->check_bit;
				para->stop_bit = db_para->stop_bit;
				para->data_bit = db_para->data_bit;
				para->flow = db_para->flow;
				para->initial_stat = db_para->initial_stat;
				para->stop_time_limit = db_para->stop_gap;
				para->bit_error_rate = db_para->bit_rate_error;
				para->default_attend_host = db_para->default_attend_host;
			}
		}

		save_channel_para();  //保存通道参数到本地文件
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无通道参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读取通道表参数失败！\n"));
	}

	if (db_channel)
	{
		free(db_channel);
	}

	return rcd_cnt > 0 ? 0 : -1;
}

/**************************************************************************
读取终端参数
**************************************************************************/
int Channel_chuser::read_rtu_para_from_db(void)
{
	struct RTU_
	{ 
		unsigned short rtu_no; //终端序号
		char rtu_code[CODE_LENTH]; //终端代码
		char describe[SHORT_DESC_LENTH]; //终端名
		unsigned short channel_no; //通道号
		unsigned short yc_num; //遥测数目
		unsigned short yx_num;//遥信数目
		unsigned short kwh_num; //电度量数目
		unsigned char valid; //使用标志
	};

	RTU_* db_rtu = NULL;
	int rcd_cnt = Db_connector::select("select * from rtu order by rtu_no asc",(void **)&db_rtu);
	
	//读取终端表成功，将参数写入本地文件
	if (rcd_cnt > 0)
	{
		//将读出的终端参数赋给终端参数结构体
		for (int i = 0; i < rcd_cnt; i++)
		{
			RTU_* db_para = db_rtu + i; 
			int rtu_no = db_rtu[i].rtu_no; //获取终端号

			if (rtu_no >= 0 && rtu_no < MAX_CHAN_NUM)
			{
				//将读出终端号的终端参数赋值给相应的终端参数结构体元素
				RTUPARA* para = m_rtu_para + rtu_no;

				para->order = db_para->rtu_no;
				para->last_channel = db_para->channel_no;
				ACE_OS::strcpy(para->rtu_code,db_para->rtu_code);
				ACE_OS::strcpy(para->desdribe,db_para->describe);
				para->yc_num = db_para->yc_num;
				para->yx_num = db_para->yx_num;
				para->kwh_num = db_para->kwh_num;
				para->valid = db_para->valid;
			}

		}//for (int i = 0; i<rcd_cnt; i++)

		save_rtu_para(); //保存终端参数到本地文件 		
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无终端参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读取终端表参数失败！\n"));
	}

	if (db_rtu)
	{
		free(db_rtu);
	}

	return rcd_cnt > 0 ? 0 : -1;
}
/**************************************************************************
读取遥测参数
**************************************************************************/
int Channel_chuser::read_yc_para_from_db(void)
{
	struct YC_PARA_DB
	{

		double slope;	//斜率
		double ordinate;//截距
		unsigned short  ter_no;//终端序号
		unsigned short  yc_no;//遥测序号


	};

	YC_PARA_DB* db_yc_para = NULL;
	vector<YC_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YC_PARA_DB> yc_buffer_db;
	vector<YC_PARA> yc_buffer;
	YC_PARA_DB tem_yc_buffer;
	YC_PARA tem_yc;
	int rtu_no = 0;
	int pos = 0;
	int current_yc_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();

	int rcd_cnt = Db_connector::select("select 斜率/倍率,截距,终端序号,遥测序号 from 遥测参数表 order by 终端序号, 遥测序号 asc ",(void **)&db_yc_para);
	int rcd_act_num = 0;
	bool isend = false;

	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
// 			m_fes_data_shm_allocator.alloc_channel_data(i);
			if (chan_para->yc_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->yc_num);
			pos = 0;
			while (true)
			{    
				current_yc_num = (m_rtu_para + rtu_no)->yc_num;
				if (current_yc_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_yc_para + j)->ter_no == rtu_no)
					{
						yc_buffer_db.push_back(*(db_yc_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}
				
				rcd_act_num = yc_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (yc_buffer_db[k].yc_no < yc_buffer_db[k-1].yc_no)
							{
								tem_yc_buffer = yc_buffer_db[k];
								yc_buffer_db[k] = yc_buffer_db[k-1];
								yc_buffer_db[k-1] = tem_yc_buffer;
							}
						}
						tem_yc.ordinate = yc_buffer_db[j].ordinate;
						tem_yc.slope = yc_buffer_db[j].slope;
						yc_buffer.push_back(tem_yc);
					}//for (int j = 0; j < rcd_act_num ; j++)
					//以当前终端设置个数为准，若实际YC个数大于终端设置个数则不算多余的YC
					if(rcd_act_num > current_yc_num)
					{
						rcd_act_num = current_yc_num;
					}
					memcpy(&chan_buffer[pos],&yc_buffer[0],rcd_act_num * sizeof(YC_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_yc_num;
				yc_buffer.clear();
				yc_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}
			m_fes_data_shm_allocator.set_yc_para(i,&chan_buffer[0],chan_para->yc_num * sizeof(YC_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无遥测参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读取遥测参数表参数失败！\n"));

	}
	return rcd_cnt > 0 ? 0 : -1;



}

/**************************************************************************
读取遥信参数
**************************************************************************/
int Channel_chuser::read_yx_para_from_db(void)
{
	struct YX_PARA_DB
	{
		unsigned char negate;	//取反，0为不取反，1为取反
		unsigned short ter_no;//终端序号
		unsigned short  yx_no;//遥信序号

	};
	YX_PARA_DB* db_yx_para = NULL;
	vector<YX_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<YX_PARA_DB> yx_buffer_db;
	vector<YX_PARA> yx_buffer;
	YX_PARA_DB tem_yx_buffer;
	YX_PARA tem_yx;
	int rtu_no = 0;
	int pos = 0;
	int current_yx_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select 取反标志,终端序号,遥信序号 from 遥信参数表 order by 终端序号, 遥信序号 asc ",(void **)&db_yx_para);		//modidfy by chris
	int rcd_act_num = 0;
	bool isend = false;
	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
			if (chan_para->yx_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->yx_num);
			pos = 0;
			while (true)
			{    
				current_yx_num = (m_rtu_para + rtu_no)->yx_num;
				if (current_yx_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_yx_para + j)->ter_no == rtu_no)
					{
						yx_buffer_db.push_back(*(db_yx_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}

				rcd_act_num = yx_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (yx_buffer_db[k].yx_no < yx_buffer_db[k-1].yx_no)
							{
								tem_yx_buffer = yx_buffer_db[k];
								yx_buffer_db[k] = yx_buffer_db[k-1];
								yx_buffer_db[k-1] = tem_yx_buffer;
							}
						}
						tem_yx.negate = yx_buffer_db[j].negate;
						yx_buffer.push_back(tem_yx);
					}
					//以当前终端设置个数为准，若实际YX个数大于终端设置个数则不算多余的YX
					if(rcd_act_num > current_yx_num)
					{
						rcd_act_num = current_yx_num;
					}
					memcpy(&chan_buffer[pos],&yx_buffer[0],rcd_act_num * sizeof(YX_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_yx_num;
				yx_buffer.clear();
				yx_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}
  			m_fes_data_shm_allocator.set_yx_para(i,&chan_buffer[0],chan_para->yx_num * sizeof(YX_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无遥信参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读取遥信参数表参数失败！\n"));

	}

	return rcd_cnt > 0 ? 0 : -1;
}

/**************************************************************************
读取电度量参数
**************************************************************************/
int Channel_chuser::read_kwh_para_from_db(void)
{
	struct KWH_PARA_DB
	{
		double full_code;	//满码
		double slope;		//斜率 = 倍率/标度
		unsigned short  ter_no;//终端序号
		unsigned short  kwh_no;//电度序号

	};
	KWH_PARA_DB* db_kwh_para = NULL;
	vector<KWH_PARA> chan_buffer; 
	CHANPARA * chan_para = NULL;
	vector<KWH_PARA_DB> kwh_buffer_db;
	vector<KWH_PARA> kwh_buffer;
	KWH_PARA_DB tem_kwh_buffer;
	KWH_PARA tem_kwh;
	int rtu_no = 0;
	int pos = 0;
	int current_kwh_num = 0;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();
	int rcd_cnt = Db_connector::select("select 满码,倍率/标度,终端序号,电度序号 from 电度参数表 order by 终端序号, 电度序号 asc ",(void **)&db_kwh_para);
	int rcd_act_num = 0;
	bool isend = false;
	if (rcd_cnt > 0)
	{
		for (int i = 0; i < MAX_CHAN_NUM; i++)
		{
			chan_para = m_channel_para + i;
			rtu_no = chan_para->child_rtu;
			if (chan_para->valid == 0)
			{
				continue;
			}
			if (chan_para->kwh_num <= 0)
			{
				continue;
			}
			chan_buffer.resize(chan_para->kwh_num);
			pos = 0;
			while (true)
			{    
				current_kwh_num = (m_rtu_para + rtu_no)->kwh_num;
				if (current_kwh_num <= 0)
				{
					rtu_no = (m_rtu_para + rtu_no)->next_rtu;
					continue;
				}

				isend = false;
				for (int j = 0; j < rcd_cnt; j++)
				{
					if ((db_kwh_para + j)->ter_no == rtu_no)
					{
						kwh_buffer_db.push_back(*(db_kwh_para + j));
						isend = true;

					}
					else if (isend)
					{
						break;

					}
				}

				rcd_act_num = kwh_buffer_db.size();
				if (rcd_act_num > 0)
				{

					for (int j = 0; j < rcd_act_num ; j++)
					{
						for (int k = rcd_act_num - 1; k > j; k--)
						{

							if (kwh_buffer_db[k].kwh_no < kwh_buffer_db[k-1].kwh_no)
							{
								tem_kwh_buffer = kwh_buffer_db[k];
								kwh_buffer_db[k] = kwh_buffer_db[k-1];
								kwh_buffer_db[k-1] = tem_kwh_buffer;
							}
						}

						tem_kwh.full_code = kwh_buffer_db[j].full_code;
						tem_kwh.slope = kwh_buffer_db[j].slope;
						kwh_buffer.push_back(tem_kwh);
					}

					//以当前终端设置个数为准，若实际KWH个数大于终端设置个数则不算多余的KWH
					if(rcd_act_num > current_kwh_num)
					{
						rcd_act_num = current_kwh_num;
					}
					memcpy(&chan_buffer[pos],&kwh_buffer[0],rcd_act_num * sizeof(KWH_PARA));
				}
				rtu_no = (m_rtu_para + rtu_no)->next_rtu;
				pos += current_kwh_num;
				kwh_buffer.clear();
				kwh_buffer_db.clear();
				if (rtu_no == chan_para->child_rtu)
				{
					break;
				}

			}

			m_fes_data_shm_allocator.set_kwh_para(i,&chan_buffer[0],chan_para->kwh_num * sizeof(KWH_PARA));
			chan_buffer.clear();
		}//for (int i = 0; i < MAX_CHAN_NUM; i++)
	}
	else if (rcd_cnt == 0)
	{
		ACE_DEBUG((LM_ERROR, "数据库中无电度参数记录！\n"));
	}
	else
	{
		ACE_DEBUG((LM_ERROR, "读取电度参数表参数失败！\n"));

	}
	return rcd_cnt > 0 ? 0 : -1;

}

/**************************************************************************
检查通道参数
**************************************************************************/
void Channel_chuser::check_channels(void)
{
	//	CHANPARA* 
	CHANPARA* m_shm_para = Fes_shm::get().chan_para_ptr;
	Fes_data_shm_allocator &m_fes_data_shm_allocator = Fes_data_shm_allocator::get();

	CHANDATA* m_shm_chandata = Fes_shm::get().chan_data_ptr;
	int channel_no;

	for (channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{

		CHANPARA* para = m_channel_para + channel_no;

		CHANPARA* shm_para = m_shm_para + channel_no;
// 		Base_protocol* protocol = m_protocol[channel_no];

		//与共享内存对象进行对比...
		if (memcmp(para,shm_para,sizeof(CHANPARA)) == 0)	//相同，pass
		{
			continue;
		}
		else			//如果不同，即发生了加载，先写入共享内存,并删除通道实例
		{
			ACE_OS::memcpy(shm_para,para,sizeof(CHANPARA));
			if (m_protocol[channel_no])
			{
				delete m_protocol[channel_no];
				m_protocol[channel_no] = NULL;
				m_fes_data_shm_allocator.recycle_channel_data(channel_no);
			}
		}

		int rtu_no = para->child_rtu;		//rtu号
		RTUPARA *rtu_para = NULL;		//本地rtupara内存
		RTUPARA *m_shm_rtu_para = Fes_shm::get().rtu_para_ptr;		//共享rtupara内存
		RTUPARA *shm_rtu_para = NULL;		//共享rtupara内存
		while(true)
		{
			rtu_para = m_rtu_para + rtu_no;
			shm_rtu_para = m_shm_rtu_para + rtu_no;
			memcpy(shm_rtu_para,rtu_para,sizeof(RTUPARA));
			rtu_no = rtu_para->next_rtu;
			if (rtu_no == para->child_rtu)		//如果是最后一个了，所有rtupara拷贝结束
			{
				break;
			}

		}

		//是否需要创建新对象
		if (para->valid == 0)
		{
			continue;
		}
		else
		{
			//写入CHANDATA初始值,设置默认值班节点
			int attend_host = m_shm_para[channel_no].default_attend_host;
			calc_hostno_enale(attend_host);//检查节点号是否合格
// 			m_shm_chandata[channel_no].curr_attend_host = attend_host;

			m_protocol[channel_no] = Protocol_factory::instance()->get_instance(para->protocol,channel_no);
			//如果实例可用
			if (m_protocol[channel_no] != NULL)
			{
				m_fes_data_shm_allocator.alloc_channel_data(channel_no);
				//todo 加载该通道的YX，YC，KWH参数
				m_bnew_protocol[channel_no] = true;
			}	
		}
	}	//end first loop

	read_yc_para_from_db();
	read_yx_para_from_db();
	read_kwh_para_from_db();

	for (channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)
	{
		CHANPARA* para = m_channel_para + channel_no;
		if (m_protocol[channel_no] && m_bnew_protocol[channel_no])
		{
			m_protocol[channel_no]->open();
			m_bnew_protocol[channel_no] = false;
		}
	}//end second loop
}
 
/**************************************************************************
从文件中读取通道参数
**************************************************************************/
int Channel_chuser::read_channel_para_from_file(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), CHANNEL_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "打开文件失败！\n"),-1);
	}

	ACE_OS::fread(m_channel_para, sizeof(CHANPARA), MAX_CHAN_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_DEBUG, "从文件读取通道参数成功！\n"));

	return 0; 
}

/**************************************************************************
从文件中读取终端参数
**************************************************************************/
int Channel_chuser::read_rtu_para_from_file(void)
{
	
	char path[LONGCHAR_LEN];

	ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), RTU_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "rb");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "打开文件失败！\n"), -1);
	}

	ACE_OS::fread(m_rtu_para, sizeof(RTUPARA), MAX_RTU_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "从文件读取终端参数成功！\n"));

	return 0;
}

/**************************************************************************
计算通道优先级
**************************************************************************/
void Channel_chuser::calc_channel_priority(void)
{
	
	vector<int> same_channel_pos;
	vector<int> exist_parent_channel;
	vector<int> total_cope_channel;
	int parent_channel = 0;
	bool is_exist = false;
	bool is_marked = false;		//是否已处理过
	int same_size = 0;
	int exist_size = 0;

	CHANDATA* chandata = Fes_shm::get().chan_data_ptr;
	GPS_INFO* gps_info = Fes_shm::get().gps_info_ptr;
	for (int i = 0; i < MAX_CHAN_NUM; i++)
	{
		(m_channel_para + i)->next_channel = -1;	//计算通道优先级之前,将其next_channel全部置-1，由计算后填写
		if ((m_channel_para + i)->initial_stat == 1)//初始化通道初始stat
		{
			chandata[i].stat = CHANRUN;
		}
		else
		{
			chandata[i].stat = CHANSTOP;	
		}

		//(m_channel_para+ i)->child_rtu = -1;//初始化通道的首个终端号,-1表示没有终端
		if ((m_channel_para + i)->valid == 0)
		{
			continue;

		}
//todo
// 		chandata[i].auto_switch = 1;
		if ((m_channel_para + i)->last_channel == -1)	//该通道为主通道
		{
			chandata[i].master_flag = 1;
		}

		//通道若为GPS通道，通道的处理方式 todo
// 		if ((m_channel_para + i)->gps == 1)
// 		{
// 			
// 		}

		/*判断该通道是否被处理*/
		is_marked = false;
		for (vector<int>::iterator j = total_cope_channel.begin(); j != total_cope_channel.end(); j++)
		{
			if (i == *j)
			{
				total_cope_channel.erase(j);
				is_marked = true;
				break;
			}
		}
		if (is_marked)
		{
			continue;
		}


		exist_size = exist_parent_channel.size();
		parent_channel = (m_channel_para + i)->last_channel;

		/*初始化优先级最高的通道，使其next_channel = -1,假定该通道没有备用通道*/
		if (parent_channel == -1)
		{
// 			(m_channel_para + i)->next_channel = -1;
			continue;
		}

		/*判断该通道是不是优先级最高的通道*/
		is_exist = false;
		for (int j = 0; j < exist_size; j++)
		{
			if (parent_channel == exist_parent_channel.at(j)) 
			{
				is_exist = true;
				break;

			}

		}
		if (is_exist)//不是优先级最高的通道
		{
			continue;
		}
		/*是优先级最高的通道，记录该通道*/
		exist_parent_channel.push_back(parent_channel);
        
		/*记录该通道的备用通道*/
		for (int j = i; j < MAX_CHAN_NUM; j++)
		{
			if ((m_channel_para + j)->valid == 0)
			{
				continue;
			}
			if ((m_channel_para + j)->last_channel == parent_channel)
			{
				same_channel_pos.push_back(j);
				total_cope_channel.push_back(j);
			}

		}

		/*对优先级最高的通道及其备用通道进行链表处理*/
		same_size = same_channel_pos.size();
		if (same_size == 1)
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = same_channel_pos.at(0);
			(m_channel_para + same_channel_pos.at(0))->last_channel = parent_channel;
			(m_channel_para + same_channel_pos.at(0))->next_channel = -1;

		}
		else if(same_size > 1)
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = same_channel_pos.at(0);
			(m_channel_para + same_channel_pos.at(0))->last_channel = parent_channel;
			(m_channel_para + same_channel_pos.at(0))->next_channel = same_channel_pos.at(1);
			for (int j = 1; j < same_size - 1; j++)
			{
				(m_channel_para + same_channel_pos.at(j))->last_channel = same_channel_pos.at(j - 1);
				(m_channel_para + same_channel_pos.at(j))->next_channel = same_channel_pos.at(j + 1);

			}

			(m_channel_para + same_channel_pos.at(same_size - 1))->last_channel = same_channel_pos.at(same_size - 2);
			(m_channel_para + same_channel_pos.at(same_size - 1))->next_channel = -1;

		}
		else
		{
			(m_channel_para + parent_channel)->last_channel = -1;
			(m_channel_para + parent_channel)->next_channel = -1;
		}
		
		same_channel_pos.clear();
	}//for (int i = 0; i < MAX_CHAN_NUM; i++)
}

/**************************************************************************
计算终端优先级
**************************************************************************/
void Channel_chuser::calc_rtu_priority( void )
{
	vector<int> same_rtu_pos;			//属于统一通道的所有终端的终端号
	vector<int> dealt_channel;			//已处理通道数组
	int parent_channel = 0;
	bool already_dealt = false;
	int same_size = 0;
	int exist_size = 0;
	for (int i = 0; i < MAX_RTU_NUM; i++)		//以rtu号作为排序,通道号最小的为某通道的第一个终端
	{
		if ((m_rtu_para + i)->valid == 0)
		{
			continue;
		}

		exist_size = dealt_channel.size();

		parent_channel = (m_rtu_para + i)->last_channel;
		already_dealt = false;
		for (int j = 0; j < exist_size; j++)
		{
			if (parent_channel == dealt_channel.at(j)) 
			{
				already_dealt = true;
				break;

			}

		}
		if (already_dealt)//已经处理过该rtu的关系，跳过
		{
			continue;
		}

        /*未处理该rtu，该rtu应为其通道的第一个终端，将该通道加入已处理队列*/

		dealt_channel.push_back(parent_channel);
		/*记录属于该通道的所有终端*/
		for (int j = i; j < MAX_RTU_NUM; j++)
		{
			if ((m_rtu_para + j)->valid == 0)
			{
				continue;
			}
			if ((m_rtu_para + j)->last_channel == parent_channel)
			{
				same_rtu_pos.push_back(j);
			}


		}

		if (same_rtu_pos.empty())
		{
			continue;

		}

		same_size = same_rtu_pos.size();
		/*将终端连成链表*/
		for (int j = 0; j < same_size - 1; j++)
		{

			(m_rtu_para + same_rtu_pos.at(j))->next_rtu = same_rtu_pos.at(j + 1);

		}

		(m_rtu_para + same_rtu_pos.at(same_size - 1))->next_rtu = same_rtu_pos.at(0);
		same_rtu_pos.clear();
	}//for (int i = 0; i < MAX_RTU_NUM; i++)
}

/**************************************************************************
计算终端通道关系
**************************************************************************/
void Channel_chuser::calc_rtu_channel(void)
{
	
	vector<int> exist_parent_channel;
	int current_channel = 0;
	bool is_exist = false;
	int exist_size = 0;
	CHANPARA* chanpara = NULL;

	for (int i = 0; i < MAX_RTU_NUM; i++)
	{
		if ((m_rtu_para + i)->valid == 0)
		{
			continue;
		}

        /*判断属于某个通道的第一个终端号*/
		exist_size = exist_parent_channel.size();
		current_channel = (m_rtu_para + i)->last_channel;
		is_exist = false;
		for (vector<int>::iterator j = exist_parent_channel.begin(); j != exist_parent_channel.end(); j++)
		{
			if (current_channel == *j) 
			{
				is_exist = true;
				break;
			}

		}
		/*不是第一个终端号*/
		if (is_exist)
		{
			continue;
		}

		(m_channel_para + current_channel)->child_rtu = i;//将该通道的child_rtu赋值为第一个终端号

		chanpara = m_channel_para + current_channel; 
		while(true)	//有备通道将该通道的子通道rtu关系填写好
		{
			chanpara->child_rtu = i;
			if (chanpara->next_channel == -1)
			{
				break;
			}
			chanpara = m_channel_para + chanpara->next_channel;
		}

	exist_parent_channel.push_back(current_channel);
	}//for (int i = 0; i < MAX_RTU_NUM; i++)
	int total_yc = 0;
	int total_yx = 0;
    int total_kwh = 0;
	RTUPARA *tem_rtu_para = NULL;
	/*计算通道总的遥信，遥测，电度量*/
	for (int i = 0; i < MAX_CHAN_NUM; i++)
	{
		if ((m_channel_para + i)->valid == 0)
		{
			continue;
		}
		tem_rtu_para = m_rtu_para + (m_channel_para + i)->child_rtu;
		total_yc = 0;
		total_yx = 0;
		total_kwh = 0;

		while (true)
		{
			total_yc += tem_rtu_para->yc_num;
			total_yx += tem_rtu_para->yx_num;
			total_kwh += tem_rtu_para->kwh_num;
			if(tem_rtu_para->next_rtu == (m_channel_para + i)->child_rtu)
			{
				break;
			}
			tem_rtu_para = m_rtu_para + tem_rtu_para->next_rtu;

		}//while (true)
		(m_channel_para + i)->yc_num = total_yc;
		(m_channel_para + i)->yx_num = total_yx;
		(m_channel_para + i)->kwh_num = total_kwh;

	}//for (int i = 0; i < MAX_CHAN_NUM; i++)


}

/**************************************************************************
保存通道参数
**************************************************************************/
int Channel_chuser::save_channel_para(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), CHANNEL_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "打开文件失败！\n"), -1);
	}

	ACE_OS::fwrite(m_channel_para, sizeof(CHANPARA), MAX_CHAN_NUM, fp);
	ACE_OS::fclose(fp);
		
	ACE_DEBUG((LM_ERROR, "保存通道参数到本地文件成功！\n"));
	return 0; 
}

/**************************************************************************
保存终端参数
**************************************************************************/
int Channel_chuser::save_rtu_para(void)
{
	char path[LONGCHAR_LEN];
    ACE_OS::sprintf(path, "%s/%s", ACE_OS::getenv(DEFAULT_ENV), RTU_FILE_NAME);
	FILE* fp = ACE_OS::fopen(path, "w+b");
	if (fp == NULL)
	{
		ACE_ERROR_RETURN((LM_ERROR, "打开文件失败！\n"), -1);
	}

	ACE_OS::fwrite(m_rtu_para, sizeof(RTUPARA), MAX_RTU_NUM, fp);
	ACE_OS::fclose(fp);

	ACE_DEBUG((LM_ERROR, "保存终端参数到本地文件成功！\n"));
	return 0; 
}

/**************************************************************************
设置GPS参数
**************************************************************************/
void Channel_chuser::set_gps_para(void)
{
	//todo

}

/**************************************************************************
设置默认的通道参数
**************************************************************************/
void Channel_chuser::clear_para(void)
{
	ACE_OS::memset(m_channel_para, 0, sizeof(CHANPARA) * MAX_CHAN_NUM);
	for(int channel_no = 0; channel_no < MAX_CHAN_NUM; channel_no++)	//完成结构体定义初始化要求
	{
		m_channel_para[channel_no].default_attend_host = -1;
	}
	ACE_OS::memset(m_rtu_para, 0, sizeof(RTUPARA) * MAX_RTU_NUM);
}



void Channel_chuser::calc_hostno_enale(int& hostno)
{
	int ncnt;
	int nodes[MAX_FE_SRV_NUM];
	int node_num = Net_stat::get_fe_nodes(nodes, MAX_FE_SRV_NUM);
	int min_node = nodes[0];
	for( ncnt = 0; ncnt<node_num; ncnt++)
	{

		if (hostno == nodes[ncnt])
		{
			break;
		}
		else if (nodes[ncnt] < min_node)		//比较最小序号
		{
			min_node = nodes[ncnt];
		}
	}

	if (ncnt == node_num)		//不是前置组节点,由序号最小的前置接管
	{
		hostno = min_node;
	}
}