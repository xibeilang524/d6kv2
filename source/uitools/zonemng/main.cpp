#include <QApplication>
#include <QTextCodec>
#include <QDateTime>
#include <QCheckBox>
#include <QDir> 
#include <QMessageBox>
#include <QTextCodec>
#include <QWindowsStyle>

#include "net/netstr.h"
#include "QZoneMainWnd.h"
#include "globaldef.h"
#include "verapi.h"

int main( int argc, char ** argv )
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("zonemng,1.1");
				printf("zonemng, 1.1");
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
// 	a.setStyle( new QWindowsStyle );

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());


// #ifdef _WIN32
// 	a.setFont( QFont( QObject::tr( "宋体" ), 12 ) );
// #elif __hpux
// 	a.setFont( QFont( QObject::tr( "ya_hei" ), 14 ) );
// #else
// 	a.setFont( QFont( QObject::tr( "Song" ), 14 ) );
// #endif



	QZoneMainWnd w( NULL, "QZoneMainWnd", Qt::Dialog );
	
// 	a.setMainWidget( &w );
	
	w.showNormal();

	NetDbg netdbg;
	netdbg.RegProc("监控区管理");
	
    int ok = a.exec();

	netdbg.UnRegProc();
	return ok;
}
