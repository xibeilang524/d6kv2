/*
 * --------------------------------------------------
 *   文件名：answermsg.cxx 
 *   软件包：DS3210配网监控系统Web服务器   
 *   文件主要内容：回答客户端的消息
 *
 *   Copyright , 2002
 *   版本号：v1.0.0,	2002/06/03
 *					 
 *   设计人：
 *	 说明：
 * --------------------------------------------------
 */

#ifdef WIN32
	#include "stdafx.h"
#endif 

#ifdef __unix
	#include <stdlib.h>
	#include <strings.h>
	#include <sys/types.h>
    #include <dirent.h>
#endif

#include "webstruct.h"

#include "externdefine.h"
#include "db.h"
#include "scd/saveflag.h"
#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scd/dev.h"
#include "filestat.h"

THREADMANAGE threadinfo[INITCLIENTNUMS];
char DBNAME[64];
char HISDBNAME[64];
char DBO[64];

#ifdef WIN32
	extern int  clntchangef;
#else
	extern void recvStream(int iii);
#endif

CDB dbo;
USEREVENT *puserevent = NULL;	//指向web事件管理表
DEVPARATABLE devparatable;		//设备参数对照表
extern void readstation(char *groupname);




int strcmpNoCase(const char * string1, const char * string2)
{
#ifdef WIN32
	return _stricmp(string1,string2);
#else
	return strcasecmp(string1,string2);
#endif
}

