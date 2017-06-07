#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QDateTime>
#include <QCheckBox>
#include <QDir> 
#include <QMessageBox>
#include <QWindowsXPStyle>

#include "verapi.h"

#ifdef _HISEVENT_VERSION
	#include "QEventMainWnd.h"
#else
	#include "event_real_wnd.h"
#endif

#include "read_event_proc.h"
#include "net/netstr.h"
#include "globaldef.h"

//#include "login_dlg.h"
#include "uitools/login_widget.h"

#ifdef _HISEVENT_VERSION
	#pragma message("compile histroy event query")
#else
	#pragma message("compile real event warning")
#endif

QWidget* getMainWnd()
{
	//return qApp->mainWidget();
	QMainWindow *mainWin=NULL;
	QWidgetList list = QApplication::topLevelWidgets();
	for (int i = 0; i < list.size(); ++i) 
	{
		mainWin = qobject_cast<QMainWindow *>(list.at(i));
		if ( mainWin != NULL)
		{
			break;
		}
	}
	return mainWin;
}

int main( int argc, char ** argv )
{
#ifdef __unix
#ifdef __sun
	putenv( "LANG=zh.GBK" );
#endif
	sigset(SIGUSR1,SIG_IGN);
#endif
	
    QApplication a( argc,argv );
// 	a.setStyle( new QWindowsStyle );
//	a.setStyle( new QWindowsXPStyle );
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

// #ifdef _WIN32
// 	a.setFont( QFont( _C2Q( "宋体" ), 9 ) );
// #elif __hpux
// 	a.setFont( QFont( _C2Q( "Zycjksun" ), 14 ) );
// #else
// 	a.setFont( QFont( _C2Q( "Song" ), 12 ) );
// #endif


//事项查询版本
#ifdef _HISEVENT_VERSION
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("dbquery,1.51.1");
				printf("dbquery, 1.51.1");
				return 0;
			}
		}
	}

	QEventMainWnd w;

	a.setMainWidget( &w );

	w.showMaximized();

	//-D日期，-S事项分类，-T事项类型 -O对象代码

	if( argc > 1 )
	{
		int i;
		QString tmpStr;

		bool error = false;

		QDate date;
		QTime time;
		time.setHMS( 0, 0, 0, 0 );
		date = QDate::currentDate();
		QDateTime datetime( date, time );
		w.m_sql = tr( "" );
		for( i = 1; i < argc; i ++ )
		{
			tmpStr = tr( argv[ i ] );
			if( tmpStr.find( tr( "-D" ) ) == 0 )
			{
				tmpStr = tmpStr.right( tmpStr.length() - 2 );
				if( tmpStr == tr( "0" ) ||
					tmpStr == tr( "T" ) || 
					tmpStr == tr( "t" ))
				{
					w.onTodayBtnClicked();
				}
				else if( tmpStr == tr( "1" ) ||
					tmpStr == tr( "Y" ) || 
					tmpStr == tr( "y" ) )
				{
					w.onYestodayBtnClicked();
				}
				else
				{
					error = true;
				}
			}
			/*else if( tmpStr.find( tr( "-S" ) ) == 0 )
			{
				tmpStr = tmpStr.right( tmpStr.length() - 2 );

				if( !w.m_sql.isEmpty() )
				{
					w.m_sql += tr( " and " );
				}

				w.m_sql += tr( "分类号 = " );
				w.m_sql += tmpStr;
			}*/
			else if( tmpStr.find( tr( "-T" ) ) == 0 )
			{
				tmpStr = tmpStr.right( tmpStr.length() - 2 );

				if( !tmpStr.isEmpty() )
				{
					w.addEventType( tmpStr.toUInt( ) ); 
				}
			}
			else if( tmpStr.find( tr( "-O" ) ) == 0 )
			{
				tmpStr = tmpStr.right( tmpStr.length() - 2 );

				if( !tmpStr.isEmpty() )
				{
					w.addDev( tmpStr );
				}
			}
			else 
			{
				error = true;
			}
		}

		if( !error )
		{
			w.onEventTypeQueryBtn();
			w.m_reSearchChkBox->setChecked( true );
			w.onDevQueryBtn();
		}
	}
