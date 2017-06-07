#include "login_top_widget.h"

#include <QLabel>
#include <QMouseEvent>
#include <QHBoxLayout>

Login_top_widget::Login_top_widget( QWidget *parent ) : QFrame( parent )
{
	m_pix_index = 1 ;
	this->init_ui();
	this->setObjectName( "topwidget" ) ;
	this->setStyleSheet( tr( "#topwidget {background-image: url(:/res/1.png);}" ) ) ;
}

void Login_top_widget::init_ui( void )
{
	this->setGeometry( 0 , 0 , LOGIN_TOP_WIDTH , LOGIN_TOP_HEIGHT ) ;
	this->init_top_title_widget() ;
}

void Login_top_widget::init_top_title_widget( void )
{
	m_pTitle_widget = new QWidget( this ) ;
	m_pTitle_widget->setAttribute( Qt::WA_TranslucentBackground ) ;
	m_pTitle_widget->setFixedWidth( width() ) ;
	m_pTitle_widget->move( 0 , 0 ) ;

	m_pClose_button = new QPushButton( this ) ;
	m_pClose_button->setObjectName( tr( "closebutton" ) ) ;
	m_pMin_button = new QPushButton( this ) ;
	m_pMin_button->setObjectName( tr( "minbutton" ) ) ;
	m_pSkin_button = new QPushButton( this ) ;
	m_pSkin_button->setObjectName( tr( "skinbutton" ) ) ;

	m_pClose_button->move( LOGIN_TOP_WIDTH - 25 , 0 ) ;
	m_pMin_button->move( LOGIN_TOP_WIDTH - 50 , 0 ) ;
	m_pSkin_button->move( LOGIN_TOP_WIDTH - 75 , 0 ) ;

	m_pLogo_label = new QLabel( this ) ;
	m_pLogo_label->setFixedSize( 110 , 37 ) ;
	m_pLogo_label->move( 160 , 72 ) ;
	m_pLogo_label->setObjectName( "logolabel" ) ;

	connect( m_pSkin_button , SIGNAL( clicked() ) , this , SLOT( slot_change_skin() ) ) ;
	connect( m_pMin_button , SIGNAL( clicked() ) , this , SLOT( slot_min() ) ) ;
	connect( m_pClose_button , SIGNAL( clicked() ) , this , SLOT( slot_close() ) ) ;

	QString stylesheet = tr( "#closebutton\
							 {\
							 background-color:transparent;\
							 image: url(:/res/close.png);\
							 border: 0px;\
							 min-width: 25px;\
							 min-height: 25px;\
							 max-width: 25px;\
							 max-height: 25px;\
							 }\
							 #closebutton:hover\
							 {\
							 background-image: url(:/res/closebutton.png);\
							 image: url(:/res/close.png);\
							 border: 0px;\
							 }\
							 #closebutton:checked\
							 {\
							 background-image: url(:/res/closebutton.png);\
							 image: url(:/res/close.png);\
							 border: 0px;\
							 }\
							 ") ;
	m_pClose_button->setStyleSheet( stylesheet ) ;

	stylesheet = tr( "#minbutton\
							 {\
							 background-color:transparent;\
							 image: url(:/res/min.png);\
							 border: 0px;\
							 min-width: 25px;\
							 min-height: 25px;\
							 max-width: 25px;\
							 max-height: 25px;\
							 }\
							 #minbutton:hover\
							 {\
							 background-image: url(:/res/background_press.png);\
							 image: url(:/res/min.png);\
							 border: 0px;\
							 }\
							 #minbutton:checked\
							 {\
							 background-image: url(:/res/background_press.png);\
							 image: url(:/res/min.png);\
							 border: 0px;\
							 }") ;
	m_pMin_button->setStyleSheet( stylesheet ) ;

	stylesheet = tr( "#skinbutton\
					 {\
					 background-color:transparent;\
					 image: url(:/res/skin.png);\
					 border: 0px;\
					 min-width: 25px;\
					 min-height: 25px;\
					 max-width: 25px;\
					 max-height: 25px;\
					 }\
					 #skinbutton:hover\
					 {\
					 background-image: url(:/res/background_press.png);\
					 image: url(:/res/skin.png);\
					 border: 0px;\
					 }\
					 #skinbutton:checked\
					 {\
					 background-image: url(:/res/background_press.png);\
					 image: url(:/res/skin.png);\
					 border: 0px;\
					 }") ;
	m_pSkin_button->setStyleSheet( stylesheet ) ;

	stylesheet = tr( "#logolabel\
					 {\
					 image: url(:/res/Daqo_en.png);\
					 background-color:transparent;\
					 }") ;
	m_pLogo_label->setStyleSheet( stylesheet ) ;
}

void Login_top_widget::slot_change_skin( void )
{
	m_pix_index++ ;
	m_pix_index = m_pix_index % 7 ;
	this->setStyleSheet( tr( "#topwidget {background-image: url(:/res/%1.png);}" ).arg( m_pix_index ) ) ;
}

void Login_top_widget::slot_min( void )
{
	( ( QWidget* )this->parent() )->showMinimized() ;
}

void Login_top_widget::slot_close( void )
{
	( ( QWidget* )this->parent() )->close() ;
}