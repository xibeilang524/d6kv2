//#include <stdafx.h>
#include <signal.h>
#include "net/netdef.h"
#include "scd/scdapi.h"
#include "rdb/rdbapi.h"
#include "verapi.h"
// #include "fes/fesscdshm.h"

typedef struct {
  int   groupno;
  int   terminalno;
  int   yxno;
  int   state;
  } YXENTPARA;


void  test_yx_ent(int argc,char *argv[])
{
int  i,yxnums;
YXENTPARA   *yxentpara;
//SCADA_RAWEVENT_STRU   *TERMINALNo;
FE_DATA_EVENT_NODE* TERMINALNo;
ScadaServer  tmpscd;
RDB_DATETIME_MS_STRU     curclock;
Rdb_Time       rdbtime;

if(argc<5 || (argc-1)%4!=0) {
    printf("\n    命令行错误\n");
    printf("\n    使用方法: evtsim <分组序号> <终端号> <点号> <测试值 0/1> ....\n");
    exit(1);
 }


yxnums=(argc-1)/3;
yxentpara=(YXENTPARA *)malloc(yxnums * sizeof(YXENTPARA));
TERMINALNo=(FE_DATA_EVENT_NODE  *)malloc(yxnums * sizeof(FE_DATA_EVENT_NODE));

for(i=0;i<yxnums;i++) {
  yxentpara[i].groupno=atoi(argv[i*4+0+1]);
  yxentpara[i].terminalno=atoi(argv[i*4+1+1]);
  yxentpara[i].yxno=atoi(argv[i*4+2+1]);
  yxentpara[i].state=atoi(argv[i*4+3+1]);
 }

rdbtime.RdbGetCurClock(&curclock);


for(i=0;i<yxnums;i++) {
rdbtime.RdbGetCurClock(&curclock);
 TERMINALNo[i].time.year=curclock.year-1900;
 TERMINALNo[i].time.month=curclock.month;
 TERMINALNo[i].time.day=curclock.day;
 TERMINALNo[i].time.hour=curclock.hour;
 TERMINALNo[i].time.minute=curclock.minute;
 TERMINALNo[i].time.second=curclock.second;
 //TERMINALNo[i].reserve=1;
 TERMINALNo[i].time.milisecond=curclock.msecond;
 TERMINALNo[i].source=2;
 //TERMINALNo[i].groupno=yxentpara[i].groupno;
 TERMINALNo[i].rtuno=yxentpara[i].terminalno;
 TERMINALNo[i].val.yx.order=yxentpara[i].yxno;
 TERMINALNo[i].val.yx.value=yxentpara[i].state;
 }

printf("\n time : %d-%d-%d:%d\n",curclock.hour,curclock.minute,curclock.second,curclock.msecond);
tmpscd.ForeSendRawEventToScd("scada",TERMINALNo,yxnums);
RdbSleepms(1,0);

for(i=0;i<yxnums;i++) {
rdbtime.RdbGetCurClock(&curclock);
TERMINALNo[i].time.year=curclock.year-1900;
TERMINALNo[i].time.month=curclock.month;
TERMINALNo[i].time.day=curclock.day;
TERMINALNo[i].time.hour=curclock.hour;
TERMINALNo[i].time.minute=curclock.minute;
TERMINALNo[i].time.second=curclock.second;
//TERMINALNo[i].reserve=1;
TERMINALNo[i].time.milisecond=curclock.msecond;
 TERMINALNo[i].source=1;
 TERMINALNo[i].rtuno=yxentpara[i].terminalno;
 TERMINALNo[i].val.yx.order=yxentpara[i].yxno;
 TERMINALNo[i].val.yx.value=yxentpara[i].state;
 }

tmpscd.ForeSendRawEventToScd("scada",TERMINALNo,yxnums);
}


int main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("evtsim,1.1");
				printf("evtsim, 1.1");
				return 0;
			}
		}
	}

#ifdef RDB_PLATFORM_UNIX
	sigset(SIGUSR1,SIG_IGN);
#endif
	NetDbg dbg;
	dbg.RegProc("evtsim");
	test_yx_ent(argc,argv);
	dbg.UnRegProc();
	return(0);
}

/*
{
RdbTime    rdbtime;
RDB_CLOCK  curclock;
event_proc  tmpep;
SYS_EVENT   sysevent;
int   value=1;

rdbtime.GetCurrentClock(&curclock);
memset(&sysevent,0,sizeof(sysevent));
sysevent.DispOut=1;
sysevent.event_sort=EVENT_SCADA;
sysevent.event_type=ENTTYPE_BREAKSOE;
sysevent.ymd=curclock.year*10000+curclock.month*100+curclock.day;
sysevent.hmsms=curclock.hour*10000000+curclock.minute*100000+
		curclock.second*1000+curclock.msecond;
sysevent.state=value;
strcpy(sysevent.group_name,"TEST");
strcpy(sysevent.member_name0,"yxdck_01");
strcpy(sysevent.char_info,"TEST ENT");
tmpep.save_event(&sysevent,1);
}
*/