//实时事项版本
#else
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("realalarm,1.1");
				printf("realalarm, 1.1");
				return 0;
			}
		}
	}

	char filename[ 256 ];
	sprintf( filename , "%s/log/dat" , getenv("NBENV"));
	QDir d( filename ); 
	if(!d.exists()) {
		d.mkdir(filename);
	}


	Event_real_wnd	w;
// 	a.setMainWidget( &w );			//modify by chenkai

	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	Login_widget dlg;
	USEROPERRIGHT_STRU tmpRight;
	DISPATCHER_STRU    tmpOperInfo;

	sprintf( filename , "%s/resource/xml/realalarmuser.xml" , getenv("NBENV") );
	FILE *fp;
	//如果该文件存在说明时异常退出，读出最后登陆的用户
	if( QFile::exists( QObject::tr( filename ) ) )
	{
		char userName[ 100 ];
		memset( userName, 0, 100 );
		fp = fopen( filename, "r" ) ;
		if( fp != NULL )
		{
			fscanf( fp,  "%s", userName );
			fclose( fp );
		}
		DISPATCHER_STRU* pOper_dispatchers = dlg.get_oper_infos() ;
		for( int i = 0; i < dlg.get_user_count(); i ++ )
		{
			if( !strcmp( pOper_dispatchers[ i ].username, userName ) )
			{
				memcpy( &tmpOperInfo, &( pOper_dispatchers[ i ] ), sizeof( DISPATCHER_STRU ) );
				break;
			}
		}
		
		USEROPERRIGHT_STRU* pUser_rights = dlg.get_user_rights() ;
		for( int i = 0; i < dlg.get_host_count(); i ++ )
		{
			if( !strcmp( pUser_rights[ i ].username, userName ) )
			{
				memcpy( &tmpRight, &( pUser_rights[ i ] ), sizeof( USEROPERRIGHT_STRU ) );
				break;
			}
		}
	}
	else
	{
		if( dlg.exec() == QDialog::Accepted )
		{
			dlg.get_user_right( tmpRight );
			dlg.get_oper_info( tmpOperInfo );
			EventProc evtproc;
			SYS_EVENT	sysevent;
			SYS_TIME	tmpTime;
			NetStatus ns;
			memset( &sysevent, 0, sizeof( SYS_EVENT ) );
			sysevent.grpno = ns.GetHostGrpNo( ns.GetHostNo() );

			sysevent.sort  = ENTCLASS_OPERATION ;
			sysevent.type  = ENTTYPE_SCADA_LOGIN ;
			sysevent.DiskOut = 1;
			sysevent.DispOut = 1;
			TimeProc tmpTimeProc;
			tmpTimeProc.GetTime( tmpTime );
			sysevent.ymd = tmpTime.year * 10000 + tmpTime.mon * 100 + tmpTime.day;
			sysevent.hmsms = tmpTime.hour * 10000000 + tmpTime.min * 100000 + 
				tmpTime.sec * 1000 + tmpTime.ms;

			strcpy( sysevent.Object , tmpRight.username );
			sprintf(sysevent.text,"%s在%s节点登录智能告警",tmpOperInfo.describe, 
				ns.GetNodeName( sysevent.grpno, ns.GetHostName( ns.GetHostNo() ) ) );
			evtproc.Save( &sysevent, 1 );

			fp = fopen( filename, "w+" );
			if( fp != NULL )
			{
				fprintf( fp, "%s", tmpRight.username );
				fclose( fp );
			}
		}
		else
		{
			w.slot_stop_thread();
			return 1;
		}
	}
	
	memcpy( &( w.m_cur_user ), &tmpRight, sizeof( USEROPERRIGHT_STRU ) );
	printf( "安全级=%d, %d, %d\n", w.m_cur_user.opright[ 0 ], w.m_cur_user.opright[ 1 ],
		w.m_cur_user.opright[ 2 ]);
	memcpy( &( w.m_cur_oper_man ), &tmpOperInfo, sizeof( DISPATCHER_STRU ) );

	w.show();
#endif
 	
     int ok = a.exec();
 	return ok;
}
