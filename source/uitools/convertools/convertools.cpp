#include <QDir>
#include <QtCore/QFileInfo>
#include <QtGui/QFileDialog>
#include <QFile>
#include <QList>
#include "convertools.h"
#include "uitools/dmessagebox.h"


Convertools::Convertools( QWidget* parent /*= NULL */, const char* name /*= NULL*/ ) :QMainWindow(parent)
{
	setupUi(this);
	m_sourceList->clear();
	m_sourceList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_outputList->clear();
	m_file_list.clear();
	m_file_path = tr(getenv("NBENV")) ;

	connect(actionOpenDir , SIGNAL(triggered()) , this , SLOT(slot_open_dir()));
	connect(actionOpenFile , SIGNAL(triggered()) , this , SLOT(slot_open_file()));
	connect(actionDelete , SIGNAL(triggered()) , this , SLOT(slot_del_filelist()));
	connect(actionConvert , SIGNAL(triggered()) , this , SLOT(slot_gdf_to_bgf()));
	connect(actionAbout , SIGNAL(triggered()) , this , SLOT(slot_about()));
	connect(actionExit , SIGNAL(triggered()) , this , SLOT(slot_exit()));
}

Convertools::~Convertools()
{

}

void Convertools::slot_open_dir()
{
	QString fliedir ;
	/**打开文件夹**/
	fliedir = QFileDialog::getExistingDirectory(this , tr("打开文件夹") , 
																		m_file_path , 
																		QFileDialog::ShowDirsOnly 
																		| QFileDialog::DontResolveSymlinks );
	QDir m_director ;
	m_director.setPath(fliedir);
	if (m_director.exists() == 0)
	{
		return;
	}

	QString m_filter = tr(".gdf");

	int gdfcount = get_file_list(m_file_list , m_director ,m_filter);

	m_sourceList->addItems(m_file_list) ;

}

void Convertools::slot_open_file()
{
	QStringList files;
	/**打开文件**/
// 	file = QFileDialog::getOpenFileName(this , tr("选取文件") ,
// 																	m_file_path , 
// 																	tr("Files(*.gdf )" ));
	
	/**打开多个文件**/
	files = QFileDialog::getOpenFileNames(this , tr("选取文件") , 
																	  m_file_path , 
																	  tr("File(*.gdf)")) ;

	m_file_list = files ;

	m_sourceList->addItems(m_file_list) ;

}	

int Convertools::get_file_list( QStringList &resultList , QDir director , QString filter )
{
	int ret = 0;
	QString str ;
	QFileInfo fi;
	QFileInfoList filist = director.entryInfoList();
	for (int i =0 ; i <filist.count() ; i++ )
	{
		fi = filist.at(i);
		if (fi.isDir())
		{
			if (fi.fileName() == tr(".") || fi.fileName() == tr("..") )	
				continue;

				QDir subdir(fi.filePath());

				int subcount = 0;
				subcount=get_file_list(resultList , subdir ,filter);			
		}
		else if (fi.isFile())
		{
			str = fi.filePath();
			if (str.endsWith( filter , Qt::CaseInsensitive))
			{
				resultList.append(str);
				ret++;
			}
		}
	}
			return ret;
}

void Convertools::slot_gdf_to_bgf()
{	
	int fileCount = 0;
	fileCount = m_file_list.size();
	if (fileCount <= 0)
	{
		return;
	}

	QString filepath;
	QString xmlpath;
	QString bgfPath;
	for (int i = 0; i<fileCount; i++)
	{
		filepath = m_file_list[i];
		QFileInfo fi( filepath );
		QString strFilePath = fi.filePath();
		QString strFileName = fi.fileName();
		QString strBaseName = fi.baseName();
		QString strFileExt = tr(".bgf");
		QString strSrcFileExt = tr(".xml");

		strFilePath = strFilePath.left(strFilePath.length() - strFileName.length());

		xmlpath = strFilePath + strBaseName + strSrcFileExt;
		bgfPath = strFilePath + strBaseName + strFileExt;

		fi.setFile(xmlpath);
		QString re_xmlpath = QString::null;
		if (fi.exists())
		{
			re_xmlpath = strFilePath + tr("re_") + strBaseName +strSrcFileExt ;
			xmlpath =re_xmlpath ;
		}

		if ( gdf_to_xml(filepath , xmlpath) );

		fi.setFile(bgfPath);
		QString re_bgfpath = QString::null;
		if (fi.exists())
		{
			re_bgfpath = strFilePath + tr("re_") + strBaseName + strFileExt ;
			bgfPath = re_bgfpath;
		}

		if ( xml_to_bgf( xmlpath , bgfPath ) );
		
		m_outputList->addItem(bgfPath);
	}

	DMessageBox::information( this , tr( "提示" ) ,  tr(" 文件格式转换成功！") ) ;

}

bool Convertools::gdf_to_xml( QString &gdfpath , QString &xmlpath )
{
	GGraphFile gf;
	GDevFile devFile;

	devFile.open(_Q2C(gdfpath) );

	gf.loadFile(devFile);

	gf.saveXML(_Q2C(xmlpath) );

	devFile.close();

	QString strAll; 
	QFile readfile(xmlpath);
	if (readfile.open((QIODevice::ReadOnly | QIODevice::Text)))
	{
		QTextStream stream (&readfile);
		stream.setCodec("UTF-8");
		strAll = stream.readAll();
	}
	readfile.close();

	QFile writefile(xmlpath);
	if (writefile.open((QIODevice::WriteOnly | QIODevice::Text)))
	{
		QTextStream stream (&writefile);
		stream.setCodec("UTF-8");
		strAll.replace(QString(".gdf") , QString(".bgf"));
		strAll.replace(QString("paradb") , QString("modeldb"));
		strAll.replace(QString("bmp") , QString("resource/image"));
		stream<<strAll;
	}
	writefile.close();
	return true;
}

bool Convertools::xml_to_bgf( QString &xmlpath , QString &bgfpath )
{
	GGraphFile bgf;

	GDevFile dev(_Q2C(bgfpath) , GFile::modeCreate|GFile::modeWrite|GFile::typeBinary);

	bgf.readXML(_Q2C(xmlpath));

	bgf.saveFile(dev);

	dev.close();		

	return true;
}

void Convertools::slot_del_filelist()
{
		if (m_sourceList->currentItem()!= NULL)
		{
			QList<QListWidgetItem *> it = m_sourceList->selectedItems();
			int item_count = it.count();
			if (item_count <= 0)
			{
				return;
			}
			for (int i = 0 ; i < item_count ; i++)
			{
				int item_row = m_sourceList->row(it.at(i));
				QListWidgetItem *item = m_sourceList->takeItem(item_row);
				QString str = item->text();
				m_file_list.removeOne(str);
				delete item;
			}
		}
}


void Convertools::slot_search_file()
{

}

void Convertools::slot_exit()
{
	close();
}

void Convertools::slot_about()
{
	QString msg = tr( "GDF转BGF工具\n南京大全自动化科技有限公司\n版权所有" ) ;
	DMessageBox::about( this , tr( "关于" ) ,  msg ) ;
}


