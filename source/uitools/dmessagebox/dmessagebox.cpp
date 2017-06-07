#include "dmessagebox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

DMessageBox::DMessageBox( QWidget* parent /*= 0 */, const QString &title /*= tr( "Tip" ) */, 
						 const QString &text /*= ""*/, QMessageBox::StandardButtons buttons /*= QMessageBox::Ok */, 
						 QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton */ ) : QDialog( parent )
{
// 	setWindowFlags( Qt::FramelessWindowHint | Qt::Dialog ) ;
	setWindowFlags( Qt::WindowCloseButtonHint | Qt::Dialog ) ;
	setObjectName( "dmessagebox" ) ;
// 	init_title_frame() ;
	init_message_frame() ;
	init_button_frame( buttons , defaultButton ) ;
//	init_center_frame() ;

	QVBoxLayout* pLayout = new QVBoxLayout( this ) ;
	setLayout( pLayout ) ;
//	QFrame* pFrame = new QFrame( this ) ;
// 	pLayout->addWidget( m_pTitle_frame ) ;
	pLayout->addWidget( m_pMessage_frame ) ;
	pLayout->addWidget( m_pButton_frame ) ;
	pLayout->setContentsMargins( 0 , 0 , 10 , 10 ) ;
	pLayout->setSpacing( 0 ) ;
	pLayout->setStretch( 1 , 1 ) ;
	QString styleSheet = tr( "#dmessagebox\
							 {\
							 background: #FFFFFF;\
							 }" ) ;
	setStyleSheet( styleSheet ) ;

// 	pLayout = new QVBoxLayout( this ) ;
// 	pLayout->setContentsMargins( 5 , 5 , 5 , 5 ) ;
// 	pFrame->setLayout( pLayout ) ;
// 	pFrame->setObjectName( tr( "mainframe" ) ) ;
// 	pFrame->setStyleSheet( tr( "#mainframe{border:1px solid #2D3035;};" ) ) ;
// 	pLayout->addWidget( m_pCenter_frame ) ;

// 	connect( m_pClose_button , SIGNAL( clicked() ) , this , SLOT( slot_close_button() ) ) ;
	connect(m_pButton_box, SIGNAL( clicked( QAbstractButton* ) ) , this , SLOT( slot_button_clicked( QAbstractButton* ) ) ) ;

	set_message_title( title ) ;
	set_message_text( text ) ;
}

/*void DMessageBox::init_title_frame( void )
{
	m_pTitle_frame = new QFrame( this ) ;
	m_pTitle_frame->setObjectName( tr( "messagetitle" ) ) ;
	m_pTitle_frame->setStyleSheet( tr( "#messagetitle{background-image: url(:/res/dbackground_normal.png);};" ) ) ;
	m_pTitle_frame->setFixedHeight( 28 ) ;

	QHBoxLayout* pLayout = new QHBoxLayout( m_pTitle_frame ) ;
	pLayout->setContentsMargins( 10 , 0 , 0 , 0 ) ;
	m_pTitle_frame->setLayout( pLayout ) ;

	m_pTitle_pix_label = new QLabel( m_pTitle_frame ) ;
	m_pTitle_pix_label->setObjectName( tr( "messagetitlepix" ) ) ;
	m_pTitle_pix_label->setPixmap( QPixmap( ":/res/information.png" ) ) ;
	m_pTitle_pix_label->setAttribute( Qt::WA_TranslucentBackground ) ;
	m_pTitle_pix_label->setScaledContents( true ) ;
	m_pTitle_pix_label->setFixedSize( 15 , 15 ) ;

	m_pTitle_text_label = new QLabel( tr( "提示" ) , m_pTitle_frame ) ;
	m_pTitle_text_label->setObjectName( tr( "messagetitlelabel" ) ) ;
	QString styleSheet;
	styleSheet = tr( "#messagetitlelabel\
					{\
					font-family: Microsoft YaHei;\
					font-size: 12px; \
					color: rgb(255, 255, 255);\
					border: 0px;\
					}") ;
	m_pTitle_text_label->setStyleSheet( styleSheet ) ;
	m_pTitle_text_label->setAttribute( Qt::WA_TranslucentBackground ) ;

	m_pClose_button = new QPushButton( m_pTitle_frame ) ;
	m_pClose_button->setIcon( QIcon( ":/res/guanbi.png" ) ) ;
	m_pClose_button->setIconSize( QSize( 10 , 10 ) ) ;
	m_pClose_button->setFixedSize( 28 , 28 ) ;
	m_pClose_button->setObjectName( tr( "messageclosebutton" ) ) ;
	styleSheet = tr("#messageclosebutton\
					{\
					border: 0px;\
					border-radius: 0px;\
					min-height:28px;\
					max-height:28px;\
					min-width:28px;\
					max-width:28px;\
					background-image: url(:/res/dbackground_normal.png);\
					}\
					#messageclosebutton:hover\
					{\
					background-image: url(:/res/closebutton.png);\
					border: 0px;\
					}\
					#messageclosebutton:pressed\
					{\
					background-image: url(:/res/closebutton.png);\
					border: 0px;\
					}");
	m_pClose_button->setStyleSheet(styleSheet);
	m_pClose_button->setAttribute( Qt::WA_TranslucentBackground ) ;

	pLayout->addWidget( m_pTitle_pix_label ) ;
	pLayout->addWidget( m_pTitle_text_label ) ;
	pLayout->addWidget( m_pClose_button ) ;
	pLayout->setStretch( 1 , 1 ) ;
}*/

