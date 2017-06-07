#include "dqinfo_mainwindow.h"
#include "dqinfo_string.h"
#include "dqinfo_dbgpara.h"
#include "sysdef.h"
#include "uitools/dmessagebox.h"
extern CDbgPara g_dbgPara;

// dqinfo信息栏最大显示数据行数5000，到达5000后删除最前面1000行数据
enum{
	REMOVE_DBGINFO_NUM = 1000,
	MAX_DBGINFO_NUM = 5000
};

mainWindow::mainWindow()
:QMainWindow(), m_getDbgInfo(this), m_pauseFlag(false)
{
	
	m_pTaskListSource = new QStandardItemModel(0, 3, this);
	m_pTaskListSource->setHeaderData(0, Qt::Horizontal, QObject::tr("进程名"));
	m_pTaskListSource->setHeaderData(1, Qt::Horizontal, QObject::tr("PID"));
	m_pTaskListSource->setHeaderData(2, Qt::Horizontal, QObject::tr("注册号"));
	m_pNetTree = new QNetTree(this);
// 	m_pNetTree->header()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	//m_pTaskList = new QTaskList(this);
	m_pTaskList = new QTreeView(this);
// 	m_pTaskList->header()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	m_pTaskList->setRootIsDecorated(false);
	m_pTaskList->setAlternatingRowColors(true);
	m_pTaskList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pTaskList->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTaskList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pTaskList->setModel(m_pTaskListSource);

	//m_pDbgList = new QDbgList(this);
	m_pRemoteHost = new QComboBox(this);
	m_pRemoteHost->setEditable(true);
	m_pRemoteCheck = new QCheckBox(_C2Q("远程"),this);

	m_pDbgInfoSource = new QStandardItemModel(0, 2, this);
	m_pDbgInfoSource->setHeaderData(0, Qt::Horizontal, QObject::tr("信息类型"));
	m_pDbgInfoSource->setHeaderData(1, Qt::Horizontal, QObject::tr("信息描述"));

	m_pdbgInfoList = new QTreeView(this);
	m_pdbgInfoList->header()->setDefaultAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	m_pdbgInfoList->setRootIsDecorated(false);
	m_pdbgInfoList->setAlternatingRowColors(true);
	m_pdbgInfoList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_pdbgInfoList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pdbgInfoList->setModel(m_pDbgInfoSource);
	QPalette palette = m_pdbgInfoList->palette() ;
	palette.setColor( QPalette::AlternateBase , QColor( "#D5D5D5" ) ) ;
	m_pdbgInfoList->setPalette( palette ) ;

	m_pRemoteSplitter = new QSplitter(Qt::Horizontal);
	m_pHorSplitter = new QSplitter(Qt::Horizontal);
	m_pVerSplitter = new QSplitter(Qt::Vertical);

	m_pTaskListTimer = new QTimer(this);
	//m_pFile = new QFile(QString(getenv(NBENV)) + QString("/dqinfo.txt"));
	m_pRemoteHost->setMaximumSize(200, 20);

//	setIconSize(QSize(32, 32));
	m_pToolBar = new QToolBar(this);
	m_pToolBar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon ) ;
	m_pToolBar->setIconSize( QSize( 16 , 16 ) ) ;
	m_pToolBar->setMovable(false);
	createActions();
	initRemoteHost();
