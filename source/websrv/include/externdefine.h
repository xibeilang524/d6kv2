/*
 * --------------------------------------------------
 *   文件名：externdefine.h
 *   软件包：DS3210配网监控系统Web服务器   
 *   文件主要内容：WEB服务器守护线程
 *
 *   Copyright 国电南自配网事业部, 2002
 *   版本号：v1.0.0,	2002/06/03
 *					 
 *   设计：蒋宏图
 *   说明：
 * --------------------------------------------------
 */

#if !defined(EXTERNDEFINE)
#define EXTERNDEFINE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
extern void delay_t(int n);
extern	void  pro_char2short(char *charp,short *shortp);
extern void pro_char2int(char *charp,int *intp);
extern void  pro_char2float(char *charp,float *floatp);
extern void  pro_char2double(char *charp,double *doublep);
extern void  pro_short2char(short *shortp,char *charp);
extern void pro_int2char(int *intp,char *charp);
#ifdef WIN32
	extern void pro_long2char(__int64 *longp,char *charp);
	extern void pro_char2long(char *charp,__int64 *longp);
#endif
extern void  pro_float2char(float *floatp,char *charp);
extern void  pro_double2char(double *doublep,char *charp);
extern char*  GetDateString();
extern int  GetDate(char *mode);
extern void  get_date(int* retv);
extern char* deleteBlanks(char *str);
extern char startsWith(char *str,char *prefix);
extern char endsWith(char *str,char *suffix);
extern char* indexOf(char *str,char *index);
extern char* getStringValue(char *str,char *prefix,char *suffix);
extern int  proc_head(char *rd,char opt,MESSAGE *msgbuf) ;
extern int  recv_data(SOCKET sockid,char *buff,int len) ;
extern int send_data(SOCKET sockid,char *buf,int len) ;
extern int  writemsg(SOCKET sockid,MESSAGE *msgbuf); 
extern int readmsg(SOCKET sockid,MESSAGE *msgbuf);
extern void getEnv(char *envvar);


#endif // !defined(EXTERNDEFINE)
