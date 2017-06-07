/**
@file globalbuf_yk_check.h
@brief 前置全局遥控返校数据缓冲区类声明
@author lujs 
@version 1.0.0
@date 2016-03-02
*/
#ifndef GLOBAL_BUF_YKCHECK_H_
#define GLOBAL_BUF_YKCHECK_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_GLOBALBUF_YKCHECK_EXPORT __declspec(dllexport) 
#else
#define	FES_GLOBALBUF_YKCHECK_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_GLOBALBUF_YKCHECK_EXPORT
#endif

#include "fes/arraylist.h"
#include "fes/fesdatashm.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"

/**
@brief 遥控返校数据缓冲区类

缓存所有向scada发送的数据，包括生数据、事项并发送给scada。由于一个数据区
往往有多个线程访问，需要设置互斥
*/
class FES_GLOBALBUF_YKCHECK_EXPORT Global_buf_yk_check : ACE_Task<ACE_MT_SYNCH>
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
	static Global_buf_yk_check& get(void)
	{
		static Global_buf_yk_check instance;
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
	Global_buf_yk_check(const Global_buf_yk_check&);

	/**
	@brief 等于运算符，只声明不实现
	
	如果不声明，类中会存在默认赋值运算符（逐字节赋值），可能会损坏单例中的数据，
	用只声明不实现的方法覆盖默认赋值运算符，如果在使用中调用了赋值运算符，会产
	生编译错误
	*/
	Global_buf_yk_check& operator = (const Global_buf_yk_check&);

	/**
	@brief 锁定遥控返校数据缓存
	*/
	void lock_ykcheck_list(void){m_ykcheck_mutex.acquire();};

	/**
	@brief 解锁遥控返校数据缓存
	*/
	void unlock_ykcheck_list(void){m_ykcheck_mutex.release();};

	/**
	@brief 把单个遥控返校数据写入全局缓存
	@param in const FE_YK_CHECK_NODE& ykcheck 遥控返校数据
	*/
	void put_yk_check(const FE_YK_CHECK_NODE& ykcheck)
	{

		ACE_Guard<ACE_Thread_Mutex> guard(m_ykcheck_mutex);
		m_ykcheck_list.insert_data(ykcheck);
	};


	/**
	@brief 把多个遥控返校数据写入全局缓存
	@param in vector<FE_YK_CHECK_NODE>& ykchecks 遥控返校数据数组
	*/
	void put_yk_check(vector<FE_YK_CHECK_NODE>& ykchecks)
	{
		ACE_Guard<ACE_Thread_Mutex> guard(m_ykcheck_mutex);
		m_ykcheck_list.insert_data(ykchecks);
	};

	/**
	@brief 清除第一个生数据数组
	*/
	void clear_head_raw(void){m_ykcheck_list.clear_head();};

	/**
	@brief 获取首个遥控返校数据数组的指针
	@return 成功返回指针，失败返回NULL
	*/
	FE_YK_CHECK_NODE*	  get_head_ykcheck(void){return m_ykcheck_list.get_head_array();};

	/**
	@brief 获取首个遥控返校数据数组的长度
	@return 成功返回长度，失败返回-1
	*/
	int	get_head_ykcheck_num(void){return m_ykcheck_list.get_head_size();};

private:
	explicit Global_buf_yk_check();

private:
	Array_list<FE_YK_CHECK_NODE>			m_ykcheck_list;			///<遥控返校数据缓存

	ACE_Thread_Mutex			m_ykcheck_mutex;		///<遥控返校数据互斥锁
	
};

#endif