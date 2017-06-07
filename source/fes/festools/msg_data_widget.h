/**
@file msg_data_widget.h
@brief 报文展示窗口

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef MSGDATAWIDGET_H_
#define MSGDATAWIDGET_H_

#include "msg_data_view.h"
#include "msg_modular_data.h"

#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QRegExpValidator>

#include "fes/fesshm.h"
#include "fes/platformwrapper.h"
#include "fes/fesdev.h"

class msg_data_widget : public QFrame
{
	Q_OBJECT
public:
	msg_data_widget( QWidget* parent = NULL ) ;
	void stop_get_msg( bool ) ;

protected:

private:
	int request_fep_msg() ;
	void ini_msg_data_widget() ;
	void get_channel_msg() ;
	void update_tool_bar() ;
	void on_current_modular_change( msg_modular* pModula ) ;
	void save_modular_data( msg_modular_data* pModula_data , QTextStream &output ,QString strtime ) ;

private:
	Fes_shm&				m_fes_shm ;
	QToolBar*				m_pTop_tool_bar ;
	QToolBar*				m_pTime_tool_bar ;
	QAction*				m_pPause_action ;
	QAction*				m_pContinue_action ;
	QAction*				m_pClean_action ;
	QAction*				m_pSave_action ;
	QAction*				m_pClear_search_action ;
	QLineEdit*			m_pSearch_lineedit ;
	QLabel*					m_pSearch_label ;
	QLabel*					m_pBegintime_label ;
	QLabel*					m_pEndtime_label ;
	QRegExpValidator* m_pValidator ;
	QCheckBox*			m_pAnalysis_msg_cbk ;    //是否解析报文  
	QCheckBox*			m_pAutosave_msg_cbk ;    //是否解析报文 
	QCheckBox*			m_pShow_raw_widget ;    //是否显示生数据窗口 
	QDateTimeEdit*	m_pBegintime_edit ;
	QDateTimeEdit*	m_pEndtime_edit ;

	QTimer*							m_pTimer ;
	Tcp_client*					m_pTcp_recv ;
	msg_data_view*			m_pMsg_data_view ;
	msg_modular*				m_pModular ;
	int m_current_channel ;

protected:
	void resizeEvent( QResizeEvent *re ) ;
	void showEvent( QShowEvent * ) ;

public slots:
	void on_analysis_msg_cbk() ;
	void on_pause() ;
	void on_continue() ;
	void on_clean() ;
	void on_save() ;
	void on_auto_save() ;
	void on_clear_search() ;
	void on_search_text_changed() ;
	void on_set_time() ;
	void on_channel_change( int channel_no ) ;
	void on_timer();

signals:
	void signal_show_raw_table( bool ) ;
};

#endif