void DMessageBox::init_message_frame( void )
{
	m_pMessage_frame = new QFrame( this ) ;
	m_pMessage_frame->setObjectName( tr( "messageframe" ) ) ;

	QHBoxLayout* pLayout = new QHBoxLayout( m_pMessage_frame ) ;
	pLayout->setContentsMargins( 10 , 20 , 0 , 20 ) ;
	pLayout->setSpacing( 15 ) ;
	m_pMessage_frame->setLayout( pLayout ) ;

	m_pPix_label = new QLabel( m_pMessage_frame ) ;
	m_pPix_label->setObjectName( tr( "messageimage" ) ) ;
	m_pPix_label->setPixmap( QPixmap( ":/res/information.png" ) ) ;
	m_pPix_label->setScaledContents( true ) ;
	m_pPix_label->setFixedSize( 30 , 30 ) ;

	m_pText_label = new QLabel( m_pMessage_frame ) ;
	m_pText_label->setText( tr( "for test !" ) ) ;
	m_pText_label->setObjectName( tr( "messagetext" ) ) ;
	m_pText_label->setAlignment( Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter ) ;
	QString styleSheet = tr( "#messageframe\
							 {\
							 background: #FFFFFF;\
							 }\
							 #messageimage\
							 {\
							 min-width:30px;\
							 max-width:30px;\
							 min-height:30px;\
							 max-height:30px;\
							 }\
							 #messagetext\
							 {\
							 font-family: Microsoft YaHei;\
							 font-size: 14px;\
							 color: rgb(0, 0, 0);\
							 border: 0px;\
							 max-height:300px;\
							 }" ) ;
	m_pMessage_frame->setStyleSheet( styleSheet ) ;
	m_pText_label->setAttribute( Qt::WA_TranslucentBackground ) ;

	pLayout->addWidget( m_pPix_label ) ;
	pLayout->addWidget( m_pText_label ) ;
	pLayout->setStretch( 1 , 1 ) ;
}