//	addToolBar(m_pToolBar);
//	createMenus();

	m_pRemoteSplitter->addWidget(m_pRemoteHost);
	m_pRemoteSplitter->addWidget(m_pRemoteCheck);
	m_pRemoteSplitter->setStretchFactor(0, 4);
	m_pRemoteSplitter->setStretchFactor(1, 1);
	m_pVerSplitter->addWidget(m_pNetTree);
	m_pVerSplitter->addWidget(m_pRemoteSplitter);

	m_pVerSplitter->addWidget(m_pTaskList);
	
	m_pHorSplitter->addWidget(m_pVerSplitter);
	m_pHorSplitter->addWidget(m_pdbgInfoList);

	QFrame* pCentral_frame = new QFrame( this ) ;
	QVBoxLayout* pLayout = new QVBoxLayout( pCentral_frame ) ;
	pCentral_frame->setLayout( pLayout ) ;
	pLayout->addWidget( m_pToolBar ) ;
	pLayout->addWidget( m_pHorSplitter ) ;
	setCentralWidget(pCentral_frame);

	m_pHorSplitter->setStretchFactor(0, 1);
	m_pHorSplitter->setStretchFactor(1, 3);	

	setGeometry(200, 100, 1000, 500);
	setWindowTitle(_C2Q("调试信息查看"));	
	setWindowIcon( QIcon( ":/res/dqinfo.png" ) ) ;
	connect(m_pTaskList, SIGNAL(clicked(QModelIndex)), this, SLOT(selectTaskChange(QModelIndex)));
	connect(m_pTaskList, SIGNAL(pressed(QModelIndex)), this, SLOT(taskRightClick(QModelIndex)));
	connect(m_pRemoteCheck, SIGNAL(stateChanged(int)), this, SLOT(onRemoteModeChange(int)));
	connect(m_pTaskListTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	connect(&m_getDbgInfo, SIGNAL(sendDbgInfo(int ,QString, QString)), this, SLOT(recvDbgInfo(int, QString, QString)));
	connect(m_pdbgInfoList, SIGNAL(pressed(QModelIndex)), this, SLOT(selectDbgMsgs(QModelIndex)));
	m_saveFlag = false;
	m_getDbgInfo.start();
	m_pTaskListTimer->start(1000);

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "QToolButton,QAction\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   color:#FFFFFF;\
						   min-height: 35px;\
						   max-height: 35px;\
						   min-width: 50px ;\
						   }\
						   QToolButton:hover,QAction:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:pressed,QAction:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:checked,QAction:checked\
						   {\
						   background: #4A90E2;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:disabled,QAction:disabled\
						   {\
						   background: #D5D5D5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QFrame\
						   {\
						   background: #FAFAFA;\
						   }\
						   QSplitter\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget\
						   {\
						   background: #FAFAFA;\
						   }\
						   QTabWidget::pane\
						   {\
						   border:none;\
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
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
						   }\
						   QCheckBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 14px;\
						   color:#3E464E;\
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
						   QTreeView\
						   {\
						   border: 1px solid #E5E5E5;\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   color:#4A4A4A;\
						   }\
						   QTreeView::item \
						   {\
						   text-align: center;\
						   }\
						   QTreeView::item:hover \
						   {\
						   background-color: wheat;\
						   }\
						   QTreeView::item:selected \
						   {\
						   background-color: lightblue;\
						   }\
						   QTreeView::indicator:unchecked\
						   {\
						   image:url(:/res/checkbox_unchecked.png);\
						   }\
						   QTreeView::indicator:checked\
						   {\
						   image:url(:/res/checkbox_checked.png);\
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
						   QGroupBox\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 13px;\
						   }\
						   QHeaderView::section \
						   {\
						   font-family:Microsoft YaHei;\
						   font-size:13px;\
						   color:#0F1923;\
						   min-width:40px;\
						   height:25px;\
						   background:#F1F1F1;\
						   border: 0px solid #567dbc;\
						   border-right:1px solid #B5B5B5;\
						   border-bottom:1px solid #B5B5B5;\
						   }\
						   QToolBar\
						   {\
						   border: 1px solid #E5E5E5;\
						   background: #FFFFFF;\
						   border: 0px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 45px;\
						   max-height: 45px;\
						   spacing:5px;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;

	m_pTaskList->setColumnWidth(0, 120);
	m_pTaskList->setColumnWidth(1, 50);
	m_pTaskList->setColumnWidth(2, 30);

	m_pNetTree->setColumnWidth(0, 150);
	m_pNetTree->setColumnWidth(1, 30);
	m_pNetTree->setColumnWidth(2, 30);
}

mainWindow::~mainWindow()
{

}

void mainWindow::onTimer(void)
{
	if (!g_dbgPara.isRemote())
	{
		localTaskList();
	}
}

