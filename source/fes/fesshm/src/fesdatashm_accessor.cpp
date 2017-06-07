/**
@file fesdatashm_accessor.h
@brief 前置生数据共享内存头文件

提供前置生数据共享内存访问

@author lujiashun 
@version 1.0.0
@date 2016-08-12
*/

#include "fes/fesdatashm_accessor.h"

Fes_data_shm_accessor::Fes_data_shm_accessor()
{

}

Fes_data_shm_accessor& Fes_data_shm_accessor::get()
{
	static Fes_data_shm_accessor instance;
	return instance;
}