void DMessageBox::init_button_frame( QMessageBox::StandardButtons buttons , QMessageBox::StandardButton defaultButton )
{
	m_pButton_frame = new QFrame( this ) ;
	m_pButton_frame->setObjectName( tr( "buttonframe" ) ) ;
	QHBoxLayout* pLayout = new QHBoxLayout( m_pButton_frame ) ;
	m_pButton_frame->setLayout( pLayout ) ;

	m_pButton_box = new QDialogButtonBox( m_pButton_frame ) ;
	m_pButton_box->setStandardButtons(QDialogButtonBox::StandardButtons( int( buttons ) ) );
	set_default_button( defaultButton ) ;
	pLayout->addWidget( m_pButton_box ) ;

	QPushButton* pYes_button = m_pButton_box->button( QDialogButtonBox::Yes ) ;
	if ( pYes_button != NULL )
	{
		pYes_button->setText( tr( "是(Y)" ) ) ;
		pYes_button->setObjectName( "dyesbutton" ) ;
	}

	QPushButton* pNo_button = m_pButton_box->button( QDialogButtonBox::No ) ;
	if ( pNo_button != NULL )
	{
		pNo_button->setText( tr( "否(N)" ) ) ;
		pNo_button->setObjectName( "dnobutton" ) ;
	}

	QPushButton* pOk_button = m_pButton_box->button( QDialogButtonBox::Ok ) ;
	if ( pOk_button != NULL )
		pOk_button->setText( tr( "确定(O)" ) ) ;

	QPushButton* pCancel_button = m_pButton_box->button( QDialogButtonBox::Cancel ) ;
	if ( pCancel_button != NULL )
	{
		pCancel_button->setText( tr( "取消(C)" ) ) ;
		pCancel_button->setObjectName( "dcancelbutton" ) ;
	}

	QPushButton* pSave_button = m_pButton_box->button( QDialogButtonBox::Save ) ;
	if ( pSave_button != NULL )
		pSave_button->setText( tr( "保存(S)" ) ) ;

	QPushButton* pDiscard_button = m_pButton_box->button( QDialogButtonBox::Discard ) ;
	if ( pDiscard_button != NULL )
		pDiscard_button->setText( tr( "放弃(D)" ) ) ;

	QPushButton* pRetry_button = m_pButton_box->button( QDialogButtonBox::Retry ) ;
	if ( pRetry_button != NULL )
		pRetry_button->setText( tr( "重试(R)" ) ) ;

	QPushButton* pAbort_button = m_pButton_box->button( QDialogButtonBox::Abort ) ;
	if ( pAbort_button != NULL )
		pAbort_button->setText( tr( "终止(R)" ) ) ;

	QPushButton* pIgnore_button = m_pButton_box->button( QDialogButtonBox::Ignore ) ;
	if ( pIgnore_button != NULL )
		pIgnore_button->setText( tr( "忽略(I)" ) ) ;

	QString styleSheet = tr("#buttonframe\
							{\
							background: #FFFFFF;\
							}\
							QAbstractButton\
							{\
							background: #4A90E2;\
							font-family: Microsoft YaHei;\
							font-size:13px;\
							color: white;\
							min-width: 60px;\
							min-height: 25px;\
							max-width: 60px;\
							max-height: 25px;\
							border-radius: 2px;\
							border-color: #4A90E2;\
							}\
							QAbstractButton:disabled\
							{\
							background:darkgray;\
							}\
							QAbstractButton:hover\
							{\
							background: #0C2543;\
							}\
							#dnobutton,#dcancelbutton\
							{\
							background: white;\
							font-family: Microsoft YaHei;\
							font-size:13px;\
							color: #4A90E2;\
							min-width: 60px;\
							min-height: 23px;\
							max-width: 60px;\
							max-height: 23px;\
							border-radius: 2px;\
							border: 1px solid #4A90E2;\
							}\
							#dnobutton:disabled,#dcancelbutton:disabled\
							{\
							background:darkgray;\
							}\
							#dnobutton:hover,#dcancelbutton:hover\
							{\
							background: #0C2543;\
							border: 0px;\
							color:white;\
							}\
							");
	m_pButton_frame->setStyleSheet(styleSheet);
}

