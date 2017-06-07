#include "msg_data_view.h"
#include "msg_modular_data.h"

#include "festools_globaldef.h"

#include <QPainter>
#include <QScrollBar>
#include <QPaintEvent>

msg_data_view::msg_data_view( QWidget* parent /*= NULL */ ) : QAbstractScrollArea( parent )
{
	m_pCurrent_msg_modular = NULL ;
	m_char = 0 ;
	m_total_line = 0 ;
	m_desc_char = 0 ;
	m_current_line = 0 ;
	m_scrollview_width = 0 ;
	m_analysis_msg_state = 0;
	QFontMetrics fm = fontMetrics() ;
	m_char_width = fm.averageCharWidth() ;   //平均字体宽度
	m_line_height = fm.lineSpacing() * 1.2 ;   //行距
	setUpdatesEnabled( true ) ;
	viewport()->setBackgroundRole( QPalette::Dark ) ;   //设置标题栏颜色
	viewport()->setAutoFillBackground( true ) ;
}

void msg_data_view::paintEvent( QPaintEvent* pe )
{
	QPainter painter ;
	painter.begin( viewport() ) ;
	painter.setPen( Qt::black ) ;
	painter.fillRect( pe->rect() , QColor( 214,214,214 ) ) ;

	m_current_line = 0 ;

	if ( NULL == m_pCurrent_msg_modular )
	{
		return ;
	}

	if ( m_pCurrent_msg_modular->m_List_data.isEmpty() )
	{
		return ;
	}

	QListIterator<msg_modular_data*> showIterator( m_pCurrent_msg_modular->m_List_data ) ;
	showIterator.toFront() ;

	while( showIterator.hasNext() )
	{

		msg_modular_data* pModula_data = showIterator.next() ;
		if( NULL != pModula_data )
		{		
			draw_modular_data( pModula_data , &painter ) ;
		}
	}

	painter.end() ;
}

