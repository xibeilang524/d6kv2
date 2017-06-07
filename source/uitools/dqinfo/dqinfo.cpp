#include "masinfo_checktime.h"


QCheckTime::QCheckTime(QWidget *parent /* = NULL */)
:QDialog(parent)
{
	m_pTime = new QTimeEdit(this);
	m_pDate  = new QDateEdit(this);
	m_pOK = new QPushButton(_C2Q("确定"), this);
	m_pCancel = new QPushButton(_C2Q("取消"), this);
	m_pHLayout = new QHBoxLayout(NULL);
	m_pVLayout = new QVBoxLayout(NULL);

	m_pVLayout->addWidget(m_pOK);
	m_pVLayout->addWidget(m_pCancel);

	m_pHLayout->addWidget(m_pDate);
	m_pHLayout->addWidget(m_pTime);
	m_pHLayout->addLayout(m_pVLayout);
	setLayout(m_pHLayout);
	setWindowTitle(_C2Q("对时"));
	setWindowIcon(QIcon(":/res/edit-history.png"));
	timeToDialog();

	connect(m_pOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(m_pCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}


void QCheckTime::timeToDialog(void)
{
	SYS_TIME sysTime;
	TimeProc timeProc;
	timeProc.GetTime(sysTime);

	m_pTime->setTime(QTime(sysTime.hour, sysTime.min, sysTime.sec));
	m_pDate->setDate(QDate(sysTime.year, sysTime.mon, sysTime.day));
}


void QCheckTime::onOK(void)
{
	NetControl  netCtrl;
	netCtrl.CheckTime(1, m_pDate->date().year(), m_pDate->date().month(), m_pDate->date().day()
		, m_pTime->time().hour(), m_pTime->time().minute(), m_pTime->time().second(), 0);
	this->done(0);
}

void QCheckTime::onCancel(void)
{
	this->done(0);
}


