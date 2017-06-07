#include "mainwindow.h"
#include "globaldef.h"
#include "settingdialog.h"
#include <fstream>
#include <windows.h>
using namespace std;

SETTINGS g_settings;
CLIENTSTAT* g_pClientStat;
int g_stop;
int g_lintenNum;
int g_linkNum;

mainWindow::mainWindow()
:QMainWindow()
{
	m_pStartListen = new QPushButton(_C2Q("开始监听"));
	m_pStartStorm = new QPushButton(_C2Q("开始发送"));
	m_pSetting = new QPushButton(_C2Q("设置"));
	m_pExit = new QPushButton(_C2Q("退出"));

	m_pStatTable = new QTableWidget(this);
	m_pVSplitter = new QSplitter(Qt::Vertical, this);
	m_pHSplitter = new QSplitter(Qt::Horizontal, this);

	m_pShowTimer = new QTimer(this);
	m_pStormTimer = new QTimer(this);
	m_pStatusLabel = new QLabel(this);
	m_pStormNumLabel = new QLabel(this);

	m_pStatTable->setColumnCount(6);
	m_pStatTable->setRowCount(100);
	m_pStatTable->setHorizontalHeaderLabels(QStringList()<<_C2Q("监听端口")
														 <<_C2Q("监听状态")
														 <<_C2Q("客户端1地址")
														 <<_C2Q("客户端2地址")
														 <<_C2Q("发送遥信次数")
														 <<_C2Q("发送SOE次数")
											);
	m_pStatTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

	m_pHSplitter->addWidget(m_pStartListen);
	m_pHSplitter->addWidget(m_pStartStorm);
	m_pHSplitter->addWidget(m_pSetting);
	m_pHSplitter->addWidget(m_pExit);

	m_pVSplitter->addWidget(m_pHSplitter);
	m_pVSplitter->addWidget(m_pStatTable);

	m_pVSplitter->setStretchFactor(0, 1);
	m_pVSplitter->setStretchFactor(1, 10);

	statusBar()->addWidget(m_pStatusLabel, 1);
	statusBar()->addWidget(m_pStormNumLabel, 1);

	setCentralWidget(m_pVSplitter);
	setGeometry(300, 100, 450, 500);
	setWindowTitle(_C2Q("雪崩测试工具"));
	setWindowIcon( QIcon( ":res/storm.png") ) ;

	connect(m_pStartListen, SIGNAL(clicked()), this, SLOT(onListen()));
	connect(m_pStartStorm, SIGNAL(clicked()), this, SLOT(onStorm()));
	connect(m_pSetting, SIGNAL(clicked()), this, SLOT(onSetting()));
	connect(m_pExit, SIGNAL(clicked()), this, SLOT(onExit()));
	connect(m_pShowTimer, SIGNAL(timeout()), this, SLOT(onShowTimer()));
	connect(m_pStormTimer, SIGNAL(timeout()), this, SLOT(onStormTimer()));

	m_pStartListen->setEnabled(false);
	m_pStartStorm->setEnabled(false);
	defaultSetting();
	m_stormCount = 0;
	g_stop = 0;
	g_linkNum = 0;
	g_lintenNum = 0;
	m_ppProtocol = NULL;

// 	initStyleSheet();
}


void mainWindow::defaultSetting(void)
{
	g_settings.clientNum = 800;
	g_settings.clientYxNum = 400;
	g_settings.clientYcNum = 150;
	g_settings.changeYcNum = 2;
	g_settings.changeYxNum = 2;
	g_settings.sendGap = 1000;
	g_settings.startPort = 10001;
	g_settings.countLimit = 100;
}


void mainWindow::refleshStatTable(void)
{
	m_pStatTable->setRowCount(g_settings.clientNum);

	for (int i = 0; i < g_settings.clientNum; i++)
	{
		if (g_pClientStat[i].listenSock > 0)
		{
			m_pStatTable->item(i, 1)->setText(_C2Q("监听成功"));
		}

		if (g_pClientStat[i].linkSock > 0)
		{
			m_pStatTable->item(i, 2)->setText(_C2Q(g_pClientStat[i].ip));
		}
		else
		{
			m_pStatTable->item(i, 2)->setText(_C2Q(""));
		}

		if (g_pClientStat[i].linkSock2 > 0)
		{
			m_pStatTable->item(i, 3)->setText(_C2Q(g_pClientStat[i].ip2));
		}
		else
		{
			m_pStatTable->item(i, 3)->setText(_C2Q(""));
		}
		m_pStatTable->item(i, 4)->setText(QString::number(m_ppProtocol[i]->m_yxSendCount));
		m_pStatTable->item(i, 5)->setText(QString::number(m_ppProtocol[i]->m_soeSendCount));
	}
}


