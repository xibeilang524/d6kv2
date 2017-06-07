#include "msg_data_widget.h"
#include "festools_globaldef.h"
#include "uitools/dmessagebox.h"

#include <QResizeEvent>
#include <QFileDialog>
#include <QMessageBox>

msg_data_widget::msg_data_widget( QWidget* parent /*= NULL */ ) : QFrame( parent ) , m_fes_shm( Fes_shm::get() )
{
	ini_msg_data_widget() ;
	m_pModular = new msg_modular ;
	m_pTimer = new QTimer();
	connect( m_pTimer , SIGNAL( timeout() ) , this , SLOT( on_timer() ) ) ;
	m_pTimer->start(150); 
	m_pTcp_recv = NULL;
}

void msg_data_widget::ini_msg_data_widget()
{
	QString dir;
	m_pMsg_data_view = new msg_data_view( this ) ;
	m_pTop_tool_bar = new QToolBar( this ) ;
	m_pTime_tool_bar = new QToolBar( this ) ;
	m_pTop_tool_bar->setMovable( false ) ;
	m_pTime_tool_bar->setMovable( false ) ;

	m_pPause_action = new QAction( QIcon( ":res/player_pause.png" ) , tr( "暂停" ) , this ) ;
	m_pContinue_action = new QAction( QIcon( ":res/player_play.png" ) , tr( "继续" ) , this ) ;
	m_pClean_action = new QAction( QIcon( ":res/clear.png" ), tr( "清屏" ) , this ) ;
	m_pSave_action = new QAction( QIcon( ":res/filesave.png" ), tr( "保存" ) , this ) ;
	m_pClear_search_action = new QAction( QIcon( ":res/clear_search.png" ) , tr( "清除筛选框" ) , this ) ;
	m_pSearch_label = new QLabel( tr( "查找" ) ) ;
	m_pBegintime_label = new QLabel( tr( "起始时间 " ) ) ;
	m_pEndtime_label = new QLabel( tr( "结束时间 " ) ) ;
	m_pSearch_lineedit = new QLineEdit();
	m_pValidator = new QRegExpValidator( QRegExp("([0-9A-F]{2}\\ {0,9}){1,10}") , this ) ;

	m_pTop_tool_bar->addAction( m_pPause_action ) ;
	m_pTop_tool_bar->addAction( m_pContinue_action ) ;
// 	m_pTop_tool_bar->addSeparator();
	m_pTop_tool_bar->addAction( m_pClean_action ) ;
// 	m_pTop_tool_bar->addSeparator() ;
	m_pTop_tool_bar->addAction( m_pSave_action ) ;
// 	m_pTop_tool_bar->addSeparator() ;
	m_pTop_tool_bar->addWidget( m_pSearch_label ) ;
	m_pTop_tool_bar->addWidget( m_pSearch_lineedit ) ;
	m_pTop_tool_bar->addAction( m_pClear_search_action ) ;
// 	m_pTop_tool_bar->addSeparator();

	m_pAnalysis_msg_cbk = new QCheckBox( tr( "是否解析报文" ) , this ) ;
	m_pAnalysis_msg_cbk->setCheckable( true ) ; 
	m_pAnalysis_msg_cbk->setChecked( false ) ; 
	m_pAnalysis_msg_cbk->hide() ;

	m_pBegintime_edit = new QDateTimeEdit( QDateTime::currentDateTime(), this ) ;
	m_pEndtime_edit = new QDateTimeEdit( QDateTime::currentDateTime(), this ) ;
	m_pBegintime_edit->setDisplayFormat( _C2Q("yyyy-MM-dd hh:mm:ss:zzz" ) ) ;
	m_pEndtime_edit->setDisplayFormat( _C2Q("yyyy-MM-dd hh:mm:ss:zzz" ) ) ;
// 	m_pTime_tool_bar->addSeparator();
	m_pTime_tool_bar->addWidget( m_pBegintime_label ) ;
	m_pTime_tool_bar->addWidget( m_pBegintime_edit ) ;
	m_pTime_tool_bar->addWidget( m_pEndtime_label ) ;
	m_pTime_tool_bar->addWidget( m_pEndtime_edit ) ;
// 	m_pTime_tool_bar->addSeparator();
	m_pAutosave_msg_cbk = new QCheckBox( tr("自动存储报文") , this ) ;
	m_pAutosave_msg_cbk->setToolTip( tr( "报文存储在d6000/message文件夹下" ) ) ;
	m_pAutosave_msg_cbk->setCheckable( true ) ; 
	m_pAutosave_msg_cbk->setChecked( false ) ;
	m_pTime_tool_bar->addWidget( m_pAutosave_msg_cbk ) ;

	m_pShow_raw_widget= new QCheckBox( tr("显示该通道对应生数据") , this ) ;
	m_pShow_raw_widget->setCheckable( true ) ; 
	m_pShow_raw_widget->setChecked( false ) ;
	m_pTime_tool_bar->addWidget( m_pShow_raw_widget ) ;

	connect( m_pAnalysis_msg_cbk , SIGNAL( toggled( bool ) ) , this , SLOT( on_analysis_msg_cbk() ) ) ;
	connect( m_pPause_action , SIGNAL( triggered() ) , this, SLOT( on_pause() ) ) ;
	connect( m_pContinue_action , SIGNAL( triggered() ) , this, SLOT( on_continue() ) ) ;
	connect( m_pClean_action , SIGNAL( triggered() ) , this, SLOT( on_clean() ) ) ;
	connect( m_pSave_action , SIGNAL( triggered() ) , this, SLOT( on_save() ) ) ;
	connect( m_pAutosave_msg_cbk , SIGNAL( toggled(bool) ) , this, SLOT( on_auto_save() ) ) ; //自动存储
	connect( m_pShow_raw_widget , SIGNAL( toggled(bool) ) , this, SIGNAL( signal_show_raw_table(bool) ) ) ; //自动存储
	connect( m_pClear_search_action , SIGNAL( triggered() ) , this, SLOT( on_clear_search() ) ) ;
	connect( m_pSearch_lineedit, SIGNAL( textEdited( const QString &) ) , this, SLOT( on_search_text_changed() ) ) ;
	connect( m_pBegintime_edit , SIGNAL( dateTimeChanged( const QDateTime & ) ) , this , SLOT( on_set_time() ) ) ;
	connect( m_pEndtime_edit , SIGNAL( dateTimeChanged( const QDateTime & ) ) , this , SLOT( set_time() ) ) ;

	update_tool_bar();
}


