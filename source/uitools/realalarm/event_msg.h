/**
@file event_msg.h
@brief 短信告警头文件

@author lujiashun
@version 1.2.0
@date 2016-9-22
*/
#ifndef EVENT_MSG_H
#define EVENT_MSG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QMap>
#include <QThread>
#include <QTimer>

#include "qextserialport.h"
#include "event_filter.h"


class QLibrary;

class Event_msg : public QThread ,public Event_filter /*, public QObject*/
{
	Q_OBJECT
public:
	Event_msg();
	virtual ~Event_msg();
	bool m_bget_msg;
	bool m_brcv_msg;
	QString m_rcv_text;

	virtual void run();
	void reload_dll();
	
	void add_msg_event( Rsys_event *pEvent, QString phoneno );
	void confirm_event( Rsys_event* eve );

	void set_serial_no( QString no );

	void set_msg_confirm_time( uint msgconfirmtime);

	void stop();
	
	void send_message(QString &phoneno,const QString &text);
	void resend_message();
private slots:
	int slot_read_mycom();


private:

	QMap<Rsys_event*, QString>	m_msg_event_list;		// 需要发送短信的事项;
	QMap<QString,QString>		m_msg_event_no_list;	// 需要发送的短信及号码;
//	QSysEventList m_eventList;

	QList<Rsys_event *>			m_event_list;
	QString						m_rcv_phone_no;
	QString						m_serial_no;
	QMutex						m_mutexstop;
    bool						m_done;
	bool						m_rcvMsg;			// 收到短信内容;
	int							count;
	int							m_msgConfirmTime;	// 短信确认时间;
	QTimer						*readTimer;
//     Win_QextSerialPort			*myCom;
	QextSerialPort				*myCom;
};

#endif // !defined(AFX_QEVENTMSG_H__B9168180_AF71_4694_AF73_C99DF83535BE__INCLUDED_)
