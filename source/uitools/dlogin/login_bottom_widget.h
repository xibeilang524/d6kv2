/**
@file login_bottom_widget.h
@brief 登录下半部分窗口

@author chenkai
@version 1.2.0
@date 2016-11-9
*/

#ifndef LOGIN_BOTTON_WIDGET_H
#define LOGIN_BOTTON_WIDGET_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include "scd/scdapi.h"
#include "db/evtapi.h"

#define LOGIN_BOTTOM_WIDTH      430
#define LOGIN_BOTTOM_HEIGHT     150
#define LOGIN_TOP_WIDTH      430
#define LOGIN_TOP_HEIGHT     180

class Login_bottom_widget : public QFrame
{
	Q_OBJECT
public:
	explicit Login_bottom_widget( QWidget* parent = 0 ) ;
	void get_user_right( USEROPERRIGHT_STRU& user_right ) ;
	void get_oper_info( DISPATCHER_STRU& oper_info ) ;
	DISPATCHER_STRU*	get_oper_infos( void ) ;
	USEROPERRIGHT_STRU*	get_user_rights( void ) ;
	int get_user_count( void ) ;
	int get_host_count( void ) ;

protected:

private:
	void ini_ui( void ) ;
	bool read_file( char* file_name , void ** buf ) ;
	bool read_file( char* file_name , void * buf, int len ) ;
	int read_user_para( void ) ;
	void ini_login_dialog( void ) ;

public slots:
	void slot_user_type_changed( int index ) ;
	void slot_ok_btn( void ) ;
	void slot_register_btn( void ) ;
	void slot_find_password_btn( void ) ;

public:
	QPushButton*			m_pLogin_button ;
	QPushButton*			m_pRegister_button ;
	QPushButton*			m_pFind_password_button ;
	QComboBox*			m_pUser_type_combobox ;
	QComboBox*			m_pUser_name_combobox ;
	QLineEdit*				m_pPassword_lineedit ;
	QLabel*						m_pUser_pix_label ;

private:
	DISPATCHER_STRU				m_cur_oper_man ;
	USEROPERRIGHT_STRU		m_cur_user_right ;
	DISPATCHER_STRU*				m_pOper_dispatcher ;
	DISPATCHER_STRU*				m_pCur_oper_dispatcher ;
	USEROPERRIGHT_STRU*	m_pOper_right ;
	USEROPERRIGHT_STRU*	m_pCur_right ;

	int m_user_types[ 10 ] ;
	int m_user_type_count ;
	int m_user_count ;
	int m_host_user_count ;
};

#endif