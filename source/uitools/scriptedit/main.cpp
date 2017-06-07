#include <QtGlobal>
#include <QApplication>
#include <QtGui>
#include <QString>
#include <QLatin1String>
#include <QTextDecoder>
#include <QTranslator>
#include <QtGlobal>
#include <QDebug>

#include "mainwindow.h"
#include "../hiscurve/dbextension.h"
#include "verapi.h"
#include "uitools/dmessagebox.h"

extern DBExtension dbex;

int main(int argc, char **argv) 
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("scriptedit,1.1");
				printf("scriptedit, 1.1");
				return 0;
			}
		}
	}
	QApplication app(argc, argv);
	app.setStyle( new QWindowsStyle );
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	QTranslator  translatorMain,translatorQt;
	QString nbenv=MainWindow::getEnvVar();
	QString local=QLocale::system().name();
	translatorMain.load(nbenv+QLatin1String("scriptedit_")+local);
	translatorQt.load(nbenv+QLatin1String("/resource/qt_")+local);  //system qt res in NBENV/resource/
	app.installTranslator(&translatorMain);
	app.installTranslator(&translatorQt);
#if defined(Q_OS_WIN32)
	QFont font("simsun",9,QFont::Normal,false);
	app.setFont(font);
#endif
	if(dbex.Open(DB_OP_WR,FALSE,TRUE)==0)
	{
		DMessageBox::critical(NULL,QObject::tr("错误提示"),QObject::tr("系统数据库连接失败，\r\n请检查数据库本机数据库客户端配置是否正确。"),
			QMessageBox::Ok,QMessageBox::NoButton);
		app.exit(0);
		return 0;
	}

	MainWindow mainWindow;
	mainWindow.showMaximized();
	return app.exec();
}
