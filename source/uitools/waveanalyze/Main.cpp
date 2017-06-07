#include <QApplication>
#include <QTextCodec> 
#include <QMainWindow>

#include "mainformimpl.h"

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

int main( int argc, char **argv )
{
    QApplication app( argc, argv );
// 	app.setDefaultCodec(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	Main_form_impl mainForm;
// 	app.setMainWidget(&mainForm);
// 	mainForm.showMaximized();
	mainForm.show();

    return app.exec();
}
