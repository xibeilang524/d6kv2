#include "login_bottom_widget.h"
#include "uitools/dmessagebox.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QProcess>

#include <sys/stat.h>

Login_bottom_widget::Login_bottom_widget( QWidget* parent /*= 0 */ ) : QFrame( parent )
{
	ini_ui() ;

	m_pOper_dispatcher = NULL ;
	m_pCur_oper_dispatcher = NULL ;
	m_pOper_right = NULL ;
	m_user_count = 0 ;
	m_host_user_count = 0 ;
	m_user_type_count = 0 ;

	connect( m_pUser_type_combobox  , SIGNAL( activated(int) ) , this , SLOT( slot_user_type_changed(int) ) ) ;
	connect( m_pLogin_button  , SIGNAL( clicked() ) , this , SLOT( slot_ok_btn() ) ) ;
	connect( m_pRegister_button  , SIGNAL( clicked() ) , this , SLOT( slot_register_btn() ) ) ;
	connect( m_pFind_password_button  , SIGNAL( clicked() ) , this , SLOT( slot_find_password_btn() ) ) ;

	if( !read_user_para() )
	{
		return  ;
	}
	else
	{
		ini_login_dialog() ;
	}
}

void Login_bottom_widget::ini_ui( void )
{
	this->setObjectName( "bottomwidget" ) ;
	this->setGeometry( 0 , LOGIN_TOP_HEIGHT , LOGIN_BOTTOM_WIDTH , LOGIN_BOTTOM_HEIGHT ) ;

	m_pLogin_button = new QPushButton( QIcon( ":/res/shield.png" ) , tr( "安 全 登 录" ) , this ) ; 
	m_pRegister_button = new QPushButton( tr( "注册账号" ) , this ) ; 
	m_pRegister_button->setObjectName( "registerbutton" ) ;
	m_pFind_password_button = new QPushButton( tr( "找回密码" ) , this ) ; 
	m_pFind_password_button->setObjectName( "findpasswordbutton" ) ;
	m_pUser_type_combobox = new QComboBox( this ) ;
	m_pUser_type_combobox->setEditable( true ) ;
	m_pUser_type_combobox->lineEdit()->setPlaceholderText( tr( "  用户类型" ) ) ;
	m_pUser_name_combobox = new QComboBox( this ) ;
	m_pUser_name_combobox->setEditable( true ) ;
	m_pUser_name_combobox->lineEdit()->setPlaceholderText( tr( "  用户名" ) ) ;
	m_pPassword_lineedit = new QLineEdit ;
	m_pPassword_lineedit->setPlaceholderText( tr( "  密码" ) ) ;
	m_pPassword_lineedit->setEchoMode( QLineEdit::Password ) ;
	m_pUser_pix_label = new QLabel ;
	m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_superman.png" ) ) ;
	m_pUser_pix_label->setAlignment(Qt::AlignCenter);
	m_pUser_pix_label->setFixedSize( QSize( 80 , 80 ) ) ;

	QGridLayout* pGridLayout = new QGridLayout ;
	QSpacerItem* pVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding) ;

	pGridLayout->setSpacing( 0 ) ;
	pGridLayout->addWidget( m_pUser_type_combobox , 0 , 1 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pUser_name_combobox , 1 , 1 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pPassword_lineedit , 2 , 1 , 1 , 1 ) ;
	pGridLayout->addItem( pVerticalSpacer , 3 , 1 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pRegister_button , 1 , 2 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pFind_password_button , 2 , 2 , 1 , 1 ) ;
	pGridLayout->addWidget( m_pUser_pix_label , 0 , 0 , 5 , 1 ) ;
	pGridLayout->addWidget( m_pLogin_button , 4 , 1 , 1 , 1 ) ;
	pGridLayout->setContentsMargins( 5 , 5 , 5 , 5 ) ;
	this->setLayout( pGridLayout ) ;

	QString buttonStyleSheet ;
	buttonStyleSheet = tr( "#bottomwidget \
						   {\
						   border:1px solid #000000;\
						   }\
							QToolButton\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   font: 16px;\
						   color:#FFFFFF;\
						   min-height: 30px;\
						   max-height: 30px;\
						   min-width: 58px ;\
						   }\
						   QToolButton:hover\
						   {\
						   background-image: url(:/res/back_normal.png);\
						   opacity: 0.6;\
						   background: #6E7D8F;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:pressed\
						   {\
						   background: #0C2543;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QToolButton:disabled\
						   {\
						   background: #E5E5E5;\
						   border: 0px solid #FFFFFF;\
						   border-radius: 3px;\
						   }\
						   QPushButton\
						   {\
						   background:#4A90E2;\
						   border-radius: 2px;\
						   font-family: Microsoft YaHei;\
						   font-size: 15px;\
						   color: white;\
						   min-width:75px;\
						   min-height:30px;\
						   }\
						   QPushButton:hover\
						   {\
						   background:#0C2543;\
						   }\
						   QLabel\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 15px;\
						   color: #3E464E;\
						   min-height:30px;\
						   }\
						   QComboBox \
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 15px;\
						   color: #3E464E;\
						   min-height:30px;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 2px;\
						   }\
						   QComboBox:selected \
						   {\
						   border: 1px solid #6699CC;\
						   }\
						   QComboBox::drop-down \
						   {\
						   subcontrol-origin: padding;\
						   subcontrol-position: top right;width: 28px;\
						   border-top-right-radius: 2px;\
						   border-bottom-right-radius: 2px;\
						   }\
						   QComboBox::down-arrow\
						   {\
						   image: url(:/res/drop_down.png);\
						   }\
						   QLineEdit\
						   {\
						   font-family: Microsoft YaHei;\
						   font-size: 15px;\
						   color: #3E464E;\
						   background: #FFFFFF;\
						   border: 1px solid #C6CFD9;\
						   border-radius: 4px;\
						   min-height: 30px;\
						   max-height: 30px;\
						   }\
						   #findpasswordbutton\
						   {\
						   background-color:transparent;\
						   color:#4990E2;\
						   }\
						   #findpasswordbutton:hover\
						   {\
						   color:#0C2543;\
						   }\
						   #findpasswordbutton:pressed\
						   {\
						   color:#0C2543;\
						   }\
						   #registerbutton\
						   {\
						   background-color:transparent;\
						   color:#4990E2;\
						   }\
						   #registerbutton:hover\
						   {\
						   color:#0C2543;\
						   }\
						   #registerbutton:pressed\
						   {\
						   color:#0C2543;\
						   }\
						   ") ;

	setStyleSheet( buttonStyleSheet ) ;
}

