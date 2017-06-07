/**
@file config.h
@brief 配置文件处理相关类头文件
负责读写qtwave.xml配置文件内容
@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include "globaldef.h"


const QString CONFIG[] =
{
	_C2Q("ftpservername"),
	_C2Q("ftpserverport"),
	_C2Q("ftpusername"),
	_C2Q("ftpuserpassword"),
	_C2Q("localwavefilepath")
};
typedef struct tagConfigSet 
{
	QString m_ftpserver_name;	///<ftp服务器名
	QString m_ftpserver_port;	///<ftp服务器端口
	QString m_user_name;	///<用户名
	QString m_password;		///<密码
	QString m_local_wavefile_path;	///<本地录波文件路径
} CONFIG_SET;

class Config
{
public:
	Config();
	virtual ~Config();

private:
	QString m_ftpserver_name;	///<ftp服务器名
	QString m_ftpserver_port;	///<ftp服务器端口
	QString m_user_name;	///<用户名
	QString m_password;		///<密码
	QString m_local_wavefile_path;	///<本地录波文件路径
	CONFIG_SET m_config;

public:
	void set_ftpserver_name(QString servername);
	QString get_ftpserver_name();
	void set_ftpserver_port(QString serverport);
	QString get_ftpserver_port();
	void set_user_name(QString username);
	QString get_user_name();
	void set_password(QString password);
	QString get_password();
	void set_local_wavefile_path(QString wavefilepath);
	QString get_local_wavefile_path();
	
	CONFIG_SET get_config_set(void);

	bool read_config_file();
	bool write_config_file();

};


#endif