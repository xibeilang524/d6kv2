/**
@file msgtransceiver.cpp
@brief 报文接收发送类的实现

@author hongxiang 
@version 1.0.0
@date 2013-11-1
*/
#include "fes/msgtransceiver.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

#if defined (__unix)
/*#include "linux/tcp.h"*/
#include "netinet/tcp.h"
#endif

using namespace std;

//同步头
unsigned char SYNC_FRAME_VAL[6]={0xEB,0x90,0xEB,0x90,0xEB,0x90};


/**************************************************************************
构造函数
**************************************************************************/
Msg_transceiver::Msg_transceiver()
:m_msg_list(Array_list<FE_MSG>(4096, 4))
{
	m_addr.set(MSG_MNG_PORT,"127.0.0.1",1,AF_INET);
	open();
}

/**************************************************************************
析构函数
**************************************************************************/
Msg_transceiver::~Msg_transceiver()
{

}

/**************************************************************************
启动
**************************************************************************/
int Msg_transceiver::open(void *args)
{
	activate();
	ACE_DEBUG((LM_DEBUG, "报文收发线程启动！\n"));
	return 0;
}

/**************************************************************************
停止
**************************************************************************/
int Msg_transceiver::close(u_long flags)
{
	m_msg_list.~Array_list();
	close_dev();
	ACE_DEBUG((LM_DEBUG, "报文收发线程退出！\n"));
	return 0;
}

/**************************************************************************
线程体
**************************************************************************/
int Msg_transceiver::svc(void)
{
	while(true)
	{
		open_dev();
		while(is_open())
		{
// 			ACE_Guard<ACE_Thread_Mutex> guard (m_msg_mutex);
			int ret = send_msg();
			if (ret<0)
			{
				ACE_DEBUG((LM_DEBUG, "报文发送线程发送出错！\n"));
				break;
			}
			ACE_OS::sleep(SEND_MSG_TIME_SPAN);
		}
		ACE_OS::sleep(SEND_MSG_TIME_SPAN);
	}
	return 0;
}

/**************************************************************************
打开通道端口
**************************************************************************/
void Msg_transceiver::open_dev(void)
{
	if(m_peer.open(SOCK_STREAM,AF_INET,IPPROTO_TCP,0) == -1)
	{
		ACE_ERROR((LM_ERROR,"%p\n","Error in open"));

		if(m_peer.close() == -1)
		{
			ACE_ERROR((LM_ERROR,"%p\n","Error in close"));
			return;
		}
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("Msg_transceiver 创建套接字成功！\n")));

	struct linger m_lin;
	m_lin.l_onoff = 1;	//修改为1，linger设为0，关闭端口时不经过CLOSE_WAIT的状态
	m_lin.l_linger = 0;
	m_peer.set_option(SOL_SOCKET, SO_LINGER, (void*)&m_lin, sizeof(linger));

	//禁用Nagle算法
	int on = 1;
	m_peer.set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(on));

	//设置发送接收缓冲区
	int temp = DEVICE_TX_BUF_SIZE;
	m_peer.set_option(SOL_SOCKET, SO_RCVBUF, (char *)&temp, sizeof(temp));
	m_peer.set_option(SOL_SOCKET, SO_SNDBUF, (char *)&temp, sizeof(temp));

	//设置为非阻塞模式
	m_peer.enable(ACE_NONBLOCK);

	ACE_Time_Value time_out(1, 0);		//add by lujiashun 非阻塞模式不设超时，connect会返回-1;

	if(m_connector.connect(m_peer, m_addr, &time_out) == -1)
	{
		ACE_ERROR((LM_ERROR,"%p\n","Msg_transceiver Error in Connect"));
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("Msg_transceiver 连接成功！\n")));
}

/**************************************************************************
关闭通道端口
**************************************************************************/
void Msg_transceiver::close_dev(void)
{
	if(m_peer.close() == -1)
	{
		ACE_ERROR((LM_ERROR, "%p\n", "Error in close"));
		return;
	}
	ACE_DEBUG((LM_INFO,ACE_TEXT("关闭通道成功！\n")));
}

/**************************************************************************
通道是否打开
**************************************************************************/
bool Msg_transceiver::is_open(void)
{
	return m_peer.get_handle() >= 0 ? true : false;
}


/**************************************************************************
把发送缓冲区的数据发送出去
**************************************************************************/
int Msg_transceiver::send_msg(void)
{
	int m_send_lenth = 0;	//发送报文的长度
	unsigned char send_buf[1024];		//发送缓存
	FE_MSG *send_msg_ptr;

	if(!m_msg_list.empty())		///无可发送报文不处理
	{
		FE_MSG *msg_ptr = get_head_msg();
		if (!msg_ptr)
		{
			return 0;
		}
		for(int n_vector_num =0; n_vector_num < get_head_msg_size(); n_vector_num++)
		{
			send_msg_ptr = msg_ptr + n_vector_num;
			ACE_OS::memset(send_buf, 0, 1024);
			ACE_OS::memcpy(send_buf, SYNC_FRAME_VAL, 6); //报文头；
			ACE_OS::memcpy(&send_buf[6], (unsigned char*)send_msg_ptr, sizeof(FE_MSG)); //报文
			m_send_lenth = m_peer.send(send_buf, sizeof(FE_MSG)+6);
			if (m_send_lenth <= 0)	//发送失败，跳出循环
			{
				break;
			}
		}
		//清空发送过的数组
		ACE_Guard<ACE_Thread_Mutex> guard (m_msg_mutex);	//申请锁d6000
		clear_head_msg();
	}
	else
		return 0;
	
	return m_send_lenth > 0 ? 0 : -1;
}
