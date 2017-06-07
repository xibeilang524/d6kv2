#include <QFile>
#include <QtXml/QDomDocument>

#include "config.h"

Config::Config()
{
	m_config.m_ftpserver_name = "172.27.27.229";
	m_config.m_ftpserver_port = "21";
	m_config.m_user_name = "d6000";
	m_config.m_password = "d6000";
}


Config::~Config()
{

}


void Config::set_ftpserver_name( QString servername )
{
	m_ftpserver_name = servername;
}


QString Config::get_ftpserver_name()
{
	return m_ftpserver_name;
}

void Config::set_ftpserver_port( QString serverport )
{
	m_ftpserver_port = serverport;
}

QString Config::get_ftpserver_port()
{
	return m_ftpserver_port;
}

void Config::set_user_name( QString username )
{
	m_user_name = username;
}

QString Config::get_user_name()
{
	return m_user_name;
}

void Config::set_password( QString password )
{
	m_password = password;
}

QString Config::get_password()
{
	return m_password;
}

void Config::set_local_wavefile_path( QString wavefilepath )
{
	m_local_wavefile_path = wavefilepath;
}

QString Config::get_local_wavefile_path()
{
	return m_local_wavefile_path;
}

bool Config::read_config_file()
{
	char config_file_name[ 256 ] ;
	sprintf( config_file_name , ":qtwave.xml") ;	
	QFile configFile( config_file_name ) ;
	if( !configFile.open(QIODevice::ReadOnly) )
	{
		return false ;
	}
	QDomDocument doc ;
	QDomNodeList relationNodeList ;
	if ( doc.setContent( &configFile ) )
	{
		relationNodeList =  doc.documentElement().childNodes();
		qint32 cnt = 0 ;
		for ( qint32 i = 0 ; i < relationNodeList.length() ; i++ )
		{
			QDomElement rootConfigElement = relationNodeList.at( i ).toElement() ;
			rootConfigElement.tagName() ;
			QString rootNameStr  = rootConfigElement.attribute( "RootName" ).trimmed() ;
			QString rootDescStr  = rootConfigElement.attribute( "RootDesc" ).trimmed() ;
			QString valueStr  = rootConfigElement.attribute( "Value" ).trimmed() ;
			if (rootNameStr == CONFIG[0])
			{
				m_config.m_ftpserver_name = valueStr;
			}
			else if (rootNameStr == CONFIG[1])
			{
				m_config.m_ftpserver_port = valueStr;
			}
			else if (rootNameStr == CONFIG[2])
			{
				m_config.m_user_name = valueStr;
			}
			else if (rootNameStr == CONFIG[3])
			{
				m_config.m_password = valueStr;
			}
			else if (rootNameStr == CONFIG[4])
			{
				m_config.m_local_wavefile_path = valueStr;
			}
		}
	}
	configFile.close() ;
	return true ;
}

bool Config::write_config_file()
{
	return true;
}

CONFIG_SET Config::get_config_set( void )
{
	return m_config;
}
