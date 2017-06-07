/**
@file channelunit.h
@brief 通道类头文件

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef CHANNELUNIT_H
#define CHANNELUNIT_H

#include <QList>

#include "comtrade.h"
/**
@brief 取出通道类
封装各种需求从DataObject中获得通道信息
*/
class Channels
{
public:
	Channels();
	~Channels();
	/**
	@brief 清空数据列表
	*/
	void clear();
	/**
	@brief 数据列表数量
	*/
	int count();
	/**
	@brief 选中数据列表数量
	*/
	int check_count();
	/**
	@brief 获取序号为index的数据
	*/
	View_data* channel(int index);
	/**
	@brief 获取序号为index的数据是否选中
	*/
	bool checked(int index);
	/**
	@brief 设置序号为index的数据是否选中
	@para in bCheck  true:checked  false:unchecked
	*/
	void set_checked(int index, bool bCheck);
	/**
	@brief 全选
	*/
	void check_all(bool bCheck = false);
	/**
	@brief 增加一个通道
	*/
	void add(Channel* obj);
	/**
	@brief 插入一个通道
	*/
	void insert(int index, Channel* obj);
	/**
	@brief 删除通道
	*/
	void Delete(int index);
	/**
	@brief 获得指定通道号的通道位置
	*/
	int Indexof(int channelNo);
	/**
	@brief 初始化
	*/
	void init_by_comtrade(Comtrade* obj, bool ana);
	
protected:
	/**
	@brief 检查CheckList中是否存在已经被删除的通道
	*/
	void reset_check_list();

private:
	 QList<View_data*> m_lst_data;	///<全部数据列表
	 QList<View_data*> m_lst_check;	///<选中数据列表
	
};

#endif