#include "convertini.h"
#include "rwini.h"
#include "rwxml.h"

#include <string.h>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMap>
#include <QtGui/QFileDialog>

#define _Q2C(str) ((const char *)((str).toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

ConvertIni::ConvertIni( QWidget* parent, const char* name ):QMainWindow( parent )
{
	setupUi(this);

	m_syspath = QObject::tr(getenv("NBENV"));

	connect(actionOpenFile, SIGNAL(triggered()), this, SLOT(slot_open_file()));
	connect(actionXMLOut, SIGNAL(triggered()), this, SLOT(slot_ini_xml()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

ConvertIni::~ConvertIni()
{

}

void ConvertIni::slot_open_file()
{
	m_sourceList->clear();
	m_outputList->clear();
	m_fileList.clear();

	QDir		m_director;
// 	QString		m_inipath = m_syspath + QObject::tr("/ini");
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		m_syspath,
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	m_director.setPath(dirPath);
	if(m_director.exists()==0)
		return;	

	QString		m_filter = QObject::tr(".ini");

	int	gdfcount = getFileList(&m_fileList,m_director,m_filter);

	m_sourceList->addItems(m_fileList);
}

int ConvertIni::getFileList( QStringList* resultList,QDir director,QString filter )
{
	int ret =0;
	QString	str;

	QFileInfo *fi;

	QFileInfoList filist = director.entryInfoList();
	QFileInfoList::iterator it;
	for( it = filist.begin();  it != filist.end(); ++it )
	{
		int	subcount = 0;
		fi = &(*it);
		if( fi->isDir())
		{
			if( fi->fileName() == QObject::tr(".") ||
				fi->fileName() == QObject::tr(".."))
				continue;
			QDir subdir(fi->filePath());
			subcount = getFileList(resultList,subdir,filter);
		}
		else if (fi->isFile())
		{
			str = fi->filePath();
			if(str.endsWith(filter,Qt::CaseInsensitive))
			{
				resultList->append(str);
				ret ++;
			}
		}
	}

	return ret;
}

void ConvertIni::slot_ini_xml()
{
	int fileCount=0;
	fileCount = m_fileList.size();
	if (fileCount <= 0)
		return;

	QString	m_xmlPath = m_syspath + QObject::tr("/resource/xml/");
	QDir dirto(m_xmlPath);
	if(!dirto.exists())
	{    
		if(!dirto.mkdir(dirto.absolutePath())) 
			return ; 
	} 

	QString filePath;
	for (int i = 0 ; i < fileCount; i++)
	{
		filePath = m_fileList[i];
		ini_RW* pIni = new ini_RW( ( char*) _Q2C( filePath ) ) ;

		QFileInfo fi( filePath );

		QString strBaseName = fi.baseName();
		QString	strFileExt  = QObject::tr(".xml");
		
		QString dstPath = m_xmlPath+strBaseName+strFileExt;

		fi.setFile(dstPath);

		QString strTmp = QString::null;

		QDir dirTmp;
		if(fi.exists() )
		{
			strTmp = m_xmlPath+ QObject::tr("re_")+strBaseName+strFileExt;
			dstPath = strTmp;
		}
		
		m_outputList->addItem(dstPath);

		Xml_RW* pXml = new Xml_RW( ( char*) _Q2C( dstPath ) ) ;
		map<SECTIONKEY, string> inimap = pIni->list_all();
		for (map<SECTIONKEY, string>::iterator it = inimap.begin(); it != inimap.end(); it++)
		{
			pXml->set_string((*it).first.first,(*it).first.second, (*it).second);
		}
	}
}
