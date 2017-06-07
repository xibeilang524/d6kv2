// QLoginWnd.cpp: implementation of the QLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "QLoginWnd.h"
#include "globaldef.h"
#include "uitools/dmessagebox.h"

#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlineedit.h>

#include "scd/scdapi.h"
#include "auth.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


QLoginWnd::QLoginWnd( QWidget* parent, const char* name, Qt::WindowFlags fl )
:QDialog( parent, fl )
{
	ui.setupUi(this);
	 
    connect( ui.m_cbxType, SIGNAL( activated(int) ), this, SLOT( onCbxType(int) ) );
 //	   connect( ui.m_btnOK, SIGNAL( clicked() ), this, SLOT( onBtnOK() ) );
 //	   connect( ui.m_btnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
 	   
	   // tab order
	setTabOrder( ui.m_edtPassword, ui.m_btnOK );
	setTabOrder( ui.m_btnOK, ui.m_btnCancel );
	setTabOrder( ui.m_btnCancel, ui.m_cbxType );
	setTabOrder( ui.m_cbxType, ui.m_cbxName );
	
	ui.m_edtPassword->setEchoMode(QLineEdit::Password);	// modify [4/11/2012 zhoujian]

	m_operDispatcher = NULL;
	m_curOperDispatcher = NULL;
	m_operRight = NULL;
	m_userSumNum = 0;
	m_hostUserNum = 0;
	m_userTypeNum = 0;

	if( !readUserPara() )
		return;

	int i,j = 0;

	int tmpusertype[ 10 ];
	char UserTypeDesc[ 10 ][ 40 ];

	for( i = 0; i < 10; i ++ )
	{
		tmpusertype[ i ] = 0;
	}

	DISPATCHER_STRU * tmp;

	tmp = (DISPATCHER_STRU *)malloc( m_userSumNum * sizeof( DISPATCHER_STRU ) );
	int usernum = 0;

	//for test 
	qDebug()<<tr("本机用户数：")<<m_hostUserNum<<"\n";
	qDebug()<<tr("数据库用户数：")<<m_userSumNum<<"\n";
	
	for( i = 0; i < m_hostUserNum; i ++ )
	{
		for( j  = 0; j < m_userSumNum; j ++ )
		{
			if( strcmp( m_operRight[ i ].username, m_operDispatcher[ j ].username ) )
				continue;

			tmpusertype[ m_operDispatcher[ j ].type ] = 1;
			strcpy( UserTypeDesc[ m_operDispatcher[ j ].type ],m_operDispatcher[ j ].typedescribe );
			memcpy( &tmp[ usernum ++ ], &m_operDispatcher[ j ], sizeof( DISPATCHER_STRU ) );
		}
	}

	if( !usernum )
		return;

	free( m_operDispatcher );
	m_operDispatcher = (DISPATCHER_STRU *)malloc( usernum * sizeof( DISPATCHER_STRU ) );
	memcpy( m_operDispatcher, tmp, usernum * sizeof( DISPATCHER_STRU ) );
	free( tmp );
	m_userSumNum = usernum;

	for( i = 0; i < 10; i ++ )
	{
		if( !tmpusertype[ i ] )
			continue;

		ui.m_cbxType->addItem( _C2Q( UserTypeDesc[ i ] ) );
		m_userTypes[ m_userTypeNum ++ ] = i;
	}
	onCbxType( 0 );
}

QLoginWnd::~QLoginWnd()
{
	if( m_operDispatcher )
	{
		free( m_operDispatcher );
	}
	
	if( m_operRight )
	{
		free( m_operRight );
	}
}

void QLoginWnd::onCbxType( int sel )
{
	int i;
	ui.m_cbxName->clear();
	for( i = 0; i < m_userSumNum; i ++ )
	{
		if( m_operDispatcher[ i ].type == m_userTypes[ sel ] )
		{
			ui.m_cbxName->addItem( _C2Q( m_operDispatcher[ i ].describe ) );
		}
	}
}

void QLoginWnd::onBtnOK()
{
	int i;
	int curuserid = 0;
	m_curOperDispatcher = NULL;
	for( i = 0; i < m_userSumNum; i ++ )
	{
		if( m_operDispatcher[ i ].type == m_userTypes[ ui.m_cbxType->currentIndex () ] )
		{
			if( curuserid == ui.m_cbxName->currentIndex () )
			{
				m_curOperDispatcher = &m_operDispatcher[ i ];
				break;
			}
			curuserid ++;
		}
	}

	passtype = m_userTypes[ ui.m_cbxType->currentIndex () ];	//type;
	ismonitor = m_curOperDispatcher->ismonitor;

//	if( m_curOperDispatcher && _C2Q( m_curOperDispatcher->password ) == ui.m_edtPassword->text() )
	if( m_curOperDispatcher && (0 == user_auth(m_curOperDispatcher->username, _Q2C(ui.m_edtPassword->text()), m_curOperDispatcher->password)) )
	{
		accept();	
	}
	else
	{
		DMessageBox::warning( this, _C2Q( "警告" ), _C2Q( "密码错误" ) );
	}
}

int QLoginWnd::readUserPara()
{
	FUNC_STR func;
	DBApi dbapi;
	CS_RETCODE ret=0;
	CS_DATAFMT *datafmt = NULL;
	
	char hostname[HOSTNAME_LEN];
	_get_host_name(hostname);

	if( dbapi.Open( DB_OP_R, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}
	
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


	return TRUE;
}