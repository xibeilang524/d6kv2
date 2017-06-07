#ifndef _PROC_THREAD_H
#define _PROC_THREAD_H

#include  "scd/cmddef.h"

#if defined(WIN32)
_SCD_DLLPORT 
#endif
int	 IsMainServer(int groupno);		//判断是否是SCADA主服务器

//系统进程管理

#ifdef WIN32
_SCD_DLLPORT 
#endif
int IsProcessOk(void);			//判断母进程是否仍在运行

int ExecCmd(char *cmdstr);		//运行系统命令

typedef  void *(*PTHREAD_CALLFUNC)(void *);

#if defined(WIN32)
_SCD_DLLPORT HANDLE
#else
int 
#endif
scd_pthread_create(PTHREAD_CALLFUNC pthfunc,void *argp,int stacksize=0);//启动线程

   
//系统错误处理
#if defined(WIN32)
_SCD_DLLPORT 
#endif
void ScdPrintError(char *context,int errorcode);	

#if defined(WIN32)
_SCD_DLLPORT 
#endif
void ScdPrintError(int errorcode,char  *errinfo);

#endif
