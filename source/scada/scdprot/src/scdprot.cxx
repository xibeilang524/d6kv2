#include <stdio.h>
#include "scadainterface.h"
#include "net/netapi.h"
#include "scadaproc.h"
#include "procthread.h"
#include "protect.h"
#include "verapi.h"


// 将 ScadaServaice 的结果打成邮件发回给请求服务的对象（HostName, UserName, TaskID）
int  send_scadaservice_err(int groupno,char *hm,char *um,hlong tid,int servicetype,int errorcode)
{
    SCADA_ERR_STRU  scdserr;
    scdserr.servicetype=servicetype;
    scdserr.errorcode=errorcode;
    ScdPrintError(errorcode,scdserr.errorinf);	// 将服务结果代码转换为相应文字信息
	scdnetbyte(RDB_DATATYPE_INT,&scdserr.errorcode);
	scdnetbyte(RDB_DATATYPE_SHORT,&scdserr.servicetype);
    return(SendScdResultMail(groupno,hm,um,tid,SCADA_TYPE_ERRRET,&scdserr,sizeof(scdserr)));
}


// 处理邮件中的请求服务（ScadaService任务）
int  proc_scadaservice(char *hm,char *um,char *pd,hlong tid,int servicetype,void *scdsbuf,int sbuflen)
{
	int ret;
	NetDbg dbg;
	
	Protect_Scdservice_serv  scadaservice(hm,um,pd,tid);
	RdbDebugMesg("-------start to get prot mail from fert-------\n");	
	switch(servicetype) {
		case	SCADA_TYPE_PROT:
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理保护邮件");
			ret = scadaservice.yieldprotectinfo((SCADA_PROT_STRU*)scdsbuf,sbuflen);
			if(ret == PROT_ERR_MAILLEN) RdbDebugMesg("保护邮件长度错误");
			else if(ret == PROT_ERR_MODULE) RdbDebugMesg("未得到保护模块信息\n");
			else if(ret == PROT_ERR_MESSAGE) RdbDebugMesg("无该微机保护信息\n");
			else if(ret == PROT_ERR_ACTION) RdbDebugMesg("保护动作信息错误\n");
			else if(ret == PROT_ERR_DETECT) RdbDebugMesg("保护自检信息错误\n");
			else if(ret == PROT_ERR_TROUBLE)	RdbDebugMesg("保护故障信息错误\n");
			else if(ret == PROT_ERR_NOTSUPPORT)	RdbDebugMesg("不支持的保护类型\n");
			else if(ret == OK)			RdbDebugMesg("生成微机保护事项信息\n");
			break;
		default:
			RdbDebugMesg("未收到保护事件邮件\n");
	}
	RdbDebugMesg("-------end to get mail prot from fert-------\n");
	return(OK);
}

// 处理Scada邮件级别为2的线程
// 调用 RecvScdRequestMail 函数取出邮件
#if defined WIN32
UINT proc_scadamail_level2(LPVOID pParam)
#else
void * proc_scadamail_level2(void *argp)
#endif
{
	hlong	scdsbuf[1000]; 
	uhlong	tid; 
	int		tmplen,servicetype;    
	char    hostname[HOSTNAME_LEN];
	char    username[USERNAME_LEN];
	char    password[PASSWORD_LEN];
	NetDbg dbg;

	for (;;) 
	{
		if ((servicetype=RecvScdRequestMail(2,hostname,username,password,&tid,(void *)scdsbuf,&tmplen))!=SCADA_TYPE_NULL) 
		{
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t服务器收到邮件，级别2");
			proc_scadaservice(hostname,username,password,tid,servicetype,scdsbuf,tmplen);
		}
		else sleepms(150);
	}
	return NULL;
}



/////////////////////////////

#ifdef __unix
int main(int argc,char **argv)
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("scdprot,1.51.1");
				printf("scdprot, 1.51.1");
				return 0;
			}
		}
	}
	uhlong   timecount=0;

	ProcMan  procman;

	procman.StartProc("scdprot","保护事件处理进程","",NULL);

	NetDbg  dbg;
	dbg.RegProc("保护事件处理");

	if (!procman.CheckParentProc())	{
		printf("scadaprot exit : procman.CheckParentProc returning 0\n");
		exit(1);
	}
	//  scada service  
	scd_pthread_create(proc_scadamail_level2,NULL,100);

    for (;;) {
		if (!procman.CheckParentProc())	{
			printf("scadaproc exit: procman.CheckParentProc returning 0\n");
			exit(1);
		}
		timecount++;
		sleepms(1,0);
    } 

	dbg.UnRegProc();
    return 0;
}
#endif

 