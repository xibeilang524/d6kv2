// QEventPrint.cpp: implementation of the QEventPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "event_print.h"
#include "globaldef.h"
#include <QMessageBox>




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int gLen[] =
{
	28,//LIST_COL_SORT,
	20,//LIST_COL_TYPE,
	25,//LIST_COL_TIME,
	48,//LIST_COL_DES,
	7,//LIST_COL_FLOAT_VAL,
	7,//LIST_COL_STATUS_VAL,
	16,//LIST_COL_GROUP_NAME,
	28,//LIST_COL_MEMBER_NAME0,
	28,//LIST_COL_MEMBER_NAME1,
	28,//LIST_COL_MEMBER_NAME2,
	28,//LIST_COL_MEMBER_NAME3,
	40,//LIST_COL_GRAPH_NAME,
	80,//LIST_COL_TONE_INFO,
	10,//LIST_COL_SECU_LEV,
	10,
	10,
	10,
	10,
	10//LIST_COL_NUM
};

Event_print::Event_print(Event_cfg *pCfg, Event_para *pEventPara)
{
	m_hprinter = NULL;
	m_done = FALSE;
	m_cfg = pCfg;
	m_event_para = pEventPara;
	m_line_len = 0;
// 	m_list.setAutoDelete( true );
	m_printer = QObject::tr( "LPT1" );
}

Event_print::~Event_print()
{
}

void Event_print::close_printer()
{
	if( m_hprinter )
		fclose( m_hprinter );
	m_bOK = FALSE;
	m_hprinter = NULL;
}

bool Event_print::start_printer()
{
	if( m_printer.isEmpty() )
		return FALSE;
	
	if( m_hprinter != NULL )
		return TRUE ;

	qWarning( _Q2C(m_printer) );
	m_hprinter = fopen( _Q2C(m_printer), "wt" );

	if( m_hprinter == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

bool Event_print::print_head()
{
/*	m_nLineLen = 6;
	int max = -1;
	QMap<int, int> ords;
	for( int i = 0 ; i < LIST_COL_NUM; i ++ )
	{
		uint disporder;
		if( !m_cfg->getColDispOrder( i, disporder ) )
			continue;

		m_nLineLen += gLen[i]+1;
		ords[disporder] = i;
		if(max<(int)disporder)
			max = disporder;
	}
*/
	char buf[1024];
	memset( buf , 0 , sizeof(buf) );

//	QString buf;
//	buf.sprintf("%-6s", tr("序号"));
//	buf.sprintf("%-6s", "序号");
	sprintf( buf , "%-10s%-30s%-48s" ,  "序号" , "事项时间" , "事项描述");

/*	for(i=0;i<=max;i++) 
	{
		if(ords.contains(i)) 
		{
			int who = ords[i];
			char fmt[10];
			sprintf(fmt, "%%-%ds", gLen[who]+1);

			//QString tmp;
			//tmp.sprintf(fmt, _Q2C(m_cfg->getColName(i)) );
			//buf += tmp;
			memset( tmp , 0 , sizeof(tmp) );
			sprintf(tmp , fmt , _Q2C(m_cfg->getColName(i)) );
			strcat( buf , tmp );
		}
	}
*/

	strcat( buf , "\r\n" );
//	buf += "\r\n";

	qWarning(buf);

	if(!print(buf) )
		return FALSE;
		
	// start line
	return print_line();
}

bool Event_print::print_line()
{
	char buff[ 1024 ];

	memset( buff , 0 , sizeof(buff) );
	int i;
	for( i=0;i<100;i++)
		buff[i] = '-';
	buff[i++] = '\r';
	buff[i++] = '\n';
	buff[i++] = '\0';
	bool bOk = print(buff);

	return bOk;
}

bool Event_print::print(const char * txt)
{
	if(txt == NULL 
		|| txt[0] == '\0' 
		|| m_hprinter==NULL)
	{
		return FALSE;
	}

	qWarning(txt);
	fprintf( m_hprinter , "%s", txt );
	fflush( m_hprinter );

	return TRUE;	
}

bool Event_print::set_printer( const QString & printer )
{
	TRY_LOCK;
	if( printer.isEmpty() )
		return FALSE;

    m_done = TRUE;
	m_bOK = FALSE;
	if( m_hprinter != NULL )
	{
		fclose( m_hprinter );
	}
	m_hprinter = NULL;
	m_printer = printer;

	if(!m_printer.isEmpty() && start_printer()) 
	{
		print_head();
		close_printer();
		return TRUE;
	}
	else
		return FALSE;
}

void Event_print::add_event( Rsys_event * e )
{
	TRY_LOCK;
	Rsys_event * pE = new Rsys_event;
	*pE = *e;
	m_list.append(pE);
}

int Event_print::get_event_text(int row, Rsys_event * e, QString & msg)
{
	QStringList ary;
	int max = -1;
	QMap<int, int> ords, mapToStringlist;

	const char *col_des[ ] = {
		"否",
		"是"
	};

	int i;
	for( i = 0 ; i < LIST_COL_NUM; i ++ )
	{
		uint disporder;
		if( !m_cfg->get_col_disp_order( i, disporder ) )
			continue;

		ords[disporder] = i;
		if(max<(int)disporder)
			max = disporder;

		QString text;
		switch( i )
		{
		/*case LIST_COL_SORT:
			{
				m_eventPara->getEventSortDesc( e->sort, text );
			}
			break;
		case LIST_COL_TYPE:
			{
				m_eventPara->getEventTypeDesc( e->type, text );
			}
			break;*/
		case LIST_COL_TIME:
			{
				ulong lYmd = (ulong)(e->ymd);
				int Year=(int)(lYmd/(100l*100l));
				int Month=(int)((lYmd%(100l*100l))/100);
				int Day=(int)((lYmd%(100l*100l))%100);

				ulong lTmp = (ulong)(e->hmsms);
				int Hour = (int)(lTmp/(100l*100l*1000l));
				int Min = (int)((lTmp%(100l*100l*1000l))/(100l*1000l));
				int Sec = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))/1000l);
				int Ms = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))%1000l);
				text.sprintf( "%04d.%02d.%02d--%02d:%02d:%02d:%03d",Year,Month,Day,Hour, Min, Sec, Ms);
			}
			break;
		case LIST_COL_DES:
			{
				text = QObject::tr( e->text );
			}
			break;
		/*case LIST_COL_FLOAT_VAL:
			{
				text.sprintf( "%.2f",  (float)e->val );
			}
			break;
		case LIST_COL_STATUS_VAL:
			{
				text.sprintf( "%d",  (uint)e->state ); //状态值
			}
			break;
		case LIST_COL_GROUP_NAME:
			{
				m_eventPara->getStaDesc( _C2Q( e->Object ),	text );
			}
			break;
		case LIST_COL_MEMBER_NAME0:
			if( !m_eventPara->getYcDesc( _C2Q( e->member0 ),text ) && 
				!m_eventPara->getYxDesc( _C2Q( e->member0 ),text ) )
			{
				text = e->member0;
			}
			break;
		case LIST_COL_MEMBER_NAME1:
			{
				text = _C2Q( e->member1 );
			}
			break;
		case LIST_COL_MEMBER_NAME2:
			{
				text = _C2Q( e->member2 );
			}
			break;
		case LIST_COL_MEMBER_NAME3:
			{
				text = _C2Q( e->member3 );
			}
			break;
		case LIST_COL_GRAPH_NAME:
			{
				text = _C2Q( e->graph );
			}
			break;
		case LIST_COL_TONE_INFO:
			{
				text = _C2Q( e->tone );
			}
			break;
		case LIST_COL_SECU_LEV:
			{
				text.sprintf( "%d",  (uint)e->security );
			}
			break;
		case LIST_COL_PRINT_FLAG:
			{
				text = _C2Q( col_des[ e->PrintOut%2 ] );  		
			}
			break;
		case LIST_COL_SAVE_FLAG:
			{
				text = _C2Q( col_des[ e->DiskOut%2 ] );  		
			}
			break;
		case LIST_COL_GRAPH_FLAG:
			{
				text = _C2Q( col_des[ e->GraphOut%2 ] );  		
			}
			break;
		case LIST_COL_WAV_FLAG:
			{
				text = _C2Q( col_des[ e->DispOut%2 ] );  		
			}
			break;*/
		default:
			continue;
		}

		ary.append(text);
		ords[disporder] = i;
		mapToStringlist[disporder] = ary.count()-1;
	}

	msg.sprintf("%5d ", row);

	for(i=0;i<=max;i++) 
	{
		if( ords.contains( i ) ) 
		{
			int who = ords[i];
			int len = gLen[who]+1;
			int idx = mapToStringlist[i];
			QString token;
			if(idx>=0 && idx<ary.size())
				token = ary[idx];
			token += " ";
			int c = strlen(_Q2C(token));
			int j;
			for(j=c;j<len;j++)
				token += " ";
			msg += token;
		}
	}

	msg += "\r\n";

	return ary.count();
}