void msg_data_view::draw_modular_data( msg_modular_data* pModula_data, QPainter* pPainter )
{
	if ( ( pModula_data == NULL ) || ( pPainter == NULL ) || ( m_line_height == 0 ) || ( m_char == 0 ) || ( m_desc_char == 0 ) )
	{
		return ;
	}

	int scroll_bar_value = verticalScrollBar()->value() ;  //滑块当前值
	int start_line = scroll_bar_value/m_line_height ;   //0-30行一页
 	int end_line = ( ( scroll_bar_value + viewport()->height() )/m_line_height ) - 1 ; //434/14 -1

	//行数计算
	if ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() )
	{
		if( ( ( ( m_current_line + ( pModula_data->get_data_length() + m_char - 1 ) / m_char + 1 ) < start_line ) 
			|| ( m_current_line > end_line ) ) && m_analysis_msg_state == 0 )
		{
			m_current_line += ( ( pModula_data->get_data_length() + m_char - 1 ) / m_char + 1 ) ;  //当前行数重置
			return ;
		}

		if ( m_analysis_msg_state == 1 )  //未完
		{
			 ;
		}
	}

	QString strTitle ;  //收发报文头SEND&RECV
	QPen pen ;
	QFont font ;
	font.setBold( false ) ; //报文头加粗
	font.setPixelSize( 14 ) ;
	font.setFamily( "SimHei" ) ;

	QString mexchange = pModula_data->get_exchange_data() ;
	int m_Strlength = pModula_data->get_exchange_data().length() ; //U帧长度52,对时长度199

	int i = 0;
	int j = 0;
	int nCount = 0 ;
	QString strTemp ;
	QString strtime ;
	
	//加上发送接收头
	switch( pModula_data->get_type() )
	{
	case 1:
		pPainter->setFont( font ) ;
//		pPainter->setPen( Qt::red ) ;
		pPainter->setPen( Qt::darkGreen ) ;
		pPainter->setBackground( Qt::green ) ;
		strTitle.sprintf( "RECV( %d )", pModula_data->get_data_length() ) ;
		strTitle += _C2Q( "    当前时间:" ) ;
		strtime = pModula_data->get_time_str() ;
		strTitle += strtime ;
		break ;
	case 2:
		pPainter->setFont( font ) ;
 		pPainter->setPen( Qt::blue ) ;
		strTitle.sprintf( "SEND( %d )", pModula_data->get_data_length() ) ;
		strTitle += _C2Q( "    当前时间:" ) ;
		strtime = pModula_data->get_time_str() ;
		strTitle += strtime ;
		break ;
	default:
		strTitle.sprintf( "UNKNOW( %d )", pModula_data->get_data_length() ) ;
		break ;
	}

	int str_count = m_Strlength / 80 ;  //QString分的行数，用于drawtext显示

	if ( pModula_data->get_protocol_type() == "iec104" )
	{	
		if ( ( m_Strlength % 80 ) > 0 )
		{
			str_count ++ ;
		}
		//80字符一行显示
		for ( int i = 0 ;i < str_count ; i++ )
		{
			mexchange.insert( 80*( i+1 )+j, "," ) ;
			j++ ;
		}
		//分割字符串
		QStringList str_list = mexchange.split( "," ) ;

		//断行处理
		for ( int i = 1 ; i < str_count ; i++ )
		{

			//mid()和remove()从0开始
			if ( str_list[i].mid( 0,1 ) == "0" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "0" ;
				str_list[i].remove( 0,1 ) ; 
			}

			if ( str_list[i].mid( 0,1 ) == "1" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "1" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "2" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "2" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "3" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "3" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "4" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "4" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "5" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "5" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "6" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "6" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "7" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "7" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "8" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "8" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "9" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "9" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "A" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "A" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "B" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "B" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "C" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "C" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "D" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "D" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "E" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "E" ;
				str_list[i].remove( 0,1 ) ;
			}

			if ( str_list[i].mid( 0,1 ) == "F" && ( str_list[i].mid( 1,1 ) == " " || str_list[i].mid( 1,1 ) == "( " ) )
			{
				str_list[i-1] += "F" ;
				str_list[i].remove( 0,1 ) ;
			}

		}

		//解析报文
		if ( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && m_analysis_msg_state == 1 )
		{

			//显示报文
			if ( ( m_current_line >= start_line ) && ( m_current_line < end_line ) )
			{		
				//画报文头
				pPainter->drawText( 0, m_line_height*( m_current_line-start_line+1 ), strTitle ) ;
			}
			m_current_line++ ;

			font.setBold( false ) ;
			font.setPixelSize( 14 ) ;
			pPainter->setFont( font ) ;
			pPainter->setPen( Qt::black ) ;

			//画报文
			for( i = 0 ; i < str_count ; i++ )
			{
				pPainter->drawText( 0, m_line_height*( m_current_line-start_line+1 ), str_list[i] ) ;
				m_current_line++ ;
			}
		}
	}

	if ( pModula_data->get_protocol_type() == "CDT" )
	{
		//分割字符串
		QStringList str_list = mexchange.split( "*" ) ;

		if ( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && m_analysis_msg_state == 1 )
		{
			//显示报文
			if ( ( m_current_line >= start_line ) && ( m_current_line < end_line ) )
			{		
				//画报文头
				pPainter->drawText( 0, m_line_height*( m_current_line-start_line+1 ), strTitle ) ;
			}
			m_current_line++ ;

			font.setBold( false ) ;
			font.setPixelSize( 14 ) ;
			pPainter->setFont( font ) ;
			pPainter->setPen( Qt::black ) ;

			//画报文
			for( i = 0 ; i < pModula_data->get_cdt_frame_cnt() + 1 ; i++ )
			{
				pPainter->drawText( 0, m_line_height*( m_current_line-start_line+1 ), str_list[i] ) ;
				m_current_line++ ;
			}
		}
	}

	if ( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && m_analysis_msg_state ==0 )
	{
			//画出报文头
			if ( ( m_current_line >= start_line ) && ( m_current_line < end_line ) )
			{
				pPainter->drawText( 0, m_line_height*( m_current_line-start_line+1 ), strTitle ) ;
			}
				m_current_line++ ;

				font.setBold( false ) ;
				pPainter->setFont( font ) ;
				pPainter->setPen( Qt::black ) ;

			while( nCount < pModula_data->get_data_length() )
		  {
			for( i = 0 ; i < m_char ; i++ )
			{
				//循环打印报文
				if( nCount + i < pModula_data->get_data_length() )
				{
					//从msg_modular_data里取数据
					unsigned char msg = ( unsigned char )( *( pModula_data->get_data()+nCount+i ) ) ;
					strTemp.sprintf( "%02X", msg ) ; //打印出字符

					if( ( m_current_line >= start_line ) && ( m_current_line < end_line ) )
					{
						//搜索高亮字符
						if ( match_keyword( msg ) )
						{
							pPainter->setPen( QColor( "#FF1493" ) ) ;
							pPainter->drawText( ( 1+i*3 )*m_char_width, m_line_height*( m_current_line-start_line+1 ), strTemp ) ;
							pPainter->setPen( Qt::black ) ;
						}
						else
						{
							//普通显示
							pPainter->drawText( ( 1+i*3 )*m_char_width, m_line_height*( m_current_line-start_line+1 ), strTemp ) ;
						}		
					}
				}
			}
			m_current_line++ ;
			nCount += m_char ;
		}
	}
}

