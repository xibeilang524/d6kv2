#include "alarmthread.h"
#include "net/netapi.h"

extern NetDbg		net_dbg;
extern ProcMan		proc_man;
extern int live_flag  ;

AlarmThread::AlarmThread( QObject * parent /*= 0 */ ) : QThread( parent )
{
}

void AlarmThread::run()
{
	while( live_flag )
	{
		if(!proc_man.CheckParentProc())
		{
			live_flag = 0;
			break;
		}
		_delay_time(1000 * 1000);
	}

	proc_man.EndProc();
	proc_man.UnRegProc();
	exit( 0 ) ;
}