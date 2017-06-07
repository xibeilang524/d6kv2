
#include "dqinfo_nettree.h"
#include "net/netdef.h"

#include <QImage>
#include <QPainter>

QNetTreeItem::QNetTreeItem(const QStringList & strings, int type /* = Type */)
:QTreeWidgetItem(strings, type)
{
	m_hostNo = -1;
	m_srvNo = -1;
	m_type = ITEM_SRV;
}


QNetTree::QNetTree(QWidget *parent /* = NULL */)
:QTreeWidget(parent)
{
	ShmNetDB  shmnetdb;
	m_netdb = shmnetdb.NetDB();
	m_procdb = shmnetdb.ProcDB();
	m_autodb = shmnetdb.AutoDB();
	m_rtdb = shmnetdb.RtdbDB();

	m_netBusyF = FALSE;
	m_grpOnLine.addPixmap(QPixmap(":res/serverup.png"));
	m_grpOffLine.addPixmap(QPixmap(":res/serverdown.png"));
	m_grpUndefine.addPixmap(QPixmap(":res/serveruncfg.png"));
	m_srvMaster.addPixmap(QPixmap(":res/computer-main.png"));
	m_srvStandBy.addPixmap(QPixmap(":res/computer-standby.png"));
	m_srvOnline.addPixmap(QPixmap(":res/computer-online.png"));
	m_srvOffLine.addPixmap(QPixmap(":res/computer-offline.png"));

	setHeaderLabels(QStringList()<<_C2Q("网络节点")<<_C2Q("A网")<<_C2Q("B网"));
	setColumnWidth(0, 150);
	setColumnWidth(1, 40);
	setColumnWidth(2, 30);
	showServerType();
	m_pTimer = new QTimer( this );
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_pTimer->start(500);
	connect(this, SIGNAL(itemPressed(QTreeWidgetItem* , int)), 
		this, SLOT(serverClicked(QTreeWidgetItem *, int)));
}


QNetTree::~QNetTree()
{

}



void QNetTree::showServerType(void)
{
	int i;
	QString dbName;
	QStringList colum;
	QNetTreeItem *tmp;
	for (i = 0; i < m_rtdb->grp[1].dbnum; i++)
	{
		dbName = _C2Q(m_rtdb->grp[1].db[i].dbname);
		colum.clear();
		colum<<dbName;
		tmp = new QNetTreeItem(colum);

		tmp->m_type = ITEM_SRV_GROUP;
		tmp->m_srvNo = i;
		tmp->m_hostNo = -1;

		//此种服务器组中没有主机，即全部离线
		if (!strcmp(m_rtdb->grp[1].db[i].main_node, ""))
		{
			tmp->setIcon(0, m_grpOffLine);
		}
		else
		{
			tmp->setIcon(0, m_grpOnLine);
		}

		addTopLevelItem(tmp);
		showServerItem(tmp, m_rtdb->grp[1].db[i].dbname);
	}

	dbName = _C2Q("未配置节点");
	colum.clear();
	colum<<dbName;
	tmp = new QNetTreeItem(colum);

	tmp->m_type = ITEM_SRV_GROUP;
	tmp->m_srvNo = -1;
	tmp->m_hostNo = -1;
	tmp->setIcon(0, m_grpUndefine);

	addTopLevelItem(tmp);
	showUndefineNodeItem(tmp);
}

