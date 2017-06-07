/**
@file Mirror_Proc.h
@brief III区实时数据处理线程头文件

III区实时数据处理

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/
#ifndef MIRROR_PROC_H_
#define MIRROR_PROC_H_

#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

#include "fes/platformwrapper.h"
#include "fes/femailstruct.h"

#define  MAX_FILE_DATA_NUM 4096

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data())) //  [1/15/2013 ljs]
#define g_Q(str) (QString::fromLocal8Bit(str))

/**
@brief III区实时数据处理线程类

该类读取III区前置发送至一区的实时数据文件，提取出
数据后，刷入共享内存
*/
	/**
	@brief 创建前置共享内存
	@return 成功返回0，失败返回-1
	*/
class  Mirror_Proc: public QThread
{
public:
	Mirror_Proc();
	~Mirror_Proc( );
	/**
	@brief 线程主体文件，循环读取数据文件，并写入共享内存
	*/
	void run(void); //线程主体，用于将数据缓存存为文件

	void stop();
private:

	/**
	@brief 扫描指定文件夹下是否有新的数据文件
	@return 成功返回1，失败返回0
	*/
	bool scan_datafile(QStringList& file_list);

	/**
	@brief 读取文件建中的数据并存入list
	*/
	void convert_file_to_list(QString filename);

	void send_data_to_scada( int data_num );

	void send_soe_to_scada( int soe_num );

	void send_channel_to_scada( int channel_num );

	void send_ykcheck_to_scada( int ykcheck_num );


private:
	FE_REAL_DATA_NODE*		m_data;
	FE_DATA_EVENT_NODE*		m_soe;
	FE_CHANNEL_STAT_MAIL*	m_channel;
	FE_YK_CHECK_NODE*		m_ykcheck;

	FE_MAIL_STRUCT* m_data_mail_struct;
	FE_MAIL_STRUCT* m_stat_mail_struct;

	QDateTime	m_last_modify_time;
};

#endif
