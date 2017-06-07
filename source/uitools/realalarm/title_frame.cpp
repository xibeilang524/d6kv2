#include <QApplication>

#include "title_frame.h"
#include "globaldef.h"


extern QWidget* getMainWnd() ;
#include "event_real_wnd.h"
#include "uitools/dmessagebox.h"

Title_frame::Title_frame( QWidget* parent /*= 0 */ ) : QFrame( parent )
{
	setObjectName( tr( "titleframe" ) ) ;
	is_max = false;
	init_title_frame() ;
}

void Title_frame::init_title_frame( void )
{
	m_plogo_label = new QLabel( this ) ;
	m_plogo_label->setGeometry(QRect(28,18,45,18));
	m_plogo_label->setPixmap( QPixmap( ":/img/images/realalarm.png" ) ) ;
	m_plogo_label->setObjectName( tr( "logolabel" ) ) ;
	m_ptext_label = new QLabel( this ) ;
	m_ptext_label->setText( tr( "D6000智能告警" ) ) ;
	m_ptext_label->setObjectName( tr( "textlabel" ) ) ;
// 	m_ptext_label->setGeometry(QRect(68, 17, 276, 18));

	m_pspace = new QSpacerItem(200,18,QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_pbutton_group = new QGroupBox(this);
	m_pbutton_group->setObjectName(tr("groupBox"));
	m_pbutton_layout = new QHBoxLayout(m_pbutton_group);
	m_pbutton_layout->setContentsMargins(0,0,0,0);
	m_pbutton_layout->setSpacing( 0 ) ; 
	m_pclose_button = new QPushButton( QIcon( ":/img/images/close_nomal.png" ) , "" , m_pbutton_group ) ;
	m_pclose_button->setObjectName( tr( "closebutton" ) ) ;
	connect( m_pclose_button , SIGNAL( clicked() ) ,  this , SLOT( slot_close_widget() ) ) ;
	m_pmin_button = new QPushButton(QIcon(":/img/images/min_nomal.png"),"", m_pbutton_group ) ;
	m_pmin_button->setObjectName( tr( "minbutton" ) ) ;
	connect( m_pmin_button , SIGNAL( clicked() ) ,  this , SLOT( slot_min_widget() ) ) ;

	m_pmax_button = new QPushButton( QIcon(":/img/images/max_nomal.png"), "", m_pbutton_group ) ;
	m_pmax_button->setObjectName( tr( "maxbutton" ) ) ;
	connect( m_pmax_button , SIGNAL( clicked() ) ,  this , SLOT( slot_max_widget() ) ) ;
	m_psetting_button = new QPushButton(  QIcon(":/img/images/setting.png"), "",  m_pbutton_group ) ;
	m_psetting_button->setObjectName( tr( "setbutton" ) ) ;
	m_pabout_button = new QPushButton(  QIcon(":/img/images/about.png"), "", m_pbutton_group ) ;
	m_pabout_button->setObjectName( tr( "aboutbutton" ) ) ;
	connect( m_pabout_button , SIGNAL( clicked() ) ,  this , SLOT( slot_about_dlg() ) ) ;
	
	m_pbutton_layout->addWidget(m_psetting_button);
	m_pbutton_layout->addWidget(m_pabout_button);
	m_pbutton_layout->addWidget(m_pmin_button);
	m_pbutton_layout->addWidget(m_pmax_button);
	m_pbutton_layout->addWidget(m_pclose_button);

	m_phbox_layout = new QHBoxLayout(this);
	m_phbox_layout->setContentsMargins(30,0,0,0);
	m_phbox_layout->addWidget(m_plogo_label);
	m_phbox_layout->addWidget(m_ptext_label);
	m_phbox_layout->addItem(m_pspace);
	m_phbox_layout->addWidget(m_pbutton_group);

	QFont font ;
	font.setFamily( tr( "微软雅黑") ) ;
	m_ptext_label->setFont( font ) ;

	QString styleSheet ;
	styleSheet = tr( "#logolabel\
					 {\
					 background-color: #2D3035;\
					 border: 0px;\
					 min-width: 50px;\
					 min-height: 24px;\
					 max-width: 50px;\
					 max-height: 24px;\
					 }") ;
	m_plogo_label->setStyleSheet( styleSheet ) ;

	styleSheet = tr( "#textlabel\
					 {\
					 font: 18px; \
					 color: rgb(255, 255, 255);\
					 border: 0px;\
					 }") ;
	m_ptext_label->setStyleSheet( styleSheet ) ;

	styleSheet = tr( "#closebutton\
					 {\
					 border: 0px;\
					 min-width: 64px;\
					 min-height: 64px;\
					 max-width: 64px;\
					 max-height: 64px;\
					 }\
					 #closebutton:hover\
					 {\
					 background-image: url(:/img/images/close_hovor_bg.png);\
					 border: 0px;\
					 }\
					 #closebutton:pressed\
					 {\
					 background-image: url(:/img/images/close_hovor_bg.png);\
					 border: 0px;\
					 }") ;
	m_pclose_button->setStyleSheet( styleSheet ) ;

	styleSheet = tr( "#maxbutton\
					 {\
					 border: 0px;\
					 min-width: 64px;\
					 min-height: 64px;\
					 max-width: 64px;\
					 max-height: 64px;\
					 }\
					 #maxbutton:hover\
					 {\
					 background-image: url(:/img/images/status_hovor.bg.png);\
					 border: 0px;\
					 }\
					 #maxbutton:pressed\
					 {\
					 background-image: url(:/img/images/status_click_bg.png);\
					 border: 0px;\
					 }") ;
// 	QIcon max_icon = QIcon(":/img/images/max_nomal.png");
	m_pmax_button->setStyleSheet( styleSheet ) ;
// 	m_pmax_button->setIcon(max_icon);
// 	m_pmax_button->setIconSize(QSize(20,20));

	styleSheet = tr( "#minbutton\
					 {\
					 border: 0px;\
					 min-width: 64px;\
					 min-height: 64px;\
					 max-width: 64px;\
					 max-height: 64px;\
					 }\
					 #minbutton:hover\
					 {\
					 background-image: url(:/img/images/status_hovor.bg.png);\
					 border: 0px;\
					 }\
					 #minbutton:pressed\
					 {\
					 background-image: url(:/img/images/status_click_bg.png);\
					 border: 0px;\
					 }") ;
	m_pmin_button->setStyleSheet( styleSheet ) ;

	styleSheet = tr( "#setbutton\
					 {\
					 border: 0px;\
					 min-width: 64px;\
					 min-height: 64px;\
					 max-width: 64px;\
					 max-height: 64px;\
					 }\
					 #setbutton:hover\
					 {\
					 background-image: url(:/img/images/status_hovor.bg.png);\
					 border: 0px;\
					 }\
					 #setbutton:pressed\
					 {\
					 background-image: url(:/img/images/status_click_bg.png);\
					 border: 0px;\
					 }" ) ;
	m_psetting_button->setStyleSheet( styleSheet ) ;
	styleSheet = tr( "#aboutbutton\
					 {\
					 border: 0px;\
					 min-width: 64px;\
					 min-height: 64px;\
					 max-width: 64px;\
					 max-height: 64px;\
					 }\
					 #aboutbutton:hover\
					 {\
					 background-image: url(:/img/images/status_hovor.bg.png);\
					 border: 0px;\
					 }\
					 #aboutbutton:pressed\
					 {\
					 background-image: url(:/img/images/status_click_bg.png);\
					 border: 0px;\
					 }" ) ;
	m_pabout_button->setStyleSheet( styleSheet ) ;
	styleSheet = tr( "#groupBox\
					 {\
					 border: 0px;\
					 }" ) ;
	m_pbutton_group->setStyleSheet( styleSheet ) ;
	styleSheet = tr( "#titleframe\
					 {\
					 background-color: rgb(45, 48, 53);\
					 min-height: 64px;\
					 max-height: 64px;\
					 }") ;
	setStyleSheet( styleSheet ) ;
}

void Title_frame::mouseDoubleClickEvent( QMouseEvent * event )
{
	slot_max_widget() ;
	QWidget::mouseDoubleClickEvent( event ) ;
}

void Title_frame::slot_close_widget( void )
{
	Event_real_wnd *pmain_wnd = (Event_real_wnd *)getMainWnd();
	if(DMessageBox::question(this, tr("提示"),tr("是否退出智能告警?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::No)
	{
		return;
	}
	pmain_wnd->close();
}

void Title_frame::slot_min_widget( void )
{
	Event_real_wnd *pmain_wnd = (Event_real_wnd *)getMainWnd();
	pmain_wnd->showMinimized();
}

void Title_frame::slot_max_widget( void )
{
	QString stylesheet;
	QIcon max_icon;

	if (!is_max)
	{
		Event_real_wnd *pmain_wnd = (Event_real_wnd *)getMainWnd();
		pmain_wnd->showMaximized();
		is_max = true;
		max_icon = QIcon(":/img/images/reduction.png");
// 		stylesheet = tr( "#maxbutton\
// 				 image: url(:/img/images/max_hovor.png);\
// 				 image: url(:/img/images/about.png);\
// 				}") ;
	}
	else
	{
		Event_real_wnd *pMainWnd = (Event_real_wnd *)getMainWnd();
		pMainWnd->showNormal();
		is_max = false;
		max_icon = QIcon(":/img/images/max_nomal.png");
// 		stylesheet = tr( "#maxbutton\
// 				 image: url(:/img/images/max_nomal.png);\
// 				}") ;
	}
	m_pmax_button->setIcon(max_icon);

// 	m_pmax_button->setStyleSheet( stylesheet ) ;
}

void Title_frame::slot_about_dlg( void )
{
	QString msg = tr( "D6000智能告警\n南京大全自动化科技有限公司\n版权所有" ) ;
	DMessageBox::about( this , tr( "关于" ) , msg ,":/img/images/realalarm1.png") ;
}