void QNetTree::showServerItem(QTreeWidgetItem *curNode, char *srv)
{
	int i, j, k,m;
	QString srvName;
	QStringList colum;
	QNetTreeItem*	tmp;
	QList<QString> mirrorList;
	int hostNum = m_netStatus.HostNum();

	for (i = 0; i < hostNum; i++)
	{
		if (m_netStatus[i].grpno != 1)
		{
			continue;
		}

		for (j = 0; j < m_rtdb->grp[1].dbnum; j++)
		{
			if (strcmp(m_rtdb->grp[1].db[j].dbname, srv))
			{
				continue;
			}

			//添加镜像节点
			mirrorList.clear();
			for (m=0;m<m_rtdb->grp[1].db[j].mirrornum;m++)
			{
				mirrorList.append(_C2Q(m_rtdb->grp[1].db[j].mirror[m]));
			}

			if (mirrorList.contains(_C2Q(m_netStatus[i].name)) )
			{
				srvName = _C2Q(m_netStatus[i].name) + _C2Q("(镜像)");
				colum.clear();
				colum<<srvName;

				tmp = new QNetTreeItem(colum);
				if (m_netStatus[i].net_a.cur_flag || m_netStatus[i].net_b.cur_flag)
				{
					tmp->setIcon(0, m_srvOnline);
				}
				else
				{
					tmp->setIcon(0, m_srvOffLine);
				}
				tmp->m_type = ITEM_SRV;
				tmp->m_srvNo = j;
				tmp->m_hostNo = i;

				curNode->addChild(tmp);	
			}
		
			
			//添加普通的节点
			for (k = 0; k < SERVER_NODE_MAX; k++)
			{
				if (strcmp(m_netStatus[i].name, m_rtdb->grp[1].db[j].node[k]))
				{
					continue;
				}
				else
				{
					if (mirrorList.contains(_C2Q(m_netStatus[i].name)) )	//防止重复添加前置镜像节点服务器
					{
						continue;
					}
				}
	
				srvName = _C2Q(m_netStatus[i].name);
				colum.clear();
				colum<<srvName;


				tmp = new QNetTreeItem(colum);

				switch (m_netStatus[i].vstate)
				{
				case HOST_STATUS_MAIN:
					tmp->setIcon(0, m_srvMaster);
					break;
				case HOST_STATUS_BACK:
					tmp->setIcon(0, m_srvStandBy);
					break;
				case HOST_STATUS_WORK:
					tmp->setIcon(0, m_srvOnline);
					break;
				case HOST_STATUS_STOP:
					tmp->setIcon(0, m_srvOffLine);
					break;
				default:
					break;
				}
				tmp->m_type = ITEM_SRV;
				tmp->m_srvNo = j;
				tmp->m_hostNo = i;
				curNode->addChild(tmp);	

				if (m_hostList.find(srvName) == m_hostList.constEnd())
				{
					m_hostList.insert(srvName, tmp->m_hostNo);
				}
			}
		}//for (j = 0; j < m_rtdb->grp[groupNo].dbnum; j++)	
	}//for (i = 0; i < hostNum; i++)
}


void QNetTree::showUndefineNodeItem(QTreeWidgetItem *curNode)
{
	int i, j, k,m;
	bool findf;
	QString srvName;
	QStringList colum;
	QNetTreeItem* tmp;
	QList<QString> mirrorList;
	int hostNum = m_netStatus.HostNum();

	for (i = 0; i < hostNum; i++)
	{
		findf = false;

		if (m_netStatus[i].grpno != 1)
		{
			continue;
		}

		for (j = 0; j < m_rtdb->grp[1].dbnum; j++)
		{
			for (k = 0; k < SERVER_NODE_MAX; k++)
			{
				if (strcmp(m_netStatus[i].name, m_rtdb->grp[1].db[j].node[k]))
				{
					continue;
				}
				findf = true;
				break;
			}

			if (findf)
			{
				break;
			}

			mirrorList.clear();
			for (m=0;m<m_rtdb->grp[1].db[j].mirrornum;m++)
			{
				mirrorList.append(_C2Q(m_rtdb->grp[1].db[j].mirror[m]));
			}

			if (!mirrorList.contains(_C2Q(m_netStatus[i].name)) )
			{
				continue;
			}

/*
			if (strcmp(m_netStatus[i].name, m_rtdb->grp[1].db[j].mirror))
			{
				continue;
			}
*/

			findf = true;
			break;
		}//for (j = 0; j < m_rtdb->grp[1].dbnum; j++)

		if (findf)
		{
			continue;
		}

		srvName = _C2Q(m_netStatus[i].name);
		colum.clear();
		colum<<srvName;
		
		tmp = new QNetTreeItem(colum);

		switch (m_netStatus[i].vstate)
		{
		case HOST_STATUS_MAIN:
			tmp->setIcon(0, m_srvMaster);
			break;
		case HOST_STATUS_BACK:
			tmp->setIcon(0, m_srvStandBy);
			break;
		case HOST_STATUS_WORK:
			tmp->setIcon(0, m_srvOnline);
			break;
		case HOST_STATUS_STOP:
			tmp->setIcon(0, m_srvOffLine);
			break;
		default:
			break;
		}

		tmp->m_type = ITEM_SRV;
		tmp->m_srvNo = -1;
		tmp->m_hostNo = i;
		curNode->addChild(tmp);

		if (m_hostList.find(srvName) == m_hostList.constEnd())
		{
			m_hostList.insert(srvName, tmp->m_hostNo);
		}
	}//for (i = 0; i < hostNum; i++)
}


void QNetTree::onTimer(void)
{
	refleshSrvType();
	refleshSrvItem();
}

void QNetTree::refleshSrvType(void)
{
	int srvNo;
	for (int i = 0; i < topLevelItemCount(); i++)
	{
		QNetTreeItem* pCurItem = (QNetTreeItem *)topLevelItem(i);

		if (pCurItem->m_type != ITEM_SRV_GROUP)
		{
			continue;
		}

		srvNo = pCurItem->m_srvNo;

		if (srvNo < 0)
		{
			continue;
		}

		//此种服务器组中没有主机，即全部离线
		if (!strcmp(m_rtdb->grp[1].db[srvNo].main_node, ""))
		{
			pCurItem->setIcon(0, m_grpOffLine);
		}
		else
		{
			pCurItem->setIcon(0, m_grpOnLine);
		}
	}
}

