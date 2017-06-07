#include "festools_wnd.h"
#include "festools_globaldef.h"

#include <QApplication>
#include <QTextCodec>
#include <QWindowsStyle>
#include <QFile>

QWidget* getMainWnd()
{
	QMainWindow *mainWin=NULL;
	QWidgetList list = QApplication::topLevelWidgets() ;
	for ( int i = 0 ; i < list.size() ; ++i ) 
	{
		mainWin = qobject_cast<QMainWindow *>(list.at(i));
		if ( mainWin != NULL)
		{
			break;
		}
	}
	return mainWin;
}

int main( int argc ,  char *argv[] )
{
	QApplication app( argc , argv ) ;
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) ) ;
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale() ) ;

// 	QFile file("stylesheet.qss");
// 	file.open(QFile::ReadOnly);
// 	QString styleSheet = _C2Q(file.readAll());
// 	app.setStyleSheet(styleSheet);

	festools_wnd festools ;
	festools.showMaximized() ;
	int ok = app.exec() ;
	return ok ;
}