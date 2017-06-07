/*
 * --------------------------------------------------
 *   文件名：srvwatchthread.cxx 
 *   软件包：DS3210配网监控系统Web服务器   
 *   文件主要内容：WEB服务器守护线程
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

	#if defined(_AIX) || defined(__linux)
		#include	"sys/signal.h"
	#endif
	
	#ifdef __hpux
		#include <sys/procset.h>
	#endif

    #include    "sys/time.h"
	#include	"sys/types.h"
	#include	"unistd.h"
#endif

#ifdef WIN32
	HANDLE m_hEvenKillWatch;
	HANDLE m_hEventKillCheck;
	extern int answermsg(int threadno,HWND hwnd);
#else
	extern int answermsg(int threadno);
#endif
	
#include "webstruct.h"
#include "externdefine.h"


extern THREADMANAGE threadinfo[INITCLIENTNUMS];
extern USEREVENT *puserevent;
int bEndFlag = FALSE;
int clientnums = INITCLIENTNUMS;//客户端数目
int  clntchangef = FALSE;

extern void removeStream(THREADMANAGE *threadinfo1,int winno,int threadno,int bwait=FALSE);
extern void delay_t(int n);
extern int readRecorders(char* dbname,char* tablename,char* fieldname, char* where,char* orderfn,int bdistinct,int *fieldcount,void **buf);
extern int Execute(char *DBName,char* sqlStatement);
extern int parsefieldstring(char *srcstr,void **desp);
extern int sendafile(int threadid,SOCKET sockid,char *env,char* path,char* filenamepara,int type,int sendwrongf = FALSE,int bupdatetable=FALSE);
extern void updateClientFile(int threadid);

int CardRcdChgNums = 1;
time_t eventtime = 0;
int SendFileNums = 1;

//lcg 20110905添加一个保存当前cardrcd个数的变量
int cardrcdNums=0;

// 创建线程
#ifndef WIN32
int scd_pthread_create(PTHREAD_CALLFUNC  pthfunc,void *argp,int stacksize,	pthread_t *tid)
{

	int ret;

	if (stacksize==0) {
		ret=pthread_create(tid,NULL,pthfunc,argp);
	}
	else {
		pthread_attr_t   tmpattr;
  
		pthread_attr_init(&tmpattr);
		pthread_attr_setstacksize(&tmpattr,stacksize*1024);
		ret=pthread_create(tid,&tmpattr,pthfunc,argp);
	}
	return(ret);
}
#endif

/*
	valcount,变化的数据个数
*/
void getData(int valcount,int threadno,int winno,Rdb_MultiTypeValue& val)
{
	if(valcount>0)//变化数据个数>0
	{
		MULTISTREAM *pmultistream = threadinfo[threadno].pmultistream;
		int i;
		for( i = 0; i < threadinfo[threadno].streamcount; i++)
		{
			if(pmultistream == NULL) 
			{ 
				#ifdef WIN32
				//	LeaveCriticalSection(&threadinfo[threadno].critThread);
				#endif
				return;
			} 
			if(pmultistream->winno == winno)
				break;
			pmultistream = pmultistream->plist;
		}
		if(i >= threadinfo[threadno].streamcount)	
		{ 
			#ifdef WIN32
			//	LeaveCriticalSection(&threadinfo[threadno].critThread);
			#endif
			return;
		}   //没有找到窗口匹配的流
				
		CThreadStreamData *pthreadstreamdata = pmultistream->pthreadstreamdata;
		ASSERT(pthreadstreamdata != NULL);
		for(i=0;i<valcount;i++)
		{
			int order = val.RdbGetValOrderno(i);
			if(order >= 0 && order < pthreadstreamdata->datacount)
			{
				if(!pthreadstreamdata->pdatatype[order])//数据类型没有得到过
				{
					val.RdbGetValInfo(i, &(pthreadstreamdata->pvalue[order].valuetype));
					pthreadstreamdata->pdatatype[order] = TRUE;
				}//if

				uchar datatype = pthreadstreamdata->pvalue[order].valuetype;
				switch(datatype)
				{
				case RDB_DATATYPE_UCHAR:		//无符号字符型,0
				case RDB_DATATYPE_CHAR:         //有符号字符型,1
				case RDB_DATATYPE_USHORT:       //无符号短整数型,2
				case RDB_DATATYPE_SHORT:        //有符号短整数型,3
				case RDB_DATATYPE_UINT:         //无符号整数型,4
				case RDB_DATATYPE_INT:          //有符号整数型,5
					{
						pthreadstreamdata->pvalue[order].value.intval = val.RdbGetVal_int(i);
						break;
					}
				case RDB_DATATYPE_ULONG:        //无符号长整数型,6
					{
						pthreadstreamdata->pvalue[order].value.ulongval = val.RdbGetVal_ulong(i);
						break;
					}
				case RDB_DATATYPE_LONG:         //有符号长整数型(8 unsigned char long),7
					{
						pthreadstreamdata->pvalue[order].value.longval = val.RdbGetVal_long(i);
						break;
					}
				case RDB_DATATYPE_FLOAT:        //单精度浮点型,8
					{
						//浮点数不比较和原来的大小
						pthreadstreamdata->pvalue[order].value.floatval = val.RdbGetVal_float(i);
						break;
					}
				case RDB_DATATYPE_DOUBLE:       //双精度浮点型,9
					{
						pthreadstreamdata->pvalue[order].value.doubleval = val.RdbGetVal_double(i);
						break;
					}
				case RDB_DATATYPE_STRING:       //字符串型,10
					{
						 char *p = val.RdbGetString(RDB_MAX_STRVAL_LEN,i);
						 strncpy(pthreadstreamdata->pvalue[order].stringval,p,strlen(p));
						 pthreadstreamdata->pvalue[order].stringlen = strlen(p);
						break;
					}

				default:
					break;

				}//switch
				pthreadstreamdata->pdatachangedf[order] = TRUE;;
			}//if
		}//for i
		pthreadstreamdata->bchangef = TRUE;
	}//if(valcount>=0)
	#ifdef WIN32
	//	LeaveCriticalSection(&threadinfo[threadno].critThread);
	#endif
}



