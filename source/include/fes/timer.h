
/**
@file timer.h
@brief 计时类
@author zhangyun 
@version 1.0.0
@date 2013-10-11
*/
#ifndef FES_TIMER_H_
#define FES_TIMER_H_

#include <ace/OS.h>

/**
@brief 秒级计时类

仿照Qt和Boost计时类的一个简单实现，调用elapsed获得上次restart后的时间
调用restart重置计时器到当前时间
*/
class Timer
{
public:
	/**
	@brief 构造函数
	*/
	Timer(){restart();};
	/**
	@brief 重置计时器到当前时间
	*/
	void restart(void)
	{
		m_time = ACE_OS::time(NULL);
		m_stop_flag=false;
	};
	/**
	@brief 停止计时
	*/
	void stop(void){m_stop_flag=true;};
	/**
	@brief 获得上次restart后过去的时间
	@return 秒
	*/
	int elapsed(void) const 
	{
		if ( !m_stop_flag )
		{
			return static_cast<int>(ACE_OS::time(NULL) - m_time);
		}
		else
		{
			return 0 ;
		}
	};
	/**
	@brief 获得上次计时器重置的时间
	*/
	time_t get_start_time(void) const {return m_time;};
private:
	time_t m_time;
	bool m_stop_flag;
};
#endif