void msg_data_widget::on_channel_change( int channel_no )
{
	m_pShow_raw_widget->setCheckState( Qt::Unchecked ) ;
	m_current_channel = channel_no;
	m_pModular->clear_list_data();

	CHANPARA* pChan_para = m_fes_shm.chan_para_ptr + channel_no ;
	m_pModular->set_channel_name( tr( pChan_para->desdribe ) ) ;
	on_current_modular_change( m_pModular ) ;

	if( request_fep_msg() < 0 )
	{
		ACE_ERROR((LM_ERROR, "请求通道，无值班机值班！\n"));
	}
}

void msg_data_widget::on_analysis_msg_cbk()
{
	if( m_pAnalysis_msg_cbk->isChecked() )
	{
		m_pMsg_data_view->m_analysis_msg_state = 1;
	}
	else
	{
		m_pMsg_data_view->m_analysis_msg_state = 0 ;
	}
}

void msg_data_widget::update_tool_bar()
{
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		if( m_pMsg_data_view->m_pCurrent_msg_modular->get_pause() )
		{
			m_pPause_action->setEnabled( false ) ;
			m_pContinue_action->setEnabled( true ) ;
		}
		else
		{
			m_pPause_action->setEnabled( true ) ;
			m_pContinue_action->setEnabled( false ) ;
		}

		if ( m_pMsg_data_view->m_analysis_msg_state == 0) 
		{
			m_pClean_action->setEnabled( true ) ;
			m_pSave_action->setEnabled( true ) ;
			m_pSearch_lineedit->setEnabled( true ) ;
		}
		if ( m_pMsg_data_view->m_analysis_msg_state == 1)
		{
			m_pClean_action->setEnabled( false ) ;
			m_pSave_action->setEnabled( false ) ;
			m_pSearch_lineedit->setEnabled( false ) ;
		}
	}
	else
	{
		m_pPause_action->setEnabled( false ) ;
		m_pContinue_action->setEnabled( false ) ;
		m_pClean_action->setEnabled( false ) ;
		m_pSave_action->setEnabled( false ) ;
	}
}

