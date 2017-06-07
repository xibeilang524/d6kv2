/**
@file rdb_ycyx_dialog.h
@brief 相关显示设置窗口

@author chenkai
@version 1.2.0
@date 2016-11-4
*/

#ifndef RDB_YCYX_DIALOG_H
#define RDB_YCYX_DIALOG_H

#include "ui_rdb_ycyx_dialog_base.h"
#include "data_info.h"

class Rdb_ycyx_dialog : public QDialog , public Ui::Rdb_ycyx_dialog_base
{
	Q_OBJECT
public:
	Rdb_ycyx_dialog( QWidget* parent = 0 ) ;
protected:

private slots:
	void slot_ok() ;
	void slot_cancel() ;
private:
};

#endif