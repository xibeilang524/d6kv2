#include "fes/protocolfactory.h"
#include "fes/statchecker.h"

#include "fes/portwatch.h"
#include "fes/fesshm.h"
#include "fes/fesdatashm_allocator.h"
#include "fes/channelchuser.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"

#include "fes/globalbuf_evevnt.h"
#include "fes/globalbuf_raw.h"
#include "fes/globalbuf_yk_check.h"
#include "fes/timer.h"

class ACE_Time_Value;
class ACE_Thread_Mutex;
//class Femail;

class CheckProcTask: public ACE_Task<ACE_MT_SYNCH>
{
public:
	CheckProcTask(){};
	~CheckProcTask(){};

	int open(void *)
	{
		ACE_DEBUG((LM_DEBUG,"(%t) CheckProcTask open !\n")); 
		//Activate the object with a thread in it. 
		activate(); 
		return 0; 
	};
	int svc(void)
	{
		Proc_manager::start_proc("fessvc", "新前置通讯进程","supernb");
		while (true)
		{	
			if ( Proc_manager::check_parent() < 0 )		//2S启动一次checkparent
			{
				Proc_manager::end_proc();
				break;
			}
			ACE_OS::sleep(ACE_Time_Value(2,0));
		}
		exit(0);
	};
};

int main(int argc, char **argv)
{
	ACE_Thread_Mutex ace_t_m;
	Timer check_parent_timer;

	//判断是否启动了nbsvc
	int a = Net_stat::get_host_no();
	int n=Net_stat::get_host_stat(a);

	if(a<0)
	{
	
		ACE_ERROR((LM_ERROR, "节点没有运行nbsvc\n"));
		exit(0);
	}
	if (!Proc_manager::proc_exist("fessvc"))
	{
		ACE_ERROR((LM_ERROR, "通知网络程序运行 exit\n"));
		exit(0);
	}

	/*Proc_manager::start_proc("fessvc", "新前置通讯进程","supernb");*/

// 	Fes_shm& fes_share_memory = Fes_shm::get();		//创建共享内存
// 	Fes_data_shm_allocator &fes_data_shm_allocator = Fes_data_shm_allocator::get();	//生数据共享内存
	
	/*Proc_manager::check_parent();*/
	if ( Db_connector::open( ) < 0)
	{
		ACE_ERROR((LM_ERROR, "数据库打开失败\n"));
	}
	/*Proc_manager::check_parent();*/

	Port_watch tem_port_watch(ace_t_m);		//端口值班
	Stat_checker tem_stat_checker(ace_t_m);		//通道检查

	Channel_chuser ch_ch(ace_t_m);			//前置参数加载，及规约实例调用
	Global_buf_Event::get().open();		//电力事项发送进程
	Global_buf_Raw::get().open();		//实时数据发送进程
	Global_buf_yk_check::get().open();		//遥控返现信息发送进程
	Proc_manager::putdbg(MAINDBG, "启动规约线程");
	ch_ch.load();						//参数加载，创建规约实例

	printf("fessvc ch_ch.load ok!\n");
	fflush(stdout);

	Proc_manager::putdbg(MAINDBG, "启动端口值班线程");
	tem_port_watch.open();		//端口值班线程启动
	Proc_manager::putdbg(MAINDBG, "启动状态值班线程");
	tem_stat_checker.open();	//状态检查线程启动
	Db_connector::close( );


	CheckProcTask checktask;
	checktask.open(0);


//邮件接收，分发处理
	FE_MAIL_STRUCT *buff = NULL;
	int recv_len = 0;
	ACE_Message_Block *msg_stu = 0;
	while (true)
	{	
// 		if (check_parent_timer.elapsed() > 2)		//2S启动一次checkparent
// 		{
// 			int check_ret = Proc_manager::check_parent();
// 			if ( !check_ret )
// 			{
// 				check_parent_timer.restart();
// 			}
// 			else
// 			{
// 				break;
// 			}
// 		}

		int ret = Fe_mail::receive((void **)&buff,&recv_len);
		if (ret == -1) //无邮件
		{
			ACE_OS::sleep(ACE_Time_Value(0,10*1000));
		}
		else
		{
			switch (buff->mail_type)
			{
			case YK:		//遥控、召唤邮件统一处理
			case CALLING:
			case SET_POINT:
			case PROT:
				{
					ACE_DEBUG((LM_DEBUG, "前置收到遥控命令\n"));

					ch_ch.put_mail(buff, recv_len);		//调用
				}
				break;
			case PORT_WATCH:
				{
					msg_stu = new ACE_Message_Block(recv_len);
					msg_stu->copy((char*)buff,recv_len);
 //					ACE_OS::memcpy(msg_stu->wr_ptr(), buff, recv_len);
					tem_port_watch.putq(msg_stu);
				}
				break;
			case LOAD_PARA:		//参数加载邮件，重新进行加载过程
				{
					Proc_manager::putdbg(MAINDBG, "收到参数加载邮件");
					if ( !Db_connector::open( ) )
					{
						ch_ch.load();
					}
					Db_connector::close();
				}
				break;
			case CHANNEL_STAT:	//暂无通道状态邮件
			case REAL_DATA:		//暂无实时数据邮件
			case INVALID:
			default:
				break;
			}
		}
	}

	Proc_manager::end_proc();

	return 0;
}