int Login_bottom_widget::read_user_para( void )
{
	FUNC_STR func ;
	DBApi dbapi ;
	CS_RETCODE ret=0 ;
	CS_DATAFMT *datafmt = NULL ;

	char hostname[HOSTNAME_LEN] ;
	_get_host_name(hostname) ;

	if( dbapi.Open( DB_OP_R , FALSE , FALSE ) )
	{
		func.func = SEL_ISQL_RESULTS ;
		strcpy(func.dbname ,"modeldb") ;
		sprintf(func.isql ,"select * from 用户口令参数表 order by 操作员类别 ,操作员") ;
		ret=dbapi.SelectIsqlResults(&func ,&datafmt ,(void **)&m_pOper_dispatcher) ;

		if( datafmt ) free( datafmt ) ;
		datafmt = NULL ;

		if( ret != CS_SUCCEED ) 
		{
			return false ;
		}
		else
		{
			m_user_count = func.ret_roxnum ;
		}

		func.func = SEL_ISQL_RESULTS ;
		strcpy( func.dbname , "modeldb" ) ;
		sprintf( func.isql , "select * from 节点权限参数表 where 主机名 ='%s' order by 操作员" , hostname ) ;
		ret = dbapi.SelectIsqlResults( &func , &datafmt , ( void ** )&m_pOper_right ) ;

		if( datafmt ) free( datafmt ) ;
		datafmt = NULL ;

		if( ret != CS_SUCCEED ) 
		{
			if( m_pOper_dispatcher )
			{
				free( m_pOper_dispatcher ) ;
			}

			m_pOper_dispatcher = NULL ;
			return false ;
		}
		else
		{
			m_host_user_count = func.ret_roxnum ;
		}

		dbapi.Close() ;
	}
	else
	{
		char file_name[ 256 ] ;

		sprintf( file_name , "%s/dbmap/用户口令参数表1/pt" , getenv( NBENV ) ) ;
		if( !read_file( file_name , ( void** )&m_pOper_dispatcher ) )
		{
			return false ;
		}

		sprintf( file_name , "%s/dbmap/用户口令参数表1/func" , getenv( NBENV ) ) ;
		if( !read_file( file_name , &func , sizeof( FUNC_STR ) ) )
		{
			return false ;
		}

		m_user_count = func.ret_roxnum ;

		sprintf( file_name , "%s/dbmap/节点权限参数表1/pt" , getenv( NBENV ) ) ;
		if( !read_file(file_name , (void**)&m_pOper_right ) )
		{
			return false ;
		}

		sprintf( file_name , "%s/dbmap/节点权限参数表1/func" , getenv( NBENV ) ) ;
		if( !read_file(file_name , &func , sizeof( FUNC_STR ) ) )
		{
			return false ;
		}

		m_host_user_count = func.ret_roxnum ;
	}

	return true ;
}

