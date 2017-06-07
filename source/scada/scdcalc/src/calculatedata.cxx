#include  <stdio.h>
#include  <stdlib.h>
#include  "db/dbapi.h"
#include  "scadaproc.h"
#include  "procthread.h"
#include  "scadainterface.h"
#include  "upexec.h"
#include "verapi.h"

/////////////
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

#ifdef __unix
void *proc_timer_userproc(void *argp)
{
User_Procedure  userproc;
ProcMan  procman;


for(;;) {
  userproc.ExecTimerProc();
  userproc.ExecDbChgProc();
  if (!procman.CheckParentProc()) {
        printf("scdcal exit : CheckParentProc returning 0\n");
        exit(1);
  }
  sleepms(1000);
  }
return(NULL);
}
#else
UINT proc_timer_userproc(void *argp)
{
	User_Procedure  userproc;
	ProcMan  procman;
	for(;;) 
	{
		userproc.ExecTimerProc();
		userproc.ExecDbChgProc();
		sleepms(500);
	}
	return(NULL);
}
#endif

////////////
#ifdef __unix
void *proc_hdbtimer_userproc(void *argp)
{
User_Procedure  userproc;

for(;;) {
  userproc.ExecHdbTimerProc();
  sleepms(5,0);
  }
return(NULL);
}
#else
UINT proc_hdbtimer_userproc(void *argp)
{
	User_Procedure  userproc;
	for(;;) 
	{
		userproc.ExecHdbTimerProc();
		sleepms(5,0);
	}
	return(NULL);
}
#endif


/*****************
int fieldupd_userproc(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *userprocp)
{
ENT_DBPUTFLD_STRU  *dbmsg;

dbmsg=(ENT_DBPUTFLD_STRU  *)apentmsg;
((User_Procedure *)userprocp)->ExecFldUpdProc(dbmsg->tabname,dbmsg->objectname,dbmsg->fieldname);
return(OK);
}


void *proc_fieldupd_userproc(void *argp)
{
User_Procedure  userproc;
APEvent     apevent;

apevent.AddApEventCallback(APEVENT_DBUPD,fieldupd_userproc,&userproc,NULL);
apevent.ApEventLoop();
return(NULL);
}

***************/


int stachange_userproc(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *userprocp)
{
ENT_STATUSCHG_STRU  *stamsg;

stamsg=(ENT_STATUSCHG_STRU  *)apentmsg;
((User_Procedure *)userprocp)->ExecStaChgProc(stamsg->tabname,stamsg->objectname);
return(OK);
}

#ifdef WIN32
UINT proc_stachange_userproc(void *argp)
{
	User_Procedure  userproc;
	APEvent     apevent;
	unsigned int i=0;

	apevent.AddApEventCallback(APEVENT_STACHG,stachange_userproc,&userproc,
							  APEVENT_MSTATUSCHG,stachange_userproc,&userproc,
							  NULL);
	for(;;)
	{
		i++;
		apevent.ApEventLoop(1);
		sleepms(100);
	}
	return(NULL);
}
#else
void *proc_stachange_userproc(void *argp)
{
	User_Procedure  userproc;
	APEvent     apevent;

	apevent.AddApEventCallback(APEVENT_STACHG,stachange_userproc,&userproc,
							  APEVENT_MSTATUSCHG,stachange_userproc,&userproc,
							  NULL);
	apevent.ApEventLoop();
	return(NULL);
}
#endif
//////////////
int sgalarm_userproc(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *userprocp)
{
ENT_BREAK_STRU  *breakmsg;

breakmsg=(ENT_BREAK_STRU  *)apentmsg;
if(breakmsg->sgf) {
   ((User_Procedure *)userprocp)->ExecAccidentAlarmProc(breakmsg->breakname);
   }
return(OK);
}


#ifdef WIN32
UINT proc_sgalarm_userproc(void *argp)
{
	User_Procedure  userproc;
	APEvent     apevent;
	unsigned int i=0;
	
	apevent.AddApEventCallback(APEVENT_BREAK,sgalarm_userproc,&userproc,NULL);
	for(;;)
	{
		i++;
		apevent.ApEventLoop(1);
		sleepms(100);
	}
	return(NULL);
}

#else
void *proc_sgalarm_userproc(void *argp)
{
	User_Procedure  userproc;
	APEvent     apevent;

	apevent.AddApEventCallback(APEVENT_BREAK,sgalarm_userproc,&userproc,NULL);
	apevent.ApEventLoop();
	return(NULL);
}
#endif

