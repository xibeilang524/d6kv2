/**
@file login_widget.h
@brief 登录主窗口

@author chenkai
@version 1.2.0
@date 2016-11-9
*/
#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

class Login_top_widget ;
class Login_bottom_widget ;

#include <QFrame>
#include <QDialog>

#include "scd/scdapi.h"

#ifdef WIN32
#ifdef UI_LOGIN_DLL
#define	UI_LOGIN_EXPORT __declspec(dllexport) 
#else
#define	UI_LOGIN_EXPORT __declspec(dllimport) 
#endif
#else
#define	UI_LOGIN_EXPORT
#endif

class UI_LOGIN_EXPORT Login_widget : public QDialog
{
	Q_OBJECT
public:
	explicit Login_widget( QWidget* parent = 0 ) ;
	void get_user_right( USEROPERRIGHT_STRU& user_right );
	void get_oper_info( DISPATCHER_STRU& oper_info );
	int get_user_count( void ) ;
	int get_host_count( void ) ;
	DISPATCHER_STRU*	get_oper_infos( void ) ;
	USEROPERRIGHT_STRU*	get_user_rights( void ) ;

protected:
	void mousePressEvent( QMouseEvent* event ) ;
	void mouseReleaseEvent( QMouseEvent* event ) ;
	void mouseMoveEvent( QMouseEvent* event ) ;
	void keyPressEvent ( QKeyEvent * event ) ;

private:
	void init_ui() ;

private:
	QPoint									m_move_point ; //移动的距离
	bool											m_bMouse_press ; //按下鼠标左键
	Login_top_widget*				m_pLogin_top_widget ;
	Login_bottom_widget*		m_pLogin_bottom_widget ;
	DISPATCHER_STRU			m_cur_oper_man ;
	USEROPERRIGHT_STRU	m_cur_power ;
};

#endif