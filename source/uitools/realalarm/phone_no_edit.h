/**
@file phone_no_edit.h
@brief 电话号码编辑窗口类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef PHONE_NO_EDIT_H_
#define PHONE_NO_EDIT_H_

#include "ui_qphonenoedit.h"
#include "db/dbapi.h"
#include "globaldef.h"
#include "event_cfg.h"

#include <QTreeWidgetItem>

class Staion_tree_item :public QTreeWidgetItem
{
public:
	int m_station_no;
public:
	Staion_tree_item(const QStringList & strings, int station_no);
	virtual ~Staion_tree_item();

	int get_station_no(){return m_station_no;};

};

class Phone_no_edit : public QDialog, public Ui::phoneNoEditDlg  
{
	Q_OBJECT
public:
// 	int m_reportCount ;

public:
	Phone_no_edit( Event_cfg* pcfg, QString phoneNo_list, uint type ,QWidget* parent );
// 	void setCfgPoint(QEventCfg* cfg){ m_eventcfg = cfg; };
	QString get_descname();
	virtual ~Phone_no_edit();

	void show_all_staion();
	int select(const char* sql, void**result_pt);
public slots:
	void slot_refresh_desc_tree(QTreeWidgetItem * item, int column);
	void slot_refresh_selected_desc_tree(const QString &phoneNo);
	void slot_add_desc();
	void slot_delete_desc();
	void slot_load_desc();
private:
	Event_cfg * m_event_cfg;
	uint m_event_type;

	DBApi m_db;
	bool m_bload;
};

#endif
