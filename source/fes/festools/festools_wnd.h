/**
@file festools_wnd.h
@brief 前置工具主窗口

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef FESTOOLS_WND_H_
#define FESTOOLS_WND_H_

#include <QMainWindow>
#include <QTimerEvent>
#include <QStatusBar>
#include <QTabBar>
#include <QSplitter>
#include <QLabel>

#include "channel_data_widget.h"
#include "channel_tree_widget.h"
#include "raw_data_widget.h"
#include "status_bar_widget.h"

#include "fes/fesshm.h"

class festools_wnd : public QMainWindow
{
	Q_OBJECT
public:
	festools_wnd( QWidget* parent = NULL ) ;

protected:
	void timerEvent( QTimerEvent* event );

private:
	void ini_festools_widget() ;
	void ini_festools_menubar() ;
	void ini_festools_statusbar() ;
	void ini_stylesheet() ;
	void update_time() ;

private slots:
	void on_current_channel_changed( QTreeWidgetItem* , int ) ;
	void on_show_raw_table(bool) ;

private:
	raw_data_widget*					m_pRaw_data_widget ;
	channel_tree_widget*			m_pChannel_tree_widget ;
	channel_data_widget*			m_pChannel_data_widget ;
	status_bar_widget*		m_pStatus_bar_widget ;
	QStatusBar*					m_pStatus_bar ;
	QLabel*							m_pData_label ;
	QLabel*							m_pTime_label ;
	QSplitter*						m_pVsplitter ;
	QSplitter*						m_pHsplitter ;
	int									m_timerTime ;
	Fes_shm&						m_fes_shm ;
};

#endif