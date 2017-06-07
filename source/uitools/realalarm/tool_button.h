/**
@file tool_button.h
@brief 封装菜单按钮头文件
重新封装按钮，以达到自己想要的效果
@author lujiashun
@version 1.0.0
@date 2016-09-18
*/

#ifndef TOOL_BUTTON_H
#define TOOL_BUTTON_H

#include <QPushButton>

class Tool_button : public QPushButton
{
	Q_OBJECT
public:
/**
@para in  QString button_name 按钮名称
@para in  QString button_desc	按钮描述
@para in  QString button_img	图标名
@para in  QWidget * parent 父窗体
*/
Tool_button( QString button_name , QString button_desc , QString button_img = 0 , QWidget* parent = 0 ) ;


private:
	void init_tool_button( void ) ;

private:
	QPushButton* m_ptool_button ;
	QString  m_button_name ;		///< 按钮名称
	QString  m_button_desc ;		///< 按钮描述
	QString  m_button_img ;			///< 按钮图标
};

#endif