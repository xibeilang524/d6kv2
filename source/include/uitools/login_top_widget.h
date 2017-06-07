/**
@file login_top_widget.h
@brief µÇÂ¼´°¿Ú±êÌâÀ¸

@author chenkai
@version 1.2.0
@date 2016-11-9
*/
#ifndef LOGIN_TOP_WIDGET_H
#define LOGIN_TOP_WIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>

#define LOGIN_TOP_WIDTH      430
#define LOGIN_TOP_HEIGHT     180

class Login_top_widget : public QFrame
{
	Q_OBJECT
public:
	explicit Login_top_widget( QWidget *parent = 0 ) ;

protected:

private:
	void init_ui( void ) ;
	void init_top_title_widget( void ) ;

private slots:
	void slot_change_skin( void ) ;
	void slot_min( void ) ;
	void slot_close( void ) ;

public:
	QWidget*			m_pTitle_widget ;
	QLabel*				m_pLogo_label ;
	QPushButton*	m_pClose_button ;
	QPushButton*	m_pMin_button ;
	QPushButton*	m_pSkin_button ;
	int						m_pix_index ;
};

#endif