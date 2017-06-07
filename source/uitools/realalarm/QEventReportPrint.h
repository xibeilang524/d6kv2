#ifndef QEVENTREPORTPRINT_H_
#define QEVENTREPORTPRINT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QtCore/QThread>

#include "QEventFilter.h"
#include "QEventPara.h"
#include "QEventCfg.h"

#ifdef _WIN32
#include <winspool.h>
#include <windows.h>
#endif

class QEventReportPrint
{
private:
    QMutex							m_mutex;
    bool									m_done, m_bOK;
	
	QEventCfg*						m_cfg;
	QEventPara*						m_eventPara;
	int									m_nLineLen;

	QList<QSYS_EVENT *>		m_list;
	int getEventText(int row, QSYS_EVENT * e, QString & msg);
	bool printHead();
	bool printLine();
	bool print(const char * txt);
	void closePrinter();
	bool startPrinter();

public:
	virtual void addEvent( QSYS_EVENT * eve );

	QEventReportPrint(QEventCfg *pCfg, QEventPara *pEventPara);
	~QEventReportPrint();

  	void print();
	//void stop();
};

#endif 
