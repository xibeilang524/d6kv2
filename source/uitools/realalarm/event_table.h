/**
@file event_table.h
@brief 封装事项表格类 继承自QTreeWidget

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/


#ifndef EVENT_TABLE_H
#define EVENT_TABLE_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMap>
// #include <qptrdict.h>
#include <QPixmap>
#include "list_view_color_item.h"

#include "event_filter.h"

class Event_cfg;
class Event_para;
class QPopupMenu;

#define CONFIRM_MENU	100
#define CONFIRMALL_MENU 101
#define DELETE_MENU		102
#define CURVE_MENU		103
#define GRAPH_MENU		104

//读取所有事项，一页对应一个过滤器，如果该事项通过过滤则显示到该页
class Event_table : public QTreeWidget, public Event_filter
{
	Q_OBJECT
public:
	Event_table( Event_cfg *pCfg, Event_para *pEventPara, QWidget * parent = 0, const char * name = 0, Qt::WindowFlags f = 0 );
	virtual ~Event_table();

	virtual void add_event( Rsys_event * eve );
	virtual void confirm_event( Rsys_event * eve );
	virtual void del_event( Sys_event_list & eventlist );

	virtual void set_page_name( QString name );
	QString get_page_name( );

	void set_page_index(int index);
	int	get_page_index();
	
	void refresh_table();

	//得到界面上选择事项
	bool get_selected_event( Sys_event_list & eventlist );

	virtual void timerEvent ( QTimerEvent * e );

	virtual void closeEvent( QCloseEvent * e );

	virtual void clear();

	bool m_bhave_new_event;

	int			m_lock;//设置图标锁
	int		m_tabel_index;	//当前table序号，用于区分页面和窗口

public slots:
	void slot_pop_menu( QAction * );

// 	void rightClicked( QTreeWidgetItem *, const QPoint &, int );
	void right_clicked( QTreeWidgetItem *,  int );
	
	void slot_current_page_changed(QTreeWidgetItem *,  int );
// 	void onClickColumn(int);
private:
	QMap<Rsys_event*, List_view_color_item*> m_event_to_item_map;

	Event_cfg			*m_cfg;
	Event_para			*m_event_para;

	void fill_table( int row, Rsys_event * eve );

	void get_event_color( Rsys_event *eve, QColor &clr );

	QString		m_page_name;

// 	QPixmap		m_nullPixmap;
// 	QPixmap		m_ConfirmedPixmap;
// 	QPixmap		m_unConfirmPixmap;
// 	QPixmap		m_MatchingPixmap;
// 	QPixmap		m_matchedPixmap;
	QIcon		m_null_pixmap;
	QIcon		m_confirmed_pixmap;
	QIcon		m_unconfirm_pixmap;
	QIcon		m_matching_pixmap;
	QIcon		m_matched_pixmap;

	//表格中已填入事项数
	uint		m_event_num;

	QMenu		*m_right_pop_menu;
	QFont		m_cur_font;
	int			m_fresh_timer;
};

#endif 