void msg_data_widget::on_pause()
{
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		m_pMsg_data_view->m_pCurrent_msg_modular->set_pause( true ) ;
		update_tool_bar() ;
	}	
}

void msg_data_widget::on_continue()
{
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		m_pMsg_data_view->m_pCurrent_msg_modular->set_pause( false ) ;
		update_tool_bar();
	}
}

void msg_data_widget::on_clean()
{
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		m_pMsg_data_view->m_pCurrent_msg_modular->clear_list_data() ;
	}
	m_pMsg_data_view->update_scroll() ;
	m_pMsg_data_view->viewport()->update() ;
}

void msg_data_widget::on_save()
{
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		QString fileName = QFileDialog::getSaveFileName( this , _C2Q( "另存为" ) , "message", \
			tr("Text Files (*.txt);;Text Files (*.txt)") ) ;
		if ( !fileName.isEmpty() ) 
		{
			QFile file( fileName ) ;
			if ( !file.open(QFile::Truncate | QFile::WriteOnly | QFile::Text)) 
			{
				DMessageBox::information( this , tr("保存报文"), tr("保存文件 ") + fileName + tr(" 失败!" ) ) ;
				return ;
			}

			if( !m_pMsg_data_view->m_pCurrent_msg_modular->m_List_data.isEmpty() )
			{
				QTextStream out( &file ) ;
				out << tr("通道: ");
				out << m_pMsg_data_view->m_pCurrent_msg_modular->get_channel_name() ; //获取通道名称
				out << QString(" ");
				out << tr( "保存时间: " ) ;
				QDateTime datetime = QDateTime::currentDateTime() ;
				out << datetime.toString("yyyy-MM-dd hh:mm:ss (zzz 'ms')");
				out << QString("\n");

				QListIterator<msg_modular_data*> saveIterator( m_pMsg_data_view->m_pCurrent_msg_modular->m_List_data ) ;
				saveIterator.toFront() ;

				while( saveIterator.hasNext() )
				{
					msg_modular_data* pModula_data = saveIterator.next();

					if( pModula_data )
					{
						save_modular_data( pModula_data , out, pModula_data->get_time_str() ) ; //存储报文
					}
				}
			}

			file.close();
			DMessageBox::information( this , tr("保存报文"), tr("保存文件 ") + fileName + tr(" 成功!" ) ) ;
		}
	}
}

void msg_data_widget::save_modular_data( msg_modular_data* pModula_data , QTextStream &output ,QString strtime )
{
	if( !pModula_data )
		return;

	QString strTitle ;

	switch( pModula_data->get_type() )
	{
	case 1:
		strTitle.sprintf("RECV(%d)", pModula_data->get_data_length());
		strTitle += _C2Q(" 当前时间:");
		strTitle += strtime;
		strTitle += _C2Q("\n");
		break;	
	case 2:
		strTitle.sprintf("SEND(%d)", pModula_data->get_data_length());
		strTitle += _C2Q(" 当前时间:");
		strTitle += strtime;
		strTitle += _C2Q("\n");
		break;
	default:
		strTitle.sprintf("UNKNOW(%d)\n", pModula_data->get_data_length());
		break;
	}

	if (1 == pModula_data->get_type() || 2 == pModula_data->get_type())
	{
		output << strTitle;
	}
	
	
	if (1 == pModula_data->get_type() || 2 == pModula_data->get_type())
	{
		QString strTemp;
		int nCount = 0;
		
		while(nCount < pModula_data->get_data_length())
		{
			for(int i = 0; i < 16; i++)
			{
				if(nCount + i < pModula_data->get_data_length())
				{
					strTemp.sprintf("%02X ", (unsigned char)(*(pModula_data->get_data()+nCount+i)));
					output << strTemp;
				}
			}
			output << QString(" ");
			output << QString("\n");
			nCount += 16;
		}
	}
}

