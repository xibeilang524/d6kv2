/**
@file fesshm.h
@brief 前置共享内存头文件

提供前置共享内存创建、访问接口

@author zhangyun 
@version 1.0.0
@date 2012-10-22
*/
#ifndef FES_SHM_H_
#define FES_SHM_H_

#ifdef WIN32
#ifdef FESSHM_DLL
#define	FESSHM_EXPORT __declspec(dllexport) 
#else
#define	FESSHM_EXPORT __declspec(dllimport) 
#endif
#else
#define	FESSHM_EXPORT
#endif

#include "fes/fesshare.h"

/**
@brief 前置共享内存访问访问类

提供了前置共享内存单例访问的接口，可以用Fes_shm::get().各种
成员 访问共享内存各个结构
*/
class FESSHM_EXPORT Fes_shm
{
	enum 
	{
		SHM_ALIGN = 16,			///<以16字节对齐
		FES_SHM_KEY = 5555666,	///<共享内存key
		FES_SHM_LEN = 0x1000000	///<共享内存长度
	};

public:
	/**
	@brief 获取实例
	@return Fes_shm实例

	@verbatim
	Fes_shm::get().fe_sys_data_ptr;
	Fes_shm::get().chan_para_ptr;
	@endverbatim
	*/
	static Fes_shm& get();

	/**
	@brief 值班机切换

	把所有old_host值班的通道变成new_host值班
	@param in const int& defalut_host 默认值班机
	@param in const int& new_host 新值班机
	*/
	void attend_host_switch(const int& old_host, const int& new_host);

	/**
	@brief 本机值班应该由本机值班的通道
	@param in const int& host_no 节点号
	*/
	void attend_self(const int& host_no);

private:
	/**
	@brief 构造函数，初始化成员，不予显式调用
	*/
	Fes_shm();

	/**
	@brief 拷贝构造函数，只声明不实现
	
	如果不声明，类中会存在默认拷贝构造函数（逐字节拷贝），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Fes_shm(const Fes_shm&);

	/**
	@brief 等于运算符，只声明不实现
	
	如果不声明，类中会存在默认赋值运算符（逐字节赋值），可能会损坏单例中的数据，
	用只声明不实现的方法覆盖默认赋值运算符，如果在使用中调用了赋值运算符，会产
	生编译错误
	*/
	Fes_shm& operator = (const Fes_shm&);

	/**
	@brief 创建前置共享内存
	
	@return 成功返回0，失败返回-1
	*/
	void create_fes_shm(void);

	//以下是实际的数据
public:
	char*			shm_addr_ptr;		///<共享内存指针
	FE_SYS_DATA*	fe_sys_data_ptr;	///<本机及其它前置节点的信息
	CHANPARA*		chan_para_ptr;		///<通道参数
	RTUPARA*		rtu_para_ptr;		///<终端参数
	CHANDATA*		chan_data_ptr;		///<通道数据 
	GPS_INFO*		gps_info_ptr;		///<GPS数据
};

#endif
