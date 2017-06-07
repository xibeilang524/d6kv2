/**
@file fesnode.h
@brief 前置节点类声明
@author zhangyun 
@version 1.0.0
@date 2013-11-06
*/
#ifndef FES_NOED_H_
#define FES_NOED_H_

#include "fes/timer.h"

/**
@brief 前置节点信息
*/
class Fe_node
{
	enum
	{
		DEAD_TIME = 5
	};
public:
	/**
	@brief 构造函数
	@param in const int& host_no节点号
	@param in const int& host_index节点序号
	*/
	Fe_node(const int& host_no, const int& host_index);

	/**
	@brief 获取服务器运行状态
	*/
	int get_stat(void) const;

	/**
	@brief 收到心跳

	重置计时器
	*/
	void got_heartbeat(void){m_live_timer.restart();};

	/**
	@brief 获取服务器的负荷位图
	@return 服务器负荷
	*/
	unsigned long get_load(void) const {return m_load_map;};

	/**
	@brief 设置服务器负荷位图
	@return 服务器负荷
	*/
	void set_load(const unsigned long& load){m_load_map = load;};

	/**
	@brief 获取服务器状态
	@return 服务器状态，正常返回0，异常返回-1
	*/
	int get_srv_stat(void);

	/**
	@brief 通过负荷位图计算负荷
	@return 负荷
	*/
	int calc_load(void) const;
	/**
	@brief 获取前置服务器的序号
	@return 前置服务器的序号
	*/
	int get_index(void) const {return m_index;};
	/**
	@brief 获取节点号
	@return 节点号
	*/
	int get_host_no(void) const {return m_host_no;};

	/**
	@brief 是否主前置
	@return 0主前置
	*/
	int Is_Main_Server(void) const;

	/**
	@brief 获取采集区号
	@return 采集区号
	*/
	int get_collect_zone(void) const {return m_collect_zone;};

private:
	const int m_host_no;//主机号
	const int m_index;	//前置服务器的序号

	int m_collect_zone;	//采集区号

	unsigned long m_load_map;	//负载位图

	Timer m_live_timer;			//运行计时器，当收到该机邮件的时候重置
};
#endif