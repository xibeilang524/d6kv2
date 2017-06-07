// PB_Dialog.cpp: implementation of the PB_Dialog class.
//
//////////////////////////////////////////////////////////////////////

//导出文件名的约定
//电量DL,配变PB,调度DD
//包含参数:para
//电量:包含表码MA,电量DL,对象电量OL,瞬时量SSL,事项EV,结算数据JS,
//最后是导出的日期。
//////////////////////////////////////////////////////////////////////////

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QTextEdit>
#include <QtDebug>
#include <QTextCodec>
#include <QDateTime>
#include <QDir>

#include "DbUtilityDialog.h"
#include "ui_PB_Dialog.h"
#include "ui_DD_Dialog.h"
#include "ui_DL_Dialog.h"
#include "DbLoginDlg.h"
#include "ui_Login_Dialog.h"
#include "net/netapi.h"
#include "db/dbapi.h"
#include "rwxml.h"
#include "uitools/dmessagebox.h"

#define  DL_PARAM_TABLE_INI ("DL_Param_table.xml")		//定义了参数表的存放文件，可重配置和修改
#define  PW_PARAM_TABLE_INI ("PW_Param_table.xml")      //定义了配网参数表的存放文件，可重配置和修改-
#define  ONLY_MAIN_DB						//仅有主库
#define  DL_FUNC_EXP						//具有导出功能
#define  DL_FUNC_IMP						//具有导入功能
#define	 DL_FUNC_CLR						//具有清除功能
//#define  DL_SAFE_USE						//是否需要输入数据库用户名和密码
#define  SYSTEMEXECUTE						//命令执行方式
#define  CLEARDBSCRIPT	"cleantables.sql"	//电量清除脚本文件名 

#define  _Q2C(str) (str.toAscii().data())
#define _C2Q(str) (QString::fromLocal8Bit(str))

extern NetDbg dbg;

DbUtilityDlg::DbUtilityDlg(int nDlgStyle,QWidget *parent /*= 0*/ )
:QDialog(parent)/*,ui(new Ui::pb_Dialog)*/
{
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale( ));

	
	dd_ui=NULL;
	dl_ui=NULL;
	pb_ui=NULL; 
	m_pProcess=NULL;
	m_pwTablesList.clear() ;
	m_dbtype = DBTYPE_ORACLE ;

	switch(nDlgStyle)
	{
	case DD_DB_STYLE:
		dd_ui=new Ui::dd_Dialog;
		dd_ui->setupUi(this);
		dd_ui->tabWidget->setCurrentIndex(0);

//		dd_ui->tabWidget->removeTab(2);
		this->setWindowTitle(tr("数据库工具"));
		this->setWindowIcon( QIcon( ":/res/dbmtui.png" ) ) ;

		if ( !readPWTables( m_pwTablesList ) )
		{
// 			QMessageBox msgBox;
// 			msgBox.setText( _C2Q( "未能正确获取表名！") );
// 			msgBox.exec();
			DMessageBox::information( this , tr( "提示" ) ,  tr("未能正确获取表名！") ) ;
		}

		break;
	case PB_DB_STYLE:
		pb_ui=new Ui::pb_Dialog;
		pb_ui->setupUi(this);
		this->setWindowTitle(tr("配变数据库工具"));
		this->setWindowIcon( QIcon( ":/res/dbmtui.png" ) ) ;
		break;
	case DL_DB_STYLE:
		dl_ui=new Ui::dl_Dialog;
		dl_ui->setupUi(this);
		setWindowTitle(tr("电量数据库工具"));
		setWindowIcon( QIcon( ":/res/dbmtui.png" ) ) ;
#if defined ONLY_MAIN_DB
		dl_ui->chkBD->setVisible(false);
		dl_ui->chkMD->setChecked(true);
#endif
		dl_ui->tabWidget->clear();
#if defined DL_FUNC_EXP
		dl_ui->tabWidget->addTab(dl_ui->tab1,tr("导出"));
#endif
#if defined DL_FUNC_IMP
		dl_ui->tabWidget->addTab(dl_ui->tab2,tr("导入"));
#endif
#if defined DL_FUNC_CLR
		dl_ui->tabWidget->addTab(dl_ui->tab3,tr("清除"));
#endif
		break;
	}
	m_nDbStyle=nDlgStyle;
	tab_selChanged(0);
	connect(dd_ui->chk_MDL,SIGNAL(stateChanged(int) ),this,SLOT(slotChekmdlChanged(int)) );

	ini_style_sheet() ;
}

DbUtilityDlg::~DbUtilityDlg()
{
	if(dd_ui) delete dd_ui,dd_ui=NULL;
	if(pb_ui) delete pb_ui,pb_ui=NULL;
	if(dl_ui) delete dl_ui,dl_ui=NULL;
	if(m_pProcess) delete m_pProcess,m_pProcess=NULL;
	dbg.UnRegProc();
}

//“模型数据” checked
void DbUtilityDlg::slotChekmdlChanged(int state)
{
	if (Qt::Unchecked == state)
	{
	//	dd_ui->chk_PT->setChecked(true);
		dd_ui->txtExpFile->setEnabled(true);
		dd_ui->btnExp->setEnabled(true);
		dd_ui->textLabel1_3->setText(_C2Q("注：路径中不要包含中文字符"));
	} 
	else if (Qt::Checked == state)
	{
		dd_ui->chk_PT->setChecked(false);
		dd_ui->chkMT->setChecked(false);
		dd_ui->chkMCT->setChecked(false);
		dd_ui->chkYT->setChecked(false);
		dd_ui->chkET->setChecked(false);
		dd_ui->txtExpFile->setEnabled(false);
		dd_ui->btnExp->setEnabled(false);
		dd_ui->textLabel1_3->setText(_C2Q("注：备份路径在/backup目录下"));
	}
}

void DbUtilityDlg::exit()
{
	if(dd_ui) delete dd_ui,dd_ui=NULL;
	if(pb_ui) delete pb_ui,pb_ui=NULL;
	if(dl_ui) delete dl_ui,dl_ui=NULL;
	if(m_pProcess) delete m_pProcess,m_pProcess=NULL;
	dbg.UnRegProc();
	close();
}

