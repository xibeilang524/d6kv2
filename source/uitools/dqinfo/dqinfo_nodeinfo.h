#ifndef MASINFO_NODEINFO_H_
#define MASINFO_NODEINFO_H_
#include <QtGui>
#include <QTableWidget>
#include <QString>
#include "db/dbapi.h"

#define _Q2C(str) ((const char *)(str.local8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))

class QNodeInfo: public QDialog
{
public:
	QNodeInfo(QWidget *parent = NULL);

private:
	void showNode(void);
private:
	DBApi	m_db;
	QTableWidget*  m_pNodeTable;
	
};
#endif