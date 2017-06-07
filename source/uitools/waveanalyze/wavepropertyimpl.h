/**
@file wavepropertyimpl.h
@brief 波形图属性对话框头文件
@author lujiashun 
@version 1.0.0
@date 2016-11-07
*/
#ifndef WAVEPROPERTYIMPL_H
#define WAVEPROPERTYIMPL_H

#include <qlistview.h>
#include "ui_waveproperty.h"

class Wave_property_impl :public QDialog, public Ui::WaveProperty
{
    Q_OBJECT

public:
    Wave_property_impl( QWidget* parent = 0,  Qt::WindowFlags f = 0 );
    ~Wave_property_impl();

	QColor Colors(int n);
	void color_set(int n, const QColor& clr);
	bool Checked(int n);
	void set_checked(int n, bool check);
	void set_edit_data(int n, double val);
	double edit_data(int n);

protected:
	void accept();
	void init_controls();
	bool event_filter(QObject* watched, QEvent* e);

	void set_colors(int n);
	QPushButton* get_button(int n);
	QListWidgetItem* get_item(int n);
	QLineEdit* get_edit(int n);

protected slots:
	void slot_set_color1();
	void slot_set_color2();
	void slot_set_color3();
	void slot_set_color4();
	void slot_set_color5();

};

#endif