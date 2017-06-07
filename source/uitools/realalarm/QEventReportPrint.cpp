#include "QEventPrint.h"
#include "globaldef.h"
#include <QMessageBox>

QEventReportPrint::QEventReportPrint(QEventCfg *pCfg, QEventPara *pEventPara)
{
	m_done = FALSE;
	m_cfg = pCfg;
	m_eventPara = pEventPara;
}

QEventPrint::~QEventPrint()
{
}

void QEventPrint::closePrinter()
{
	m_bOK = FALSE;
}

bool QEventPrint::startPrinter()
{
	if( m_printer.isEmpty() )
		return FALSE;
	
	if( m_hPrinter != NULL )
		return TRUE ;

	qWarning( _Q2C(m_printer) );
	m_hPrinter = fopen( _Q2C(m_printer), "wt" );

	if( m_hPrinter == NULL )
	{
		return FALSE;
	}

	return TRUE;
}







bool QEventPrint::SetPrinter( const QString & printer )
{
	TRY_LOCK;
	if( printer.isEmpty() )
		return FALSE;

    m_done = TRUE;
	m_bOK = FALSE;
	if( m_hPrinter != NULL )
	{
		fclose( m_hPrinter );
	}
	m_hPrinter = NULL;
	m_printer = printer;

	if(!m_printer.isEmpty() && startPrinter()) 
	{
		printHead();
		closePrinter();
		return TRUE;
	}
	else
		return FALSE;
}

void QEventReportPrint::addEvent( QSYS_EVENT * e )
{
	TRY_LOCK;
	QSYS_EVENT * pE = new QSYS_EVENT;
	*pE = *e;
	m_list.append(pE);
}

int QEventPrint::getEventText(int row, QSYS_EVENT * e, QString & msg)
{
	QString station = _C2Q( e->Object ) ;
	if ( m_cfg.getReportName( station ).count() != 0 )
	{
		int16u pType = e->type ;
		QString ymd = QString::number( e->ymd ) ;
		QString pTypeStr ;
		QStringList typeStrList = m_cfg.getReportEventType( station ) ;
		QStringList nameStrList = m_cfg.getReportName( station ) ;
		int index = typeStrList.indexOf( pTypeStr ) ;
		if ( index )
		{
			QString reportName = nameStrList[ index ] ; 
// 			QString program = _C2Q( "java" ) ;
// 			QString reportjar = QString( getenv( NBENV ) ) + _C2Q( "/bin/report.jar" ) ;
			int Id = 0 ;
			m_eventPara.getReportId( reportName , Id ) ;
			QString ymdStr = QString( "%1,%2,%3" ).arg( ymd.left( 4 ) ).arg( ymd.mid( 5 , 2 ) ).arg( ymd.right( 2 ) ) ;
// 			QStringList arguments;
			QString psubStr = _C2Q("/%1,0,%2,%3").arg( Id ).arg( ymdStr ).arg( _C2Q( "autoprint ") );
// 			arguments << "-jar" << reportjar << psubStr ;
			msg = psubStr ;
			return 1 ;
		}
		else
		{
			return 0 ;
		}
	}
	else
	{
		return 0 ;
	}
}

void QEventPrint::print()
{
	if( m_list.count() == 0 )
		return ;

	static int nCount = 0;
	char buf[1024];
	char time[1024];
	
	while(!m_list.isEmpty()) 
	{
		QSYS_EVENT eve;		
		{
			TRY_LOCK;
			QSYS_EVENT * e = m_list.first();
			if(!e) 
			{
				int i = m_list.indexOf(e);
				if ( i != -1 )
					delete m_list.takeAt( i ) ;
				continue;
			}
			else 
			{
				eve = *e;
				int i = m_list.indexOf(e);
				if (i != -1)
					delete m_list.takeAt(i);
			}
		}

//		getEventText(++nCount, &eve, msg);

		++nCount;
		
		if(!print(buf))
			break;
	}

	closePrinter();
}