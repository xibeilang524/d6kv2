#include "status_bar_widget.h"
#include "festools_wnd.h"
#include <QResizeEvent>

extern QWidget* getMainWnd() ;

status_bar_widget::status_bar_widget( QWidget * parent /* = 0 */ ): QObject( parent )
{

}

void status_bar_widget::add_status_widget( QWidget* widget , int width )
{
	m_widget_list.append( widget );

	int* pwidth = new int;
	*pwidth = width ;

	m_width_list.append( pwidth );
}

bool status_bar_widget::eventFilter(QObject *o, QEvent * e)
{
	if(e->type() == QEvent::Resize)
	{
		festools_wnd* pMainWnd = ( festools_wnd* )getMainWnd() ;
		int w = ( ( QResizeEvent* ) e )->size().width() ;
		int h = ( ( QResizeEvent* ) e )->size().height() ;

		if( pMainWnd->isMaximized() )
		{
			pMainWnd->statusBar()->setSizeGripEnabled( false ) ;
		}
		else
		{
			pMainWnd->statusBar()->setSizeGripEnabled( true ) ;
			w -= 16 ;
		}

		for ( int i = 0 ; i < m_widget_list.size() ; i++)
		{
			int width = *m_width_list[ i ] ;
			m_widget_list[i]->setGeometry( w -2 - width , 2 , width , h - 4 ) ;

			w = w - 2 - width ;
		}
	}

	return false ;
}