void msg_data_widget::on_auto_save()
{
	QDateTime auto_save_begintime = m_pBegintime_edit->dateTime() ; //起始时间
	QDateTime auto_save_endtime = m_pEndtime_edit->dateTime() ;

	if ( m_pAutosave_msg_cbk->isChecked() ) //存储时间进行传递
	{
		m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_begintime( auto_save_begintime ) ;
		m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_endtime( auto_save_endtime ) ;
		m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_checkbox_state( 1 ) ;

	}
	else
	{
		m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_checkbox_state( 0 ) ;
	}
}

void msg_data_widget::on_clear_search()
{
	m_pSearch_lineedit->clear() ;
	m_pMsg_data_view->m_keyword_list.clear() ;
}

void msg_data_widget::on_search_text_changed()
{
	m_pMsg_data_view->m_keyword_list.clear()  ;
	QString search_str = m_pSearch_lineedit->text().toUpper()  ;
	QString temp ;
	int pos = 0 ;
	if (  m_pValidator->validate( search_str , pos ) != QValidator::Acceptable )
	{
		printf( "\nstring unaccept!" ) ;
		return ;
	}

	pos = search_str.indexOf( _C2Q( " " ) ) ;

	while ( pos != -1 )
	{
		search_str.remove( pos , 1 ) ;
		pos = search_str.indexOf( _C2Q( " " ) ) ;
	}

	QByteArray ba = search_str.toAscii() ;
	char* str = ba.data() ;
	printf( "\n%s" ,str ) ;

	if ( search_str.length() < 2 )
	{
		return ;
	}

	while ( search_str.length() >= 2 )
	{
		bool ok ;
		unsigned char tmp = search_str.mid( 0 ,2 ).toInt( &ok , 16 ) ;
		if ( true == ok )
		{
			printf( "\nthe key word is %02X" ,tmp ) ;
			m_pMsg_data_view->m_keyword_list.push_back( tmp ) ;
			search_str = search_str.remove( 0 ,2 ) ;
		}
	}
}

void msg_data_widget::on_set_time()
{
	QDateTime auto_save_begintime = m_pBegintime_edit->dateTime();
	QDateTime auto_save_endtime = m_pEndtime_edit->dateTime();

	m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_begintime( auto_save_begintime ) ;
	m_pMsg_data_view->m_pCurrent_msg_modular->set_autosave_endtime( auto_save_endtime ) ;
}

void msg_data_widget::on_current_modular_change( msg_modular* pModula )
{
	if ( NULL == pModula )
	{
		return;
	}
	m_pMsg_data_view->m_pCurrent_msg_modular = pModula ;
	update_tool_bar();
	m_pMsg_data_view->update_scroll() ;
	m_pMsg_data_view->viewport()->update() ;
}

void msg_data_widget::resizeEvent( QResizeEvent *re )
{
	m_pTop_tool_bar->setGeometry( 0 , 1 , re->size().width() ,  m_pTop_tool_bar->size().height() ) ;
	m_pTime_tool_bar->setGeometry( 0 , m_pTop_tool_bar->size().height() + 2 , re->size().width() ,   m_pTime_tool_bar->size().height() ) ;
	m_pMsg_data_view->setGeometry( 0 , m_pTop_tool_bar->size().height() + m_pTime_tool_bar->size().height() + 4 ,  re->size().width() , \
		re->size().height() -  m_pTop_tool_bar->size().height() -2 - m_pTime_tool_bar->size().height(  ) - 2);
}

void msg_data_widget::showEvent( QShowEvent * )
{
	m_pMsg_data_view->viewport()->update() ;
}

