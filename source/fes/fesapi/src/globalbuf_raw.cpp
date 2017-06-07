/**
@file globalbuf.cpp
@brief 前置全局数据缓冲区类的实现

@author hongxiang 
@version 1.0.0
@date 2013-10-28
*/
#include "fes/globalbuf_raw.h"
#include "fes/CMyEFile.h"

#include <ace/OS.h>
#include <ace/UUID.h>

using namespace std;



Global_buf_Raw::Global_buf_Raw()
:m_raw_list(Array_list<FE_REAL_DATA_NODE>(ARRAY_RAW_NUM, RAW_LIST_SIZE))
{

}

/**************************************************************************
启动
**************************************************************************/
int Global_buf_Raw::open(void *args)
{
	activate();
	return 0;
}

/**************************************************************************
停止
**************************************************************************/
int Global_buf_Raw::close (u_long flags)
{
	return 0;
}

/**************************************************************************
数据发送线程体
**************************************************************************/
int Global_buf_Raw::svc(void)
{
	static int timecount = 0;
	FE_MAIL_STRUCT* mail_struct;

	mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (ARRAY_RAW_NUM * sizeof(FE_REAL_DATA_NODE)) );		//定义为最大长度

	while(true)		//线程循环体
	{
		lock_raw_list();		//申请锁
		if (!m_raw_list.empty())	//有数据需要发送
		{
			while(m_raw_list.get_head_array() != NULL)
			{
				int datalen = get_head_raw_num()* sizeof(FE_REAL_DATA_NODE);		//数据区长度

				ACE_OS::memset(mail_struct, 0, mail_struct->get_lenth()+datalen);

				mail_struct->mail_type = REAL_DATA;
				mail_struct->mail_lenth = mail_struct->get_lenth() + datalen;		//邮件总长度
				mail_struct->sub_information1 = get_head_raw_num();

				ACE_OS::memcpy(mail_struct->mail_body, (unsigned char*)get_head_raw(), datalen);

				FE_REAL_DATA_NODE* real_data_src = get_head_raw();
				FE_REAL_DATA_NODE* real_data_dest = (FE_REAL_DATA_NODE*)mail_struct->mail_body;

				//todo III区前置存文件
				if(Net_stat::get_PUBFEP_flag())
				{
					if (get_head_raw_num() >= 1000 || timecount >250)	//5S或1000条生数据
					{
						timecount = 0;
						EFILE_DATA *event_list = new EFILE_DATA( mail_struct->sub_information1 );
						Proc_manager::putdbg(201,"raw num = %d\n",mail_struct->sub_information1);

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
							ACE_OS::sprintf(file_path,"%s/log/dat/IIIFEP/%s/raw_%s.txt",getenv("NBENV"),hosts[j],uuid.to_string ()->c_str ());

							event_list->set_file_name(file_path);

							event_list->save_data_cross_table( real_data_dest );

						}
						m_raw_list.clear_head();

						for( int k = 0; k < MAX_FE_SRV_NUM; k ++ )
						{
							delete [] hosts[ k ];
						}
						delete [] hosts;
						hosts = NULL;

					}
					else
						break;
						
				}
				else
				{
					int ret = Fe_mail::send_to_scada( (void*)mail_struct, mail_struct->mail_lenth, 2 );		//返回-1失败
// 					if (!ret)
// 					{
// 						ACE_DEBUG((LM_DEBUG,"send raw event mail\n"));
// 					}
					m_raw_list.clear_head();
				}
				

// 				static int rawevent = 0;
// 				if (ret == 0)
// 				{
// 					rawevent++;
// 					if (rawevent % 100 == 0)
// 					{
// 						ACE_DEBUG((LM_DEBUG,"send mail = %d!\n",rawevent));
// 					}
// 				}
				//清空发送过的报文体数组，需申请锁操作

			}
		}
		unlock_raw_list();		//释放锁
		timecount++;
		ACE_OS::sleep(ACE_Time_Value(0,20*1000));	//无数据，sleep

// 		ACE_DEBUG((LM_DEBUG, "Global_buf_Raw time = %f\n", t.elapsed()));
	}//线程循环体

	free(mail_struct);

	return 0;
}
