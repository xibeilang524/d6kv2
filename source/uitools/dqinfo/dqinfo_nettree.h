#ifndef MASINFO_NETTREE_H_
#define MASINFO_NETTREE_H_

#include <QTreeWidget>
#include <QString>
#include <iostream>
#include <QIcon>
#include <QTimer>
#include <QApplication>
#include <QMenu>
#include <QList>
#include <QMap>
using namespace std;
#include "net/netapi.h"
#include "dqinfo_string.h"

#define ITEM_SRV		1
#define ITEM_SRV_GROUP		2

class QNetTreeItem: public QTreeWidgetItem
{
public:
	QNetTreeItem(const QStringList & strings, int type = Type);
public:
	int		m_type;
	int		m_srvNo;
	int		m_hostNo;
};

class QNetTree: public QTreeWidget
{
	Q_OBJECT

public:
	QNetTree(QWidget *parent = NULL);
	~QNetTree();

	QMap<QString, int> hostList(void);
protected:
	void refleshSrvItem(void);
	void refleshSrvType(void);

	void showServerType(void);
	void showServerItem(QTreeWidgetItem *curNode, char *srv);
	void showUndefineNodeItem(QTreeWidgetItem *curNode);
	void showMenu(void);

private slots:
	void onTimer(void);
	void serverClicked(QTreeWidgetItem *server, int column);
	void setMaster();
	void setMainSrv();

private:
	QTimer*			m_pTimer;
	NetStatus		m_netStatus;
	NET_DB*			m_netdb;
	PROC_DB*		m_procdb;
	AUTO_DB*		m_autodb;
	RTDB_DB*		m_rtdb;
	int				m_type;
	int				m_netBusyF;

	QIcon			m_grpOnLine;
	QIcon			m_grpOffLine;
	QIcon			m_grpUndefine;
	QIcon			m_srvOffLine;
	QIcon			m_srvOnline;
	QIcon			m_srvMaster;
	QIcon			m_srvStandBy;

	QMap<QString, int>	m_hostList;
};

#endif