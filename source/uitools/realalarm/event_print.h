/**
@file event_print.h
@brief 事项打印类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/


#ifndef EVENT_PRINT_H
#define EVENT_PRINT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QThread>

#include "event_filter.h"
#include "event_para.h"
#include "event_cfg.h"

#ifdef _WIN32
#include <winspool.h>
#include <windows.h>
#endif

class Event_print
{
private:
	QString m_printer;
	
    QMutex m_mutex;
    bool m_done;
	bool m_bOK;
	FILE			*m_hprinter;
	
	Event_cfg		*m_cfg;
	Event_para		*m_event_para;
	int				m_line_len;

	QList<Rsys_event *> m_list;
	int get_event_text(int row, Rsys_event * e, QString & msg);
	bool print_head();
	bool print_line();
	bool print(const char * txt);
	void close_printer();
	bool start_printer();

public:
	bool set_printer(const QString & printer);
	
	virtual void add_event( Rsys_event * eve );

	Event_print(Event_cfg *pCfg, Event_para *pEventPara);
	~Event_print();

  	void print();
	//void stop();
};

#endif