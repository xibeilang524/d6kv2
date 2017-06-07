#include "masinfo_tasklist.h"
#include "masinfo_dbgpara.h"
// #include "masinfo_remotewatch.h"
// 
// extern CRemoteDbg g_remoteDbg; 
extern CDbgPara g_dbgPara;


QTaskList::QTaskList(QWidget *parent /* = NULL */)
:QTreeWidget(parent)
{
	setHeaderLabels(QStringList()<<_C2Q("进程名")<<_C2Q("PID")<<_C2Q("注册号"));
	setColumnWidth(0, 130);
	setColumnWidth(1, 40);
	setColumnWidth(2, 40);
	setRootIsDecorated(false);
	setAlternatingRowColors(true);
	m_pTimer = new QTimer();
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_pTimer->start(1000);

	connect(this, SIGNAL(itemPressed(QTreeWidgetItem* , int)), this, SLOT(taskClicked(QTreeWidgetItem* , int)));
}

QTaskList::~QTaskList()
{

}

void QTaskList::onTimer(void)
{
	if (g_dbgPara.isRemote())
	{
		return;
	}
	taskList();
}


void QTaskList::taskList(void)
{
	int i, j;
	bool findF;
	int taskNum;
	QString curPid;
	char procDes[DEBUG_MAX_PROC][DESC_LEN];
    int procNum = m_dbg.GetProcEnm(procDes);
    int* pid = m_dbg.GetProcPidEnm();
    int* idx = m_dbg.GetRegIndex();

	taskNum = topLevelItemCount();
	//删除不存在的进程
	for (i = 0; i < taskNum; i++)
	{
		findF = false;
		curPid = topLevelItem(i)->text(1);

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
			takeTopLevelItem(i);
			taskNum = topLevelItemCount();
		}
	}

	//添加新出现的进程
	for (i = 0; i < procNum; i++)
	{
		taskNum = topLevelItemCount();
		findF = false;

		for (j = 0; j < taskNum; j++)
		{
			curPid = topLevelItem(j)->text(1);
			if (pid[i] == curPid.toInt())
			{
				findF = true;
				break;
			}
		}

		if (!findF)
		{
			addTopLevelItem(new QTreeWidgetItem(QStringList()<<_C2Q(procDes[i])<<QString::number(pid[i])<<QString::number(idx[i])));
		}
	}
}


void QTaskList::remoteTaskList(void)
{
	DbgProcInfo* procInfo;
	int taskNum = g_dbgPara.getRenmoteTask(procInfo);

	while (topLevelItemCount() > 0)
	{
		takeTopLevelItem(0);
	}

	for (int i = 0; i < taskNum; i++)
	{
		addTopLevelItem(new QTreeWidgetItem(QStringList()<<_C2Q(procInfo->proc_asc)<<QString::number(procInfo->pid)<<QString::number(procInfo->regidx)));
		procInfo++;
	}
}


void QTaskList::taskClicked(QTreeWidgetItem* task, int index)
{
	if (qApp->mouseButtons() == Qt::LeftButton)
	{
		return;
	}
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		//父节点为NULL，是顶层节点
		if (NULL == task->parent())
		{
			showMenu();
		}
	}
}


void QTaskList::showMenu(void)
{
	QPoint pos;
	QMenu menu(this);
	QAction* logout = menu.addAction(_C2Q("注销进程"));
	QAction* kill = menu.addAction(_C2Q("杀死进程"));
	connect(logout, SIGNAL(triggered()), this, SLOT(logOutTask()));
	connect(kill, SIGNAL(triggered()), this, SLOT(killTask()));
	menu.exec(QCursor::pos());
}


void QTaskList::logOutTask(void)
{
	int idx = selectedItems()[0]->text(2).toInt();
	m_dbg.UnRegProc(idx);
}

void QTaskList::killTask(void)
{
	NetStatus cfg;
	NetControl netCtrl;
	int hostNo = cfg.GetHostNo();
	char * host = cfg[hostNo].hostname;
	netCtrl.KillProc(1, host, selectedItems()[0]->text(1).toInt());
}


int QTaskList::getSelectID(void)
{
	int ID = selectedItems()[0]->text(2).toInt();
	return ID;
}

void QTaskList::clearTaskList(void)
{
	//clearSelection();
	while (topLevelItemCount() > 0)
	{
		//cout<<topLevelItemCount()<<endl;
		takeTopLevelItem(0);
		cout<<topLevelItemCount()<<endl;
	}
	cout<<"clear done"<<endl;
}
