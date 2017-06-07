// QLoginDlg.cpp: implementation of the QLoginDlg class.
//
//////////////////////////////////////////////////////////////////////
#include "login_dlg.h"
#include "uitools/dmessagebox.h"
#include <QMessageBox>

#include <sys/stat.h>

#include "globaldef.h"
#include "auth.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int AUTH_FAIL_LIMIT = 3;

Login_dlg::Login_dlg( QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl ) : QDialog( parent )
{
	setupUi(this);
	m_operDispatcher = NULL;
	m_curOperDispatcher = NULL;
	m_operRight = NULL;
	m_userSumNum = 0;
	m_hostUserNum = 0;
	m_userTypeNum = 0;

	this->setStyleSheet("QDialog{background:white;}"
									"QComboBox {font-family: Microsoft YaHei;font-size: 13px;color: #3E464E;\
										min-height:25px;border: 1px solid #C6CFD9;border-radius: 2px;}"
									"QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 28px;\
										border-top-right-radius: 2px;border-bottom-right-radius: 2px;}"
									"QComboBox::down-arrow{image: url(:/img/images/dropdown.png);}"
									"QComboBox::down-arrow:pressed{image: url(:/img/images/dropdown_pressed.png);}"
									"QLabel{font-family: Microsoft YaHei;font-size: 13px;color: #0F1923;}"
									"QLineEdit {font-family:Microsoft YaHei;font-size: 13px;border: 1px solid #C6CFD9;	border-radius: 2px;min-height:25px;}");
	
	QString stytle = tr("QPushButton{background:#4A90E2;border-radius: 2px;\
						font-family: Microsoft YaHei;font-size: 13px;color: white;\
						min-width:75px;min-height:25px;}"
						"QPushButton:hover{background:#0C2543;}");
	m_okBtn->setStyleSheet(stytle);

	stytle = tr("QPushButton{background:white;border-radius: 2px;border: 1px solid #4A90E2;\
						color: #4A90E2;font-family: Microsoft YaHei;font-size: 13px;\
						min-width:75px;min-height:25px;}"
						"QPushButton:hover{background:#0C2543;border: 0px;color:white;}"	);
	m_cancelBtn->setStyleSheet(stytle);

	connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_userTypeList, SIGNAL(activated(int)), this, SLOT(onUserTypeChanged(int)));
	connect(m_okBtn, SIGNAL(clicked()), this, SLOT(onOk()));

	if( !readUserPara() )
		return;

}

Login_dlg::~Login_dlg()
{

}

int Login_dlg::exec()
{
	int i, j;

	m_userTypeList->clear();
	
	int tmpusertype[ 10 ];
	char UserTypeDesc[ 10 ][ 40 ];
	
	for( i = 0; i < 10; i ++ )
	{
		tmpusertype[ i ] = 0;
	}
	
	DISPATCHER_STRU * tmp;
	
	tmp = (DISPATCHER_STRU *)malloc( m_userSumNum * sizeof( DISPATCHER_STRU ) );
	int usernum = 0;
	
	for( i = 0; i < m_hostUserNum; i ++ )
	{
		for( j  = 0; j < m_userSumNum; j ++ )
		{
			if( strcmp( m_operRight[ i ].username, 
				m_operDispatcher[ j ].username ) )
				continue;
			
			tmpusertype[ m_operDispatcher[ j ].type ] = 1;
			strcpy( UserTypeDesc[ m_operDispatcher[ j ].type ],
				m_operDispatcher[ j ].typedescribe );
			memcpy( &tmp[ usernum ++ ], &m_operDispatcher[ j ], sizeof( DISPATCHER_STRU ) );
		}
	}
	
	if( !usernum )
	{
		free( m_operDispatcher );
		free( m_operRight );
		free( tmp );
		return FALSE;
	}
	
	free( m_operDispatcher );
	m_operDispatcher = (DISPATCHER_STRU *)malloc( usernum * sizeof( DISPATCHER_STRU ) );
	memcpy( m_operDispatcher, tmp, usernum * sizeof( DISPATCHER_STRU ) );
	free( tmp );
	m_userSumNum = usernum;
	
	for( i = 0; i < 10; i ++ )
	{
		if( !tmpusertype[ i ] )
			continue;
		
		m_userTypeList->addItem( tr( UserTypeDesc[ i ] ) );
		m_userTypes[ m_userTypeNum ++ ] = i;
	}


	onUserTypeChanged( 0 );

	return QDialog::exec();
}

void Login_dlg::onUserTypeChanged( int item )
{
	int i;
	m_userList->clear();
	for( i = 0; i < m_userSumNum; i ++ )
	{
		if( m_operDispatcher[ i ].type == m_userTypes[ item ] )
		{
			m_userList->addItem( tr( m_operDispatcher[ i ].describe ) );
		}
	}
}

