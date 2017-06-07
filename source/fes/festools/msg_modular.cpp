#include "msg_modular.h"
#include "msg_modular_data.h"
#include "festools_globaldef.h"

#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

float g_LogFileSize = 10.0 ;
int  g_LogFileNum = 5 ;

msg_modular::msg_modular()
{
	m_nTotalBytes = 0 ;
	m_bPause = false ;
	m_max_bytes = NETVIEW_MAXBYTE ;
	m_bSaveToFile = false ;
	m_saveFile = NULL ;
}

msg_modular::~msg_modular()
{
	clear_list_data() ;
}

void msg_modular::clear_list_data()
{
	while( !m_List_data.isEmpty() )
	{
		delete m_List_data.takeFirst() ;
	}

	m_nTotalBytes = 0 ;
}

//		m_Modula->AddData( buf, sendNum, 2 ) ;
bool msg_modular::add_data( char* pData, int nLength, int nType, QString qprotocol, QDateTime save_begintime, QDateTime save_endtime,\
						 int save_checkbox, QString channel_name, FETIME* msgtime )
{
	if( get_save_to_file() )
		trace_to_file( pData,nLength ) ;

	int checkbox_state = save_checkbox ;
		
	if( m_bPause )
		return false ;

	QDateTime current_msgtime ;
	current_msgtime.setDate( QDate( msgtime->year+1900,msgtime->month,msgtime->day ) ) ;
	current_msgtime.setTime( QTime( msgtime->hour,msgtime->minute,msgtime->second,msgtime->milisecond ) ) ;

	//定义并初始化数据存储类
	msg_modular_data* pDataObj ;
	pDataObj = new msg_modular_data() ;

	if( !pDataObj )
	{
		return false ;
	}

	pDataObj->set_protocol_type( qprotocol ) ;  //规约类型
	pDataObj->set_type( nType ) ;    //收发类型
	pDataObj->set_time( current_msgtime ) ;

	if( !pDataObj->set_data( pData, nLength ) )
	{
		delete pDataObj ;
		return false ;
	}

	{
		m_List_data.push_back( pDataObj ) ; //public m_ListData		
		m_nTotalBytes += nLength ;
	}

	if ( pDataObj )
	{
		if ( ( save_checkbox == 1 ) && ( save_begintime < save_endtime ) )
		{
			char file_path[ 256 ] ;
			memset( file_path,0,256 ) ;
			sprintf( file_path,"%s/message/",getenv( "NBENV" ) ) ;
			QDir dir ; 
			dir.mkpath( file_path ) ;  //必须和后面路径一致

			QString file_name = _C2Q( file_path ) ;
			file_name += channel_name ;
			file_name += "_" ;
			file_name += save_begintime.toString( "hh_mm_ss_zzz" ) ;
			file_name += "-" ;
			file_name += save_endtime.toString( "hh_mm_ss_zzz" ) ;
			file_name += ".txt" ;

			QFile file( file_name ) ; //创建不成功

			if ( !file.open( QFile::Append | QFile::WriteOnly | QFile::Text ) )
			{
				return false ;
			}
			QTextStream out( &file ) ;		
	
			if ( file.exists() )
			{
				if ( ( current_msgtime >= m_save_begintime ) && ( current_msgtime <= m_save_endtime ) )
				{
					auto_save_message( pDataObj, save_begintime, save_endtime, out ) ;
				}
				else
					file.close() ;
			}
			
		}
	}

	while( m_nTotalBytes > m_max_bytes )		//缓存过大时，删除4帧报文
		remove_data( 4 ) ;

	return true ;
}

void msg_modular::auto_save_message( msg_modular_data* pModula_data, QDateTime save_begintime, QDateTime save_endtime, QTextStream &out )
{
	char filename[256] ;
	QString strTitle ;
	
	if( !pModula_data )
		return ;

	switch( pModula_data->get_type() )
	{
		case 1:
			strTitle.sprintf( "RECV( %d )", pModula_data->get_data_length() ) ;
			strTitle += _C2Q( "    当前时间:" ) ;
			strTitle += pModula_data->get_time_str() ;
			strTitle += _C2Q( "\n" ) ;
		break ;	
		case 2:
			strTitle.sprintf( "SEND( %d )", pModula_data->get_data_length() ) ;
			strTitle += _C2Q( "    当前时间:" ) ;
			strTitle += pModula_data->get_time_str() ;
			strTitle += _C2Q( "\n" ) ;
		break ;
		default:
			strTitle.sprintf( "UNKNOW( %d )\n", pModula_data->get_data_length() ) ;
		break ;
	}

	if ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() )
	{
		out << strTitle ;
	}

	if ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() )
	{
		QString strTemp ;
		int nCount = 0 ;

		while( nCount < pModula_data->get_data_length() )
		{
			for( int i = 0 ; i < 16 ; i++ )
			{
				if( nCount + i < pModula_data->get_data_length() )
				{
					strTemp.sprintf( "%02X ", ( unsigned char )( *( pModula_data->get_data()+nCount+i ) ) ) ;
					out << strTemp ;
				}
			}
			out << QString( "  " ) ;
			out << QString( "\n" ) ;
			nCount += 16 ;
		}
	}
}

