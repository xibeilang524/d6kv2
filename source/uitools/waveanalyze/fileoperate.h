/**
@file fileoperate.h
@brief comtrade文件操作头文件
可以连接远程ftp，下载装置上comtrade录波文件
@author lujiashun 
@version 1.0.0
@date 2016-11-24
*/
#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include "comtrade.h"

#include <qstring.h>

class File_operate
{
public:
	File_operate(Comtrade* data) { m_pComtrade = data; }

private:
	QString		m_strFileName;
	Comtrade*	m_pComtrade;

protected:
	/**
	@brief 读取comtrade录波文件内容
	*/
	bool	read_comtrade();
	/**
	@brief 读取comtrade录波配置文件内容
	即：filename.cfg文件
	*/
	bool	read_cfg(const QString& cfg);
	/**
	@brief 设置模拟量通道参数
	*/
	void	set_analog(const QString& anaStr, Analog* ana);
	/**
	@brief 设置开关量通道参数
	*/
	void	set_digtal(const QString& digStr, Digtal* dig);
	/**
	@brief 获取采样信息
	*/	
	SAMPLE	get_sample_info(const QString& samStr);
	/**
	@brief 检查文件大小是否正确
	*/	
	bool	check_data_file(int size, int& nRec, int& nStart);
	/**
	@brief 读取录波数据文件 filename.dat
	数据类型为ascii文本型
	*/	
	bool	read_ascii(const QString& dat);
	/**
	@brief 读取录波数据文件 filename.dat
	数据类型为binary 二进制
	*/	
	bool	read_binary(const QString& dat, int& nError);

	/**
	@brief 保存录波配置文件 filename.cfg
	*/	
	bool	save_cfg_file(const QString& cfgFile);
	/**
	@brief 保存录波数据文件 filename.dat
	*/	
	bool	save_dat_file(const QString& datFile);

	short	BigToLitter(ushort* p, int pos)
	{
		ushort ss = p[pos];
		ss = (ss << 8) | (ss >> 8);
		return (short)ss;
	}

public:
	bool	load_file(const QString& fileName, int fileType);
	bool	save_as_comtrade(const QString& fileName);
	/**
	@brief 加载配置文件
	*/	
	bool	load_config(const QString& fileName);
};

#endif
