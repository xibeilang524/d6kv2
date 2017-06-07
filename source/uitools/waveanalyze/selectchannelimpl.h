/**
@file selectchannelimpl.h
@brief 选择通道对话框头文件
@author lujiashun 
@version 1.0.0
@date 2016-11-24
*/
#ifndef SELECTCHANNELIMPL_H
#define SELECTCHANNELIMPL_H

#include <qstringlist.h>
#include "ui_selectchannel.h"

class Dlg_select_channel_impl :public QDialog,public Ui::QDlgSelectChannel
{
    Q_OBJECT

public:
    Dlg_select_channel_impl( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~Dlg_select_channel_impl();

	void set_multi_check(bool b);
	bool Checked(int index);
	int only_check_index();
	void set_only_analog(int val);
	int Count();
	int check_count();
	void add_item(const QString& str, bool b = true);
	void set_state(bool ana);
	void select_items(bool b);

protected slots:
	void slot_select_all();
	void slot_unselect();
	void slot_style_clicked(bool checked);
	void slot_on_item(QListWidgetItem* current,QListWidgetItem* previous);


private:
	bool	m_multi_check_flag;
	QStringList m_analog_list;
	QStringList m_digtal_list;

};


#endif