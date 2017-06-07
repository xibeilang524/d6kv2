#include "scadaproc.h"
#include "db/dbapi.h"
#include "rdb/rdbapi.h"
#include "stdio.h"
#include "scadahislib.h"

#include "procthread.h"
#include "pdr.h"
#include "verapi.h"

int pdrgroup_initflag;
PDRGROUP_MANINFO  pdrgrp_maninfo[MAXPDRGROUPNUMS];

int IsScadaServer(void)
{
	NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
	int groupnum=netstat.GetHostGrpNum();
	for(int i=0;i<groupnum;i++){
		if(netstat.IsServer(host_group_no[i],SCD_SERVER))
		{
			return 1;
		}
    }
	return 0;
}

//遇到ENT_STARTPDR_STRU事件调用该函数启动事故追忆
int startpdr(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	intertime	curtime;
	ENT_STARTPDR_STRU pdrmsg;
	char    tmpcause[1000];
	NetDbg dbg;
	memcpy(&pdrmsg,apentmsg,sizeof(ENT_STARTPDR_STRU));

	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"PDR:\t启动追忆\n");

	if(pdrmsg.zygrpno<0 || pdrmsg.zygrpno>=MAXPDRGROUPNUMS) return -1;
	if(!pdrgrp_maninfo[pdrmsg.zygrpno].availflag)return(-1);

	PdrSaveTaskMan	pstm;

	if(pdrmsg.causetype<2)GetInterTime(&curtime);
	else curtime=pdrmsg.timeflag;

	if(pdrmsg.causetype==1 || pdrmsg.causetype==0) {
		sprintf(tmpcause,"%s %s",pdrmsg.cause,pdrgrp_maninfo[pdrmsg.zygrpno].groupname);
		tmpcause[sizeof(pdrmsg.cause)-1]='\0';
		strcpy(pdrmsg.cause,tmpcause);
	}

	if(pdrmsg.causetype==1)
	pstm.AddPdrSaveTask(pdrmsg.zygrpno,curtime,pdrmsg.breakname,&pdrmsg.clock,
			pdrmsg.causetype,pdrmsg.cause,pdrgrp_maninfo[pdrmsg.zygrpno].preframenums,
			pdrgrp_maninfo[pdrmsg.zygrpno].bakframenums);
	else 
	pstm.AddPdrSaveTask(pdrmsg.zygrpno,curtime,pdrmsg.breakname,&pdrmsg.clock,
			pdrmsg.causetype,pdrmsg.cause,pdrmsg.preframenums,
			pdrmsg.postframenums);

	return 0;
}



//处理请求启动追忆应用事件线程
#ifdef WIN32 
UINT PdrCheckStartMsg(void *argp)
#else 
void *PdrCheckStartMsg(void *argp)
#endif
{
	APEvent apevent;
	apevent.AddApEventCallback(APEVENT_STARTPDR,startpdr,NULL,APEVENT_NULL);
	apevent.ApEventLoop();
	return NULL;
}




//检查追忆队列线程---200毫秒定时检查追忆队列
#ifdef WIN32
UINT PdrCheckSaveTask(LPVOID)
#else
void *PdrCheckSaveTask(void *argp)
#endif
{
	PdrSaveTaskMan	pstm;
	int timecount=0;

	for(;;) {
		sleepms(500);
		pstm.ProcAllSaveTask();
		/*
		timecount++;
		if(timecount==20) {
			int curtime;
			SYS_CLOCK  curclock;
			GetCurrentClock(&curclock);
			GetInterTime(&curtime);
			pstm.AddPdrSaveTask(0,curtime,"Kzf500Cb",&curclock,
			1,"aaaaa",pdrgrp_maninfo[0].preframenums,
			pdrgrp_maninfo[0].bakframenums);
		}
		*/

	}
	return NULL;
}

//检查追忆队列线程---200毫秒定时检查追忆队列
#ifdef WIN32
UINT PdrDataFreshTask(LPVOID)
#else
void *PdrDataFreshTask(void *argp)
#endif
{
	PdrGroupMan  pdrgrpman;

	pdrgrpman.UpdAllPdrPara();
    for(;;)
    {
		pdrgrpman.ProcAllPdrGroup();
		sleepms(1,0);
    }
    return(NULL);
}

//检查追忆队列线程---200毫秒定时检查追忆队列
#ifdef WIN32
UINT PdrDataSaveToDB(LPVOID)
#else
void *PdrDataSaveToDB(void *argp)
#endif
{
	PdrDBTaskMan pdbtaskm;

    for(;;)
    {
		pdbtaskm.ProcAllDBTask();
		sleepms(1,0);
    }
    return(NULL);
}

//检查追忆队列线程---200毫秒定时检查追忆队列
#ifdef WIN32
UINT PdrParaUpdCheck(LPVOID)
#else
void *PdrParaUpdCheck(void *argp)
#endif
{
	int entcode,updflag=0;
	PdrGroupMan  pdrgrpman;
	Rdb_EventNoUser rdbenu;
	RDB_EVENT_LOADTAB_STRU  *loadmsg;
	for(;;) {
		entcode=rdbenu.RdbGetEvent(NULL,NULL,(void **)&loadmsg);
		if(entcode==RDB_EVENT_LOADTAB) {
			if(strcmp(loadmsg->tabname,ANALOG_TABNAME)==0 ||
				strcmp(loadmsg->tabname,BREAK_TABNAME)==0 ||
				strcmp(loadmsg->tabname,SWITCH_TABNAME)==0 ||
				strcmp(loadmsg->tabname,PDRPARA_TABNAME)==0 ||
				strcmp(loadmsg->tabname,PDRGRP_TABNAME)==0) {

				pdrgrpman.UpdAllPdrPara();

			}
		}
		if(entcode==RDB_EVENT_NULL)sleepms(1,0);
	} 
	return(NULL);
}


#ifdef __unix
void  *PdrCheckNetbus(void *argp)
{
    ProcMan proc;
    for(;;)
    {
		if (!proc.CheckParentProc()) {
			RdbDebugMesg("pdr exit : CheckParentProc returning 0\n");
			exit(1);
		}
		sleepms(1,0);
    }
    return(NULL);
}
#endif


//主进程:定时追忆所有追忆组的数据；
#ifdef __unix
int main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("scdpdr,1.51.1");
				printf("scdpdr, 1.51.1");
				return 0;
			}
		}
	}

	ProcMan proc;
	proc.StartProc("scdpdr", "事故追忆进程", "",NULL);

	NetDbg dbg;
	dbg.RegProc("事故追忆进程");
 
    if(!IsScadaServer()){
		printf("\n本机不是SCADA服务器!!!\t请将本机配置为SCADA服务器后运行\n");
		return 0;
	}
	//启动历史数据库
	int m;
    DBApi dbapi;

	for(m=0;m<4;m++) {
		if(dbapi.Open(DB_OP_W,FALSE,TRUE)==CS_SUCCEED)break;
		sleepms(2,0);
	}
	if(m>=4)return 1;

	scd_pthread_create(PdrDataFreshTask,NULL,100);		//追忆请求消息处理线程
	scd_pthread_create(PdrCheckStartMsg,NULL,100);		//定时检查追忆队列线程
	scd_pthread_create(PdrCheckSaveTask,NULL,100);
	scd_pthread_create(PdrCheckNetbus,NULL,100);
	scd_pthread_create(PdrParaUpdCheck,NULL,100);
	PdrDataSaveToDB(NULL);

	dbapi.Close();
	dbg.UnRegProc();
    return 0;
}
#endif