int msg_data_widget::request_fep_msg()
{
	unsigned char buf[ 10 ] ;		//向报文服务请求通道号，地位在前，高位在后
	int nodes[ MAX_FE_SRV_NUM ] ;
	struct in_addr fep_addr[ 2 ] ;
	int ret = -1 ;

	buf[ 0 ] = m_current_channel%256 ;
	buf[ 1 ] = m_current_channel/256 ;

	int node_num = Net_stat::get_fe_nodes( nodes , MAX_FE_SRV_NUM ) ;

	for ( int i = 0 ; i< node_num ; i++ )
	{
		Net_stat::get_host_addr( nodes[ i ] , fep_addr ) ;
		if( Net_stat::get_host_stat( nodes[ i ] ) != 0 )	//若不在线 ,不进行轮询
		{
			continue ;
		}
		if ( m_pTcp_recv )
		{
			m_pTcp_recv->close_dev() ;
			delete m_pTcp_recv ;
			m_pTcp_recv = NULL ;
		}

		char ip_addr0[ DEV_NAME_LEN ] ;
		ACE_OS::strcpy( ip_addr0 , inet_ntoa( fep_addr[ 0 ] ) ) ;
		printf( "ip = %s \n" ,ip_addr0 ) ;
		char ip_addr1[ DEV_NAME_LEN ] ;
		ACE_OS::strcpy( ip_addr1 , inet_ntoa( fep_addr[ 1 ] ) ) ;
		printf( "ip = %s \n" ,ip_addr1 ) ;
		m_pTcp_recv = new Tcp_client( ip_addr0 , PORT_NO , ip_addr1 , PORT_NO ) ;

		ret = m_pTcp_recv->open_dev() ;
		if ( ret < 0 || m_pTcp_recv->get_stat() < 0 )
		{
			m_pTcp_recv->close_dev() ;
			delete m_pTcp_recv ;
			m_pTcp_recv = NULL ;
			continue ;
		}

		m_pTcp_recv->write_dev( buf , 2 ) ;
		while ( m_pTcp_recv->read_dev( buf ,2 ) == 0 )
		{
			continue ;
		}

		if( buf[ 0 ] == 0xcc &&buf[ 1 ] == 0xcc )
		{
			ACE_DEBUG( ( LM_DEBUG , "请求主机host_no = %d！\n" ,nodes[ i ] ) ) ;
			ret = 0 ;
			break ;
		}
	}
	return ret ;
}

void msg_data_widget::get_channel_msg()
{
	if ( -1 == m_current_channel || m_pTcp_recv == NULL ) 
	{
		return ;
	}

	FE_MSG *msg_buf ;
	unsigned char buf[ sizeof( FE_MSG ) ] ;
	memset( buf , 0 , sizeof( FE_MSG ) ) ;

	int ret = m_pTcp_recv->read_dev( buf ,sizeof( FE_MSG ) ) ;
	if ( ret <= 0 )		//接收出错，或无报文
	{
		return ;
	}

	msg_buf = ( FE_MSG* )buf ;
	if ( msg_buf->channel_no != m_current_channel || msg_buf->lenth == 0 )
	{
		return ;
	}
	int type = msg_buf->recv_flag ? 1 : 2 ;
	CHANPARA* pChan_para = m_fes_shm.chan_para_ptr + msg_buf->channel_no ;
	QString protocol_str = _C2Q( pChan_para->protocol ) ;

	m_pModular->add_data( ( char* )msg_buf->msg , msg_buf->lenth , type , protocol_str ,
		m_pModular->get_autosave_begintime() , m_pModular->get_autosave_endtime() ,
		m_pModular->get_autosave_checkbox_state() , m_pModular->get_channel_name() , &( msg_buf->time));
}

void msg_data_widget::on_timer()
{
	if ( m_pTcp_recv )
	{
		get_channel_msg() ;
	}
	if( m_pMsg_data_view->m_pCurrent_msg_modular )
	{
		if( m_pMsg_data_view->m_pCurrent_msg_modular->get_pause() )
		{
			return;
		}
		m_pMsg_data_view->update_scroll() ;
		m_pMsg_data_view->viewport()->update() ;
	}
}

void msg_data_widget::stop_get_msg( bool flag )
{
	if ( flag ==  true )
	{
		m_pTimer->stop() ;
	}
	else
	{
		m_pTimer->start( 150 ) ;
	}
}