#ifndef WIN32
void recvStream(int kk)
{
	Rdb_QuickStream *stream = NULL;
	Rdb_MultiTypeValue val;
	MULTISTREAM *pmultistream = NULL;
	int winno;
	THREADMANAGE *pthreadinfo = NULL;
	for(int i = 0; i < INITCLIENTNUMS; i++)
	{
		pthreadinfo = threadinfo + i;
		if(pthreadinfo->runf == 1 && pthreadinfo->validf != INVALIDF)
		{
			stream = pthreadinfo->prdb_quickstream;
			if(stream != NULL)
			{
				pmultistream = pthreadinfo->pmultistream;
				while(pmultistream != NULL)
				{
					winno = pmultistream->winno;
					int valcount = stream->RdbGetFieldValue(BASEWINNO * i + winno, &val);
					if(valcount > 0)
					{
						printf("处理流数据,序号=%d,个数 = %d,%s\n",i,valcount,threadinfo[i].client_addr);
						getData(valcount,i,winno,val);
					}
					pmultistream = pmultistream->plist;
				}
			}
		}
	}
}
#endif





/*	函数说明
 * --------------------------------------------------
 * 函数名  ：readstation()
 * 功能描述：读"站所线系参数表"，更新事项中的对象组代码 －－－描述
 * 参数    ：
 * 返回值：  
 * --------------------------------------------------
 */
void readstation(char *groupname)
{
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(STATION);
	rdbsql.RdbSelectField("name");
	rdbsql.RdbSelectField("describe");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount == 0) return;
		for(int i=0;i<nRcdCount;i++)
		{
			char *name = rdbsql.RdbGetString(i,"name");//代码
			if(name == NULL  || name[0] == '\0')
			{
				continue;
			}
			if(!strcmp(name,groupname))
			{
				name = rdbsql.RdbGetString(i,"describe");
				if(name == NULL) return;
				memset(groupname,0,SHORTNAME_LEN);
				strcpy(groupname,name);
				return;
			}			
		}
	}
}

#if defined(_AIX)||defined(__linux)
void all_exit_s (int sid )
{
	printf("==异常退出==\n");
	exit( 0 );
}
#endif

//获得当前实时库表cardrcd记录个数
int getCardRcdNums()
{
	Rdb_SQL rdbsql;
	rdbsql.RdbSelectFrom(CARDRCD);
	rdbsql.RdbSelectField("objtype");
	rdbsql.RdbSelectField("objname");
	rdbsql.RdbSelectField("cardtype");
	rdbsql.RdbSelectField("cardno");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount<0) 
			return 0;
		else 
			return nRcdCount;
	}
	return 0;
}


#ifdef WIN32
	UINT  checkEvent(LPVOID pParam)
#else
	void * checkEvent(void *argp)
#endif
{
	EventProc eventproc;
	SYS_EVENT sysevent;
	while(!bEndFlag)
	{	
		int hvaeevent = eventproc.Read(&sysevent);//
		if(hvaeevent)
		{
			short int type = sysevent.type;
			short int sort = sysevent.sort;
			readstation(sysevent.Object);
			USEREVENT *pevent = puserevent;				//链表中的每个链都要增加事项，并移动相应的指针
			while(pevent)
			{
				for(int i=0;i<pevent->eventcount;i++)
				{
					if(sysevent.type == pevent->excludeevent[i])
					{
						memcpy(&(pevent->sysevent[pevent->ptr]),&sysevent,sizeof(SYS_EVENT));
						pevent->ptr++;
						pevent->ptr = pevent->ptr % EVENT_NUM_MAX;
						break;
					}
				}
				pevent = pevent->puserevent;
			}
			if(sysevent.type ==	ENTTYPE_SCADA_SETFLAG)//挂牌
			{
				TRACE("收到挂牌信息\n");
				CardRcdChgNums++;
				CardRcdChgNums = CardRcdChgNums % MAXLOOPNUMS;
				if(CardRcdChgNums == 0) CardRcdChgNums = 1;
				time(&eventtime);
				//lcg changed
				cardrcdNums=getCardRcdNums();
				printf("当前牌总个数%d\r\n",cardrcdNums);
				
			}
			if(sysevent.type ==	ENTTYPE_FILE_SAVE)//文件操作事件
			{
				SendFileNums++;
				SendFileNums = SendFileNums % MAXLOOPNUMS;
				if(SendFileNums == 0) SendFileNums = 1;
				time(&eventtime);
			}
			else if(type == ENTTYPE_NULL && sort == ENTCLASS_WEB)//特制的定义事项
			{
				printf("收到重载web实时库事项\n");
				#ifndef WIN32
				#if defined(_AIX)||defined(__linux)
					signal(SIGTERM, all_exit_s);
  					signal(SIGKILL, all_exit_s);
				#else
					sigsend(P_PID,getpid(),SIGTERM);
					sigsend(P_PID,getpid(),SIGKILL);
				#endif
				#endif
			}

		}
		delay_t(WAIT_TIME/2 *1000);
	}
#ifdef WIN32
	ASSERT(SetEvent(m_hEventKillCheck));
	return TRUE;
#else
	return(NULL);
#endif	
}




