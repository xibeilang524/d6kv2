/**
@file rdb_scada_dialog.h
@brief SCADA实时表设置窗口

@author chenkai
@version 1.2.0
@date 2016-11-4
*/
#ifndef RDB_SCADA_DIALOG_H
#define RDB_SCADA_DIALOG_H

#include "ui_rdb_scada_dialog_base.h"

class Rdb_scada_dialog : public QDialog , public Ui::Rdb_scada_dialog_base
{
	Q_OBJECT
public:
	Rdb_scada_dialog( QWidget* parent = 0 ) ;
protected:

private slots:
	void slot_exit( void ) ;
	void slot_select_all( void ) ;
	void slot_select_none( void ) ;
	void slot_save_config( void ) ;
	void slot_table_select_change( int ) ;

private:
	bool m_bChanged ;
};

#endif