/**
@file Mirror_Proc.cpp
@brief III区实时数据处理线程源文件

III区实时数据处理

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#include "Mirror_Proc.h"
#include "fes/CMyEFile.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <ace/OS.h>

Mirror_Proc::Mirror_Proc( ) : QThread()
{
	m_data = new FE_REAL_DATA_NODE[MAX_FILE_DATA_NUM];
	m_soe = new FE_DATA_EVENT_NODE[MAX_FILE_DATA_NUM];
	m_channel = new FE_CHANNEL_STAT_MAIL[MAX_FILE_DATA_NUM];
	m_ykcheck = new FE_YK_CHECK_NODE[MAX_FILE_DATA_NUM];


	m_data_mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (MAX_FILE_DATA_NUM * sizeof(FE_REAL_DATA_NODE)) );		//定义为最大长度

	m_stat_mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (MAX_FILE_DATA_NUM * sizeof(FE_CHANNEL_STAT_MAIL)) );		//定义为最大长度
	
	m_last_modify_time = QDateTime::currentDateTime();
}

Mirror_Proc::~Mirror_Proc()
{
	delete m_data;
	delete m_soe;
	delete m_channel;
	delete m_ykcheck;
	free(m_data_mail_struct);
	free(m_stat_mail_struct);
}


void Mirror_Proc::run()
{
	QStringList file_list;
	while(true)		//线程循环体
	{
		if (scan_datafile(file_list))	//有数据文件
		{
			for (int filenum = file_list.size(); filenum > 0; filenum--)
			{

				convert_file_to_list(file_list[filenum-1]);	
			}
		}
		QThread::sleep(1);
	}//线程循环体
}

bool Mirror_Proc::scan_datafile(QStringList& file_list)
{
	char dir_path[512];
	sprintf(dir_path,"%s/log/dat/IIIFEP/",getenv("NBENV"));
	
	QDir data_file_dir(g_Q(dir_path));

	file_list.clear();
	file_list = data_file_dir.entryList(QDir::Files, QDir::Time);
	
	if (file_list.isEmpty())
	{
		return false;
	}
	else
		return true;
}

void Mirror_Proc::convert_file_to_list(QString filename)
{
	char file_path[512];
	sprintf(file_path,"%s/log/dat/IIIFEP/%s",getenv("NBENV"),_Q2C(filename));
	QFile file(file_path);
	QFileInfo file_info( file );

	if ((file_info.lastModified()) < m_last_modify_time)	//时间早于上次文件时间 不处理
	{
		file.remove();
		return;
	}
	else //记录该文件的生成时间
	{
		m_last_modify_time = file_info.lastModified();
	}

	if (!Net_stat::get_host_master_flag(Net_stat::get_host_no()))		//主前置标志,由主前置发送数据邮件
	{
		QStringList list1 = filename.split("_");
		if (list1[0] == "raw")	//数据类型处理
		{
			EFILE_DATA* tmp_data = new EFILE_DATA(1);
			tmp_data->set_file_name(file_path);
			memset(m_data,0,sizeof(FE_REAL_DATA_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_data);
			Proc_manager::putdbg(201,"raw num = %d\n",num);
			send_data_to_scada(num);
		}
		else if(list1[0] == "soe")//soe类型处理
		{
			EFILE_SOE* tmp_data = new EFILE_SOE(1);
			tmp_data->set_file_name(file_path);
			memset(m_soe,0,sizeof(FE_DATA_EVENT_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_soe);
			send_soe_to_scada(num);

		}
		else if ((list1[0] == "channel"))//通道状态处理
		{
			EFILE_CHANNEL* tmp_data = new EFILE_CHANNEL(1);
			tmp_data->set_file_name(file_path);
			memset(m_channel,0,sizeof(FE_CHANNEL_STAT_MAIL)*MAX_FILE_DATA_NUM);
			int num  = tmp_data->read_data_cross_table(m_channel);
			send_channel_to_scada(num);
		}
		else if ((list1[0] == "channelevent"))//通道事项处理
		{
			float channel_stat;
			char desdribe[64];
			char namestr[128];

			EFILE_CHANNEL* tmp_data = new EFILE_CHANNEL(1);
			tmp_data->set_file_name(file_path);
			tmp_data->read_event_cross_table(channel_stat, desdribe, namestr);
			
			printf("%f %s %s \n", channel_stat, desdribe, namestr);

			//生成通道事项
			Fe_event::make_channel_event(channel_stat,"", desdribe, namestr);
		}
		if (list1[0] == "ykcheck")	//遥控返校处理
		{
			EFILE_YKCHHECK* tmp_data = new EFILE_YKCHHECK(1);
			tmp_data->set_file_name(file_path);
			memset(m_ykcheck,0,sizeof(FE_YK_CHECK_NODE)*MAX_FILE_DATA_NUM);
			int num = tmp_data->read_data_cross_table(m_ykcheck);
			send_ykcheck_to_scada(num);
		}

	}

 	file.remove();
}

void Mirror_Proc::send_data_to_scada( int data_num )
{
	if (data_num < 1)
	{
		return;
	}

	int datalen = data_num * sizeof(FE_REAL_DATA_NODE);		//数据区长度

	ACE_OS::memset(m_data_mail_struct, 0, m_data_mail_struct->get_lenth()+datalen);

	m_data_mail_struct->mail_type = REAL_DATA;
	m_data_mail_struct->mail_lenth = m_data_mail_struct->get_lenth() + datalen;		//邮件总长度
	m_data_mail_struct->sub_information1 = data_num;

	ACE_OS::memcpy(m_data_mail_struct->mail_body, (unsigned char*)m_data, datalen);

// 	FE_REAL_DATA_NODE* real_data_src = get_head_raw();
// 	FE_REAL_DATA_NODE* real_data_dest = (FE_REAL_DATA_NODE*)mail_struct->mail_body;

	int ret = Fe_mail::send_to_scada( (void*)m_data_mail_struct, m_data_mail_struct->mail_lenth, 2 );		//返回-1失败
	
	if(ret)
		printf("send raw faile\n");
}

void Mirror_Proc::send_soe_to_scada( int soe_num )
{
	if (soe_num < 1)
	{
		return;
	}
	int ret =Fe_scasrv::fe_send_electric_event(m_soe, soe_num);
	if(ret)
		printf("send soe faile\n");
}

void Mirror_Proc::send_channel_to_scada( int channel_num )
{
	if (channel_num < 1)
	{
		return;
	}
	int datalen = channel_num * sizeof(FE_CHANNEL_STAT_MAIL);		//数据区长度

	ACE_OS::memset(m_stat_mail_struct, 0, m_stat_mail_struct->get_lenth()+datalen);

	m_stat_mail_struct->mail_type = CHANNEL_STAT;
	m_stat_mail_struct->mail_lenth = m_stat_mail_struct->get_lenth() + datalen;		//邮件总长度
	m_stat_mail_struct->sub_information1 = channel_num;

	ACE_OS::memcpy(m_stat_mail_struct->mail_body, (unsigned char*)m_channel, datalen);

	int ret = Fe_mail::send_to_scada(m_stat_mail_struct, m_stat_mail_struct->mail_lenth, 2);

	if(ret)
		printf("send channel faile\n");

}


void Mirror_Proc::send_ykcheck_to_scada( int ykcheck_num )
{
	if (ykcheck_num < 1)
	{
		return;
	}
	for (int i=0;i<ykcheck_num;i++)
	{
		int ret = Fe_scasrv::fe_send_ykret(m_ykcheck->rtu_no,m_ykcheck->yk_no,m_ykcheck->yk_ret,m_ykcheck->msg );
		m_ykcheck++;
	}

// 	int ret =Fe_scasrv::fe_send_ykret(m_ykcheck, ykcheck_num);
// 	if(ret)
// 		printf("send ykcheck faile\n");
}