void mainWindow::recvDbgInfo(int typeNo, QString type, QString desc)
{
	QStandardItem* typeItem = new QStandardItem(type);
	QStandardItem* descItem = new QStandardItem(desc);
	QList<QStandardItem*> itemList;
	itemList<<typeItem<<descItem;
	
	if ((!m_StringFilter.contains(type)) && (m_StringFilter.size() != 0))
	{
		return;
	}
	
	if (m_saveFlag)
	{
		saveDbgToFile(type, desc);
	}
	
	if (m_pauseFlag)
	{
		m_tmpItems.push_back(itemList);
	}
	else
	{
		while (m_tmpItems.size() > 0)
		{
			m_pDbgInfoSource->appendRow(m_tmpItems.front());
			QModelIndex index = typeItem->index();
			m_pdbgInfoList->scrollTo(index);
			m_tmpItems.pop_front();
		}
		//m_dbgInfoSrcMudex.lockForWrite();
		m_pDbgInfoSource->appendRow(itemList);
		QModelIndex index = typeItem->index();
		m_pdbgInfoList->scrollTo(index);
		if (m_pDbgInfoSource->rowCount()> MAX_DBGINFO_NUM)
		{
			m_pDbgInfoSource->removeRows(0,REMOVE_DBGINFO_NUM);
		}
		//m_dbgInfoSrcMudex.unlock();
	}
}


void mainWindow::localTaskList(void)
{
	int i, j;
	bool findF;
	int taskNum;
	QString curPid;
	char procDes[DEBUG_MAX_PROC][DESC_LEN];
    int procNum = m_dbg.GetProcEnm(procDes);
    int* pid = m_dbg.GetProcPidEnm();
    int* idx = m_dbg.GetRegIndex();
	
	//taskNum = topLevelItemCount();
	taskNum = m_pTaskListSource->rowCount();
	//删除不存在的进程
	for (i = 0; i < taskNum; i++)
	{
		findF = false;
		//curPid = topLevelItem(i)->text(1);
		curPid = m_pTaskListSource->item(i, 1)->text();
		
		for (j = 0; j < procNum; j++)
		{
			if (pid[j] != curPid.toInt())
			{
				continue;	
			}
			findF = true;
			break;
		}
		
		if (!findF)
		{
			//takeTopLevelItem(i);
			m_pTaskListSource->takeRow(i);
			taskNum = m_pTaskListSource->rowCount();
		}
	}
	
	//添加新出现的进程
	for (i = 0; i < procNum; i++)
	{
		//taskNum = topLevelItemCount();
		taskNum = m_pTaskListSource->rowCount();
		findF = false;
		
		for (j = 0; j < taskNum; j++)
		{
			//curPid = topLevelItem(j)->text(1);
			curPid = m_pTaskListSource->item(j, 1)->text();
			if (pid[i] == curPid.toInt())
			{
				findF = true;
				break;
			}
		}
		
		if (!findF)
		{
			QList<QStandardItem *> items;
			items<<new QStandardItem(_C2Q(procDes[i]))<<new QStandardItem(QString::number(pid[i]))<<new QStandardItem(QString::number(idx[i]));
			m_pTaskListSource->appendRow(items);
			//addTopLevelItem(new QTreeWidgetItem(QStringList()<<_C2Q(procDes[i])<<QString::number(pid[i])<<QString::number(idx[i])));
		}
	}
}


void mainWindow::selectTaskChange(QModelIndex index)
{
	int row = index.row();
	QStandardItem* item = m_pTaskListSource->item(row, 2);
	int ID = item->text().toInt();
	g_dbgPara.setDbgID(ID);

	m_pPause->setEnabled(true);
	m_pContinue->setEnabled(false);
	m_pSave->setEnabled(true);
	m_pFilter->setEnabled(true);
	m_pClear->setEnabled(true);
	m_pauseFlag = false;

	QString filename;
	item = m_pTaskListSource->item(row, 0);
	if( m_pRemoteCheck->isChecked() )
	{
		filename = _C2Q( getenv(NBENV) ) + QString( "/log/" ) +
			m_pRemoteHost->currentText() + QString( "_" ) +
			item->text() + _C2Q( ".txt" );
	}
	else
	{
		filename = _C2Q( getenv(NBENV) ) + QString( "/log/" ) + item->text() + _C2Q( ".txt" );
	}
	m_file.setFileName( filename );
//	m_dbgInfoSrcMudex.lockForWrite();
	m_StringFilter.clear();
	m_pDbgInfoSource->removeRows(0, m_pDbgInfoSource->rowCount());
//	m_dbgInfoSrcMudex.unlock();
}