bool msg_data_view::match_keyword( unsigned char msg )
{
	if ( m_keyword_list.isEmpty() )
	{
		return false ;
	}

	QListIterator<unsigned char> findIterator( m_keyword_list ) ;
	findIterator.toFront() ;
	while( findIterator.hasNext() )
	{
		unsigned char key = findIterator.next() ;

		if ( key == msg )
		{
			return true ;
		}
	}

	return false ;
}

void msg_data_view::resizeEvent( QResizeEvent *e )
{
	if( m_char_width == 0 )
		return ;

	m_char = ( e->size().width() / m_char_width ) / 3 ; 
	m_scrollview_width = e->size().width() ;  //显示窗口宽度

	if( m_char < 0 )
		m_char = 0 ;
	m_desc_char = ( e->size().width() / m_char_width ) - 1 ;
	if( m_desc_char < 0 )
		m_desc_char = 0 ;
	m_total_line = calc_total_line() ;
	if( m_total_line == 0 )
		return ;
	verticalScrollBar()->setPageStep( viewport()->height() ) ;
	verticalScrollBar()->setRange( 0, qMax( 0, ( m_total_line+1 )*m_line_height - viewport()->height() ) ) ;
	QAbstractScrollArea::resizeEvent( e ) ;
}

int msg_data_view::calc_total_line()
{
	if( ( m_char == 0 ) || ( m_desc_char == 0 ) )
	{
		return 0 ;
	}

	if( NULL == m_pCurrent_msg_modular )
	{
		return 0 ;
	}

	int iTotalLine = 0 ;

	QListIterator<msg_modular_data*> calcIterator( m_pCurrent_msg_modular->m_List_data ) ; //存储的报文，从一开始到现在
	calcIterator.toFront() ;


	while( calcIterator.hasNext() )
	{
		msg_modular_data* pModula_data = calcIterator.next() ;

		if( pModula_data )
		{
			//原来情况，不解析104和CDT报文时正常计算行数			
			if( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && ( m_analysis_msg_state == 0 ) )
			{
				//一条报文占3行，一个LIST里的元素*3=iTotalLine总行数
				iTotalLine += ( pModula_data->get_data_length() + m_char - 1 ) / m_char + 1 ;
			}


			//解析CDT报文
			if( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && ( m_analysis_msg_state == 1 ) && ( pModula_data->get_protocol_type() == "CDT" ) )
			{
				iTotalLine += ( pModula_data->get_data_length() + m_char - 1 ) / m_char + 1 ;
			}

			//解析104报文
			if( ( 1 == pModula_data->get_type() || 2 == pModula_data->get_type() ) && ( m_analysis_msg_state == 1 ) && ( pModula_data->get_protocol_type() == "iec104" ) )
			{
				iTotalLine += ( pModula_data->get_data_length() + m_char - 1 ) / m_char + 1 ;
			}
		}
	}
	return iTotalLine + 1 ;
}

void msg_data_view::update_scroll()
{
	m_total_line = calc_total_line() ;
	if( m_total_line == 0 )
		return ;
	verticalScrollBar()->setPageStep( viewport()->height() ) ;  //使用pageup和pagedown来翻页的幅度
	verticalScrollBar()->setRange( 0, qMax( 0, ( m_total_line+1 )*m_line_height - viewport()->height() ) ) ; //设置滚动范围
	verticalScrollBar()->setSingleStep( m_line_height ) ;  //设置鼠标箭头滚动步长
	verticalScrollBar()->setValue( ( m_total_line+1 )*m_line_height - viewport()->height() ) ; //单击一次滚动条滚动的幅度
}

void msg_data_view::set_up_down( int up_down )
{
	m_up_down = up_down ;
}