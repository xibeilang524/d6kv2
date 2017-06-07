/**
@file event_ui_set_dlg.h
@brief 窗口配置对话框类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef EVENT_UI_SET_DLG_H
#define EVENT_UI_SET_DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_qeventuisetbase.h"
#include <QMessageBox>
#include "uitools/dmessagebox.h"

#include <QMap>

class Event_table;


class Event_ui_set_dlg : public QDialog, public Ui::QEventUISetBase  
{
	Q_OBJECT
public:
	Event_ui_set_dlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
	virtual ~Event_ui_set_dlg();

public slots:
	//增加页
	void slot_add_page();
	//修改页名称
	void slot_modify_page();
	//删除页
	void slot_del_page();
	//双击页面设置页的页列表
	void slot_double_clicked_pagelist( QTreeWidgetItem * item);
	//刷新页列表
	void slot_refresh_pagelist( );

	//刷新事项页面的页列表
	void slot_refresh_pageCB( int item );
	//刷新事项过滤树型
	void slot_refresh_filter( int item );

	void slot_event_type_listview_clicked( QTreeWidgetItem * pItem );

	void slot_save_filter();

	void slot_cancel_btn();

	virtual void show_page ( int );
private:
	//void refreshPage2();
	//void refreshPage3();
	void set_wnd_stylesheet();

	//创建事项过滤树型
	void create_filter_tree();
	void init_para();
	void deal_child_items(Event_table *pTable,QTreeWidgetItem *item);

	void set_child_checkstate(QTreeWidgetItem *item,Qt::CheckState cs);
	void set_parent_checkstate(QTreeWidgetItem *item);
	bool is_topitem(QTreeWidgetItem *item);

private:
	QMap<QTreeWidgetItem*,uint>	m_item_to_eventtype_map;
	QMap<QTreeWidgetItem*,QString> m_item_to_rtucode_map;
	QMap<QTreeWidgetItem*,uint>	m_item_to_pageid_map;

	QMap<int , QStringList> m_station_area_map ;

	QList<QStringList> m_station_area_para ;		//区域
	QList<QStringList> m_station_para ;			//厂站

};

#endif 