////////////////////////////////////////////////
int proc_formula_cc(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *userprocp)
{
	char  *sachomepath;
	char   errfilename[200];
	char   *errmesg;
	ENT_CCFORMULA_STRU  *apccformula;

	apccformula=(ENT_CCFORMULA_STRU *)apentmsg;
	sachomepath=getenv(NBENV);
	if(sachomepath==NULL) {
		return(-1);
	}
	
#ifdef WIN32
	STARTUPINFO si;
	PROCESS_INFORMATION	pi;
	memset(&si,0,sizeof(STARTUPINFO));
	si.cb=sizeof(STARTUPINFO);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	::CreateProcess(NULL,"formula",NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	sprintf(errfilename,"%s\\procscript\\计算表达式表.err",sachomepath);
#else
	system("formula");
	sprintf(errfilename,"%s/procscript/计算表达式表.err",sachomepath);
#endif
	int retlen=RdbGetFileLen(errfilename);

	if(retlen<=0)return(retlen);

	errmesg=new char[retlen+1];
	FILE  *errfp=fopen(errfilename,"rb");
	if(errfp!=NULL) {
		int ret=fread(errmesg,1,retlen,errfp);
		if(ret==retlen) {
			errmesg[retlen]='\0';
			SendScdResultMail(apccformula->groupno,apccformula->ophostname,apccformula->operatorname,
				apccformula->optaskid,SCADA_TYPE_CCFORMULA,errmesg,retlen+1);
		}
		fclose(errfp);
	}
	else
	delete [] errmesg;
	return(OK);
}




#ifdef WIN32
UINT proc_formula_exec(void *argp)
{
	Userpcexec  pce("",MyUserName,MyPassWord,0);
	APEvent     apevent;
	int			returncode;
	double		returnvalue;
	unsigned int i=0;

	STARTUPINFO si;
	PROCESS_INFORMATION	pi;
	memset(&si,0,sizeof(STARTUPINFO));
	si.cb=sizeof(STARTUPINFO);
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	::CreateProcess(NULL,"formula",NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);

	apevent.AddApEventCallback(APEVENT_CCFORMULA,proc_formula_cc,NULL,NULL);
	for(;;)
	{
		apevent.ApEventLoop(1);
		if(i%4==0) returncode=pce.call_pcexecfile("计算表达式表",&returnvalue);
		sleepms(1,0);
		i++;
	}
	return(NULL);
}

#else

void *proc_formula_exec(void *argp)
{
	Userpcexec  *pce = new Userpcexec("",MyUserName,MyPassWord,0);
	//Userpcexec  pce("",MyUserName,MyPassWord,0);
	APEvent     apevent;
	int			returncode;
	double		returnvalue;
	unsigned int i=0;

	system("formula");

	apevent.AddApEventCallback(APEVENT_CCFORMULA,proc_formula_cc,NULL,NULL);
	for(;;)
	{
		apevent.ApEventLoop(1);
		if(i%4==0) returncode=pce->call_pcexecfile("计算表达式表",&returnvalue);
		sleepms(1,0);
		i++;
	}
	return(NULL);
}
#endif



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
				writever("scdcalc,1.51.1");
				printf("scdcalc, 1.51.1");
				return 0;
			}
		}
	}

	ProcMan  procman;
	procman.StartProc("scdcalc","计算量处理进程","",NULL);

	if (!procman.CheckParentProc()) 
	{
			printf("scdcal exit : CheckParentProc returning 0\n");
			return(1);
	}
	NetDbg dbg;
	dbg.RegProc("计算量处理进程");

    if(!IsScadaServer()){
		printf("\n本机不是SCADA服务器!!!\t请将本机配置为SCADA服务器后运行\n");
		return 0;
	}
	//启动历史数据库
	int m;
    DBApi dbapi;

//	for(m=0;m<4;m++) {
	dbapi.Open(DB_OP_W,FALSE,TRUE);
//		sleepms(2,0);
//	}
//	if(m>=4)return 1;

	scd_pthread_create(proc_timer_userproc,NULL,300);  
	scd_pthread_create(proc_stachange_userproc,NULL,100);
	scd_pthread_create(proc_sgalarm_userproc,NULL,100);
	scd_pthread_create(proc_formula_exec,NULL,100);
	proc_hdbtimer_userproc(NULL);

	dbapi.Close();
	dbg.UnRegProc();
	return 0;
}
#endif
