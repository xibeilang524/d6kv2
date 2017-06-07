/**
@file channelpropertyimpl.h
@brief 通道属性对话框头文件

@author lujiashun 
@version 1.0.0
@date 2016-11-8
*/
#ifndef CHANNELPROPERTYIMPL_H
#define CHANNELPROPERTYIMPL_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QAction>

#include "ui_channelproperty.h"
#include "viewintf.h"
#include "wavegraph.h"
#include "comtrade.h"
#include "wavecaption.h"

class Channel_property_impl :public QDialog, public Ui::QDlgChannelProperty
{
    Q_OBJECT

public:
    Channel_property_impl(Wave_graph* g, Comtrade* d, QWidget* parent);
    ~Channel_property_impl();

	void select_item(int idx);

protected:
	void fresh_list();
	void unselect_digtal();
	void select_channel(int idx);
	void update_details();
	int selected_count();
	void set_controls(bool enable = true);
	void zoom(bool enlarge);
	Wave_caption* wave_captions();
	void update_list_checkstate();
	void create_actions();
	// menu
// 	void contextMenuEvent ( QContextMenuEvent* e );

protected slots:
	void slot_select(QListWidgetItem* item);
	void slot_channel_menu(const QPoint& pos);		
	void slot_check_visible();
	void slot_auto_position();
	void slot_zoom_in();
	void slot_zoom_out();
	void slot_auto_size();
	void slot_channel_color();
	void slot_caption_change();
	void slot_set_position();
	void slot_resume_defualt();
	void slot_locate_channel(QListWidgetItem* item);
	void slot_check_select();

	void slot_select_all(bool);		///< 全选
	void slot_select_analog_channel();		///< 模拟量通道
	void slot_select_digtal_channel();		///< 开关量通道
	void slot_select_A_channel();		///< A相通道
	void slot_select_B_channel();		///< B相通道
	void slot_select_C_channel();		///< C相通道

private:
	Wave_graph* m_pGraph;
	Comtrade* m_pData;

	QMenu *m_pop_menu;
	QAction *action_select_all;		///< 全选
	QAction *action_analog_chennel;	///< 模拟量通道
	QAction *action_digtal_channel;	///< 开关量通道
	QAction *action_channel_A;	///< A相通道
	QAction *action_channel_B;	///< B相通道
	QAction *action_channel_C;	///< C相通道

};

#endif