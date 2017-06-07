#ifndef MSG_SERVICE_ACCEPTOR_H_
#define MSG_SERVICE_ACCEPTOR_H_

#include <ace/Thread_Mutex.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Task.h>
#include <map>
#include <utility>
#include <vector>
using std::multimap;
using std::pair;
using std::make_pair;
using std::vector;

class Msg_service;
class ACE_SOCK_Stream;

#include "fes/fesshare.h"

class Msg_service_acceptor : public ACE_Task<ACE_MT_SYNCH>
{
public:
	enum
	{
		MAX_THREAD_SIZE = 16,
		PORT_NO = 9005			//想办法移动到更好的地方去
	};

	typedef multimap<int, Msg_service*>::iterator IT;		//linux下const_iterator无法erase；
	typedef pair<IT, IT> Range_NOCONST;
	typedef multimap<int, Msg_service*>::const_iterator CIT;
	typedef pair<CIT, CIT> Range;
public:
	/**
	单例访问接口
	Msg_service_acceptor::get().open....
	*/
	static Msg_service_acceptor* get(void);

	static Msg_service_acceptor* instance;

	~Msg_service_acceptor();

	/**
	@brief 继承自Task类需重新实例化的虚函数
	*/
	int open(void *);
	int close(u_long flags = 0);

	/**
	@brief 线程体

	其内部是一个循环

	Msg_service* free_service = get_free_service();

	while(true)
	{

	尝试accept

	m_acceptor.accept(free_service->get_peer(), ....)
	
	accept成功走到这，不然continue

	连接成功自然就不free了
	free_service = get_free_service();
	}
	*/
	int svc(void);

	/**
	@brief 初始化程序，建立线程池，被构造函数调用
	*/
	void init();

	/**
	@brief 把报文写入需要的服务,以下是实现的例子
	*/
	void put_msg(const FE_MSG& msg);

	/**
	@brief 删除通道和服务的映射关系
	*/
	void remove_service(const int& channel_no, const Msg_service* service);

	/**
	@brief 添加通道和服务的映射关系
	*/
	void add_service(const int& channel_no, Msg_service* service);

private:

	Msg_service_acceptor();

	Msg_service* get_free_service(void);

private:
	//通道号与服务线程之间的映射，由于通道号可能有多个，所以是multimap
	multimap<int, Msg_service*> m_active_task;

	ACE_Thread_Mutex	 task_list_mutex;

	vector<Msg_service *>	m_service_list;		//service池
	ACE_SOCK_Acceptor	m_acceptor;					//TCP接受器
};

#endif