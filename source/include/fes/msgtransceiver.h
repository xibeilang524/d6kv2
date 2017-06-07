/**
@file msgtransceiver.h
@brief 报文收发类声明
@author hongxiang 
@version 1.0.0
@date 2013-11-01
*/
#ifndef MSG_TRANSCEIVER_H_
#define MSG_TRANSCEIVER_H_

//#include "sysdef.h"
#include "fes/arraylist.h"
#include "fes/fesshare.h"
#include "fes/fesshm.h"
#include "fes/fesdev.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	MSG_TRANSCEIVER_EXPORT __declspec(dllexport) 
#else
#define	MSG_TRANSCEIVER_EXPORT __declspec(dllimport) 
#endif
#else
#define	MSG_TRANSCEIVER_EXPORT
#endif

class Msg_transceiver;

//报文收发类的单实例
typedef ACE_Singleton<Msg_transceiver, ACE_SYNCH_MUTEX> Msg_sender;

/**
@brief 报文收发类
*/

class MSG_TRANSCEIVER_EXPORT Msg_transceiver : public ACE_Task<ACE_MT_SYNCH>
{
	//定义字符长度
	enum
	{
		NET_NUM = 2,
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		DEVICE_TX_BUF_SIZE = 16384,
		SEND_MSG_TIME_SPAN = 1
	};

	friend class ACE_Singleton<Msg_transceiver, ACE_SYNCH_MUTEX>;

public:

	~Msg_transceiver();

	/**
	@brief 启动
	*/
	virtual int open (void *args = 0);

	/**
	@brief 停止
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief 线程体
	*/
	virtual int svc(void);
	/**
	@brief 打开通道端口
	*/
	void open_dev(void);

	/**
	@brief 关闭通道端口
	*/
	void close_dev(void);

	/**
	@brief 通道端口是否打开
	打开返回true，未打开返回false
	*/
	bool is_open(void);

	/**
	@brief 把单个报文结构体数据写入全局缓存
	@param in FE_MSG& msg 报文
	*/
	void put_msg(const FE_MSG& msg)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_msg_mutex);
		m_msg_list.insert_data(msg);
	};

	/**
	@brief 把多个报文结构体数据写入全局缓存
	@param in vector<FE_MSG>& msgs 事项数组
	*/
	void put_msg(vector<FE_MSG>& msgs)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_msg_mutex);
		m_msg_list.insert_data(msgs);
	};

	/**
	@brief 清除第一个报文数组
	*/
	void clear_head_msg(void){m_msg_list.clear_head();};

	/**
	@brief 获取首个报文数组的指针
	@return 成功返回指针，失败返回NULL
	*/
	FE_MSG* get_head_msg(void){return m_msg_list.get_head_array();};

	/**
	@brief 获取首个报文数组的长度
	@return 成功返回长度，失败返回-1
	*/
	int	get_head_msg_size(void){return m_msg_list.get_head_size();};

protected:

	/**
	@brief 把发送缓冲区的数据发送出去

	@return 成功返回0，失败返回-1
	*/
	int send_msg(void);

private:
	/**
	@brief 构造函数，初始化成员，不予显式调用
	*/
	Msg_transceiver();
	
	/**
	@brief 拷贝构造函数，只声明不实现
	
	如果不声明，类中会存在默认拷贝构造函数（逐字节拷贝），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Msg_transceiver(const Msg_transceiver&);

private:
	ACE_INET_Addr	m_addr; 
	ACE_SOCK_Stream		m_peer; 
	ACE_SOCK_Connector	m_connector; 
//	map<int, FE_MSG> m_msg_map; ///通道号与通道报文之间的映射
	Array_list<FE_MSG> m_msg_list;  ///报文收发缓存
	ACE_Thread_Mutex   m_msg_mutex;	///<事项互斥锁
};

#endif