bool Login_bottom_widget::read_file( char *filename , void ** buf )
{
	struct stat sf ;

	if( stat( filename , &sf )!=0 )
	{
		return false ;
	}

	FILE *fp ;
	int ret ;

	if( sf.st_size == 0 )
		return true ;

	fp = fopen( filename , "rb" ) ;
	if( fp == NULL )
		return false ;
	*buf =  (char*)malloc( sf.st_size ) ;
	ret = fread( *buf , sf.st_size , 1 , fp ) ;

	fclose( fp ) ;
	fp = NULL ;
	if( ret != 1 )
	{
		return false ;
	}

	return true ;

}

bool Login_bottom_widget::read_file( char *filename , void * buf , int len )
{
	struct stat sf ;

	if( stat( filename , &sf )!=0 )
	{
		return false ;
	}

	FILE *fp ;
	int ret ;

	fp = fopen( filename , "rb" ) ;
	if( fp == NULL )
		return false ;
	ret = fread( buf , len , 1 , fp ) ;

	fclose( fp ) ;
	fp = NULL ;
	if( ret != 1 )
	{
		return false ;
	}

	return true ;
}

void Login_bottom_widget::ini_login_dialog( void )
{
	m_pUser_type_combobox->clear() ;

	int tmp_user_type[ 10 ] ;
	char user_type_desc[ 10 ][ 40 ] ;

	for( int i = 0 ; i < 10 ; i ++ )
	{
		tmp_user_type[ i ] = 0 ;
	}

	DISPATCHER_STRU* pTmp_dispatcher ;

	pTmp_dispatcher = ( DISPATCHER_STRU * )malloc( m_user_count * sizeof( DISPATCHER_STRU ) ) ;
	int user_count = 0 ;

	for( int i = 0 ; i < m_host_user_count ; i ++ )
	{
		for( int j  = 0 ; j < m_user_count ; j ++ )
		{
			if( strcmp( m_pOper_right[ i ].username , m_pOper_dispatcher[ j ].username ) )
			{
				continue ;
			}

			tmp_user_type[ m_pOper_dispatcher[ j ].type ] = 1 ;
			strcpy( user_type_desc[ m_pOper_dispatcher[ j ].type ] ,m_pOper_dispatcher[ j ].typedescribe ) ;
			memcpy( &pTmp_dispatcher[ user_count ++ ] , &m_pOper_dispatcher[ j ] , sizeof( DISPATCHER_STRU ) ) ;
		}
	}

	if( !user_count )
	{
		free( m_pOper_dispatcher ) ;
		free( m_pOper_right ) ;
		free( pTmp_dispatcher ) ;
		return ;
	}

	free( m_pOper_dispatcher ) ;
	m_pOper_dispatcher = ( DISPATCHER_STRU * )malloc( user_count * sizeof( DISPATCHER_STRU ) ) ;
	memcpy( m_pOper_dispatcher , pTmp_dispatcher , user_count * sizeof( DISPATCHER_STRU ) ) ;
	free( pTmp_dispatcher ) ;
	m_user_count = user_count ;

	for( int i = 0 ; i < 10 ; i ++ )
	{
		if( !tmp_user_type[ i ] )
		{
			continue ;
		}

		m_pUser_type_combobox->addItem( tr( user_type_desc[ i ] ) ) ;
		m_user_types[ m_user_type_count ++ ] = i ;
	}

	slot_user_type_changed( 0 ) ;
}

void Login_bottom_widget::slot_user_type_changed( int index )
{
	m_pUser_name_combobox->clear() ;
	switch ( m_user_types[ index ] )
	{
	case 0:
		m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_superman.png" ) ) ;
		break;
	case 1:
		m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_dd.png" ) ) ;
		break;
	case 2:
		m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_jk.png" ) ) ;
		break;
	case 3:
		m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_normal.png" ) ) ;
		break;
	default:
		m_pUser_pix_label->setPixmap( QPixmap( ":/res/login_hover.png" ) ) ;
		break;
	}
	for( int i = 0 ; i < m_user_count ; i ++ )
	{
		if( m_pOper_dispatcher[ i ].type == m_user_types[ index ] )
		{
			m_pUser_name_combobox->addItem( tr( m_pOper_dispatcher[ i ].describe ) ) ;
		}
	}

	m_pUser_name_combobox->setEditable( false ) ;
	m_pUser_type_combobox->setEditable( false ) ;
}