void mainWindow::onListen(void)
{
	m_listemThread.start();
	m_pStartListen->setEnabled(false);
	m_pSetting->setEnabled(false);
	//m_pStartStorm->setEnabled(true);

	for (int i = 0; i < g_settings.clientNum; i++)
	{
		m_ppProtocol[i]->init();
		m_ppProtocol[i]->start();
	}
	m_pShowTimer->start(1000);
}

void mainWindow::onStorm(void)
{
	m_pStartStorm->setEnabled(false);
	m_stormCount = 0;
	m_pStormTimer->start(g_settings.sendGap);
// 	for (int i = 0; i < g_settings.clientNum; i++)
// 	{
// 		m_ppProtocol[i]->addAStorm();
// 	}
}

void mainWindow::onSetting(void)
{
	int i;
	QSetDialog dlg(this);

	if (m_listemThread.isRunning())
	{
		m_listemThread.stop();
		m_listemThread.terminate();
	}

	if (m_ppProtocol != NULL)
	{
		for (i = 0; i < g_settings.clientNum; i++)
		{
			m_ppProtocol[i]->stop();
// 			if (m_ppProtocol[i]->isRunning())
// 			{
// 				m_ppProtocol[i]->stop();
// 				m_ppProtocol[i]->terminate();
// 			}
		}

		delete[] m_ppProtocol;
		m_ppProtocol = NULL;
	}


	m_pStatTable->clearContents();

	delete[] g_pClientStat;
	g_pClientStat = NULL;

	dlg.exec();

	g_pClientStat = new CLIENTSTAT[g_settings.clientNum];
	m_ppProtocol = new QProtocolThread*[g_settings.clientNum];

	m_pStatTable->setRowCount(g_settings.clientNum);

	for (i = 0; i < g_settings.clientNum; i++)
	{
		m_ppProtocol[i] = new QProtocolThread(this, i);
		g_pClientStat[i].listenSock = -1;
		g_pClientStat[i].linkSock = -1;

		m_pStatTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + g_settings.startPort)));
		m_pStatTable->setItem(i, 1, new QTableWidgetItem(_C2Q("未监听")));
		m_pStatTable->setItem(i, 2, new QTableWidgetItem(_C2Q("")));
		m_pStatTable->setItem(i, 3, new QTableWidgetItem(_C2Q("")));
		m_pStatTable->setItem(i, 4, new QTableWidgetItem(_C2Q("")));
		m_pStatTable->setItem(i, 5, new QTableWidgetItem(_C2Q("")));
	}
	m_pStartListen->setEnabled(true);	
}

void mainWindow::onExit(void)
{
	g_stop = 1;
	QMainWindow::close();	
}

void mainWindow::onShowTimer(void)
{
	int i;
	int allReady = 1;
	refleshStatTable();
	QString tmpStr = _C2Q("监听端口数%1, 连接数%2")
		.arg(g_lintenNum)
		.arg(g_linkNum);
	m_pStatusLabel->setText(tmpStr);

	for (i = 0; i < g_settings.clientNum; i++)
	{
		if (g_pClientStat[i].linkSock > 0 || g_pClientStat[i].linkSock2 > 0)
		{
			allReady *= 1;
		}
		else
		{
			allReady *= 0;
		}
	}

	if (1 == allReady && !m_pStormTimer->isActive())
	{
		m_pStartStorm->setEnabled(true);
	}	
}

void mainWindow::onStormTimer(void)
{
	int i;
//	bool allReady = false;

	for (i = 0; i < g_settings.clientNum; i++)
	{
		m_ppProtocol[i]->addAStorm();
	}
	m_stormCount++;

	QString tmpStr = _C2Q("发送第%1波风暴")
		.arg(m_stormCount);
	m_pStormNumLabel->setText(tmpStr);

	if (m_stormCount >= g_settings.countLimit)
	{
		m_pStormTimer->stop();
	}
}

