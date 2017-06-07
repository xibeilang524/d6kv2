/**
@file platformwrayyer.h
@brief 前置平台包装头文件

由于ACE和pthread-win32两个开源库在windows下都定义了pid、timespec等posix标准
需要的类型，导致两者混用编译时发生冲突，而不幸平台头文件包含了pthread。
为了在前置使用ACE而不同时更改平台而编写平台包装

@author zhangyun 
@version 1.0.0
@date 2013-11-04
*/
#ifndef FES_PLATFORM_WRAPPER_H_
#define FES_PLATFORM_WRAPPER_H_ 

#ifdef WIN32
#ifdef PLATFORM_WRAPPER_DLL
#define	PLATFORM_WRAPPER_EXPORT __declspec(dllexport) 
#else
#define	PLATFORM_WRAPPER_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLATFORM_WRAPPER_EXPORT
#endif

#include "fes/fesshare.h"

//获取毫秒级时间
void PLATFORM_WRAPPER_EXPORT get_time(FETIME* time_ptr);

//常用函数检同步头，BCH校验
#define  START_CODE		0xEB
#define  SYNC_CODE		0x90

//CRC校验码状态
enum CRC_STATE
{
	CRC_READ = 0 ,
	CRC_WRITE = 1
};

bool PLATFORM_WRAPPER_EXPORT Scan_msg_head(unsigned char* buf);
void PLATFORM_WRAPPER_EXPORT MakeBch(unsigned char* bch,unsigned char* buf);
void PLATFORM_WRAPPER_EXPORT MakeBCHList (void);
bool PLATFORM_WRAPPER_EXPORT CRC_test(unsigned char *buf , int len , CRC_STATE flag ) ;


//采集网状态
#define		MAINDBG		201
#define		G101DBG		202

#define		PUB_FEP_SERVER				"PUBFEP"			/*3区采集前置机 front end processor*/

/**
@brief 共享内存类
*/
class PLATFORM_WRAPPER_EXPORT Share_memery
{
public:
	/**
	@brief 创建共享内存
	@param in int shm_key 共享内存ID
	@param in int shm_lenth 共享内存长度
	@return 成功返回共享内存地址，失败返回NULL
	*/
	static void* create_share_memory(int shm_key, int shm_lenth);
};

/**
@brief 进程管理类
*/
class PLATFORM_WRAPPER_EXPORT Proc_manager
{
public:
	/**
	@brief 向总线注册进程
	@param in char* asc_name 进程ascii名
	@param in char* ch_name 进程汉字名
	@param in char* username 进程用户名
	@param in HWND hwnd 进程主窗口句柄
	*/
//#ifdef WIN32
//	static void start_proc(char *asc_name, char *ch_name, char *username, HWND hwnd = NULL);
//#else
	static void start_proc(char *asc_name, char *ch_name, char *username);	
//#endif
	/**
	@brief 判断进程是否已经存在
	@param in char* proc 进程名
	@return 不存在返回-1，存在返回0
	*/
	static int proc_exist(char* proc);
	/**
	@brief 检查总线是否还在运行，通知总线本进程运行正常
	
	在遇到一组非常耗时的操作时，需要经常调用之，防止总线
	杀死进程
	@return 运行返回-1，运行返回0
	*/
	static int check_parent(void);
	/**
	@brief 向总线反注册进程
	*/
	static void end_proc(void);

	static void putdbg(int type,char *format,...);

};

/**
@brief 网络状态类

仅实现了当前需要的几个接口，如果有新的需要，请自行添加
*/
class PLATFORM_WRAPPER_EXPORT Net_stat
{
public:
	/**
	@brief 获取本机主机号
	@return 本机主机号
	*/
	static int get_host_no(void);
	/**
	@brief 根据节点名获取节点号
	@param in const char* node_name 节点名
	@return 主机号
	*/
	static int get_host_no(char* node_name);
	/**
	@brief 根据节点名获取主机名
	@param in const char* node_name 节点名
	@return 主机名
	*/
	static char* get_host_name(char* node_name);
	/**
	@brief 获取所有前置节点的节点号
	@param out int* nodes 写节点号的内存
	@param in int lenth 内存的长度
	@return 节点的数量
	*/
	static int get_fe_nodes(int* nodes, const int&lenth);
	/**
	@brief 获取所有I区前置节点的主机名
	@param out char** hosts 写主机名的内存
	@param in int lenth 内存的长度
	@return 主机的数量
	*/
	static int get_fe_hosts(char** hosts, const int&lenth);
	/**
	@brief 获取主机在前置节点中的序号
	@param in int host_no 主机序号
	@return 获取成功返回序号，失败返回-1
	*/
	static int get_fe_host_index(int host_no);
	/**
	@brief 获取主机状态
	@param in int host_no 主机序号
	@return 状态正常返回0，退出返回-1
	*/
	static int get_host_stat(int host_no);
	/**
	@brief 获取节点名
	@return 节点名
	*/
	static char* get_node_name(void);