/*	函数说明
 * --------------------------------------------------
 * 函数名  ：clearThread
 * 功能描述：清理管理线程
 * 参数    ：i,线程号
 *
 * 返回值  ：
 * --------------------------------------------------
 */
void clearThread(int i)
{
	THREADMANAGE *pthread = threadinfo + i;
	//删除流数据的内存
	FREE(pthread->psnddata);
	FREE(pthread->prcvdata);

	pthread->invalidtime = 0;
	pthread->starttime = 0;

	pthread->validf = 0;
	pthread->cardrcdchgnums = 0;
	pthread->sendfilenums = 0;
	if(pthread->streamcount >0)
	{
		removeStream(pthread,0xff,i,TRUE);
	}
	
#ifdef WIN32
	closesocket(pthread->sockid);
#else
	close(pthread->sockid);
#endif
	delay_t(10000);
	pthread->sockid = 0;


	pthread->clienttype = 0;
	pthread->right = 0;
	pthread->puserevent = NULL;
	pthread->eventptr = 0;

	pthread->streamcount = 0;
	pthread->prdb_quickstream = NULL;
	pthread->pmultistream = NULL;
	
	pthread->realcurvecount = 0;
	DELETEP(pthread->prealcurve);
	memset(pthread->client_name,0,WEBNODENAMELEN);
	memset(pthread->client_addr,0,WEBNODENAMELEN);
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：sendregresult()
 * 功能描述：发送客户端注册标志
 * 参数    ：regf注册标志，=true注册成功，=false失败
 *
 * 返回值  ：
 * --------------------------------------------------
 */
void sendregresult(SOCKET sockid,int regf)
{
	MESSAGE regmsg;

	regmsg.length=32;
	if(regf)//注册成功
	{
		regmsg.type = S_SUCCESS;
		strcpy(regmsg.messagebuf,"登录成功!");
	}
	else//注册失败
	{
		regmsg.type = S_FAILLOG;
		strcpy(regmsg.messagebuf,"登录失败!");
	}
	int ret = writemsg(sockid,&regmsg);
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：err_abort()
 * 功能描述：异常退出
 * 参数    ：
 *
 * 返回值  ：
 * --------------------------------------------------
 */
void error_abort()
{
	perror("error,and exit!");
	exit(0);
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：acceptclient()
 * 功能描述：在线程管理表中，填加一记录
 * 参数    ：sockid=服务器端和客户端对应的SOCKET
 *			 clientaddr=客户IP地址
 * 返回值  ：成功，返回>=0
 *           失败，返回-1  
 *
 * --------------------------------------------------
 */
static int acceptclient(SOCKET sockid,struct sockaddr_in clientaddr,int *threadno)
{
	register int i = 0;
	for( i = 0; i < clientnums; i++ )
	{
		if(threadinfo[i].runf == 0 && threadinfo[i].validf == 0)
		{
			threadinfo[i].runf = 1;
			break;
		}

	}
	*threadno = i;
	if(i >= clientnums)//客户已满,增容
	{
		return FALSE;
	}
	threadinfo[i].sockid = sockid;
	strncpy(threadinfo[i].client_addr, inet_ntoa(clientaddr.sin_addr), WEBNODENAMELEN);
	threadinfo[i].client_name[0] = 0;
	threadinfo[i].invalidtime = time(NULL);
	time(&(threadinfo[i].starttime));
	threadinfo[i].psnddata = (MESSAGE*)calloc(1,sizeof(MESSAGE));
	threadinfo[i].prcvdata = (MESSAGE*)calloc(1,sizeof(MESSAGE));
	printf("客户端地址%s,%d\n",threadinfo[i].client_addr,i);
	return TRUE;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  dealStream(int threadno)
 * 功能描述：
 *          处理线程的流数据
 * 参数    ：threadno,线程号
 *
 * 返回值  ：
 *
 * --------------------------------------------------
 */

void dealStream(int threadno)
{
	THREADMANAGE *threadinfo1 = &threadinfo[threadno];
	ASSERT(threadinfo1 != NULL);

	MULTISTREAM *pmultistream = threadinfo1->pmultistream;
	
	for(int i=0;i<threadinfo1->streamcount;i++)
	{
		ASSERT(pmultistream);

		CThreadStreamData *pthreadstreamdata = pmultistream->pthreadstreamdata;  //获得流链表的一项中的CThreadStreamData对象
		ASSERT(pthreadstreamdata);
		if(pthreadstreamdata->bchangef)			//有数据被刷新
		{
			pthreadstreamdata->bchangef = FALSE;

			MESSAGE *sendmsg = threadinfo1->psnddata;
			ASSERT(sendmsg);

			sendmsg->type = S_STREAM;
			char *pchar = sendmsg->messagebuf;
			
			pchar += sizeof(int);//预留点个数的位置

			pro_int2char(&(pmultistream->winno),pchar);//窗口号
			pchar += sizeof(int);

			int streamdatacount=0;
			for(int k=0;k<pthreadstreamdata->datacount;k++)		//遍历CThreadStreamData的每个数据点
			{
				if(pthreadstreamdata->pdatachangedf[k])//本列值改变了
				{
					pthreadstreamdata->pdatachangedf[k] = FALSE;
					pro_int2char(&(pthreadstreamdata->pold_order[k]),pchar);//点号

					pchar += sizeof(int);
					pchar[0] = pthreadstreamdata->pvalue[k].valuetype;//类型
					pchar += sizeof(char);
					
					streamdatacount++;
					switch(pthreadstreamdata->pvalue[k].valuetype)
					{
						case RDB_DATATYPE_UCHAR:		//无符号字符型,0
						case RDB_DATATYPE_CHAR:         //有符号字符型,1
						case RDB_DATATYPE_USHORT:       //无符号短整数型,2
						case RDB_DATATYPE_SHORT:        //有符号短整数型,3
						case RDB_DATATYPE_UINT:         //无符号整数型,4
						case RDB_DATATYPE_INT:          //有符号整数型,5
							{
								int intval = (int)pthreadstreamdata->pvalue[k].value.intval;
								pchar[0] = sizeof(int);//点值长度
								pchar += sizeof(unsigned char);
								pro_int2char(&intval,pchar);//点值
								pchar += sizeof(int);
								break;
							}
						/*case RDB_DATATYPE_ULONG:        //无符号长整数型,6
							{
								hlong hlongval = (hlong)pthreadstreamdata->pvalue[k].value.ulongval;
								pchar[0] = sizeof(hlong);//点值长度
								pchar += sizeof(unsigned char);
								pro_long2char(&hlongval,pchar);//点值
								pchar += sizeof(hlong);
								break;
							}
						case RDB_DATATYPE_LONG:         //有符号长整数型(8 unsigned char long),7
							{
								hlong hlongval = (hlong)pthreadstreamdata->pvalue[k].value.longval;
								pchar[0] = sizeof(hlong);//点值长度
								pchar += sizeof(unsigned char);
								pro_long2char(&hlongval,pchar);//点值
								pchar += sizeof(hlong);
								break;
							}*/
						case RDB_DATATYPE_FLOAT:        //单精度浮点型,8
							{
								float floatval = (float)pthreadstreamdata->pvalue[k].value.floatval;
								pchar[0] = sizeof(float);//点值长度
								pchar += sizeof(unsigned char);
								pro_float2char(&floatval,pchar);//点值
								pchar += sizeof(float);
								break;
							}
						case RDB_DATATYPE_DOUBLE:       //双精度浮点型,9
							{
								double floatval = pthreadstreamdata->pvalue[k].value.doubleval;
								pchar[0] = sizeof(double);//点值长度
								pchar += sizeof(unsigned char);
								pro_double2char(&floatval,pchar);//点值
								pchar += sizeof(double);
								break;
							}
						case RDB_DATATYPE_STRING:       //字符串型,10
							{
								pchar[0] = (unsigned char)pthreadstreamdata->pvalue[k].stringlen;//点值长度
								pchar += sizeof(unsigned char);
								strcpy(pchar,pthreadstreamdata->pvalue[k].stringval);//点值
								pchar += pthreadstreamdata->pvalue[k].stringlen;
								break;
							}

						default:
							{
								streamdatacount--;
								break;
							}
						}//switch
				}//if(pthreadstreamdata->pdatachangedf[k])//本列值改变了
			}//for k
			if(streamdatacount>0)
			{
				sendmsg->length = abs(sendmsg->messagebuf - pchar);
				pro_int2char(&streamdatacount,sendmsg->messagebuf);//有效点个数
				writemsg(threadinfo1->sockid,sendmsg);//发送流数据
				memset(sendmsg,0,sizeof(MESSAGE));
			}
		}//if(pthreadstreamdata->bchangf)//有数据被刷新
		pmultistream = pmultistream->plist;
	}//for i
}


//组合成事项报文
void combineEvent(char **pbuf,SYS_EVENT *sysevent)
{
		char *psendbuf = *pbuf;
//		short int shortvar = sysevent->grpno;//分组号
		short int shortvar = 0;//分组号
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		shortvar = sysevent->sort;//分类号
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		shortvar = sysevent->type ;//类型
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		int intvar = sysevent->ymd;//年月日
		pro_int2char(&intvar,psendbuf);
		psendbuf += sizeof(int);

		intvar = sysevent->hmsms ;//4、时分秒毫秒
		pro_int2char(&intvar,psendbuf);
		psendbuf += sizeof(int);

		psendbuf[0] = sysevent->security ;//5、安全级
		psendbuf += sizeof(unsigned char);

		shortvar = sysevent->state;//状态
		pro_short2char(&shortvar,psendbuf);
		psendbuf += sizeof(short int);

		float floatvar = sysevent->val; //当前浮点值
		pro_float2char(&floatvar,psendbuf);
		psendbuf += sizeof(float);

		unsigned char bytelen = strlen(sysevent->Object);//事件对象组名长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->Object,bytelen);//事件对象组名
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member0);//事件对象名0长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member0,bytelen);//事件对象名0
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member1);//事件对象名1长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member1,bytelen);//事件对象名1
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member2);//事件对象名2长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member2,bytelen);//事件对象名2
		psendbuf += bytelen;

		bytelen = strlen(sysevent->member3);//事件对象名3长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->member3,bytelen);//事件对象名3
		psendbuf += bytelen;

		bytelen = strlen(sysevent->graph);//18、推图名长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->graph,bytelen);//18、推图名
		psendbuf += bytelen;

		bytelen = strlen(sysevent->tone);//语音描述信息长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->tone,bytelen);//语音描述信息
		psendbuf += bytelen;

		bytelen = strlen(sysevent->text);//文字描述信息长
		psendbuf[0] = bytelen;
		psendbuf += sizeof(unsigned char);

		memcpy(psendbuf,sysevent->text,bytelen);//文字描述信息
		psendbuf += bytelen;

		*pbuf = psendbuf;

}


/*	函数说明
 * --------------------------------------------------
 * 函数名  dealSysEvent(threadno)
 * 功能描述：
 *          1 处理系统事项
 * 参数    ：
 *
 * 返回值  ：
 *
 * --------------------------------------------------
 */

void dealSysEvent(int threadno)
{
	//1次最多发送50个事项，多于这个数，分多个报文发
	THREADMANAGE *threadinfo1 = &threadinfo[threadno];
	ASSERT(threadinfo1 != NULL);

	if(threadinfo1->validf == -1 || threadinfo1->puserevent == NULL) return;
	MESSAGE *psendmsg = threadinfo1->psnddata;
	ASSERT(psendmsg != NULL);

	char *psendbuf = psendmsg->messagebuf ;
	int syseventptr = threadinfo1->puserevent->ptr;//threadinfo1->puserevent->ptr的值取出来，因其值是动态变化的
	psendmsg->type = S_REALEVENT;//报文类型
	short int eventnums;
	int count = 0;
	short count1 = 0;
	if(threadinfo1->eventptr >  syseventptr)
	{
		eventnums = syseventptr + EVENT_NUM_MAX - threadinfo1->eventptr;
		count = eventnums / 50 ;//发送缓冲区容量有限，分批发送
		count1 = eventnums % 50;
	}
	else if(threadinfo1->eventptr < syseventptr)
	{
		eventnums = syseventptr - threadinfo1->eventptr;
		
		count = eventnums / 50 ;//发送缓冲区容量有限，分批发送
		count1 = eventnums % 50;
	}
	else
		return;

	for(int i=0;i<count;i++)
	{		
		psendbuf = psendmsg->messagebuf ;
		eventnums = 50;
		pro_short2char(&eventnums,psendbuf);//事项个数  >50时
		psendbuf += sizeof(short int);
		for(int ptr=threadinfo1->eventptr;ptr<threadinfo1->eventptr+50;ptr++)
		{
			SYS_EVENT *sysevent =   &threadinfo1->puserevent->sysevent[ptr%EVENT_NUM_MAX];
			combineEvent(&psendbuf,sysevent);
		}
		psendmsg->length = abs(psendmsg->messagebuf - psendbuf);//报文长度

		writemsg(threadinfo1->sockid,psendmsg);

		threadinfo1->eventptr = (threadinfo1->eventptr+50) % EVENT_NUM_MAX;
	}
	if(count1<=0 || count1>EVENT_NUM_MAX) return;
		//发送50以外的余数
	psendbuf = psendmsg->messagebuf ;
	pro_short2char(&count1,psendbuf);//事项个数 
	psendbuf += sizeof(short int);
	for(int ptr=threadinfo1->eventptr;ptr<threadinfo1->eventptr+count1;ptr++)
	{
		SYS_EVENT *sysevent =   &threadinfo1->puserevent->sysevent[ptr%EVENT_NUM_MAX];
		combineEvent(&psendbuf,sysevent);
	}
	psendmsg->length = abs(psendmsg->messagebuf - psendbuf);//报文长度
	writemsg(threadinfo1->sockid,psendmsg);
	threadinfo1->eventptr = (threadinfo1->eventptr+count1) % EVENT_NUM_MAX;
	return;
}


/*	函数说明
 * --------------------------------------------------
 * 函数名  ：sendCardRCD()
 * 功能描述：更新客户端"标志牌记录表"
 * 参数    ：threadid,线程号
 * 返回值：  
 * --------------------------------------------------
 */
void sendCardRCD(int threadid)
{
	Rdb_SQL rdbsql;

	rdbsql.RdbSelectFrom(CARDRCD);
	rdbsql.RdbSelectField("groupno");
	rdbsql.RdbSelectField("firststate");
	rdbsql.RdbSelectField("secondstate");
	rdbsql.RdbSelectField("objtype");
	rdbsql.RdbSelectField("objname");
	rdbsql.RdbSelectField("cardtype");
	rdbsql.RdbSelectField("cardno");
	rdbsql.RdbSelectField("startx");
	rdbsql.RdbSelectField("starty");
	rdbsql.RdbSelectField("metaid");
	rdbsql.RdbSelectField("hostname");
	rdbsql.RdbSelectField("username");
	rdbsql.RdbSelectField("context");
	rdbsql.RdbSelectField("graphpath");
	rdbsql.RdbSelectField("metapath");
	rdbsql.RdbSelectField("devdescribe");


// 	rdbsql.RdbSelectFrom(CARDRCD);	
// 	rdbsql.RdbSelectField("bitmap");
// 	rdbsql.RdbSelectField("cardno");
// 	rdbsql.RdbSelectField("cardtype");
// 	rdbsql.RdbSelectField("context");
// 	rdbsql.RdbSelectField("firststate");
// 	rdbsql.RdbSelectField("groupno");
// 	rdbsql.RdbSelectField("height");
// 	rdbsql.RdbSelectField("hostname");
// 	rdbsql.RdbSelectField("imagename");
// 	rdbsql.RdbSelectField("imagetype");
// 	rdbsql.RdbSelectField("objname");
// 	rdbsql.RdbSelectField("objtype");
// 	rdbsql.RdbSelectField("secondstate");
// 	rdbsql.RdbSelectField("startx");
// 	rdbsql.RdbSelectField("starty");
// 	rdbsql.RdbSelectField("userdefgr");
// 	rdbsql.RdbSelectField("username");
// 	rdbsql.RdbSelectField("width");
	int ret = rdbsql.RdbSelCmd("scada","scada");
	if(ret == RDB_OK)
	{
		int nRcdCount = rdbsql.RdbGetRcdCnt();
		if(nRcdCount < 0) return;
		MESSAGE msgCardrcd;
		char *msgbuf = msgCardrcd.messagebuf;
		char *ptmp;
		msgCardrcd.type = S_CARDRCD;
		pro_int2char(&nRcdCount,msgbuf);
		msgbuf += sizeof(int);
		int len;
		for(int i=0;i<nRcdCount;i++)
		{
			
			//从实时库获得记录并保存到缓冲区中.
			len = rdbsql.RdbGetVal_int(i,"groupno");//分组序号
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"firststate");//第一态
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"secondstate");//第二态
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"objtype");//对象类型
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"objname");//对象编号(对象代码)；
			len = strlen(ptmp);						//对象代码长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				//对象编号(对象代码)；
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"cardtype");//标志牌类型
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"cardno");//标志号
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"startx");//坐标X；
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			len = rdbsql.RdbGetVal_int(i,"starty");//坐标Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			
			len = rdbsql.RdbGetVal_int(i,"metaid");//坐标Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"hostname");//主机名
			len = strlen(ptmp);//主机名长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//主机名
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"username");//用户名
			len = strlen(ptmp);//用户名长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//用户名
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"context");//注释
			len = strlen(ptmp);//注释长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//注释
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"graphpath");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"metapath");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"devdescribe");
			len = strlen(ptmp);					
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);				
			msgbuf += len;
			