void DMessageBox::init_center_frame( void )
{
	m_pCenter_frame = new QFrame( this ) ;
	m_pCenter_frame->setObjectName( tr( "messagecenter" ) ) ;

	QVBoxLayout* pLayout = new QVBoxLayout( m_pCenter_frame ) ;
	pLayout->setContentsMargins( 5 , 5 , 5 , 5 ) ;
	pLayout->setSpacing( 0 ) ;
	m_pCenter_frame->setLayout( pLayout ) ;
	pLayout->addWidget( m_pMessage_frame ) ;
	pLayout->addWidget( m_pButton_frame ) ;
	QString styleSheet;
	styleSheet = tr( "#messagecenter\
					 {\
					 border:1px solid #CDC0B0;\
					 border-radius: 5px;\
					 }") ;
	m_pCenter_frame->setStyleSheet( styleSheet ) ;
}

// void DMessageBox::mousePressEvent( QMouseEvent* event )
// {
// 	if( event->button() == Qt::LeftButton )
// 	{
// 		m_mousePress = true ;
// 	}
// 	m_movePoint = event->globalPos() - pos() ;
// }
// 
// void DMessageBox::mouseReleaseEvent( QMouseEvent * )
// {
// 	m_mousePress = false ;
// }
// 
// void DMessageBox::mouseMoveEvent( QMouseEvent *event )
// {
// 	if( m_mousePress )
// 	{
// 		QPoint movePos = event->globalPos() ;
// 		move( movePos - m_movePoint ) ;
// 	}
// }
// 
// void DMessageBox::slot_close_button()
// {
// 	close() ;
// }

void DMessageBox::set_message_title( QString title )
{
	setWindowTitle( title ) ;
//	m_pTitle_text_label->setText( title ) ;
}

void DMessageBox::set_message_text( QString text )
{
	m_pText_label->setText( text ) ;
}

void DMessageBox::set_message_pix( QString pixmap , bool flag )
{
// 	m_pTitle_pix_label->setPixmap( QPixmap( pixmap ) ) ;
	QString styleSheet ;
	m_pMessage_frame->setStyleSheet( styleSheet ) ;
	if ( flag )
	{
		styleSheet = tr( "#messageframe\
						 {\
						 background: #FFFFFF;\
						 }\
						 #messageimage\
						 {\
						 min-width:64px;\
						 max-width:64px;\
						 min-height:64px;\
						 max-height:64px;\
						 }\
						 #messagetext\
						 {\
						 font-family: Microsoft YaHei;\
						 font-size: 14px;\
						 color: rgb(0, 0, 0);\
						 border: 0px;\
						 max-height:300px;\
						 }" ) ;
		m_pPix_label->setFixedSize( 64 , 64 ) ;
	}
	else
	{
		styleSheet = tr( "#messageframe\
						 {\
						 background: #FFFFFF;\
						 }\
						 #messageimage\
						 {\
						 min-width:30px;\
						 max-width:30px;\
						 min-height:30px;\
						 max-height:30px;\
						 }\
						 #messagetext\
						 {\
						 font-family: Microsoft YaHei;\
						 font-size: 14px;\
						 color: rgb(0, 0, 0);\
						 border: 0px;\
						 max-height:300px;\
						 }" ) ;
		m_pPix_label->setFixedSize( 30 , 30 ) ;
	}
	m_pPix_label->setPixmap( QPixmap( pixmap ) ) ;
	m_pMessage_frame->setStyleSheet( styleSheet ) ;
}

void DMessageBox::set_default_button( QPushButton *pButton )
{
	if ( !m_pButton_box->buttons().contains( pButton ) )
		return;
	m_pDefault_button = pButton ;
	pButton->setDefault( true ) ;
	pButton->setFocus();
}

void DMessageBox::set_default_button( QMessageBox::StandardButton button )
{
	set_default_button( m_pButton_box->button( QDialogButtonBox::StandardButton( button ) ) ) ;
}

void DMessageBox::slot_button_clicked( QAbstractButton* pButton )
{
	m_pClicked_button = pButton ;
	done( exec_return_code( pButton ) ) ;
}

QAbstractButton * DMessageBox::clicked_button( void ) const
{
	return m_pClicked_button;
}