void Login_bottom_widget::slot_ok_btn( void )
{
	if( m_pPassword_lineedit->text().isEmpty() )
	{
		QMessageBox::information( this , tr( "提示" ) , tr( "请输入密码!" ) ) ;
		return ;
	}

	int cur_user_id = 0 ;
	m_pCur_oper_dispatcher = NULL ;

	for( int i = 0 ; i < m_user_count ; i ++ )
	{
		if( m_pOper_dispatcher[ i ].type == m_user_types[ m_pUser_type_combobox->currentIndex() ] )
		{
			if( cur_user_id == m_pUser_name_combobox->currentIndex() )
			{
				m_pCur_oper_dispatcher = &m_pOper_dispatcher[ i ] ;
				memcpy( &m_cur_oper_man , m_pCur_oper_dispatcher , sizeof( DISPATCHER_STRU ) ) ;
				break ;
			}
			cur_user_id ++ ;
		}
	}

	if( m_pCur_oper_dispatcher && tr( m_pCur_oper_dispatcher->password ) == m_pPassword_lineedit->text() )
	{
		for( int i = 0 ; i < m_host_user_count ; i ++ )
		{
			if( strcmp( m_pCur_oper_dispatcher->username , m_pOper_right[ i ].username ) )
			{
				continue ;
			}
			memcpy( &m_cur_user_right , &m_pOper_right[ i ] , sizeof( USEROPERRIGHT_STRU ) ) ;
		}
		( ( QDialog* )this->parent() )->accept() ;	
	}
	else
	{
		DMessageBox::warning( this , tr( "警告" ) , tr( "密码错误" ) ) ;
	}
}

void Login_bottom_widget::get_user_right( USEROPERRIGHT_STRU& user_right )
{
	memcpy( &user_right , &m_cur_user_right, sizeof( USEROPERRIGHT_STRU ) );
}

void Login_bottom_widget::get_oper_info( DISPATCHER_STRU& oper_info )
{
	memcpy( &oper_info, &m_cur_oper_man, sizeof( DISPATCHER_STRU ) );
}

void Login_bottom_widget::slot_register_btn( void )
{
	QProcess process ;
	process.startDetached( "dbconfig" ) ;
}

void Login_bottom_widget::slot_find_password_btn( void )
{
	QMessageBox msg_box;
	QString     msg_text;
#ifdef WIN32
	msg_text = QString( tr("<p><b style=color:#00F; font-size: large; >请联系南京大全自动化科技有限公司</b></p> \
						   <code style=color:#00F; font-size: 24px;><br>Tel:13888888888<br>\
						   <a>Mail:gongchengbu@daqo.com</a><br>\
						   <a href=\"http://www.daqo.com/\">http://www.daqo.com/</a></code>" ) ) ;
#else
	msg_text=QString( tr("<p><b style=color:#00F; >请联系南京大全自动化科技有限公司</b></p> \
						 <code style=color:#00F;><br>Tel:13888888888<br>\
						 <a>Mail:gongchengbu@daqo.com</a><br>\
						 <a href=\"http://www.daqo.com/\">http://www.daqo.com/</a></code>" ) ) ;
#endif
	msg_box.setText( msg_text ) ;
	msg_box.setStandardButtons( QMessageBox::Ok ) ;
	msg_box.setDefaultButton( QMessageBox::No ) ;
	QPixmap pixmap( ":/res/Daqo_en.png" ) ;
	msg_box.setIconPixmap( pixmap.scaled( 100 , 33 ) ) ;
	msg_box.setWindowTitle( tr( "忘记密码") ) ;
	msg_box.exec();
	return;
}

int Login_bottom_widget::get_user_count( void )
{
	return m_user_count ;
}

DISPATCHER_STRU* Login_bottom_widget::get_oper_infos( void )
{
	return m_pOper_dispatcher ;
}

int Login_bottom_widget::get_host_count( void )
{
	return m_host_user_count ;
}

USEROPERRIGHT_STRU* Login_bottom_widget::get_user_rights( void )
{
	return m_pOper_right ;
}
