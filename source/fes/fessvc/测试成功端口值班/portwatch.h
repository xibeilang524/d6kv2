/**
@file portwatch.h
@brief 端口值班类声明
@author zhangyun 
@version 1.0.0
@date 2012-11-15
*/
#ifndef FES_PORT_WATCH_H_
#define FES_PORT_WATCH_H_

#include "ace/Task.h"
#include <vector>
#include "fes/fesshare.h"
using namespace std;

class ACE_Thread_Mutex;
class Fe_node;
class Fes_shm;


/**
@brief 端口值班类

提供端口值班状态发送、接收、判断功能
*/
class Port_watch : public ACE_Task<ACE_MT_SYNCH>
{
	enum
	{
		SLEEP_TIME = 1,				///<每次端口值班操作后的睡眠时间
		GET_MSG_WAIT = 200,			///<从消息队列获取邮件的等待时间,200毫秒
		GET_MSG_NUM = 1,			///<每次获取获取几个消息
		HEART_BEAT_TIME_LIMIT = 5	///<心跳
	};
	/**
	@brief 端口值班报文的格式
	*/
	struct PORT_WATCH_MSG
	{
		int fe_index;				///<主机号
		unsigned long load_map;		///<负载
	
		unsigned char give_back_flag;//有归还通道

// 		time_t msg_time;			///邮件时标

		int attend_gps_num;			///<监管的GPS的数量
		GPS_INFO gps[MAX_GPS_NUM];	///<最大的GPS数量

		int attend_channel_num;		///<监管的通道的数量

		FE_CHAN_STAT chan_stat[1];	///<通道状态，注意此数组长度为1是为了在有内存足够的前提下提供一种简易的变长数组，省去

		/**
		@brief 根据通道的数量计算报文的长度
		*/
		int get_lenth(void)
		{
			return sizeof(PORT_WATCH_MSG) + ((attend_channel_num > 0 ? (attend_channel_num - 1) * sizeof(FE_CHAN_STAT) : 0));
		}

		void clear(void)
		{
			fe_index = 0;
			attend_gps_num = 0;
			attend_channel_num = 0;
			load_map = 0;
			memset(gps, 0, sizeof(gps));
		}

		static int get_lenth(const int& channel_num)
		{
			return sizeof(PORT_WATCH_MSG) + (channel_num > 0 ? (channel_num - 1) * sizeof(FE_CHAN_STAT) : 0);
		}
	};

public:

	Port_watch(ACE_Thread_Mutex& check_watch_load_mutex);

	/**
	@brief 拷贝构造函数

	只声明不实现，防止误用
	*/
	Port_watch(const Port_watch&);

	/**
	@brief 赋值运算符

	只声明不实现，防止误用
	*/
	Port_watch operator = (const Port_watch&);

	/**
	@brief 析构函数
	*/
	virtual ~Port_watch();

	/**
	@brief 启动
	*/
	virtual int open(void *args = 0);

	/**
	@brief 停止
	*/
	virtual int close(u_long flags = 0);

	/**
	@brief 负载均衡线程体

	port_watch_proc-睡眠一秒钟
	*/
	virtual int svc(void);
private:
	/**
	@brief 创建端口值班用的各前置节点
	*/
	void create_host_nodes(void);
	/**
	@brief 负载均衡过程

	其流程是发送-接收-解决冲突-接管
	注意检查前需要锁定check_watch_load_mutex,可以使用
	如:
	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_load_mutex)，这样无需手动去解锁
	*/
	void port_watch(void);
	/**
	@brief 用邮件向所有前置节点和scada节点发送端口值班报文
	*/
	int send_stat(void);
	/**
	@brief 从消息队列取出邮件线程转送的端口值班邮件
	
	把接收到通道状态写入共享内存，同时更新每个节点的状态
	注意需要忽略由本机发出的邮件
	*/
	void recv_stat(void);

	/**
	@brief 解决本机与其它节点的值班冲突

	即如果发现本机与其它节点同时值班一个服务器，如果对方节点负荷与本机一样大且节点号比本机小，则本机放弃接管
	如果对方节点是默认值班机，也放弃接管
	*/
	void resolve_conflict(void);

	/**
	@brief 接管过程

	检查是否有主机从运行到停止，如果有，检查本机是否是负荷最小且节点号最小的，是则进行接管
	检查是否有主机从停止到运行，如果有且是本机在接管，则放弃接管
	*/
	void take_over(void);

private:
	vector<Fe_node*>	m_fe_nodes;		//所有前置节点的集合，按照其在平台中的排序
	int					m_index;		//本机在前置节点集合中的位置
	unsigned char*		m_mail_buf;		//邮件缓存，必须大于等于最大可能的邮件长度
	ACE_Thread_Mutex&	m_check_watch_load_mutex;	//为了加载、负载均衡和状态检查的互斥
	Fes_shm&			m_fes_shm;			//共享内存访问引用

	unsigned char		m_give_back_flag;		//本前置发生了归还动作,0:无 1:有
// 	unsigned char		m_valid_msg;		//本机有效符合变化位,0:无效 1:有效 使其他前置能够判断该前置负荷已发生变化
	unsigned char		m_already_give_up;	//其他前置发生了归还动作,0:无 1:有
};

#endif