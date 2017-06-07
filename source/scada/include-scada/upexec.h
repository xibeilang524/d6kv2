#ifndef _USERPROC_EXEC_H
#define _USERPROC_EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#include "uplang.h"
#include "scadaproc.h"

#define  PCOK    0

#define  PCEXECERR_OTHER           -999999
#define  PCEXECERR_FILE            -999998
#define  PCEXECERR_INPARA          -999997
#define  PCEXECERR_ARRAY           -999996

typedef struct {
      int stackp; //栈指针 
      void *stackpt[100]; //栈内容 void指针数组
   } PARASTACK;


typedef struct {
     char  tabname[TABNAME_LEN]; //表名
     char  objectname[OBJECTNAME_LEN];//对象名
     char  fieldname[FLDNAME_LEN];  //域名
     RDB_VALUETYPE_STRU  mnvalue;          //不同类型值联合
   } PCMNOP_VALUE;

#define  MAXPCMNOPNUMS  10

typedef struct {
      void * point;
      DATATYPE datatype;
   } OBJECT;


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Pcexec {
   char  procname[256]; //过程名
   PARASTACK     *parastack;    //堆栈
   OBJECT		 object;
   PCINSTRUMENT  *pcinstru;     //指令
   char          *dataaddr;     //数据指针
   uchar         crparastackf;  //创建参数栈标志
   CALLPARA      externinpara;  //外部输入参数
   int           mnopvalnums;   //模拟操作值个数
   PCMNOP_VALUE  mnopvalue[MAXPCMNOPNUMS]; //模拟操作值结构
   APEvent       *apeventp;     //应用事件指针
   uchar         crapeventf;    //创建应用事件标志
   int           apentcode;     //应用事件代码
   SYS_CLOCK     apentclock;    //应用事件时间
   uhlong        apentmsg[MAXAPENTMSG_LEN/sizeof(uhlong)+1]; //应用事件消息
   int  loadpcinstrument(FILE *fp,PROC_HEAD *phd);//加载过程指令
   int  callfunction(int  funcno,CALLPARA  *cpp,double *raddr); //调用功能号
   int  get_apevent(void); //取事件
   void reset_apevent(void); //复位事件指针
   int  callbasisfunc(int  funcno,CALLPARA  *cpp,double *raddr);
   int  call_hiscomnfunc(int  funcno,CALLPARA  *cpp,double *raddr);
   int  call_ykfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //调YK功能
   int  call_rsqlfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);//调SQL查询功能
   int  call_memtblfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);//调实时库函数
   int  call_apentfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //调应用事件回调函数
   int  call_hisdataopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //调历史数据处理函数
   int  call_histableopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr); //调历史数据处理函数
   int  call_timetypefunc(int  funcno,CALLPARA  *cpp,double *raddr);//时段属性函数
   int  call_adduserentfunc(int  funcno,CALLPARA  *cpp,double *raddr,int groupno); //添加用户事件函数
   int  call_fileopfunc(OBJECT *objectp,int  funcno,CALLPARA  *cpp,double *raddr);
 public:
   Pcexec(PARASTACK *psp=NULL,APEvent  *apep=NULL);  //构造函数
  ~Pcexec(void);
   void push_parastack(void *pt);
   void *pop_parastack(void);
   void *get_parastack(int orderno);
virtual int  callappfunc(int  funcno,CALLPARA  *cpp,double *raddr);//
virtual int  callprocedure(PARASTACK *,CALLPARA  *cpp,double *raddr);
   int  exec_pcexecfile(char *pcexecfname,CALLPARA *callparap=NULL,double *resultaddr=NULL);
   int  exec_instrument(int start_instruno,PCINSTRUMENT *insp,char  *database,double *resultaddr);
   int  push_inpara(DATATYPE datatype,void *paraaddr);
   int  call_pcexecfile(char *pcexecfname,double *resultaddr);
   int  call_pcexecfile(char *pcexecfname,double *resultaddr,int datatype,...);
   int  set_pcmnopvalue(int mnopnums,PCMNOP_VALUE *mnopvalp);
   int  set_pcmnopvalue(char *tabname,char *objectname,char *fieldname,uchar valtype,void *valaddr);
   int  get_pcmnopvalue(char *tabname,char *objectname,char *fieldname,RDB_VALUETYPE_STRU *mnval);
   int  get_pcmnopvalue(int *mnopnums,PCMNOP_VALUE **mnopvalp);
virtual   void PrintOutString(char *s);
   };
   

class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Userpcexec : public Pcexec {
   char          *hostname;
   char          *username;
   char          *password;
   long          taskid;
public:
   Userpcexec(char *hm,char *um, char *pw, long tid,PARASTACK *psp=NULL,APEvent  *apep=NULL);
virtual int  callappfunc(int  funcno,CALLPARA  *cpp,double *raddr);
virtual int  callprocedure(PARASTACK *,CALLPARA  *cpp,double *raddr);
   };



#endif
