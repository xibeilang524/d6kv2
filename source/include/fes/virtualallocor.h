/**
@file virtualallocor.h
@brief 内存虚拟分配器头文件

@author zhangyun 
@version 1.0.0
@date 2013-11-01
*/
#ifndef FES_VIRTUAL_ALLOCTOR_H_
#define FES_VIRTUAL_ALLOCTOR_H_

/**
@brief 虚拟分配器

在一块连续的内存空间中按需分配所需的内存，只分配偏移量，不实际分配内存
当前只是简单的分配、回收，不设计任何分配回收算法，有可能在很多次不同大
小的分配和回收后，会形成碎片导致有空间而无法分配，只好扩展的情况。为了
实现简单暂时忽略，暂时不去研究buddy之类的算法。
*/
class Virtual_allocor
{
	typedef map<int, int>::iterator MAP_IT;
	typedef map<int, int>::reverse_iterator MAP_RIT;

	enum
	{
		DEFAULT_ALLOCOR_SIZE = 0x40000
	};
public:
	Virtual_allocor();
	explicit Virtual_allocor(const int& in_size);
protected:

private:
	/**
	@brief 虚拟申请内存

	成功返回序号，失败返回-1
	*/
	int alloc(const int& lenth);

	/**
	@brief 虚拟回收内存

	成功返回0，失败返回-1
	*/
	int recycle(const int& position);

	/**
	@brief 改变长度

	成功返回0，失败返回-1
	*/
	int resize(void);
private:
	int m_size;
	
	map<int, int>	m_used_mem;	//已使用的内存的链表，<起始元素序号, 长度>
	map<int, int>	m_free_mem;	//未使用的内存的链表，<起始元素序号, 长度>
};



#endif
