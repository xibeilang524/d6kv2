/**
@file CMyEFile.h
@brief E语言读写头文件
目前暂使用横表式写入
@author zhoujian 
@version 1.0.0
@date 2014-09-17
*/
#ifndef PLATFORM_E_LANG_FILE_H
#define PLATFORM_E_LANG_FILE_H 

#ifdef WIN32
#ifdef PLATFORM_E_LANG_FILE_DLL
#define	PLATFORM_E_LANG_FILE_EXPORT __declspec(dllexport) 
#else
#define	PLATFORM_E_LANG_FILE_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLATFORM_E_LANG_FILE_EXPORT
#endif

#include "fes/fertfile.h"
#include "fes/fesscdshm.h"
#include "fes/femailstruct.h"

#include <stdlib.h>

#define		CLASS_NAME_LEN	40
#define		ENTITY_NAME_LEN	64
#define		DATA_LEN		512


#define		CORSS_TABLE		1
#define		SINGLE_COL		2
#define		MULTI_COL		3

#include "stdio.h"
#include <string.h>

class PLATFORM_E_LANG_FILE_EXPORT EFILE_BaseData
{
public:
	/**
	@brief 构造函数
	@param in int input_num 元素个数
	*/
	EFILE_BaseData( int input_num );
	~EFILE_BaseData( );

	/**
	@brief 将数据存入文件
	@param in unsigned char save_type 存储方式
	*/
	virtual void save_data_to_file(unsigned char save_type);

	/**
	@brief 设置数据类的属性个数
	@param in int num 属性个数ID
	*/
// 	void	set_property_num(int num){ m_property_num = num };

	/**
	@brief 设置该数据存储的文件名
	@param in char* file_name	文件名
	*/
	void	set_file_name(char* file_name);

	/**
	@brief 数据块开始标记
	*/
	void entity_begin();

	/**
	@brief 数据块结束标记
	*/
	void	entity_end();

	/**
	@brief 加入注释
	@param in const char* pNotes 注释内容
	*/
	void add_notes(const char* pNotes);

	/**
	@brief 设置系统声明
	@param in const char* systemname 应用系统名称
	@param in float version E语言版本
	@param in const char* code 字符集
	@param in float data_version 数据版本
	*/
	void set_system_declare(const char* systemname, const char* version, const char* code, const char* data_version);

	/**
	@brief 数据块横表式存储
	*/
	virtual void save_data_cross_table( );

	/**
	@brief 数据块单列式存储
	*/
	virtual	void save_data_single_col( );

	/**
	@brief 多列式存储
	*/
	virtual void save_data_multi_col( );

	void	convert_FETIME_systime(FETIME*, int& procs);

	void	convert_systime_FETIME(FETIME* fetime, int& procs);

protected:
	int		m_property_num;
	char	class_name[CLASS_NAME_LEN];
	char	entity_name[ENTITY_NAME_LEN];
	int		data_group[512];

	int		m_entity_num;

	FertFile* m_pfertfile;		//文件读写指针
};


class PLATFORM_E_LANG_FILE_EXPORT EFILE_DATA : public EFILE_BaseData
{
public:
	EFILE_DATA(int input_num);
	~EFILE_DATA(void);

	/**
	@brief 数据块横表式存储
	*/
	virtual void save_data_cross_table( FE_REAL_DATA_NODE* );

	/**
	@brief 数据块单列式存储
	*/
	virtual	void save_data_single_col( );

	/**
	@brief 多列式存储
	*/
	virtual void save_data_multi_col( );

	virtual int read_data_cross_table(	FE_REAL_DATA_NODE* data_point );


};

class PLATFORM_E_LANG_FILE_EXPORT EFILE_SOE : public EFILE_BaseData
{
public:
	EFILE_SOE(int input_num);
	~EFILE_SOE(void);

	/**
	@brief 数据块横表式存储
	*/
	virtual void save_data_cross_table( FE_DATA_EVENT_NODE* );

	virtual int read_data_cross_table(	FE_DATA_EVENT_NODE* data_point );

};

class PLATFORM_E_LANG_FILE_EXPORT EFILE_CHANNEL : public EFILE_BaseData
{
public:
	EFILE_CHANNEL(int input_num);
	~EFILE_CHANNEL(void);

	/**
	@brief 数据块横表式存储
	*/
	virtual void save_data_cross_table( FE_CHANNEL_STAT_MAIL* );

	virtual int read_data_cross_table(	FE_CHANNEL_STAT_MAIL* data_point );

	virtual void save_event_cross_table( float channel_event_val, char* desdribe, char*  namestr);

	virtual void read_event_cross_table( float& channel_event_val, char* desdribe, char*  namestr);

};


class PLATFORM_E_LANG_FILE_EXPORT EFILE_YKCHHECK : public EFILE_BaseData
{
public:
	EFILE_YKCHHECK(int input_num);
	~EFILE_YKCHHECK(void);

	/**
	@brief 数据块横表式存储
	*/
	virtual void save_data_cross_table( FE_YK_CHECK_NODE* );

	virtual int read_data_cross_table(	FE_YK_CHECK_NODE* data_point );

};

#endif