BOOL IsLittleEndian(void) 
{ 
	unsigned short  wValue = 0x5678; 
	return (*((unsigned char*)&wValue) == 0x78); 
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：convertPasswd()
 * 功能描述：翻译口令
 * 参数    ：
 *          PWD=口令原文指针
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void  convertPasswd(char* pwd)
{
      unsigned int i;
      for(i=0;i<strlen(pwd);i++)  
		  pwd[i]=pwd[i]-50-i;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：parsefieldstring()
 * 功能描述：分析选择域字符串
 * 参数    ：
 *           srcstr=目标字符串
 *           desp分析结束后的指针
 * 返回值  ： 
 *           域的个数
 *            
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
int parsefieldstring(char *srcstr,void **desp)
{

	int num = 10,count=0;//num,递进基数;  count,域计数
	NAME *p = (NAME *)malloc(num*sizeof(NAME));
	memset(p,0,sizeof(NAME)*num);
	int len = strlen(srcstr);
	int j = 0,i;
	for( i =0;i<len;i++)
	{
		if(srcstr[i] == ',')
		{
			if(i>j)
			{
				if(count>0 && num/count >=1)
				{
					NAME *temp = p;
					p = (NAME*)malloc((num+count)*sizeof(NAME));
					memset(p,0,sizeof(NAME)*(num+count));
					memcpy(p,temp,num*sizeof(NAME));
					FREE(temp);
					num += count;
				}
				memcpy((char*)(p[count].name),(char*)&srcstr[j],i-j);
				count++;
			}
			j = i+1;
		}
	}
	if(i>j)//最后一个不是",",还有可能最后一个是","
	{
		memcpy((char*)(p[count].name ),(char*)&srcstr[j],i-j);
		j = i+1;
		count++;
	}
	*desp = (void*)p;
	p = NULL;
	return count;
}

void getTableinfo(char*tablename,void**buf)
{
	dbo.requestDBApi();
	*buf = NULL;

	CS_RETCODE      retcode = 0; 
	FUNC_STR func;
	CS_DATAFMT *datafmt = NULL;

	void* pt = NULL;
	func.func = GET_TABLE_INFO;
	strcpy(func.dbname,DBNAME);
	strcpy(func.table,tablename);
	if(dbo.GetTableInfo(&func,&datafmt)!=CS_FAIL)
	{
		NAME *pval = (NAME *)malloc(func.colnum*sizeof(NAME));
		memset(pval,0,func.colnum*sizeof(NAME));
		for(int i=0; i<func.colnum; i++)
		{
			sprintf((pval+i)->name,"%s",(datafmt+i)->name);//域名
		}
		*buf = (void*)pval;
		pval = NULL;
	}
	FREE(datafmt);
	dbo.releaseDBApi();
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：ReadRecorders
 * 功能描述：读数据库
 * 参数    ：tablename,表名;fieldname,域名;  where,条件;orderfn,排序域名
 *		   fieldcount,选择域的数量
 *
 * 返回值  ：值的个数(不是记录的个数),<0时出错。
 *           buf,为查询的值缓冲区。行列鱼贯而入
 *
 * --------------------------------------------------
 */
int readRecorders(char* dbname,char* tablename,char* fieldname, char* where,char* orderfn,int bdistinct,int *fieldcount,void **buf)
{
	dbo.requestDBApi();
	CS_RETCODE      retcode = 0; 
	FUNC_STR func;
	CS_DATAFMT *datafmt = NULL;
	void* pt = NULL;
    *buf = NULL;
	
	func.func = GET_TABLE_INFO;
	strcpy(func.dbname,dbname);
	strcpy(func.table,tablename);
	
	if(fieldname != NULL  && fieldname[0] != '\0')
	{
		if(dbo.GetTableInfo(&func,&datafmt) != CS_FAIL)
		{		
			NAME *parsedfdname = NULL;

			int colcount = parsefieldstring(fieldname,(void**)&parsedfdname);

			int orderf = (orderfn == NULL || orderfn[0] =='\0') ? FALSE : TRUE;
			func.distinctflag = bdistinct;
			if(colcount > 0 && parsedfdname[0].name[0] == '*')//全选
			{
				colcount = func.colnum;
				for(int i=0; i<func.colnum; i++)
				{
					func.col[i].condition = NO;
					func.col[i].select = YES;
					func.col[i].order_d = i; //order data,used to column order ,must be 赋值
					func.col[i].order = -1;
					if(orderf && !strcmpNoCase(datafmt[i].name,orderfn))
						func.col[i].order = 0;	//used to  order by,需要就赋值
					
				}
			}
			else
			{
				for(int i=0; i<func.colnum; i++)
				{
					func.col[i].condition = NO;
					func.col[i].select = NO;
					func.col[i].order = -1;
					func.col[i].order_d = -1;
					for(int j=0; j<colcount; j++)
					{
						if(!strcmpNoCase(datafmt[i].name,parsedfdname[j].name))
						{
							func.col[i].select = YES;
							func.col[i].order_d = j;
						}
					}
					if(orderf && !strcmpNoCase(datafmt[i].name,orderfn))
						func.col[i].order = 0;
				}
			} 
		
			*fieldcount = colcount;
			FREE(parsedfdname);

			func.func = SEL_RECORDERS;
			if(where != NULL && where[0] != '\0')
			{
				strcpy(func.isql,where);
				func.func = SEL_ISQL_RECORDERS;
			}//if(we != NULL)
			if((retcode = dbo.SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
			{
				retcode = func.ret_roxnum * colcount;
				if(retcode>0)
				{
					int ptcnt = 0;
					NAME *pval = (NAME *)malloc(retcode*sizeof(NAME));
					memset(pval,0,retcode*sizeof(NAME));
					for(int i =0;i<func.ret_roxnum;i++)//row
					{
						for(int j =0;j<colcount;j++)
						{
							for(int k =0;k<func.colnum;k++)
							{
								if(func.col[k].select!=YES)		continue;

								int od = func.col[k].order_d;
								if(od!=j) continue;
									switch ((datafmt+k)->datatype)
									{
										case CS_BINARY_TYPE:
										{
											break;
										}
										case CS_TINYINT_TYPE:
										{
											unsigned char bb = (unsigned char)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name,"%d",bb);
											break;
										}
										case CS_SMALLINT_TYPE:
										{
											short sint = (short)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",sint);
											break;
										}
										case CS_INT_TYPE:
										{
											int  intrtn = (int)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",intrtn);
											break;
										}
										case CS_LONG_TYPE:
										{
											long longrtn = (long)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%d",longrtn);
											break;
										}
										case CS_REAL_TYPE:
										case CS_FLOAT_TYPE:
										{
											double  frtn = (double)dbo.GetData(i,j,&func,datafmt,pt,ptcnt,NULL);
											sprintf(pval[i*colcount + j].name ,"%f",frtn);
											break;
										}
										case CS_CHAR_TYPE:
										case CS_VARCHAR_TYPE:
										{
											dbo.GetData(i,j,&func,datafmt,pt,ptcnt,pval[i*colcount + j].name );
											break;
										}
										case CS_DECIMAL_TYPE:
										{
											break;
										}
									}//switch
									break;
								}//for k
							}//for j
						}//for i
					*buf = (void*)pval;
					pval = NULL;
				}//if(retcode>0)
			}//if((retcode = pdb->SelectRcds(&func,datafmt,&pt)) == CS_SUCCEED)
			else
				retcode = -2;
		}
		else
			retcode = -1;
	}	
	dbo.releaseDBApi();
	FREE(datafmt);
	FREE(pt);
	return retcode;
}


/*
 * 功能描述:执行SQL命令
 * 参数    ：DBName,数据库名；sqlStatement，sql表达式.
 * 返回值  ：true成功.false失败.
 * --------------------------------------------------
 */
int Execute(char *DBName,char* sqlStatement)
{
	if (sqlStatement == 0 || sqlStatement[0] == '\0')
	{
		return CS_FAIL;
	}

	dbo.requestDBApi();

	FUNC_STR    func;
	CS_INT		retcode;
	
	
	strcpy(func.dbname,DBName);
    func.func = ISQL_COMMAND;
	
	strcpy(func.isql,sqlStatement);
	retcode = dbo.IsqlCommand(&func);
	dbo.releaseDBApi();
   	return (retcode);

}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：loadExcludeEvent
 * 功能描述：读"web事件管理表"
 * 参数    ：
 *
 * 返回值  ：
 *
 * --------------------------------------------------
 */
void loadExcludeEvent()
{
	//服务器只发送"web事件管理表"中设置的事项
	int fieldcount;
	void *buf=NULL;
	int ret = readRecorders(DBNAME,"web事件管理表","类型号,组名","","组名",TRUE,&fieldcount,&buf);
	if(ret>0)
	{
		NAME *pname = NULL;
		pname = (NAME*)buf;
		unsigned char userright = 0;
		USEREVENT *pevent = puserevent;//事件的用户数为任意多。但缺省是2一定要记住
		USEREVENT *poldevent = pevent;
		for(int i=0;i<ret/fieldcount;i++)
		{
			userright = (unsigned char)atoi(pname[2*i+1].name);//组名
			int haveuserflag = FALSE;
			pevent = puserevent;
			while(pevent != NULL)
			{
				if(userright == pevent->userright)
				{
					haveuserflag = TRUE;
					pevent->excludeevent[pevent->eventcount] = atoi(pname[2*i].name);
					pevent->eventcount++;
					break;
				}
				poldevent = pevent;
				pevent = pevent->puserevent;
			}
			if(!haveuserflag)
			{
				if(puserevent == NULL)
				{
					puserevent = new USEREVENT();
					puserevent->userright = userright;
					puserevent->excludeevent[puserevent->eventcount] = atoi(pname[2*i].name);
					puserevent->eventcount++;
				}
				else
				{
					poldevent->puserevent = new USEREVENT();//生成1个链表
					poldevent->puserevent->userright = userright;
					poldevent->puserevent->excludeevent[poldevent->puserevent->eventcount] = atoi(pname[2*i].name);
					poldevent->puserevent->eventcount++;
				}
			}
		}
		buf = NULL;
		FREE(pname);
	}
}


//设备参数表
void loadDevParaTable()
{
	int fieldcount;
	void *buf=NULL;
	int ret = readRecorders(DBNAME,"web设备参数表对照表","设备类型,参数表名,显示域名","","设备类型",TRUE,&fieldcount,&buf);
	if(ret>0)
	{
		NAME *pname = NULL;
		pname = (NAME*)buf;
		
		int devcount = ret/fieldcount > DEVTYPENUM ? DEVTYPENUM :ret/fieldcount;
		devparatable.count = devcount;
	    for(int i=0;i<devcount;i++)
		{
			devparatable.devtype[i] = atoi(pname[i*fieldcount].name);
			strncpy(devparatable.paratable[i].name,pname[i*fieldcount+1].name,CODESIZE);
			strncpy(devparatable.showfield[i].name,pname[i*fieldcount+2].name,CODESIZE);
		}
		FREE(pname);
	}
}

//取设备类型
char *getDevParaTable(int devtype,char *showfield)
{
	for(int i=0;i<devparatable.count;i++)
	{
		if(devparatable.devtype[i] == devtype)
		{
			strcpy(showfield,devparatable.showfield[i].name) ;
			return devparatable.paratable[i].name;
		}
	}
	return NULL;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：specifyDir()
 * 功能描述：判断某一目录是否是指定的目录
 * 参数    ：dir,目录名
 * 返回值：  true,是
 *			 false,否
 *           
 *
 * --------------------------------------------------
 */
int  specifyDir(char *dir)
{
#ifdef WIN32
	if(!_stricmp(dir,"config") || !_stricmp(dir,"line") || !_stricmp(dir,"normal") || 
		!_stricmp(dir,"station") || !_stricmp(dir,"system"))
		return TRUE;
#else
	if(!strcasecmp(dir,"config") || !strcasecmp(dir,"line") || !strcasecmp(dir,"normal") || 
		!strcasecmp(dir,"station") || !strcasecmp(dir,"system"))
		return TRUE;
#endif
	return FALSE;

}

//发送"web用户表"
void  readWebUser(int threadno)
{
	int fieldcount;
	void *valbuf;
	char where[256];
	memset(where,0,256);
	int ret = readRecorders(DBNAME,"web用户表","用户名,口令,组名","","",FALSE,&fieldcount,&valbuf);
	printf("\n发送'web用户表表' readRecord ret=%d fieldcnt=%d \n",ret,fieldcount);
	MESSAGE msgbuf;
	msgbuf.type = S_USER;
	char *buf = msgbuf.messagebuf ;

	NAME *pname = NULL;
	
	if(ret>0)
	{
		int usercount = ret/fieldcount;
		
		pro_int2char(&usercount,buf);//用户个数
		buf += sizeof(int);
		
		pname = (NAME*)valbuf;
		unsigned char len = 0;
		char pass[256];
		for(int i=0;i<usercount;i++)
		{
			len = strlen(pname[i*fieldcount].name );//用户名长度
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);//用户名
			buf += len;

			len = strlen(pname[i*fieldcount+1].name); //口令长度
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memset(pass,0,256);
			memcpy(pass,pname[i*fieldcount+1].name,len);//口令
			memcpy(buf,pass,len);//口令
			buf += len;

			buf[0] = (unsigned char)atoi(pname[i*fieldcount+2].name);//组名
			buf += sizeof(unsigned char);
		}
	}
	else
	{
		int usercount = 0;
		pro_int2char(&usercount,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);

}

//发送web图形分组表
void readWebFilename(int threadno)
{
	int fieldcount = 0;
	void *valbuf = NULL;

	//图形用户的个数也是可以不限
	int ret = readRecorders(DBNAME,"web图形分组表","*",NULL,NULL,FALSE,&fieldcount,&valbuf);
		MESSAGE msgbuf;
		msgbuf.type = S_GRAPHGROUP;
		char *buf = msgbuf.messagebuf ;
		int usercount = 0;
		if(ret>0) 
		  usercount = ret/fieldcount;
		
		pro_int2char(&usercount,buf);//图形记录个数
		buf += sizeof(int);
		
		NAME *pname = NULL;
		pname = (NAME*)valbuf;
		valbuf = NULL;
		unsigned char len = 0;
		for(int i=0;i<usercount;i++)
		{
			len = strlen(pname[i*fieldcount].name );//图形文件名长度
			buf[0] = len;
			buf += sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);//禁止访问的图形文件名
			buf += len;

			buf[0] = (unsigned char)atoi(pname[i*fieldcount+1].name);//组名
			buf += sizeof(unsigned char);
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		FREE(pname);
}

//发送用户权限参数表
void readWebUserRight(int threadno)
{
	int fieldcount;
	void *valbuf;
	char where[256];
	memset(where,0,256);
	char fields[512]={"主机名,操作员,查看历史曲线,查看历史事项,安全级别0,安全级别1,安全级别2,安全级别3,安全级别4,安全级别5,安全级别6,安全级别7,安全级别8,安全级别9,安全级别10,安全级别11,安全级别12,安全级别13,安全级别14,安全级别15,安全级别16,安全级别17,安全级别18,安全级别19,安全级别20,安全级别21,安全级别22,安全级别23,安全级别24,安全级别25,安全级别26,安全级别27,安全级别28,安全级别29,安全级别30,安全级别31"};
	int ret = readRecorders(DBNAME,"web权限参数表",fields,"","",FALSE,&fieldcount,&valbuf);
	printf("\n发送'web权限参数表' readRecord ret=%d fieldcnt=%d \n",ret,fieldcount);
	MESSAGE msgbuf;
	msgbuf.type = S_USERRIGHT;
	char *buf = msgbuf.messagebuf ;
	
	NAME *pname = NULL;
	
	if(ret>0)
	{
		int rowcount=ret/fieldcount;
		pro_int2char(&rowcount,buf);  //记录个数
		buf += sizeof(int);

		pname=(NAME*)valbuf;
		unsigned char len=0;
		for(int i=0;i<rowcount;i++)
		{
			len=strlen(pname[i*fieldcount].name);		//主机名长度
			buf[0]=len;
			buf+=sizeof(unsigned char);
			
			memcpy(buf,pname[i*fieldcount].name,len);	//主机名
			buf+=len;

			len=strlen(pname[i*fieldcount+1].name);		//操作员长度
			buf[0]=len;
			buf+=sizeof(unsigned char);

			memcpy(buf,pname[i*fieldcount+1].name,len);	//操作员
			buf+=len;
			

			//前面加2个表示曲线和事项权限的字段
			//安全级别长度,定义为2，表示安全级别长度2字节short int型。
			len=strlen(pname[i*fieldcount+2].name);
			if(len==1) 
				len=2;
			buf[0]=len;
			buf+=sizeof(unsigned char);
			short int val;
			for(int j=0;j<32+2;j++)
			{
				val=atoi(pname[i*fieldcount+2+j].name);
				pro_short2char(&val,buf);
				buf+=sizeof(short int);
			}
		}
		
	}
	else
	{
		int usercount = 0;
		pro_int2char(&usercount,buf);
		buf += sizeof(int);
	}
	msgbuf.length=abs((int)(msgbuf.messagebuf-buf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
	return;
}


/*
LiCG : SYS_TIME在linux64下返回年有问题，c运行时库
*/
//sdate为1970以来的天数,objname,对象名;day返回接受客户端的时间
//day，1970年以来的秒数
void  DayCurve(int threadno,char *objname,unsigned char curFlag,int curType,int day)
{
	time_t datetime = time_t(day);
	//utc seconds to local date time
	//struct tm *when = localtime(&datetime);
	//utc seconds to utc date time
	struct tm *when = gmtime(&datetime);
	when->tm_year += 1900;
	when->tm_mon +=1;

	MESSAGE msgbuf;
	msgbuf.type = S_CURVE; 
	char *psendmsgbuf = msgbuf.messagebuf ;

	char tablename[128];
	memset(tablename,0,128);
	//TimeProc timeproc;
	//SYS_TIME sys_time = timeproc.ConvertTime(day);
	int fieldcount=0;
	int fieldcount1 =0;
	short savetime = 5;		//存盘周期,累加电度值,当在某点以后缺点时,也应该有值
	void *valbuf = NULL;
	NAME *pname = NULL;
	int ret = 0;

	int bValid = TRUE;
	if(curFlag == DAYYCCURVE)//日遥测曲线
	{
#ifdef USE_YC_DAYTABLE
		sprintf(tablename,"yc%04d%02d%02d",when->tm_year,when->tm_mon,when->tm_year,when->tm_mday);
#else
		sprintf(tablename,"yc%04d%02d",when->tm_year,when->tm_mon);
#endif
		char con[256];
		memset(con,0,256);
		sprintf(con,"代码 = '%s'",objname);
		int fdc;
		if(readRecorders(DBNAME,"遥测参数表","存盘周期",con,"",TRUE,&fdc,&valbuf))
		{
			pname = (NAME *)valbuf;
			savetime = atoi(pname->name);
			FREE(pname);
		}
		else 
			bValid = FALSE;
	}
	else if(curFlag == MONTHYCCURVE)//月遥测曲线
	{
		sprintf(tablename,"dayyc%04d%02d",when->tm_year,when->tm_mon);
	}
	else if(curFlag == DAYKWHCURVE)//日电度曲线
	{
		sprintf(tablename,"kwh%04d%02d",when->tm_year,when->tm_mon);
		char con[256];
		memset(con,0,256);
		sprintf(con,"代码 = '%s'",objname);
		int fdc;
		if(readRecorders(DBNAME,"电度参数表","存盘周期",con,"",TRUE,&fdc,&valbuf))
		{
			pname = (NAME *)valbuf;
			savetime = atoi(pname->name);
			FREE(pname);
		}
		else 
			bValid = FALSE;
	}
	else if(curFlag == MONTHKWHCURVE)//月电度曲线
	{
		sprintf(tablename,"daykwh%04d%02d",when->tm_year,when->tm_mon);
	}
	else
	{
		bValid = FALSE;
	}

	if(!bValid)
	{
		psendmsgbuf[0] = 0;//无效数据
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		return;
	}

	char where[256];
	memset(where,0,256);

	if(curFlag == DAYYCCURVE || curFlag == MONTHYCCURVE || curFlag == DAYKWHCURVE || curFlag == MONTHKWHCURVE)
	{
		char fieldname[128],orderfdname[64];
		memset(fieldname,0,128);
		memset(orderfdname,0,64);

		if(curFlag == DAYYCCURVE || curFlag == DAYKWHCURVE)	//日曲线
		{
			strcpy(fieldname,"time,data");
			strcpy(orderfdname,"time");
			if(curType>=50)
			{
				sprintf(where ,"name ='%s' and sdate = %d and flag=%d",objname,(int)day/(3600*24),curType);//计划值等曲线;
			}
			else
				sprintf(where,"name ='%s' and sdate = %d and flag<50",objname,(int)day/(3600*24));//HISDATA_REAL);
		}
		else
		{

			//月曲线,计算出该月第一天和下月第一天的sdate
			int fromdate=0,todate=0;
			fromdate = day/(3600*24) - when->tm_mday;
			struct tm newtime;
			newtime.tm_year = when->tm_year -1900;
			newtime.tm_mon = when->tm_mon - 1;
			newtime.tm_mday = 1;
			newtime.tm_hour =  when->tm_hour;
			newtime.tm_min = when->tm_min;
			newtime.tm_sec = when->tm_sec;
			newtime.tm_wday = 0;
			newtime.tm_yday = 0;
			newtime.tm_isdst = -1;
			if(newtime.tm_mon == 11)
			{
				newtime.tm_year ++;
				newtime.tm_mon = 0;
			}
			else
			{
				newtime.tm_mon ++;
			}
			time_t totime = mktime(&newtime);
			todate = totime/(60*60*24);
			strcpy(fieldname,"sdate,data");
			strcpy(orderfdname,"sdate");
			//历史数据库中sdate存储的是1970年以来的天数
			sprintf(where,"name ='%s' and sdate > %d and sdate <= %d and flag = %d",objname,fromdate,todate,curType);
		}

		ret = readRecorders(HISDBNAME,tablename,fieldname,where,orderfdname,TRUE,&fieldcount1,&valbuf);
		if(ret>0)
		{
			pname = (NAME*)valbuf;
		}

	}
	psendmsgbuf[0] = curFlag;//日月曲线标志，1为日曲线;
	psendmsgbuf += sizeof(unsigned char);
	pro_int2char(&curType,psendmsgbuf);//曲线类型标志，主要是月曲线用//待完善
	psendmsgbuf += sizeof(int);
	
	pro_int2char(&day,psendmsgbuf);//年月日
	psendmsgbuf += sizeof(int);

	pro_short2char(&savetime,psendmsgbuf);//存盘周期
	psendmsgbuf += sizeof(short);

	psendmsgbuf[0] = strlen(objname);//对象长度
	psendmsgbuf += sizeof(unsigned char);
	memcpy(psendmsgbuf,objname,strlen(objname));//对象代码
	psendmsgbuf += strlen(objname);
	

	short valnums ;//值个数
	if(ret<=0 || savetime <= 0) {ret=0;valnums=0;} //没有存盘时间,不做曲线
	else
	{
		valnums = ret/fieldcount1;
	}
	
	char *pvalnumsbuf = psendmsgbuf;//保存值个数指针
	psendmsgbuf += sizeof(short);

	int intval;
	int timepoint,oldtimepoint=-1;
	short count=0;
	for(int i=0;i<valnums;i++)
	{
		timepoint = (int)atof(pname[i*2].name);
		
		if(timepoint == oldtimepoint) continue;
		oldtimepoint =  timepoint;
		//if( (curFlag == DAYYCCURVE || curFlag == DAYKWHCURVE) && (timepoint % savetime != 0)) continue;//日曲线是存盘时间整数倍的数据就发送
		
		intval = (int)(atof(pname[i*2+1].name)*100);
		pro_int2char(&timepoint,psendmsgbuf);//时间点,日曲线是分钟；月曲线是sdate(1970/01/01的天数)
		psendmsgbuf += sizeof(int);
		pro_int2char(&intval,psendmsgbuf);//值
		psendmsgbuf += sizeof(int);
		count++;
	}//for i

	pro_short2char(&count,pvalnumsbuf);

	FREE(pname);

	int calVar  = 0;  //计算量个数
	int calType  = 0; //计算量类型
	int val = 0;	  //计算量的值

	//在曲线后要增加统计量
	if(curFlag == DAYYCCURVE || curFlag == MONTHYCCURVE)
	{
		
		int dataType = 0; //遥测类型
		//得到遥测代码类型,P、Q要算负荷率，U要算合格率,其他要算平均值
		memset(where,0,256);
		sprintf(where,"代码=\'%s\'",objname);
		
		ret = readRecorders(DBNAME,YCPARATABLE,"类型",where,NULL,FALSE,&fieldcount,&valbuf);
		if(ret > 0)
		{
		    calVar = 1;
		    pname = (NAME*)valbuf;
		    dataType = atoi(pname[0].name);
		    FREE(pname);
		}
	        
		if(calVar != 0 || dataType != 0)
		{
		    if(dataType == OBJTYPE_P   || dataType == OBJTYPE_Pa  ||  \
		       dataType == OBJTYPE_Pb  || dataType == OBJTYPE_Pc  || \
		       dataType == OBJTYPE_Pab || dataType == OBJTYPE_Pbc || \
		       dataType == OBJTYPE_Pca || dataType == OBJTYPE_Q   || \
		       dataType == OBJTYPE_Qa  || dataType == OBJTYPE_Qb  || \
		       dataType == OBJTYPE_Qc  || dataType == OBJTYPE_Qab || \
		       dataType == OBJTYPE_Qbc || dataType == OBJTYPE_Qca )   //功率类型
		     
		       calType = HISDATA_DAYS_PQRATE;	//负荷率
		       
		     else if(dataType == OBJTYPE_V   || dataType == OBJTYPE_Va  ||  \
		       	     dataType == OBJTYPE_Vb  || dataType == OBJTYPE_Vc  || \
		             dataType == OBJTYPE_Vab || dataType == OBJTYPE_Vbc || \
		             dataType == OBJTYPE_Vca || dataType == OBJTYPE_V0   || \
		             dataType == OBJTYPE_V1  || dataType == OBJTYPE_V2 )	//电压类型

		       calType = HISDATA_DAYS_HGL;	//合格率
		     else 
		     	calType = HISDATA_DAYS_AVERAGE;	//平均值	     		     
		    	
			if(curFlag == MONTHYCCURVE && curType == HISDATA_DAYS_PQRATE)	//月负荷率曲线求其平均值
			{
				calType = HISDATA_DAYS_AVERAGE;
			}
		    memset(where,0,256);
		    if(curFlag == DAYYCCURVE)
				sprintf(where,"name='%s' and sdate = %d and flag = %d ",objname,(int)day/(3600*24),calType);
			else
				sprintf(where,"name='%s' and  flag = %d ",objname,calType);

			sprintf(tablename,"dayyc%04d%02d",when->tm_year,when->tm_mon);
 			ret = readRecorders(HISDBNAME,tablename,"data",where,NULL,FALSE,&fieldcount,&valbuf);
 			if(ret > 0)
 			{
 			    pname = (NAME*)valbuf;
			    val = int(atof(pname[0].name) * 1000);
			    FREE(pname);
 			}
		}
	}
		
	pro_int2char(&calVar,psendmsgbuf);//计算量个数
	psendmsgbuf += sizeof(int);
	pro_int2char(&calType,psendmsgbuf);//计算量类型
	psendmsgbuf += sizeof(int);
	
	pro_int2char(&val,psendmsgbuf);//计算量的值
	psendmsgbuf += sizeof(int);

	msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
	writemsg(threadinfo[threadno].sockid,&msgbuf);

}


//取实时曲线数据
void getSendRealCurveData(int threadno)
{
	void *valbuf = NULL;
	NAME *pname = NULL;
	int ret = 0;
	int fieldcount;


	THREADMANAGE *pthread = threadinfo + threadno;
	time_t now;
	time( &now );
	struct tm *when = localtime( &now );
	for(int i = 0; i < pthread->realcurvecount ; i++)
	{
		MESSAGE msgbuf;
		msgbuf.type = S_REALCURVE;
		char *psendmsgbuf = msgbuf.messagebuf ;
		REALCURVE *p = pthread->prealcurve + i;
		if(p->savetime >0 && now - p->lastSendTime > p->savetime*60)//存盘时间要大于0,处理间隔要大于存盘时间
		{
			p->lastSendTime = now;
			ret = readRecorders(HISDBNAME,p->hisTable,p->selfname,p->where,p->orderfname,TRUE,&fieldcount,&valbuf);
			if(ret > 0)
			{
				pname = (NAME*)valbuf;
				psendmsgbuf = msgbuf.messagebuf;

				psendmsgbuf[0] = p->curStructNo;//曲线结构编号;
				psendmsgbuf += sizeof(unsigned char);
				psendmsgbuf[0] = p->cur_no ;//曲线号
				psendmsgbuf += sizeof(unsigned char);
				psendmsgbuf[0] = p->cur_type ;//曲线类型
				psendmsgbuf += sizeof(unsigned char);

				pro_short2char(&p->savetime,psendmsgbuf);//存盘周期
				psendmsgbuf += sizeof(short);
				
				psendmsgbuf[0] = strlen(p->objName);//对象名长度
				psendmsgbuf += sizeof(unsigned char);
				strcpy(psendmsgbuf,p->objName);
				psendmsgbuf += strlen(p->objName);//对象名

				short valnums;//值个数
				valnums = ret/fieldcount;

				char *pvalnumsbuf = psendmsgbuf;//保存值个数指针
				psendmsgbuf += sizeof(short);
				
				int intval;
				int timepoint,oldtimepoint=-1;
				short count=0;
				for(int i=0;i<valnums;i++)
				{
					timepoint = (int)atof(pname[i*2].name);
					
					if(timepoint == oldtimepoint) continue;
					oldtimepoint =  timepoint;
					
					intval = (int)(atof(pname[i*2+1].name)*1000);
					pro_int2char(&timepoint,psendmsgbuf);//时间点,日曲线是分钟；月曲线是sdate(1970/01/01的天数)
					psendmsgbuf += sizeof(int);
					pro_int2char(&intval,psendmsgbuf);//值
					psendmsgbuf += sizeof(int);
					count++;
				}//for i
				pro_short2char(&count,pvalnumsbuf);
				FREE(pname);
				msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
				writemsg(threadinfo[threadno].sockid,&msgbuf);
			}
		}		
	}
}




//实时曲线预处理
void  RealCurve(int threadno)
{
	THREADMANAGE *pthread = threadinfo + threadno;
	time_t now;
	time( &now );
	struct tm *when = localtime(&now);
	when->tm_year += 1900;
	when->tm_mon += 1;
	for(int i = 0; i < pthread->realcurvecount ; i++)
	{
		REALCURVE *p = pthread->prealcurve + i;
		char con[256];
		memset(con,0,256);
		sprintf(con,"代码 = '%s'",p->objName);
		int fdc;
		void *valbuf = NULL;
		if(p->cur_type == REALYCCURVE)
		{
#ifdef USE_YC_DAYTABLE
			sprintf(p->hisTable,"yc%04d%02d%02d",when->tm_year,when->tm_mon,when->tm_mday);
#else
			sprintf(p->hisTable,"yc%04d%02d",when->tm_year,when->tm_mon);
#endif
			if(readRecorders(DBNAME,"遥测参数表","存盘周期",con,"",TRUE,&fdc,&valbuf)>0)
			{
				NAME *pname = (NAME *)valbuf;
				p->savetime = (short)atoi(pname->name);
				strcpy(p->selfname,"time,data");
				strcpy(p->orderfname,"time");
				FREE(pname);
			}
			else
				continue;
		}
		else
		{
			sprintf(p->hisTable,"kwh%04d%02d",when->tm_year,when->tm_mon);
			if(readRecorders(DBNAME,"电度参数表","存盘周期",con,"",TRUE,&fdc,&valbuf)>0)
			{
				NAME *pname = (NAME *)valbuf;
				p->savetime = (short)atoi(pname->name);
				strcpy(p->selfname,"time,data");
				strcpy(p->orderfname,"time");
				FREE(pname);
			}
			else
				continue;
		}
	//	memset(p->where,0,128);
		sprintf(p->where,"name ='%s' and sdate = %d and flag<50",p->objName,(int)now/(3600*24));//HISDATA_REAL);
	}
	getSendRealCurveData(threadno);
}


#define AnalogID	0
#define StationID	1
#define RtuSn		2
void readAnalogView(int threadno,int paramClassify,char* param)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	if(paramClassify == AnalogID)
	{
		sprintf(where,"代码='%s'",param);
	}
	else
	{
		printf("readAnalogView 暂时未处理的参数类别 \n");
		return;
	}
	int ret = readRecorders(DBNAME,"模拟量视图","代码,描述,站所线系代码,设备代码,终端序号",where,"代码",FALSE,&fieldcount,&valbuf);
	
	MESSAGE msgbuf;
	msgbuf.type = S_ANALOGVIEW;
	char *buf = msgbuf.messagebuf ;
	short recCount = ret/fieldcount;
	
	pro_short2char(&recCount,buf);//记录个数
	buf += sizeof(short);
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	unsigned char len = 0;
	for(int i=0;i<recCount;i++)
	{
		len = strlen(pname[i*fieldcount].name); //长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount].name,len);//字符串 
		buf += len;

		len = strlen(pname[i*fieldcount+1].name); //长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+1].name,len);//字符串 
		buf += len;

		len = strlen(pname[i*fieldcount+2].name); //长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+2].name,len);//字符串 
		buf += len;

		len = strlen(pname[i*fieldcount+3].name); //长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+3].name,len);//字符串 
		buf += len;

		int rtusn=atoi(pname[i*fieldcount+4].name);  //终端序号
		pro_int2char(&rtusn,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("发送模拟量视图数据 \n");
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}


//lcg 2011-07-21修改返回给客户端终端参数表内容，用于分区权限的实现和控制。
//groupno目前暂时不使用。
void readRtuPara(int threadno,int groupno)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	sprintf(where,"使用标志=1");
	int ret = readRecorders(DBNAME,"终端参数表","分组序号,终端序号,代码,描述,安全级,站所线系代码",where,"终端序号",FALSE,&fieldcount,&valbuf);
	
	MESSAGE msgbuf;
	msgbuf.type = S_RTUNAME;
	char *buf = msgbuf.messagebuf ;
	short rtucount = ret/fieldcount;
	
	pro_short2char(&rtucount,buf);//RTU个数
	buf += sizeof(short);
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	unsigned char len = 0;
	for(int i=0;i<rtucount;i++)
	{
		int rtugroupno = atoi(pname[i*fieldcount].name );//分组号
		pro_int2char(&rtugroupno,buf);
		buf += sizeof(int);
		
		int rtuid=atoi(pname[i*fieldcount+1].name );    //终端序号
		pro_int2char(&rtuid,buf);
		buf += sizeof(int);

		len = strlen(pname[i*fieldcount+2].name); //代码长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+2].name,len);//代码
		buf += len;
		
		len = strlen(pname[i*fieldcount+3].name); //描述长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+3].name,len);//描述
		buf += len;

		int safelev=atoi(pname[i*fieldcount+4].name );  //安全级
		pro_int2char(&safelev,buf);
		buf += sizeof(int);

		len = strlen(pname[i*fieldcount+5].name); //站所线系代码长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+5].name,len);//站所线系代码
		buf += len;
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("发送终端参数表参数\n");
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}

//传送站所线系参数表,group
void  readRTU(int threadno,int groupno)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
//	if(groupno != 0xfff)//全部分组的站所线系
//	{
	sprintf(where,"代码 in(select 站所线系代码 from 终端参数表 where 使用标志=1)");
//	}
//	printf("%s\n","分组序号=%d and 代码 in(select 站所线系代码 from 终端参数表 where 使用标志=1)");
	int ret = readRecorders(DBNAME,"站所线系参数表","代码,描述",where,"描述",FALSE,&fieldcount,&valbuf);
		
	if(ret>0) {
		MESSAGE msgbuf;
		msgbuf.type = S_STATION;
		char *buf = msgbuf.messagebuf ;
		short rtucount = ret/fieldcount;
		
		pro_short2char(&rtucount,buf);//RTU个数
		buf += sizeof(short);
		
		NAME *pname = NULL;
		pname = (NAME*)valbuf;
		valbuf = NULL;
		unsigned char len = 0;
		for(int i=0;i<rtucount;i++)
		{
// 			int rtugroupno = atoi(pname[i*fieldcount].name );//分组号
// 			pro_int2char(&rtugroupno,buf);
// 			buf += sizeof(int);
			
			len = strlen(pname[i*fieldcount].name); //代码长度
			buf[0] = len;
			buf += sizeof(unsigned char);
			memcpy(buf,pname[i*fieldcount].name,len);//代码
			buf += len;

			len = strlen(pname[i*fieldcount+1].name); //描述长度
			buf[0] = len;
			buf += sizeof(unsigned char);
			memcpy(buf,pname[i*fieldcount+1].name,len);//描述
			buf += len;
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		printf("发送站所线系代码\n");
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		FREE(pname);
	}
}


//发送遥测代码,发送电度代码
//20110721 lcg修改读取终端参数表获得对应的遥测代码
void  readYCCode(int threadno,char *tablename,char *rtucode)
{
	int fieldcount;
	void *valbuf = NULL;
	char where[256];
	memset(where,0,256);
	
/*
	//WEB曲线查看界面中过滤虚拟终端
	//sprintf(where,"站所线系代码=\'%s\'",rtucode);
	sprintf(where,"显示标志=1 and 终端序号 in (select 终端序号 from 终端参数表 where 使用标志=1 and 站所线系代码=\'%s\')",rtucode);
	printf("%s\n",where);

	int ret = readRecorders(DBNAME,tablename,"代码,描述",where,"描述",FALSE,&fieldcount,&valbuf);
*/
	
	sprintf(where,"显示标志=1 and 终端序号=%s",rtucode);
	printf("%s\n",where);
	
	int ret = readRecorders(DBNAME,tablename,"代码,描述,安全级",where,"描述",FALSE,&fieldcount,&valbuf);

	if(ret<0) fieldcount = 2;	
	MESSAGE msgbuf;
	msgbuf.type = S_YCCODE;
	char *buf = msgbuf.messagebuf ;
	short rtucount = ret/fieldcount;
	
	if(ret <= 0) //此站没有遥测
	{
		pro_short2char(&rtucount,buf);
		buf += sizeof(short);
		msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
		printf("没有\"%s\"的遥测代码\n",rtucode);
		writemsg(threadinfo[threadno].sockid,&msgbuf);
		return;
	}
	pro_short2char(&rtucount,buf);//代码个数
	buf += sizeof(short);

	unsigned char len = 0;
	
	len = (unsigned char)strlen(rtucode);
	buf[0] = len;//厂站代码长度
	buf += sizeof(unsigned char);
	memcpy(buf,rtucode,len);//厂站代码
	buf += len;
	
	NAME *pname = NULL;
	pname = (NAME*)valbuf;
	valbuf = NULL;
	for(int i=0;i<rtucount;i++)
	{
		len = strlen(pname[i*fieldcount].name); //代码长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount].name,len);//代码
		buf += len;

		len = strlen(pname[i*fieldcount+1].name); //描述长度
		buf[0] = len;
		buf += sizeof(unsigned char);
		memcpy(buf,pname[i*fieldcount+1].name,len);//描述
		buf += len;

		int safelev=atoi(pname[i*fieldcount+2].name );  //安全级
		pro_int2char(&safelev,buf);
		buf += sizeof(int);
	}
	msgbuf.length = abs((int)(msgbuf.messagebuf-buf));
	printf("发送\"%s\"的遥测代码\n",rtucode);
	writemsg(threadinfo[threadno].sockid,&msgbuf);
	FREE(pname);
}


//bwait,在停止刷新流时，是否需要让线程Sleep
void removeStream(THREADMANAGE *threadinfo1,int winno,int threadno,int bwait = FALSE)
{
	//处理注销流
	if(threadinfo1->streamcount <= 0) return;
	if(winno == 0xff)//注销该客户端的全部流
	{
		#ifdef WIN32
			EnterCriticalSection(&threadinfo1->critThread);
		#endif

		MULTISTREAM *pmultistream = threadinfo1->pmultistream;
		MULTISTREAM *poldmultistream = pmultistream;
		for(int i = 0; i < threadinfo1->streamcount; i++)
		{
			ASSERT(pmultistream != NULL);

			threadinfo1->prdb_quickstream->RdbStopFresh( BASEWINNO * threadno + threadinfo1->pmultistream->winno);
			#ifndef WIN32
				delay_t(WAIT_TIME);
			#endif
			pmultistream = pmultistream->plist ;
			
			delete poldmultistream;
			poldmultistream = NULL;

			poldmultistream = pmultistream;
			threadinfo1->streamcount--;
		}
		threadinfo1->pmultistream = NULL;
		threadinfo1->streamcount = 0;
		delete threadinfo1->prdb_quickstream ; threadinfo1->prdb_quickstream = NULL;
		#ifdef WIN32
			LeaveCriticalSection(&threadinfo1->critThread);
		#endif

	}
	else //注销单个流
	{
		#ifdef WIN32
			EnterCriticalSection(&threadinfo1->critThread);
		#endif
		MULTISTREAM *pmultistream = threadinfo1->pmultistream;
		MULTISTREAM *poldmultistream = pmultistream;
		for(int i=0;i<threadinfo1->streamcount;i++)
		{
			ASSERT(pmultistream != NULL);

			if(pmultistream->winno == winno)
			{				

				threadinfo1->prdb_quickstream->RdbStopFresh(BASEWINNO * threadno + winno);
				
				#ifndef WIN32
					delay_t(WAIT_TIME);
				#endif

				pmultistream = pmultistream->plist;
				delete poldmultistream; poldmultistream = NULL;
				
				poldmultistream = pmultistream;
				threadinfo1->streamcount--;
				break;
			}
		}
		if(threadinfo1->streamcount == 0)
		{
			delete threadinfo1->prdb_quickstream ; threadinfo1->prdb_quickstream = NULL;
			threadinfo1->pmultistream = NULL;
		}
		#ifdef WIN32
			LeaveCriticalSection(&threadinfo1->critThread);
		#endif
	}
}

#ifdef WIN32
	void acceptStream(int threadno,HWND hwnd,BOOL bwait=false)
#else 
	void acceptStream(int threadno,int bwait=FALSE)
#endif
{
	THREADMANAGE *threadinfo1 = threadinfo + threadno;
	ASSERT(threadinfo1 != NULL);
	char *pbuf = threadinfo1->prcvdata->messagebuf;
	ASSERT(pbuf != NULL);

	int paranums;//有效点个数
	pro_char2int((char *)(pbuf),&paranums);
	pbuf += sizeof(int);
	

	int winno;////Winno窗口号
	pro_char2int((char *)(pbuf),&winno);
	pbuf += sizeof(int);

	//注销流处理
	if(winno == 0xff)
	{
		//printf("收到注销流消息!\n");
		removeStream(threadinfo1,winno,threadno,bwait);
		return;
	}
	else if(winno<0)
	{
		removeStream(threadinfo1,abs(winno),threadno,bwait);
		return;
	}

	if(paranums<=0) 
	{
		MESSAGE *msg = threadinfo1->psnddata;
		msg->type = S_STREAM;
		msg->length = sizeof(int) * 2;
		paranums = 0;
		pro_int2char(&paranums, (char *)(msg->messagebuf));//有效点个数
		pro_int2char(&winno, (char *)(msg->messagebuf + sizeof(int)));//Winno窗口号
		writemsg(threadinfo1->sockid, msg);
		return;
	}//发送有效点个数不对的失败消息

	int freshperiod;//Freshperiod刷新周期
	pro_char2int((char *)(pbuf), &freshperiod);
	pbuf += sizeof(int);
	
	char username[32],password[32];
	memset(username,0,32);
	memset(password,0,32);
	strncpy(username,USERNAME,strlen(USERNAME));
	strncpy(password,PASSWORD,strlen(PASSWORD));
	
	RDB_FIELD_STRU *fldaddr = (RDB_FIELD_STRU *)calloc(1,sizeof(RDB_FIELD_STRU)*paranums);
	ASSERT(fldaddr);

	unsigned char len = 0;
    
	int *oldorder = (int*)calloc(1,sizeof(int)*paranums);

	NetStatus netstatus;
	int grooupnum = netstatus.GetHostGrpNum();
	int *grpEnm = netstatus.GetHostGrpEnm();
    int validnum = 0;//有效点个数
   
	int i;
	for( i = 0; i < paranums; i++)
	{ 

		int j = 0;
		int groupno=0;
		pro_char2int(pbuf,&groupno);//组号
		pbuf += sizeof(int);

		for(j = 0; j < grooupnum; j++)
		{
			if(groupno == grpEnm[j] && (netstatus.IsMirrorServer(groupno,SCD_SERVER) || netstatus.IsServer(groupno,SCD_SERVER)))
				break;
		}

		if(j < grooupnum)
		{
			oldorder[validnum] = i;
			fldaddr[validnum].groupno = groupno;
			
			len = pbuf[0];//tablename的长度
			pbuf += sizeof(unsigned char);
            if(len > RDB_TABNAME_LEN) len = RDB_TABNAME_LEN;
			memcpy(fldaddr[validnum].tabname,pbuf,len);//tablename的内容
			pbuf += len;

			len = pbuf[0];//objname的长度
			pbuf += sizeof(unsigned char);
			if(len > RDB_OBJNAME_LEN) len = RDB_OBJNAME_LEN;
			memcpy(fldaddr[validnum].objname,pbuf,len);//objname的内容
			pbuf += len;

			len = pbuf[0];//fldname的长度
			pbuf += sizeof(unsigned char);
			if(len > RDB_COLNAME_LEN) len = RDB_COLNAME_LEN;
			memcpy(fldaddr[validnum].fldname,pbuf,len);//fldname的内容
			pbuf += len;
			validnum++;
		}
		else
		{
			int invalidno = paranums-(i-validnum)-1;
			oldorder[invalidno] = i;
			fldaddr[invalidno].groupno = groupno;
			
			len = pbuf[0];//tablename的长度
			pbuf += sizeof(unsigned char);
			if(len > RDB_TABNAME_LEN) len = RDB_TABNAME_LEN;
			memcpy(fldaddr[invalidno].tabname,pbuf,len);//tablename的内容
			pbuf += len;

			len = pbuf[0];//objname的长度
			pbuf += sizeof(unsigned char);
			if(len > RDB_OBJNAME_LEN) len = RDB_OBJNAME_LEN;
			memcpy(fldaddr[invalidno].objname,pbuf,len);//objname的内容
			pbuf += len;

			len = pbuf[0];//fldname的长度
			pbuf += sizeof(unsigned char);
			if(len > RDB_COLNAME_LEN) len = RDB_COLNAME_LEN;
			memcpy(fldaddr[invalidno].fldname,pbuf,len);//fldname的内容
			pbuf += len;
		}
		if(abs((int)(pbuf - threadinfo1->prcvdata->messagebuf )) > (MAXBUFSIZE - sizeof(RDB_FIELD_STRU))) //有可能内存溢出
		{
			paranums = i + 1;
			break;
		}
	}
	MULTISTREAM *pmultistream = threadinfo1->pmultistream;
	MULTISTREAM *poldmultistream = pmultistream;
	i=0;
	if(threadinfo1->streamcount>0)//若已有流
	{
		for(i=0;i<threadinfo1->streamcount;i++)
		{
			ASSERT(pmultistream != NULL );
			if(pmultistream->winno == winno)//窗口号和已有流相同
			{
				#ifdef WIN32
					EnterCriticalSection(&threadinfo1->critThread );
				#endif

				threadinfo1->prdb_quickstream->RdbStopFresh(BASEWINNO*threadno + winno);				
				#ifndef WIN32
					delay_t(WAIT_TIME);
				#endif

				while(pmultistream->pthreadstreamdata)
				{
					delete pmultistream->pthreadstreamdata ;//删除这个流数据
					pmultistream->pthreadstreamdata = NULL;
					break;
				}

				pmultistream->pthreadstreamdata = new CThreadStreamData(paranums);
				ASSERT(pmultistream->pthreadstreamdata != NULL );
				memcpy(pmultistream->pthreadstreamdata->pold_order,oldorder,sizeof(int)*paranums);
				#ifdef WIN32
					LeaveCriticalSection(&threadinfo1->critThread );
				#endif
				break;
			}
			poldmultistream = pmultistream;
			pmultistream = pmultistream->plist ;
		}
	}
	
	if(i >= threadinfo1->streamcount || threadinfo1->streamcount == 0)//没有找到相同winno的流,或没有流数据
	{
		pmultistream = new MULTISTREAM();
		ASSERT(pmultistream);

		pmultistream->plist = NULL;
		pmultistream->winno = winno;
		pmultistream->pthreadstreamdata = new CThreadStreamData(paranums);
		ASSERT(pmultistream->pthreadstreamdata);

		memcpy(pmultistream->pthreadstreamdata->pold_order,oldorder,sizeof(int)*paranums);
		if(threadinfo1->streamcount == 0)
		{
			threadinfo1->prdb_quickstream = new Rdb_QuickStream();
			ASSERT(threadinfo1->prdb_quickstream);

			threadinfo1->pmultistream = pmultistream;
		}
		else
		{
			poldmultistream->plist = pmultistream;
		}
		threadinfo1->streamcount++;
	}
#ifdef WIN32
	int setresult = threadinfo1->prdb_quickstream->RdbSetStreamPara(username,password,freshperiod,validnum,fldaddr,BASEWINNO * threadno + winno,hwnd);//窗口号用threadno取代，因threadno唯一,以便映射.
#else
	sigset(SIGUSR1,recvStream);
	int setresult = threadinfo1->prdb_quickstream->RdbSetStreamPara(username,password,freshperiod,validnum,fldaddr,BASEWINNO * threadno + winno);//窗口号用threadno取代，因threadno唯一,以便映射.
	printf("注册流信息,个数%d,注册返回结果%d\n",validnum,setresult);
#endif
	if(setresult != RDB_OK)
	{//发注册错误信息
		MESSAGE *msg = threadinfo1->psnddata;
		msg->type = S_STREAM;
		msg->length = sizeof(int)*2;
		paranums = -1;
		pro_int2char(&paranums,(char *)(msg->messagebuf));//有效点个数
		pro_int2char(&winno,(char *)(msg->messagebuf+sizeof(int)));//Winno窗口号
		writemsg(threadinfo1->sockid,msg);

		removeStream(threadinfo1,abs(winno),threadno,FALSE);
	}
	FREE(oldorder);
	FREE(fldaddr);

	return;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：sendafile()
 * 功能描述：发送一个文件
 * 参数    ：sockid=客户socket号;env,环境变量，后面不带"\"；path,文件的相对路径，前后都不能带\,filenamepara文件名
 *			 type,传送的文件类型.	sendwrongf,是否发送错误信息
 * 返回值：  -1,打开文件失败;
 *			 -2,读文件错误；成功时，返回发送的文件的长度
 *           -3,发送错误
 *
 * --------------------------------------------------
 */
int sendafile(int threadid,SOCKET sockid,char *env,char* path,char* filenamepara,int type,int sendwrongf = FALSE,int bupdatetable=FALSE)
{

	if(path == NULL || path[0] == '\0') return 0;
	char fn[256];
	memset(fn,0,256); 

	if(filenamepara == NULL && filenamepara[0] == '\0') return 0;
	strcpy(fn,env);
	strcat(fn,"/");
	strcat(fn,path);//文件的相对路径
	strcat(fn,"/");
	strcat(fn,filenamepara);
	printf("发送文件%s到%s\n",fn,threadinfo[threadid].client_addr);
	FStat filestat;
	int fileexist = filestat.isExist(fn);
	int length = 0;
	short fnlength = strlen(filenamepara);
	//							标识         文件路径长度    文件路径        文件名长度(n)  + 文件名
	int prefixlen = MSGHEADLEN + sizeof(unsigned char) + sizeof(short) +strlen(path) + sizeof(short) + fnlength;
	int rtn = 0;
	if(fileexist)
	{
		int openf = filestat.openfile();
		if(openf == TRUE)
		{
			long filelen = filestat.getFileLen();
			if(filelen > 0)
			{
				char *pfilebuf = (char*)malloc(filelen + prefixlen);
				memset(pfilebuf,0,filelen + prefixlen);
				char *pbuf = pfilebuf;
				int realcount = filestat.read(pfilebuf+prefixlen,filelen);
				if(realcount!=filelen)
				{
					rtn = -2;//读文件错误!
				}
				else//发送文件
				{
					length = filelen + prefixlen -  MSGHEADLEN;//报文长度
					pro_int2char(&type,pbuf);//类型
					pbuf += sizeof(int);
					pro_int2char(&length,pbuf);//长度
					pbuf += sizeof(int);
					pbuf += sizeof(int);//报头结束

					pbuf[0] = 0;//标识
					pbuf += sizeof(unsigned char);

					short pathlen = (short)strlen(path);//文件路径长度
					pro_short2char(&pathlen,pbuf);
					pbuf += sizeof(short);

					strcpy(pbuf,path);//文件路径
					pbuf += pathlen;

					pro_short2char(&fnlength,pbuf);//文件名长度
					pbuf += sizeof(short);

					memcpy(pbuf,filenamepara,fnlength);//文件名

					if(send_data(sockid,pfilebuf,filelen + prefixlen) == -1)
					{
						rtn = -3;//发送错误
					}
					else
					{
						rtn = filelen;
						//如果自动需要保持客户端文件的一致
						char sql[MAXSQLWORDLEN];
						memset(sql,0,MAXSQLWORDLEN);
						//IP地址        char(40)        not null,
						//目录			char(128)       not null,
						//文件名		char(128)       not null,
						//年月日		int             not null,
						//时分秒		int				not null
						int yearmonthday,hourminsecond;
						filestat.getmtime(yearmonthday,hourminsecond);

						if(bupdatetable)
						{
							sprintf(sql,"delete %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
								DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
							Execute(DBNAME,sql);

							sprintf(sql,"delete %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
								DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr,path,filenamepara);
							Execute(DBNAME,sql);
							memset(sql,0,MAXSQLWORDLEN);
						}
						sprintf(sql,"insert into %s.%s%s values('%s','%s','%s',%d,%d)",
							 DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr,path,filenamepara,yearmonthday,hourminsecond);
						Execute(DBNAME,sql);
					//	_delay_time(2000*1000);
					//	sprintf(sql,"delete from %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
						//		DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
					//	Execute(DBNAME,sql);
					}
				}
				FREE(pfilebuf);
			}
			filestat.close();
			return rtn;
		}
	}
	rtn = -1;//打开文件失败

	if(sendwrongf && rtn<0)
	{
		//客户端请求发送某一文件时，要发错误信息
		char *pbuf = (char*)malloc(prefixlen*sizeof(char));
		memset(pbuf,0,prefixlen*sizeof(char));
		char *pbuf1 = pbuf;
		length = prefixlen -  MSGHEADLEN;//报文长度
		pro_int2char(&type,pbuf1);//类型
		pbuf1 += sizeof(int);
		pro_int2char(&length,pbuf1);//长度
		pbuf1 += sizeof(int);
		pbuf1 += sizeof(int);//报头结束

		pbuf1[0] = rtn;//标识
		pbuf1 += sizeof(unsigned char);

		short pathlen = (short)strlen(path);//文件路径长度
		pro_short2char(&pathlen,pbuf1);
		pbuf1 += sizeof(short);

		strcpy(pbuf1,path);//文件路径
		pbuf1 += pathlen;

		pro_short2char(&fnlength,pbuf1);//文件名长度
		pbuf1 += sizeof(short);

		memcpy(pbuf1,filenamepara,fnlength);//文件名

		send_data(sockid,pbuf,prefixlen);
		FREE(pbuf);
	}
	return rtn;
}


#ifndef WIN32
void addPathList(DIRLIST **dirlist,char *pathname)
{
	if(*dirlist == NULL)
	{
		*dirlist = new DIRLIST;
		(*dirlist)->dirlist = NULL;
		memset((*dirlist)->path,0,256);
		strcpy((*dirlist)->path,pathname);
		(*dirlist)->bsendf = FALSE;
	}
	else
	{
		DIRLIST *pdirlisthead = *dirlist;
		DIRLIST *pdirlistend = (*dirlist)->dirlist;
		while(pdirlistend)
		{
			pdirlisthead = pdirlistend;
			pdirlistend = pdirlistend->dirlist;
		}
		pdirlisthead->dirlist = new DIRLIST;
		pdirlisthead->dirlist->dirlist = NULL;
		memset(pdirlisthead->dirlist->path,0,256);
		strcpy(pdirlisthead->dirlist->path,pathname);
		pdirlisthead->dirlist->bsendf = FALSE;
	}
	return;
}


void dealDirSendFile(DIRLIST **dirlist,char *filen,FStat fstat,char *path,char *env,int threadid,int sockid)
{
	DIR *dirp = opendir(filen);
	struct dirent *dp;
	while (dirp) {
	 errno = 0;
	 if ((dp = readdir(dirp)) != NULL) {
		 char filetmp[256];
		 memset(filetmp,0,256);
		 strcpy(filetmp,filen);
		 strcat(filetmp,"/");
		 strcat(filetmp,dp->d_name);
		 if(fstat.isExist(filetmp))
		 {
			 if(fstat.isDir())//是目录
			 {
				 if(specifyDir(dp->d_name))
				 {
					 char pathtmp[256];
					 memset(pathtmp,0,256);
					 strcpy(pathtmp,path);
					 strcat(pathtmp,"/");
					 strcat(pathtmp,dp->d_name);
					 addPathList(dirlist,pathtmp);
				 }
			 }
			 else
			 {
				 sendafile(threadid,sockid,env,path,dp->d_name,S_ALLFILE,FALSE);
			 }
		 }//if(fstat.isExist(filetmp))
	 }//if ((dp = readdir(dirp)) != NULL) 
	 else
	 {
		if (errno == 0) {
			 closedir(dirp);
			 return ;
		 }//if (errno == 0) 
		 closedir(dirp);
		 return ;
		}//else
	}//while
	return;
}




void dealDir(DIRLIST **dirlist,FILELIST **ppfilelist,FILELIST **ppfilelist1,char *filen,FStat fstat,char *path,char *env)
{
	DIR *dirp = opendir(filen);
	struct dirent *dp;
	while (dirp) {
	 errno = 0;
	 if ((dp = readdir(dirp)) != NULL) {
		 char filetmp[256];
		 memset(filetmp,0,256);
		 strcpy(filetmp,filen);
		 strcat(filetmp,"/");
		 strcat(filetmp,dp->d_name);
		 if(fstat.isExist(filetmp))
		 {
			 if(fstat.isDir())//是目录
			 {
				 if(specifyDir(dp->d_name))
				 {
					 char pathtmp[256];
					 memset(pathtmp,0,256);
					 strcpy(pathtmp,path);
					 strcat(pathtmp,"/");
					 strcat(pathtmp,dp->d_name);
					 addPathList(dirlist,pathtmp);
				 }
			 }
			 else
			 {
				 if(*ppfilelist == NULL)
				 {
					 *ppfilelist = new FILELIST();
					 *ppfilelist1 = *ppfilelist;
					 strcpy((*ppfilelist)->filename,dp->d_name);
					 strcpy((*ppfilelist)->path,path);
					 fstat.getmtime((*ppfilelist)->yearmonday,(*ppfilelist)->hourminsec);					 
				 }
				 else
				 {
					 FILELIST *p = *ppfilelist;
					 while(p->pfilelist != NULL)
					 {
						 p = p->pfilelist ;
					 }
					p->pfilelist = new FILELIST();
					strcpy(p->pfilelist->path,path);
					strcpy(p->pfilelist->filename,dp->d_name);
					fstat.getmtime(p->pfilelist->yearmonday,p->pfilelist->hourminsec);
					
				}
			 }
		 }//if(fstat.isExist(filetmp))
	 }//if ((dp = readdir(dirp)) != NULL) 
	 else
	 {
		if (errno == 0) {
			 closedir(dirp);
			 return ;
		 }//if (errno == 0) 
		 closedir(dirp);
		 return ;
		}//else
	}//while
	return;
}

#endif

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：sendfile()
 * 功能描述：发送某一目录下的所有文件
 * 参数    ：sockid=客户socket号;
 *           env,运行环境变量;path,文件的相对路径，最后都不能带\
 *			 filenamepara,不带路径的文件名
 * 返回值：  -1,打开文件失败;
 *			 -2,读文件错误；成功时，返回发送的文件的长度
 *			 
 *           
 *
 * --------------------------------------------------
 */
int sendfile(int threadid,SOCKET sockid,char *env,char *path,char *filenamepara)
{
#ifdef WIN32
	WIN32_FIND_DATA FileData; 
	
	HANDLE hSearch = NULL; 
	BOOL fFinished = FALSE; 
	
	//取得带全路径的文件名
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
	strcat(filen,"/");
	strcat(filen,filenamepara);

	hSearch = FindFirstFile(filen, &FileData); 
	if(hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return 0;
	}
	else
	{
		if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//是目录文件
		{
			if(specifyDir(FileData.cFileName))
			{
				char path1[256];
				memset(path1,0,256);
				strcpy(path1,path);
				strcat(path1,"/");
				strcat(path1,FileData.cFileName);
				sendfile(threadid,sockid,env,path1,"*");//递归调用目录查找文件函数
			}
		}
		else
		{
			sendafile(threadid,sockid,env,path,FileData.cFileName,S_ALLFILE,false);
		}
	}

	while (!fFinished) 
	{ 
		if (!FindNextFile(hSearch, &FileData)) 
		{
			fFinished = TRUE;
		}
		else
		{
			if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//是目录文件
			{
				if(specifyDir(FileData.cFileName))
				{
					char path1[256];
					memset(path1,0,256);
					strcpy(path1,path);
					strcat(path1,"/");
					strcat(path1,FileData.cFileName);
					sendfile(threadid,sockid,env,path1,"*");//递归调用目录查找文件函数
				}
			}
			else
			{
				if(sendafile(threadid,sockid,env,path,FileData.cFileName,S_ALLFILE,false)>=0)
				{
				char sql[256];
				memset(sql,0,256);
				sprintf(sql,"delete %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
				DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,FileData.cFileName);
				Execute(DBNAME,sql);
				//	break;
				}
				else 
					break;
			/*	char sql[256];
				memset(sql,0,256);
				sprintf(sql,"delete from %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
				DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,FileData.cFileName);
				Execute(DBNAME,sql);*/
			}
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
	} 
	return 0;
#else
	//取得带全路径的文件名
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
	DIRLIST *dirlist = NULL;
	if(strcasecmp(filenamepara,"*"))//不是"*"时，考虑为一个文件
	{
		strcat(filen,"/");
		strcat(filen,filenamepara);
	}
	FStat fstat;
	if(fstat.isExist(filen))//文件或目录存在
	{
		if(fstat.isDir())//是目录
		{
			dealDirSendFile(&dirlist,filen,fstat,path,env,threadid,sockid);
		}//if(fstat.isDir())//是目录
		else
		{
			sendafile(threadid,sockid,env,path,filenamepara,S_ALLFILE,FALSE);

		/*	char ssql1[256];
			memset(ssql1,0,256);
			sprintf(ssql1,"delete from %s.%s.%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
			DBNAME,"dbo",WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
			Execute(DBNAME,ssql1);*/

			char osql1[256];
			memset(osql1,0,256);
			sprintf(osql1,"delete %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
			DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
			Execute(DBNAME,osql1);
		}
		DIRLIST *pdirlist = dirlist;
		while(pdirlist)
		{
			if(pdirlist->bsendf == FALSE)
			{
				memset(filen,0,256);
				strcpy(filen,env);
				strcat(filen,"/");
				strcat(filen,pdirlist->path);
				if(fstat.isExist(filen))
				{
					if(fstat.isDir())//是目录
					{
						dealDirSendFile(&dirlist,filen,fstat,pdirlist->path,env,threadid,sockid);
					}//if(fstat.isDir())//是目录
				}
				pdirlist->bsendf == TRUE;
			}
			pdirlist = pdirlist->dirlist;
		}

		//开始释放内存
		pdirlist = dirlist;
		while(pdirlist)
		{
			DIRLIST *dirlistend = pdirlist->dirlist;
			delete pdirlist;
			pdirlist = NULL;
			pdirlist = dirlistend;
		}
	}//if(fstat.isExist(filen))//文件或目录存在
	return 0;

#endif
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：getPathFile()
 * 功能描述：读取目录下的所有文件
 * 参数    ：env,环境变量;path，路径
 * 返回值：  
 *			 
 *           
 *
 * --------------------------------------------------
 */
FILELIST * getPathFile(char *env,char *path)
{
#ifdef WIN32
	//取得带全路径的文件名
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"\\");
	strcat(filen,path);
	strcat(filen,"\\");
	strcat(filen,"*");

	WIN32_FIND_DATA FileData; 
	
	HANDLE hSearch = NULL; 
	BOOL fFinished = FALSE; 

	SYSTEMTIME lastwritetime,localtime;

    FILELIST *pfilelist = NULL;
    FILELIST *pfilelist1 = NULL;
	hSearch = FindFirstFile(filen, &FileData); 
	if(hSearch == INVALID_HANDLE_VALUE) 
	{ 
		return NULL;
	}
	else
	{
		if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//是目录文件
		{
			if(specifyDir(FileData.cFileName))
			{
				char path1[256];
				memset(path1,0,256);
				strcpy(path1,path);
				strcat(path1,"\\");
				strcat(path1,FileData.cFileName);
				if(pfilelist == NULL) pfilelist = getPathFile(env,path1);//递归调用目录查找文件函数
				else
				{
					while(pfilelist->pfilelist != NULL)
						pfilelist = pfilelist->pfilelist;
					pfilelist->pfilelist = getPathFile(env,path1);//递归调用目录查找文件函数
				}
				if(pfilelist1 == NULL) pfilelist1 = pfilelist;
			}
		}
		else
		{
			
			if(pfilelist == NULL) pfilelist = (FILELIST*)new FILELIST();
			else 
			{
				while(pfilelist->pfilelist != NULL)
					pfilelist = pfilelist->pfilelist;
				pfilelist->pfilelist = (FILELIST*)new FILELIST();
				pfilelist = pfilelist->pfilelist ;
			}
			strcpy(pfilelist->filename,FileData.cFileName);
			strcpy(pfilelist->path,path);
			pfilelist->pfilelist = NULL;
			FileTimeToSystemTime(&FileData.ftLastWriteTime,&lastwritetime);
			SystemTimeToTzSpecificLocalTime(NULL,&lastwritetime,&localtime);
			pfilelist->yearmonday = localtime.wYear*10000 + localtime.wMonth *100 + localtime.wDay ;
			pfilelist->hourminsec = localtime.wHour*10000 + localtime.wMinute*100 + localtime.wSecond ;
			if(pfilelist1 == NULL)
				pfilelist1 = pfilelist;			
		}
	}

	while (!fFinished) 
	{ 
		if (!FindNextFile(hSearch, &FileData)) 
		{
			fFinished = TRUE;
		}
		else
		{
			if((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)//是目录文件
			{
				if(specifyDir(FileData.cFileName))
				{
					char path1[256];
					memset(path1,0,256);
					strcpy(path1,path);
					strcat(path1,"\\");
					strcat(path1,FileData.cFileName);
					if(pfilelist == NULL) pfilelist = getPathFile(env,path1);//递归调用目录查找文件函数
					else
					{
						while(pfilelist->pfilelist != NULL)
							pfilelist = pfilelist->pfilelist;
						pfilelist->pfilelist = getPathFile(env,path1);//递归调用目录查找文件函数
					}
					if(pfilelist1 == NULL) pfilelist1 = pfilelist;
				}
			}
			else
			{
				if(pfilelist == NULL) pfilelist = (FILELIST*)new FILELIST();
				else 
				{
					while(pfilelist->pfilelist != NULL)
						pfilelist = pfilelist->pfilelist;
					pfilelist->pfilelist = (FILELIST*)new FILELIST();
					pfilelist = pfilelist->pfilelist ;
				}
				strcpy(pfilelist->filename,FileData.cFileName);
				strcpy(pfilelist->path,path);
				pfilelist->pfilelist = NULL;
				FileTimeToSystemTime(&FileData.ftLastWriteTime,&lastwritetime);
				SystemTimeToTzSpecificLocalTime(NULL,&lastwritetime,&localtime);
				pfilelist->yearmonday = localtime.wYear*10000 + localtime.wMonth *100 + localtime.wDay ;
				pfilelist->hourminsec = localtime.wHour*10000 + localtime.wMinute*100 + localtime.wSecond ;
				if(pfilelist1 == NULL)
					pfilelist1 = pfilelist;
			}
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
	} 
	return pfilelist1;
#else
	//取得带全路径的文件名
	char filen[256];
	memset(filen,0,256);
	strcpy(filen,env);
	strcat(filen,"/");
	strcat(filen,path);
    FILELIST *pfilelist = NULL;
    FILELIST *pfilelist1 = NULL;
	DIRLIST *dirlist = NULL;
	FStat fstat;
	if(fstat.isExist(filen))//文件或目录存在
	{
		if(fstat.isDir())//是目录
		{
			dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,path,env);
		}//if(fstat.isDir())//是目录
		DIRLIST *pdirlist = dirlist;
		while(pdirlist)
		{
			if(pdirlist->bsendf == FALSE)
			{
				memset(filen,0,256);
				strcpy(filen,env);
				strcat(filen,"/");
				strcat(filen,pdirlist->path);
				if(fstat.isExist(filen))
				{
					if(fstat.isDir())//是目录
					{
						dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,pdirlist->path,env);
					}//if(fstat.isDir())//是目录
				}
				pdirlist->bsendf == TRUE;
			}
			pdirlist = pdirlist->dirlist;
		}

		//开始释放内存
		pdirlist = dirlist;
		while(pdirlist)
		{
			DIRLIST *dirlistend = pdirlist->dirlist;
			delete pdirlist;
			pdirlist = NULL;
			pdirlist = dirlistend;
		}
	}//if(fstat.isExist(filen))//文件或目录存在
	return pfilelist1;
#endif
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：updateClientFile()
 * 功能描述：更新客户端文件
 * 参数    ：threadid,线程号
 * 返回值：  
 * --------------------------------------------------
 */
void updateClientFile(int threadid)
{
	char where[64];
	memset(where,0,64);
	sprintf(where,"ip地址='%s'",threadinfo[threadid].client_addr);
	int fieldcount;
	void *buf = NULL;
	int retcount = readRecorders(DBNAME,WEBVISITTABLE,"ip地址",where,"ip地址",TRUE,&fieldcount,&buf);
	if(buf != NULL)
	{
		free(buf);
		buf = NULL;
	} 
	if(retcount <= 0)
	{
		char sql[256];
		memset(sql,0,256);
		
		sprintf(sql,"insert into %s.%s%s values('%s','graph','graph.lib',19700101,101010)",DBNAME,DBO,WEBVISITTABLE,threadinfo[threadid].client_addr);
		Execute(DBNAME,sql);
	}
	else 
	{
		retcount = readRecorders(DBNAME,WEBUPDATETABLE,"目录,文件名,ip地址",where,"目录,文件名,ip地址",FALSE,&fieldcount,&buf);
		//retcount = readRecorders(DBNAME,WEBUPDATETABLE,"ip地址",where,"ip地址",FALSE,&fieldcount,&buf);
		if(retcount>0)
		{
			
		//	char sql[256];
		//	memset(sql,0,256);
		//	sprintf(sql,"delete from %s.%s%s where ip地址='%s'",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr);
		//	Execute(DBNAME,sql);

			char envvar[256];//环境变量,程序会自动去掉后面的"\"
			memset(envvar,0,256);
			getEnv(envvar);
			int row = retcount/fieldcount;
			NAME *pname = (NAME*)buf;
			for(int i=0;i<row;i++)
			{
				sendafile(threadid,threadinfo[threadid].sockid,envvar,pname[i*fieldcount].name,pname[i*fieldcount+1].name,S_FILE,FALSE,TRUE);						
			//	sprintf(sql,"delete from %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,pname[i*fieldcount].name,pname[i*fieldcount+1].name);
			//	Execute(DBNAME,sql);
			}
			//FREE(pname);
		}
		if(buf != NULL)
		{
			free(buf);
			buf = NULL;
		} 
	}
}


int getLineHyperlink(const char* lineID,char** hyperlinkFile)
{
	int linkfileCnt=0;
	int rcdcnt,fieldCnt;	
	char chsWhere[255];
	void   *valbuf1,*valbuf2;
	NAME *pname1=NULL,*pname2=NULL;
	//
	char chsStations[255];
	char *pChBuf=(char*)hyperlinkFile;
	
	sprintf(chsWhere,"name='%s'",lineID);
	rcdcnt=readRecorders(DBNAME,"DEV_LINE","name,sname",chsWhere,"",TRUE,&fieldCnt,&valbuf1); //可能将来会返回多条记录
	if(rcdcnt>0)
	{
		pname1 = (NAME*)valbuf1;
		int rcdval=rcdcnt;
		for(int i=0;i<rcdval;i++)
		{
			strcpy(chsStations,pname1[i].name);
			sprintf(chsWhere,"station='%s'",chsStations);
			
			rcdcnt=readRecorders(DBNAME,"FILEINFO","filename",chsWhere,"",TRUE,&fieldCnt,&valbuf2); 
			if(rcdcnt>0)
			{
				pname2 = (NAME*)valbuf2;
				for(int j=0;j<rcdcnt/fieldCnt;j++)
				{
					strcpy(pChBuf,pname2[j].name);
					pChBuf+=CHAR_BUF_LEN;
					linkfileCnt++;
				}
				FREE(pname2);
			}
		}
		FREE(pname1);
	}
	return linkfileCnt;
}

void SetPropToBuff(char* &msgBuf,const char* propname,const char* propval)
{
	short fieldlen;
	
	fieldlen=strlen(propname);
	pro_short2char(&fieldlen,msgBuf);
	msgBuf+=sizeof(short int);
	strncpy(msgBuf,propname,fieldlen);     //属性的头内容
	
	msgBuf+=fieldlen;
	
	fieldlen=strlen(propval);
	pro_short2char(&fieldlen,msgBuf);
	msgBuf+=sizeof(short int);
	strncpy(msgBuf,propval,fieldlen);      //属性的内容
	msgBuf+=fieldlen;
}

/************************************************************************/
/* 由设备的类型编号获得设备类型的描述信息                               */
/* 函数由scd设备类型表中获得信息                                        */
/************************************************************************/
int getDevTypeDesc(const char* devtypeno,char* devTypeDesc){
	int succ=0;
	int fieldCnt;
	int rcdcnt;
	void *valbuf;
	char chsWhere[256];
	sprintf(chsWhere,"type='%s'",devtypeno);
	rcdcnt=readRecorders(DBNAME,"DEVTYPE","describe",chsWhere,"",false,&fieldCnt,&valbuf);
	if(rcdcnt>0){
		NAME* pname=(NAME*)valbuf;
		strcpy(devTypeDesc,pname[0].name);
		succ=1;
		FREE (pname);
	}
	return succ;
}

char *yc_yx_type[]={"1*电流","2*A相电流","3*B相电流","4*C相电流",
"300*开关","301*小车开关","302*母联小车开关","303*母联开关","304*旁母开关","305*旁母兼母联开关","400*状态",
"500*刀闸","501*线路隔离刀","502*I母隔离刀","503*II母隔离刀","504*III母隔离刀","505*IV母隔离刀",
"506*旁母隔离刀","507*主变刀闸","508*站变刀闸","509*PT刀闸","510*母联刀闸","541*线路接地刀","542*主变接地刀",
"543*站变接地刀","544*PT接地刀","571*I母接地刀","572*II母接地刀","573*III母接地刀","574*IV 母接地刀","575*旁母接地刀"};

char *break_type[]={"串联刀闸","分支刀闸","接地刀闸"};
/************************************************************************/
/*从“测遥信类型.txt”中获得类型描述信息,放到结构中供查询用             */
/************************************************************************/
int getYXTypeDesc(int type,char* YXTypeDesc){
	//char* chPos;
	//char* chtype;
	int   nTmp;
	char *token;
	char chsBuf[255];
	char seps[]   = "*";
	for(int i=0;i<sizeof(yc_yx_type)/sizeof(char*);i++)
	{
		strcpy(chsBuf,yc_yx_type[i]);
		token = strtok(chsBuf, seps );
		nTmp=atoi(token);
		if(nTmp==type)
		{
			strcpy(YXTypeDesc,token = strtok( NULL, seps ));
			return TRUE;
		}
	}
	return FALSE;
}


/************************************************************************/
/*设备参数采用属性名+属性值的方式。
/*@devtypedesc 设备所属的层名，表示设备类型。
/*@codedesc 设备的描述名，唯一。
/*svg查询，发送设备参数表给客户端
/*暂时的使用，更换平台后，表名和字段都将被更改。
/*需要注意的是这里传递的是设备的描述。
/*
/************************************************************************/
int sendDevParaTable(int threadno,char* devtypedesc ,char* codedesc){
	MESSAGE msgbuf;
	msgbuf.type = S_DEVPARA;
	char *psendmsgbuf = msgbuf.messagebuf;
	
	short  fieldlen;
	char   chsTmp[256];

	int    fieldCnt;
	int    rcdcnt;
	void   *valbuf;
	char   chsWhere[256];
	
	short proppagecnt;
	char  *proppagecntpos;   //用于保存在缓冲区中属性个数的位置，用于标识本缓冲区中的属性页的个数

	char chsdevid[CODESIZE];
	
	
	/*
	memset(chsdevid,0,sizeof(chsdevid));
	strcpy(chsdevid,codedesc);
	readstation(chsdevid);
	if(chsdevid[0]=='\0'){
	return 0;					//在实时库scddevide表中为获得相应描述的代码。
	}
	*/
	strcpy(chsdevid,codedesc);
	if(chsdevid[0]=='\0') return 0;
	

	//发送遥测参数到客户端
	if(strcmpNoCase(devtypedesc,DEV_MEAS)==0 ){
		//设置设备帧类型
		fieldlen=DEV_FRAME_STYLE_DEVPROP;
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		
		proppagecntpos=psendmsgbuf; //保留位置
		proppagecnt=1;
		pro_short2char(&proppagecnt,proppagecntpos);
		psendmsgbuf += sizeof(short int);
		
		sprintf(chsWhere,"代码='%s'",chsdevid);
		printf("从数据库中读取信息:[%s]\n",chsWhere);

		rcdcnt=readRecorders(DBNAME,"遥测参数表","代码,描述,设备代码,遥测序号,存盘周期,事项处理方式,警告上限,警告下限,警报上限,警报下限,计算量标志,转发标志",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		
		printf("\n readcord rcdcnt=%d  fieldcnt=%d",rcdcnt,fieldCnt);
		if(rcdcnt>0){
			NAME* pname=NULL;
			pname = (NAME*)valbuf;
			
			strcpy(chsTmp,"遥测参数");
			fieldlen=strlen(chsTmp);
			
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //属性的头内容
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt;
			pro_short2char(&fieldlen,psendmsgbuf);     //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"代码",pname[0].name);
			SetPropToBuff(psendmsgbuf,"描述",pname[1].name);
			sprintf(chsTmp,"%s",pname[2].name);
			readstation(chsTmp);
			SetPropToBuff(psendmsgbuf,"所属站名",chsTmp);
			SetPropToBuff(psendmsgbuf,"遥测序号",pname[3].name);
			SetPropToBuff(psendmsgbuf,"存盘周期",pname[4].name);
			SetPropToBuff(psendmsgbuf,"事项处理方式",pname[5].name);
			SetPropToBuff(psendmsgbuf,"警告上限",pname[6].name);
			SetPropToBuff(psendmsgbuf,"警告下限",pname[7].name);
			SetPropToBuff(psendmsgbuf,"警报上限",pname[8].name);
			SetPropToBuff(psendmsgbuf,"警报下限",pname[9].name);
			SetPropToBuff(psendmsgbuf,"计算量标志",pname[10].name);
			SetPropToBuff(psendmsgbuf,"转发标志",pname[11].name);
			//pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);
			msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
			return writemsg(threadinfo[threadno].sockid,&msgbuf);
		}		
	}




	//处理特定的超链接参数,目前使用数据库中关联参数来获得线路上所有的对应图形文件名。

	if(strcmpNoCase(devtypedesc,DEV_LINE_HYPERLINK)==0){
		short  filecnt=0;
		char file[HYPERLINK_MAX_FILENAME][CHAR_BUF_LEN];  //设置做多文件为10个，每个文件名长度为255个字符。
		//设置设备帧类型
		fieldlen=DEV_FRAME_STYLE_HYPERLINK;
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		filecnt=getLineHyperlink(chsdevid,(char**)file);

		pro_short2char(&filecnt,psendmsgbuf);
		psendmsgbuf += sizeof(short int);

		for(int i=0;i<filecnt;i++)
		{
			fieldlen=strlen(file[i]);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,file[i],fieldlen);              //文件名
			psendmsgbuf+=fieldlen;
		}
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}

	//设置设备帧类型
	fieldlen=DEV_FRAME_STYLE_DEVPROP;
	pro_short2char(&fieldlen,psendmsgbuf);
	psendmsgbuf += sizeof(short int);
		
	proppagecntpos=psendmsgbuf; //保留位置
	proppagecnt=0;
	pro_short2char(&proppagecnt,proppagecntpos);
	psendmsgbuf += sizeof(short int);
	
	sprintf(chsWhere,"代码='%s'",chsdevid);
	rcdcnt=readRecorders(DBNAME,"scd设备视图","代码,描述,站所线系代码,类型",chsWhere,"",TRUE,&fieldCnt,&valbuf);
	if(rcdcnt>0){
		NAME* pname=NULL;
		pname = (NAME*)valbuf;
		
		strcpy(chsTmp,"SCD设备");
		fieldlen=strlen(chsTmp);
		
		pro_short2char(&fieldlen,psendmsgbuf);
		psendmsgbuf += sizeof(short int);
		strncpy(psendmsgbuf,chsTmp,fieldlen);              //属性的头内容
		psendmsgbuf+=fieldlen;
		
		fieldlen = fieldCnt;
		pro_short2char(&fieldlen,psendmsgbuf);     //一个表示属性的个数，注意要*2
		psendmsgbuf+=sizeof(short int);
		
		SetPropToBuff(psendmsgbuf,"代码",pname[0].name);
		SetPropToBuff(psendmsgbuf,"描述",pname[1].name);
		sprintf(chsTmp,"%s",pname[2].name);
		readstation(chsTmp);
		SetPropToBuff(psendmsgbuf,"所属站名",chsTmp);
		
		memset(chsTmp,0,sizeof(chsTmp));
		if(getDevTypeDesc(pname[3].name,chsTmp)){
			SetPropToBuff(psendmsgbuf,"类型",chsTmp);
		}
		else{
			SetPropToBuff(psendmsgbuf,"类型",pname[3].name);
		}
		//SetPropToBuff(psendmsgbuf,"型号","");
		proppagecnt++;
		pro_short2char(&proppagecnt,proppagecntpos);
		FREE (pname);
	}
		
	if(strcmpNoCase(devtypedesc,DEV_BREAK)==0 || strcmpNoCase(devtypedesc,DEV_SWITCH)==0)
	{
		BOOL bIsBreak; 
		if(strcmpNoCase(devtypedesc,DEV_BREAK)==0)bIsBreak=TRUE;else bIsBreak=FALSE;
		if(bIsBreak)
			sprintf(chsWhere,"代码='%s%s'",chsdevid,BREAK_POSTFIX);
		else
			sprintf(chsWhere,"代码='%s%s'",chsdevid,SWITCH_POSTFIX);

		rcdcnt=readRecorders(DBNAME,"遥信参数表","代码,描述,类型,是否追忆,是否遥控,终端序号,遥信序号,事项处理方式,推图名,SOE句,事项句",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;

			strcpy(chsTmp,"SCD遥信");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //属性的头内容
			psendmsgbuf+=fieldlen;

			fieldlen = fieldCnt;
			pro_short2char(&fieldlen,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"代码",pname[0].name);
			SetPropToBuff(psendmsgbuf,"描述",pname[1].name);
			
			memset(chsTmp,0,sizeof(chsTmp));
			getYXTypeDesc(atoi(pname[2].name),chsTmp);
			SetPropToBuff(psendmsgbuf,"类型",chsTmp);
		
			SetPropToBuff(psendmsgbuf,"是否追忆",atoi(pname[3].name)!=0 ? "是" : "否");
			SetPropToBuff(psendmsgbuf,"是否遥控",atoi(pname[4].name)!=0 ? "是" : "否");
			SetPropToBuff(psendmsgbuf,"终端序号",pname[5].name);
			SetPropToBuff(psendmsgbuf,"遥信序号",pname[6].name);
			SetPropToBuff(psendmsgbuf,"事项处理方式",pname[7].name);
			SetPropToBuff(psendmsgbuf,"推图名",pname[8].name);
			SetPropToBuff(psendmsgbuf,"SOE句",pname[9].name);
			SetPropToBuff(psendmsgbuf,"事项句",pname[10].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);

			FREE (pname);	
		}
/*		
		sprintf(chsWhere,"name='%s'",chsdevid);
		if(bIsBreak)
			rcdcnt=readRecorders(DBNAME,"DEV_BREAKER","DEVID,NAME,STATUS,STYLE,ISZERO,ENGCODE",
				chsWhere,"",TRUE,&fieldCnt,&valbuf);
		else
			rcdcnt=readRecorders(DBNAME,"DEV_SWITCH","DEVID,NAME,STATUS,STYLE,	ENGCODE",
				chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			if(bIsBreak)
				strcpy(chsTmp,"EMS开关");
			else
				strcpy(chsTmp,"EMS刀闸");
			fieldlen=strlen(chsTmp);
			
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);              //属性的头内容
			psendmsgbuf+=fieldlen;

			pro_short2char((short*)&fieldCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			
			SetPropToBuff(psendmsgbuf,"DEVID",pname[0].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[1].name);
			SetPropToBuff(psendmsgbuf,"正常状态",atoi(pname[2].name)==0 ? "开" : "合");
			SetPropToBuff(psendmsgbuf,"刀闸类型",break_type[(atoi(pname[3].name) <2 ? atoi(pname[3].name) : 2)]);
			if(bIsBreak)
			{
				SetPropToBuff(psendmsgbuf,"是否零值",pname[3].name);
				SetPropToBuff(psendmsgbuf,"代码",pname[4].name);
			}
			else
			{
				SetPropToBuff(psendmsgbuf,"代码",pname[3].name);
			}
			
			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);

			FREE (pname);	
			
		}
*/
	}
	else if(strcmpNoCase(devtypedesc,DEV_LINE)==0) {
		int nCol=0;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,"DEV_LINE",
			"devid,name,maxmva,resist,react,halfcap,safecurrent,zeroresist,zeroreact,zerohalfcap,length,type,paralinenumber,linemj, \
			ismoniset,isteleline,belongtl,isresistvalue,headpoint,tailpoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMS线路");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			
			int devid=0;
			SetPropToBuff(psendmsgbuf,"DEVID",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"容量上限(MVA)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"标么电阻(×100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"标么电抗(×100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"标么半容纳(×100)",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"安全电流",pname[devid++].name);

			SetPropToBuff(psendmsgbuf,"接地电阻",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"接地电抗",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"接地半容纳",pname[devid++].name);

			SetPropToBuff(psendmsgbuf,"线路长度",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"线路类型",pname[devid++].name); //应该从linetype表中获得

			SetPropToBuff(psendmsgbuf,"参数线号[paralinenumber]",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"linemj",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"ismoniset",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"isteleline",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"belongtl",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"isresistvalue",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"headpoint",pname[devid++].name);
			SetPropToBuff(psendmsgbuf,"tailpoint",pname[devid++].name);
			
			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_BUS)==0) {
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_BUS",
			"devid,name,realbus,iscentrumbus,maxvol,minvol,normalvol,engcode",
		chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMS母线");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			
			int nCol=0;

			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"母线类型",atoi(pname[nCol].name)==1 ? "旁路母线" : "普通母线");
			nCol++;
			SetPropToBuff(psendmsgbuf,"中枢母线",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"电压上限",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"电压下限",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"正常电压",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"代码",pname[nCol++].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			
			FREE (pname);	
		}
	}
/*
这里去掉了后面的ishtapesti开始的3个字段，共9个。
devid,name,hvl,mvl,lvl,isresistvalue,it,mpt,
hmmva,hmtapty,hmresist,hmreact,hmextdg,hmextdb,hmnormaltap,hmisitcx_f0,hmisitcx_f1,hmconnected,hmzeroreact,hmzeroresist,hus,lmps,ishtapesti,hscadapoint,mtappoint
mlmva,mltapty,mlresist,mlreact,mlextdg,mlextdb,mlnormaltap,mlisitcx_f0,mlisitcx_f1,mlconnected,mlzeroreact,mlzeroresist,mus,mmps,ismtapesti,htappoint,lscadapoint
hlmva,hltapty,hlresist,hlreact,hlextdg,hlextdb,hlnormaltap,hlisitcx_f0,hlisitcx_f1,hlconnected,hlzeroreact,hlzeroresist,lus,hmps,isltapesti,mscadapoint,ltappoint
	*/
	else if(strcmpNoCase(devtypedesc,DEV_TRANSFORMER3LAYER)==0) {
		int nCol=0;
		short nPagePropCnt;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_TRANSTHREE",
			"devid,name,hvl,mvl,lvl,isresistvalue,it,mpt,hmmva,hmtapty,hmresist,hmreact,hmextdg,hmextdb,hmnormaltap,hmisitcx_f0,hmisitcx_f1,hmconnected,hmzeroreact,hmzeroresist,hus,lmps,mlmva,mltapty,mlresist,mlreact,mlextdg,mlextdb,mlnormaltap,mlisitcx_f0,mlisitcx_f1,mlconnected,mlzeroreact,mlzeroresist,mus,mmps,hlmva,hltapty,hlresist,hlreact,hlextdg,hlextdb,hlnormaltap,hlisitcx_f0,hlisitcx_f1,hlconnected,hlzeroreact,hlzeroresist,lus,hmps",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;


			strcpy(chsTmp,"EMS三绕组变压器");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=8;

			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"高压端额定电压(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"中压端额定电压(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"低压端额定电压(KV)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标么值有效",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"空载电流",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"空载损耗",pname[nCol++].name);


			strcpy(chsTmp,"高压端");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"额定容量",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"分接头类型",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*标么电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标么电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电导",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电纳",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"正常档位",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"有载调压变压器",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"变比估计",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"接线组别",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路损耗(高-低)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路电压(高-中)",pname[nCol++].name);
			

			strcpy(chsTmp,"中压端");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;

			SetPropToBuff(psendmsgbuf,"额定容量",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"分接头类型",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*标么电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标么电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电导",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电纳",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"正常档位",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"有载调压变压器",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"变比估计",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"接线组别",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路损耗(高-低)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路电压(高-中)",pname[nCol++].name);

			strcpy(chsTmp,"低压端");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=14;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"额定容量",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"分接头类型",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*标么电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标么电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电导",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电纳",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"正常档位",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"有载调压变压器",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"变比估计",atoi(pname[nCol].name)==0 ? "否" : "是");
			nCol++;
			SetPropToBuff(psendmsgbuf,"接线组别",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路损耗(高-低)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路电压(高-中)",pname[nCol++].name);


			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_SUBTRANSFORMER)==0) {
		int nCol=0;
		short nPagePropCnt;
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_TRANSTWO",
			"devid,name,mva,mvl,zvl,istapside,tapty,normaltap,isitcx_f0,isitcx_f1,istapesti,connected,isresistvalue,us,mps,mpt,it,resist,react,extdg,extdb,zeroreact,zeroresist,hscadapoint,lscadapoint,tappoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			
			strcpy(chsTmp,"EMS变压器参数I");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=11;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"额定容量",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"高压端额定电压",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"低压端额定电压",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"分接头在",atoi(pname[nCol++].name)==0 ? "高压端" : "低压端");
			SetPropToBuff(psendmsgbuf,"分接头类型",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"正常档位",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"有载调压",atoi(pname[nCol++].name)==0 ? "否" : "是");
			SetPropToBuff(psendmsgbuf,"变比估计",atoi(pname[nCol++].name)==0 ? "否" : "是");
			SetPropToBuff(psendmsgbuf,"采用档位估计",atoi(pname[nCol++].name)==0 ? "否" : "是");
		
			
			strcpy(chsTmp,"EMS变压器参数II");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			
			nPagePropCnt=15;
			pro_short2char(&nPagePropCnt,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);
			proppagecnt++;
			
			SetPropToBuff(psendmsgbuf,"标么置有效",atoi(pname[nCol++].name)==0 ? "否" : "是");
			SetPropToBuff(psendmsgbuf,"短路电压",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"短路损耗",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"空载损耗",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"空载电流",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*标么电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"*标么电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电导(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"励磁电纳(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电阻(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电抗(X100)",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"零序电抗(X100)",pname[nCol++].name);

			SetPropToBuff(psendmsgbuf,"hscadapoint",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"lscadapoint",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"tappoin",pname[nCol++].name);

			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);	
		}
	}
	else if(strcmpNoCase(devtypedesc,DEV_LOAD)==0) {
		sprintf(chsWhere,"name='%s'",chsdevid);
		rcdcnt=readRecorders(DBNAME,
			"DEV_LOAD",
			"devid,name,type,snom,dtstype,constreact,mwnom,mrnom,wmin,wmax,rmin,rmax,engpoint",
			chsWhere,"",TRUE,&fieldCnt,&valbuf);
		if(rcdcnt>0){
			NAME* pname=(NAME*)valbuf;
			strcpy(chsTmp,"EMS负荷");
			fieldlen=strlen(chsTmp);
			pro_short2char(&fieldlen,psendmsgbuf);
			psendmsgbuf += sizeof(short int);
			strncpy(psendmsgbuf,chsTmp,fieldlen);                         //属性的头内容
			psendmsgbuf+=fieldlen;
			fieldlen = fieldCnt ;
			pro_short2char(&fieldlen,psendmsgbuf);                //一个表示属性的个数，注意要*2
			psendmsgbuf+=sizeof(short int);

			int nCol=0;
			SetPropToBuff(psendmsgbuf,"DEVID",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"标识名",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"负荷静特性型号",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"额定容量",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"DTS负荷类型",atoi(pname[nCol++].name)==0 ? "电动机" : "恒阻抗");
			SetPropToBuff(psendmsgbuf,"恒阻抗百分比",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"额定有功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"额定无功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"最小有功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"最大有功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"最小无功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"最大无功",pname[nCol++].name);
			SetPropToBuff(psendmsgbuf,"engpoint",pname[nCol++].name);

			proppagecnt++;
			pro_short2char(&proppagecnt,proppagecntpos);
			FREE (pname);		
		}
	}

	else if(strcmpNoCase(devtypedesc,DEV_MLBREAK)==0) {
		
	}

	else if(strcmpNoCase(devtypedesc,DEV_CAPACITOR)==0) {
		
	}
	else if(strcmpNoCase(devtypedesc,DEV_COIL)==0) {
		
	}
	else if(strcmpNoCase(devtypedesc,DEV_GNDSWITCH)==0) {
		
	}

	msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
	return writemsg(threadinfo[threadno].sockid,&msgbuf);
}



/*
//发送设备参数表
int sendDevParaTable(int threadno,int devtype,char*code)
{
	//代码，秒数，备注，站所线系
	MESSAGE msgbuf;
	msgbuf.type = S_DEVPARA;
	char showfield[CODESIZE];
	memset(showfield,0,CODESIZE);
	//char *tb = getDevParaTable(devtype,showfield);
	char *tb = "SCD设备参数表";
	strcpy(showfield,"站所线系代码,描述,型号,备注");
	//变电所，设备名称
	if( tb != NULL && showfield[0] != '\0')
	{
		void *fieldbuf = NULL;
		
		if(!strcmp(showfield,"*")) {//=="*"
			getTableinfo(tb,&fieldbuf);
		}
		else
		{
			parsefieldstring(showfield,&fieldbuf);
		}
		NAME *pfield = (NAME*)fieldbuf;
		if(fieldbuf != NULL)
		{
			char where[128];
			memset(where,0,128);
			sprintf(where,"代码='%s'",code);//临时,有可能表中无代码字段
			int fieldcount;
			void *buf = NULL;
			int ret = readRecorders(DBNAME,tb,showfield,where,"",FALSE,&fieldcount,&buf);
			NAME *pval = (NAME*)buf;
			char *psendmsgbuf = msgbuf.messagebuf;
			pro_int2char(&fieldcount,psendmsgbuf);//域的个数
			psendmsgbuf += sizeof(int);

			short fieldlen;
			for(int i=0;i<fieldcount;i++)
			{
				if(i == 0)
				{
					memset((pfield+i)->name,0,sizeof((pfield+i)->name));
					strcpy((pfield+i)->name,"变电所");
				}
				else if(i == 1)
				{
					memset((pfield+i)->name,0,sizeof((pfield+i)->name));
					strcpy((pfield+i)->name,"设备名称");
				}
				fieldlen = strlen((pfield+i)->name);//域名长度
				pro_short2char(&fieldlen,psendmsgbuf);
				psendmsgbuf += sizeof(short int);

				strncpy(psendmsgbuf,(pfield+i)->name,fieldlen);//域名
				psendmsgbuf += fieldlen;

				if(ret>0)
				{
					if(i == 0)//站所线系代码，要翻译成描述
					{
						readstation((char *)(pval+i)->name);
					}
					fieldlen = strlen((pval+i)->name);//域值长度
					pro_short2char(&fieldlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);

					strncpy(psendmsgbuf,(pval+i)->name,fieldlen);//域值
					psendmsgbuf += fieldlen;
				}
				else
				{
					fieldlen = 0;//域值长度
					pro_short2char(&fieldlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);
				}
			}//for i
			fieldbuf = NULL;
			FREE(pval);
			FREE(pfield);
			msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
			printf("发送\"%s\"的设备参数\n",code);
			return writemsg(threadinfo[threadno].sockid,&msgbuf);
		}//if(fieldbuf != NULL)
		FREE(pfield);
	}//if( tb!= NULL)

	//查找失败
	int num = 0;
	pro_int2char(&num,(char*)msgbuf.messagebuf);
	msgbuf.length = sizeof(int);
	printf("没有\"%s\"的设备参数\n",code);
	return writemsg(threadinfo[threadno].sockid,&msgbuf);
}

*/



//CODE two-dimensional char array,fixed char length equal code_len;
//return code count
int sendTopologyTable(int threadno,int codecnt,char** code)
{
	short charlen;

	MESSAGE msgbuf;
	msgbuf.type = S_TOPOLOGY;

	int  matchCodeCnt=0;
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(TOPO);
	rdbsql.RdbSelectField("name");
	rdbsql.RdbSelectField("colorstate");  //带电状态
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount == 0) return 0;
		char *psendmsgbuf = msgbuf.messagebuf;

		pro_int2char(&matchCodeCnt,psendmsgbuf);//域的个数
		psendmsgbuf += sizeof(int);
		
		char *pChBuf=(char*)code;
		for(int i=0;i<codecnt;i++)
		{
			for(int j=0;j<nRcdCount;j++)
			{
				char *name = rdbsql.RdbGetString(j,"name");
				if(!strcmpNoCase(name,pChBuf)) //find it in objname collection
				{
					//comb frame
					charlen=strlen(name);
					pro_short2char(&charlen,psendmsgbuf);
					psendmsgbuf += sizeof(short int);
					strncpy(psendmsgbuf,name,charlen);//域名
					psendmsgbuf += charlen;
					
					short val=rdbsql.RdbGetVal_int(j,"active");
					pro_short2char(&val,psendmsgbuf);
					psendmsgbuf+=sizeof(short int);
					matchCodeCnt++;
					break;
				}
			}
			pChBuf+=CHAR_BUF_LEN_DEF;  //next code name
		}
		pro_int2char(&matchCodeCnt,(char*)msgbuf.messagebuf);
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));

		printf("发送拓扑结果参数\n");
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}
	return 0;
}

//send table all records to client
//
int sendTopoColorTable(int threadno)
{
	MESSAGE msgbuf;
	msgbuf.type = S_TOPOCOLOR;
	char *psendmsgbuf = msgbuf.messagebuf;
	
	int		fieldCnt;
	short   rcdcnt;
	void	*valbuf=NULL;
	char	chsWhere[256];
	
	sprintf(chsWhere,"%s","\0");
	rcdcnt=readRecorders(DBNAME,TOPOCOLORTABLE,"NO,RED,GREEN,BLUE",chsWhere,"",TRUE,&fieldCnt,&valbuf);
	printf("\n 发送拓扑颜色参数topcolor表 readcord rcdcnt=%d fieldcnt=%d   \n",rcdcnt,fieldCnt);
	if(rcdcnt>0)
	{	
		int topcolorCount=rcdcnt/fieldCnt;

		pro_int2char(&topcolorCount,psendmsgbuf);              //record count
		psendmsgbuf += sizeof(int);
	
		NAME* pname=NULL;
		pname = (NAME*)valbuf;
		unsigned char len = 0;
		short nTmp;
		for(int i=0;i<topcolorCount;i++)
		{
			nTmp=atoi(pname[i*fieldCnt].name);   //no
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			nTmp=atoi(pname[i*fieldCnt+1].name);   //red
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			nTmp=atoi(pname[i*fieldCnt+2].name);   //green
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);

			
			nTmp=atoi(pname[i*fieldCnt+3].name);   //blue
			pro_short2char(&nTmp,psendmsgbuf);
			psendmsgbuf+=sizeof(short int);
		}
		
		FREE (pname);
		msgbuf.length = abs((int)(msgbuf.messagebuf-psendmsgbuf));
		return writemsg(threadinfo[threadno].sockid,&msgbuf);
	}
	return 0;
}

#ifdef WIN32
	int answermsg(int threadno,HWND hwnd)
#else
	int answermsg(int threadno)
#endif
{
	int ret = 0;
	THREADMANAGE *pthreadinfo = threadinfo + threadno;
	ASSERT(pthreadinfo);
	printf( "消息类型=%d\n",pthreadinfo->prcvdata->type );
	switch(pthreadinfo->prcvdata->type)//消息类型 
	{
	case S_STREAM:	//10,流处理
		{	
			#ifdef WIN32
				acceptStream(threadno,hwnd,true);
			#else
				acceptStream(threadno,TRUE);
			#endif
			break;
		}
	case S_DEVPARA://设备参数,14
		{
/* old code
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			int devtype  = 0;
			pro_char2int(prcvbuf,&devtype);//设备类型
			prcvbuf += sizeof(int);

			unsigned char namelen = prcvbuf[0];//设备代码长度
			
			if(namelen > CODE_LEN) namelen = CODE_LEN;

			prcvbuf += sizeof(unsigned char);
			char objname[128];
			memset(objname,0,128);
			memcpy(objname,prcvbuf,namelen);

			sendDevParaTable(threadno,devtype,objname);
*/
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			char objnamedesc[128],objtypedesc[128];
			unsigned char namelen;
			memset(objnamedesc,0,sizeof(objnamedesc));
			memset(objtypedesc,0,sizeof(objtypedesc));
			
			namelen = prcvbuf[0];//描述长度
			prcvbuf += sizeof(unsigned char);
			memcpy(objtypedesc,prcvbuf,namelen);
			prcvbuf += namelen;
			namelen = prcvbuf[0];//描述长度
			prcvbuf += sizeof(unsigned char);
			memcpy(objnamedesc,prcvbuf,namelen);
			printf("发送设备参数到客户端:类型[%s] 编码[%s]\r\n",objtypedesc,objnamedesc);
			sendDevParaTable(threadno,objtypedesc,objnamedesc);
			break;
		}
	case S_TOPOLOGY: //拓扑结构表,18
		{
			//char objname[TOPU_MAX_COUNT][CHAR_BUF_LEN];
			char *objBuf=(char*)malloc(TOPU_MAX_COUNT*CHAR_BUF_LEN_DEF);
			if(objBuf==NULL)
				break;
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;

			memset(objBuf,0,TOPU_MAX_COUNT*CHAR_BUF_LEN_DEF);
			char *objname=objBuf;

			int objcnt;
			pro_char2int(prcvbuf,&objcnt);
			prcvbuf += sizeof(int);

			if(objcnt>=TOPU_MAX_COUNT)
				break;

			for(int i=0;i<objcnt;i++)
			{
				unsigned char namelen = prcvbuf[0];   //设备代码长度
				if(namelen > CODE_LEN) 
					namelen = CODE_LEN;
				prcvbuf += sizeof(unsigned char);
				memcpy(objname,prcvbuf,namelen);
				prcvbuf+=namelen;
				objname+=CHAR_BUF_LEN_DEF;
			}
			sendTopologyTable(threadno,objcnt,(char**)objBuf);
			free(objBuf);
			break;
		}
	case S_RTUNAME://终端参数表,21
		{
			int groupno;
			pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
			//2011-07-21 lcg修改返回终端参数表内容
			readRtuPara(threadno,groupno);
			//readRTU(threadno,groupno);
			break;
		}
	case S_STATION:  //站所线系表
		{
			int groupno;
			pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
			//2012-05-03 lcg修改返回终端站所线系表内容
			readRTU(threadno,groupno);
			break;
		}
	case S_ANALOGVIEW:
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			int			    paramClassify;
			unsigned char   paramLength;
			char			param[CODE_LEN];
			paramClassify = pbuf[0];
			pbuf += sizeof(unsigned char);
			paramLength = pbuf[0];
			if(paramLength > CODE_LEN) paramLength = CODE_LEN;
			pbuf += sizeof(unsigned char);
			memset(param,0,CODE_LEN);
			memcpy(param,pbuf,paramLength);
			readAnalogView(threadno,paramClassify,param);
			break;
		}
	case S_YCCODE://遥测代码,22
	case S_DDCODE://电度代码,24
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			int rtunums;//RTU个数
			pro_char2int(pbuf,&rtunums);
			pbuf += sizeof(int);
			unsigned char codelen = pbuf[0];

			if(codelen > CODE_LEN) codelen = CODE_LEN;

			pbuf += sizeof(unsigned char);
			char rtucode[64];
			memset(rtucode,0,64);
			memcpy(rtucode,pbuf,codelen);
			if(pthreadinfo->prcvdata->type == S_YCCODE)
			 	readYCCode(threadno,"模拟量视图",rtucode);
		    else
				readYCCode(threadno,"电度视图",rtucode);
			break;
		}
	case S_CURVE:////曲线数据,23
		{
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			unsigned char curFlag = prcvbuf[0];
			prcvbuf += sizeof(unsigned char);
			int curType;
			pro_char2int(prcvbuf,&curType);//曲线类型
			prcvbuf += sizeof(int);
			int daytime;
			pro_char2int(prcvbuf,&daytime);//年月日,1970年以来的秒
			prcvbuf += sizeof(int);
			unsigned char namelen = prcvbuf[0];//代码长度

			if(namelen > CODE_LEN) namelen = CODE_LEN;

			prcvbuf += sizeof(unsigned char);
			char objname[128];
			memset(objname,0,128);
			memcpy(objname,prcvbuf,namelen);
			DayCurve(threadno,objname,curFlag,curType,daytime);
			break;
		}
	case S_REALCURVE://实时曲线类型数据
		{
			char *prcvbuf = pthreadinfo->prcvdata->messagebuf;
			unsigned char curveNums = prcvbuf[0];///曲线数目
			prcvbuf += sizeof(unsigned char);
			if(curveNums>0)
			{
				pthreadinfo->realcurvecount = curveNums;
				pthreadinfo->prealcurve =(REALCURVE*) new REALCURVE[curveNums];
				for(int i = 0; i < curveNums; i++)
				{
					REALCURVE *p = pthreadinfo->prealcurve + i;
					p->curStructNo = prcvbuf[0];
					p->cur_type = prcvbuf[1];
					p->cur_no = prcvbuf[2];
					prcvbuf += 3*sizeof(unsigned char);

					unsigned char len;
					len = prcvbuf[0];//库名长度
					prcvbuf += sizeof(unsigned char);
					strncpy(p->libName,prcvbuf,len);
					prcvbuf += len;

					len = prcvbuf[0];
					prcvbuf += sizeof(unsigned char);
					strncpy(p->objName,prcvbuf,len);
					prcvbuf += len;

					len = prcvbuf[0];
					prcvbuf += sizeof(unsigned char);
					strncpy(p->fieldName,prcvbuf,len);
					prcvbuf += len;					
				}
				RealCurve(threadno);
			}
			else
			{
				pthreadinfo->realcurvecount = 0;
				DELETEP(pthreadinfo->prealcurve);
			}
			break;
		}
	case S_FILE://传送文件,30
		{
			char *pbuf = pthreadinfo->prcvdata->messagebuf;
			char envvar[256];//环境变量，程序会自动去掉后面的"\"
			memset(envvar,0,256);
			getEnv(envvar);

			char path[256],filen[128];
			memset(path,0,256); memset(filen,0,128);
			short pathlen;
			pro_char2short(pbuf,&pathlen);//路径长度
			pbuf += sizeof(short);
			memcpy(path,pbuf,pathlen);
			pbuf += pathlen;
			short filenamelen;
			pro_char2short(pbuf,&filenamelen);//文件名长度 
			pbuf += sizeof(short int);
			memcpy(filen,pbuf,filenamelen);
			sendafile(threadno,pthreadinfo->sockid,envvar,path,filen, S_FILE,TRUE,TRUE);
			break;
		}
	case S_ALLFILE:////传全部文件,31
		{
			char envvar[256];//环境变量，程序会自动去掉后面的"\"
			memset(envvar,0,256);
			getEnv(envvar);
			
			char sql[256];
			memset(sql,0,256);
			sprintf(sql,"delete %s.%s%s where ip地址='%s'",DBNAME,DBO,WEBVISITTABLE,threadinfo[threadno].client_addr);
			Execute(DBNAME,sql);


		#ifdef WIN32
			CFileFind finder;
			CFileFind finderbmp;
			CFileFind finderbmpson;
			CFileFind finderson;
//			static const TCHAR szFileToFindbmp[] = _T("C:\\ds3000\\bmp\\*.*");
//			static const TCHAR szFileToFindgraph[] = _T("C:\\ds3000\\graph\\*.*");
			char szFileToFindbmp[256];
			strcpy(szFileToFindbmp,getenv("NBENV"));
			strcat(szFileToFindbmp,"\\bmp\\*.*");
			char szFileToFindgraph[256];
			strcpy(szFileToFindgraph,getenv("NBENV"));
			strcat(szFileToFindgraph,"\\graph\\*.*");

			BOOL bResultbmp = finderbmp.FindFile(szFileToFindbmp);
			BOOL bResultgraph = finder.FindFile(szFileToFindgraph);
			CString str;
			char Directory[256];
			CString direct;
			while (bResultgraph)//(bResultbmp || 
			{
			//   bResultbmp = finder.FindNextFile();
			  bResultgraph = finder.FindNextFile();
			//	str = 	(LPCTSTR) finder.GetRoot();
			  // str =  (LPCTSTR) finder.GetFileTitle();
			// str =  (LPCTSTR)  finder.GetFilePath();
			 //  str =  (LPCTSTR)  finder.GetFileURL();
			  str =  (LPCTSTR) finder.GetFileName();
			  if (str!="." && str!="..")
			  {
			    BOOL IsDirectory = finder.IsDirectory();
				direct = "graph";
				while (IsDirectory)
				{		
					if (str!="." && str!="..")
						direct = str;
					{
						sprintf(Directory,"%s\\graph\\%s\\*.*",envvar,str);
					}
					BOOL bResultgraphSon = finderson.FindFile(Directory);
					while(bResultgraphSon)
					{
						bResultgraphSon = finderson.FindNextFile();
						str =  (LPCTSTR) finderson.GetFileName();
						if (str!="." && str!="..")
						{
							sprintf(sql,"insert into %s.%s%s values('%s','graph/%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
							Execute(DBNAME,sql);
						}
					}
				break;
				}
			//	else
			//	{
				//	direct = "graph";
			//	}
			    sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
				Execute(DBNAME,sql);
			 }
			}
			while (bResultbmp)
			{
			   bResultbmp = finderbmp.FindNextFile();
			   str =  (LPCTSTR) finderbmp.GetFileName();
			 if (str!="." && str!="..")
			 {
			    BOOL IsDirectory = finderbmp.IsDirectory();
				direct = "bmp";
				while (IsDirectory)
				{		
					if (str!="." && str!="..")
						direct = str;
					{
						sprintf(Directory,"%s\\bmp\\%s\\*.*",envvar,str);
					}
					BOOL bResultbmpSon = finderbmpson.FindFile(Directory);
					while(bResultbmpSon)
					{
						bResultbmpSon = finderbmpson.FindNextFile();
						str =  (LPCTSTR) finderbmpson.GetFileName();
						if (str!="." && str!="..")
						{
							sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
							Execute(DBNAME,sql);
						}
					}
					break;
				}
			//	else
			//	{
				//	direct = "bmp";
			//	}
			    sprintf(sql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,direct,str);
				Execute(DBNAME,sql);
			 }
			}
			finder.Close();
			finderbmp.Close();
			finderbmpson.Close();
			finderson.Close();
		#else
			char filen[256];
			memset(filen,0,256);
			char files[256];
			memset(files,0,256);
			char env[256];
			strcpy(env,getenv("NBENV"));
			char path[256] = "graph";
			strcpy(filen,env);
			strcat(filen,"/");
			strcat(filen,path);

			FILELIST *pfilelist = NULL;
			FILELIST *pfilelist1 = NULL;

			DIRLIST *dirlist = NULL;
			FStat fstat;

			if(fstat.isExist(filen))//文件或目录存在
			{
				dealDir(&dirlist,&pfilelist,&pfilelist1,filen,fstat,path,env);
				FILELIST *ppfilelist = pfilelist;
				while(ppfilelist)//pdirlist &&
				{
					if(fstat.isExist(filen))
						{
							char osql[256];
							memset(osql,0,256);
							sprintf(osql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,path,ppfilelist->filename);
							printf("%s\n",osql);
							Execute(DBNAME,osql);
						}
					ppfilelist=ppfilelist->pfilelist;
				}
				ppfilelist=pfilelist;
			while(ppfilelist)
			{
				FILELIST *filelistend = ppfilelist->pfilelist;
				delete ppfilelist;
				ppfilelist = NULL;
				dirlist = NULL;
				ppfilelist = filelistend;
			}
			}



			char bfilen[256];
			memset(bfilen,0,256);
			char bfiles[256];
			memset(bfiles,0,256);
			char bpath[256] = "bmp";
			strcpy(bfilen,env);
			strcat(bfilen,"/");
			strcat(bfilen,bpath);

			FILELIST *bfilelist = NULL;
			FILELIST *bfilelist1 = NULL;

			DIRLIST *bdirlist = NULL;

			if(fstat.isExist(bfilen))//文件或目录存在
			{
				dealDir(&bdirlist,&bfilelist,&bfilelist1,bfilen,fstat,bpath,env);
				FILELIST *bbfilelist = bfilelist;
				while(bbfilelist)//pdirlist &&
				{
					if(fstat.isExist(bfilen))
						{
							char bosql[256];
							memset(bosql,0,256);
							sprintf(bosql,"insert into %s.%s%s values('%s','%s','%s')",DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadno].client_addr,bpath,bbfilelist->filename);
							printf("%s\n",bosql);
							Execute(DBNAME,bosql);
						}
					bbfilelist=bbfilelist->pfilelist;
				}
				bbfilelist=bfilelist;
			while(bbfilelist)
			{
				FILELIST *bfilelistend = bbfilelist->pfilelist;
				delete bbfilelist;
				bbfilelist = NULL;
				bdirlist = NULL;
				bbfilelist = bfilelistend;
			}
			}
		#endif
		//	return;
			MESSAGE msg;
			msg.type = S_ALLFILE;
			msg.length = 1;
			msg.messagebuf[0] =10;
			writemsg(pthreadinfo->sockid,&msg);
			if (threadinfo->runf==1)
			{
				sendfile(threadno,pthreadinfo->sockid,envvar,"graph","*");
				sendfile(threadno,pthreadinfo->sockid,envvar,"bmp","*");
			}
			
		//	sprintf(sql,"delete from %s.%s%s where ip地址='%s' and 目录='%s' and 文件名='%s'",
			//		DBNAME,DBO,WEBUPDATETABLE,threadinfo[threadid].client_addr,path,filenamepara);
		//	Execute(DBNAME,sql);

			//传送结束
			msg.type = S_ALLFILE;
			msg.length = 1;
			msg.messagebuf[0] =1;
			writemsg(pthreadinfo->sockid,&msg);
			break;
		}
	case S_USER://传用户表
		{
			readWebUser(threadno);
			break;
		}
	case   S_ACK://平安报文,99
		{
			pthreadinfo->invalidtime = time(NULL);//重新计算时间
			if(pthreadinfo->validf != INVALIDF)	// == -1 服务器通知即将停止服务
				pthreadinfo->validf = 1;//有效标志

			break;
		}
	case S_TYPE://客户端类型
		{
			pthreadinfo->clienttype = pthreadinfo->prcvdata->messagebuf[0];
			#ifdef WIN32
				clntchangef = true;
			#endif

			if(pthreadinfo->clienttype == WEBSCADA)//如果是webscada用户，要发送用户表和图形分组表
			{
				int userright;
				pro_char2int(pthreadinfo->prcvdata->messagebuf+2,&userright);
				USEREVENT *p = puserevent;
				while(p)
				{
					if(p->userright == userright)
					{
						pthreadinfo->puserevent = p;							//要根据用户登录的权限来赋值
						pthreadinfo->eventptr = pthreadinfo->puserevent->ptr;//当前事项指针
						break;
					}
					p = p->puserevent ;
				}
				if(pthreadinfo->prcvdata->messagebuf[1] == 1)//第一次登录
				{
					readWebUser(threadno);	
					readWebUserRight(threadno);
					sendTopoColorTable(threadno);

					int groupno;
					pro_char2int(pthreadinfo->prcvdata->messagebuf,&groupno);
					//2012-05-03 lcg修改返回终端站所线系表内容
					readRTU(threadno,groupno);
					//readRtuPara(threadno,groupno);
//					readWebFilename(threadno);   //发送WEB图像分组
//					updateClientFile(threadno);
				}
				else
				{
					char where[64];
					memset(where,0,64);
					sprintf(where,"ip地址='%s'",threadinfo->client_addr);
					int fieldcount;
					void *buf = NULL;
					int	retcount = readRecorders(DBNAME,WEBUPDATETABLE,"目录,文件名,ip地址",where,"目录,文件名,ip地址",FALSE,&fieldcount,&buf);
					if (retcount>0)
					{
						readWebUser(threadno);
						readWebUserRight(threadno);
//						readWebFilename(threadno);
//						updateClientFile(threadno);
					}
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}

	return ret;
}
