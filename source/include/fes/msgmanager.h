#ifndef MSG_MANAGER_H_
#define MSG_MANAGER_H_

#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Task.h>
#include <vector>
#include <map>
using std::vector;
using std::map;

#include "fes/fesshare.h"

#define  MAX_MSG_FRAME	10

#define  MAX_RECV_SIZE	5120		//每次接收的数组
#define  MAX_CACHE_SIZE	102400		//缓存空间的大小

#define  START_CODE		0xEB
#define  SYNC_CODE		0x90


/**
@brief 接收前置报文，存储报文，若有需要且分发报文  
*/
class Msg_manage : public ACE_Task<ACE_MT_SYNCH>
{
public:
	typedef vector<FE_MSG*> Msg_Save_List;

	/**
	单例访问接口

	Msg_manage::get().open....
	*/
	static Msg_manage* get(void);

	static Msg_manage* m_instance;

	~Msg_manage(void);

private:
	Msg_manage(); 
	
public: 
	int open(void*);

	int close(u_long);

	/**
	@brief 报文管理线程体

	其内部是一个双层循环

	while(true)

	尝试accept

	accept成功走到这，不然continue
	while(true)
	{
		接收报文，失败break，成功往下

		得到的报文应该有很多个依次执行save和deliver，save暂时不要实现吧，因为怎么报文数千个文件还需要好好想想
	}
	*/
	int svc(void);

public:

	/**
	@brief 保存报文
	@return 成功返回0，失败返回-1
	*/
	int save(const FE_MSG& msg);

	/**
	@brief 将报文分发至报文服务,调用acceptor的put_msg方法
	*/
	int deliver(const FE_MSG& msg);

	/**
	@brief 报文保存至文件
	*/
	void save_to_file(Msg_Save_List *save_list);
	/**
	@brief 报文保存至文件
	@para in unsigned char* recv_pointer  tcp接收缓存区的指针
	@para in int recv_pointer  tcp接收成功后的接收字节数
	*/
	void proc_recv_buf(unsigned char* recv_poniter, int nrecv_count );

private:
	ACE_SOCK_Acceptor m_acceptor;	//TCP接收器
	ACE_SOCK_Stream	  m_peer;		//TCP流

	map<int, Msg_Save_List*> m_channel_msg_list;	//通道对应的缓存链表

	unsigned char  cache_buf[MAX_CACHE_SIZE];		//接收处理缓冲区
	int write_pos;


};

/**
@brief 检测前置发送的报文的同步头
@para in unsigned char* buf  需检测的报文数组的指针
@return 成功返回true，失败返回false
*/
bool Check_msg_head(unsigned char* buf);


#endif



