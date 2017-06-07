#ifndef MASINFO_TASKTREE_H_
#define MASINFO_TASKTREE_H_

#include <QtGui>
#include <QTreeWidget>
#include <QString>
#include <iostream>
#include <Qicon>
#include <QTimer>
#include <QApplication>
#include <QMenu>
#include <QList>
using namespace std;
#include "net/netapi.h"
#include "masinfo_string.h"


class QTaskList: public QTreeWidget
{
	Q_OBJECT

public:
	QTaskList(QWidget *parent = NULL);
	~QTaskList();

public:
	void taskList(void);
	void remoteTaskList(void);
	int getSelectID(void);
	void clearTaskList(void);

private slots:
	void onTimer();
	void taskClicked(QTreeWidgetItem* task, int index);
	void showMenu();
	void logOutTask();
	void killTask();

private:
	NetDbg			 m_dbg;
	//REMOTE_DBG_PROC  m_remoteDbg;
	QTimer*			 m_pTimer;
};


#endif


