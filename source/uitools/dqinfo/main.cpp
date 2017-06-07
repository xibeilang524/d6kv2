#include <QtGui>
#include <QApplication>
#include <QEvent>
#include "dqinfo_mainwindow.h"

#include "dqinfo_dbgpara.h"

CDbgPara g_dbgPara;

int main(int argc, char *argv[])
{
	printf( "dqinfo start\n" );
#ifdef __unix
	for(int aa=1;aa<33;aa++)
	{
		if(aa!=SIGUSR1)
			sigset(aa,SIG_IGN);	
	}
#endif

	QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	mainWindow window;
	window.show();
	return app.exec();
}