void DbUtilityDlg::about()
{
// 	QMessageBox msgBox;
// 	msgBox.setText(_C2Q("数据库备份、还原、清理工具！"));
// 	msgBox.exec();
	DMessageBox::information( this , tr( "提示" ) ,  tr("数据库备份、还原、清理工具！") ) ;
}

void DbUtilityDlg::tab_selChanged( int tabIndex )
{
	QDate date=QDate::currentDate();
	QDate beginDate,endDate;

	switch(tabIndex)
	{
	case 0:	//导出
		beginDate=date.addMonths(-1);
		endDate=date.addMonths(0);
		break;
	case 1: //导入
		beginDate=date.addMonths(-1);
		endDate=date.addMonths(0);
		break;
	case 2:
// 		beginDate=date.addMonths(-48);
// 		endDate=date.addMonths(-36);
		beginDate=date.addMonths(-1);
		endDate=date.addMonths(0);
		break;
	case 3:
		break;
	}
	
	if(m_nDbStyle==PB_DB_STYLE)
	{
		pb_ui->dtST->setDate(beginDate);
		pb_ui->dtET->setDate(endDate);
	}
	else if(m_nDbStyle==DD_DB_STYLE)
	{
		dd_ui->dtST->setDate(beginDate);
		dd_ui->dtET->setDate(endDate);
	}
	else if(m_nDbStyle==DL_DB_STYLE)
	{
		dl_ui->dtST->setDate(beginDate);
		dl_ui->dtET->setDate(endDate);
	}

}

void DbUtilityDlg::exportDb()
{
	QDir dirPath;
	QString fileName;

	QString dirString=QFileDialog::getExistingDirectory(this,tr("保存导出文件"),tr(""));  //tr("导出文件 (*.dmp)")
	if(dirString.isEmpty())
		return;
	dirPath.setPath(dirString);
	m_filePath=dirPath.absolutePath();
	m_filePath=dirPath.toNativeSeparators(m_filePath);
	if(dirPath.isRoot())		//处理一下根目录情况，去掉最后的"/"符号
		m_filePath=m_filePath.mid(0,m_filePath.size()-1);
	qDebug()<<m_filePath;
	if(m_nDbStyle==PB_DB_STYLE)
	{
		pb_ui->txtExpFile->setText(m_fileName+".dmp");
	}
	else if(m_nDbStyle==DD_DB_STYLE)
	{
		m_fileName=getPWFilename();
		dd_ui->txtExpFile->setText(m_filePath+QDir::separator()+m_fileName+".dmp");
	}
	else if(m_nDbStyle==DL_DB_STYLE)
	{
		m_fileName=getDLFilename();
		dl_ui->txtExpFile->setText(m_filePath+QDir::separator()+m_fileName+".dmp");
	}
}

void DbUtilityDlg::importDb()
{
	if (dd_ui->chk_MDL->isChecked())
	{
		QDir dirPath;
		char fileName[255];
		QString fileGraphName;
		sprintf(fileName,"%s/backup",getenv("NBENV"));
		
		QString dirString=QFileDialog::getExistingDirectory(this,tr("选择模型文件"),tr(fileName));  //tr("导出文件 (*.dmp)")
		dirPath.setPath(dirString);
		dd_ui->txtImpFile->setText(dirString);
		
	}
	else
	{
		QDir dirPath;
		QString fullFileName=QFileDialog::getOpenFileName(this,tr("打开导入文件"),tr(""),tr("导入文件 (*.dmp)"));
		fullFileName=QDir::toNativeSeparators(fullFileName);
		qDebug()<<fullFileName;
		if(fullFileName.isEmpty())
			return;
		if(m_nDbStyle==PB_DB_STYLE)
		{
			pb_ui->txtImpFile->setText(fullFileName);
		}
		else if(m_nDbStyle==DD_DB_STYLE)
		{
			//解析文件名和路径
			int sepPos=fullFileName.lastIndexOf(QDir::separator(),-1,Qt::CaseInsensitive);
			int commaPos=fullFileName.lastIndexOf(QChar('.'));
			if(commaPos<0) commaPos=0;
			m_fileName=fullFileName.mid(sepPos+1,commaPos-sepPos-1);
			m_filePath=fullFileName.left(sepPos);
			dd_ui->txtImpFile->setText(fullFileName);
			qDebug()<<m_fileName;
			qDebug()<<m_filePath;
			
		}
		else if(m_nDbStyle==DL_DB_STYLE)
		{
			//解析文件名和路径
			int sepPos=fullFileName.lastIndexOf(QDir::separator(),-1,Qt::CaseInsensitive);
			int commaPos=fullFileName.lastIndexOf(QChar('.'));
			if(commaPos<0) commaPos=0;
			m_fileName=fullFileName.mid(sepPos+1,commaPos-sepPos-1);
			m_filePath=fullFileName.left(sepPos);
			dl_ui->txtImpFile->setText(fullFileName);
			
			qDebug()<<m_fileName;
			qDebug()<<m_filePath;
	}

	}

	return;
}

