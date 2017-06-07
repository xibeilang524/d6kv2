/**
@file paraloader.h
@brief 前置参数加载类声明
@author zhangyun 
@version 1.0.0
@date 2012-11-16
*/
#ifndef CHANNEL_CHUSER_H_
#define CHANNEL_CHUSER_H_

#include "fes/fesshm.h"
#include "fes/protocolfactory.h"

#include <map>
#include <string>
using namespace std;

class Base_protocol;
class ACE_Thread_Mutex;


//int ret = Protocol_factory::instance()->get_all_config();

/**
@brief 前置通道簇类

提供参数的加载，通道实例的创建等功能
*/
class Channel_chuser
{
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		LONGCHAR_LEN = 256
	};

public:
	explicit Channel_chuser(ACE_Thread_Mutex& check_watch_load_mutex);
	/**
	@brief 拷贝构造函数

	只声明不实现，防止误用
	*/
	Channel_chuser(const Channel_chuser&);
	/**
	@brief 赋值运算符

	只声明不实现，防止误用
	*/
	Channel_chuser operator = (const Channel_chuser&);

	~Channel_chuser();
	/**
	@brief 读入规约特殊配置、通道、终端等

	注意之前需要检查规约工厂是否为空，如果是空的，可以直接返回错误
	参数读取成功后，可调用check_channels
	*/
	int load(void);

	/**
	@brief 把邮件写入规约的消息队列

	调用ACE_Task的putq()方法把邮件写入消息队列
	@return 成功返回0，失败返回-1
	*/
	int put_mail(void* mail, const int& lenth);
private:
	/**
	@brief 读取规约类型表, 创建规约工厂

	只在系统初始化时调用一次，以后的每次load都不涉及规约工厂，所以放在构造函数而不是load函数中调用
	*/
	void generate_protocol_factory(void);
	/**
	@brief 调用规约库配置读取功能读取规约特殊配置
	*/
	void read_protocol_config(void);
	/**
	@brief 从数据库中读取通道参数

	读取成功后会把参数写入文件
	@return 成功返回0，失败返回-1
	*/
	int read_channel_para_from_db(void);
	/**
	@brief 从文件中读取通道参数
	@return 成功返回0，失败返回-1
	*/
	int read_channel_para_from_file(void);
	/**
	@brief 从数据库中读取通道参数

	读取成功后会把参数写入文件
	@return 成功返回0，失败返回-1
	*/
	int read_rtu_para_from_db(void);
	/**
	@brief 从文件中读取通道参数
	@return 成功返回0，失败返回-1
	*/
	int read_rtu_para_from_file(void);
	/**
	@brief 从文件中读取遥测参数
	@return 成功返回0，失败返回-1
	*/
	int read_yc_para_from_db(void);
	/**
	@brief 从文件中读取遥测参数
	@return 成功返回个数，失败返回-1
	*/
	int read_yc_para_from_file(YC_PARA_DB*);
	/**
	@brief 从文件中读取遥测参数
	@return 成功返回个数，失败返回-1
	*/
	int read_yx_para_from_file(YX_PARA_DB*);
	/**
	@brief 从文件中读取遥测参数
	@return 成功返回个数，失败返回-1
	*/
	int read_kwh_para_from_file(KWH_PARA_DB*);
    /**
	@brief 从文件中读取遥信参数
	@return 成功返回0，失败返回-1
	*/
	int read_yx_para_from_db(void);
	/**
	@brief 从文件中读取电度量参数
	@return 成功返回0，失败返回-1
	*/
	int read_kwh_para_from_db(void);

	/**
	@brief 设置GPS参数

	设置GPS通道和GPS数据的对应关系
	*/
	void set_gps_para(void);
	/**
	@brief 清空本地缓存的参数
	*/
	void clear_para(void);
	/**
	@brief 计算通道主备关系
	*/
	void calc_channel_priority(void);
	/**
	@brief 计算终端优先级
	*/
	void calc_rtu_priority(void);
	/**
	@brief 计算终端通道关系
	*/
	void calc_rtu_channel(void);
	/**
	@brief 保存通道参数到本地文件,成功返回0,失败返回-1
	*/
	int save_channel_para(void); 
	/**
	@brief 保存终端参数到本地文件,成功返回0,失败返回-1
	*/
	int save_rtu_para(void); 
	/**
	@brief 保存遥测参数到本地文件,成功返回0,失败返回-1
	*/
	int save_yc_para(YC_PARA_DB* p_data, int rcd_cnt);
	/**
	@brief 保存遥信参数到本地文件,成功返回0,失败返回-1
	*/
	int save_yx_para(YX_PARA_DB* p_data, int rcd_cnt);
	/**
	@brief 保存电度参数到本地文件,成功返回0,失败返回-1
	*/
	int save_kwh_para(KWH_PARA_DB* p_data, int rcd_cnt);

	/**
	@brief 检查每个通道参数的变化并根据变化的情况创建、删除库规约实例

	如果通道从无到有，通过工厂创建并启动，如果从有到无，则关闭并删除之，如果参数被修改，则先关闭、删除再重新创建
	注意检查前需要锁定check_watch_load_mutex,可以使用
	如:
	ACE_Guard<ACE_Thread_Mutex> guard (*m_check_watch_load_mutex)，这样无需手动去解锁
	*/
	void check_channels(void);

	/**
	@brief 检测通道参数表中的默认节点是否为前置节点，若不是将其设置为序号最小的前置机

	*/
	void calc_hostno_enale(int& hostno);


private:
	CHANPARA*				   m_channel_para;		///<读取的通道参数
	RTUPARA*				   m_rtu_para;			///<读取的终端参数
	int						   m_max_channel;		///<从数据库中读出的最大通道号
	int						   m_max_rtu;			///<从数据库中读出的最大RTU
	Base_protocol**			   m_protocol;			///<规约实例指针的数组
	ACE_Thread_Mutex&		   m_check_watch_load_mutex;//为了加载、负载均衡和状态检查的互斥

	bool					   m_bnew_protocol[MAX_CHAN_NUM];	//是不是新创建线程
};

//文件名定义
// const char* const PROTOCOLFILE_NAME = "/dat/protocol.dat";
// const char* const CHANNELFILE_NAME = "/dat/channel.dat";
// const char* const RTUFILE_NAME = "/dat/rtu.dat";

#endif