void msg_modular::remove_data( int nTotal )
{
	while( nTotal )
	{
		if( m_List_data.isEmpty() )
			break ;

		msg_modular_data* pDataObj = m_List_data.takeFirst() ;
		m_nTotalBytes -= pDataObj->get_data_length() ;
		delete pDataObj ;
		nTotal-- ;
	}
}

#define TIMEDATE_FORMAT_MS "yyyy-MM-dd hh:mm:ss.zzz"

void msg_modular::trace_to_file( char* pdata,int nlength )
{
	QDateTime timestamp = QDateTime::currentDateTime() ;
	if ( timestamp > get_end_time() ) return ;
	
	if ( !m_saveFile )
	{
		QString filePath = get_save_path() ;
		m_saveFile = new QFile( filePath ) ;
		if( !m_saveFile->open( QFile::Append | QFile::WriteOnly | QFile::Text ) )
		{
			delete m_saveFile ;
			m_saveFile = NULL ;
			return ;
		}
	}
	
	#ifndef ACE_WIN32
	if ( m_saveFile->size() > int( g_LogFileSize*1024*1024 ) ) //g_LogFileSize Mbyte
	{
		QString str = "" ;
		m_saveFile->close() ;
		if ( g_LogFileNum > 100 )
			g_LogFileNum = 100 ;
		int j,k ;
		QString strj,strk ;
		for ( int i=0 ; i<=( g_LogFileNum-2 ) ; i++ )
		{	
			 j = g_LogFileNum - i - 2 ;
			 k = g_LogFileNum - i - 1 ;
			 strj.sprintf( "%d",j ) ;
			 strk.sprintf( "%d",k ) ;
			if ( j != 0 )
				str+= "mv '" + get_save_path() + ".old" + strj + "' '" + get_save_path() + ".old" + strk +"' ;" ;
			else
				str+= "mv '" + get_save_path() + "' '" + get_save_path() + ".old1' ;" ;
		}
		/*str = "mv '" + GetSavePath() + ".old4' '" + GetSavePath() + ".old5' ;" ;
		str+= "mv '" + GetSavePath() + ".old3' '" + GetSavePath() + ".old4' ;" ;
		str+= "mv '" + GetSavePath() + ".old2' '" + GetSavePath() + ".old3' ;" ;
		str+= "mv '" + GetSavePath() + ".old1' '" + GetSavePath() + ".old2' ;" ;
		str+= "mv '" + GetSavePath() + "' '" + GetSavePath() + ".old1' ;" ;*/
		if ( system( ( const char* )( str.toStdString().c_str() ) ) != -1 )
		{	
			if( !m_saveFile->open( QFile::Append | QFile::WriteOnly | QFile::Text ) )
			{
				delete m_saveFile ;
				m_saveFile = NULL ;
				return ;
			}
		}
		else
		{	
			return ;
		}
	}
	#endif
	
	
	QTextStream out( m_saveFile ) ;
	QString timestr = timestamp.toString( TIMEDATE_FORMAT_MS ) ;
	out<<timestr ;
	out<<QString( " " ) ;
	char* buffer = new char[nlength + 1 ] ;
	memset( buffer, '\0', nlength + 1 ) ;
	memcpy( buffer, pdata, nlength ) ;
	out<<buffer ;
	out<<QString( "\n" ) ;
	delete []buffer ;
}

void msg_modular::set_save_to_file( bool b )
{
	m_bSaveToFile = b ;
	if ( b )
	{
		if( !m_saveFile )
		{
			QString filePath = get_save_path() ;
			m_saveFile = new QFile( filePath ) ;
			if( !m_saveFile->open( QFile::Append | QFile::WriteOnly | QFile::Text ) )
			{
				delete m_saveFile ;
				m_saveFile = NULL ;
				return ;
			}
		}
	}
	else
	{
		if( m_saveFile != NULL )
		{
			m_saveFile->close() ;
			delete m_saveFile ;
			m_saveFile = NULL ;
		}
	}
}

void msg_modular::set_pause( bool bPause )
{
	m_bPause = bPause ;
}

bool msg_modular::get_pause()
{
	return m_bPause ;
}

void msg_modular::set_channel_name( QString channel_name_str )
{
	m_channel_name_str = channel_name_str ;
}

QString msg_modular::get_channel_name()
{
	return m_channel_name_str ; 
}

void msg_modular::set_max_bytes( int max_bytes )
{
	m_max_bytes = max_bytes ;
}

bool msg_modular::get_save_to_file()
{
	return m_bSaveToFile ;
}

void msg_modular::set_save_path( QString strPath )
{
	m_savePath = strPath ;
}