void DbUtilityDlg::begin()
{
// 	QMessageBox msgBox;
	QDate beginDate,endDate,tmpDate;
	qDebug()<<m_fileName;
	qDebug()<<m_filePath;
#if defined(DL_SAFE_USE)
	DbLoginDlg loginDlg(this);
	loginDlg.setWindowModality(Qt::WindowModal);
	if(loginDlg.exec()!=0)
		return;
	m_userName=loginDlg.m_user;
	m_password=loginDlg.m_pwd;
#else
	m_userName="modeldb";
	m_password="supernb";
#endif	
	char filename[256];
	sprintf(filename,"%s/backup/",getenv("NBENV"));
	QDir my_dir;
	if(!my_dir.exists(_C2Q(filename)))
		
	{     //给my_dir创建一个路径
		
		my_dir.mkpath(_C2Q(filename));
		
    }
	switch(m_nDbStyle)
	{
	case DD_DB_STYLE:
		{
			beginDate=dd_ui->dtST->date();
			endDate=dd_ui->dtET->date();
			if(!(dd_ui->chk_MDL->isChecked()||dd_ui->chk_PT->isChecked()  || dd_ui->chkMT->isChecked() || dd_ui->chkMCT->isChecked() || dd_ui->chkYT->isChecked() || dd_ui->chkET->isChecked()))
			{
// 				msgBox.setText(tr("请在导出选项中选择需要导出内容!"));
// 				msgBox.exec();
				DMessageBox::information( this , tr( "提示" ) ,  tr("请在导出选项中选择需要导出内容！") ) ;
				return;
			}
			if(beginDate>endDate)
			{
// 				msgBox.setText(tr("开始日期大于结束日期，请重新选择导出日期范围!"));
// 				msgBox.exec();
				DMessageBox::information( this , tr( "提示" ) ,  tr("开始日期大于结束日期，\n请重新选择导出日期范围！") ) ;
				return;
			}

			if(dd_ui->tabWidget->currentIndex()==0)
				exportPWTables();
			else if(dd_ui->tabWidget->currentIndex()==1)
				importPWTables();
			else if(dd_ui->tabWidget->currentIndex()==2)
				cleanPWTables();
		}
		break;
	case PB_DB_STYLE:
		break;
	case DL_DB_STYLE:
		{
			beginDate=dl_ui->dtST->date();
			endDate=dl_ui->dtET->date();
			if(!(dl_ui->chk_PT->isChecked() || dl_ui->chkXL->isChecked() || dl_ui->chkBMT->isChecked() || dl_ui->chkDLT->isChecked() || 
				dl_ui->chkSSLT->isChecked() || dl_ui->chkODLT->isChecked()|| dl_ui->chkJST->isChecked() || dl_ui->chkET->isChecked()))
			{
// 				msgBox.setText(tr("请在导出选项中选择需要导出内容!"));
// 				msgBox.exec();
				DMessageBox::information( this , tr( "提示" ) ,  tr("请在导出选项中选择需要导出内容！") ) ;
				return;
			}
			if(beginDate>endDate)
			{
// 				msgBox.setText(tr("开始日期大于结束日期，请重新选择导出日期范围!"));
// 				msgBox.exec();
				DMessageBox::information( this , tr( "提示" ) ,  tr("开始日期大于结束日期，\n请重新选择导出日期范围！") ) ;
				return;
			}
			if(dl_ui->tabWidget->currentIndex()==0)
				exportDL();
			else if(dl_ui->tabWidget->currentIndex()==1)
				importDL();
			else if(dl_ui->tabWidget->currentIndex()==2)
				cleanDL();
		}
		break;
	}
	return;
}


