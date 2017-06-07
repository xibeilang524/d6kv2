#include "fes/msgserviceacceptor.h"
#include "fes/msgmanager.h"
#include "fes/msgservice.h"
#include "fes/platformwrapper.h"

#include <ace/OS.h>

int main(int argc, char *argv[])
{
	//Version

	//todo
	if (!Proc_manager::proc_exist("msgengine"))
	{
		ACE_OS::printf("通知网络程序运行 exit\n");
		exit(0);
	}

	Proc_manager::start_proc("msgengine", "报文转发进程","supernb");

	//初始化建立两个主要线程
	Msg_service_acceptor* msg_acceptor = Msg_service_acceptor::get();
	Msg_manage* msg_mangage = Msg_manage::get();

	msg_acceptor->open(0);
	msg_mangage->open(0);

	while(true)
	{
		if (!Proc_manager::check_parent())
		{
			ACE_OS::sleep(2);
		}
		else
		{
			break;
		}
	}

//  	ACE_Thread_Manager::instance()->wait();

	Proc_manager::end_proc();

	return 0;
}