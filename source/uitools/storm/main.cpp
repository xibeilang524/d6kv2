#include <mainwindow.h>
#include <QApplication>
#include <QTextCodec>
#include <QWindowsStyle>

#include "verapi.h"

 #ifdef _WIN32
 #include <WINSOCK2.H>
 #endif

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("storm,1.51.1");
				printf("storm, 1.51.1");
				return 0;
			}
		}
	}

#ifdef _WIN32
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return FALSE;
    }
#endif

	QApplication app(argc, argv);
// 	app.setStyle( new QWindowsStyle );
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	mainWindow window;
	window.show();
	return app.exec();

}