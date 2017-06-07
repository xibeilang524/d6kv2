#include "tool_button.h"
#include "globaldef.h"


Tool_button::Tool_button( QString buttonName , QString buttonDesc , QString buttonImg /*= 0 */, QWidget* parent /*= 0 */ ) : QPushButton( parent )
{
	m_button_name = buttonName ;
	m_button_desc = buttonDesc ;
	if ( !buttonImg.isEmpty() )
	{
		m_button_img = buttonImg ; 
	}
	init_tool_button() ;
}

void Tool_button::init_tool_button( void )
{
	setObjectName( m_button_name ) ;
	setText( m_button_desc ) ;
	QFont font ;
	font.setFamily( tr( "Î¢ÈíÑÅºÚ") ) ;
	setFont( font ) ;
	QString buttonStyleSheet ;

	if ( !m_button_img.isEmpty() )
	{
		QIcon icon(m_button_img);
		setIcon(icon);
		QString labelStyleSheet ;
		labelStyleSheet = tr( "#%1\
							  {\
							  background-image: url(:/img/images/button_nomal.png);\
							  border: 0px;\
							  min-width: 120px;\
							  min-height: 41px;\
							  max-width: 120px;\
							  max-height: 41px;\
							  font-size: 16px;\
							  color: #FFFFFF;\
							  }\
							  #%1:hover\
							  {\
							  background-image: url(:/img/images/button_hovor.png);\
							  border: 0px;\
							  }\
							  #%1:checked\
							  {\
							  background-image: url(:/img/images/button_click.png);\
							  border: 0px;\
							  }\
							  #%1:pressed\
							  {\
							  background-image: url(:/img/images/button_click.png);\
							  border: 0px;\
							  }").arg( m_button_name ) ;

		setStyleSheet( labelStyleSheet ) ;
	}
	else
	{
		buttonStyleSheet = tr( "#%1\
							   {\
							   background-image: url(:/img/images/button_nomal.png);\
							   border: 0px;\
							   min-width: 120px;\
							   min-height: 41px;\
							   max-width: 120px;\
							   max-height: 41px;\
							   font-size: 18px;\
							   color: #4A90E2;\
							   }\
							   #%1:hover\
							   {\
							   background-image: url(:/img/images/button_hovor.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }\
							   #%1:pressed\
							   {\
							   background-image: url(:/img/images/button_click.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }").arg( m_button_name ) ;
		setStyleSheet( buttonStyleSheet ) ;
	}

}
