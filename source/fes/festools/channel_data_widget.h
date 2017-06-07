/**
@file channel_data_widget.h
@brief 报文与通道状态窗口

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/

#ifndef CHANNELDATAWIDGET_H_
#define CHANNELDATAWIDGET_H_

#include <QMenu>
#include <QFrame>
#include <QTabBar>
#include <QTableWidget>

#include "festools_globaldef.h"
#include "msg_data_widget.h"
#include "channel_detail_widget.h"
#include "db/dbapi.h"

class channel_data_widget : public QFrame
{
	Q_OBJECT
public:
	channel_data_widget( QWidget* parent = NULL ) ;
	void hide_msg_widget() ;
	void hide_channel_table() ;
	void hide_all_widgets() ;
	void change_channel_no( int ) ;
	void change_termianl_no( int ) ;

private:
	void ini_channel_data_widget() ;
	void ini_raw_data_table( int ) ;

private slots:
	void on_timer() ;

public:
	msg_data_widget*		m_pMsg_widget ;

private:
	int								m_channel_no ;
	int								m_terminal_no ;
	Fes_shm&					m_fes_shm ;
	channel_detail_widget*	m_pChannel_detail_widget ; ;
	QTimer*						m_pTimer ;
};

#endif