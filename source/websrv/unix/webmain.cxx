
#include <stdio.h>
#include <stdlib.h>

#include "rdbapi.h"
#include "db.h"
#include "webstruct.h"
#include "p_process.h"
#include <sys/wait.h>
#ifdef APP_HAS_VERSION
#include "appfunc.h"
#endif

extern CDB dbo;
extern void getEnv(char *envvar);
extern void loadExcludeEvent();
extern void loadDevParaTable();
extern int scd_pthread_create(PTHREAD_CALLFUNC  pthfunc,void *argp,int stacksize,pthread_t *tid);
extern void * srvwatchthread(void *argp);
extern void * checkEvent(void *argp);
extern void delay_t(int n);

extern BOOL IsLittleEndian(void);

ProcMan proc;

extern int bEndFlag;
extern void killAllClient();

extern USEREVENT *puserevent;


pthread_t srvwahch_id,eventcheck_id;
void preexit()
{
	bEndFlag = true;
	pthread_join(eventcheck_id,NULL);
	killAllClient();
	pthread_join(srvwahch_id,NULL);
	while(puserevent)
	{
		USEREVENT *pevent = puserevent->puserevent;
		delete puserevent;
		puserevent = pevent;
	}	
	dbo.Close();	
	proc.EndProc();
}

void signal_function(int flag)
{	
	signal(flag,signal_function);
	if(flag==SIGTERM || flag==SIGKILL){
		perror("received SIGTERM\n\r");
		preexit();
		exit(0);
	}
	sigrelse(flag);
}



int main(int argc,char *argv[])
{
#ifdef APP_HAS_VERSION
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("websrv,1.31.1");
				printf("websrv, 1.31.1");
				return 0;
			}
		}
	}
#endif
	CProcInfo p_proc;
	
	sigset(SIGKILL,signal_function);

	if(signal(SIGTERM,signal_function)<0)
	{
		perror("SIGTERM");
		exit(0);	
	}

	int procid=getpid();
	if(p_proc.GetProcNum("websrv")>1) return 0;		//保证运行一个实例
	
	//printf("IsLittleEndian=%s\r\n",IsLittleEndian() ? "YES" : "NO");

	NetDbg m_netbug;
	
	m_netbug.RegProc("websrv");

	dbo.Open(DB_OP_WR);
	loadExcludeEvent();	//读表失败时,有内存泄露
	loadDevParaTable();
	
	scd_pthread_create(srvwatchthread,NULL,0,&srvwahch_id);//创建SOCKET守护线程
	scd_pthread_create(checkEvent,NULL,0,&eventcheck_id);//事件检测线程
	proc.StartProc("websrv","Web服务器","");
	    for (;;) {
		if(bEndFlag) break;
		proc.CheckParentProc();
		delay_t(2000000);
	    } 
	 m_netbug.UnRegProc(); 
	 exit(0);	
}