void mainWindow::taskRightClick(QModelIndex index)
{
	if (qApp->mouseButtons() == Qt::LeftButton)
	{
		return;
	}
	else
	{
		showTaskMenu();
	}
}

void mainWindow::showTaskMenu(void)
{
	QPoint pos;
	QMenu menu(this);
	QAction* logout = menu.addAction(_C2Q("注销进程"));
	QAction* kill = menu.addAction(_C2Q("杀死进程"));
	connect(logout, SIGNAL(triggered()), this, SLOT(logOutTask()));
	connect(kill, SIGNAL(triggered()), this, SLOT(killTask()));
	menu.exec(QCursor::pos());
}

void mainWindow::logOutTask(void)
{
	QItemSelectionModel* selectedModel = m_pTaskList->selectionModel();
	QModelIndexList indexes = selectedModel->selectedIndexes();
	QStandardItem* idxItem;
	
	idxItem = m_pTaskListSource->itemFromIndex(indexes[2]);
	
	if (!g_dbgPara.isRemote())
	{
		m_dbg.UnRegProc(idxItem->text().toInt());
	}
}

void mainWindow::killTask(void)
{
	QItemSelectionModel* selectedModel = m_pTaskList->selectionModel();
	QModelIndexList indexes = selectedModel->selectedIndexes();
	QStandardItem* pidItem;
	
	pidItem = m_pTaskListSource->itemFromIndex(indexes[1]);
	
	if (!g_dbgPara.isRemote())
	{
		NetStatus cfg;
		NetControl netCtrl;
		int hostNo = cfg.GetHostNo();
		char * host = cfg[hostNo].hostname;
		netCtrl.KillProc(1, host, pidItem->text().toInt());
	}
}


