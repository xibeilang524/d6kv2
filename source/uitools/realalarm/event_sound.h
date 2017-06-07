/**
@file event_sound.h
@brief 语音报警类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/

#ifndef EVENT_SOUND_H
#define EVENT_SOUND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QThread>
#include <QMap>
#include <QMutex>
#include <QList> 
#include <QFile>

#include "event_filter.h"

#if defined (__unix)
	#include "QMySound.h"
#endif

class Event_sound : public QThread
{
public:
	Event_sound();
	~Event_sound();

	void run();
	void stop();

	void add_sound_event( Rsys_event *eve, int soundcount );

	void confirm_event( Rsys_event* eve );

	void confirm_all_event();

	void play( const QString &filename );
	void play_text( const QString &text );
	void read_status();

	void save_play_filename( const QString &file, const QString &str );
	void save_play_filename( const QString &file, Rsys_event *eve );

	//语音告警模式
	void set_tone_dev( const QString &dev );;
private:

	//语音报警事项和剩余报警次数对应表
	QMap<Rsys_event*,int>	m_event_sounds;
	QList<Rsys_event *>		m_event_list;

	QMutex					m_mutex, m_mutexstop;
    bool					m_done;
	QString					m_tone_dev_name;
	QFile					m_tone_dev;
	//设备状态
	int						m_tone_dev_status;
	
#if defined (__unix)
	QMySound mySound;
#endif
};

#endif 