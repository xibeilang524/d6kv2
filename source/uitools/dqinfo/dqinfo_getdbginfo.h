#ifndef MASINFO_GETDBGINFO_H_
#define MASINFO_GETDBGINFO_H_
#endif

#include <QThread>
#include <QStandardItemModel>
#include <QString>
#include <iostream>
using namespace std;

#include "net/netapi.h"


class QGetDbgInfo: public QThread
{
	Q_OBJECT  
public:
	QGetDbgInfo(QWidget *o);
	virtual void run(void);

public:
	QStandardItemModel *m_pDbgModel;

signals:
	void sendDbgInfo(int, QString, QString);

};