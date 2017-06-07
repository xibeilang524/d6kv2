/**
@file hisquery_modify_dialog.h
@brief ÊÂÏî±à¼­´°¿Ú

@author chenkai
@version 1.2.0
@date 2016-11-10
*/

#ifndef HISQUERY_MODIFY_DIALOG_H
#define HISQUERY_MODIFY_DIALOG_H

#include "ui_hisquery_modify_base.h"

#include "sysstr.h"

extern QWidget* getMainWnd();

class Event_modify_dialog : public QDialog , public Ui::Event_modify_dialog_base
{
	Q_OBJECT
public:
	Event_modify_dialog( HIS_EVENT *eve , QWidget* parent = 0 ) ;
protected:
private:
	void ini_ui( void ) ;
	void ini_data( HIS_EVENT *eve ) ;
	void get_event();

private:
	QList<int>				m_group_no_list;
	QList<int>				m_event_type_list;
	QStringList			m_station_list;
	QStringList			m_object_list;

	HIS_EVENT			m_cur_event;
	HIS_EVENT			m_old_event;

private slots:
	void slot_event_sort_changed( int item ) ;
	void slot_event_type_changed( int item ) ;
	void slot_rtu_changed( int item ) ;
	void slot_group_changed( int item ) ;
	void slot_modify_event( void ) ;
	void slot_delete_event( void ) ;
	void slot_add_event( void ) ;
};

#endif