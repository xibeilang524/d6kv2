#include <QApplication>
#include <QTextCodec>
#include "hisquery_wnd.h"

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

int main( int argc , char* argv[] )
{
	QApplication app( argc , argv ) ;
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) ) ;
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale() ) ;

	Hisquery_wnd window ;
	window.showMaximized() ;
	return app.exec() ;
}