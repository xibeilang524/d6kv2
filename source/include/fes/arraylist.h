/**
@file arraylist.h
@brief 数组链表模板头文件
@author zhangyun 
@version 1.0.0
@date 2012-10-31
*/
#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <vector>
#include <list>
#include <algorithm>
using namespace std;

/**
@brief 数组链表模板类

提供近常数时间复杂度的元素批量插入、删除、获取接口，非线程安全。
插入的数据会依次放入数组，当数组达到长度上限时，则写入链表中下个
数组，当链表中所有数组已满时，插入新的数组。删除以数组为单位，
直接清空首数组，然后把首数组移动到链表尾部。
*/

template <typename T>
class Array_list
{
	enum
	{
		DEFAULT_ARRAY_SIZE = 4096,
		DEFAULT_LIST_SIZE = 4
	};
public:
	/**
	@brief 构造函数
	@param in const int& array_size 单个数组的长度
	@param in const int& array_size 链表的长度
	*/
	explicit Array_list(const int& array_size, const int& list_size)
	:ARRAY_SIZE(array_size > 0 ? array_size : DEFAULT_ARRAY_SIZE),
	LIST_SIZE(list_size > 0 ? list_size : DEFAULT_LIST_SIZE)
	{
		for (int i = 0; i < list_size; i++)
		{
			append_an_array();
		}
	};

	/**
	@brief 拷贝构造函数 覆盖默认拷贝构造函数，不实现，防止调用
	*/
	Array_list(const Array_list&);

	/**
	@brief 赋值操作符 覆盖默认赋值操作符，不实现，防止调用
	*/
	Array_list operator = (const Array_list&);

	/**
	@brief 析构函数
	*/
	~Array_list()
	{
		//逐个删除数组
		typename std::list<vector<T>*>::iterator end = m_array_list.end();

		for (typename std::list<vector<T>*>::iterator it = m_array_list.begin(); it != end; it++)
		{
			delete *it;
			*it = NULL;
		}

		m_array_list.clear();
	};

	/**
	@brief 判断是否为空
	*/
	bool empty(void) const
	{
		return m_array_list.empty() ? true : m_array_list.front()->empty();
	};

	/**
	@brief 获取首个数组的指针
	@return 成功返回首个数组的指针，失败返回NULL
	*/
	T* get_head_array(void) const
	{
		//vector可以通过&v[0]取首元素的地址
		return empty() ? NULL : &(*(m_array_list.front()))[0];
	};

	/**
	@brief 获取首个数组的长度
	@return 成功返回首个数组的长度，失败返回-1
	*/
	int	get_head_size(void) const
	{
		return empty() ? -1 : m_array_list.front()->size();
	};

	/**
	@brief 清空并移动首个数组到链表尾部，使链表头的数组始终有数据
	*/
	void clear_head(void)
	{
		if (!empty())
		{
			m_array_list.front()->clear();

			//第一个数组还没有满的情况下，没有必要移动数组到链表尾部，只需要移动array迭代器到首个array的头部
			if (m_list_it == m_array_list.begin())
			{
				m_array_it = (*m_array_list.begin())->begin();
			}
			else
			{
				//如果第一个数组已经满了，对它的移动不会影响两个迭代器中的任意一个
				m_array_list.push_back(m_array_list.front());
				m_array_list.pop_front();
			}
		}
	};

	/**
	@brief 插入数据，如果没有剩余空间则自动扩充
	@param in const vector<T>& buf 输入的数据容器
	*/
	void insert_data(vector<T>& source)
	{
		if (source.empty())
		{
			return;
		}

		typename vector<T>::iterator it = source.begin();
		typename vector<T>::iterator end = source.end();

		while (it != end)
		{
			int data_left = end - it;
			int array_left = (*m_list_it)->empty() ? ARRAY_SIZE : ARRAY_SIZE - (*m_list_it)->size();
			int data_insert = min(data_left, array_left);

			//插入数据
			(*m_list_it)->insert(m_array_it, it, it + data_insert);
			//移动数据源的迭代器
			advance(it, data_insert);

			//插入导致array迭代器失效，重新设置之
			m_array_it = (*m_list_it)->begin() += ((*m_list_it)->size()); 

			//数组满了
			if ((*m_list_it)->size() == ARRAY_SIZE)
			{
				m_list_it++;

				//队列也满了
				if (m_list_it == m_array_list.end())
				{
					append_an_array();
				}
				else
				{
					m_array_it = (*m_list_it)->begin();
				}
			}
		}	
	};

	/**
	@brief 插入数据，如果没有剩余空间则自动扩充
	@param in T 输入的数据
	@return 成功返回0，失败返回-1
	*/
	int insert_data(T data)
	{
	
// 		vector<T>* element_ptr = *m_list_it;
// 		if (!element_ptr)		//判断正在插入的vector指针否有效
// 		{
// 			return -1;
// 		}
// 		else
// 		{
// 			element_ptr->push_back(data);		//直接插入，若不够的话在下面做增
// 		}
// 
// 		//此处list迭代器不应该失效，但是VC对迭代器的处理不好，只能重设
// 		bool empty_flag = empty();
// 		m_list_it = m_array_list.begin();
// 		if (!empty_flag)
// 		{
// 			advance(m_list_it, m_array_list.size() - 1);
// 		}
// 
// 		m_array_it = (*m_list_it)->begin();
// 
// 		//数组满了
// 		if ((*m_list_it)->size() == ARRAY_SIZE)
// 		{
// 			m_list_it++;
// 
// 			//队列也满了
// 			if (m_list_it == m_array_list.end())
// 			{
// 				append_an_array();
// 			}
// 			else
// 			{
// 				m_array_it = (*m_list_it)->begin();
// 			}
// 		}

		vector<T> vector_insert;
		vector_insert.push_back(data);
		insert_data(vector_insert);


  		return 0;
	};

private:
	/**
	@brief 新增一个数组到链表中
	*/
	void append_an_array(void)
	{
		bool empty_flag = empty();

		vector<T>* element_ptr = new vector<T>;
		element_ptr->reserve(ARRAY_SIZE);
		m_array_list.push_back(element_ptr);

		//此处list迭代器不应该失效，但是VC对迭代器的处理不好，只能重设
		m_list_it = m_array_list.begin();

		if (!empty_flag)
		{
			advance(m_list_it, m_array_list.size() - 1);
		}
		
		m_array_it = (*m_list_it)->begin();
	};

private:
	typename list<vector<T>* >::iterator	m_list_it;	///<下一次插入时的链表迭代器
	typename vector<T>::iterator			m_array_it;	///<下一次插入时的数组迭代器

	const int			ARRAY_SIZE;	///<数组大小
	const int			LIST_SIZE;	///链表大小
	list<vector<T>* >	m_array_list;///<链表数组

};

#endif







