/**
@file tool_button_frame.h
@brief 封装菜单栏头文件

@author lujiashun
@version 1.0.0
@date 2016-09-19
*/
#ifndef TOOL_BUTTON_FRAME_H
#define TOOL_BUTTON_FRAME_H

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

#include "tool_button.h"

class Tool_button_frame : public QFrame
{
	Q_OBJECT
public:
	Tool_button_frame( QWidget* parent = 0 ) ;
	
	/**
	@brief 设置暂停按钮状态
	*/
	void set_pause_checked(bool);

protected:

private:
	void init_tool_button_frame( void ) ;

	private slots:

public:
	Tool_button*	m_pwin_set_button ;				///< 窗口配置
	Tool_button*	m_pwin_top_button ;				///< 窗口置顶
	Tool_button*	m_ppause_button ;					///< 暂停刷新
	Tool_button* m_pdelete_button;					///< 删除事项
	Tool_button* m_pconfirm_sound_button;		///< 语音确认
	Tool_button* m_pconfirm_all_button;			///< 确认所有
	Tool_button* m_pconfirm_event_button;		///< 事项确认

	QHBoxLayout *m_phorizontal_layout;
	QSpacerItem *m_phorizontal_spacer;

private:
	bool m_bpause;
};

#endif