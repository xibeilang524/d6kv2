#include <QtGui/QApplication>
#include <QTextCodec>
#include <QWindowsStyle>

#include "outagequery.h"
#include "verapi.h"

QWidget* getMainWnd()
{
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

int main(int argc ,char *argv[])
{
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("outagequery,1.51.1");
				printf("outagequery, 1.51.1");
				return 0;
			}
		}
	}
	
	QApplication app(argc,argv) ;
	app.setStyle( new QWindowsStyle );
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	QPowerCutQuery powercutmng;
	powercutmng.show();
	return app.exec() ;
}
