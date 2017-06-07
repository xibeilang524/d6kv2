#include "tool_button_frame.h"
#include "globaldef.h"

#include <QApplication>

extern QWidget* getMainWnd() ;
#include "event_real_wnd.h"

Tool_button_frame::Tool_button_frame( QWidget* parent /*= 0 */ ) : QFrame( parent )
{
	setObjectName( tr( "toolbuttonframe" ) ) ;
	m_bpause = false;
	init_tool_button_frame() ;
}

void Tool_button_frame::init_tool_button_frame( void )
{
	m_phorizontal_layout = new QHBoxLayout(this);
	m_pwin_set_button = new Tool_button(  tr( "winsetbutton" ),tr(" 窗口配置"),":/img/images/winset.png",this  ) ;
	m_pwin_top_button = new Tool_button(  tr( "wintopbutton" ),tr(" 窗口置顶"),":/img/images/wintop.png",this  ) ;
	m_ppause_button = new Tool_button(  tr( "pausebutton" ),tr(" 暂停刷新"),":/img/images/pause.png",this  ) ;
	m_pdelete_button = new Tool_button(  tr( "deletebutton" ),tr(" 删除事项"),":/img/images/delete.png",this  ) ;
	m_pconfirm_sound_button = new Tool_button(  tr( "confirmsndbutton" ),tr(" 语音确认"),":/img/images/confirmsnd.png",this  ) ;
	m_pconfirm_all_button = new Tool_button(  tr( "confirmallbutton" ),tr(" 确认所有"),":/img/images/confirmall.png",this  ) ;
	m_pconfirm_event_button = new Tool_button(  tr( "confirmeventbutton" ),tr(" 事项确认"),":/img/images/confirm.png",this  ) ;
	m_phorizontal_spacer = new QSpacerItem(348, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	Qt::WindowFlags flags;
	flags = windowFlags();
	bool m = (flags.testFlag(Qt::WindowStaysOnTopHint)) ? 1 : 0 ;
	// 	m_topmostBtn->setOn( testWFlags( Qt::WStyle_StaysOnTop ) );		//modify by chenkai 2013年3月6日 11:30:20
	m_pwin_top_button->setCheckable( true );
	m_pwin_top_button->setChecked( m );
	m_ppause_button->setCheckable( true );

	m_phorizontal_layout->addWidget(m_pwin_set_button);
	m_phorizontal_layout->addWidget(m_pwin_top_button);
	m_phorizontal_layout->addWidget(m_ppause_button);
	m_phorizontal_layout->addWidget(m_pdelete_button);
	m_phorizontal_layout->addWidget(m_pconfirm_sound_button);
	m_phorizontal_layout->addWidget(m_pconfirm_all_button);
	m_phorizontal_layout->addWidget(m_pconfirm_event_button);
	m_phorizontal_layout->addItem(m_phorizontal_spacer);
	QString styleSheet;
	styleSheet = tr( "#toolbuttonframe\
					 {\
					 background-color: rgb(250, 250, 250);\
					 min-height: 64px;\
					 max-height: 64px;\
					 }") ;
	setStyleSheet( styleSheet ) ;
}

void Tool_button_frame::set_pause_checked( bool pause )
{
	m_ppause_button->setChecked(pause);
}
