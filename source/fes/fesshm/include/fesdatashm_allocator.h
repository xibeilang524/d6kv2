/**
@file fesdatashm_allocator.h
@brief 前置生数据共享内存 分配头文件

提供前置生数据共享内存的申请、初始化，数据设置

@author zhoujian 
@version 1.0.0
@date 2013-11-18
*/

#include "fes/fesdatashm.h"

/**
@brief 前置生数据共享内存分配类

提供了前置生数据共享内存的回收，在申请回收的时候不会移动其它通道的数据
除非空间不够，所以此处空间都设置的很大

*/
class FES_DATA_EXPORT Fes_data_shm_allocator : public Fes_data_shm
{
public:
	/**
	@brief 获取实例,单实例访问
	@return Fes_data_shm_allocator实例
	*/
	static Fes_data_shm_allocator& get();

	/**
	@brief 申请通道的生数据空间
	@param in const int& channel 通道号
	*/
	int alloc_channel_data(const int& channel);
	/**
	@brief 回收通道的生数据空间
	*/
	int recycle_channel_data(const int& channel);
    /**
	@brief 设置参数区的值
	*/
	int set_yc_para(const int& channel, void *data, int len);
	int set_yx_para(const int& channel, void *data, int len);
	int set_kwh_para(const int& channel, void *data, int len);

	~Fes_data_shm_allocator();

protected:
	/**
	@brief 初始化遥测相关的各项数据
	*/
	void set_yc_default(const int& channel);
	/**
	@brief 初始化遥信相关的各项数据
	*/
	void set_yx_default(const int& channel);
	/**
	@brief 初始化电度相关的各项数据
	*/
	void set_kwh_default(const int& channel);
	/**
	@brief 设置遥测值
	@para in data_no 为通道中的偏移地址，即对应遥测点号

	同时进行工程计算，如果time为NULL，说明是不带时标的数据，需要函数计算时间
	最后把生成的FE_DATA_MSG_NODE写入全局缓存（这个暂时不用实现）
	*/
	void set_yc_value(const int& channel, const int& data_no, const float& value,
					  const unsigned char& quality, FETIME* time = NULL);
	/**
	@brief 设置遥信值
	@para in data_no 为通道中的偏移地址，即对应遥信点号

	同时进行工程计算，如果time为NULL，说明是不带时标的数据，需要函数计算时间
	最后把生成的FE_DATA_MSG_NODE写入全局缓存（这个暂时不用实现）
	*/
	void set_yx_value(const int& channel, const int& data_no, const unsigned char& value,
		const unsigned char& quality, FETIME* time = NULL);
	/**
	@brief 设置电度值
	@para in data_no 为通道中的偏移地址，即对应遥脉点号

	同时进行工程计算，如果time为NULL，说明是不带时标的数据，需要函数计算时间
	最后把生成的FE_DATA_MSG_NODE写入全局缓存（这个暂时不用实现）
	*/
	void set_kwh_value(const int& channel, const int& data_no, const unsigned long& value,
		const unsigned char& quality, FETIME* time = NULL);

private:
	/**
	@brief 构造函数，初始化成员，不予显式调用
	*/
	Fes_data_shm_allocator();
	/**
	@brief 只声明，不实现
	如果不声明，类中会存在默认拷贝构造函数（逐字节拷贝），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Fes_data_shm_allocator(const Fes_data_shm_allocator&);
	/**
	@brief 只声明，不实现
	如果不声明，类中会存在默认赋值运算符（逐字节赋值），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Fes_data_shm_allocator& operator = (const Fes_data_shm_allocator&);

private:
	buddy*		m_yc_alloctor;	//遥测分配器
	buddy*		m_yx_alloctor;	//遥信分配器
	buddy*		m_kwh_alloctor;	//电度分配器

};