/*
			ptmp = rdbsql.RdbGetString(i,"bitmap");//位图名
			len = strlen(ptmp);
			msgbuf[0] = len;//位图名长度
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//位图名
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"cardno");//标志号
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"cardtype");//标志牌类型
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"context");//注释
			len = strlen(ptmp);//注释长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//注释
			msgbuf += len;

			len = rdbsql.RdbGetVal_int(i,"firststate");//第一态
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"groupno");//分组序号
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"height");// 高度；
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"hostname");//主机名
			len = strlen(ptmp);//主机名长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//主机名
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"imagename");//图象名
			len = strlen(ptmp);//图象名长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//图象名
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"imagetype");//图象类型
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			ptmp = rdbsql.RdbGetString(i,"objname");//对象编号(对象代码)；
			len = strlen(ptmp);//对象代码长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//对象编号(对象代码)；
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"objtype");//对象类型
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"secondstate");//第二态
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"startx");//坐标X；
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);

			len = rdbsql.RdbGetVal_int(i,"starty");//坐标Y
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
			
			ptmp = rdbsql.RdbGetString(i,"userdefgr");//用户定义图元
			len = strlen(ptmp);//用户定义图元长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//用户定义图元
			msgbuf += len;

			ptmp = rdbsql.RdbGetString(i,"username");//用户名
			len = strlen(ptmp);//用户名长度
			msgbuf[0] = len;
			msgbuf += sizeof(unsigned char);
			strncpy(msgbuf,ptmp,len);//用户名
			msgbuf += len;
			
			len = rdbsql.RdbGetVal_int(i,"width");//宽
			pro_int2char(&len,msgbuf);
			msgbuf += sizeof(int);
*/
		}
		msgCardrcd.length = abs(msgbuf - msgCardrcd.messagebuf);
		writemsg(threadinfo[threadid].sockid,&msgCardrcd);
	}
}





