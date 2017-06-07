#include <QApplication>
#include "DbUtilityDialog.h"
#include <QTextCodec>

#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextEdit>
#include <QtDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QWindowsStyle>
#include "verapi.h"

#include "net/netapi.h"
#include "db/dbapi.h"
#include "rwxml.h"

#define SYSTEMEXECUTE
int GetexportTables( QStringList& tables );
void exportTable();
QString m_fileName;
QString m_filePath;
NetDbg dbg;
ProcMan procman;
#define  _Q2C(str) (str.toAscii().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))
int main( int argc, char ** argv )
{
	//-----
	QString agstring = _C2Q(argv[ 1 ]);
	char filename[256];
	sprintf(filename,"%s/backup/",getenv("NBENV"));
	QDir my_dir;
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("dbmtui,1.1");
				printf("dbmtui, 1.1");
				return 0;
			}
		}
	}
	//-----

	if(!my_dir.exists(_C2Q(filename)))
		
	{     //给my_dir创建一个路径
		
		my_dir.mkpath(_C2Q(filename));
		
     }
	if (QString::compare(agstring,"auto",Qt::CaseInsensitive)==0)
	{
		exportTable();
		return 0;
	}
	else
	{
		//-----
		QDialog *pDlg=NULL;
		QApplication a( argc, argv );
		// 以下部分解决中文乱码  
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
		QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
		dbg.RegProc("dbmtui");
		QMessageBox msgBox;
/*	#if defined(WIN32)
		procman.StartProc( "DbUtility", "DbUtility", "", (HWND)NULL );
	#else
		procman.StartProc( "DbUtility", "DbUtility", "" );
	#endif
*/
		// 以上部分解决中文乱码  
			


		QStringList CmdLineArgs=QApplication::arguments();


		if(CmdLineArgs.size()>=2)
		{
			if(QString::compare(CmdLineArgs.at(1),"dd_db",Qt::CaseInsensitive)==0)
			{
				pDlg=new DbUtilityDlg(DD_DB_STYLE);
			}
			else if(QString::compare(CmdLineArgs.at(1),"pb_db",Qt::CaseInsensitive)==0)
			{
				pDlg=new DbUtilityDlg(PB_DB_STYLE);
			}
			else if(QString::compare(CmdLineArgs.at(1),"dl_db",Qt::CaseInsensitive)==0)
			{
				pDlg=new DbUtilityDlg(DL_DB_STYLE);
			}
			else 
			{
				pDlg=new DbUtilityDlg(DD_DB_STYLE);

			}
		}
		else
		{
			pDlg=new DbUtilityDlg(DD_DB_STYLE);
		}
		pDlg->show();
		
		return a.exec();
	}
	
}
int GetexportTables( QStringList& tables )
	{
		QString strTmp;
		tables.clear();

		char szFileName[256];
// 		sprintf(szFileName,"%s/ini/PW_Param_table.ini",getenv("NBENV"));
		sprintf(szFileName,"%s/resource/xml/PW_Param_table.xml",getenv("NBENV"));
		QFile fileTmp;
		
			fileTmp.setFileName(szFileName);
			if(fileTmp.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream textStream(&fileTmp);
				while(!textStream.atEnd())
				{
					strTmp=textStream.readLine();
					qDebug()<<strTmp;
					tables<<strTmp;
				}
				fileTmp.close();
			}

		QDate date=QDate::currentDate();
			
		QDate tmpDate=date.addMonths(0);
		//下面处理月表,年表
		char filename[256];
		sprintf(filename,"%s/backup/",getenv("NBENV"));
		

		strTmp=QString("%1%2%3").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0')).arg(tmpDate.day(),2,10,QLatin1Char('0'));
		m_fileName="EXPTABLE" + strTmp;
		qDebug()<< m_fileName;
		m_filePath=QObject::tr(filename);
		qDebug() << m_filePath;
		/*
				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append("YC"+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append("YX"+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append("KWH"+strTmp);
				qDebug()<<tables[tables.size()-1];

				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append("DAYYC"+strTmp);
				qDebug()<<tables[tables.size()-1];

				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append("DAYKWH"+strTmp);
				qDebug()<<tables[tables.size()-1];

				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append(QObject::tr("开关变位统计表")+strTmp);
				qDebug()<<tables[tables.size()-1];
				//设备运行统计表
				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append(QObject::tr("设备运行统计表")+strTmp);
				qDebug()<<tables[tables.size()-1];


				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append(QObject::tr("操作历史事项表")+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
				tables.append(QObject::tr("电力历史事项表")+strTmp);
				qDebug()<<tables[tables.size()-1];

				strTmp=QString("%1").arg(tmpDate.year());
				tables.append("YEARKWH"+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1").arg(tmpDate.year());
				tables.append("YEARYC"+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1").arg(tmpDate.year());
				tables.append("MONTHKWH"+strTmp);
				qDebug()<<tables[tables.size()-1];
				strTmp=QString("%1").arg(tmpDate.year());
				tables.append("MONTHYC"+strTmp);
				qDebug()<<tables[tables.size()-1];
*/
		return tables.size();
	}

void exportTable()
	{
		QString tables;
		QString cmdText;
		QStringList strListTabs;
		//////////////////////////////////////////////////////////////////////////
		//获得导出表名
		GetexportTables(strListTabs);
		//生成导出命令
		tables=strListTabs.join(",");
		qDebug()<<tables;

		DBApi db ;
		if( db.isDBType( DBTYPE_DMSQL ) ) 
		{
			char iniFile[128];
			char * envtmp = getenv( "NBENV" );
			if ( envtmp == NULL || envtmp[0] == '\0' )
			{
				return ;
			}
			else
			{
				strcpy( iniFile , envtmp );
			}
// 			strcat( iniFile , "/ini/db_dmsql.ini" );
// 			ini_RW ini( iniFile );
			strcat( iniFile , "/resource/xml/db_dmsql.xml" );
			Xml_RW ini( iniFile );
			char srv_name[ DESC_LEN ] ;
			char *pBuf=0;
			int ret_rw = FAIL ;

			ret_rw = ini.get_string( "nbdb" , "hostA" , pBuf );
			if ( SUCCESS == ret_rw )
			{
				strcpy( srv_name , pBuf );
			}

			cmdText=QString(QObject::tr("dexp %1/%2@%3 log=%4 file=%5 tables=%6 compress=y  grants=y triggers=y indexes=y rows=y constraints=y ")).
				arg("MODELDB").arg("supernb").arg(srv_name).arg(m_filePath+QDir::separator()+m_fileName+"_exp.log")
				.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
		}
		else
		{
			cmdText=QString(QObject::tr("exp %1/%2@%3 log=%4 file=%5 tables=%6 buffer=81920 compress=yes consistent=yes grants=y triggers=yes indexes=yes rows=yes constraints=yes ")).
				arg("modeldb").arg("supernb").arg("nbdb").arg(m_filePath+QDir::separator()+m_fileName+"_exp.log").arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
		}
		qDebug()<<cmdText;	
	#ifdef SYSTEMEXECUTE
		system(cmdText.toLocal8Bit().data());
	#else
	#endif
		return;
	}