void mainWindow::createActions(void)
{
	m_pTopWindow = new QAction(QIcon(":/res/go-top-3.png"), tr(" 前端显示"), this);
	//m_pRemote = new QAction(QIcon(":/res/remote-desktop.png"), tr("远程监视"), this);
	m_pPause = new QAction(QIcon(":/res/media-playback-pause.png"), tr(" 暂停显示"), this);
	m_pContinue = new QAction(QIcon(":/res/media-playback-start.png"), tr(" 继续显示"), this);
	m_pClear = new QAction(QIcon(":/res/edit-clear-list.png"), tr(" 清除信息"), this);
	m_pSave = new QAction(QIcon(":/res/document-save-5.png"), tr(" 保存信息"), this);
	m_pSave->setToolTip( tr( "信息被保存在D6000根目录下") ) ;

	m_pCheckTime = new QAction(QIcon(":/res/edit-history.png"), tr(" 系统对时"), this);
	m_pFilter = new QAction(QIcon(":/res/view-filter.png"), tr(" 信息过滤"), this);
	m_pExit = new QAction(QIcon(":/res/application-exit.png"), tr(" 退出程序"), this);
	m_pCopy = new QAction(QIcon(":/res/edit-copy-3.png"), tr(" 复制信息"), this);
	//m_pShowNode = new QAction(QIcon(":/res/computer-4.png"), tr("显示节点"), this);

	m_pTopWindow->setCheckable(true);
	//m_pRemote->setCheckable(true);
	m_pSave->setCheckable(true);
	connect(m_pTopWindow, SIGNAL(triggered()), this, SLOT(setWindowTop()));
	//connect(m_pRemote, SIGNAL(triggered()), this, SLOT(onRemote()));

	connect(m_pPause, SIGNAL(triggered()), this, SLOT(setPause()));
	connect(m_pContinue, SIGNAL(triggered()), this, SLOT(setContinue()));
	connect(m_pClear, SIGNAL(triggered()), this, SLOT(clearDbgInfo()));
	connect(m_pSave, SIGNAL(triggered()), this, SLOT(setSave()));
	connect(m_pCheckTime, SIGNAL(triggered()), this, SLOT(checkTime()));
	connect(m_pFilter, SIGNAL(triggered()), this, SLOT(setFilter()));
	connect(m_pExit, SIGNAL(triggered()), this, SLOT(exitApp()));
	connect(m_pCopy, SIGNAL(triggered()), this, SLOT(copyInfo()));
	//connect(m_pCopy, SIGNAL(triggered()), m_pDbgList, SLOT(copyInfo()));

	m_pToolBar->addAction(m_pTopWindow);
	//m_pToolBar->addAction(m_pRemote);
//	m_pToolBar->addSeparator();

	m_pToolBar->addAction(m_pPause);
	m_pToolBar->addAction(m_pContinue);
	m_pToolBar->addAction(m_pCopy);
	m_pToolBar->addAction(m_pClear);
	m_pToolBar->addAction(m_pSave);
//	m_pToolBar->addSeparator();

	m_pToolBar->addAction(m_pCheckTime);
	m_pToolBar->addAction(m_pFilter);
//	m_pToolBar->addSeparator();
	m_pToolBar->addAction(m_pExit);
	//m_pToolBar->addAction(m_pShowNode);
	m_pPause->setEnabled(false);
	m_pContinue->setEnabled(false);
	m_pSave->setEnabled(false);
	m_pFilter->setEnabled(false);
	m_pClear->setEnabled(false);
}

void mainWindow::createMenus(void)
{
	m_pFileMenu = menuBar()->addMenu(_C2Q("文件"));
	m_pFileMenu->addAction(m_pSave);
	m_pFileMenu->addAction(m_pExit);

	m_pViewMenu = menuBar()->addMenu(_C2Q("查看"));
	m_pViewMenu->addAction(m_pPause);
	m_pViewMenu->addAction(m_pContinue);
	//m_pViewMenu->addAction(m_pRemote);

	m_pOperMenu = menuBar()->addMenu(_C2Q("操作"));
	m_pOperMenu->addAction(m_pCopy);
	m_pOperMenu->addAction(m_pClear);
	m_pOperMenu->addAction(m_pCheckTime);
	m_pOperMenu->addAction(m_pFilter);	
}


void mainWindow::setWindowTop(void)
{
	if ((Qt::WindowStaysOnTopHint & windowFlags()) == 0)
	{
		hide();
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		show();
		
	}
	else
	{
		hide();
		setWindowFlags(windowFlags()^Qt::WindowStaysOnTopHint);
		show();
	}
}

void mainWindow::setPause(void)
{
 	m_pPause->setEnabled(false);
 	m_pContinue->setEnabled(true);
	m_pauseFlag = true;
	
}

void mainWindow::setContinue(void)
{
	m_pauseFlag = false;
	m_pPause->setEnabled(true);
	m_pContinue->setEnabled(false);
}

void mainWindow::clearDbgInfo(void)
{
	m_pDbgInfoSource->removeRows(0, m_pDbgInfoSource->rowCount());
}

void mainWindow::setSave(void)
{
	if (m_pSave->isChecked())
	{
		m_saveFlag = TRUE;
	}
	else
	{
		m_saveFlag = FALSE;
	}
}


void mainWindow::checkTime(void)
{
	QCheckTime dlg(this);

	if( dlg.exec() != QDialog::Accepted )
	{	
		return;
	}
}

void mainWindow::setFilter(void)
{
 	QFilter dlg(this);
	dlg.setFilter(m_StringFilter);
	qDebug()<<m_StringFilter;

	if (dlg.exec() == QDialog::Accepted)
	{
		m_StringFilter = dlg.getFilter();
	}
	
}

void mainWindow::exitApp(void)
{
	this->close();
}

