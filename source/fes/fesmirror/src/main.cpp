#include <QApplication>

#include "Mirror_Proc.h"
#include "fes/platformwrapper.h"
#include <ace/OS.h>

int main(int argc, char* argv[])
{

	Mirror_Proc mirror_proc;

	if (!Proc_manager::proc_exist("fesmirror"))
	{
		ACE_OS::printf("通知网络程序运行 exit\n");
		exit(0);
	}

	Proc_manager::start_proc("fesmirror", "三区数据镜像进程","supernb");
	mirror_proc.start();

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