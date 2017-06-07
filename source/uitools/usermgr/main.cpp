#include <qapplication.h>
#include <qtextcodec.h>
#include <qdatetime.h>
#include <qcheckbox.h>
#include <qdir.h> 
#include <qmessagebox.h>

#include "net/netstr.h"
//#include "QZoneMainWnd.h"
#include "globaldef.h"

#include "QLoginWnd.h"
#include "QusercfgDlg.h"
#include "QuserWnd.h"
#include "verapi.h"

NetDbg netdbg;

int main( int argc, char ** argv )
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("usermng,1.51.1");
				printf("usermng, 1.51.1");
				return 0;
			}
		}
	}
#ifdef __unix
#ifdef __sun
	putenv( "LANG=zh.GBK" );
#endif
	sigset(SIGUSR1,SIG_IGN);
#endif

#ifdef __alpha
	{
		if(setsid()==(pid_t)-1) {
			if(setpgrp(0,getpid())==-1)
				printf("\n can't change process group\n");
			else 
				printf("\n change process group ok1\n");
		}
		else 
			printf("\n change process group ok2\n");
	}
#elif __sun
	{
		if(setpgrp()==-1)              //脱离控制台运行
			printf("\n can't change process group\n");
		else 
			printf("\n change process group ok\n");
	}
#endif
	
    QApplication a( argc,argv );

	a.setStyle( new QWindowsStyle );
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

/*#ifdef _WIN32
	a.setFont( QFont( _C2Q( "宋体" ), 12 ) );
#elif __hpux
	a.setFont( QFont( _C2Q( "ya_hei" ), 14 ) );
#else
	a.setFont( QFont( _C2Q( "Song" ), 14 ) );
#endif*/

//	QZoneMainWnd w( NULL, "QZoneMainWnd", Qt::WType_Dialog );
//	QLoginWnd w(NULL, "QLOginWnd", Qt::Window);
	QuserWnd w(NULL, "QUsercfgDlg");
//	a.setMainWidget( &w );

	w.window()->setWindowTitle(_C2Q("角色权限管理"));
	w.showNormal();

	netdbg.RegProc("角色权限管理");
	netdbg.RegType(ATUHTYPE,"XMAN");

//	netdbg.PutDbg(ATUHTYPE,"%s",)


	
    int ok = a.exec();

	netdbg.UnRegProc();
	return ok;
}
