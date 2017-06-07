#include <QApplication>
#include <QTextCodec>
#include <QWindowsStyle>
#include "stdio.h"
#include "fespara.h"
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
				printf( "fespara, 1.2" );
				writever( "fespara,1.2" );
				return 0;
			}
		}
	}
	QApplication app( argc, argv );
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale(  ) );

	QFesPara *pFesPara;
	pFesPara = new QFesPara( NULL );
	pFesPara->show(  );

	return app.exec(  );
}