void Event_print::print()
{
	if( m_list.count() == 0 )
		return ;

	if(m_hprinter==NULL  && !start_printer())
	{
		return;
	}

	static int nCount = 0;
	char buf[1024];
	char time[1024];
	
	while(!m_list.isEmpty()) {

		Rsys_event eve;		
		{
			TRY_LOCK;

			Rsys_event * e = m_list.first();
			if(!e) 
			{
// 				m_list.remove(e);
				int i = m_list.indexOf(e);
				if (i != -1)
					delete m_list.takeAt(i);
				continue;
			}
			else 
			{
				eve = *e;
// 				m_list.remove(e);
				int i = m_list.indexOf(e);
				if (i != -1)
					delete m_list.takeAt(i);
			}
		}

//		getEventText(++nCount, &eve, msg);

		++nCount;
		ulong lYmd = (ulong)(eve.ymd);
		int Year=(int)(lYmd/(10000l));
		int Month=(int)((lYmd%(10000l))/100);
		int Day=(int)((lYmd%(10000l))%100);

		///YYYYMMDD
		ulong lTmp = (ulong)(eve.hmsms);
		int Hour = (int)(lTmp/(10000000l));
		int Min = (int)((lTmp%(10000000l))/(100000l));
		int Sec = (int)(((lTmp%(10000000l))%(100000l))/1000l);
		int Ms = (int)(((lTmp%(10000000l))%(100000l))%1000l);

		memset( time , 0 , sizeof(time) );
		sprintf( time , "%04d.%02d.%02d--%02d:%02d:%02d:%03d",Year,Month,Day,Hour,Min,Sec,Ms);

		memset( buf , 0 , sizeof(buf) );
		sprintf( buf , "%-10d%-30s%-48s\r\n" ,  nCount , time , eve.text );
		
		if(!print(buf))
			break;
	}

	close_printer();
}