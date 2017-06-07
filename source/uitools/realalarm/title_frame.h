/**
@file title_frame.h
@brief 封装标题栏头文件

@author lujiashun
@version 1.0.0
@date 2016-09-18
*/

#ifndef TITLE_FRAME_H
#define TITLE_FRAME_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpacerItem>

/**
@brief 标题栏类

标题栏类，继承自QFrame，封装好标题栏，方便主窗口直接调用
*/
class Title_frame : public QFrame
{
	Q_OBJECT
public:
	Title_frame( QWidget* parent = 0 ) ;

private:
	/**
	@brief 初始化，绘制标题栏
	*/
	void init_title_frame( void ) ;

protected:
	void mouseDoubleClickEvent( QMouseEvent * event ) ;

private slots:
	/**
	@brief 最小化槽函数
	*/
	void slot_min_widget( void ) ;
	/**
	@brief 最大化槽函数
	*/
	void slot_max_widget( void ) ;
	/**
	@brief 关闭槽函数
	*/
	void slot_close_widget( void ) ;
	/**
	@brief 关于槽函数
	*/
	void slot_about_dlg(void);

public:
	QLabel* m_plogo_label ;	///< 图标标签
	QLabel* m_ptext_label ;		///< 标题标签
	QPushButton*	m_pmin_button ;		///< 最小化按钮
	QPushButton*	m_pmax_button ;	///< 最大化按钮
	QPushButton*	m_pclose_button ;	///< 关闭按钮
	QPushButton* m_psetting_button;	///< 设置按钮
	QPushButton* m_pabout_button;	///< 关于按钮
	QGroupBox*  m_pbutton_group;	///< 按钮组
	QHBoxLayout* m_pbutton_layout;
	QHBoxLayout* m_phbox_layout;
	QSpacerItem* m_pspace;
	bool is_max;		///< 是否最大化标志
};

#endif