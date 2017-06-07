/**
@file msg_modular_data.h
@brief 报文存储

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGMODULARDATA_H_
#define MSGMODULARDATA_H_

#include <QString>
#include <QList>
#include <QDateTime>

#include "fes/fesscdshm.h"

//报文存储类
class msg_modular_data
{
public:
	msg_modular_data() ;
	~msg_modular_data() ;

public:
	bool set_data( char* pData, int nLength ) ;
	void set_type( int nType ) ;
	void set_protocol_type( QString qprotocol ) ;
	void set_time( QDateTime datetime ) ;
	int get_cdt_frame_cnt() ;
	int get_data_length() ;  //获得收发报文长度
	int get_type() ;
	int get_exdata_len() ;
	uchar* get_key_flag() ;
	char* get_data() ;  //取用出存储的字符串供显示报文
	QString get_protocol_type() ;
	QString get_exchange_data() ;  //返回存储的字符串，便于显示
	QString get_time_str() ;

private:
	char*					m_pData ;
	uchar*					m_pKey_flag ;
	int						m_length ;
	int						m_type ;            //判断报文是上下行
	int						m_cdt_frame_cnt ;  //用"*"分割CDT字符串，用以显示循环长度
	QString				m_exchange_str ;   //char型转换过后存储到QString里
	QString				m_protocol_type ; 
	QDateTime			m_msg_time ;
} ;

#endif