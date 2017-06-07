/**
@file hisquery_wnd.h
@brief hisquery主界面

@author chenkai
@version 1.2.0
@date 2016-11-7
*/

#ifndef HISQUERY_WND_H
#define HISQUERY_WND_H

#include <QMainWindow>
#include <QToolButton>
#include <QCheckBox>
#include <QFrame>
#include <QGroupBox>
#include <QDateTimeEdit>
#include <QTableWidget>
#include <QTreeWidget>

#include "scd/scdapi.h"
#include "uitools/login_widget.h"

#include "event_para.h"

class Hisquery_wnd : public QMainWindow
{
	Q_OBJECT
public:
	Hisquery_wnd( QWidget* parent = 0 ) ;
	~Hisquery_wnd() ;
	Event_para			m_db_para ;

protected:
	virtual void closeEvent(QCloseEvent *event);
	virtual void timerEvent(QTimerEvent *event);

private:
	void ini_mainwindow( void ) ;
	void ini_toolbar( void ) ;
	void ini_filterframe( void ) ;
	void ini_para( void ) ;
	void ini_classified_query_page( void ) ;
	void ini_advanced_query_page( void ) ;

	bool reg_proc( void ) ;
	void un_reg_proc( void ) ;

	void find_event_by_eventtype( void ) ;
	void fill_event_table( void ) ;

private slots:
	void slot_print( void ) ;
	void slot_event_output( void ) ;
	void slot_event_delete( void ) ;
	void slot_exit( void ) ;
	void slot_today_button( void ) ;
	void slot_yesterday_button( void ) ;
	void slot_toolbar_search_button( void ) ;
	void slot_event_type_query( void ) ;
	void slot_dev_query( void ) ;
	void slot_fuzzy_query( void ) ;
	void slot_all_type( void ) ;
	void slot_all_station( void ) ;
	void slot_event_type_tree_clicked( QTreeWidgetItem* pItem , int column ) ;
	void slot_show_station_dev( QTreeWidgetItem* pItem ) ;
	void slot_modify_event( QTableWidgetItem* pItem ) ;

private:
	QFrame*			m_pCentral_frame ;
	QFrame*			m_pToolbar_frame ;
	QFrame*			m_pMain_frame ;
	QFrame*			m_pFilter_frame ;
	QTableWidget*	m_pEvent_table ;
	QToolButton*	m_pEvent_print_button ;
	QToolButton*	m_pEvent_output_button ;
	QToolButton*	m_pEvent_delete_button ;
	QToolButton*	m_pEvent_exit_button ;
	QToolButton*	m_pYesterday_button ;
	QToolButton*	m_pToday_button ;
	QToolButton*	m_pToolbar_search_button ;
	QToolButton*	m_pAll_type_button ;
	QToolButton*	m_pAll_station_button ;
	QToolButton*	m_pClassified_query_button ;
	QCheckBox*		m_pSearch_in_result_checkbox ;
	QDateTimeEdit*	m_pBegin_datatime_edit ;
	QDateTimeEdit*	m_pEnd_datatime_edit ;
	QTabWidget*		m_pFilter_tabwidget ;
	QFrame*			m_pClassified_query_frame ;
	QFrame*			m_pAdvances_query_frame ;
	QFrame*			m_pDev_query_frame ;
	QGroupBox*		m_pFuzzy_query_frame ;
	QToolButton*	m_pDev_query_button ;
	QLineEdit*		m_pFuzzy_query_lineedit ;
	QToolButton*	m_pFuzzy_query_button ;
	QTreeWidget*	m_pEvent_type_select_tree ;
	QTreeWidget*	m_pStation_select_tree ;
	QTreeWidget*	m_pDev_select_tree ;
	USEROPERRIGHT_STRU *	m_pCur_user ;
	DISPATCHER_STRU				m_cur_oper_man ;

	QList<HIS_EVENT *>		m_event_list ;
	QMap<int , QStringList> m_station_area_map ;
	QList<QStringList> m_station_area_para ;		//区域
	QList<QStringList> m_station_para ;				//厂站

	//按类型查询页用
	QMap<QTreeWidgetItem*, uint> m_item_to_event_type_map ;
	QMap<uint, QTreeWidgetItem*> m_event_type_to_item_map ;
	QMap<QTreeWidgetItem*,QString> m_item_to_rtu_code_map ;
	QMap<QString,QTreeWidgetItem*> m_rtu_code_to_item_map ;

	//按设备查询页用
	QMap<QTreeWidgetItem*,QString> m_item_to_dev_code_map ;
	QMap<QString,QTreeWidgetItem*> m_dev_code_to_item_map ;
	QMap<QTreeWidgetItem*,QString> m_dev_page_item_to_rtu_code_map ;

	QStringList			m_object_code_list ;
	QStringList			m_rtu_code_list ;
	QList<uint>			m_event_type_list ;
	QString					m_fuzzy_string ;

	ProcMan			m_proc_man ;
	NetDbg			m_net_dbg ;
	QPrinter*		m_pPrinter ;
	QStatusBar*	m_pStatus_bar ;
	SYS_TIME			m_beginTime;
	SYS_TIME			m_endTime;
	int					m_proc_time_id ;
};

#endif