/*	函数说明
 * --------------------------------------------------
 * 函数名  answerclient(LPVOID pParam)
 * 功能描述：
 *          1 响应客户请求的线程
 * 参数    ：LPVOID pParam，传送服务端的Socket
 *
 * 返回值  ：
 *
 * --------------------------------------------------
 */
#ifdef WIN32
	UINT answerclient(LPVOID pParam)
#else
	void * answerclient(void *pParam)
#endif
	{
	int threadno = ((int*)pParam)[0];
	#ifdef WIN32
		HWND hwnd =	   (HWND)(((int*)pParam)[1]);
	#endif
	delete pParam;


	fd_set				readset;
    struct timeval		readtv;
	int					nums;

	readtv.tv_sec  = 0;
	readtv.tv_usec = 100*1000;//0.1秒,若为0，则CPU负荷太大20*100;//

	THREADMANAGE *threadinfo1 = threadinfo + threadno;
	ASSERT(threadinfo1);

	sendregresult(threadinfo1->sockid ,TRUE);   //发送注册成功信息
	
	threadinfo1->runf = 1;
	clntchangef	= TRUE;
	time_t  threadtime;
	
	while(TRUE) {
		readtv.tv_sec  = 0;
		readtv.tv_usec = 100*1000;
		if(threadinfo1->validf == INVALIDF){
			printf("网络连接终止!\n");
			//平安报文超时 
			break;
		}
	    FD_ZERO(&readset);
		FD_CLR(threadinfo1->sockid, &readset);
		FD_SET(threadinfo1->sockid,&readset);
		int fd_isset = FD_ISSET(threadinfo1->sockid,&readset);
		nums = select(FD_SETSIZE,&readset,0,0,&readtv);
		FD_CLR((SOCKET)threadinfo1->sockid,&readset);
        if(nums>0) {//收到网络报文
			if(readmsg(threadinfo1->sockid,threadinfo1->prcvdata) == 0)
			{
				#ifdef WIN32
					answermsg(threadno,hwnd);//报文处理
					//updateClientFile(threadno);
				#else
					answermsg(threadno);//报文处理
					//	if(sysevent.type ==	ENTTYPE_FILE_SAVE)//文件操作事件
				//	updateClientFile(threadno);
					//updateClientsFile(threadno);
				#endif
			}
			else//读报文错，中断处理
			{
				printf("读报文错,网络连接终止!\n");
				threadinfo1->validf = INVALIDF;
				break;//有连接信息，但没有读到内容，说明客户端退出
			}
        }                    
        else if(nums == SOCKET_ERROR) {        //SELECT被信号中断，或网络退出
			if(errno != EINTR)
			{
				threadinfo1->validf = INVALIDF;
				break;
			}
		}
        else {//线程空闲时,超时处理，扫描实时及线程退出标志
			if(threadinfo1->streamcount >0)//注册了流
			{
				dealStream(threadno);
			}

			if(threadinfo1->clienttype == WEBSCADA)//用户类型是webscada
			{
				dealSysEvent(threadno);//发送实时事项
				
				if(threadinfo1->cardrcdchgnums !=  CardRcdChgNums )     //考虑到,收到事项到实时库改变，有一个延迟
				{
					time(&threadtime);
					//printf("threadtime = %d,eventtime = %d",threadtime,eventtime);
					//10s延迟,注意，？？这里代码是由问题的。10表示10秒的意思
					//由于存在客户端接收过多挂牌信息的原因，改为30
					if(threadtime - eventtime > 10)
					{
						//TRACE1("发送挂牌信息到%s\n",threadinfo1->client_addr );
						printf("10秒超时,发送挂牌信息到%s ?\r\n",threadinfo1->client_addr);
						sendCardRCD(threadno);
						threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums + 1 ;
						threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums % MAXLOOPNUMS;
						time(&eventtime);  //reinit eventtime variable

					}
					else
					{
						int cardrcdnums=getCardRcdNums();
						if(cardrcdnums!=cardrcdNums)   //在事项改变过程中，实时库挂牌表记录个数改变了。
						{
							printf("标志牌记录改变,发送挂牌信息,当前牌总个数%d \r\n",cardrcdnums);
							sendCardRCD(threadno);
							threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums + 1 ;
							threadinfo1->cardrcdchgnums = threadinfo1->cardrcdchgnums % MAXLOOPNUMS;
							cardrcdNums = cardrcdnums;
						}
					}
				}

				/*if(threadinfo1->sendfilenums !=  SendFileNums )//向客户端发送更新文件
				{
					
					time(&threadtime);
					if(threadtime - eventtime > 3)//1s延迟
					{
						updateClientFile(threadno);
						threadinfo1->sendfilenums = threadinfo1->sendfilenums + 1 ;
						threadinfo1->sendfilenums = threadinfo1->sendfilenums % MAXLOOPNUMS;
					}
				}*/

			}
			//超时处理
			//CLIENTSTOPTIME秒没收到平安报,将该节点标志validf为无效-》通知对应的服务线程退出
			{
				time_t curtime = time(NULL);
				if(curtime - threadinfo1->invalidtime > CLIENTSTOPTIME)
				{
					threadinfo1->validf = INVALIDF;
				}
			}
		}//线程空闲时,超时处理，扫描实时及线程退出标志
	}//while
	//在主线程清理线程开辟的内存，关闭socket
	clearThread(threadno);
	threadinfo1->runf = 0;
	clntchangef	= TRUE;
	printf("服务进程退出!\n");
	#ifdef WIN32
	//	AfxEndThread(TRUE);
		return TRUE;
	#else
		return(NULL);
	#endif
}



