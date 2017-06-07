#include "settingdialog.h"
#include "globaldef.h"
#include "rwxml.h"
#include "uitools/dmessagebox.h"
#include <QtGui/QMessageBox>

extern SETTINGS g_settings;

QSetDialog::QSetDialog(QWidget* parent)
:QDialog(parent)
{
	m_pClientNumLabel = new QLabel(_C2Q("终端数量:"), this);
	m_pClientYxLabel = new QLabel(_C2Q("各终端遥信数量:"), this);
	m_pClientYcLabel = new QLabel(_C2Q("各终端遥测数量:"), this);
	m_pClientChgYXLabel = new QLabel(_C2Q("各终端每次遥信变位数:"), this);
	m_pClientChgYCLabel = new QLabel(_C2Q("各终端每次遥测变化数:"), this);
	m_pSendGapLabel = new QLabel(_C2Q("发送间隔:"), this);
	m_pStartPortLabel = new QLabel(_C2Q("起始端口号:"), this);
	m_pSendCountLabel = new QLabel(_C2Q("发送次数:"), this);

	char szFileName[ 256 ] ;
// 	sprintf( szFileName , "%s/ini/storm.ini" , getenv("NBENV") ) ;	
// 	ini_RW* pini  =  new ini_RW( szFileName ) ;
	sprintf( szFileName , "%s/resource/xml/storm.xml" , getenv("NBENV") ) ;
	Xml_RW* pini  =  new Xml_RW( szFileName ) ;

	if( SUCCESS != pini->get_int( "雪崩配置", "终端数量", g_settings.clientNum ) || 
		 SUCCESS != pini->get_int( "雪崩配置", "各终端遥信数量", g_settings.clientYxNum ) ||
		 SUCCESS != pini->get_int( "雪崩配置", "各终端遥测数量", g_settings.clientYcNum ) ||
		 SUCCESS != pini->get_int( "雪崩配置", "各终端每次遥信变位数", g_settings.changeYcNum ) ||
		 SUCCESS != pini->get_int( "雪崩配置", "各终端每次遥测变位数", g_settings.changeYxNum ) ||
		 SUCCESS != pini->get_int( "雪崩配置", "发送间隔", g_settings.sendGap ) || 
		 SUCCESS != pini->get_int( "雪崩配置", "起始端口号", g_settings.startPort ) ||
		 SUCCESS != pini->get_int( "雪崩配置", "发送次数", g_settings.countLimit ) )
	{	
		DMessageBox :: warning( this , _C2Q( "警告 ") , _C2Q( "读取配置文件失败！") ) ;
	}

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}

	m_pClientNum = new QLineEdit(QString::number(g_settings.clientNum), this);
	m_pClientYxNum = new QLineEdit(QString::number(g_settings.clientYxNum), this);
	m_pClientYcNum = new QLineEdit(QString::number(g_settings.clientYcNum), this);
	m_pClientChgYxNum = new QLineEdit(QString::number(g_settings.changeYxNum), this);
	m_pClientChgYcNum = new QLineEdit(QString::number(g_settings.changeYcNum), this);
	m_pSendGap = new QLineEdit(QString::number(g_settings.sendGap), this);
	m_pStartPort = new QLineEdit(QString::number(g_settings.startPort), this);
	m_pSendCount = new QLineEdit(QString::number(g_settings.countLimit), this);
	
	m_pOK = new QPushButton(_C2Q("确定"), this);
	m_pCancel = new QPushButton(_C2Q("取消"), this);  
	
	m_pGridlayout = new QGridLayout(this);

	m_pGridlayout->addWidget(m_pClientNumLabel, 0, 0);
	m_pGridlayout->addWidget(m_pClientYxLabel, 1, 0);
	m_pGridlayout->addWidget(m_pClientYcLabel, 2, 0);
	m_pGridlayout->addWidget(m_pClientChgYXLabel, 3, 0);
	m_pGridlayout->addWidget(m_pClientChgYCLabel, 4, 0);
	m_pGridlayout->addWidget(m_pSendGapLabel, 5, 0);
	m_pGridlayout->addWidget(m_pStartPortLabel, 6, 0);
	m_pGridlayout->addWidget(m_pSendCountLabel, 7, 0);

	m_pGridlayout->addWidget(m_pClientNum, 0, 1);
	m_pGridlayout->addWidget(m_pClientYxNum, 1, 1);
	m_pGridlayout->addWidget(m_pClientYcNum, 2, 1);
	m_pGridlayout->addWidget(m_pClientChgYxNum, 3, 1);
	m_pGridlayout->addWidget(m_pClientChgYcNum, 4, 1);
	m_pGridlayout->addWidget(m_pSendGap, 5, 1);
	m_pGridlayout->addWidget(m_pStartPort, 6, 1);
	m_pGridlayout->addWidget(m_pSendCount, 7, 1);

	m_pGridlayout->addWidget(m_pOK, 8, 0);
	m_pGridlayout->addWidget(m_pCancel, 8, 1);

	connect(m_pOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(m_pCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

	setWindowTitle(_C2Q("设置窗口"));
}

void QSetDialog::onOK(void)
{
	g_settings.clientNum = m_pClientNum->text().toInt();
	g_settings.clientYxNum = m_pClientYxNum->text().toInt();
	g_settings.clientYcNum = m_pClientYcNum->text().toInt();
	g_settings.changeYcNum = m_pClientChgYcNum->text().toInt();
	g_settings.changeYxNum = m_pClientChgYxNum->text().toInt();
	g_settings.sendGap = m_pSendGap->text().toInt();
	g_settings.startPort = m_pStartPort->text().toInt();
	g_settings.countLimit = m_pSendCount->text().toInt();


	char szFileName[ 256 ] ;
// 	sprintf( szFileName , "%s/ini/storm.ini" , getenv("NBENV") ) ;
// 	ini_RW* pini  =  new ini_RW( szFileName ) ;
	sprintf( szFileName , "%s/resource/xml/storm.xml" , getenv("NBENV") ) ;
	Xml_RW* pini  =  new Xml_RW( szFileName ) ;

	if( SUCCESS != pini->set_int( "雪崩配置", "终端数量", g_settings.clientNum ) || 
		 SUCCESS != pini->set_int( "雪崩配置", "各终端遥信数量", g_settings.clientYxNum ) ||
		 SUCCESS != pini->set_int( "雪崩配置", "各终端遥测数量", g_settings.clientYcNum ) ||
		 SUCCESS != pini->set_int( "雪崩配置", "各终端每次遥信变位数", g_settings.changeYcNum ) ||
		 SUCCESS != pini->set_int( "雪崩配置", "各终端每次遥测变位数", g_settings.changeYxNum ) ||
		 SUCCESS != pini->set_int( "雪崩配置", "发送间隔", g_settings.sendGap ) || 
		 SUCCESS != pini->set_int( "雪崩配置", "起始端口号", g_settings.startPort ) ||
		 SUCCESS != pini->set_int( "雪崩配置", "发送次数", g_settings.countLimit ) )
	{	
		DMessageBox :: warning( this , tr( "警告 ") , tr( "保存失败！") ) ;
	}

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}

	accept();	
}

void QSetDialog::onCancel(void)
{
	reject();
}
