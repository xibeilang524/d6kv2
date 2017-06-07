#include "login_widget.h"
#include "login_top_widget.h"
#include "login_bottom_widget.h"

#include <QMouseEvent>

Login_widget::Login_widget( QWidget* parent /* = 0 */ ) : QDialog( parent ) , m_bMouse_press( false )
{
	this->init_ui() ;
	this->setWindowTitle( tr( "D6000登录窗口" ) ) ;
	this->setWindowIcon( QIcon( ":/res/login_hover.png" ) ) ;
	this->setFixedSize( 430 , 330 ) ;
}

void Login_widget::init_ui()
{
	setWindowFlags( Qt::FramelessWindowHint | Qt::Dialog ) ;

	m_pLogin_bottom_widget = new Login_bottom_widget( this ) ;
	m_pLogin_top_widget = new Login_top_widget( this ) ;

	QWidget::setTabOrder( m_pLogin_bottom_widget->m_pUser_type_combobox , m_pLogin_bottom_widget->m_pUser_name_combobox ) ;
	QWidget::setTabOrder( m_pLogin_bottom_widget->m_pUser_name_combobox , m_pLogin_bottom_widget->m_pPassword_lineedit ) ;
	QWidget::setTabOrder( m_pLogin_bottom_widget->m_pPassword_lineedit , m_pLogin_bottom_widget->m_pLogin_button ) ;
	QWidget::setTabOrder( m_pLogin_bottom_widget->m_pLogin_button , m_pLogin_bottom_widget->m_pRegister_button ) ;
	QWidget::setTabOrder( m_pLogin_bottom_widget->m_pRegister_button , m_pLogin_bottom_widget->m_pFind_password_button ) ;
}

void Login_widget::mousePressEvent( QMouseEvent *event )
{
	//只能是鼠标左键移动和改变大小
	if( event->button() == Qt::LeftButton )
	{
		m_bMouse_press = true ;
	}
	//窗口移动距离
	m_move_point = event->globalPos() - pos() ;
}

void Login_widget::mouseReleaseEvent( QMouseEvent * )
{
	m_bMouse_press = false ;
}

void Login_widget::mouseMoveEvent( QMouseEvent *event )
{
	//移动窗口
	if( m_bMouse_press )
	{
		QPoint movePos = event->globalPos() ;
		move( movePos - m_move_point ) ;
	}
}

void Login_widget::get_user_right( USEROPERRIGHT_STRU& user_right )
{
	m_pLogin_bottom_widget->get_user_right( user_right ) ;
}

void Login_widget::get_oper_info( DISPATCHER_STRU& oper_info )
{
	m_pLogin_bottom_widget->get_oper_info( oper_info ) ;
}

void Login_widget::keyPressEvent( QKeyEvent * event )
{
	if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
	{
		m_pLogin_bottom_widget->slot_ok_btn() ;
	}
}

int Login_widget::get_user_count( void )
{
	return m_pLogin_bottom_widget->get_user_count() ;
}

DISPATCHER_STRU* Login_widget::get_oper_infos( void )
{
	return m_pLogin_bottom_widget->get_oper_infos() ;
}

int Login_widget::get_host_count( void )
{
	return m_pLogin_bottom_widget->get_host_count() ;
}

USEROPERRIGHT_STRU* Login_widget::get_user_rights( void )
{
	return m_pLogin_bottom_widget->get_user_rights() ;
}
