#include "QMyToolButton.h"
#include "globaldef.h"

QMyToolButton::QMyToolButton( QString buttonName , QString buttonDesc , QString buttonImg /*= 0 */, QWidget* parent /*= 0 */ ) : QPushButton( parent )
{
	m_buttonName = buttonName ;
	m_buttonDesc = buttonDesc ;
	m_pixLabelName = buttonName + tr( "_label" ) ;
	m_pImgBtn = NULL ;
	if ( !buttonImg.isEmpty() )
	{
		QString baseButtomImg = buttonImg ; 
// 		m_buttonImgHover = baseButtomImg.insert( baseButtomImg.indexOf( "." ) , BUTTON_STATE_HOVER ) ;
// 		baseButtomImg = buttonImg ; 
// 		m_buttonImgPress = baseButtomImg.insert( baseButtomImg.indexOf( "." ) , BUTTON_STATE_PRESSED ) ;
// 		baseButtomImg = buttonImg ; 
// 		m_buttonImgNormal = baseButtomImg.insert( baseButtomImg.indexOf( "." ) , BUTTON_STATE_NORMAL ) ;
	}
	iniMyToolButton() ;
}

void QMyToolButton::iniMyToolButton( void )
{
	setObjectName( m_buttonName ) ;
	setText( m_buttonDesc ) ;
	char dqFileName[ 256 ] ;
	sprintf( dqFileName , "%s" , getenv( "NBENV" ) ) ;
	QString fileNameStr = tr( dqFileName ).replace( '\\' , '/' ) ;
	QFont font ;
	font.setFamily( tr( "Æ¼·½-¼ò") ) ;
	setFont( font ) ;
	QString buttonStyleSheet ;

	if ( !m_buttonImgHover.isEmpty() )
	{
		m_pImgBtn = new QPushButton( this ) ;
// 		m_pImgBtn->move( CHILDBUTTON_LEFT , CHILDBUTTON_TOP ) ;
		m_pImgBtn->setObjectName( m_pixLabelName ) ;
		QString labelStyleSheet ;
		labelStyleSheet = tr( "#%1\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%2);\
							  border: 0px;\
							  min-width: 24px;\
							  min-height: 24px;\
							  max-width: 24px;\
							  max-height: 24px;\
							  }\
							  #%3:hover\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%4);\
							  border: 0px;\
							  }\
							  #%5:checked\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%6);\
							  border: 0px;\
							  }").arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgNormal ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgHover ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgPress ) ;

		m_pImgBtn->setStyleSheet( labelStyleSheet ) ;

		buttonStyleSheet = tr( "#%1\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   min-width: 285px;\
							   min-height: 70px;\
							   max-width: 285px;\
							   max-height: 70px;\
							   font-size: 18px;\
							   color: #B2BBC6;\
							   }\
							   #%2:hover\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }\
							   #%3:checked\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }").arg( m_buttonName ).arg( m_buttonName ).arg( m_buttonName ) ;
	}
	else
	{
		buttonStyleSheet = tr( "#%1\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   min-width: 285px;\
							   min-height: 70px;\
							   max-width: 285px;\
							   max-height: 70px;\
							   font-size: 18px;\
							   color: #4A90E2;\
							   }\
							   #%2:hover\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }\
							   #%3:checked\
							   {\
							   background-image: url(:/res/background_normal.png);\
							   border: 0px;\
							   color: #4A90E2;\
							   }").arg( m_buttonName ).arg( m_buttonName ).arg( m_buttonName ) ;
	}

	setStyleSheet( buttonStyleSheet ) ;
}

void QMyToolButton::enterEvent( QEvent * event )
{
	if ( !m_buttonImgHover.isEmpty() )
	{
		char dqFileName[ 256 ] ;
		sprintf( dqFileName , "%s" , getenv( "NBENV" ) ) ;
		QString fileNameStr = tr( dqFileName ).replace( '\\' , '/' ) ;
		QString labelStyleSheet ;
		labelStyleSheet = tr( "#%1\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%2);\
							  border: 0px;\
							  min-width: 24px;\
							  min-height: 24px;\
							  max-width: 24px;\
							  max-height: 24px;\
							  }\
							  #%3:hover\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%4);\
							  border: 0px;\
							  }\
							  #%5:checked\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%6);\
							  border: 0px;\
							  }").arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgHover ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgHover ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgPress ) ;

		m_pImgBtn->setStyleSheet( labelStyleSheet ) ;
	}
	QPushButton::enterEvent( event ) ;
}

void QMyToolButton::leaveEvent( QEvent * event )
{
	if ( !m_buttonImgHover.isEmpty() )
	{
		char dqFileName[ 256 ] ;
		sprintf( dqFileName , "%s" , getenv( "NBENV" ) ) ;
		QString fileNameStr = tr( dqFileName ).replace( '\\' , '/' ) ;
		QString labelStyleSheet ;
		labelStyleSheet = tr( "#%1\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%2);\
							  border: 0px;\
							  min-width: 24px;\
							  min-height: 24px;\
							  max-width: 24px;\
							  max-height: 24px;\
							  }\
							  #%3:hover\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%4);\
							  border: 0px;\
							  }\
							  #%5:checked\
							  {\
							  background-image: url(:/res/background_normal.png);\
							  image: url(%6);\
							  border: 0px;\
							  }").arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgNormal ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgHover ) \
							  .arg( m_pixLabelName ).arg( fileNameStr + m_buttonImgPress ) ;

		m_pImgBtn->setStyleSheet( labelStyleSheet ) ;
	}
	QPushButton::enterEvent( event ) ;
}

void QMyToolButton::mouseReleaseEvent( QMouseEvent *event )
{
	if ( !m_buttonImgHover.isEmpty() )
	{
	}
	QPushButton::mouseReleaseEvent( event ) ;
}

