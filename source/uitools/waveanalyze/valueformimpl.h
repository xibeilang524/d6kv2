/**
@file valueformimpl.h
@brief 曲线信息对话框头文件
@author lujiashun 
@version 1.0.0
@date 2016-11-24
*/
#ifndef VALUEFORMIMPL_H
#define VALUEFORMIMPL_H

#include <QTableWidget>

#include "channelunit.h"
#include "analyzeutils.h"
#include "selectdetailimpl.h"

#include "ui_valueform.h"

class Dlg_value_impl : public QWidget,public Ui::QDlgValue
{
    Q_OBJECT

public:
    Dlg_value_impl( QWidget* parent = 0, Qt::WindowFlags f= 0);
    ~Dlg_value_impl();
	
	void Fresh(int pos);
	void set_analyze(QAnalyze* fun);
	void update_grid_color(QColor* args);
	QAnalyze* Analyze()			{ return m_pAnalyze; }

protected slots:
	void slot_analog_context_menu(const QPoint& pos);
	void slot_digtal_context_menu(const QPoint& pos);

	void slot_ana_name(void);
	void slot_ana_select_column(void);
	void slot_signal(void);
	void slot_cursor(void);
	void slot_hide(void);
	void slot_select_dig_column(void);

protected:
	void init(QWidget* parent);
	void update_table(QTableWidget* tab, QDetails* dets);
	void fresh_table(QTableWidget* tab, Channels* channels);
	void fresh_analog(int pos);
	void fresh_digtal(int pos);
	void update_dig_table();
	void show_all_switch();

	QTableWidget* get_table(int index);

private:
	QAnalyze* m_pAnalyze;

	QMenu *menu_ana_pop;	
	QMenu *menu_dig_pop;

	QAction *action_name;	///< 名称
	QAction *action_select_ana_column;  ///< 模拟量 选择列
	QAction *action_signal;		///< 信号
	QAction *action_cursor;		///< 游标基准
	QAction *action_hide;			///< 隐藏无变位通道
	QAction *action_select_dig_column;		///< 电气量 选择列

};

#endif