/**
@file raw_data_widget.h
@brief 生数据窗口

@author chenkai 
@version 1.0.0
@date 2016-07-31
*/
#ifndef RAWDATAWIDGET_H_
#define RAWDATAWIDGET_H_

#include <QFrame>
#include <QTabBar>
#include <QTableWidget>

#include "festools_globaldef.h"
#include "msg_data_widget.h"
#include "db/dbapi.h"

class raw_data_widget : public QFrame
{
	Q_OBJECT
public:
	raw_data_widget( QWidget* parent = NULL ) ;
	void hide_all_widgets( bool ) ;
	void change_channel_no( int ) ;
	void change_termianl_no( int ) ;

protected:

private:
	void ini_raw_data_widget() ;
	void ini_raw_data_table( int ) ;
	bool read_yc_data() ;
	bool read_yx_data() ;
	bool read_kwh_data() ;
	void fill_raw_desc() ;
	void fill_raw_value() ;
	void clear_rcd_list() ;
	bool	 read_file( char *filename, void ** buf );
	bool	 read_file( char *filename, void * buf, int len );

private slots:
	void on_tab_changed( int ) ;
	void on_timer() ;

private:
	int								m_table_type ;
	int								m_channel_no ;
	int								m_terminal_no ;
	Fes_shm&					m_fes_shm ;
	QTabBar*					m_pTab_bar ;
	QTableWidget*			m_pData_table ;
	DBApi							m_db ;
	PRTDATA					m_yc_list ;
	PRTDATA					m_yx_list ;
	PRTDATA					m_kwh_list ;
	QTimer*						m_pTimer ;

	int								m_yc_cnt;
	int								m_yx_cnt;
	int								m_kwh_cnt;
};

#endif