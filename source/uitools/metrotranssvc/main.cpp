#include <QApplication>
#include <QTextCodec>

#include "globaldef.h"
#include "fileinfo.h"
#include "manageevent.h"
#include "mainsvc.h"

#include "verapi.h"


int g_flagDebug = TRUE;
int gbNeedStart = FALSE;
int bDaQuit = FALSE;
int bDebug = FALSE;

int GroupNo = 0;
bool isBeginControl = FALSE;

File_info fileInfo;
Mng_event Event;


int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("metrotranssvc,1.0.0\n");
				printf("metrotranssvc,1.0.0\n");
				return 0;
			}
		}
	}

	ProcMan	procMan;

	if( procMan.IsProcExist( "metrotranssvc" ) ) return 0;

	NetDbg dbg;

	dbg.RegProc("地铁转发遥控进程");

	procMan.StartProc("metrotranssvc","地铁转发遥控进程","supernb");

	Metro_trans_svc* mainsvc = new Metro_trans_svc;
	mainsvc->Init();
	while(true)
	{
		if (!procMan.CheckParentProc())
			break;
		_delay_time(1*1000*1000);
	}
	procMan.EndProc();
	dbg.UnRegProc();
	exit(0);
}