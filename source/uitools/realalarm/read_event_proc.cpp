	// QReadEventProc.cpp: implementation of the QReadEventProc class.
//
//////////////////////////////////////////////////////////////////////

#include "read_event_proc.h"
#include <QApplication>
#include "sysstr.h"
#include "net/netapi.h"
#include "globaldef.h"
#include "event_filter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void saveFileName( SYS_EVENT *eve )
{
	FILE *pfile;
	char * env = getenv("NBENV");
	char filename[ 1024 ];

	sprintf( filename, "%s/log/dat/evelog.txt", env );

	pfile = fopen( filename, "a+" );

	ulong lYmd = (ulong)(eve->ymd);
				int Year=(int)(lYmd/(100l*100l));
				int Month=(int)((lYmd%(100l*100l))/100);
				int Day=(int)((lYmd%(100l*100l))%100);

				ulong lTmp = (ulong)(eve->hmsms);
				int Hour = (int)(lTmp/(100l*100l*1000l));
				int Min = (int)((lTmp%(100l*100l*1000l))/(100l*1000l));
				int Sec = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))/1000l);
				int Ms = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))%1000l);


	fprintf( pfile, "%04d.%02d.%02d--%02d:%02d:%02d:%03d %s,DispOut=%d\n", Year,Month,Day,Hour, Min, Sec, Ms, 
		eve->text, eve->DispOut );

	fclose( pfile );
}

void Read_event_proc::stop()
{
    TRY_LOCK;
    m_done = TRUE;
}

void Read_event_proc::pause()
{
	m_pause = !m_pause;
}

void Read_event_proc::run()
{
	static EventProc eventproc;
    bool stop = FALSE;
	m_pause = FALSE;

	{
		TRY_LOCK;
		stop = m_done;
	}
	while( !stop )
	{
 		SYS_EVENT event ;
		memset(&event, 0, sizeof(SYS_EVENT));
		while( !m_pause && eventproc.Read( &event ) )
		{
			if( event.sort == 0 ) 
			{
				memset(&event, 0, sizeof(SYS_EVENT));
				TRY_LOCK;
				stop = m_done;
				continue;
			}
			
			//saveFileName( sevent );
// 			memset(&sevent, 0, sizeof(Rsys_event));
			Rsys_event *sevent = new Rsys_event(&event);
			//memset(&sevent, 0, sizeof(SYS_EVENT));
			//sevent = event;
			emit signal_send_event( sevent ,QMSG_NEW_EVENT);

// 			QCustomEvent ce(QMSG_NEW_EVENT);
// 			ce.setData(&event);
// 			qApp->notify( m_receiver, &ce );
			//saveFileName( sevent );

			memset(&event, 0, sizeof(SYS_EVENT));
			TRY_LOCK;
			stop = m_done;
		}

		{
			TRY_LOCK;
			stop = m_done;
		}
		msleep( 20 );
	}
}