int DbUtilityDlg::GetDLTables( QStringList& tables )
{
	QString strTmp;
	QDate beginDate,endDate,tmpDate;
	tables.clear();
	QFile fileTmp;

	beginDate=dl_ui->dtST->date();
	endDate=dl_ui->dtET->date();

	if(dl_ui->chk_PT->isChecked())		//参数
	{
		if(!fileTmp.exists(DL_PARAM_TABLE_INI))
		{
// 			QMessageBox::critical(this,tr("错误"),tr("未能找到电量参数表的配置文件"));
			DMessageBox::critical( this , tr( "错误" ) ,  tr("未能找到电量参数表的配置文件！") ) ;
			return 0;
		}
		fileTmp.setFileName(DL_PARAM_TABLE_INI);
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
		else
		{
			//文件打开失败
		}
	}
	
	if(dl_ui->chkJST->isChecked())		//结算数据
	{
		tables.append(tr("DL_JS线路电费表"));
		qDebug()<<tables[tables.size()-1];
		tables.append(tr("DL_JS用户电费表"));
		qDebug()<<tables[tables.size()-1];
	}
	
	if(dl_ui->chkXL->isChecked())
	{
		tables.append(tr("DL需量数据表"));
		qDebug()<<tables[tables.size()-1];
	}
	
	//下面处理月表
	tmpDate=beginDate;
	do 
	{
		if(dl_ui->chkBMT->isChecked())	//表码数据
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DL表码原始数据表")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DL表码派生数据表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		if(dl_ui->chkDLT->isChecked())	//电量
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("dlpoint电量数据表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		if(dl_ui->chkSSLT->isChecked())
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DL瞬时量数据表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		
		if(dl_ui->chkODLT->isChecked())
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DL对象电量数据表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		
		if(dl_ui->chkET->isChecked())
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("操作历史事项表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		tmpDate=tmpDate.addMonths(1);
	} while (tmpDate<=endDate);
	return tables.size();
}

int DbUtilityDlg::GetPWTables( QStringList& tables )
{
	QString strTmp;
	QDate beginDate,endDate,tmpDate;
	tables.clear();

	beginDate=dd_ui->dtST->date();
	endDate=dd_ui->dtET->date();
	
	if(dd_ui->chk_PT->isChecked() ||dd_ui->chk_MDL->isChecked())		//参数
	{
		tables << m_pwTablesList ;
	}
	if (dd_ui->chkBD->isChecked())
	{

	}
	
	//下面处理月表,年表
	tmpDate=beginDate;
	do 
	{
		if(dd_ui->chkMT->isChecked())	//月历史数据  yc  yx   kwh
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("YC")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("YX")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("KWH")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		if(dd_ui->chkMCT->isChecked())	//月统计数据  dayyc  daykwh   开关变位统计
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DAYYC")+strTmp);
			qDebug()<<tables[tables.size()-1];

			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("DAYKWH")+strTmp);
			qDebug()<<tables[tables.size()-1];

			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("开关变位统计表")+strTmp);
			qDebug()<<tables[tables.size()-1];
			//设备运行统计表
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("设备运行统计表")/*+strTmp*/);
			qDebug()<<tables[tables.size()-1];
		}
		if(dd_ui->chkET->isChecked())	//事项记录表  操作历史事项表  电力历史事项表
		{
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("操作历史事项表")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1%2").arg(tmpDate.year()).arg(tmpDate.month(),2,10,QLatin1Char('0'));
			tables.append(tr("电力历史事项表")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		if (dd_ui->chkYT->isChecked())  //yearkwh,yearyc,monthkwh,monthyc
		{
			strTmp=QString("%1").arg(tmpDate.year());
			tables.append(tr("YEARKWH")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1").arg(tmpDate.year());
			tables.append(tr("YEARYC")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1").arg(tmpDate.year());
			tables.append(tr("MONTHKWH")+strTmp);
			qDebug()<<tables[tables.size()-1];
			strTmp=QString("%1").arg(tmpDate.year());
			tables.append(tr("MONTHYC")+strTmp);
			qDebug()<<tables[tables.size()-1];
		}
		tmpDate=tmpDate.addMonths(1);
	} while (tmpDate<=endDate);
	return tables.size();
}

void DbUtilityDlg::exportPWTables()
{
	QString tables;
	QString cmdText;
	QStringList strListTabs;
	DBApi db ;
	if (dd_ui->chk_MDL->isChecked())
	{
		char fileName1[255];
		QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
		QString str = time.toString("yyyyMMddhhmmss"); //设置显示格式

		sprintf(fileName1,"%s/backup/model",getenv("NBENV"));
		QString filename2;
		filename2=_C2Q(fileName1);
		filename2=filename2 +str;
		QDir my_dir;
		if(!my_dir.exists(filename2))
			
        {     //给my_dir创建一个路径
			
            my_dir.mkpath(filename2);
			
        }
		//模型表备份
		GetPWTables(strListTabs);
		tables=strListTabs.join(",");
		if ( m_dbtype == DBTYPE_DMSQL)
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

			if ( dd_ui->chkMD->isChecked()  )
			{
				ret_rw = ini.get_string( "nbdb" , "hostA" , pBuf );
				if ( SUCCESS == ret_rw )
				{
					strcpy( srv_name , pBuf );
				}
			}
			else
			{
				ret_rw = ini.get_string( "nbdbb" , "hostA" , pBuf );
				if ( SUCCESS == ret_rw )
				{
					strcpy( srv_name , pBuf );
				}
			}
			cmdText=QString(tr("dexp %1/%2@%3 log=%4 file=%5 tables=%6 buffer=81920 compress=yes consistent=yes grants=y triggers=yes indexes=yes rows=yes constraints=yes ")).
				arg(m_userName).arg(m_password).arg(srv_name).arg(filename2+QDir::separator()+"para_exp.log")
				.arg(filename2+QDir::separator()+"para_exp.dmp").arg(tables);
		}

		else
		{
			cmdText=QString(tr("exp %1/%2@%3 log=%4 file=%5 tables=%6 buffer=81920 compress=yes consistent=yes grants=y triggers=yes indexes=yes rows=yes constraints=yes ")).
				arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(filename2+QDir::separator()+"para_exp.log")
				.arg(filename2+QDir::separator()+"para_exp.dmp").arg(tables);
		}
		qDebug()<<cmdText;	
		#ifdef SYSTEMEXECUTE
				system(cmdText.toLocal8Bit().data());
		#else
		#endif
		//end
		//模型graph备份
		QString strGraph;
		strGraph.sprintf("%s/graph",getenv("NBENV"));
		copyGraphFullDir(QDir(strGraph),QDir(filename2 + "/graph"));
// 		QMessageBox msgBox;
// 		msgBox.setText(_C2Q("模型备份成功!"));
// 		msgBox.exec();
		DMessageBox::information( this , tr( "提示" ) ,  tr("模型备份成功！") ) ;

		//end
		return;
	}
	if(dd_ui->txtExpFile->text().isEmpty()|| dd_ui->txtExpFile->text().isNull())
	{
// 		QMessageBox msgBox;
// 		msgBox.setText(_C2Q("请输入需要导出的文件名称"));
// 		msgBox.exec();
		DMessageBox::information( this , tr( "提示" ) ,  tr("请输入需要导出的文件名称！") ) ;
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//获得导出表名
	qDebug()<<tr("getpwtable start");

	GetPWTables(strListTabs);

	qDebug()<<tr("getpwtable end");
	//生成导出命令
	tables=strListTabs.join(",");
	qDebug()<<tables;
	if ( m_dbtype == DBTYPE_DMSQL)
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
// 		strcat( iniFile , "/ini/db_dmsql.ini" );
// 		ini_RW ini( iniFile );
		strcat( iniFile , "/resource/xml/db_dmsql.xml" );
		Xml_RW ini( iniFile );
		char srv_name[ DESC_LEN ] ;
		char *pBuf=0;
		int ret_rw = FAIL ;

		if ( dd_ui->chkMD->isChecked()  )
		{
			ret_rw = ini.get_string( "nbdb" , "hostA" , pBuf );
			if ( SUCCESS == ret_rw )
			{
				strcpy( srv_name , pBuf );
			}
		}
		else
		{
			ret_rw = ini.get_string( "nbdbb" , "hostA" , pBuf );
			if ( SUCCESS == ret_rw )
			{
				strcpy( srv_name , pBuf );
			}
		}
		cmdText=QString(tr("dexp %1/%2@%3 log=%4 file=%5 tables=%6 compress=y  grants=y triggers=y indexes=y rows=y constraints=y ")).
			arg(m_userName.toUpper()).arg(m_password).arg(srv_name).arg(m_filePath+QDir::separator()+m_fileName+"_exp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}
	else
	{
		cmdText=QString(tr("exp %1/%2@%3 log=%4 file=%5 tables=%6 buffer=81920 compress=yes consistent=yes grants=y triggers=yes indexes=yes rows=yes constraints=yes ")).
			arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_exp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}

	qDebug()<<cmdText;	
#ifdef SYSTEMEXECUTE
	system(cmdText.toLocal8Bit().data());
#else
#endif
	return;
}

//////////////////////////////////////////////////////////////////////////
//备份图形目录
//[in] QDir dirfrom 源路径
//[in] QDir dirto 目标路径
//
bool DbUtilityDlg::copyGraphFullDir(QDir dirfrom,QDir dirto)
{
	if(!dirto.exists())
	{    
		if(!dirto.mkdir(dirto.absolutePath())) 
			return false; 
	} 
	
	QFileInfoList fileInfoList = dirfrom.entryInfoList(); 
	foreach(QFileInfo fileInfo, fileInfoList)
	{ 
		if(fileInfo.fileName() == "." || fileInfo.fileName() == "..") 
			continue; 
		
		QString dirFromTemp = fileInfo.filePath();
		QString dirToTemp = dirto.filePath(fileInfo.fileName());
		if(fileInfo.isDir())
		{
			if(!copyGraphFullDir(QDir(dirFromTemp), QDir(dirToTemp) ) ) 
				return false; 
		} 
		else
		{
			if(dirto.exists(fileInfo.fileName()))
			{ 
				dirto.remove(fileInfo.fileName());  
			} 
			
			if(!QFile::copy(fileInfo.filePath(),dirto.filePath(fileInfo.fileName())) )
			{ 
				return false; 
			} 
		} 
	} 
	return true; 
}

void DbUtilityDlg::exportDL()
{
	QString tables;
	QString cmdText;
	QStringList strListTabs;

	if(dl_ui->txtExpFile->text().isEmpty()|| dl_ui->txtExpFile->text().isNull())
	{
	//	QMessageBox::warning(this,tr("警告"),tr("请输入需要导出的文件名称!"));
// 		QMessageBox msgBox;
// 		msgBox.setText(_C2Q("请输入需要导出的文件名称!"));
// 		msgBox.exec();
		DMessageBox::warning( this , tr( "警告" ) ,  tr("请输入需要导出的文件名称！") ) ;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//获得导出表名
	GetDLTables(strListTabs);
	//生成导出命令
	tables=strListTabs.join(",");
	qDebug()<<tables;

	if( m_dbtype == DBTYPE_DMSQL ) 
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
// 		strcat( iniFile , "/ini/db_dmsql.ini" );
// 		ini_RW ini( iniFile );
		strcat( iniFile , "/resource/xml/db_dmsql.xml" );
		Xml_RW ini( iniFile );
		char srv_name[ DESC_LEN ] ;
		char *pBuf=0;
		int ret_rw = FAIL ;

		if ( dd_ui->chkMD->isChecked()  )
		{
			ret_rw = ini.get_string( "nbdb" , "hostA" , pBuf );
			if ( SUCCESS == ret_rw )
			{
				strcpy( srv_name , pBuf );
			}
		}
		else
		{
			ret_rw = ini.get_string( "nbdbb" , "hostA" , pBuf );
			if ( SUCCESS == ret_rw )
			{
				strcpy( srv_name , pBuf );
			}
		}
		cmdText=QString(QObject::tr("dexp %1/%2@%3 log=%4 file=%5 tables=%6 compress=y  grants=y triggers=y indexes=y rows=y constraints=y ")).
			arg(m_userName.toUpper()).arg(m_password).arg(srv_name).arg(m_filePath+QDir::separator()+m_fileName+"_exp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}
	else
	{
		cmdText=QString(tr("exp userid=%1/%2@%3 log=%4 file=%5 tables=(%6) buffer=81920 compress=yes consistent=yes grants=y triggers=yes indexes=yes rows=yes constraints=yes ")).
			arg(m_userName).arg(m_password).arg(dl_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_exp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}
	qDebug()<<cmdText;	
#ifdef SYSTEMEXECUTE
	system(cmdText.toLocal8Bit().data());
#else
#endif
	return;
}


//导入电量备份到数据库
void DbUtilityDlg::importDL()
{
	QString tables;
	QString cmdText;
	QStringList strListTabs;
	qDebug()<<m_fileName;
	qDebug()<<m_filePath;

	if(dl_ui->txtImpFile->text().isEmpty()|| dl_ui->txtImpFile->text().isNull())
	{
// 		QMessageBox::warning(this,tr("警告"),tr("请选择需要导入的文件!"));
		DMessageBox::warning( this , tr( "警告" ) ,  tr("请选择需要导入的文件！") ) ;
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//从界面上获得导出表名
	GetDLTables(strListTabs);
	//生成导出命令
	tables=strListTabs.join(",");
	if(dl_ui->chkImpAll->isChecked())
	{
		cmdText=QString(tr("imp userid=%1/%2@%3 buffer=1024 log=%4 file=%5 full=y commit=no grants=yes ignore=yes indexes=yes rows=yes show=no constraints=yes" )).
			arg(m_userName).arg(m_password).arg(dl_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_imp.log")
		.arg(m_filePath+QDir::separator()+m_fileName+".dmp");
	}
	else
	{
		cmdText=QString(tr("imp userid=%1/%2@%3 buffer=1024 log=%4 file=%5 tables=(%6) fromuser=superdb touser=superdb full=n commit=no grants=yes ignore=yes indexes=yes rows=yes show=no constraints=yes" )).
			arg(m_userName).arg(m_password).arg(dl_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_imp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}
	qDebug()<<cmdText;	
#ifdef SYSTEMEXECUTE
	system(cmdText.toLocal8Bit().data());
#else
#endif
	return;
}

//导入配网数据备份到数据库
void DbUtilityDlg::importPWTables()
{
	QString tables;
	QString cmdText;
	QStringList strListTabs;
	qDebug()<<m_fileName;
	qDebug()<<m_filePath;
	
	if (dd_ui->chk_MDL->isChecked())
	{

		QString dirString;
		dirString=dd_ui->txtImpFile->text();


		//模型参数库恢复
			cmdText=QString(tr("imp userid=%1/%2@%3 buffer=1024 log=%4 file=%5 full=y commit=no grants=yes ignore=yes indexes=yes rows=yes show=no constraints=yes" )).
			arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_imp.log")
			.arg(dirString +"//para_exp.dmp");

			qDebug()<<cmdText;	
			#ifdef SYSTEMEXECUTE
				system(cmdText.toLocal8Bit().data());
			#else
			#endif
		//end
		//模型图形恢复

		QString prsGraphPath;
		prsGraphPath.sprintf("%s//graph",getenv("NBENV"));

		copyGraphFullDir(QDir(dirString+"//graph"),QDir(prsGraphPath));
		//end
// 		QMessageBox msgBox;
// 		msgBox.setText(_C2Q("模型恢复成功!"));
// 		msgBox.exec();
		DMessageBox::information( this , tr( "提示" ) ,  tr("模型恢复成功！") ) ;

		return;
	}
	if(dd_ui->txtImpFile->text().isEmpty()|| dd_ui->txtImpFile->text().isNull())
	{
	//	QMessageBox::warning(this,tr("警告"),tr("请选择需要导入的文件!"));
// 		QMessageBox msgBox;
// 		msgBox.setText(_C2Q("请选择需要导入的文件!"));
// 		msgBox.exec();
		DMessageBox::warning( this , tr( "警告" ) ,  tr("请选择需要导入的文件！") ) ;
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//从界面上获得导出表名
	GetPWTables(strListTabs);

	//生成导出命令
	tables=strListTabs.join(",");
	if(dd_ui->chkImpAll->isChecked())
	{
		cmdText=QString(tr("imp userid=%1/%2@%3 buffer=1024 log=%4 file=%5 full=y commit=no grants=yes ignore=yes indexes=yes rows=yes show=no constraints=yes" )).
			arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_imp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp");
	}
	else
	{
		cmdText=QString(tr("imp userid=%1/%2@%3 buffer=1024 log=%4 file=%5 tables=(%6) fromuser=modeldb touser=modeldb full=n commit=no grants=yes ignore=yes indexes=yes rows=yes show=no constraints=yes" )).
			arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb")).arg(m_filePath+QDir::separator()+m_fileName+"_imp.log")
			.arg(m_filePath+QDir::separator()+m_fileName+".dmp").arg(tables);
	}
	qDebug()<<cmdText;	
#ifdef SYSTEMEXECUTE
	system(cmdText.toLocal8Bit().data());
#else
#endif
	return;
}

//清除电量库中相关表,使用drop table命令
void DbUtilityDlg::cleanDL()
{
	int			i;
	QFile		file;
	QTextStream textStream;
	QStringList strListTabs;
	QString		cmdText;
	
	//获得需要清除的表名	
	GetDLTables(strListTabs);

	file.setFileName(CLEARDBSCRIPT);
	file.remove();
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		textStream.setDevice(&file);
		for(i=0;i<strListTabs.size();i++)
		{
			textStream << "drop table " << strListTabs[i] << ";" <<endl;
		}
		textStream << "commit;" << endl;
		textStream << "exit";
	}
	file.close();
	//执行命令
#ifdef SYSTEMEXECUTE
	cmdText=QString("sqlplus %1/%2@%3 @%4").
		arg(m_userName).arg(m_password).arg(dl_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb"))
		.arg(CLEARDBSCRIPT);
	system(cmdText.toLocal8Bit().data());
#else
#endif	
	file.remove();
	return;
}

//清除电量库中相关表,使用drop table命令
void DbUtilityDlg::cleanPWTables()
{
	int			i;
	QFile		file;
	QTextStream textStream;
	QStringList strListTabs;
	QString		cmdText;
	
	//获得需要清除的表名	
	GetPWTables(strListTabs);
	
	file.setFileName(CLEARDBSCRIPT);
	file.remove();
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		textStream.setDevice(&file);
		for(i=0;i<strListTabs.size();i++)
		{
			textStream << "drop table " << strListTabs[i] << ";" <<endl;
		}
		textStream << "commit;" << endl;
		textStream << "exit";
	}
	file.close();
	//执行命令
#ifdef SYSTEMEXECUTE
	cmdText=QString("sqlplus %1/%2@%3 @%4").
		arg(m_userName).arg(m_password).arg(dd_ui->chkMD->isChecked() ? tr("nbdb"): tr("nbdbb"))
		.arg(CLEARDBSCRIPT);
	system(cmdText.toLocal8Bit().data());
#else
#endif	
	file.remove();
	return;
}

void DbUtilityDlg::userSelChanged()
{
	switch(m_nDbStyle)
	{
	case DD_DB_STYLE:
		if(!m_filePath.isEmpty() && dd_ui->tabWidget->currentIndex()==0)  //必须是导出选项时，自动生成文件名
		{
			m_fileName=getPWFilename();
			dd_ui->txtExpFile->setText(m_filePath+QDir::separator()+m_fileName+".dmp");
		}
		break;
	case PB_DB_STYLE:
		break;
	case DL_DB_STYLE:
		if(!m_filePath.isEmpty() && dl_ui->tabWidget->currentIndex()==0)  //必须是导出选项时，自动生成文件名
		{
			m_fileName=getDLFilename();
			dl_ui->txtExpFile->setText(m_filePath+QDir::separator()+m_fileName+".dmp");
		}
		break;
	}
}

QString DbUtilityDlg::getDLFilename()
{
	QString fileName("");
	fileName="DL";
	if(dl_ui->chk_PT->isChecked()) fileName+="_para";
	if(dl_ui->chkJST->isChecked()) fileName+="_JS";
	if(dl_ui->chkXL->isChecked())  fileName+="_XL";
	if(dl_ui->chkBMT->isChecked()) fileName+="_MA";
	if(dl_ui->chkDLT->isChecked()) fileName+="_DL";
	if(dl_ui->chkSSLT->isChecked()) fileName+="_SSL";
	if(dl_ui->chkODLT->isChecked()) fileName+="_OL";
	if(dl_ui->chkET->isChecked()) fileName+="_ET";
	if(dl_ui->chkBMT->isChecked() || dl_ui->chkDLT->isChecked() || dl_ui->chkSSLT->isChecked() ||
		dl_ui->chkODLT->isChecked() || dl_ui->chkET->isChecked())
	{
		fileName+=dl_ui->dtST->date().toString("_yyyyMM");
		fileName+=dl_ui->dtET->date().toString("_yyyyMM");
	}
	return fileName;
}
QString DbUtilityDlg::getPWFilename()
{
	QString fileName("");
	fileName="PW";
	if(dd_ui->chk_PT->isChecked()) fileName+="_para";
	if(dd_ui->chkMT->isChecked()) fileName+="_MT";
	if(dd_ui->chkMCT->isChecked()) fileName+="_MCT";
	if(dd_ui->chkET->isChecked()) fileName+="_ET";
	if(dd_ui->chkYT->isChecked()) fileName+="_YT";
	if(dd_ui->chkMT->isChecked() || dd_ui->chkMCT->isChecked() || dd_ui->chkET->isChecked() ||
		dd_ui->chkYT->isChecked())
	{
		fileName+=dd_ui->dtST->date().toString("_yyyyMM");
		fileName+=dd_ui->dtET->date().toString("_yyyyMM");
	}
	return fileName;
}

int DbUtilityDlg::readPWTables( QStringList& tables )
{
	DBApi db ;
//	int dbtype = DBTYPE_ORACLE ;

	CS_RETCODE      retcode = 0; 
	FUNC_STR			func;
	CS_DATAFMT*		datafmt=NULL;
	int							ptcnt = 0;
	int	i , j  ;

	char * col_name[] = { "TABLE_NAME" } ;

	if(db.isDBType(DBTYPE_SYBASE))  //Sybase数据库
	{
		col_name[ 0 ] = "TABNAME" ;
		strcpy(func.table,"SYSCAT.TABLES");
		strcpy(func.dbname,"modeldb");
//		dbtype = DBTYPE_SYBASE;
		m_dbtype = DBTYPE_SYBASE;
	}
	else if(db.isDBType(DBTYPE_ORACLE))  //oracle数据库
	{
		col_name[ 0 ] = "TABLE_NAME" ;
		strcpy(func.table,"user_tables");
		strcpy(func.dbname,"modeldb");
//		dbtype = DBTYPE_ORACLE;
		m_dbtype = DBTYPE_ORACLE;
	}
	else if(db.isDBType(DBTYPE_DMSQL))  //oracle数据库
	{
		col_name[ 0 ] = "TABLE_NAME" ;
		strcpy(func.table,"user_tables");
		strcpy(func.dbname,"modeldb");
//		dbtype = DBTYPE_DMSQL;
		m_dbtype = DBTYPE_DMSQL;
	}
	else if(db.isDBType(DBTYPE_SQLSERVER)) //   SQLSERVER
	{
		col_name[ 0 ] = "NAME" ;
		strcpy(func.table,"modeldb.dbo.sysobjects");
		strcpy(func.dbname,"modeldb");
//		dbtype = DBTYPE_SQLSERVER;
		m_dbtype = DBTYPE_SQLSERVER;
	}
	else if(db.isDBType(DBTYPE_MYSQL)) //   MYSQL
	{
		col_name[ 0 ] = "TABLE_NAME" ;
		strcpy(func.table,"INFORMATION_SCHEMA.TABLES");
		strcpy(func.dbname,"modeldb");
//		dbtype = DBTYPE_MYSQL;
		m_dbtype = DBTYPE_MYSQL;
	}

	if( !db.Open( DB_OP_R, FALSE, FALSE ) )
	{
// 		QMessageBox::warning(this,tr("警告"),tr("数据库接口打开失败!"));
		DMessageBox::warning( this , tr( "警告" ) ,  tr("数据库接口打开失败！") ) ;
		return FALSE;
	}

	datafmt = 0;
	void *pt=0;
	func.func = GET_TABLE_INFO;
	if( db.GetTableInfo( &func , &datafmt ) == CS_FAIL )
	{
		if( datafmt )	free( datafmt );
		datafmt = NULL ;
		db.Close() ;	
		return FALSE ;
	}

	if ( func.colnum<1 || !datafmt )
	{
		if( datafmt )	free( datafmt );
		datafmt = NULL ;
		db.Close() ;	
		return FALSE ;
	}

	int   col_num = sizeof(col_name)/sizeof(char*);
	for( i = 0 ; i < func.colnum ; i++ )
	{
		func.col[i].select = NO;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=NO;
		for( j = 0 ; j < col_num ;  j++ )
		{
			if(strcmp(datafmt[i].name,col_name[j]))	continue;
			func.col[i].select = YES;
			func.col[i].order = j ;
			func.col[i].order_d = j;
		}
	}

	func.func = SEL_ISQL_RECORDERS;

	if ( m_dbtype==DBTYPE_DB2 )
	{
		strcpy( func.isql , "DEFINER='modeldb' and TYPE='T' \
							and TABNAME not like 'TMP_%' \
							and TABNAME not like 'YC%' \
							and TABNAME not like 'YX%' \
							and TABNAME not like 'KWH%' \
							and TABNAME not like 'BK%' \
							and TABNAME not like 'DAYYC%' \
							and TABNAME not like 'DAYKWH%' \
							and TABNAME not like 'MONTHYC%' \
							and TABNAME not like 'MONTHKWH%' \
							and TABNAME not like 'YEARYC%' \
							and TABNAME not like 'YEARKWH%' \
							and TABNAME not like 'SDATA%' \
							and TABNAME not like '电力历史事项表%' \
							and TABNAME not like '操作历史事项表%' \
							and TABNAME not like '设备运行统计表%' \
							and TABNAME not like 'PDRDESCRIBE%' \
							and TABNAME not like 'PDRDATA%' \
							and TABNAME not like '开关变位统计表%' ");
	}
 	else if(m_dbtype==DBTYPE_ORACLE)
 	{
		strcpy( func.isql , "table_name not like 'TMP_%' \
							and table_name not like 'YC%' \
							and table_name not like 'YX%' \
							and table_name not like 'KWH%' \
							and table_name not like 'BK%' \
							and table_name not like 'DAYYC%' \
							and table_name not like 'DAYKWH%' \
							and table_name not like 'MONTHYC%' \
							and table_name not like 'MONTHKWH%' \
							and table_name not like 'YEARYC%' \
							and table_name not like 'YEARKWH%' \
							and table_name not like 'SDATA%' \
							and table_name not like '电力历史事项表%' \
							and table_name not like '操作历史事项表%' \
							and table_name not like '设备运行统计表%' \
							and table_name not like 'PDRDESCRIBE%' \
							and table_name not like 'PDRDATA%' \
							and table_name not like '开关变位统计表%' ") ;
	}
	else if(m_dbtype==DBTYPE_DMSQL)
	{
		strcpy( func.isql , "table_name not like 'TMP_%' \
							and table_name not like 'YC%' \
							and table_name not like 'YX%' \
							and table_name not like 'KWH%' \
							and table_name not like 'BK%' \
							and table_name not like 'DAYYC%' \
							and table_name not like 'DAYKWH%' \
							and table_name not like 'MONTHYC%' \
							and table_name not like 'MONTHKWH%' \
							and table_name not like 'YEARYC%' \
							and table_name not like 'YEARKWH%' \
							and table_name not like 'SDATA%' \
							and table_name not like '电力历史事项表%' \
							and table_name not like '操作历史事项表%' \
							and table_name not like '设备运行统计表%' \
							and table_name not like 'PDRDESCRIBE%' \
							and table_name not like 'PDRDATA%' \
							and table_name not like '开关变位统计表%' ") ;
	}
	else if(m_dbtype==DBTYPE_MYSQL)
	{
		strcpy( func.isql , "table_schema = 'modeldb' and table_type = 'base table' \
							and table_name not like 'TMP_%' \
							and table_name not like 'YC%' \
							and table_name not like 'YX%' \
							and table_name not like 'KWH%' \
							and table_name not like 'BK%' \
							and table_name not like 'DAYYC%' \
							and table_name not like 'DAYKWH%' \
							and table_name not like 'MONTHYC%' \
							and table_name not like 'MONTHKWH%' \
							and table_name not like 'YEARYC%' \
							and table_name not like 'YEARKWH%' \
							and table_name not like 'SDATA%' \
							and table_name not like '电力历史事项表%' \
							and table_name not like '操作历史事项表%' \
							and table_name not like '设备运行统计表%' \
							and table_name not like 'PDRDESCRIBE%' \
							and table_name not like 'PDRDATA%' \
							and table_name not like '开关变位统计表%' ") ;
		
	}
	else
	{
		strcpy( func.isql , " type='U' \
							 and name not like 'TMP_%' \
							 and name not like 'YC%' \
							 and name not like 'YX%' \
							 and name not like 'KWH%' \
							 and name not like 'BK%' \
							 and name not like 'DAYYC%' \
							 and name not like 'DAYKWH%' \
							 and name not like 'MONTHYC%' \
							 and name not like 'MONTHKWH%' \
							 and name not like 'YEARYC%' \
							 and name not like 'YEARKWH%' \
							 and name not like 'SDATA%' \
							 and name not like '电力历史事项表%' \
							 and name not like '操作历史事项表%' \
							 and name not like '设备运行统计表%' \
							 and name not like 'PDRDESCRIBE%' \
							 and name not like 'PDRDATA%' \
							 and name not like '开关变位统计表%' " ) ;
	}


	if( CS_SUCCEED != db.SelectRcds( &func , datafmt , &pt ) )  
	{
		if(datafmt)	free(datafmt);
		datafmt = NULL;
		if(pt)	free(pt);
		pt = NULL;
		db.Close();	
		return FALSE;
	}

	if( func.ret_roxnum == 0 )
	{
		if(datafmt)	free(datafmt);
		datafmt = NULL;
		if(pt)	free(pt);
		pt = NULL;
		db.Close();	
		return FALSE;
	}

	int count;
	char tmpbuf[ 1024 ];

	QString table_name ;
	tables.clear() ;

	for( i = 0; i < func.ret_roxnum; i++  )
	{	
		count = 0;
		db.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,tmpbuf);
		table_name  = _C2Q( tmpbuf );
		tables.append( table_name ) ;
	}
	if (pt) free(pt);
	if (datafmt) free(datafmt);

	db.Close();
	return TRUE ;
}

void DbUtilityDlg::ini_style_sheet( void )
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QToolButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font: 16px;\
						   color:#FFFFFF;\
						   min-height: 40px;\
						   max-height: 40px;\
						   min-width: 58px ;\
						   }\
						   QToolButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:disabled\
						   {\
						   background: #D5D5D5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   #toolbarwidget\
						   {\
						   spacing:5px;\
						   background: #FAFAFA;\
						   min-height: 64px;\
						   max-height:64px;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QTableWidget \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 12px;\
						   color:#4A4A4A;\
						   background: #FAFAFA;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QTableWidget::item\
						   {\
						   text-align:center;\
						   }\
						   QScrollBar\
						   {\
						   background:white;\
						   height:8px;\
						   }\
						   QScrollBar::handle\
						   {\
						   background:lightgray;\
						   border:2px solid transparent;\
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar:vertical\
						   {\
						   width:8px;\
						   background:white;\
						   padding-top:9px;\
						   padding-bottom:9px;\
						   }\
						   QScrollBar::handle:vertical\
						   {\
						   background:lightgray; \
						   width:8px;\
						   min-height:20;\
						   border:2px solid transparent; \
						   border-radius:4px;\
						   }\
						   QScrollBar::handle:vertical:hover\
						   {\
						   background:gray;\
						   }\
						   QScrollBar::sub-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QScrollBar::add-line:vertical\
						   {\
						   background:transparent;\
						   }\
						   QPushButton\
						   {\
						   background:#4A90E2;\
						   border-radius: 2px;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: white;\
						   min-width:75px;\
						   min-height:25px;\
						   }\
						   QPushButton:hover\
						   {\
						   background:#0C2543;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QCheckBox::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QCheckBox::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QComboBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QListView,QTreeView\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   border: 1px solid #E5E5E5;\
						   }\
						   QListView::item:hover,QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QListView::item:selected,QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QListView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QListView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
						   }\
						   QSpinBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QSpinBox::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::up-arrow \
						   {\
						   image: url(:/res/add.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QSpinBox::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QSpinBox::down-arrow \
						   {\
						   image: url(:/res/remove.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 40px;\
						   max-height: 40px;\
						   }\
						   QDateTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit::up-arrow:hover \
						   {\
						   background:#4A90E2;\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit::down-arrow:hover \
						   {\
						   background:#4A90E2;\
						   width: 18px;\
						   height: 18px;\
						   }\
						   	QDateTimeEdit::up-button,QDateTimeEdit::down-button \
						   {\
							background: #FAFAFA;\
							}\
							QTabBar::tab\
							{\
							color:#0F1923;\
							font-family:Microsoft YaHei;\
							font-size: 13px;\
							min-width:100px;\
							min-height:30px;\
							border:0px;\
							}\
							QTabBar::tab:hover\
							{\
							background:rgb(255,255, 255, 100);\
							}\
							QTabBar::tab:selected\
							{\
							border-color:white;\
							background:white;\
							color:#5D9CE5;\
							border-bottom: 2px solid #4A90E2;\
							}\
							QLineEdit\
							{\
							font-family: Microsoft YaHei;\
							font-size: 13px;\
							background: #FFFFFF;\
							border: 1px solid #C6CFD9;\
							border-radius: 4px;\
							min-height: 40px;\
							max-height: 40px;\
							}\
							QRadioButton::indicator:unchecked\
							{\
							image:url(:/res/radio.png);\
							}\
							QRadioButton::indicator:checked\
							{\
							image:url(:/res/radio_checked.png);\
							}\
							QLabel,QGroupBox,QRadioButton\
							{\
							font-family: Microsoft YaHei;\
							font-size: 14px;\
							color:#3E464E;\
							}\
							") ;

	setStyleSheet( buttonStyleSheet ) ;
}
