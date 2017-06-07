#ifndef MASINFO_FILTER_H_
#define MASINFO_FILTER_H_

#include <QtGui>
#include <QDialog>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include <QVector>
#include "net/netapi.h"
#include "dqinfo_string.h"

class QDbgCheck: public QCheckBox
{
public:
	QDbgCheck(int type, QString desc, QWidget * parent = NULL);
	int m_type;
	QString m_desc;
};

class QFilter: public QDialog
{
	Q_OBJECT
public:
	QFilter(QWidget* parent = NULL);
	QVector<QString> getFilter(void);
	void setFilter(QVector<QString> filter);

private slots:
	void onSelectAll(void);
	void onSelectNone(void);
	void onOk(void);
	void onCancel(void);
private:
	void getAllType(void);

private:
	NetDbg			m_dbg;
	QGridLayout*    m_pGridlayout;
	QHBoxLayout*	m_pHLayout;
	QVBoxLayout*	m_pVLayout;
	QPushButton*	m_pOK;
	QPushButton*	m_pCancel;
	QPushButton*	m_pSelectAll;
	QPushButton*	m_pSelectNone;
	QVector<QDbgCheck *> m_checkGroup;
	QVector<QString>	m_filter;	
};
#endif