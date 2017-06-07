#ifndef MASINFO_CHECKTIME_H_
#define MASINFO_CHECKTIME_H_

#include <QtGui>
#include <QDialog>
#include <QTimeEdit>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTime>
#include <QDate>
#include "net/netapi.h"
#include "dqinfo_string.h"

class QCheckTime: public QDialog
{
	Q_OBJECT
public:
	QCheckTime(QWidget *parent = NULL);

private slots:
	void onOK(void);
	void onCancel(void);
private:
	void timeToDialog(void);
private:
	QTimeEdit*	m_pTime;
	QDateEdit*	m_pDate;
	QHBoxLayout*	m_pHLayout;
	QHBoxLayout*	m_pHLayout_button;
	QPushButton*	m_pOK;
	QPushButton*	m_pCancel;
};
#endif