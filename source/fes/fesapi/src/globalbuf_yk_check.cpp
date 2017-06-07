/**
@file globalbuf_yk_check.cpp
@brief 前置遥控返校数据缓冲区类的实现

@author lujiashun
@version 1.0.0
@date 2016-03-02
*/
#include "fes/globalbuf_yk_check.h"
#include "fes/CMyEFile.h"

#include <ace/OS.h>
#include <ace/UUID.h>

using namespace std;



Global_buf_yk_check::Global_buf_yk_check()
:m_ykcheck_list(Array_list<FE_YK_CHECK_NODE>(ARRAY_RAW_NUM, RAW_LIST_SIZE))
{

}

/**************************************************************************
启动
**************************************************************************/
int Global_buf_yk_check::open(void *args)
{
	activate();
	return 0;
}

/**************************************************************************
停止
**************************************************************************/
int Global_buf_yk_check::close (u_long flags)
{
	return 0;
}

/**************************************************************************
数据发送线程体
**************************************************************************/
int Global_buf_yk_check::svc(void)
{
	static int timecount = 0;
// 	FE_MAIL_STRUCT* mail_struct;
// 
// 	mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + (ARRAY_RAW_NUM * sizeof(FE_YK_CHECK_NODE)) );		//定义为最大长度

	while(true)		//线程循环体
	{
		lock_ykcheck_list();		//申请锁
		if (!m_ykcheck_list.empty())	//有数据需要发送
		{
			while(m_ykcheck_list.get_head_array() != NULL)
			{
// 				int datalen = get_head_ykcheck_num()* sizeof(FE_YK_CHECK_NODE);		//数据区长度
// 
// 				ACE_OS::memset(mail_struct, 0, mail_struct->get_lenth()+datalen);

// 				mail_struct->mail_type = YK;
// 				mail_struct->mail_lenth = mail_struct->get_lenth() + datalen;		//邮件总长度
// 				mail_struct->sub_information1 = get_head_ykcheck_num();
// 
// 				ACE_OS::memcpy(mail_struct->mail_body, (unsigned char*)get_head_ykcheck(), datalen);
// 
// 				FE_YK_CHECK_NODE* real_data_src = get_head_ykcheck();
// 				FE_YK_CHECK_NODE* real_data_dest = (FE_YK_CHECK_NODE*)mail_struct->mail_body;
				
				int yk_check_num = get_head_ykcheck_num();		//遥控返校数据数量
				FE_YK_CHECK_NODE* yk_check_data = get_head_ykcheck();	//遥控返校数据

				//todo III区前置存文件
				if(Net_stat::get_PUBFEP_flag())
				{
					if (get_head_ykcheck_num() >= 1000 || timecount >100)	//2S(遥控返校的时效性要求，可以更短？)或1000条生数据
					{
						timecount = 0;
						EFILE_YKCHHECK *ykcheck_list = new EFILE_YKCHHECK( yk_check_num );
						Proc_manager::putdbg(201,"yk_check num = %d\n",yk_check_num);

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
							ACE_OS::sprintf(file_path,"%s/log/dat/IIIFEP/%s/ykcheck_%s.txt",getenv("NBENV"),hosts[j],uuid.to_string ()->c_str ());

							ykcheck_list->set_file_name(file_path);

							ykcheck_list->save_data_cross_table( yk_check_data );

						}
						m_ykcheck_list.clear_head();

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
					for (int i=0;i<yk_check_num;i++)
					{
						int ret = Fe_scasrv::fe_send_ykret(yk_check_data->rtu_no,yk_check_data->yk_no,yk_check_data->yk_ret,yk_check_data->msg );
						yk_check_data++;
					}
// 					int ret = Fe_mail::send_to_scada( (void*)mail_struct, mail_struct->mail_lenth, 2 );		//返回-1失败
// 					if (!ret)
// 					{
// 						ACE_DEBUG((LM_DEBUG,"send raw event mail\n"));
// 					}
					m_ykcheck_list.clear_head();
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
		unlock_ykcheck_list();		//释放锁
		timecount++;
		ACE_OS::sleep(ACE_Time_Value(0,20*1000));	//无数据，sleep

// 		ACE_DEBUG((LM_DEBUG, "Global_buf_Raw time = %f\n", t.elapsed()));
	}//线程循环体

// 	free(mail_struct);

	return 0;
}
