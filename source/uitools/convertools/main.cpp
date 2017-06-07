#include <QApplication>
#include <QTextCodec>

#include "convertools.h"

int main(int argc, char* argv[] )
{
	QApplication a(argc , argv);

	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale() );

	Convertools window;
	window.show();
	return a.exec();
}