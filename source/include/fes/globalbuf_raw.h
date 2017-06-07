/**
@file globalbuf_raw.h
@brief 前置全局生数据缓冲区类声明
@author zhangyun 
@version 1.0.0
@date 2012-11-02
*/
#ifndef GLOBAL_BUF_H_
#define GLOBAL_BUF_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_GLOBALBUF_EXPORT __declspec(dllexport) 
#else
#define	FES_GLOBALBUF_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_GLOBALBUF_EXPORT
#endif

#include "fes/arraylist.h"
#include "fes/fesdatashm.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"

/**
@brief 全局数据缓冲区类

缓存所有向scada发送的数据，包括生数据、事项并发送给scada。由于一个数据区
往往有多个线程访问，需要设置互斥
*/
class FES_GLOBALBUF_EXPORT Global_buf_Raw : ACE_Task<ACE_MT_SYNCH>
{
	enum
	{
		ARRAY_EVENT_NUM = 8192,	///<全局缓存中事项数组的长度
		ARRAY_RAW_NUM = 8192,	///<全局缓存中生数据数组的长度
		EVENT_LIST_SIZE = 8,	///<全局缓存中事项数组链表的长度
		RAW_LIST_SIZE = 8		///<全局缓冲中生数据数组链表的长度
	};
public:
	/**
	@brief 单实例获取函数
	*/
	static Global_buf_Raw& get(void)
	{
		static Global_buf_Raw instance;
		return instance;
	};

	/**
	@brief 启动
	*/
	virtual int open (void *args = 0);

	/**
	@brief 停止
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief 数据发送线程体
	*/
	virtual int svc(void);

	/**
	@brief 拷贝构造函数，只声明不实现
	
	如果不声明，类中会存在默认拷贝构造函数（逐字节拷贝），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Global_buf_Raw(const Global_buf_Raw&);

	/**
	@brief 等于运算符，只声明不实现
	
	如果不声明，类中会存在默认赋值运算符（逐字节赋值），可能会损坏单例中的数据，
	用只声明不实现的方法覆盖默认赋值运算符，如果在使用中调用了赋值运算符，会产
	生编译错误
	*/
	Global_buf_Raw& operator = (const Global_buf_Raw&);

	/**
	@brief 锁定生数据缓存
	*/
	void lock_raw_list(void){m_raw_mutex.acquire();};

	/**
	@brief 解锁生数据缓存
	*/
	void unlock_raw_list(void){m_raw_mutex.release();};

	/**
	@brief 把单个生数据写入全局缓存
	@param in const FE_RAW& raw 生数据
	*/
	void put_raw(const FE_REAL_DATA_NODE& raw)
	{

		ACE_Guard<ACE_Thread_Mutex> guard(m_raw_mutex);
		m_raw_list.insert_data(raw);
	};


	/**
	@brief 把多个生数据写入全局缓存
	@param in vector<FE_RAW>& raws 生数据数组
	*/
	void put_raw(vector<FE_REAL_DATA_NODE>& raws)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_raw_mutex);
		m_raw_list.insert_data(raws);
	};

	/**
	@brief 清除第一个生数据数组
	*/
	void clear_head_raw(void){m_raw_list.clear_head();};

	/**
	@brief 获取首个生数据数组的指针
	@return 成功返回指针，失败返回NULL
	*/
	FE_REAL_DATA_NODE*	  get_head_raw(void){return m_raw_list.get_head_array();};

	/**
	@brief 获取首个生数据数组的长度
	@return 成功返回长度，失败返回-1
	*/
	int	get_head_raw_num(void){return m_raw_list.get_head_size();};

private:
	explicit Global_buf_Raw();

private:
	Array_list<FE_REAL_DATA_NODE>			m_raw_list;			///<生数据缓存

	ACE_Thread_Mutex			m_raw_mutex;		///<生数据互斥锁
	
};

#endif