#include "fes/fesdev.h"

Udp_dev::Udp_dev(const char* local_dev, const int& local_port, 
				 const char* dev1, const int& port1, 
				 const char* dev2, const int& port2)
:Net_dev(dev1,port1,dev2,port2)
{
	unsigned long ip_addr;

	if (local_dev && (ip_addr = ACE_OS::inet_addr(local_dev)) != 
#ifdef WIN32
		INADDR_NONE
#else 
		((in_addr_t) - 1)
#endif
		)
	{
		local_addr.set(local_port, local_dev, 1, AF_INET);
	}
}


int Udp_dev::open_dev(void)
{
	if (0 == valid_addr_num)
	{
		printf("there are no valid address.\n");
		return -1;
	}

	addr_index = (addr_index + 1) % valid_addr_num;

	if(-1 == peer.open(local_addr))
	{
		ACE_ERROR((LM_ERROR,"%p\n","Error in open"));
		if(-1 == peer.close())
		{
			ACE_ERROR((LM_ERROR,"%p\n","Error in close"));
			return -1;
		}
		return -1;

	}

	//设置发送接收缓冲区
	int temp = DEVICE_TX_BUF_SIZE;
	peer.set_option(SOL_SOCKET, SO_RCVBUF, (char *)&temp, sizeof(temp));
	peer.set_option(SOL_SOCKET, SO_SNDBUF, (char *)&temp, sizeof(temp));

	peer.enable(ACE_NONBLOCK);

	ACE_DEBUG((LM_INFO,ACE_TEXT("创建套接字成功！\n")));


	return 0;
}

void Udp_dev::close_dev()
{
	if(-1 == peer.close())
	{
		ACE_ERROR((LM_ERROR, "%p\n", "Error in close"));
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("关闭套接字成功！\n")));

}

int Udp_dev::write_dev(unsigned char* buf, int len)
{
	int send_count = 0;
	if((send_count = peer.send(buf,len,addr[addr_index])) == -1)
	{
		ACE_ERROR((LM_ERROR, "%p\n", "Error in Send"));
		return -1;
	}

	ACE_DEBUG((LM_INFO, ACE_TEXT("成功发送%d个字节！\n"), send_count));
	return send_count;

}

int Udp_dev::read_dev(unsigned char* buf, int len,unsigned int nTimeOut)
{
	int recv_count = 0;

	if((recv_count = peer.recv(buf, len, addr[addr_index])) == -1)
	{
		if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			recv_count = 0;
		}
		else
		{
			ACE_ERROR((LM_ERROR, "%p\n", "Error in Recv"));
			return -1;
		}
	}
//	ACE_DEBUG((LM_INFO, ACE_TEXT("成功接受%d个字节！\n"), recv_count));
	return recv_count;

}

//add by chenkai 获取通道状态
int Udp_dev::get_stat(void)
{
	return peer.get_handle() >= 0 ? 0 : -1;
}