void mainWindow::onRemote(void)
{
	//m_pRemoteDialog->exec();
}

void mainWindow::initRemoteHost(void)
{
	QMap<QString, int> hostList = m_pNetTree->hostList();

	for (QMap<QString, int>::const_iterator i = hostList.constBegin(); i != hostList.constEnd(); i++)
	{
		m_pRemoteHost->addItem(i.key(), i.value());
	}
}

void mainWindow::onRemoteModeChange(int state)
{
	int ret = FALSE;
	if (2 == state)
	{
		//如果是手动填写的地址
		if (-1 == m_pRemoteHost->findText(m_pRemoteHost->currentText()))
		{
			ret = g_dbgPara.setRemote(true, -1, _Q2C(m_pRemoteHost->currentText()));
		}
		else
		{
			ret = g_dbgPara.setRemote(true, m_pRemoteHost->itemData(m_pRemoteHost->currentIndex()).toInt(), NULL);
		}
		m_pRemoteHost->setEnabled( false );
	}
	else
	{
		g_dbgPara.setRemote(false, -1, NULL);
		clearTaskList();
		//m_pTaskList->clearTaskList();
		m_pRemoteHost->setEnabled( true );
	}

	if (TRUE == ret)
	{
		//m_pTaskList->remoteTaskList();
		remoteTaskList();
	}
	//连接失败的
	else if ((2 == state) && (FALSE == ret))
	{
		clearTaskList();
	}
}


void mainWindow::remoteTaskList(void)
{
	DbgProcInfo* procInfo;
	int taskNum = g_dbgPara.getRenmoteTask(procInfo);
	clearTaskList();

	for (int i = 0; i < taskNum; i++)
	{
		QList<QStandardItem *> items;
		items<<new QStandardItem(_C2Q(procInfo->proc_asc))<<new QStandardItem(QString::number(procInfo->pid))<<new QStandardItem(QString::number(procInfo->regidx));
		procInfo++;
		m_pTaskListSource->appendRow(items);
	}
}

void mainWindow::clearTaskList(void)
{
	m_pTaskListSource->removeRows(0, m_pTaskListSource->rowCount());
	m_pTaskListSource->setRowCount(0);
}


void mainWindow::saveDbgToFile(QString type, QString desc)
{
	if (m_file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&m_file);
		stream<<type<<"  "<<desc<<"\r\n";
	}
	
	m_file.close();
}

void mainWindow::selectDbgMsgs(QModelIndex index)
{
	if (qApp->mouseButtons() == Qt::LeftButton)
	{
		return;
	}
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		showMenu();
	}
}

void mainWindow::showMenu(void)
{
	QPoint pos;
	QMenu menu(this);
	QAction* copyAction = menu.addAction(QIcon(":/res/edit-copy-3.png"), _C2Q("复制信息"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copyInfo()));
	menu.exec(QCursor::pos());
}


void mainWindow::copyInfo(void)
{
	QItemSelectionModel* selectedModel = m_pdbgInfoList->selectionModel();
	QModelIndexList indexes = selectedModel->selectedIndexes();
	QStandardItem* typeItem;
	QStandardItem* descItem;
	QClipboard *board = QApplication::clipboard();
	QString copyContext;
	
	for (int i = 0; i < indexes.size() / 2; i++)
	{
		typeItem = m_pDbgInfoSource->itemFromIndex(indexes[i]);
		descItem = m_pDbgInfoSource->itemFromIndex(indexes[indexes.size() / 2 + i]);
		copyContext = copyContext + typeItem->text() + _C2Q("    ") + descItem->text() + _C2Q("\n");
	}
	
	board->setText(copyContext);
}


void mainWindow::closeEvent(QCloseEvent *event)
{
//  	if(QMessageBox::question(this, tr("提示"),tr("是否退出节点调试?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No)
	if( DMessageBox::question( this , tr("提示") , tr("是否退出节点调试?") , QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No )
	{
		event->ignore();
		return;
	}
	m_getDbgInfo.terminate();
	event->accept();
}


