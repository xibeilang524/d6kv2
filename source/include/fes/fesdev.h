/**
@file fesdev.h
@brief 设备通讯接口库

提供TCP、UDP、串口、MOXA等设备的通用通讯接口，使用者调用

@author qianjin 
@version 1.0.0
@date 2012-10-22
*/

#ifndef _FES_DEV_H_
#define _FES_DEV_H_

#include <ace/OS.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include "ace/DEV_Addr.h"
#include <ace/DEV_Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/TTY_IO.h>

#ifdef WIN32
#ifdef FESDEV_DLL
#define	FESDEV_EXPORT __declspec(dllexport) 
#else
#define	FESDEV_EXPORT __declspec(dllimport) 
#endif
#else
#define	FESDEV_EXPORT
#endif

/**
@brief 设备类型定义
*/
enum DEV_TYPE
{
	NET_DEV = 0,
	SERIAL_DEV,
	TCP_CLINET_DEV,
	TCP_SERVER_DEV,
	UDP_DEV
};

/**
@brief 设备操作基类

Dev_base类提供的打开、关闭、读写接口

Dev_base仅仅定义了通讯设备的通用接口，接口的实现由其子类根据

自身的设备类型完成。
*/
class FESDEV_EXPORT Dev_base
{
public:
	virtual ~Dev_base();
	/**
	@brief 打开设备
	@return 成功返回0，失败返回-1
	*/
	virtual int open_dev(void) = 0;
	/**
	@brief 关闭设备
	*/
	virtual void close_dev(void) = 0;
	/**
	@brief 读设备
	@param out unsigned char* buf 接收缓冲区
	@param in int len 要读取的字节数,
	@return 读到的字节数，不可读时返回0，失败返回-1
	*/
	virtual int read_dev(unsigned char* buf, int len) = 0;
	/**
	@brief 写设备
	@param in unsigned char* buf 缓冲区
	@param in int len 要写入的字节数
	@return 成功写入的字节数，不可写时返回0，失败返回-1
	*/
	virtual int write_dev(unsigned char* buf, int len) = 0;
	/**
	@brief 获取状态
	@return 打开状态返回0，关闭状态返回-1
	*/
	virtual int get_stat(void) = 0;

	/**
	@brief 获取错误号描述
	@return 错误号描述
	*/
	char* get_error(void);

	
protected:
	int valid_addr_num;		//有效地址的个数
	int addr_index;			//当前使用地址的索引
};

class FESDEV_EXPORT Net_dev : public Dev_base
{
public:
	enum
	{
		NET_NUM = 2,
		DEVICE_TX_BUF_SIZE = 16384,
		DEVICE_RX_BUF_SIZE = 16384,
		SOCK_TIMEOUT = 5,
		CLOSE_RETRY = 5
	};

public:
	Net_dev(const char* dev1, const int& port1, const char* dev2, const int& port2);

	~Net_dev();
protected:
	ACE_INET_Addr addr[NET_NUM];
};

/**
@brief Udp接口类
*/
class FESDEV_EXPORT Udp_dev: public Net_dev
{
	
public:
	/**
	@brief 构造函数
	@param in const char* dev1 地址1
	@param in const int& port1 地址1的端口
	@param in const char* dev2 = NULL 地址2
	@param in const const int& port2 地址2的端口
	*/
	Udp_dev(const char* local_dev, const int& local_port, const char* dev1, 
		    const int& port1, const char* dev2, const int& port2);
	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len); 
	virtual int get_stat(void);		//add by chris

private:
	ACE_INET_Addr local_addr;
	ACE_SOCK_Dgram peer;

};

/**
@brief Tcp客户端接口类

*/
class FESDEV_EXPORT Tcp_client: public Net_dev
{
public:
	/**
	@brief 构造函数
	@param in const char* dev1 地址1
	@param in const int& port1 地址1的端口
	@param in const char* dev2 = NULL 地址2
	@param in const const int& port2 地址2的端口
	*/
	Tcp_client(const char* dev1, const int& port1, const char* dev2, const int& port2);
	virtual ~Tcp_client();

	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len); 
	virtual int get_stat(void);

protected:
	/**
	@brief 设置socket的参数
	*/
	void set_fd_opt(void) const;

protected:
	ACE_SOCK_Stream peer;
};

/**
@brief Tcp服务端接口类

*/
class FESDEV_EXPORT Tcp_server: public Tcp_client
{
public:
	/**
	@brief 构造函数

	@param in const char* local_ip 本地监听的IP地址，如果为NULL，则监听本地所有IP
	@param in const int& local_port 本地监听的端口号
	@param in const char* peer_ip1 = NULL 允许连接的对端IP地址1
	@param in const char* peer_ip2 = NULL 允许连接的对端IP地址2
	*/
	Tcp_server(const char* local_ip, const int& local_port, const char* peer_ip1 = NULL, const char* peer_ip2 = NULL);
	~Tcp_server();

	virtual int open_dev(void);
	/**
	@brief 监听端口
	@return 成功返回0，失败返回-1
	*/
	int listen_port(void);

	int close_listen(void);
private:
	ACE_SOCK_Acceptor acceptor;
	ACE_INET_Addr local_addr;
	bool listen_flag ;
};

/**
@brief Moxa串口服务器接口类
*/
class FESDEV_EXPORT Moxa_dev: public Tcp_client
{
public:
	Moxa_dev(const char* dev1, const int& port1, const char* dev2, const int& port2);
};

/**
@brief 串口接口类

ACE串口的使用参考ACE_wrappers\examples\IPC_SAP\DEV_SAP
*/
class FESDEV_EXPORT Serial: public Dev_base
{
public:
	enum
	{
		DEV_NUM = 2,
		READ_TIME_OUT = 10 * 1000, //读超时时间10秒
		CLOSE_RETRY = 5
	};
public:
	Serial(const char* dev1, const char* dev2, const int& band_rate, 
		   const int& check_bit, const int& stop_bit, const int& data_bit, const int& flow);
	virtual int open_dev(void);
	virtual void close_dev(void);
	virtual int read_dev(unsigned char* buf, int len);
	virtual int write_dev(unsigned char* buf, int len);
	virtual int get_stat(void);
private:
	ACE_TTY_IO::Serial_Params param;
	ACE_DEV_Connector connector;
	ACE_TTY_IO dev;
	ACE_DEV_Addr dev_addr[DEV_NUM];
};
#endif