/*	函数说明
 * --------------------------------------------------
 * 函数名  srvwatchthread()
 * 功能描述：
 *          1 构建SOCKET
 *          2 永久监听WEBPORT端口
 * 参数    ：LPVOID pParam,保留未用
 *
 * 返回值  ：0,构建服务端Socket失败；
 *
 * --------------------------------------------------
 */
#ifdef WIN32
	UINT srvwatchthread(LPVOID pParam)
#else
	void * srvwatchthread(void *argp)
#endif
{ 
	SOCKET	listensock,srvsock/*受到连接构建的服务器端Socket*/;
	int		maxbufsize = MAXBUFSIZE;
	int		reuseaddr=1,keepvalue=1;
    struct	sockaddr_in localaddr/*服务器本机地址*/,clientaddr/*客户机地址*/;
    struct	timeval     selecttv;
    fd_set  readfds;
	int		threadno=-1;//当前线程在创建的线程组中的序号
#ifdef _AIX
	unsigned long len;
#elif __linux
	socklen_t    len;
#else
	int len;
#endif

	#ifdef WIN32
		HWND    hwnd = (HWND)pParam;//CMainFrame的窗口句柄
	#endif
	

    localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(WEBPORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	selecttv.tv_sec =  0;       //1
	selecttv.tv_usec = 500*1000;		//0
 	
	//构建侦听Socket
	listensock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//socket(int af,int type,int protocol); 
	if(listensock == SOCKET_ERROR) {
	     //AfxMessageBox("Socket服务端构建失败!",MB_OK|MB_ICONHAND);
		printf("Socket服务端构建失败\n");
	     return 0;
	}

	//设置socket option
	if(setsockopt(listensock,SOL_SOCKET,SO_RCVBUF,(char *)&maxbufsize,sizeof(maxbufsize))<0)  error_abort(); //SO_RCVBUF means to Specify buffer size for receives 
	if(setsockopt(listensock,SOL_SOCKET,SO_SNDBUF,(char *)&maxbufsize,sizeof(maxbufsize))<0)  error_abort(); //SO_SNDBUF means to Specify buffer size for sends 
	if(setsockopt(listensock,SOL_SOCKET,SO_REUSEADDR,(char *)&reuseaddr,sizeof(reuseaddr))<0)   error_abort(); //SO_REUSEADDR means to Allow the socket to be bound to an address that is already in use
    if(setsockopt( listensock, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepvalue, sizeof(keepvalue)) < 0 )   error_abort(); //SO_KEEPALIVE means to Send keepalives (连接活跃)

#ifndef WIN32
 	struct timeval tv;
	tv.tv_sec = SOCKTIMEOUT;
	tv.tv_usec = 0;
	setsockopt(listensock,SOL_SOCKET,SO_SNDTIMEO,(char *)&tv,sizeof(tv));
	setsockopt(listensock,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv,sizeof(tv));//只有BSD支持这两个选项
#endif
	if(bind(listensock,(struct sockaddr *)&localaddr,sizeof(localaddr))<0)  error_abort(); //bind fail,exit.
    
//	int listen ( SOCKET s,int backlog );  SOMAXCONN=5;
	if(listen(listensock,SOMAXCONN) != 0) error_abort(); //places a socket a state where it is listening for an incoming connection.
	
	printf("web服务器启动,处于服务状态\n");
	int count = 0;
	/*主线程连续监视服务器端口*/
	while(!bEndFlag) {
   		FD_ZERO(&readfds);
		FD_SET(listensock,&readfds);
		selecttv.tv_sec =  0;       //1
		selecttv.tv_usec = 500*1000;
        int characternum = select(FD_SETSIZE,&readfds,NULL,NULL,&selecttv);//&selecttv);//if &selecttv is NULL ,for blocking operation,仅仅select被阻塞，而非整个socket将变为阻塞的
 		if(characternum>0){//characternum>1时，待处理
			len = sizeof(clientaddr);
            srvsock = accept(listensock,(struct sockaddr *)&clientaddr,&len);
			if(srvsock == SOCKET_ERROR)
				continue;
            else {
				int ret = acceptclient(srvsock,clientaddr,&threadno);
				if(ret)
				{
					#ifdef WIN32
						int *p = new int[2];
						p[0] = threadno;
						p[1] = (int)(void*)hwnd;
						AfxBeginThread(answerclient,(LPVOID)p);
					#else
						int *p = new int[1];
						p[0] = threadno;
						pthread_t pthread_id;
						scd_pthread_create(answerclient,(void*)p,2048,&pthread_id);
					#endif
				}
				else
					sendregresult(srvsock,FALSE);//发注册失败信息
			}
		}
	}
#ifdef WIN32
	ASSERT(SetEvent(m_hEvenKillWatch));
	return TRUE;
#else
	return (NULL);
#endif
}



/*	函数说明
 * --------------------------------------------------
 * 函数名  ：getLifeThrNums()
 * 功能描述：得到活动客户端数目
 * 参数    ：
 *
 * 返回值  ： 活动客户端的数目
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */

int getLifeThrNums()
{
	int count = 0;
	for(int threadno =0;threadno<clientnums;threadno++)
	{
		if(threadinfo[threadno].runf == 1 && threadinfo[threadno].validf == 1)
		{
			count++;
		}//if 
	}//for
	return count;
}

/*	函数说明
 * --------------------------------------------------
 * 函数名  ：killAllClient()
 * 功能描述：发服务器停止信息到客户端,停止所有的客户端服务线程
 * 参数    ：
 *
 * 返回值  ： 
 *            
 * 备注    ： 
 *
 * --------------------------------------------------
 */
void killAllClient()
{
	int count = 0;
	MESSAGE msg;
	msg.type = S_SRVEXIT;//设置消息类型
	msg.length = 0;//设置消息的长度
	int bFirst = TRUE;
	while(bFirst || count>0)
	{
		count = 0;
		for(int threadno =0;threadno<clientnums;threadno++)
		{
			if(threadinfo[threadno].runf == 1 )
			{
				count++;
				if(bFirst)
					//发送数据,在此发是不安全的，假如对同一个客户端，两个线程同时发会产生报文混乱
					writemsg(threadinfo[threadno].sockid,&msg);
				threadinfo[threadno].validf = INVALIDF;
			}//if 
		}//for		
		delay_t(count*2*1000);
		bFirst = FALSE;
	}
	return;
}

