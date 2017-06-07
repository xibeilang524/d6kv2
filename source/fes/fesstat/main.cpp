#include <QApplication>
#include <QtCore/QTextCodec>
#include <QWindowsStyle>
#include <stdio.h>
#include "fesstat.h"
#include "verapi.h"

int main( int argc, char* argv[] )
{
	//Version
	if( argc > 1 )
	{
		for( int i=1; i<argc; i++ )
		{
			if( strcmp( "--version", argv[i] ) == 0 )
			{
				printf( "fesstat 1.2" );
				writever( "fesstat,1.2" );
				return 0;
			}
		}
	}
	QApplication app( argc, argv );
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale(  ) );
	
	QFesStat *pFepStat;
	pFepStat = new QFesStat( NULL );
	pFepStat->show(  );
	
	return app.exec(  );
}
