/*
 * --------------------------------------------------
 *   文件名：commfunction.cxx 
 *   软件包：DS3210配网监控系统Web服务器   
 *   文件主要内容：公用函数
 *
 *   Copyright , 2002
 *   版本号：v1.0.0,	2002/06/03
 *					 
 *   设计人：
 *	 说明
 * --------------------------------------------------
 */


#ifdef WIN32
#include "stdafx.h"
#endif

#include "message.h"


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：delay_t()
 * 功能描述：延时处理
 * 参数    ：
 *		   n=1/1000 ms(毫秒)unix,windows的单位是秒
 *
 * 返回值  ：
 *
 *
 * --------------------------------------------------
 */


void delay_t(int n)
{
	_delay_time( n );
/*
#if defined (WIN32)
        Sleep(n/1000);
#elif defined (__unix)
		struct timeval tt;
		int    ret;
		while(TRUE)
		{			
			tt.tv_sec=n/1000000;
			tt.tv_usec=n%1000000;
			ret=select(0,0,0,0,&tt);
			if(ret<0){
				if(errno==EINTR)
				{
						n=n/2;
						if(n>=50000)	continue;
				}
			}
			break;
		}
#endif*/
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_char2short()
 * 功能描述：将字节串转化为short
 * 参数    ：
 *           charp=字节串指针(源)
 *           shortp=结果指针(目标)
 *
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */

void  pro_char2short(char *charp,short *shortp)
{
	char *tmpcharp;
	tmpcharp=(char *)shortp;
#if !defined(WIN32) && !defined(__linux)
	tmpcharp[0]=charp[1];
	tmpcharp[1]=charp[0];
#else
	tmpcharp[0]=charp[0];
	tmpcharp[1]=charp[1];
#endif
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_char2int()
 * 功能描述：将字节串转化为int
 * 参数    ：
 *           charp=字节串指针
 *           inttp=结果指针
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void pro_char2int(char *charp,int *intp)
{
	char *tmpcharp;
	int i;
 
	tmpcharp=(char *)intp;
	int intsize = sizeof(int);
	for(i=0;i<intsize;i++)  {
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[intsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_char2long()
 * 功能描述：将字节串转化为long
 * 参数    ：
 *           charp=字节串指针
 *           longp=结果指针
 *
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
/*void pro_char2long(char *charp,__int64 *longp)
{
	char *tmpcharp;
	int i;
 
	tmpcharp=(char *)longp;
	int longsize = sizeof(__int64);
	for(i=0;i<longsize;i++)  {
#ifdef ALPHA_PLATFORM
		tmpcharp[i]=charp[longsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}*/

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_char2float()
 * 功能描述：将字节串转化为float
 * 参数    ：
 *           charp=字节串指针
 *           floatp=结果指针
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  pro_char2float(char *charp,float *floatp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)floatp;
	int floatsize = sizeof(float);
	for(i=0;i<floatsize;i++){
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[floatsize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_char2double()
 * 功能描述：将字节串转化为float
 * 参数    ：
 *           charp=字节串指针
 *           doublep=结果指针
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  pro_char2double(char *charp,double *doublep)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)doublep;
	int doublesize = sizeof(double);
	for(i=0;i<doublesize;i++){
#if !defined(WIN32) && !defined(__linux)
		tmpcharp[i]=charp[doublesize-1-i]; 
#else
		tmpcharp[i]=charp[i]; 
#endif
	}
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_short2char()
 * 功能描述：将short转化为字节串
 * 参数    ：
 *           shortp=源指针
 *           charp=字节串指针(目标)
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  pro_short2char(short *shortp,char *charp)
{
	char *tmpcharp;

	tmpcharp=(char *)shortp;
#if !defined(WIN32) && !defined(__linux)
	charp[0]=tmpcharp[1];
	charp[1]=tmpcharp[0];
#else
	charp[0]=tmpcharp[0];
	charp[1]=tmpcharp[1];
#endif

}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_int2char()
 * 功能描述：将int转化为字节串
 * 参数    ：
 *           intp=目标指针
 *           charp=字节串指针
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void pro_int2char(int *intp,char *charp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)intp;
	int intsize = sizeof(int);
	for(i=0;i<intsize;i++)  {
#if !defined(WIN32) && !defined(__linux)
           charp[i]=tmpcharp[intsize-1-i];
#else
           charp[i]=tmpcharp[i];
#endif
	}
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_long2char()
 * 功能描述：将long转化为字节串
 * 参数    ：
 *           longp=目标指针
 *           charp=字节串指针
 *
 * 返回值  ：  
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
/*void pro_long2char(__int64 *longp,char *charp)
{
	char *tmpcharp;
	int i;

	tmpcharp=(char *)longp;
	int longsize = sizeof(__int64);
	for(i=0;i<longsize;i++){
#ifdef ALPHA_PLATFORM
           charp[i]=tmpcharp[longsize-1-i];
#else
           charp[i]=tmpcharp[i];
#endif
	}
}*/

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_float2char()
 * 功能描述：将float转化为字节串
 * 参数    ：
 *           floatp=目标指针
 *           charp=字节串指针
 *
 * 返回值  ：  

 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  pro_float2char(float *floatp,char *charp)
{
     char *tmpcharp;
     int i;

	tmpcharp=(char *)floatp;

	int floatsize = sizeof(float);
	for(i=0;i<floatsize;i++){
#if !defined(WIN32) && !defined(__linux)
          charp[i]=tmpcharp[floatsize-1-i];
#else
          charp[i]=tmpcharp[i];
#endif
	}
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：pro_double2char()
 * 功能描述：将float转化为字节串
 * 参数    ：
 *           doublep=目标指针
 *           charp=字节串指针
 *
 * 返回值  ：  

 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  pro_double2char(double *doublep,char *charp)
{
     char *tmpcharp;
     int i;

	tmpcharp=(char *)doublep;

	int doublesize = sizeof(double);
	for(i=0;i<doublesize;i++){
#if !defined(WIN32) && !defined(__linux)
          charp[i]=tmpcharp[doublesize-1-i];
#else
          charp[i]=tmpcharp[i];
#endif
	}
}
      
/*	函数说明
 * --------------------------------------------------
 * 函数名  ：GetDateString()
 * 功能描述：获取日期字符串
 * 参数    ：
 *
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char*  GetDateString()
{
	time_t ntim;
	struct tm *tt;
    time(&ntim);
    tt=localtime(&ntim);
    return(asctime(tt));
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：GetDate()
 * 功能描述：取日期指定字段的值
 * 参数    ：
 *			mode=指定字段
 * 返回值  ： 
 *          指定字段的值   
 * 备注    ： 
 *
 * --------------------------------------------------
 */
int  GetDate(char *mode)
{
	time_t ntim;
	struct tm *tt;

    time(&ntim);
    tt=localtime(&ntim);
  
    if(strcmp(mode,"year")==0)
       return  tt->tm_year;
    else if(strcmp(mode,"mon")==0)
       return  tt->tm_mon+1;
    else if(strcmp(mode,"day")==0)
       return  tt->tm_mday;
    else if(strcmp(mode,"hour")==0)
       return  tt->tm_hour;
    else if(strcmp(mode,"min")==0)
       return  tt->tm_min;
    else if(strcmp(mode,"sec")==0)
       return  tt->tm_sec;
    else if(strcmp(mode,"week")==0)
       return  tt->tm_wday;
	return TRUE;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：get_date()
 * 功能描述：取日期值
 * 参数    ：
 *			retv=结果缓冲区指针
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  get_date(int* retv)
{
	    time_t ntim;
    	struct tm *tt;

        time(&ntim);
        tt=localtime(&ntim);
  
        *retv= tt->tm_year;
        *(++retv)= tt->tm_mon+1;
        *(++retv)= tt->tm_mday;
        *(++retv)= tt->tm_hour;
        *(++retv)= tt->tm_min;
        *(++retv)= tt->tm_sec;
        *(++retv)=tt->tm_wday;
}



/*	函数说明
 * --------------------------------------------------
 * 函数名  ：deleteBlanks()
 * 功能描述：删除字符串头部的空格
 * 参数    ：
 *           str=目标字符串
 * 返回值  ： 
 *           结果字符串 
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char* deleteBlanks(char *str)
{
	char *tmpp;
	tmpp=str;
	if(tmpp==NULL)
		return NULL;
	while(*tmpp==' ' || *tmpp=='\t') tmpp++;
	return tmpp;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：startsWith()
 * 功能描述：检查字符串str是否以字符串prefix开头
 * 参数    ：
 *           str=目标字符串
 *           prefix=搜索字符串
 * 返回值  ： 
 *           1=是 
 *           0=否 
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char startsWith(char *str,char *prefix)
{
	int i;
	int len=strlen(prefix);
    for(i=0;i<len;i++){
		if(str[i]!=prefix[i])
			break;
	}
	return i==len ? 1:0;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：endsWith()
 * 功能描述：检查字符串str是否以字符串postfix结尾
 * 参数    ：
 *           str=目标字符串
 *           postfix=搜索字符串
 * 返回值  ： 
 *           1=是 
 *           0=否 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char endsWith(char *str,char *postfix)
{
	int i;
	int lenpost=strlen(postfix),lenstr=strlen(str);
    for(i=0;i<lenpost;i++){
		if(str[lenstr-1-i]!=postfix[lenpost-1-i])
			break;
	}
	return i==lenpost ? 1:0;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：indexOf()
 * 功能描述：检查字符串index在字符串str中出现的位置
 * 参数    ：
 *           str=目标字符串
 *           index=搜索字符串
 * 返回值  ： 
 *             
 *           NULL=无效 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char* indexOf(char *str,char *index) /*修改了输入参数*/
{
	char *tmp=strstr(str,index);
	char *val=deleteBlanks((char*)(tmp+strlen(index)));
	if(val==NULL)
		return NULL;
	if(val[strlen(val)-1]=='\n')
		val[strlen(val)-1]='\0';
	else if((tmp=strchr(val,' '))!=NULL)
		*tmp='\0';
	return val;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：getStringValue()
 * 功能描述：获取一特殊字符串
 * 参数    ：
 *
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
char* getStringValue(char *str,char *prefix,char *suffix) /*修改了输入参数*/
{
	char *prep,*sufp;
	
	prep=strstr(str,prefix);
	if(prep==NULL)
		return NULL;
	sufp=strstr(str,suffix);
	if(sufp==NULL)
		return NULL;
	if(sufp<prep)
		return NULL;

	*sufp='\0';
	return deleteBlanks((char*)(prep+1));
}



/*	函数说明
 * --------------------------------------------------
 * 函数名  ：proc_head()
 * 功能描述：报头处理
 * 参数    ：
 *		   rd=数据缓冲区
 *		   opt=模式('r'=接受 'w'=发送)
 *		   id=客户服务线程ID号
 *
 * 返回值  ：
 *
 *
 * --------------------------------------------------
 */
int  proc_head(char *rd,char opt,MESSAGE *msgbuf) 
{
     if(opt=='r') 
	 {
		pro_char2int(&(rd[0]),&(msgbuf->type));
		pro_char2int(&(rd[4]),&(msgbuf->length));
		msgbuf->registerf=rd[8];
	  }
      else if(opt=='w') 
	  {
		pro_int2char(&(msgbuf->type),&(rd[0]));
		pro_int2char(&(msgbuf->length),&(rd[4]));
		rd[8]=msgbuf->registerf;
	  }
	  return TRUE;
} 


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：recv_data()
 * 功能描述：从SOCKET读取指定长度的数据
 *           
 * 参数    ：
 *         sockid=SOCKET的ID号 
 *         buff= 接受缓冲区指针
 *         len=  指定读取数据长度
 *
 * 返回值  ：成功，返回>=0
 *           失败，返回-1
 * --------------------------------------------------
 */
 int  recv_data(SOCKET sockid,char *buff,int len) 
 {
	int retnums=-1,rettimes=0;
	int max_count = 10*(1+len/MAXBUFSIZE);//40*(1+len/MAXBUFSIZE);
    char *tmpbuf;
    int tmplen=0;  

    struct timeval  delay;
    delay.tv_sec=0;
    delay.tv_usec=1000;
	

    tmpbuf=buff;
	if(buff == NULL) 
		return -1;
    while(TRUE) { 
		delay.tv_sec=0;
		delay.tv_usec=1000;
		retnums = recv(sockid,tmpbuf,len,0); //If the connection has been gracefully closed, the return value is zero
		if(retnums>0) { 
             tmplen+=retnums;
             len-=retnums;     
             tmpbuf+=retnums;
             if(len<=0)
				 break;
             else
		         select(0,0,0,0,&delay);
		} 
        else if(retnums == SOCKET_ERROR) { 
			if((errno == EINTR)||(errno == EWOULDBLOCK)||(errno == 0))
			{ 
				if((rettimes++)>=max_count) break;
				delay_t(50000);
				continue;
			 } 
             else {
                  return -1;
			 } 
		}
        else {
             return -1;
		} 
	} /*while*/
  return tmplen;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：send_data()
 * 功能描述：内部函数
 *           向SOCKET写入指定长度的数据
 * 参数    ：
 *         sockid=SOCKET的ID号 
 *         buff= 发送缓冲区指针
 *         len=  指定发送数据长度
 *
 * 返回值  ：成功，返回>=0
 *           失败，返回-1
 *
 * --------------------------------------------------
 */
int send_data(SOCKET sockid,char *buf,int len) 
{
    int retn=-1;
	if(sockid<0) return -1;
	char *ptr = (char *)buf;
	int max_count = 10*(1+len/MAXBUFSIZE);//40*(1+len/MAXBUFSIZE);
	int txn = 0;
	if(len<=0) return 0;
	if(buf==NULL) return -1;
	int txntime = 0;
	while(TRUE)
	{
		txn = send(sockid,ptr,len,0);
		if(txn==SOCKET_ERROR)
		{
			if((errno == EINTR)||(errno == EWOULDBLOCK)||(errno == 0))
			{
				if((txntime++)<max_count){
					delay_t(50000);
					continue;
				}
			}
			return -1;
		}
		else if(txn==0){
			return -1;
		}
		txntime=0;
		len -= txn;
		ptr += txn;
		if(len<=0) return 0;
	}
	return retn;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  writemsg()
 * 功能描述：发送一完整报文(报头+正文),(发送数据在发送缓冲区).
 * 参数    ：id=客户服务线程ID号
 *
 * 返回值  ：成功，返回0
 *           失败，返回-1
 *int readmsg(SOCKET sockid,MESSAGE *msgbuf)
 * --------------------------------------------------
 */
int  writemsg(SOCKET sockid,MESSAGE *msgbuf) 
{
	  
	   //报头和报文一起发送
	   int    ret;
       char   tmpbuf[MSGHEADLEN];

	   int length = msgbuf->length+MSGHEADLEN;
	   memset(tmpbuf,0,MSGHEADLEN);
       proc_head(tmpbuf,'w',msgbuf);
	   memcpy((char*)msgbuf,tmpbuf,MSGHEADLEN);
       ret=send_data(sockid,(char *)msgbuf,length);
       if(ret==-1) {
             printf("数据发送失败");
             return -1 ;
	   }
	   return ret;
}



/*	函数说明
 * --------------------------------------------------
 * 函数名  ：readmsg()
 * 功能描述：读取一完整报文(报头+正文),放入接受缓冲区。
 * 参数    ：id=客户服务线程ID号
 *
 * 返回值  ：成功,返回0
 *           失败,返回-1
 *
 * --------------------------------------------------
 */
int readmsg(SOCKET sockid,MESSAGE *msgbuf)
{
       int    ret;
       char		tmpbuf[MSGHEADLEN];

	   memset(tmpbuf,0,MSGHEADLEN);
       ret=recv_data(sockid,tmpbuf, MSGHEADLEN);//在客户端中断时，返回0
	   if(ret <= 0)
		   return -1;
       ret=proc_head(tmpbuf,'r',msgbuf);
       if(ret==-1) 
     	    return -1;
       if(msgbuf->length>0 && msgbuf->length<MAXBUFSIZE) {
           ret=recv_data(sockid,(char *)msgbuf->messagebuf,msgbuf->length);
		   if(ret <= 0)
	           return -1;;
	   }
	   else if(msgbuf->length >= MAXBUFSIZE)
	   {
		   msgbuf->hugebuf = (char*)malloc(msgbuf->length);
		   if(msgbuf->hugebuf == NULL) 
			   return -1;
           ret=recv_data(sockid,(char *)msgbuf->hugebuf,msgbuf->length);
		   if(ret <= 0)
	           return -1;;
	   }
      return 0;
}

#ifndef WIN32
int GetEnvironmentVariable(char *env,char *envvar,int size)
{
	char * envtmp = getenv(env);
	if(envtmp == NULL || envtmp[0] == '\0') 
	{
		strcpy(envvar,"/export/home/ds3100");
	}
	else
		strcpy(envvar,envtmp);
	return strlen(envvar);
}
#endif
//取环境变量
//环境变量，程序会自动去掉后面的"\"
void getEnv(char *envvar)
{
	int num = GetEnvironmentVariable("NBENV", envvar, 256-1);

	if(num == 0)
	{
		strcpy(envvar,"c:/ds3210");
	}
	else
	{
		char post[4];
		memset((char*)post,0,4);
		strcpy(post,"/");
		char rtn = endsWith((char*)envvar,(char*)post);
		if(rtn)
			envvar[strlen(envvar)-1]='\0';//去掉后面的"\"
	}
	return;
}
