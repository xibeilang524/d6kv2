/**
@file fesdatashm_accessor.h
@brief 前置生数据共享内存访问头文件

提供前置生数据共享内存的申请访问

@author zhoujian 
@version 1.0.0
@date 2013-11-18
*/

#include "fes/fesdatashm.h"

/**
@brief 前置生数据共享内存访问类
提供了前置生数据共享内存的访问

*/
class FES_DATA_EXPORT Fes_data_shm_accessor : public Fes_data_shm
{
public:
	/**
	@brief 获取实例，单实例访问
	@return Fes_data_shm_accessor实例
	*/
	static Fes_data_shm_accessor& get();

	/**
	@brief 返回生数据地址
	*/
	FE_DATA* fe_data() const {return m_fe_data;};


private:
	/**
	@brief 构造函数，初始化成员，不予显式调用
	*/
	Fes_data_shm_accessor();
	/**
	@brief 只声明，不实现
	如果不声明，类中会存在默认拷贝构造函数（逐字节拷贝），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Fes_data_shm_accessor(const Fes_data_shm_accessor&);
	/**
	@brief 只声明，不实现
	如果不声明，类中会存在默认赋值运算符（逐字节赋值），可能会损坏单例
	中的数据，用只声明不实现的方法覆盖默认拷贝构造函数，如果在使用中调用
	拷贝构造函数，会产生编译错误
	*/
	Fes_data_shm_accessor& operator = (const Fes_data_shm_accessor&);

};
