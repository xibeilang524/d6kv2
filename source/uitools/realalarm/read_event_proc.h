/**
@file read_event_proc.h
@brief 读取事项进程类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef READ_EVENT_PROC_H
#define READ_EVENT_PROC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QThread>
#include <QEvent>
#include <QMutex>
#include <QMainWindow>
#include "sysstr.h"

#define QMSG_NEW_EVENT (QEvent::User+100)
// const QEvent::Type QMSG_NEW_EVENT = (QEvent::Type)(QEvent::User+100);

class Rsys_event;

class Read_event_proc : public QThread  
{
	Q_OBJECT
public:
// 	QReadEventProc(QWidget *o): m_receiver(o), m_done(FALSE),QThread( 1024*1024*64 )
// 	{
// 		;
// 	}
	Read_event_proc(QObject *o): m_receiver(o), m_done(FALSE),QThread()
	{
		setStackSize(1024*1024*64);
	}	
	void run();
    void stop();
	void pause();
	bool is_pause(){return m_pause;};
	SYS_EVENT event ;
	QObject * m_receiver;

private:
    QMutex m_mutex;
    bool m_done;
	bool m_pause;

signals:
	void signal_send_event(Rsys_event * ,int);
};

#endif 