	/**
	@brief 获取节点名
	@param in int host_no 主机序号
	@return 节点名
	*/
	static char* get_node_name(int host_no);

	/**
	@brief 获取是否主前置
	@param in int host_no 主机序号
	@return 是主机返回0，不是返回-1
	*/
	static int get_host_master_flag(int host_no);

	/**
	@brief 获取主机地址
	@param in int host_no 主机序号
	*/
	static void get_host_addr(int host_no, struct in_addr* addr);

	/**
	@brief 获取本机是否三区采集前置
	@return 是三区采集前置1，不是返回0
	*/
	static int get_PUBFEP_flag( );

};

/**
@brief 数据库接口类

仅实现了SQL方式读取的接口

一次批量操作，如一次前置的加载过程，需要一次open、若干次select和一次
close组成

注意该接口的几个函数都是线程不安全的，因为目前没有多线程加载的需要
*/
class PLATFORM_WRAPPER_EXPORT Db_connector
{
public:
	/**
	@brief 打开数据库
	@return 成功返回0，失败返回-1
	*/
	static int open(void);
	/**
	@brief 打开数据库
	@return 成功返回行数，失败返回-1
	*/
	static int select(const char* sql, void**result_pt);
	/**
	@brief 关闭数据库
	*/
	static void close(void);
};

/**
@brief 前置邮件类
*/
class PLATFORM_WRAPPER_EXPORT Fe_mail
{
public:
	/**
	@brief 接收邮件
	@param out void** mail 邮件缓冲区
	@param out int* lenth 邮件的长度
	@return 成功返回0，失败返回-1
	*/
	static int receive(void** mail, int* lenth);
	/**
	@brief 发送邮件到所有scada节点
	@param in void* mail 邮件缓冲区
	@param in int* lenth 邮件的长度
	@return 成功返回0，失败返回-1
	*/
	static int send_to_scada(void* mail, int lenth, int level);
	/**
	@brief 发送邮件到所有前置节点
	@param in void* mail 邮件缓冲区
	@param in int* lenth 邮件的长度
	@return 成功返回0，失败返回-1
	*/
	static int send_to_fe(void* mail, int lenth);

	static int send_prot_ack( void* mail, int lenth, int rtu_no);
private:
	/**
	@brief 发送邮件
	@param in char* user_name 邮件的用户
	@param in void* mail 邮件缓冲区
	@param in int* lenth 邮件的长度
	@return 成功返回0，失败返回-1
	*/
	static int send(char* user_name, void* mail, int lenth, int level);
};

/**
@brief 前置事件类

目前仅实现了通道、终端事件，其它的如通道切换、端口值班事件
请自行扩展
*/
class PLATFORM_WRAPPER_EXPORT Fe_event
{
public:
	/**
	@brief 生成通道事项
	@param in const char* station 厂站
	@param in const char* channel_name 通道名
	@param in const char* text事件的文本
	*/
	static void make_channel_event(float& val, const char* station, const char* channel_order, 
							  const char* text);

	static void make_rtu_event(float& val, const char* station, const char* rtu_name, 
							   const char* text);

private:
	/**
	@brief 生成事项
	@param in const int& sort 事件分类
	@param in const int& type 事件类型
	@param in const char* object 对象
	@param in const char* member0 成员0
	@param in const char* member1 成员1
	@param in const char* text事件的文本
	*/
	static void make_event(const int& sort, const int& type, float& val,
					  const char* object, const char* member0, 
					  const char* member1, const char* text);
};

/**
@brief 前置SCADA服务类
*/
class PLATFORM_WRAPPER_EXPORT Fe_scasrv
{
public:
	/**
	@brief FEP服务器发往SCADA服务器的遥控返校信息
	@param in int rtuno 终端名
	@param in int ykno 遥控号
	@param in int yk_ret 遥控反校结果，正确为0，错误为-1
	@param in const char* msg 遥控反校描述
	*/
	static int fe_send_ykret(int rtuno, int ykno, int yk_ret, const char* msg);

	/**
	@brief FEP服务器发往SCADA服务器的电力事项邮件
	@param in FE_DATA_EVENT_NODE* 电力事项vector的v[0]的指针
	@param in int eventnum 事项个数
	*/
	static int fe_send_electric_event(const FE_DATA_EVENT_NODE* tmpEventData, int eventnum);

	/**
	@brief FEP服务器发往SCADA服务器的电力事项邮件
	@param in FE_DATA_EVENT_NODE* 电力事项vector的v[0]的指针
	@param in int eventnum 事项个数
	*/
	static int fe_send_prot_event( void* mail);

private:

};

/**
@brief 前置网络接口类
*/
class PLATFORM_WRAPPER_EXPORT Fe_net_control
{
public:
	/**
	@brief FES前置系统和gps对时
	成功返回0，错误返回-1
	*/
	static int check_time(int year,int month,int day,int hour,int minute,int second,int milisecond);

private:

};

#endif






