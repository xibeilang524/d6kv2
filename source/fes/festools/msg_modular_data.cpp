#include "msg_modular_data.h"
#include "string.h"
#include "stdio.h"

#include <QtCore/QCoreApplication>
#include <QString>
#include <QLatin1String>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QMutableListIterator>

msg_modular_data::msg_modular_data()
{
	m_pData = NULL;
	//m_exData = NULL;
	m_pKey_flag = NULL;
	m_length = 0;
	m_type = -1;
	m_exchange_str = "\0";

	m_cdt_frame_cnt = 0;
	m_protocol_type = "";   
	//checkbox_type = 0;
}

msg_modular_data::~msg_modular_data()
{
	if(m_pData)
		delete[] m_pData;

	if(m_pKey_flag)
		delete[] m_pKey_flag;
}

//报文存储
bool msg_modular_data::set_data(char* pData, int nLength)
{
	if(m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_length = 0;
	}

	if(m_pKey_flag)
	{
		delete[] m_pKey_flag;
		m_pKey_flag = NULL;
	}

	m_pData = new char[nLength];
	if(!m_pData)
	{
		return false;
	}

	m_pKey_flag = new uchar[nLength];
	if(!m_pKey_flag)
	{
		delete[] m_pData;
		m_pData = NULL;
		return false;
	}
	
	//数据存储，拷贝到m_pData数组中
	memcpy(m_pData, pData, nLength); 
	m_length = nLength;

	memset(m_pKey_flag, 0, nLength*sizeof(uchar));

	return true;
}

void msg_modular_data::set_type( int nType )
{
	m_type = nType;
}

void msg_modular_data::set_protocol_type( QString qprotocol )
{
	m_protocol_type = qprotocol;
}

void msg_modular_data::set_time( QDateTime datetime )
{
	m_msg_time = datetime;
}

QString msg_modular_data::get_protocol_type()
{
	return m_protocol_type;
}

int msg_modular_data::get_cdt_frame_cnt()
{
	return m_cdt_frame_cnt;
}

char* msg_modular_data::get_data()
{
	return m_pData;
}

int msg_modular_data::get_data_length()
{
	return m_length;
}

int msg_modular_data::get_type()
{
	return m_type;
}

uchar* msg_modular_data::get_key_flag()
{
	return m_pKey_flag;
}

int msg_modular_data::get_exdata_len()
{
	return m_exchange_str.length();
}

QString msg_modular_data::get_exchange_data()
{
	return m_exchange_str;
}

QString msg_modular_data::get_time_str()
{
	return m_msg_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
}
