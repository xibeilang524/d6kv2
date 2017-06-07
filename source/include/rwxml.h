/**
@file rwini.h
@brief 

@author chenkai
@version 1.2.0
@date 2017-1-10
*/
#ifndef RWINI_H
#define RWINI_H

#include <QString>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <string>

#include "rwxml_def.h"

using namespace std;

#define SUCCESS	 1
#define FAIL	 0

class RWXML_EXPORT Xml_RW : public QObject
{
public:
	Xml_RW( char* file_name ) ;
	~Xml_RW() ;

public:
	int get_bool( char *section , char *key , bool &value ) ;
	int get_int( char *section , char *key , int &value ) ;
	int get_string( char *section , char *key , char *&value ) ;
 	int get_string( string section , string key , string &value ) ;
	int get_string( char *section , char *key , QString &value ) ;
 	int set_bool( char *section , char *key , bool value ) ;
 	int set_int( char *section , char *key, int value);
 	int set_string( char*section , char *key , char *value ) ;
 	int set_string( string section , string key , string value ) ;
	int set_string( char*section , char *key , QString value ) ;

private:

private:
	QString	m_file_name;
	QFile*	m_pConfig_file ;
	QDomDocument* m_pDoc ;
	bool			m_bModify_flag ;
	char			m_buf[ 256 ] ;
};

#endif