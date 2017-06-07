#include <QtGui>
#include "rawviewwnd.h"
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
				printf( "rawview, 1.2" );
				writever( "rawview,1.2" );
				return 0;
			}
		}
	}
	QApplication app( argc, argv );
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "GBK" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForLocale(  ) );
	QRawViewWnd* rawViewWnd = new QRawViewWnd( NULL );

	rawViewWnd->show(  );
	return app.exec(  );
}