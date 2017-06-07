#include "fes/msgservice.h"
#include "fes/msgserviceacceptor.h"

#include "fes/fesshm.h"

#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>

#include <ace/OS.h>


Msg_service::Msg_service()
{
	m_channel_no = FREE_SERVICE;		//初始化通道号为FREE_SERVICE，默认空转
	m_current_host_no = Fes_shm::get().fe_sys_data_ptr->host_no;		//当前机器的hostno
}

Msg_service::~Msg_service()
{
	close();
}

int Msg_service::open(void *)
{
	ACE_DEBUG((LM_DEBUG,"(%t) Msg_Service open !\n")); 
	//Activate the object with a thread in it. 
	activate(); 
	return 0; 

}

int Msg_service::close(u_long)
{
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_Service being closed down !\n"));

	ACE_OS::memset(recv_buf,0,SIZE_DATA);
	m_peer.close();
	return 0; 

}

int Msg_service::svc()
{
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_Service start thread !\n")); 
	// do thread specific work here 

	ACE_INET_Addr client_addr;
	int byte_count=0; 
	int new_channel_no = 0;

	while(true)
	{
		if(m_peer.get_remote_addr(client_addr) == -1)
		{
			ACE_OS::sleep(ACE_Time_Value(0,1*1000));			//空转线程，抛弃cpu使用，降低cpu使用率
			continue;
		}
		else
		{
			ACE_DEBUG((LM_DEBUG,"(%t) Msg_service connect client !\n"));
			m_channel_no = UN_INIT;
		}

		while(true)
		{
			ACE_OS::memset(recv_buf,0,SIZE_DATA); //清空接收缓存区

			byte_count = m_peer.recv(recv_buf,SIZE_DATA);
			if ( byte_count == 0 )			//连接中断 break；
			{
				ACE_ERROR ((LM_ERROR, "%p\n", "(%t) Msg_Service Stream Closed !")); 
				m_peer.close();
				break;
			}
			else if( byte_count == -1 )		//接收空，或连接上无接收数据 break;
			{
				if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
				{
					if (m_channel_no == UN_INIT)		//msgview连接成功，但未请求通道
					{
						ACE_OS::sleep(ACE_Time_Value(0,1*1000));	//抛出cpu
						continue;
					}	
				}
				else
				{
					ACE_ERROR ((LM_ERROR, "%p\n", "(%t) Msg_Service Stream recv error!")); 
					m_peer.close();
					break;
				}
			}
			else 							//成功接收	
			{ 
				recv_buf[byte_count]=0; 
				new_channel_no = recv_buf[0] + recv_buf[1]*256;		//暂时约定低位在前，高位在后
				ACE_DEBUG((LM_DEBUG,"(%t) Msg_Service received request channel_no = %d !\n",new_channel_no));

				//共享内存获取，进行比对 hostno
				
				CHANDATA* pchanneldata =  Fes_shm::get().chan_data_ptr + new_channel_no;
				int channel_host_no = pchanneldata->curr_attend_host;				//该通道的值班节点no


				unsigned char confirmbuf[2];
				if (channel_host_no != m_current_host_no)//channel_no不由本机值班 break
				{
// 					confirmbuf[0] = 0xff;
// 					confirmbuf[1] = 0xff;
// 					m_peer.send(confirmbuf,2);
					m_peer.close();
					break;
				}
				else
				{
					confirmbuf[0] = 0xcc;
					confirmbuf[1] = 0xcc;
					m_peer.send(confirmbuf,2);
				}
				

				//若通道号改变,重新注册服务
				if( new_channel_no != m_channel_no )		
				{
					Msg_service_acceptor::get()->remove_service(m_channel_no, this);
					Msg_service_acceptor::get()->add_service(new_channel_no, this);
					m_channel_no = new_channel_no;
				}
			}

			//从消息队列获取报文
			ACE_Message_Block * rmb = 0;
 			ACE_Time_Value timeout(1,0);			//超时时间1S
			if(getq( rmb, &timeout )==-1)		//超时不做处理，直接continue
			{
				ACE_OS::sleep(ACE_Time_Value(0,1*1000));
				continue;
			}
			ACE_OS::memset((unsigned char*)&msgmail,0,sizeof(FE_MSG));
			ACE_OS::memcpy((unsigned char*)&msgmail,rmb->rd_ptr(),sizeof(FE_MSG));			//copy报文
			delete rmb;


			//如果接收到的报文不是当前需要传送通道的报文，清空，break；
			if(msgmail.channel_no != m_channel_no)				
			{
				ACE_OS::memset((unsigned char*)&msgmail,0,sizeof(FE_MSG));
				continue;
			}


			//从peer发出，失败break；
			if (m_peer.send(&msgmail,sizeof(FE_MSG)) == -1) 
			{ 
				ACE_ERROR ((LM_ERROR,"%p\n","(%t) Msg_Service send error !"));
				m_peer.close();
				break; 
			}
		}

		Msg_service_acceptor::get()->remove_service(m_channel_no, this);
		m_channel_no = FREE_SERVICE;

		ACE_OS::sleep(ACE_Time_Value(0,1*1000));			//空转线程，抛弃cpu使用，降低cpu使用率

	}
	return 0; 

}