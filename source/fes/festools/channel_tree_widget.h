/**
@file channel_tree_widget.h
@brief 通道树窗口

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef CHANNELTREEWIDGET_H_
#define CHANNELTREEWIDGET_H_

#include <QMap>
#include <QSplitter>
#include <QFrame>
#include <QLineEdit>
#include <QTreeWidget>
#include <QPushButton>

#include "festools_globaldef.h"
#include "fes/fesshm.h"
#include "fes/femailstruct.h"
#include "fes/platformwrapper.h"
#include "db/dbapi.h"

class channel_tree_widget : public QFrame 
{
	Q_OBJECT
public:
	channel_tree_widget( QWidget* parent = NULL ) ;
	void refresh_channel_tree() ;
	int get_cur_channel_no() ;
	int get_cur_terminal_no() ;

protected:

private:
	void ini_channel_tree() ;
	void ini_tree_data() ;
	void init_mail_buf();
	int read_station_area() ;
	int read_terminal_view() ;

signals:
	void on_channel_select_changed( QTreeWidgetItem* , int  ) ;

private slots:
	void on_search_btn() ;
	void on_call_alldata() ;
	void on_call_kwh() ;
	void on_check_time() ;
	void on_init_terminal() ;
	void on_channel_changed( QTreeWidgetItem* , int  ) ;

private:
	Fes_shm&			m_fes_shm ;
	QLineEdit*		m_pSearch_lineedit ;
	QPushButton*	m_pSearch_btn ;
	QTreeWidget*	m_pChannel_tree ;
	QPushButton*	m_pIni_terminal_btn ;
	QPushButton*	m_pCall_alldata_btn ;
	QPushButton*	m_pCall_kwh_btn ;
	QPushButton*	m_pChk_time_btn ;
	int						m_stationarea_cnt ;
	int						m_terminalview_cnt ;
	DBApi					m_dbapi ;
	PSTATIONAREAPARA						m_pStation_area_para ;
	PTERMINALVIEWPARA					m_pTerminal_view_para ;
	QMap<int , QTreeWidgetItem*>		m_station_area_map ;
	QMap<int , QTreeWidgetItem*>		m_terminal_map ;
	QMap<int , QTreeWidgetItem*>		m_channel_map ;
	FE_MAIL_STRUCT*	m_pMail_struct;		//邮件指针
};

#endif