void QNetTree::refleshSrvItem(void)
{
	int hostNo;
	int srvNo;
	QNetTreeItem* pSrvGrp;
	QNetTreeItem* pSrv;
	
	for (int i = 0; i < topLevelItemCount(); i++)
	{
		pSrvGrp = (QNetTreeItem *)topLevelItem(i);

		for (int j = 0; j < pSrvGrp->childCount(); j++)
		{
			pSrv = (QNetTreeItem *)pSrvGrp->child(j);
			hostNo = pSrv->m_hostNo;
			srvNo  = pSrv->m_srvNo;

			QImage  tmpimage1, tmpimage2;
			QPixmap tmppixmap( 24, 16 );
			QPainter tmppainter( &tmppixmap );

			switch (m_netStatus[hostNo].vstate)
			{
			case HOST_STATUS_MAIN:
				pSrv->setIcon(0, m_srvMaster);
				break;
			case HOST_STATUS_BACK:
				pSrv->setIcon(0, m_srvStandBy);
				break;
			case HOST_STATUS_WORK:
				pSrv->setIcon(0, m_srvOnline);
				break;
			case HOST_STATUS_STOP:
				pSrv->setIcon(0, m_srvOffLine);
				break;
			default:
				break;
			}

			if (srvNo >= 0)
			{
				if (!strcmp(m_netStatus[hostNo].name, m_rtdb->grp[1].db[srvNo].main_node))
				{
					pSrv->setTextColor(0, QColor(255, 0, 0));
				}
				else
				{
					pSrv->setTextColor(0, QColor(0, 0, 0));
				}
			}

			if (1 == m_netStatus[hostNo].net_a.cur_flag)
			{
				//pSrv->setText(1, _C2Q("正常"));
				pSrv->setIcon(1, QPixmap( ":res/netup.png" ) );
			}
			else
			{
				//pSrv->setText(1, _C2Q("异常"));
				pSrv->setIcon(1, QPixmap( ":res/netdown.png" ) );
			}

			if (1 == m_netStatus[hostNo].net_b.cur_flag)
			{
				//pSrv->setText(2, _C2Q("正常"));
				pSrv->setIcon(2, QPixmap( ":res/netup.png" ) );
			}
			else
			{
				//pSrv->setText(2, _C2Q("异常"));
				pSrv->setIcon(2, QPixmap( ":res/netdown.png" ) );
			}
		}//for (int j = 0; j < pSrvGrp->childCount(); j++)
	}//for (int i = 0; i < topLevelItemCount(); i++)
}

void QNetTree::serverClicked(QTreeWidgetItem *server, int column)
{
	if (qApp->mouseButtons() == Qt::LeftButton)
	{
		return;
	}
	if (qApp->mouseButtons() == Qt::RightButton)
	{
		//父节点为NULL，是顶层节点
		if (NULL == server->parent())
		{
			return;
		}
		showMenu();
	}
}

void QNetTree::showMenu(void)
{
	QPoint pos;
	QMenu menu(this);

// 	QAction* setMasterAction = menu.addAction(_C2Q("切为网络主控"));
	QAction* setMasterAction = new QAction(QIcon(":/res/computer-main.png"), tr(" 切为网络主控"), this);
	menu.addAction( setMasterAction ) ;
	QAction* setMainSrvAction = new QAction(QIcon(":/res/flag-red.png"), tr(" 切为主服务器"), this);
	menu.addAction( setMainSrvAction ) ;
//	QAction* setMainSrvAction = menu.addAction(_C2Q("切为主服务器"));
	connect(setMasterAction, SIGNAL(triggered()), this, SLOT(setMaster()));
	connect(setMainSrvAction, SIGNAL(triggered()), this, SLOT(setMainSrv()));
	menu.exec(QCursor::pos());
}

void QNetTree::setMaster(void)
{
	NetControl netCtrl;
	QNetTreeItem* selectedItem = (QNetTreeItem *)selectedItems()[0];
	netCtrl.SwitchHost(1, HOST_TYPE_CENTER, selectedItem->m_hostNo);
}

void QNetTree::setMainSrv(void)
{
	NetControl netCtrl;
	QNetTreeItem* selectedItem = (QNetTreeItem *)selectedItems()[0];
	int srvNo = selectedItem->m_srvNo;
	int hostNo = selectedItem->m_hostNo;
	char *srv = m_rtdb->grp[1].db[srvNo].dbname;
	netCtrl.SwitchServer(1, srv, m_netStatus[hostNo].hostname);
}

QMap<QString, int> QNetTree::hostList(void)
{
	return m_hostList;
}

