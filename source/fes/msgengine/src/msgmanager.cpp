#include "fes/msgmanager.h"
#include "fes/msgserviceacceptor.h"

#include <ace/OS.h>
#include <iostream>
#include <sys/stat.h>
#include <time.h>
using namespace std;

Msg_manage* Msg_manage::m_instance = NULL;

Msg_manage* Msg_manage::get()
{
	if (NULL == m_instance)
	{
		m_instance = new Msg_manage();
	}
	return m_instance;

}

Msg_manage::Msg_manage()
{
	//创建acceptor
	ACE_INET_Addr manage_server_addr(MSG_MNG_PORT);
	
	m_acceptor.open(manage_server_addr);

	if ( m_acceptor.get_local_addr(manage_server_addr) == -1 )
	{
		ACE_ERROR((LM_ERROR,"%p\n","Msg_manage Error in get_local_addr !"));
	}
	
	ACE_DEBUG ((LM_DEBUG,"Msg_manage Starting server at port %d !\n",manage_server_addr.get_port_number())); 

	ACE_OS::memset(cache_buf, 0, MAX_CACHE_SIZE);
	write_pos = 0;

	m_channel_msg_list.clear();
}

Msg_manage::~Msg_manage()
{
	close(0);
	m_channel_msg_list.clear();
	ACE_OS::memset(cache_buf, 0, MAX_CACHE_SIZE);
	write_pos = 0;

}

int Msg_manage::open(void*)
{
	ACE_DEBUG((LM_DEBUG,"(%t) Msg_manage opened !\n")); 
	activate();
	return 0;
}

int Msg_manage::close(u_long)
{
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_manage being closed down ! \n")); 
	m_peer.close();
	m_acceptor.close();
	return 0;
}

int Msg_manage::svc()
{ 
	ACE_DEBUG((LM_DEBUG, "(%t) Msg_manage start svc !\n")); 
	// do thread specific work here 

// 	ACE_Time_Value timeout (0,0);		//非阻塞方式

// 	FE_MSG msg;

	unsigned char recv_tmp[MAX_RECV_SIZE];
	
	while(true)
	{
		//尝试accept
		int retcode = m_acceptor.accept(m_peer,0);
		if ( -1 == retcode )
		{
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
// 				continue;
			}
			else
			{
				ACE_ERROR((LM_ERROR, "%p\n", "Msg_manage accept error"));
			}
			ACE_OS::sleep(ACE_Time_Value(0,1*1000));
			continue;
		}//出错continue
		else
		{
			ACE_DEBUG((LM_DEBUG,"%p\n", "Msg_manage accept new connect !\n"));
			m_peer.enable(ACE_NONBLOCK);  //接收模式非阻塞
		}

		ACE_OS::memset(cache_buf, 0, MAX_CACHE_SIZE);		//重新连接后清空缓冲区
		write_pos = 0;

		//accept成功走到这，不然continue	
		while(true)
		{
			//接收报文，失败break，成功往下
			ACE_OS::memset(recv_tmp, 0 , MAX_RECV_SIZE);
			int retresult = m_peer.recv(recv_tmp, MAX_RECV_SIZE );
			if (0 == retresult)
			{
				ACE_ERROR((LM_ERROR, "%p\n", "Msg_manage fep_msg recv_peer discon !"));
				m_peer.close();
				break;
			}
			if ( -1 == retresult )
			{
//				ACE_ERROR((LM_ERROR, "%p\n", "Msg_manage recv fep_msg nonthing"));
				if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
				{
					ACE_OS::sleep(ACE_Time_Value(0,1*1000));
				}
				else
				{
					break;
				}
			}

			proc_recv_buf(recv_tmp, retresult);
		}
	}

	return 0; 
}

int Msg_manage::save(const FE_MSG& msg)
{
	//To do  暂时定为报文收取后直接根据通道号放入不同的通道对
	//应的缓存中，在缓存到一定程度时存储
	Msg_Save_List *save_list = NULL;
	bool bsave = false;
	int nchannelno = msg.channel_no;
	FE_MSG *tmpmsg = new FE_MSG;
	ACE_OS::memcpy(tmpmsg, &msg, sizeof(FE_MSG));

	//缓存至内存中
	map<int, Msg_Save_List*>::iterator it =  m_channel_msg_list.find(nchannelno);

	if (it == m_channel_msg_list.end())		//不存在
	{
		save_list = new Msg_Save_List;
		m_channel_msg_list[nchannelno] = save_list;
	}
	else
	{
		save_list = it->second;
		save_list->push_back(tmpmsg);

		if (save_list->size() > MAX_MSG_FRAME)
		{
			bsave = true;
		}
	}
	
	//是否需要报文至文件
	if (bsave)
	{
		save_to_file(save_list);
	}


	return 0;
}

