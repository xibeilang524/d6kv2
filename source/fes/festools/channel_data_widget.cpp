#include "channel_data_widget.h"
#include "fes/fesdatashm_accessor.h"

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QHeaderView>

channel_data_widget::channel_data_widget( QWidget* parent /*= NULL */ ) : QFrame( parent ) , m_fes_shm( Fes_shm::get() )
{
	ini_channel_data_widget() ;
	setFrameStyle( QFrame::Panel | QFrame::Sunken ) ;

	m_terminal_no = -1 ;
	m_channel_no = -1 ;

	m_pTimer = new QTimer();
	m_pTimer->start( 1000 ) ;
	connect( m_pTimer , SIGNAL( timeout() ) , this , SLOT( on_timer() ) ) ;
}

void channel_data_widget::ini_channel_data_widget()
{
	m_pChannel_detail_widget = new channel_detail_widget( this ) ;
	m_pMsg_widget = new msg_data_widget( this ) ;

	QVBoxLayout* pVerticalLayout = new QVBoxLayout( this );
	pVerticalLayout->addWidget( m_pChannel_detail_widget ) ;
	pVerticalLayout->addWidget( m_pMsg_widget ) ;
	pVerticalLayout->setSpacing(0);
	setLayout( pVerticalLayout ) ;
}

void channel_data_widget::hide_msg_widget()
{
	m_pChannel_detail_widget->show() ;
	m_pTimer->start( 1000 ) ;
	m_pMsg_widget->hide() ;
	m_pMsg_widget->stop_get_msg( true ) ;
}

void channel_data_widget::hide_channel_table()
{
	m_pChannel_detail_widget->hide() ;
	m_pTimer->stop() ;
	m_pMsg_widget->show() ;
	m_pMsg_widget->stop_get_msg( false ) ;
}

void channel_data_widget::hide_all_widgets()
{
	m_pChannel_detail_widget->hide() ;
	m_pTimer->start( 1000 ) ;
	m_pMsg_widget->hide() ;
	m_pMsg_widget->stop_get_msg( true ) ;
}

void channel_data_widget::change_channel_no( int channel_no )
{
	m_channel_no = channel_no ;
	m_pMsg_widget->on_channel_change( channel_no ) ;
}

void channel_data_widget::change_termianl_no( int terminal_no )
{
	m_terminal_no = terminal_no ;
}

void channel_data_widget::on_timer()
{
	m_pChannel_detail_widget->refresh_channel_table() ;
}