QMessageBox::StandardButton DMessageBox::standard_button( QAbstractButton* pButton ) const
{
	QMessageBox::StandardButton ret = ( QMessageBox::StandardButton )m_pButton_box->standardButton( pButton ) ;
	if  (ret == QMessageBox::NoButton ) 
	{
		ret = QMessageBox::No ;
	} 

	return ret ;
}

int DMessageBox::exec_return_code( QAbstractButton* pButton )
{
	int ret = m_pButton_box->standardButton( pButton ) ;
	if  (ret == QMessageBox::NoButton ) 
	{
		ret = QMessageBox::No ;
	} 

	return ret ;
}

QMessageBox::StandardButton DMessageBox::information( QWidget* parent , const QString &title , const QString &text , 
													 QMessageBox::StandardButtons buttons /*= QMessageBox::Ok*/, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton*/ )
{
	DMessageBox msg_box( parent , title , text , buttons , defaultButton ) ;
	msg_box.set_message_pix( ":/res/dinformation.png" ) ;
	msg_box.setWindowIcon( QIcon( ":/res/dinformation1.png" ) ) ;
	if ( msg_box.exec() == -1 )
	{
		return QMessageBox::Cancel ;
	}
	else
	{
		return msg_box.standard_button( msg_box.clicked_button() ) ;
	}
}

QMessageBox::StandardButton DMessageBox::question( QWidget* parent , const QString &title , const QString &text , QMessageBox::StandardButtons buttons /*= QMessageBox::Ok */, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton */ )
{
	DMessageBox msg_box( parent , title , text , buttons , defaultButton ) ;
	msg_box.set_message_pix( ":/res/dquestion.png" ) ;
	msg_box.setWindowIcon( QIcon( ":/res/dquestion1.png" ) ) ;
	if ( msg_box.exec() == -1 )
	{
		return QMessageBox::Cancel ;
	}
	else
	{
		return msg_box.standard_button( msg_box.clicked_button() ) ;
	}
}

QMessageBox::StandardButton DMessageBox::warning( QWidget* parent , const QString &title , const QString &text , QMessageBox::StandardButtons buttons /*= QMessageBox::Ok */, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton */ )
{
	DMessageBox msg_box( parent , title , text , buttons , defaultButton ) ;
	msg_box.set_message_pix( ":/res/dwarning.png" ) ;
	msg_box.setWindowIcon( QIcon( ":/res/dwarning1.png" ) ) ;
	if ( msg_box.exec() == -1 )
	{
		return QMessageBox::Cancel ;
	}
	else
	{
		return msg_box.standard_button( msg_box.clicked_button() ) ;
	}
}

QMessageBox::StandardButton DMessageBox::critical( QWidget* parent , const QString &title , const QString &text , QMessageBox::StandardButtons buttons /*= QMessageBox::Ok */, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton */ )
{
	DMessageBox msg_box( parent , title , text , buttons , defaultButton ) ;
	msg_box.set_message_pix( ":/res/derror.png" ) ;
	msg_box.setWindowIcon( QIcon( ":/res/derror1.png" ) ) ;
	if ( msg_box.exec() == -1 )
	{
		return QMessageBox::Cancel ;
	}
	else
	{
		return msg_box.standard_button( msg_box.clicked_button() ) ;
	}
}

QMessageBox::StandardButton DMessageBox::about( QWidget* parent , const QString &title , const QString &text , const QString &pixmap ,QMessageBox::StandardButtons buttons /*= QMessageBox::Ok */, QMessageBox::StandardButton defaultButton /*= QMessageBox::NoButton */ )
{
	DMessageBox msg_box( parent , title , text , buttons , defaultButton ) ;
	if ( pixmap.isEmpty() )
	{
		msg_box.set_message_pix( ":/res/dabout.png" ) ;
	}
	else
	{
		msg_box.set_message_pix( pixmap , true ) ;
	}
	msg_box.setWindowIcon( QIcon( ":/res/dabout1.png" ) ) ;
	if ( msg_box.exec() == -1 )
	{
		return QMessageBox::Cancel ;
	}
	else
	{
		return msg_box.standard_button( msg_box.clicked_button() ) ;
	}
}
