/**
@file statchecker.h
@brief 通道状态检查类声明文件
@author zhangyun 
@version 1.0.0
@date 2012-11-15
*/

#ifndef STAT_CHECKER_H_
#define STAT_CHECKER_H_

#include "ace/Task.h"

class ACE_Thread_Mutex;

/**
@brief 检查通道状态类

循环检查本机值班的所有通道的状态
*/
class Stat_checker : public ACE_Task<ACE_MT_SYNCH>
{
	//定义字符长度
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		CHECK_STAT_TIME_SPAN = 2
	};

public:
	explicit Stat_checker(ACE_Thread_Mutex& check_watch_load_mutex);
	/**
	@brief 拷贝构造函数

	只声明不实现，防止误用
	*/
	Stat_checker(const Stat_checker&);

	/**
	@brief 赋值运算符

	只声明不实现，防止误用
	*/
	Stat_checker operator = (const Stat_checker&);

	/**
	@brief 析构函数
	*/
	virtual ~Stat_checker();

	/**
	@brief 启动
	*/
	virtual int open (void *args = 0);

	/**
	@brief 停止
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief 通道状态检查线程体

	过程是check_stat，然后休眠一段时间，200ms ?
	*/
	virtual int svc(void);

private:
	/**
	@brief 通道状态检查过程

	除了检查通道状态，还需要从消息队列获取通道的switch policy，自动或手动
	注意检查前需要锁定check_watch_mutex,可以使用
	如:
	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_mutex)，这样无需手动去解锁
	*/
	virtual int check_stat(void);
	/**
	@brief 多通道切换
	如果switch policy是自动，则通过CHANPARA中的parent_channel上溯，切换到最近的状态正常的通道。
	如果上溯失败，则通过child_channel下溯，切换到最近的状态正常的通道。
	*/
	void switch_channel(const int& channel_no);
	/**
	@brief 通道事项
	当通道状态发生变化，发出相应的事项
	*/
	void channel_event(int channel_no);
	/**
	@brief 通道切换事项
	当通道主备状态发生变化，发出相应的事项
	*/
	void switch_channel_event(int channel_no, bool main_back);
	/**
	@brief 检查通道事项
	当通道发出事项，返回TRUE，否则返回FALSE
	*/
	bool check_channel_event(int channel_no);
	/**
	@brief 检查通道状态
	执行检查通道状态,并强制执行主备状态的切换
	*/
	void check_channel_status(int channel_no);

private:
	ACE_Thread_Mutex&	m_check_watch_load_mutex;	//为了加载、负载均衡和状态检查的互斥
	unsigned char*		m_mail_buf;		//邮件缓存，必须大于等于最大可能的邮件长度
	int m_host_no;		//本机节点号
};
#endif