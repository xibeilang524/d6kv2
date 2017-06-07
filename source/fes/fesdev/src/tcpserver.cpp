#include "fes/fesdev.h"




Tcp_server::Tcp_server(const char* local_ip, const int& local_port, 
					   const char* peer_ip1 /* = NULL */, 
					   const char* peer_ip2 /* = NULL */)
:Tcp_client(peer_ip1, 0, peer_ip2, 0)
{
	unsigned long ip_addr;

	if (local_ip && (ip_addr = ACE_OS::inet_addr(local_ip)) != 
#ifdef WIN32
		INADDR_NONE
#else 
		((in_addr_t) - 1)
#endif
		)
	{
		local_addr.set(local_port, local_ip, 1, AF_INET);
	}
	else if(local_ip == NULL)
	{
		local_addr.set(local_port);

	}

	listen_flag = false ;
// 	listen_port();
}



int Tcp_server::listen_port(void)
{
	if (acceptor.open(local_addr,1) == -1)
	{
		listen_flag = false ;
		ACE_ERROR_RETURN((LM_ERROR,  ACE_TEXT("listen failed")), -1);
		return -1;
	}
	listen_flag = true ;
	ACE_DEBUG((LM_INFO, ACE_TEXT("创建套接字成功！\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("绑定成功！\n")));
	ACE_DEBUG((LM_INFO, ACE_TEXT("侦听成功！\n")));
	return 0;
}



int Tcp_server::open_dev(void)
{
	if ( !listen_flag )
	{
		if (acceptor.open(local_addr,1) == -1)
		{
			listen_flag = false ;
			ACE_ERROR_RETURN((LM_ERROR,  ACE_TEXT("listen failed")), -1);
			return -1;
		}
		else
		{
			listen_flag = true ;
		}
	}
	ACE_INET_Addr client_addr;
	//accept等待时间10s ，避免程序死在这里，外层循环无法继续
	ACE_Time_Value acceptor_time(10);
	if (acceptor.accept(peer, &client_addr,&acceptor_time) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR,  ACE_TEXT("Failed to accept")), -1);
		acceptor.close();
		return -1;
	}
	bool isvalidaddr = false;
	int i = 0;
	int numzero = 0;

	for (i = 0; i < valid_addr_num; i++)
	{
		if(addr[i].is_any())
		{
			numzero++;
		}

		else if (client_addr.get_ip_address() == addr[i].get_ip_address())
		{
			isvalidaddr = true;
			break;
		}
	}
	if (numzero == valid_addr_num)
	{
		addr_index = 0;
// 		addr[addr_index].set(client_addr);
	}
	else if (isvalidaddr)
	{
		addr_index = i;
	}
	else
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("收到一个连接，但不在允许访问的IP地址列表内！")), -1);
// 		return -1;
	}

	ACE_DEBUG((LM_INFO, ACE_TEXT("收到一个连接，对方ip地址为：%s\n"), client_addr.get_host_addr()));

	set_fd_opt() ;
	return 0;

}

Tcp_server::~Tcp_server()
{
	acceptor.close();
	close_dev();
}

int Tcp_server::close_listen(void)
{
	acceptor.close();
	listen_flag = false;

	return true;
}