void Login_dlg::onOk()
{
	if( m_userPasswd->text().isEmpty() )
	{
// 		QMessageBox::message( _C2Q( "提示" ), _C2Q( "请输入密码!" ),QString::null, this );
		DMessageBox::information( this ,tr( "提示" ), tr( "请输入密码!" ) );
		return;
	}

	int i;
	int curuserid = 0;
	m_curOperDispatcher = NULL;
	for( i = 0; i < m_userSumNum; i ++ )
	{
		if( m_operDispatcher[ i ].type == m_userTypes[ m_userTypeList->currentIndex() ] )
		{
			if( curuserid == m_userList->currentIndex() )
			{
				m_curOperDispatcher = &m_operDispatcher[ i ];
				memcpy( &m_curOperMan, m_curOperDispatcher, sizeof( DISPATCHER_STRU ) );
				break;
			}
			curuserid ++;
		}
	}
	
	static int auth_fail_count = 0;

	if( m_curOperDispatcher && (0 == user_auth(m_curOperDispatcher->username, _Q2C(m_userPasswd->text()), m_curOperDispatcher->password)) )
	{
		for( i = 0; i < m_hostUserNum; i ++ )
		{
			if( strcmp( m_curOperDispatcher->username, m_operRight[ i ].username ) )
				continue;
			memcpy( &m_curPower, &m_operRight[ i ], sizeof( USEROPERRIGHT_STRU ) );
		}
		accept();	
	}
	else
	{
		auth_fail_count++;
		if (auth_fail_count < AUTH_FAIL_LIMIT)
		{
			DMessageBox::warning( this, tr( "警告" ), tr( "密码错误!!!\n请重新输入密码" ));
			m_userPasswd->clear();
			
		}
		else
		{
			DMessageBox::warning( this, tr( "警告" ), tr( "鉴别失败次数过多，请联系管理员重置口令" ));

			//I really hate to do that....please forgive me
			exit(-1);
		}
		
	}
}

void Login_dlg::getUserRight( USEROPERRIGHT_STRU& userright )
{
	memcpy( &userright, &m_curPower, sizeof( USEROPERRIGHT_STRU ) );
}

void Login_dlg::getOperInfo( DISPATCHER_STRU & operinfo )
{
	memcpy( &operinfo, &m_curOperMan, sizeof( DISPATCHER_STRU ) );
}

int Login_dlg::readUserPara()
{
	FUNC_STR func;
	DBApi dbapi;
	CS_RETCODE ret=0;
	CS_DATAFMT *datafmt = NULL;
	
	char hostname[HOSTNAME_LEN];
	_get_host_name(hostname);
	
	if( dbapi.Open( DB_OP_R, FALSE, FALSE ) )
	{
		func.func=SEL_ISQL_RESULTS;
		strcpy(func.dbname,"modeldb");
		sprintf(func.isql,"select * from 用户口令参数表 order by 操作员类别,操作员");
		ret=dbapi.SelectIsqlResults(&func,&datafmt,(void **)&m_operDispatcher);
		
		if(datafmt) free(datafmt);
		datafmt = NULL;
		
		if(ret!=CS_SUCCEED) 
		{
			return FALSE;
		}
		else
		{
			m_userSumNum = func.ret_roxnum;
		}
		
		func.func=SEL_ISQL_RESULTS;
		strcpy(func.dbname,"modeldb");
		sprintf(func.isql,"select * from 节点权限参数表 where 主机名 ='%s' order by 操作员",hostname);
		ret=dbapi.SelectIsqlResults(&func,&datafmt,(void **)&m_operRight);
		
		if(datafmt) free(datafmt);
		datafmt = NULL;
		
		if(ret!=CS_SUCCEED) 
		{
			if( m_operDispatcher )
				free( m_operDispatcher );
			
			m_operDispatcher = NULL;
			return FALSE;
		}
		else
		{
			m_hostUserNum = func.ret_roxnum;
		}
		
		dbapi.Close();
	}
	else
	{
		char filename[ 256 ];
		
		sprintf( filename, "%s/dbmap/用户口令参数表1/pt", getenv( NBENV ) );
		if( !readFile(filename, (void**)&m_operDispatcher ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/用户口令参数表1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_userSumNum = func.ret_roxnum;

		sprintf( filename, "%s/dbmap/节点权限参数表1/pt", getenv( NBENV ) );
		if( !readFile(filename, (void**)&m_operRight ) )
			return FALSE;

		sprintf( filename, "%s/dbmap/节点权限参数表1/func", getenv( NBENV ) );
		if( !readFile(filename, &func, sizeof( FUNC_STR ) ) )
			return FALSE;

		m_hostUserNum = func.ret_roxnum;
	}
	
	return TRUE;
}

bool Login_dlg::readFile( char *filename, void ** buf )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}
	
	FILE *fp;
	int ret;
	
	if( sf.st_size == 0 )
		return TRUE;
	
	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	*buf =  (char*)malloc( sf.st_size );
	ret = fread( *buf, sf.st_size, 1, fp );
	
	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}
	
	return TRUE;
	
}

bool Login_dlg::readFile( char *filename, void * buf, int len )
{
	struct stat sf;
	
	if( stat( filename, &sf )!=0 )
	{
		return FALSE;
	}
	
	FILE *fp;
	int ret;
	
	fp = fopen( filename, "rb" );
	if( fp == NULL )
		return FALSE;
	ret = fread( buf, len, 1, fp );
	
	fclose( fp );
	fp = NULL;
	if( ret != 1 )
	{
		return FALSE;
	}
	
	return TRUE;
}