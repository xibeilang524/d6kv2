#ifndef MSG_SERVICE_H_
#define MSG_SERVICE_H_

#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Task.h>

#include "fes/fesshare.h"

#define  SIZE_DATA 2		//接收msgview发送请求的通道号

class Msg_service_acceptor;

class Msg_service : public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		FREE_SERVICE = -2,			//空闲状态
		UN_INIT = -1			//未初始化通道号
	};
	Msg_service();
	~Msg_service();

	/**
	@brief 继承自Task类需重新实例化的虚函数
	*/
	int open(void *);
	int close(u_long flags = 0);

	/**
	@brief 线程体

	其内部是一个循环

	while(true)
	{
		while(true)
		{
			//连接中断 break；
			//channel_no不由本机值班 break；

			//从消息队列获取报文

			//从peer发出，失败break；
			

			//从peer接收,更新channel_no，失败break；

			if(通道号发生变化)
			{
				acceptor.remove_service(old_channel_no, this);
				acceptor.add_service(m_channel_no, this);
			}
		}

		acceptor.remove_service(m_channel_no, this);
		m_channel_no = -1；
	}
	*/
	int svc(void);

	/**
	@brief 服务空闲
	*/
	int service_free(){return m_channel_no < -1 ? 1 : 0;};

	ACE_SOCK_Stream& get_peer(void){return m_peer;};

private:
	int						m_channel_no;
	int						m_current_host_no;
	ACE_SOCK_Stream			m_peer;
	FE_MSG					msgmail;
	unsigned char			recv_buf[SIZE_DATA];
// 	Msg_service_acceptor*   acceptor;
};



#endif