int Msg_manage::deliver(const FE_MSG& msg)
{
	Msg_service_acceptor::get()->put_msg(msg);
	return 0;
}

void Msg_manage::save_to_file(Msg_Save_List *save_list)
{
	char *Dir;
	char tm[512];
	Dir = getenv ("NBENV");
	char Name[300];
	FE_MSG *tmpmsg = save_list->front();
	int ncount = save_list->size();

	sprintf (Name, "%s/test/channel%d.txt", Dir, tmpmsg->channel_no);
	char* pFileName = Name;
	FILE* fp=NULL;

#ifdef WIN32
	struct _stat rdbfstat;
	int ret=_stat(Name,&rdbfstat);
#else
	struct stat rdbfstat;
	int ret=stat(Name,&rdbfstat);
#endif

	if(rdbfstat.st_size>=1024*1024*20)
		fp=fopen(pFileName,"w");
	else
		fp = fopen(pFileName,"a+");

	if( fp == NULL )	//打开失败返回
	{
		ACE_ERROR((LM_ERROR, "Msg_manage fep_msg open file fail !"));
		return;
	}

	for (int nframe = 0;nframe<ncount;++nframe)
	{
		strcat(tm,(char*)tmpmsg);
		fprintf(fp,"%d-%d-%d %d:%d:%d:%d ",tmpmsg->time.year+1900,tmpmsg->time.month,\
			tmpmsg->time.day,tmpmsg->time.hour,tmpmsg->time.minute,\
			tmpmsg->time.second,tmpmsg->time.milisecond);

		fprintf(fp,"%s", tmpmsg->recv_flag ? "(recv): " : "(send): ");

		for (int nlength = 0;nlength<tmpmsg->lenth;++nlength)
		{
			fprintf(fp,"%02x ",tmpmsg->msg[nlength]);
		}

		fprintf(fp,"\n");

// 		save_list->pop_front();

		tmpmsg = save_list->at(nframe);
	}
	fclose(fp);

	save_list->clear();

	return;
}

void Msg_manage::proc_recv_buf(unsigned char* recv_poniter, int nrecv_count )
{
	FE_MSG msg;

	//copy报文至缓冲区
	if (nrecv_count>0)
	{
		ACE_OS::memcpy(&cache_buf[write_pos], recv_poniter, nrecv_count);
		write_pos += nrecv_count;
	}

	//proc msg frame head
	int msg_length = sizeof(FE_MSG) + 6;  //一帧完整的报文长度

	int read_pos = 0;
	while(read_pos < write_pos)		//一次性处理完缓冲区的所有数据，除非剩余一帧不完整报文
	{
		while(cache_buf[read_pos] != START_CODE && write_pos > read_pos)	//找到报文同步头
		{
			read_pos++;
		}
		if ((write_pos - read_pos) >= msg_length)	//缓存中有完整报文才进行处理
		{
			if (Check_msg_head(&(cache_buf[read_pos])))	//报文头正确
			{
				ACE_OS::memset(&msg, 0, sizeof(FE_MSG));	//初始化msg结构体
				ACE_OS::memcpy((unsigned char*)&msg, &(cache_buf[read_pos+6]), sizeof(FE_MSG));
// 				save(msg);
				deliver(msg);
			}
			read_pos += msg_length;	//同步报文头不对，放弃该帧
		}
		else
		{
			break;
		}
	}

	ACE_OS::memmove(cache_buf, &cache_buf[read_pos], write_pos - read_pos);
	write_pos -= read_pos;
	ACE_OS::memset(&cache_buf[write_pos], 0, MAX_CACHE_SIZE - write_pos);	//移位后的数据区需初始化
}

//检测报文头，若满足同步帧返回true，否则返回false
bool Check_msg_head(unsigned char* buf)	
{
	bool retcode = true;

	for (int loop =0; loop < 3; loop++)
	{
		if (buf[2*loop] != START_CODE || buf[2*loop+1] != SYNC_CODE)
		{
			retcode = false;
			break;
		}
	}

	return retcode;
}