void mainWindow::closeEvent(QCloseEvent *event)
{
// 	int i;
// 	if (m_listemThread.isRunning())
// 	{
// 		m_listemThread.stop();
// 		m_listemThread.terminate();
// 	}
// 	
// 	if (m_ppProtocol != NULL)
// 	{
// 		for (i = 0; i < g_settings.clientNum; i++)
// 		{
// 			m_ppProtocol[i]->stop();
// 		}
// 		
// 		delete[] m_ppProtocol;
// 		m_ppProtocol = NULL;
// 	}
// 
// 	Sleep(2000);
	event->accept();
}

void mainWindow::initStyleSheet( void )
{
	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QToolBar\
						   {\
						   spacing:10px;\
						   min-height:35px;\
						   background:#FAFAFA;\
						   }\
						   QToolButton\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   min-height:25px;\
						   }\
						   QToolButton:checked\
						   {\
						   background: #FFFFFF;\
						   border: 1px solid #4A90E2;\
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
						   QComboBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox::drop-down\
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
						   QTreeView::indicator:unchecked\
						   {\
						   image:url(:/zonemng/png/checkbox_unchecked.png);\
						   }\
						   QTreeView::indicator:checked\
						   {\
						   image:url(:/zonemng/png/checkbox_checked.png);\
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
						   image: url(:/res/spinup.png);\
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
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QDateTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color: #3E464E;\
						   min-height:25px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QDateTimeEdit::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QDateTimeEdit::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QToolButton#qt_calendar_prevmonth,QToolButton#qt_calendar_nextmonth,QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton\
						   {\
						   border:0px solid #000000;\
						   border-radius:3px;\
						   margin:3px 3px 3px 3px;\
						   padding:3px;\
						   background-color:rgba(0,0,0,0);\
						   color:#FFFFFF;\
						   }\
						   QToolButton#qt_calendar_prevmonth:hover,QToolButton#qt_calendar_nextmonth:hover,QToolButton#qt_calendar_monthbutton:hover,QToolButton#qt_calendar_yearbutton:hover,QToolButton#qt_calendar_prevmonth:pressed,QToolButton#qt_calendar_nextmonth:pressed,QToolButton#qt_calendar_monthbutton:pressed,QToolButton#qt_calendar_yearbutton:pressed\
						   {\
						   border:1px solid #D8D8D8;\
						   }\
						   QCalendarWidget QSpinBox#qt_calendar_yearedit\
						   {\
						   margin:3px 3px 3px 3px;\
						   padding:0px 0px 0px 0px;\
						   font-family: Microsoft YaHei;\
						   font-size: 9px;\
						   background-color:rgba(0,0,0,0);\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 25px;\
						   max-height: 25px;\
						   min-width:70px;\
						   max-width:70px;\
						   }\
						   QCalendarWidget QToolButton::menu-indicator\
						   {\
						   image:None;\
						   }\
						   QCalendarWidget QTableView\
						   {\
						   border-width:0px;\
						   }\
						   QCalendarWidget QWidget#qt_calendar_navigationbar\
						   {\
						   border:1px solid #575757;\
						   border-width:1px 1px 0px 1px;\
						   background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #4D4D4D,stop:1 #292929);\
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
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QRadioButton::indicator:unchecked\
						   {\
						   image:url(:/res/radio.png);\
						   }\
						   QRadioButton::indicator:checked\
						   {\
						   image:url(:/res/radio_checked.png);\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   min-height: 25px;\
						   max-height: 25px;\
						   }\
						   QGroupBox,QRadioButton,QDockWidget\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#3E464E;\
						   }\
						   QMenuBar\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:14px;\
						   color:#3E464E;\
						   background: #FAFAFA;\
						   min-width:60px;\
						   min-height:32px;\
						   border:1px solid #D5D5D5;\
						   }\
						   QMenuBar::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QMenu\
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:black;\
						   background-color:#FAFAFA;\
						   }\
						   QMenu::item:selected\
						   {\
						   background:#4A90E2;\
						   color:white;\
						   }\
						   QTimeEdit \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTimeEdit::up-button \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: right;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::up-arrow \
						   {\
						   image: url(:/res/spinup.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QTimeEdit::down-button \
						   {\
						   subcontrol-origin: border;\
						   subcontrol-position: left;\
						   width: 24px;\
						   height: 24px;\
						   border-width: 3px;\
						   }\
						   QTimeEdit::down-arrow \
						   {\
						   image: url(:/res/spindown.png);\
						   width: 18px;\
						   height: 18px;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   min-width:50px;\
						   height:30px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}


