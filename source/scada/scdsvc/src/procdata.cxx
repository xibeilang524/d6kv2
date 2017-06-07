#include <stdio.h>
#include "scadainterface.h"
#include "net/netapi.h"
#include "scd/scddef.h"
#include "scadaproc.h"
#include "procthread.h"
#include "verapi.h"

#include "trenddb/trenddbapi.h"



//#define     PROC_FERTRAWDATA
void proc_fertdata(void* buf,char* hostname);
extern DBApi dbapi;
#ifdef __unix
	#include <signal.h>
	#include "p_process.h"		//2004 05 add
	#include <netdb.h>
#endif
#ifdef __linux
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
//#include "afertscd/afert.h"
//#include "afertscd/law.hxx"
SCADACHANPARA scadachanpara;
//COMPARA   compara;
#ifdef __sun
void usr1_func(int sig)
{
	return;
}
#endif

#ifdef _AIX
	int		GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
	int		UpdateTimePeriodParaTab(void);
	void	FreshCurrentTimePeriodType(void);
	void    KwhDayInit(void);
	void    CalIntegralKwh(void);
	void    KwhBypassProcessing(void);
	int		AutoCheckBreakReplace(void);
	void    AnaBypassProcessing(void);
#endif

#ifdef __linux
	int		GetTimePeriodType(ushort district,ushort year,uchar month,uchar day,uchar hour,uchar minute);
	int		UpdateTimePeriodParaTab(void);
	void	FreshCurrentTimePeriodType(void);
	void    KwhDayInit(void);
	void    CalIntegralKwh(void);
	void    KwhBypassProcessing(void);
	int		AutoCheckBreakReplace(void);
	void    AnaBypassProcessing(void);
#endif

#ifdef __unix	
	extern void YCMultiSourceDataProcessing(void);
#endif

static int PROT_RESETTING_FLAG = GetProgramRunPara("protresetting",(int)0);
//TrendDBAPI cTrendDB;
///////////////////////////////////////////////////////
void fertykcmd (char *usrname, int type, int groupno, int terminalno, int ykno, int status)
{
    //char		str[32];
	 MailBox	mailbox(groupno,FEP_SERVER);  
	 MailBox	pubmailbox(groupno,"PUBFEP");  
    //str[0] = MAILYKOPERATION;
    //str[1] = type;
    //str[2] = LOBYTE (terminalno);
    //str[3] = HIBYTE (terminalno);
    //str[4] = LOBYTE (ykno);
    //str[5] = HIBYTE (ykno);
    //str[6] = status; 

	FE_MAIL_STRUCT* mail_struct;
	mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + sizeof(FE_YK_MAIL) );	
	int datalen = sizeof(FE_YK_MAIL);		//数据区长度
	memset(mail_struct, 0, mail_struct->get_lenth()+datalen);
	mail_struct->mail_type = YK;
	mail_struct->mail_lenth = mail_struct->get_lenth() + datalen;		//总长度
	mail_struct->sub_information1 = 1;
	FE_YK_MAIL  ykstr;
	ykstr.action = status;// 十进制204 十六进制为0Xcc 代表合；十进制51 十六进制为0X33 代表分
	ykstr.rtu_no = terminalno; 
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	TERMINAL_STRU			*terminalstationp;
	if(fobjtable.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK) return;
	terminalstationp =  (TERMINAL_STRU*)fobjtable.RdbSearchRcdAddrByKey(&(terminalno));
	if (terminalstationp!=NULL&&terminalstationp->useflag)
	{
		ykstr.channel_no = terminalstationp->runchanno;
	}
	ykstr.yk_flag = type;
	ykstr.yk_no = ykno;
	memcpy(mail_struct->mail_body, (unsigned char*)&ykstr, datalen);

    mailbox.SendSrvRequest (usrname, (void *)mail_struct, mail_struct->mail_lenth, FALSE);//主备都发
    pubmailbox.SendPubSrvRequest (usrname, (void *)mail_struct, mail_struct->mail_lenth, FALSE);//主备都发
 } 
/*void fertgroupykcmd(char *usrname,char type, int groupno, short terminalno,short num,GROUPYKPARA *groupykp)
{
	char	*str;
	int	len = 8+num*sizeof(GROUPYKPARA);
	
	str = new char[len];
	MailBox	mailbox(groupno,FEP_SERVER);

	str[0] = MAILGROUPYKOPERATION;
	str[1] = type;
	str[2] = LOBYTE(terminalno);
	str[3] = HIBYTE(terminalno);
	str[4] = LOBYTE(num);
	str[5] = HIBYTE(num);
	str[6] = 0;
	str[7] = 0;
	str += 8;

	memcpy(str,(char *)groupykp,num*sizeof(GROUPYKPARA));
	mailbox.SendSrvRequest(usrname,(void *)str,len, FALSE);
	delete [] str;
 }*/

void fertguapaicmd (char *usrname, char type, short groupno, short terminalno, short yxno, unsigned short cardtype,unsigned char setflag)
{
	char		str[32]; 
	MailBox	mailbox(groupno,FEP_SERVER);
     
    str[0] = MAILGUAPAI;
    str[1] = type;
    str[2] = LOBYTE (terminalno);
    str[3] = HIBYTE (terminalno);
    str[4] = LOBYTE (yxno);
    str[5] = HIBYTE (yxno);
    str[6] = LOBYTE (cardtype); 
    str[7] = HIBYTE (cardtype); 
    str[8] = setflag; 
    mailbox.SendSrvRequest (usrname, (void *)str, 9, FALSE);
 } 


void fertytcmd (char *usrname, short groupno, short terminalno, short ytno, unsigned char outtype,float setvalue)
{
// 	unsigned char  *up;
// 	char		str[32]; 
// 	MailBox	mailbox(groupno,FEP_SERVER);  
//      
//     str[0] = MAILYTOPERATION;
//     str[1] = LOBYTE (terminalno);
//     str[2] = HIBYTE (terminalno);
//     str[3] = LOBYTE (ytno);
//     str[4] = HIBYTE (ytno);
//     str[5] = outtype; 
// 	up=(unsigned char *)&outvalue;
// #if( ( defined(__sun) && defined(__sparc) ) || defined(_AIX) || defined(__hpux))
//     str[6] = up[3]; 
//     str[7] = up[2]; 
//     str[8] = up[1]; 
//     str[9] = up[0]; 
// #else
//     str[6] = up[0]; 
//     str[7] = up[1]; 
//     str[8] = up[2]; 
//     str[9] = up[3]; 
// #endif
//     mailbox.SendSrvRequest (usrname, (void *)str, 10, FALSE);

	//add by chenkai for new fes
	MailBox	mailbox(groupno,FEP_SERVER);  
	MailBox	pubmailbox(groupno,"PUBFEP"); 

	FE_MAIL_STRUCT* mail_struct;
	mail_struct = (FE_MAIL_STRUCT *)malloc( sizeof(FE_MAIL_STRUCT) + sizeof(FE_SETPOINT_MAIL) );	
	int datalen = sizeof(FE_SETPOINT_MAIL);		//数据区长度
	memset(mail_struct, 0, mail_struct->get_lenth()+datalen);
	mail_struct->mail_type = SET_POINT;
	mail_struct->mail_lenth = mail_struct->get_lenth() + datalen;		//总长度
	mail_struct->sub_information1 = 1;
	FE_SETPOINT_MAIL  set_point_mail ;
	set_point_mail.setvalue = setvalue ;
	set_point_mail.rtu_no = terminalno ;

	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	TERMINAL_STRU	*terminalstationp;
	if(fobjtable.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK) return;
	terminalstationp =  (TERMINAL_STRU*)fobjtable.RdbSearchRcdAddrByKey(&(terminalno));
	if (terminalstationp!=NULL&&terminalstationp->useflag)
	{
		set_point_mail.channel_no = terminalstationp->runchanno;
	}
	set_point_mail.yt_flag = outtype ;
	set_point_mail.yt_no = ytno ;
	memcpy(mail_struct->mail_body, (unsigned char*)&set_point_mail, datalen);

	mailbox.SendSrvRequest (usrname, (void *)mail_struct, mail_struct->mail_lenth, FALSE);//主备都发
	pubmailbox.SendPubSrvRequest (usrname, (void *)mail_struct, mail_struct->mail_lenth, FALSE);//主备都发
}

/*int getTerminalChanneladdr(int channo,void **addr,NETDATA *netdata)
{
	if(netdata==NULL)
	{
		*addr=NULL;
		return 0;
	}
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) {
		*addr=NULL;
		return 0;
	}
	
	if(channo<0 || channo>=G_CHANNUMS)
	{
		*addr=NULL;
		return 0;
	}

	for(int i = 0; i < G_CHANNUMS; i++)
	{
		if(netdata->channo[i] == channo)
		{
			*addr= (char *)(netdata->chan+i);
			return 1;
		}
	}
	return 0;

}*/

/*int getTerminalOtherChanneladdr(int channo,void **addr,NETDATA *netdata)
{
	if(netdata==NULL)
	{
		*addr=NULL;
		return 0;
	}
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) {
		*addr=NULL;
		return 0;
	}

	if(channo<0 || channo>=G_CHANNUMS)
	{
		*addr=NULL;
		return 0;
	}

	for(int i = 0; i < G_CHANNUMS; i++)
	{
		if(netdata->channo[i] == channo)
		{
			*addr= (char *)(netdata->otherchan+i);
			return 1;
		}
	}
	return 0;
}*/

/*void getTerminalDevaddr(ushort terminalno, uchar** addr,uchar** firstf,NETDATA *netdata)
{
	int devnum = 0;
	int	devnum_sum = 0;

	if(netdata==0)	return;
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) return;

	for(int i=0; i<netdata->rtunum; i++){
		devnum = netdata->devnum[i];
		if(netdata->rtuno[i]==terminalno){
			*addr = (uchar*)(netdata->devsta+devnum_sum*sizeof(uchar));
			*firstf = netdata->first.devfirst+devnum_sum;
			break;
		}
		devnum_sum += devnum;
	}
}*/

/*void getTerminalYcaddr (ushort terminalno, short** addr,uchar** firstf,NETDATA *netdata)
{
	int ycnum = 0;
	int	ycnum_sum = 0;

	if(netdata==0)
		return;
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) 
		return;

	for(int i=0; i<netdata->rtunum; i++)
	{
		ycnum = netdata->ycnum[i];
		if(netdata->rtuno[i]==terminalno)
		{
			*addr = (short*)(netdata->yc+ycnum_sum*2*sizeof(uchar));
			*firstf = netdata->first.ycfirst+ycnum_sum;
			break;
		}
		ycnum_sum += ycnum;
	}
}*/
/*void getTerminalYxaddr (ushort terminalno, uchar** addr,uchar** firstf,NETDATA *netdata)
{
	int yxnum = 0;
	int	yxnum_sum = 0;

	if(netdata==0)	return;
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) return;

	for(int i=0; i<netdata->rtunum; i++)
	{
		yxnum = (int) (netdata->yxnum[i]+7)/8;
		if(netdata->rtuno[i]==terminalno){
			*addr = (uchar*)(netdata->yx+yxnum_sum*sizeof(uchar));
			*firstf = netdata->first.yxfirst+yxnum_sum;
			break;
		}
		yxnum_sum += yxnum;
	}
}*/

/*void getTerminalKwhaddr (ushort terminalno, int** addr,uchar** firstf,NETDATA *netdata)
{
	int kwhnum = 0;
	int	kwhnum_sum = 0;

	if(netdata==0)	return;
	if(netdata->rtunum<=0 || netdata->rtunum>SCADA_MAX_TERMINALNUMS) return;

	for(int i=0; i<netdata->rtunum; i++){
		kwhnum = netdata->kwhnum[i];
		if(netdata->rtuno[i]==terminalno){
			*addr = (int *)(netdata->kwh+4*kwhnum_sum*sizeof(uchar));
			*firstf = netdata->first.kwhfirst+kwhnum_sum;
			break;
		}
		kwhnum_sum += kwhnum;
	}
}*/

//2004 05 add begin
int TerminalYcDeadDataProcessing(int groupno)
{
	int		i,j,analogrcdnums,terminalrcdnums=0;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
   	Rdb_Tab	fteminaltable(MyUserName, MyPassWord);
	BASEANALOG_STRU	*fbaseanalogp=NULL;	
	TERMINAL_STRU *fterminalp=NULL;
	Event   eventcl;
	int terminaldeadflag =0;

	if (fobjtable.RdbOpenTab(ANALOG_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	//fobjtable.RdbLockTabWithRW();
	analogrcdnums = fobjtable.RdbGetRcdCnt();

   	if (fteminaltable.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	//fteminaltable.RdbLockTabWithRW();
	terminalrcdnums = fteminaltable.RdbGetRcdCnt();

	for  (i=0; i<terminalrcdnums; i++) 
	{
		terminaldeadflag =1;
		fterminalp = (TERMINAL_STRU *)fteminaltable.rdbgetrcdaddr(i);
		if (fterminalp==NULL)	continue;
		//if (fterminalp->groupno != groupno) continue;
		for (j=0;j<analogrcdnums;j++)
		{
			fbaseanalogp = (BASEANALOG_STRU	*)fobjtable.rdbgetrcdaddr(j);
			if (fbaseanalogp==NULL)	continue;
			if (fbaseanalogp->groupno != groupno) continue;
			if (fbaseanalogp->terminalno==fterminalp->terminalno)
			{
				if (fbaseanalogp->deadf ==0 && !fbaseanalogp->calcultf)
				{
					terminaldeadflag = 0;//一个遥测为非死数，整个终端就非死数
					break;
				}
			}
		}//for j
		if (fterminalp->deadf != terminaldeadflag)
		{
			/*if (terminaldeadflag&&fterminalp->status)
			{
				eventcl.AddTerminalEvent(fterminalp->substation,fterminalp->name,fterminalp->describe,11,fterminalp->security);
			}*/
			fterminalp->deadf = terminaldeadflag;
		}
	}//for i
	//fteminaltable.RdbUnlockTab();
	//fobjtable.RdbUnlockTab();
	return (OK);
}
//2004 05 add end

//////////////////////////////////////////////////////////////////


#define		Analog_Table	1
#define		Status_Table	2
#define		DmsStatus_Table	3
#define		Breaker_Table	4
#define		Switch_Table	5
#define		ProtSig_Table	6
#define		Kwh_Table		7

static Analog   fanalog;
static Switch   fswitch;
static Break     fbreak;
static Protsig   fprotsig;
static StatusProcess    fstatus;
static DmsStatusProcess    fdmsstatus;
static Kwh       fkwh;
static Channel	chan;
static MultiStatus   fmultsta;
static Terminal   power_terminal;
static Substation  subst;

//static          terminal_scada_samehost=0;
//static short	*terminalycaddr[SCADA_MAX_TERMINALNUMS];
//static uchar	*terminalycfirstaddr[SCADA_MAX_TERMINALNUMS];

//static uchar	*terminalyxaddr[SCADA_MAX_TERMINALNUMS];
//static uchar	*terminalyxfirstaddr[SCADA_MAX_TERMINALNUMS];
//static char      terminalyxdelaytime[SCADA_MAX_TERMINALNUMS];

//static int		*terminalkwhaddr[SCADA_MAX_TERMINALNUMS];
//static uchar	*terminalkwhfirstaddr[SCADA_MAX_TERMINALNUMS];

static uchar	*terminaldevaddr[SCADA_MAX_TERMINALNUMS];
static uchar	*terminaldevfirstaddr[SCADA_MAX_TERMINALNUMS];

static NetControl sc;
/////////////////////////////////////////////////////
static char yx_rdbtablename[][TABNAME_LEN]={
	BREAK_TABNAME,SWITCH_TABNAME,PROT_TABNAME,STATUS_TABNAME,DMSSTATUS_TABNAME
};

#define  YX_RDBTABLENUMS  (sizeof(yx_rdbtablename)/TABNAME_LEN)


/////////////////////////////////////////////////////

void AllDataAccord(int groupno)
{
		DataAccord(groupno,ANALOG_TABNAME);
		DataAccord(groupno,BREAK_TABNAME);
		DataAccord(groupno,SWITCH_TABNAME);
		DataAccord(groupno,STATUS_TABNAME);
		DataAccord(groupno,DMSSTATUS_TABNAME);
		DataAccord(groupno,PROT_TABNAME);
		DataAccord(groupno,KWH_TABNAME);
		DataAccord(groupno,TERMINAL_TABNAME);
		DataAccord(groupno,CHANNEL_TABNAME);
}

//初始化小车开关状态
void init_carbreak_status(void)
{
	int  ret,rcdnums;
	uchar uCarBreakStat;
	Rdb_Tab memtab(MyUserName,MyPassWord);
	BREAK_STRU *pBreak;
	Break c_Break;

	if((ret=memtab.RdbOpenTab(BREAK_TABNAME))==OK) {
		rcdnums = memtab.RdbGetRcdCnt();
		for(pBreak=(BREAK_STRU *)memtab.rdbgetfirstrcdaddr();pBreak!=NULL;pBreak=(BREAK_STRU *)memtab.rdbgetnextrcdaddr()) 
		{
			if(pBreak->basestatus.baseobjtype == OBJTYPE_BRK_CAR && pBreak->basestatus.calcultf) //小车开关
				c_Break.CarbreakStatusProcessing((BASESTATUS_STRU *)pBreak,uCarBreakStat);
		}
	}
}

void init_linezero_cond(void)
{
	int  ret;
	Rdb_Tab memtab(MyUserName,MyPassWord);
	BASEANALOG_STRU  *baseanalogp;

	if((ret=memtab.RdbOpenTab(ANALOG_TABNAME))==OK) {
		for(baseanalogp=(BASEANALOG_STRU *)memtab.rdbgetfirstrcdaddr();baseanalogp!=NULL;baseanalogp=(BASEANALOG_STRU *)memtab.rdbgetnextrcdaddr()) {
			baseanalogp->linezero=0;
		}
	}

	/*
	BREAK_STRU *breakp;
	if((ret=memtab.RdbOpenTab(BREAK_TABNAME))!=OK)return;
	for(breakp=(BREAK_STRU *)memtab.rdbgetfirstrcdaddr();breakp!=NULL;breakp=(BREAK_STRU *)memtab.rdbgetnextrcdaddr()) 
	{
		check_linezero_cond(breakp);
	}
	*/
}

void scada_start_init(void)
{
	int			i;
	intertime   lasttime=0,curtime;
	SYS_CLOCK   lastclock,curclock;
	Analog		analog;
	Break		breaker;
    Terminal    terminal;
//    Ftu         ftu;
    Channel     channel;
	Rdb_SQL    rdbsql;
 
	NetStatus netstat;  //add
    int *host_group_no=netstat.GetHostGrpEnm();
	int groupnum=netstat.GetHostGrpNum();

	for(int j=0;j<groupnum;j++)
	{
		if(!IsMainServer(host_group_no[j]))
		{
			AllDataAccord(host_group_no[j]);
		}
    }
	subst.CalSafeDays(); //计算安全运行日

	//创建时段参数
	UpdateTimePeriodParaTab();
	FreshCurrentTimePeriodType();

	// 停机一天后初始化
	lasttime=GetSysLastTime();
	GetInterTime(&curtime);
	lasttime=curtime;  //add
	if(lasttime>0 && (curtime-lasttime)>120) 
	{
			ConvertTime(lasttime,&lastclock);
			ConvertTime(curtime,&curclock);
			if(lastclock.day!=curclock.day) 
			{
				analog.DayInit();
				breaker.DayInit();
				KwhDayInit();
                terminal.DayInit();
//                ftu.DayInit();
                channel.DayInit();
				
				int   inittabnums=0;
			    char  inittabname[20][TABNAME_LEN];

				strcpy(inittabname[inittabnums],ANALOG_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],BREAK_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],SWITCH_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],PROT_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],STATUS_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],DMSSTATUS_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],CHANNEL_TABNAME);inittabnums++;
				strcpy(inittabname[inittabnums],TERMINAL_TABNAME);inittabnums++;
	
				for(int i=0;i<inittabnums;i++) 
				{
					rdbsql.RdbUpdTab(inittabname[i]);
					rdbsql.RdbUpdFld("lastproct",RDB_DATATYPE_INT,&curtime);
					rdbsql.RdbUpdCmd(MyUserName,MyPassWord);
				}
			}
	}

	// 初始化小车开关
	init_carbreak_status();

	// 初始化线路
	init_linezero_cond();

	// 自动旁路替代
	AutoCheckBreakReplace();

	ProcCard   	procardcl;
	procardcl.InitBusCheckFlag();
	fanalog.ycdatap = new SAVE_YCDATA;
	memset(fanalog.ycdatap, 0, sizeof(SAVE_YCDATA));
	fstatus.yxdatap = new SAVE_YXDATA;
	memset(fstatus.yxdatap, 0, sizeof(SAVE_YXDATA));
//	terminal_scada_samehost=netstat.IsServer(FEP_SERVER);
	//memset(terminalyxdelaytime,0,sizeof(terminalyxdelaytime));
	for (i=0;i<MAX_RTU_NUM;i++)
	{
		scadachanpara.terminaldata[i].rtu_no = i;
		scadachanpara.terminaldata[i].grouppriority =-1;
		scadachanpara.terminaldata[i].channel_no = -1;
		memset(scadachanpara.terminaldata[i].chname,0,OBJECTNAME_LEN);
		scadachanpara.terminaldata[i].last_channel_no = -1;
	}
	char dirname[200];
	memset(dirname,0,sizeof(char)*200);
	char *sacenv=getenv(NBENV);

	if (sacenv==NULL)
	{
		return;
	}
	else
	{
		strcpy(dirname,sacenv);
#ifdef WIN32
		strcat(dirname,"\\procscript");
		if (!CreateDirectory(dirname, NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
		{
			return;
		}
#endif
#ifdef __linux
		strcat(dirname,"/procscript");
		DIR *dir;//声明一个句柄
		dir = opendir(dirname);
		if( dir==NULL)
		{
			mkdir(dirname,0777);
		}
#endif
	}
}




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

// 转换挂牌类型
int convert_cardtype(int type,int mode)
{
	static int  convtype[][2]=
	{	
		63,1,
		5, 2,
		1, 3,
		61,4,
		51,5,
		3, 6,
		60,7,
		62,8,
		6,9,
		7,10,
		8,11
	};

	if(mode<0 || mode>1)return(SCADA_ERR_PARA);
	for(int i=0;i<sizeof(convtype)/(2*sizeof(int));i++) 
	{
		if(convtype[i][mode]==type) 
		{
			if(mode==0)return(convtype[i][1]);
			else return(convtype[i][0]);
		}
	}

	return(SCADA_ERR_PARA);
}

static int SEND_PUTCARD_FORE=GetProgramRunPara("foresdgp",0);

// 予处理前置挂牌信息
int preproc_fertguapai(SCADA_SET_SPECIALF_STRU *setsp)
{
	int ret;
	TERMINALYXSORT_STRU				terminalyx, *terminalyxsortp;
	char  tmpdevname[POWDEVNAME_LEN],tmpdevtype;

	Rdb_Tab	yxtable(MyUserName,MyPassWord);
	ret=yxtable.RdbOpenTab(YXNOSORT_TABNAME);
	if(ret!=RDB_OK)return(ret);
	terminalyx.terminalno=(short)setsp->cardrcd.startx;
	terminalyx.yxno=(short)setsp->cardrcd.starty;
	int rcdno=yxtable.RdbSearchRcdByKey(&terminalyx);
	if(rcdno>=0)
	{
		terminalyxsortp=(TERMINALYXSORT_STRU *)yxtable.rdbgetrcdaddr(rcdno);
		if(terminalyxsortp==NULL) return(SCADA_ERR_NOOBJECT);
		if(IS_BREAK_TYPE(terminalyxsortp->objtype))setsp->cardrcd.objtype=DEVTYPE_BREAKER;
		else if(IS_SWITCH_TYPE(terminalyxsortp->objtype))setsp->cardrcd.objtype=DEVTYPE_BREAKER;
		else return(SCADA_ERR_NOOBJECT);
		ret=DecodeObjName(terminalyxsortp->objname,&tmpdevtype,NULL,tmpdevname,(short *)NULL);
		if(ret!=RDB_OK)return(ret);
		strcpy(setsp->cardrcd.objname,tmpdevname);
		setsp->cardrcd.cardtype=convert_cardtype(setsp->cardrcd.cardtype,1);
		if(setsp->cardrcd.cardtype<0)return(setsp->cardrcd.cardtype);
		setsp->cardrcd.startx=0;
		setsp->cardrcd.starty=0;
	}
	else return(rcdno);
	return(OK);
}

static void savelog( char *buf )
{
	return;
	FILE *fp;
	int i;
	char filename[60];
	sprintf(filename,"%s/log/dat/proc_scadaservice.txt",getenv("NBENV"));
	fp = fopen(filename,"a+");
	fprintf( fp, "%s\n", buf );
	fclose( fp );
}

// 处理邮件中的请求服务（ScadaService任务）
int  proc_scadaservice(char *hm,char *um,char *pd,hlong tid,int servicetype,void *scdsbuf,int sbuflen)
{
	Scdservice_serv  scadaservice(hm,um,pd,tid);
	int i,sendretf=0,freshstreamf=0,ret;
    int tmpgroupno;
	NetDbg dbg;
	
	switch(servicetype) 
	{
		case SCADA_TYPE_MANSET:	// 人工置数
			 dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理人工置数邮件\n");
			 SCADA_MANSET_STRU  *mansetp;
        	 mansetp=(SCADA_MANSET_STRU *)scdsbuf;
			 scdhostbyte(RDB_DATATYPE_INT,&mansetp->groupno);
			 scdhostbyte(RDB_DATATYPE_DOUBLE,&mansetp->setval);
			 ret=scadaservice.manset(mansetp);
			 if(ret==0)freshstreamf=1;
			 sendretf=1;
			 tmpgroupno=mansetp->groupno;
			 break;
		case SCADA_TYPE_YK:		//  遥控
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理遥控邮件\n");
				SCADA_YK_STRU*  temp = (SCADA_YK_STRU*)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&temp->groupno);
				tmpgroupno=temp->groupno;
				 if (IsMainServer(tmpgroupno)) 
				 {
					ret=scadaservice.ykcmd((SCADA_YK_STRU *)scdsbuf);
					SCADA_YK_STRU*  temp = (SCADA_YK_STRU*)scdsbuf;
					if(ret!=OK)
					{
						SendYkResMail(temp->groupno,hm,um,tid,NULL,NULL,0,
							((SCADA_YK_STRU *)scdsbuf)->objectname,((SCADA_YK_STRU *)scdsbuf)->tablename,SCADA_YK_CMD_ERR,ret);
					}
				 }
			 }
			 break;
		case SCADA_TYPE_YT:		//  遥调
			{
			     dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理遥调邮件\n");
				 scdhostbyte(RDB_DATATYPE_INT,&((SCADA_YT_STRU *)scdsbuf)->groupno);
				 tmpgroupno=((SCADA_YT_STRU *)scdsbuf)->groupno;
				 if (IsMainServer(tmpgroupno)) 
				 {
					scdhostbyte(RDB_DATATYPE_FLOAT,&((SCADA_YT_STRU *)scdsbuf)->setvalue);
					ret=scadaservice.ytcmd((SCADA_YT_STRU *)scdsbuf);
					sendretf=1;
				 }
			}
			break;
		case SCADA_TYPE_FOREYKCHK:	// FEP服务器遥控返校
			{
			     dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理FEP服务器遥控返校邮件\n");
				 SCADA_FOREYKCHK_STRU *ykchkp;
				 ykchkp=(SCADA_FOREYKCHK_STRU *)scdsbuf;
				 scdhostbyte(RDB_DATATYPE_INT,&ykchkp->groupno);
				 tmpgroupno=ykchkp->groupno;
				 if (IsMainServer(tmpgroupno)) 
				 {

					scdhostbyte(RDB_DATATYPE_INT,&ykchkp->terminalno);
					scdhostbyte(RDB_DATATYPE_INT,&ykchkp->ykno);
					ret=scadaservice.fertykchk(ykchkp);
				 }
			}
			 break;
		case SCADA_TYPE_UPDOWN:		// 升降
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理升降邮件\n");
				SCADA_UPDOWN_STRU* temp=(SCADA_UPDOWN_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&temp->groupno);
			    tmpgroupno=temp->groupno;
				if (IsMainServer(tmpgroupno)) 
				 {
					ret=scadaservice.updowncmd((SCADA_UPDOWN_STRU *)scdsbuf);
					SCADA_UPDOWN_STRU* temp=(SCADA_UPDOWN_STRU *)scdsbuf;
					if (ret!=OK) 
					{
						dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"RET:\t= %d\n",ret);
					}
					if(ret!=OK)SendYkResMail(temp->groupno,hm,um,tid,NULL,NULL,0,
						((SCADA_UPDOWN_STRU *)scdsbuf)->objectname,"",SCADA_YK_CMD_ERR,ret,1);
				 }
			}
			 break;
		case SCADA_TYPE_SETSPECIALF:	// 设置指定的操作标志
			{
				SCADA_SET_SPECIALF_STRU  *tmpsetsp;
				tmpsetsp=(SCADA_SET_SPECIALF_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&tmpsetsp->groupno);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.groupno);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.firststate);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.secondstate);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.objtype);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.cardtype);
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.cardno);
				tmpgroupno=tmpsetsp->groupno;
#ifdef GIS
				#pragma message("Compile GIS...")
				scdhostbyte(RDB_DATATYPE_DOUBLE,&tmpsetsp->cardrcd.startx);
				scdhostbyte(RDB_DATATYPE_DOUBLE,&tmpsetsp->cardrcd.starty);
				scdhostbyte(RDB_DATATYPE_DOUBLE,&tmpsetsp->cardrcd.width);
				scdhostbyte(RDB_DATATYPE_DOUBLE,&tmpsetsp->cardrcd.height);
#else
				scdhostbyte(RDB_DATATYPE_INT,&tmpsetsp->cardrcd.startx);
				scdhostbyte(RDB_DATATYPE_INT,&tmpsetsp->cardrcd.starty);
#ifndef QTGRAPH
				scdhostbyte(RDB_DATATYPE_INT,&tmpsetsp->cardrcd.width);
				scdhostbyte(RDB_DATATYPE_INT,&tmpsetsp->cardrcd.height);
#endif
#endif
#ifndef QTGRAPH
				scdhostbyte(RDB_DATATYPE_SHORT,&tmpsetsp->cardrcd.imagetype);
#endif
#ifdef QTGRAPH
				#pragma message("Compile QTGRAPH...")
				scdnetbyte(RDB_DATATYPE_INT,&tmpsetsp->cardrcd.metaid);
#endif

				tmpsetsp->cardrcd.objname[sizeof(tmpsetsp->cardrcd.objname)-1]='\0';
#ifndef QTGRAPH
				tmpsetsp->cardrcd.imagename[sizeof(tmpsetsp->cardrcd.imagename)-1]='\0';
#endif
				tmpsetsp->cardrcd.hostname[sizeof(tmpsetsp->cardrcd.hostname)-1]='\0';
				tmpsetsp->cardrcd.username[sizeof(tmpsetsp->cardrcd.username)-1]='\0';
				tmpsetsp->cardrcd.context[sizeof(tmpsetsp->cardrcd.cardtime)-1]='\0';
#ifndef QTGRAPH
				tmpsetsp->cardrcd.bitmap[sizeof(tmpsetsp->cardrcd.bitmap)-1]='\0';
				tmpsetsp->cardrcd.userdefgr[sizeof(tmpsetsp->cardrcd.userdefgr)-1]='\0';
#endif
#ifdef QTGRAPH
				tmpsetsp->cardrcd.graphpath[sizeof(tmpsetsp->cardrcd.graphpath)-1]='\0';
				tmpsetsp->cardrcd.metapath[sizeof(tmpsetsp->cardrcd.metapath)-1]='\0';
#endif
				tmpsetsp->cardrcd.context[sizeof(tmpsetsp->cardrcd.context)-1]='\0';
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理挂牌邮件,组号=%d\n",tmpgroupno);
				if(SEND_PUTCARD_FORE) 
				{
					if(strcmp(tmpsetsp->cardrcd.username,"ForePutCard")==0) 
					{
						if(preproc_fertguapai(tmpsetsp)<0)break;
					}
				}
				ret=scadaservice.SetSpecialF(tmpsetsp);
				if(ret==0)freshstreamf=1;
				sendretf=1;
			}
			 break;
		case SCADA_TYPE_CLRNOTIFYQUE:	// 事项确认
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理事项确认邮件\n");
				SCADA_CLR_NOTIFYQUE_STRU  *tmpcnqp;
				tmpcnqp=(SCADA_CLR_NOTIFYQUE_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&tmpcnqp->groupno);
				tmpgroupno=tmpcnqp->groupno;
				scdnetbyte(RDB_DATATYPE_SHORT,&tmpcnqp->notifyque.evt.sort);
				scdnetbyte(RDB_DATATYPE_SHORT,&tmpcnqp->notifyque.evt.type);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.ymd);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.hmsms);
				scdnetbyte(RDB_DATATYPE_SHORT,&tmpcnqp->notifyque.evt.state);
				scdnetbyte(RDB_DATATYPE_SHORT,&tmpcnqp->notifyque.evt.state1);
				scdnetbyte(RDB_DATATYPE_SHORT,&tmpcnqp->notifyque.evt.state2);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.val);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.val1);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.val2);
				scdnetbyte(RDB_DATATYPE_INT,&tmpcnqp->notifyque.evt.val3);
				tmpcnqp->notifyque.evt.Object[sizeof(tmpcnqp->notifyque.evt.Object)-1]='\0';
				tmpcnqp->notifyque.evt.member0[sizeof(tmpcnqp->notifyque.evt.member0)-1]='\0';
				tmpcnqp->notifyque.evt.member1[sizeof(tmpcnqp->notifyque.evt.member1)-1]='\0';
				tmpcnqp->notifyque.evt.member2[sizeof(tmpcnqp->notifyque.evt.member2)-1]='\0';
				tmpcnqp->notifyque.evt.member3[sizeof(tmpcnqp->notifyque.evt.member3)-1]='\0';
				tmpcnqp->notifyque.evt.graph[sizeof(tmpcnqp->notifyque.evt.graph)-1]='\0';
				tmpcnqp->notifyque.evt.tone[sizeof(tmpcnqp->notifyque.evt.tone)-1]='\0';
				tmpcnqp->notifyque.evt.text[sizeof(tmpcnqp->notifyque.evt.text)-1]='\0';
				tmpcnqp->notifyque.value_des[sizeof(tmpcnqp->notifyque.value_des)-1]='\0';
				tmpcnqp->notifyque.group_des[sizeof(tmpcnqp->notifyque.group_des)-1]='\0';
				tmpcnqp->notifyque.member_des0[sizeof(tmpcnqp->notifyque.member_des0)-1]='\0';
				tmpcnqp->notifyque.member_des1[sizeof(tmpcnqp->notifyque.member_des1)-1]='\0';
				tmpcnqp->notifyque.member_des2[sizeof(tmpcnqp->notifyque.member_des2)-1]='\0';
				tmpcnqp->notifyque.member_des3[sizeof(tmpcnqp->notifyque.member_des3)-1]='\0';
				tmpcnqp->notifyque.member_des4[sizeof(tmpcnqp->notifyque.member_des4)-1]='\0';
				tmpcnqp->notifyque.member_des5[sizeof(tmpcnqp->notifyque.member_des5)-1]='\0';
				tmpcnqp->notifyque.otherdes0[sizeof(tmpcnqp->notifyque.otherdes0)-1]='\0';
				tmpcnqp->notifyque.otherdes1[sizeof(tmpcnqp->notifyque.otherdes1)-1]='\0';
				tmpcnqp->notifyque.otherdes2[sizeof(tmpcnqp->notifyque.otherdes2)-1]='\0';
				tmpcnqp->notifyque.otherdes3[sizeof(tmpcnqp->notifyque.otherdes3)-1]='\0';
				tmpcnqp->notifyque.otherdes4[sizeof(tmpcnqp->notifyque.otherdes4)-1]='\0';
				tmpcnqp->notifyque.otherdes5[sizeof(tmpcnqp->notifyque.otherdes5)-1]='\0';
				tmpcnqp->notifyque.longdes[sizeof(tmpcnqp->notifyque.longdes)-1]='\0';

				ret=scadaservice.ClrNotifyEvtQue(tmpcnqp);
				if(ret==0)freshstreamf=1;
				sendretf=1;
			}
			 break;
		case SCADA_TYPE_WFBS:	//五防闭锁
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理五防闭锁邮件\n");
				SCADA_WFBS_STRU  *wfbsp;
				wfbsp=(SCADA_WFBS_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&wfbsp->groupno);
				tmpgroupno=wfbsp->groupno;
				scdnetbyte(RDB_DATATYPE_SHORT,&wfbsp->no);
				ret=scadaservice.DealWfbs(wfbsp);
				if(ret==0)freshstreamf=1;
				sendretf=1;
			}
			 break;
		case SCADA_TYPE_RAWEVENT:
			{
			    
				//static int mailcount = 0;
	           // NetDbg dbg;
			 	int rawentnums;
				FE_DATA_EVENT_NODE  *tmpscdsrawentp;
				rawentnums=sbuflen/sizeof(FE_DATA_EVENT_NODE);
				tmpscdsrawentp=(FE_DATA_EVENT_NODE  *)scdsbuf;
               // mailcount ++;
	           //TRACE( "\n mailcount = %d\n",mailcount);
		       // dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理生事项邮件\n");
				//char buf[ 256 ];

				//sprintf(buf,"SCADA:收到前置生事项%d条\n", rawentnums );
				//savelog( buf );

#ifndef _WIN32
				/*for(i=0;i<rawentnums;i++) 
				{
					scdhostbyte(RDB_DATATYPE_INT,&tmpscdsrawentp[i].groupno);
					scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].Year);
      				scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].Ms);
               		scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].reserve1);
					scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].groupno);
					scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].TERMINALNo);
       	 			scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].YxOrder1);
               		scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].FTUNo1);
					scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].YxOrder2);
       				scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].FTUNo2);
               		scdhostbyte(RDB_DATATYPE_SHORT,&tmpscdsrawentp[i].Val.yx.yxOrder);
				}*/
#endif
				tmpgroupno=1;
				ret=scadaservice.put_fertrawevent(tmpscdsrawentp,rawentnums);
				freshstreamf=1;
			}
			break;
		case	SCADA_TYPE_OLEVENT:
			{
				SCADA_OVERLIMIT_STRU *olf = (SCADA_OVERLIMIT_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&olf->groupno);
				scdhostbyte(RDB_DATATYPE_FLOAT,&olf->olvalue);
				tmpgroupno=olf->groupno;
				if(IsMainServer(tmpgroupno))
				{
					ret = scadaservice.YcOl_Event(olf);
					dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理遥测越限生事项邮件\n");
				}
			}
			break;
		case	SCADA_TYPE_STARTPDR:
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理手工启动追忆邮件\n");
				SCADA_STARTPDR_STRU *spdr = (SCADA_STARTPDR_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&spdr->groupno);
				tmpgroupno=spdr->groupno;
				if(IsMainServer(tmpgroupno))
				{

					ret = scadaservice.Start_PDR(spdr);
					sendretf=1;
				}
			}
			break;
		case	SCADA_TYPE_SERIALYK:
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理序列控制邮件\n");
				SCADA_SERIALYK_STRU *serialyk = (SCADA_SERIALYK_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&serialyk->groupno);
				tmpgroupno=serialyk->groupno;
				if(IsMainServer(tmpgroupno))
				{

					SCADA_YKOBJNAME_STRU  *serobjp=(SCADA_YKOBJNAME_STRU  *)((char *)scdsbuf + sizeof(SCADA_SERIALYK_STRU));
					scdhostbyte(RDB_DATATYPE_INT,&serialyk->objnums);
					ret = scadaservice.SerialYKCmd(serialyk,serobjp,tid);
					dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\tSerialYKCmd ret=%d groupno=%d\n",ret,serialyk->groupno);
				}
			}
			break;
		case	SCADA_TYPE_GROUPYK:
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理群控邮件,tid=%d\n",(int)tid);
				SCADA_GROUPYK_STRU *groupyk = (SCADA_GROUPYK_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&groupyk->groupno);
				tmpgroupno=groupyk->groupno;
				if(IsMainServer(tmpgroupno))
				{
					SCADA_YKOBJNAME_STRU *ykgrpobj = (SCADA_YKOBJNAME_STRU *)((char *)scdsbuf +sizeof(SCADA_GROUPYK_STRU));
					scdhostbyte(RDB_DATATYPE_INT,&groupyk->objnums);
					ret = scadaservice.GroupYKCmd(groupyk, ykgrpobj,tid);
				}
			}
			break;
		case    SCADA_TYPE_SERIALYKCTRL:
			{
				SCADA_SERIALYK_CTRL_STRU *tmpctrlp=(SCADA_SERIALYK_CTRL_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&tmpctrlp->groupno);
				tmpgroupno=tmpctrlp->groupno;
				if(IsMainServer(tmpgroupno))
				{

					ret = scadaservice.SerialYKCtrl(tmpctrlp);
				}
			}
			break;
		case    SCADA_TYPE_GROUPYKCTRL:
			{
				SCADA_SERIALYK_CTRL_STRU *tmpctrlp=(SCADA_SERIALYK_CTRL_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&tmpctrlp->groupno);
				tmpgroupno=tmpctrlp->groupno;
				if(IsMainServer(tmpgroupno))
				{

					ret = scadaservice.GroupYKCtrl(tmpctrlp);
				}
			}
			break;
		case	SCADA_TYPE_LINEREPLACE:
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理旁路替代邮件\n");
				SCADA_LINE_REPLACE_STRU *lrepl = (SCADA_LINE_REPLACE_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&lrepl->groupno);
				tmpgroupno=lrepl->groupno;
				ret = scadaservice.Line_Replace(lrepl);
				sendretf=1;
			}
			break;
		case	SCADA_TYPE_CLRCHGFLAG:
			{
				char substation[OBJECTNAME_LEN];
				strcpy(substation,(char *)scdsbuf);
				ret = scadaservice.ClearChangeF(substation);
				freshstreamf=1;
			}
			break;
		case	SCADA_TYPE_CLRALARMFLAG:
			{
			    dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t处理清除报警邮件\n");
				SCADA_CLRALARMF_STRU  *tmpclearalarm;
				char  *tmpobjname;
                tmpclearalarm=(SCADA_CLRALARMF_STRU  *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&tmpclearalarm->groupno);
				tmpgroupno=tmpclearalarm->groupno;
				scdhostbyte(RDB_DATATYPE_INT,&tmpclearalarm->mode);
				scdhostbyte(RDB_DATATYPE_INT,&tmpclearalarm->nums);
				tmpobjname=(char *)scdsbuf + sizeof(SCADA_CLRALARMF_STRU);
				ret = scadaservice.ClearAlarmF(tmpclearalarm->mode,tmpclearalarm->nums,tmpobjname);
				freshstreamf=1;
			}
			break;
		case SCADA_TYPE_MASKCHGENT:		//封锁变位事项
			{
				SCADA_MASKCHG_STRU *maskchgp;
				maskchgp=(SCADA_MASKCHG_STRU *)scdsbuf;
				scdhostbyte(RDB_DATATYPE_INT,&maskchgp->groupno);
				tmpgroupno=maskchgp->groupno;
				ret=scadaservice.maskchgent(maskchgp);
				sendretf=1;
			}
			break;
		case SCADA_TYPE_CCFORMULA:
			{
				ret=scadaservice.CCFormula((SCADA_CC_FORMULA_STRU *)scdsbuf);
				tmpgroupno=-1;
			}
			break;
		case SCADA_TYPE_NETYCDATA:
			{
				int netycnums=sbuflen/sizeof(SCADA_NET_YCDATA_STRU);
				SCADA_NET_YCDATA_STRU *netycdatap=(SCADA_NET_YCDATA_STRU *)scdsbuf;
				for(i=0;i<netycnums;i++){
					scdhostbyte(RDB_DATATYPE_FLOAT,&netycdatap[i].value);
					scdhostbyte(RDB_DATATYPE_INT,&netycdatap[i].groupno);
				}
				tmpgroupno=netycdatap->groupno;
				scadaservice.proc_netycdata(netycnums,netycdatap);
			}
			break;
		case SCADA_TYPE_NETYXDATA:
			{
				int netyxnums=sbuflen/sizeof(SCADA_NET_YXDATA_STRU);
				SCADA_NET_YXDATA_STRU *netyxdatap=(SCADA_NET_YXDATA_STRU *)scdsbuf;
				for(i=0;i<netyxnums;i++){
					scdhostbyte(RDB_DATATYPE_SHORT,&netyxdatap[i].value);
					scdhostbyte(RDB_DATATYPE_INT,&netyxdatap[i].groupno);
				}
				tmpgroupno=netyxdatap->groupno;
				scadaservice.proc_netyxdata(netyxnums,netyxdatap);
			}
			break;
		case SCADA_TYPE_NETKWHDATA:
			{
				int netkwhnums=sbuflen/sizeof(SCADA_NET_KWHDATA_STRU);
				SCADA_NET_KWHDATA_STRU *netkwhdatap=(SCADA_NET_KWHDATA_STRU *)scdsbuf;
				for(i=0;i<netkwhnums;i++){
					scdhostbyte(RDB_DATATYPE_SHORT,&netkwhdatap[i].value);
					scdhostbyte(RDB_DATATYPE_INT,&netkwhdatap[i].groupno);
				}
				tmpgroupno=netkwhdatap->groupno;
				scadaservice.proc_netkwhdata(netkwhnums,netkwhdatap);
			}
			break;
		default:
			break;
	}

	if(IsMainServer(tmpgroupno))
	{
		if(freshstreamf)
		{
			sc.FreshDataStream();
		}

		if(sendretf && tmpgroupno>=0)
		{
			send_scadaservice_err(tmpgroupno,hm, um, tid, servicetype, ret);
		}
	}
	return(OK);
}

// 处理Scada邮件级别为0的线程
// 调用 RecvScdRequestMail 函数取出邮件
#if defined WIN32
UINT scadamailproc_level0(LPVOID pParam)
#else
void * scadamailproc_level0(void *argp)
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
		if ((servicetype=RecvScdRequestMail(0,hostname,username,password,&tid,(void *)scdsbuf,&tmplen))!=SCADA_TYPE_NULL) 
		{
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t服务器收到邮件，级别0\n");
			proc_scadaservice(hostname,username,password,tid,servicetype,scdsbuf,tmplen);
		}
		else sleepms(150);
	}
	return NULL;
}


// 处理Scada邮件级别为1的线程
// 调用 RecvScdRequestMail 函数取出邮件
#if defined WIN32
UINT scadamailproc_level1(LPVOID pParam)
#else
void * scadamailproc_level1(void *argp)
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
		if ((servicetype=RecvScdRequestMail(1,hostname,username,password,&tid,(void *)scdsbuf,&tmplen))!=SCADA_TYPE_NULL) 
		{
			dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t服务器收到邮件，级别1\n");
			proc_scadaservice(hostname,username,password,tid,servicetype,scdsbuf,tmplen);
		}
		else sleepms(50);
	}
	return NULL;
}

// 处理Scada邮件级别为2的线程,处理前置带时标数据
#if defined WIN32
UINT scadamailproc_level2(LPVOID pParam)
#else
void * scadamailproc_level2(void *argp)
#endif
{
	void		*mailbuf;
    int			maillen; 
	MAIL_HEAD	srchead,taghead;
	MailBox	scadamail(0,SCD_SERVER);//0无用,由NET判
	NetStatus netstat;
	NetDbg dbg;
	int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	for (;;) 
	{
		if (scadamail.RecvSrvResult(&srchead,&taghead,&mailbuf,&maillen,SCD_SERVER,grpnum,host_group_no,2)==TRUE) 
		{
				dbg.PutDbg(NET_DBGTYPE_RX_MAIL,"SCADA:\t服务器收到邮件，级别2\n");
                proc_fertdata(mailbuf,srchead.host);
		}
     	else sleepms(10);
	}
	return NULL;
}






//处理有了服务代码的遥控命令:YK_CHECKKOK,YK_CHECKERR,YK_SUCCESS,YK_REFUSE,YK_CHECKTIMEOUT,YK_CONFIRMTIMEOUT,YK_EXECTIMEOUT;

static int YkprocCodeProcessing(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	int ret;
	ENT_YK_STRU     *ykmsg;
		
	ykmsg=(ENT_YK_STRU  *)apentmsg;

	if(apentcode==APEVENT_YK)
	{
		if(ykmsg->programykf)return(OK);
		SendYkResMail(ykmsg->groupno,ykmsg->ophostname,ykmsg->operatorname,ykmsg->optaskid,
						ykmsg->confirmhost,ykmsg->confirmname,ykmsg->confirmtaskid,
						ykmsg->objectname,ykmsg->tabname,ykmsg->procsercode,0);


		SerialYK   serialyk;
		ret=YkSerClassify(ykmsg->procsercode);
		if(ret==1) serialyk.ProcYkResult(ykmsg->objectname,1);
		else if(ret==-1) serialyk.ProcYkResult(ykmsg->objectname,0);
	}
	else if(apentcode==APEVENT_UPDOWN)
	{
		if(ykmsg->programykf)return(OK);
		SendYkResMail(ykmsg->groupno,ykmsg->ophostname,ykmsg->operatorname,ykmsg->optaskid,
						ykmsg->confirmhost,ykmsg->confirmname,ykmsg->confirmtaskid,
						ykmsg->objectname,"",ykmsg->procsercode,0,1);
	}//if(APEVENT_YK)
return(OK);
}




int YkResultProcessing(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *ykquep)
{
	ENT_STATUSCHG_STRU  *stachgmsg;
	if(apentcode==APEVENT_STACHG) 
	{
		stachgmsg=(ENT_STATUSCHG_STRU  *)apentmsg;
		if(stachgmsg->maketype<SATATUSCHG_MAKEENT_YN) {
			((Ykqueue *)ykquep)->YkResult(stachgmsg->tabname,stachgmsg->objectname,stachgmsg->staval);
		}
	}
	return(OK);
}

int ForeYkCmdProcessing(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	ENT_YKCMD_STRU  *ykcmdmsg;
	NetDbg dbg;
	if(apentcode==APEVENT_YKCMD) 
	{
		ykcmdmsg=(ENT_YKCMD_STRU  *)apentmsg;
		switch(ykcmdmsg->ykcmd) 
		{
		case YKCMD_PRESET:
			dbg.PutDbg(NET_DBGTYPE_TX_MAIL,"SCADA:\t向FEP服务器发送遥控预置邮件\n");
			fertykcmd(SCDPROC_USERNAME,MAILYKCMD,ykcmdmsg->ykgroupno,ykcmdmsg->ykrtuno,ykcmdmsg->ykno,ykcmdmsg->ykaction);
			break;
		case YKCMD_EXEC:
			dbg.PutDbg(NET_DBGTYPE_TX_MAIL,"SCADA:\t向FEP服务器发送遥控执行邮件\n");
			fertykcmd(SCDPROC_USERNAME,MAILYKEXE,ykcmdmsg->ykgroupno,ykcmdmsg->ykrtuno,ykcmdmsg->ykno,ykcmdmsg->ykaction);
			break;
		case YKCMD_CANCEL:
			dbg.PutDbg(NET_DBGTYPE_TX_MAIL,"SCADA:\t向FEP服务器发送遥控撤消邮件\n");
			fertykcmd(SCDPROC_USERNAME,MAILYKDEL,ykcmdmsg->ykgroupno,ykcmdmsg->ykrtuno,ykcmdmsg->ykno,ykcmdmsg->ykaction);
			break;
		default:break;
		}//switch
	}//if
	return(OK);
}


int YtCmdProcessing(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	ENT_YT_STRU  *ytcmdmsg;
	NetDbg dbg;
	if(apentcode==APEVENT_YT) 
	{
		ytcmdmsg=(ENT_YT_STRU  *)apentmsg;
		if(ytcmdmsg->ytsercode==YT_SENDCMD) 
		{
			dbg.PutDbg(NET_DBGTYPE_TX_MAIL,"SCADA:\t向FEP服务器发送遥调预置邮件\n");
			fertytcmd(SCDPROC_USERNAME,ytcmdmsg->groupno,ytcmdmsg->terminalno,ytcmdmsg->ytno,ytcmdmsg->outtype,ytcmdmsg->setvalue);
		}
		else if ( ytcmdmsg->ytsercode == YT_SUCCESS )
		{
			SendYkResMail(ytcmdmsg->groupno,ytcmdmsg->ophostname,ytcmdmsg->operatorname,ytcmdmsg->optaskid,
				"","",0,ytcmdmsg->objectname,"",SCADA_YK_SUCCESS,0,2);
		}
	}//if
	return(OK);
}


int ForePutCardProcessing(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	ENT_SETSPECIALFLAG_STRU  *setspmsg;
	if(apentcode==APEVENT_SETSPECIALF) 
	{
		short tmpobjtype;
		char tmpobjname[OBJECTNAME_LEN];
		setspmsg=(ENT_SETSPECIALFLAG_STRU  *)apentmsg;
		if(setspmsg->cardrcd.objtype==DEVTYPE_BREAKER)tmpobjtype=OBJTYPE_BREAK;
		else if(setspmsg->cardrcd.objtype==DEVTYPE_SWITCH)tmpobjtype=OBJTYPE_SWITCH;
		else return(OK);
		if(strcmp(setspmsg->username,"ForePutCard")==0)return(OK);
		EncodeObjName((char)setspmsg->cardrcd.objtype,"",setspmsg->cardrcd.objname,tmpobjtype,tmpobjname);
		BASESTATUS_STRU   *tmpbstap=GetYXTableTypeForRdb(tmpobjname,tmpobjtype);
		if(tmpbstap==NULL)return(OK);
		if(tmpbstap->calcultf)return(OK);
		int fertcardtype=convert_cardtype(setspmsg->cardrcd.cardtype,0);
		if(fertcardtype<0)return(OK);
		fertguapaicmd(MyUserName,0, tmpbstap->groupno,tmpbstap->terminalno,tmpbstap->yxno,
					fertcardtype,setspmsg->setflag);
	}
	return(OK);
}

///// 主机切为备机时的处理过程
void backserver_process(int groupno)
{
	SerialYK  serialyk;
	Ykqueue   ykque;

	serialyk.StopAllGroup();
	ykque.StopAllYk();
}


// 处理遥控、升降的线程
#if defined WIN32
UINT ykupdownproc(LPVOID pParam)
#else
void * ykupdownproc(void * argp)
#endif
{
	APEvent		apevent;
	Ykqueue		ykq;
	Ytqueue     ytprocess;
	uint		timecount=0;
	SerialYK    serialyk;
	static  int mainflag[GROUP_MAX];

	NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();

	apevent.AddApEventCallback(APEVENT_STACHG,YkResultProcessing,&ykq,APEVENT_NULL);
	apevent.AddApEventCallback(APEVENT_YKCMD,ForeYkCmdProcessing,NULL,APEVENT_NULL); 
	apevent.AddApEventCallback(APEVENT_YT,YtCmdProcessing,NULL,APEVENT_NULL); 
	apevent.AddApEventCallback(APEVENT_YK,YkprocCodeProcessing,NULL,APEVENT_NULL); 
	apevent.AddApEventCallback(APEVENT_UPDOWN,YkprocCodeProcessing,NULL,APEVENT_NULL); 
	if(SEND_PUTCARD_FORE)
		apevent.AddApEventCallback(APEVENT_SETSPECIALF,ForePutCardProcessing,NULL,APEVENT_NULL); 

	timecount=0;

	for (;;) 
	{
		if(timecount%2==0) 
		{
			serialyk.ExecSerialYK();
			ykq.YkQueueLoop();
			ytprocess.YtQueueLoop();
		}
		apevent.ApEventLoop(1);
        for(int j=0;j<grpnum;j++)
		{
			if(IsMainServer(host_group_no[j]))
			{
				mainflag[j]=1;
			}
			else 
			{
#if !defined(_AIX) && !defined(__linux) 
//modify by chenkai
				if(timecount%60==0) AllDataAccord(host_group_no[j]);
#endif
				if(mainflag[j]) backserver_process(host_group_no[j]);
				mainflag[j]=0;
			}
		}

		sleepms(10);
		timecount++;
		if(timecount>9999) timecount=0;
	}
	return NULL;
}



////////////  SG  process ////////////
// 在应用事件队列里如果有 APEVENT_STACHG（变位）事件则调用下面过程
int sgproc_stachgapent(int apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *sgprocp)
{
	ENT_STATUSCHG_STRU  *stachgmsg;
	PROTECT_MSG *protmsg;

	if(apentcode == APEVENT_PROT) {
		protmsg = (PROTECT_MSG *)apentmsg;
		if(protmsg->sgflag)((AccidentProc *)sgprocp)->InsertProtModuleEvent(protmsg->substation, protmsg->name, 1);	//staval域无效
	}
	else {
		stachgmsg=(ENT_STATUSCHG_STRU  *)apentmsg;
		if(stachgmsg->maketype>=SATATUSCHG_MAKEENT_YN)return(OK);

		if (strcmp(stachgmsg->tabname,BREAK_TABNAME)==0)	//开关量表
		{ 
				// 将该事件插入开关等待表中
		((AccidentProc *)sgprocp)->InsertBreakEvent(&stachgmsg->chgclock,stachgmsg->substation,
				stachgmsg->objectname,(uchar)stachgmsg->staval,stachgmsg->zygroupno,
				stachgmsg->pdata,stachgmsg->qdata,stachgmsg->idata,stachgmsg->testflag);
		}
		else if (strcmp(stachgmsg->tabname,PROT_TABNAME)==0)//保护量表
		{
			if (stachgmsg->staval==STATUS_VAL_ON)	// 动作为分-->合
			{
				if (stachgmsg->sgzflag)			// 如果是事故总信号，插入
					((AccidentProc *)sgprocp)->InsertSgzEvent(stachgmsg->substation, stachgmsg->objectname,
													  (uchar)stachgmsg->staval);
				else ((AccidentProc *)sgprocp)->InsertProtEvent(stachgmsg->substation, stachgmsg->objectname,
												   (uchar)stachgmsg->staval);
			}
		}
		else if (strcmp(stachgmsg->tabname,DMSSTATUS_TABNAME)==0)//配电故障检测状态表
		{
			if (stachgmsg->staval==STATUS_VAL_ON)	// 动作为分-->合
			{
				if (stachgmsg->sgzflag)			// 如果是事故总信号，插入
					((AccidentProc *)sgprocp)->InsertSgzEvent(stachgmsg->substation, stachgmsg->objectname,
													  (uchar)stachgmsg->staval);
				else ((AccidentProc *)sgprocp)->InsertProtEvent(stachgmsg->substation, stachgmsg->objectname,
												   (uchar)stachgmsg->staval);
			}
		}
	}
	return(OK);
}


// 处理事故的线程
#if defined WIN32
UINT accidentdealproc(LPVOID pParam)
#else
void * accidentdealproc(void * argp)
#endif
{
	APEvent		apevent;
    AccidentProc		accidentproc;

	// 遇 APEVENT_STACHG 事件调用 sgproc_stachgapent 过程 
	apevent.AddApEventCallback(APEVENT_STACHG,sgproc_stachgapent,&accidentproc,NULL);	//处理
	apevent.AddApEventCallback(APEVENT_PROT,sgproc_stachgapent,&accidentproc,NULL);	//新增--用于微机保护动作事件处理
	for (;;)
	{
		apevent.ApEventLoop(1);
		accidentproc.CheckBreakEvent();	
		sleepms(50);
	}

	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////
/////////////////////// 生成事项 //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
static  int  breakchg_flag,switchchg_flag;

int   yieldpowerevent(int  apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *eventp)
{
	
//	if(!IsMainServer())return OK;		//非主服务器不产生事项

	Event					*eventcl=(Event *)eventp;
    ENT_OVERLIMIT_STRU		*olmsgp;
    ENT_BREAK_STRU			*brkmsgp;
	ENT_STATUSCHG_STRU		*stamsgp;
	ENT_YK_STRU				*ykmsgp;
	ENT_YT_STRU				*ytmsgp;
    ENT_MANSET_STRU			*mansetmsgp;
	ENT_WFBS_STRU           *wfbsmsgp;
	ENT_SETSPECIALFLAG_STRU	*setflagmsgp;
	ENT_LINEREPLACE_STRU	*replacemsgp;
//	ENT_DADIAGNOSE_STRU	    *dasdiagnosep;


	switch(apentcode) 
	{
		case APEVENT_OVERLIMIT:	// 越限
			 olmsgp=(ENT_OVERLIMIT_STRU *)apentmsg;
			 eventcl->AddOverlimitEvent(olmsgp->tabname,olmsgp->objectname,
													apentclock,olmsgp->olgrade,olmsgp->olvalue);
			break;

		case APEVENT_MSTATUSCHG:	// 多态量变位
			stamsgp=(ENT_STATUSCHG_STRU *)apentmsg;
			if(IsMainServer(stamsgp->groupno))sc.FreshDataStream();
			 eventcl->AddMultiStaChgEvent(stamsgp->tabname,stamsgp->objectname,
													   &stamsgp->chgclock,stamsgp->staval);
			break;
		case APEVENT_STACHG:	// 状态量变位
			 stamsgp=(ENT_STATUSCHG_STRU *)apentmsg;

			 if(stamsgp->maketype>=SATATUSCHG_MAKEENT_YN) {
				if(IsMainServer(stamsgp->groupno))sc.FreshDataStream();
			 }
			 else 
			 {
				 if(stamsgp->maketype==SATATUSCHG_BY_SCADA) 
				 {
					if(IsMainServer(stamsgp->groupno))sc.FreshDataStream();
				 }
				 // 如果不是开关量表
				 if (strcmp(stamsgp->tabname,BREAK_TABNAME)!=0) 
				 {
					if (stamsgp->oddflag) 
						eventcl->AddStateChangeEvent(stamsgp->tabname,stamsgp->objectname,
								&stamsgp->chgclock,ADDENTTYPE_ODD,stamsgp->staval,
								stamsgp->pdata,stamsgp->qdata,stamsgp->idata,0,NULL,NULL,stamsgp->testflag);
					else 
						eventcl->AddStateChangeEvent(stamsgp->tabname,stamsgp->objectname,
								&stamsgp->chgclock,ADDENTTYPE_NORMAL,stamsgp->staval,
								stamsgp->pdata,stamsgp->qdata,stamsgp->idata,0,NULL,NULL,stamsgp->testflag);
				 }
	//			 else // 开关 同时 合->分
	//				eventcl->AddStateChangeEvent(stamsgp->tabname,stamsgp->objectname,
	//							&stamsgp->chgclock,ADDENTTYPE_NORMAL,stamsgp->staval,
	//							stamsgp->pdata,stamsgp->qdata,stamsgp->idata);
			 }

			if (strcmp(stamsgp->tabname,BREAK_TABNAME)==0) 
			{
				breakchg_flag=1;
			}
			else if (strcmp(stamsgp->tabname,SWITCH_TABNAME)==0) 
			{
				switchchg_flag=1;
			}
			break;
		case APEVENT_STASOE:	// 状态量SOE
			 stamsgp=(ENT_STATUSCHG_STRU *)apentmsg;
			 eventcl->AddStateChangeEvent(stamsgp->tabname,stamsgp->objectname,
													  &stamsgp->chgclock,ADDENTTYPE_SOE,stamsgp->staval,0.0,0.0,0.0,0,NULL,NULL,stamsgp->testflag);
			break;
		case APEVENT_BREAK:	// 开关事故
			 brkmsgp=(ENT_BREAK_STRU *)apentmsg;
			 if(brkmsgp->sgf) 
			 {
				eventcl->AddStateChangeEvent(BREAK_TABNAME,brkmsgp->breakname,
							&brkmsgp->entclock,ADDENTTYPE_SG,brkmsgp->staval,
							brkmsgp->pdata,brkmsgp->qdata,brkmsgp->idata,
							brkmsgp->relclasstype,brkmsgp->relname,brkmsgp->reldescribe,brkmsgp->testflag);
			 }
			 else 
			 {
				eventcl->AddStateChangeEvent(BREAK_TABNAME,brkmsgp->breakname,
							&brkmsgp->entclock,ADDENTTYPE_NORMAL,brkmsgp->staval,
							brkmsgp->pdata,brkmsgp->qdata,brkmsgp->idata,0,NULL,NULL,brkmsgp->testflag);
			 }
			break;

		case APEVENT_YK:		// 遥控事项
			 ykmsgp=(ENT_YK_STRU *)apentmsg;
			 if(ykmsgp->procsercode!=YK_CONFIRM) 
				 eventcl->AddYkandUpdownEvent(0,apentclock,ykmsgp->tabname,
										ykmsgp->objectname,ykmsgp->securegrade,
				 						YKACT_MSGNAME,ykmsgp->actype,YKPROC_MSGNAME,
										ykmsgp->procsercode,ykmsgp->ophostname,ykmsgp->operatorname,
										ykmsgp->entname);
			 else
				 eventcl->AddYkandUpdownEvent(0,apentclock,ykmsgp->tabname,
										ykmsgp->objectname,ykmsgp->securegrade,
										YKACT_MSGNAME,ykmsgp->actype,YKPROC_MSGNAME,
										ykmsgp->procsercode,ykmsgp->confirmhost,ykmsgp->confirmname,
										ykmsgp->entname);
			break;
		case APEVENT_YT:		// 遥控事项
			 ytmsgp=(ENT_YT_STRU *)apentmsg;
			 eventcl->AddYtEvent(apentclock,ytmsgp->objectname,ytmsgp->securegrade,
				 	ytmsgp->ytsercode,ytmsgp->failcode,ytmsgp->setvalue,ytmsgp->curvalue,
					ytmsgp->outvalue,ytmsgp->ophostname,ytmsgp->operatorname,
					ytmsgp->entname);
			break;

		case APEVENT_UPDOWN:	// 升降事项
			 ykmsgp=(ENT_YK_STRU *)apentmsg;
			 if(ykmsgp->procsercode!=YK_CONFIRM) 
				eventcl->AddYkandUpdownEvent(1,apentclock,ykmsgp->tabname,
								ykmsgp->objectname,ykmsgp->securegrade,
								UPDOWNACT_MSGNAME,ykmsgp->actype,UPDOWNPROC_MSGNAME,
								ykmsgp->procsercode,ykmsgp->ophostname,ykmsgp->operatorname,
								ykmsgp->entname);
			 else
				eventcl->AddYkandUpdownEvent(1,apentclock,ykmsgp->tabname,
								ykmsgp->objectname,ykmsgp->securegrade,
								UPDOWNACT_MSGNAME,ykmsgp->actype,UPDOWNPROC_MSGNAME,
								ykmsgp->procsercode,ykmsgp->confirmhost,ykmsgp->confirmname,
								ykmsgp->entname);
			break;


        case APEVENT_MANSET:	//人工置数
			{
			mansetmsgp=(ENT_MANSET_STRU*)apentmsg;
			eventcl->AddMansetEvent(mansetmsgp->hostname,mansetmsgp->username,
				mansetmsgp->tabname,mansetmsgp->objectname,mansetmsgp->securegrade, mansetmsgp->setflag,mansetmsgp->setvalue,
				apentclock);
			}
			break;
        case APEVENT_WFBS:	//五防闭锁
			{
			wfbsmsgp=(ENT_WFBS_STRU*)apentmsg;
			eventcl->AddWfbsEvent(wfbsmsgp->tabname,wfbsmsgp->objectname,wfbsmsgp->securegrade, wfbsmsgp->setflag, apentclock);
			}
			break;
		case APEVENT_SETSPECIALF:	//置特殊标志
			{
			char tmptablename[TABNAME_LEN];
			char tmpobjname[OBJECTNAME_LEN];

			tmptablename[0]='\0';
			setflagmsgp=(ENT_SETSPECIALFLAG_STRU*)apentmsg;
			if(setflagmsgp->cardrcd.objtype==(short)DEVTYPE_BREAKER) {
				EncodeObjName(DEVTYPE_BREAKER,"",setflagmsgp->cardrcd.objname,OBJTYPENM_BREAK,tmpobjname);
//                strcpy(tmpobjname,setflagmsgp->cardrcd.objname);
				strcpy(tmptablename,BREAK_TABNAME);
			}
			else if (setflagmsgp->cardrcd.objtype==(short)DEVTYPE_SWITCH) {
				EncodeObjName(DEVTYPE_SWITCH,"",setflagmsgp->cardrcd.objname,OBJTYPENM_SWITCH,tmpobjname);
//                strcpy(tmpobjname,setflagmsgp->cardrcd.objname);
				strcpy(tmptablename,SWITCH_TABNAME);
			}

			if(tmptablename[0]!='\0') {
				Rdb_QuickQuery spdquery(MyUserName, MyPassWord);
				int rcdno = spdquery.RdbGetRecordNo(tmptablename, tmpobjname);  // PATCHSCD01hb
				if(rcdno>=0) {
					RdbBackupRcd(MyUserName,MyPassWord, tmptablename, rcdno);
				}
			}
			eventcl->AddSetSpecialflagEvent(setflagmsgp->hostname,
					setflagmsgp->username,setflagmsgp->setflag,
					&setflagmsgp->cardrcd,apentclock);
			}
			break;
		case APEVENT_REPLACE:		//旁路替代
			{
			replacemsgp=(ENT_LINEREPLACE_STRU*)apentmsg;
			eventcl->AddBreakReplaceEvent(replacemsgp->hostname,replacemsgp->username,replacemsgp->breakname,replacemsgp->break_replaced,replacemsgp->mode,apentclock);
			}
			break;
/*
		case APEVENT_DASDIAGNOSE:	//配电诊断事项
			dasdiagnosep=(ENT_DADIAGNOSE_STRU*)apentmsg;
			eventcl->AddDmsDiagnoseEvent(dasdiagnosep->faulttype,dasdiagnosep->startbreakname,dasdiagnosep->startftuname,dasdiagnosep->endbreakname,
				dasdiagnosep->endftuname,apentclock);
			break;

		
		case APEVENT_PROT:		//保护事项
			((Protect_Event *)eventp)->add_protect_event(apentmsg);
			break;

		case APEVENT_PROT_DETECT:
			((Protect_Event *)eventp)->add_protect_detect(apentmsg);
			break;
		*/
		default:break;

	} // switch
	return(OK);
}


// 处理事项的线程
#if defined WIN32 
UINT powereventproc(void *)
#else
void *powereventproc(void *)
#endif
{
	APEvent   apevent;
	Event     event;
	unsigned int  count=0;
	int AddStateChange = 0;

    apevent.AddApEventCallback(APEVENT_ALL,yieldpowerevent,&event,NULL);
	for(;;) {
		apevent.ApEventLoop(1);
		if(breakchg_flag || switchchg_flag) {
			breakchg_flag=0;
			switchchg_flag=0;
			AutoCheckBreakReplace();
		}
		if(count%1200==0) AutoCheckBreakReplace();
		count++;
		sleepms(10);
	
		AddStateChange ++;

		if( count%1000 == 0)
			TRACE("%d",AddStateChange);

	}
	return NULL;
}


/////////  Display all apevent 
int   printallapent(int  apentcode,SYS_CLOCK *apentclock,void *apentmsg,void *argp)
{
	printf("\n apentcode ===========  %2d ",apentcode);
	fflush(stdout);
	return(OK);
}

void  *dispallapent(void *argp)
{
	APEvent   apevent;

	apevent.AddApEventCallback(APEVENT_ALL,printallapent,NULL,NULL);
	apevent.ApEventLoop();
	return(NULL);
}


///////////////////////////////////////////////////

//static intertime	fertrawproc_lasttime;
//static int			fertrawproc_decsecs;

//static int  PROC_RTUYCFLOAT=GetProgramRunPara("rtuycfloat",0);


/*inline int  SpecProcRTUYcData(uchar firstf,BASEANALOG_STRU *banap,short ycdata)
{
	float tmpfloat;

	if(firstf==1) {
		fanalog.RawDataProcessing(banap,ycdata);
	}
	else {
		switch(firstf) {
		case 2:	tmpfloat=(float)(ycdata/10.0);break;
		case 3:	tmpfloat=(float)(ycdata/100.0);break;
		case 4:	tmpfloat=(float)(ycdata/1000.0);break;
		default:	return -1;
		}
		banap->rawdata=ycdata;
		fanalog.EngDataProcessing(banap,tmpfloat);
	}
	return 0;
}*/


//将FEP服务器实时遥测数据放入analog表中
int TerminalYcDataInObjTable(int groupno,FE_REAL_DATA_NODE *data)
{	
	FE_YC  ycstr;
	ycstr = data->value.yc_value;
	int			i,j,rcdnums;
	short 		terminalno, ycno;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	BASEANALOG_STRU	*tmpanap;
	BASEANALOG_STRU	*fbaseanalogp,*ycmulbaseanalogp;	// 指向BASEANALOG_STRU结构的指针
	YC_MULTSRC_STRU *fycmultsrcp;
	TIME_VAL     ycproctime ;          //遥测时间：秒,微秒
	memset(&ycproctime,0,sizeof(TIME_VAL));
	intertime   procsec;
	if (fobjtable.RdbOpenTab(ANALOG_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	fobjtable.RdbLockTabWithRW();
	char ycname[OBJECTNAME_LEN];
	if (fanalog.GetYcName(data->rtu_no,data->order,ycname))
	{
		fbaseanalogp = (BASEANALOG_STRU	*)fobjtable.RdbSearchRcdAddrByKey(ycname);
		if(fbaseanalogp!=NULL&&!fbaseanalogp->lockfertf&&!IS_BYREPLACE_EQU(fbaseanalogp->bypassflag)&&
			!fbaseanalogp->mansetf&&!fbaseanalogp->calcultf)
		{
			fbaseanalogp->rawdata = ycstr.raw_yc;
			struct tm yctm;
			yctm.tm_year=ycstr.modifier.time.year;
			yctm.tm_mon=ycstr.modifier.time.month-1;
			yctm.tm_mday=ycstr.modifier.time.day;
			yctm.tm_hour=ycstr.modifier.time.hour;
			yctm.tm_min=ycstr.modifier.time.minute;
			yctm.tm_sec=ycstr.modifier.time.second;
			yctm.tm_isdst=0;
			procsec=(intertime)mktime(&yctm);
			ycproctime.tv_sec = procsec;
			ycproctime.tv_msec =ycstr.modifier.time.milisecond;
			fanalog.EngTimeDataProcessing(fbaseanalogp,ycstr.processed_yc,ycproctime);  
		}
	}
	/*rcdnums = fobjtable.RdbGetRcdCnt();
	for (i=0; i<rcdnums; i++) 
	{
		fbaseanalogp = (BASEANALOG_STRU	*)fobjtable.rdbgetrcdaddr(i);
		if (fbaseanalogp==NULL||fbaseanalogp->groupno != groupno||
			fbaseanalogp->terminalno != data->rtu_no||fbaseanalogp->ycno != data->order)
		{
			continue;
		}
		if(fbaseanalogp->lockfertf||IS_BYREPLACE_EQU(fbaseanalogp->bypassflag)||
			fbaseanalogp->mansetf||fbaseanalogp->calcultf)
		{
            break;
		}
		fbaseanalogp->rawdata = ycstr.raw_yc;
		struct tm yctm;
		yctm.tm_year=ycstr.modifier.time.year;
		yctm.tm_mon=ycstr.modifier.time.month;
		yctm.tm_mday=ycstr.modifier.time.day;
		yctm.tm_hour=ycstr.modifier.time.hour;
		yctm.tm_min=ycstr.modifier.time.minute;
		yctm.tm_sec=ycstr.modifier.time.second;
		yctm.tm_isdst=-1;
		procsec=(intertime)mktime(&yctm);
		ycproctime.tv_sec = procsec;
		ycproctime.tv_msec =ycstr.modifier.time.milisecond;
		fanalog.EngTimeDataProcessing(fbaseanalogp,ycstr.processed_yc,ycproctime);        
	}//for*/
	fobjtable.RdbUnlockTab();
	if (fobjtable.RdbOpenTab(YCMULTISRC_TABNAME)==RDB_OK) 
	{
		float  tmpycvalue;
		char   *tmpobjname;
		fobjtable.RdbLockTabWithRW();
		rcdnums = fobjtable.RdbGetRcdCnt();
		for (i=0; i<rcdnums; i++) 
		{
				// 根据记录号取得BASEANALOG_STRU结构的地址
			fycmultsrcp = (YC_MULTSRC_STRU	*)fobjtable.rdbgetrcdaddr(i);
			if (fycmultsrcp==NULL)	continue;
			ycmulbaseanalogp=(BASEANALOG_STRU	*)fanalog.GetAnalogAddr(fycmultsrcp->objname);
			if (ycmulbaseanalogp==NULL)	continue;
			for(j=0;j<MAX_YCSRC_NUMS;j++)
			{
				if(fycmultsrcp->ycsrcpara[j].samplemode==MULTSRC_SAMPLING_TERMINAL) 
				{
					terminalno=fycmultsrcp->ycsrcpara[j].sampgrpno;
					ycno=fycmultsrcp->ycsrcpara[j].samporderno;
					if(terminalno == data->rtu_no&&ycno == data->order)
					{
						fycmultsrcp->ycsrcdata[j].deadsecs=0;
						fycmultsrcp->ycsrcdata[j].deadf=0;
						if (fycmultsrcp->ycsrcpara[j].coef!=0)
						{
                           tmpycvalue=ycstr.processed_yc* fycmultsrcp->ycsrcpara[j].coef + fycmultsrcp->ycsrcpara[j].base;
						}
						else
						{
                           tmpycvalue=ycstr.processed_yc + fycmultsrcp->ycsrcpara[j].base;
						}
						fycmultsrcp->ycsrcdata[j].initedf=1;
						//处理非法标志
						if(ISZERO(ycmulbaseanalogp->maxinvld-ycmulbaseanalogp->mininvld))
						{
							fycmultsrcp->ycsrcdata[j].invalidf=0;
						}
						else	
						{
							if(tmpycvalue>ycmulbaseanalogp->maxinvld || tmpycvalue<ycmulbaseanalogp->mininvld) 
								fycmultsrcp->ycsrcdata[j].invalidf=1;
							else 
								fycmultsrcp->ycsrcdata[j].invalidf=0;
						}
						fycmultsrcp->ycsrcdata[j].procsecs = ycproctime.tv_sec;
						fycmultsrcp->ycsrcdata[j].procmsecs = ycproctime.tv_msec;
						fycmultsrcp->ycsrcdata[j].value=tmpycvalue;
					}

				}
				else if(fycmultsrcp->ycsrcpara[j].samplemode==MULTSRC_SAMPLING_OBJ)
				{
					tmpobjname=((char *)fycmultsrcp) + sizeof(YC_MULTSRC_STRU) + j * OBJECTNAME_LEN;
					tmpanap=(BASEANALOG_STRU*)fanalog.GetAnalogAddr(tmpobjname);
					if (tmpanap)
					{
						if (!strcmp(tmpobjname,fbaseanalogp->name))//tmpanap == fbaseanalogp)
						{
							fycmultsrcp->ycsrcdata[j].value=tmpanap->value;
							fycmultsrcp->ycsrcdata[j].deadsecs=tmpanap->deadsecs;
							fycmultsrcp->ycsrcdata[j].deadf=tmpanap->deadf;
							fycmultsrcp->ycsrcdata[j].initedf=1;
							fycmultsrcp->ycsrcdata[j].invalidf=tmpanap->invalidf;
							fycmultsrcp->ycsrcdata[j].deverror=tmpanap->neterror | tmpanap->terminalerror;
							fycmultsrcp->ycsrcdata[j].procsecs = tmpanap->procsecs;
							fycmultsrcp->ycsrcdata[j].procmsecs = tmpanap->procmsecs;
						}
					}
					else 
					{
                          fycmultsrcp->ycsrcdata[j].initedf=0;
					}
				}
			}
		}//for
		fobjtable.RdbUnlockTab();
	}
    return(OK);
}


/*
// 处理FEP服务器生数据 
int TerminalYcDataInObjTable(int groupno)
{
	//static unsigned int	ycproc_timecount=0;
	int			i,rcdnums;//changprocflag=0;
	short		terminalno, sortterminalno, ycno;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	BASEANALOG_STRU	*tmpanap;
	BASEANALOG_STRU	*fbaseanalogp, *sysPlp;	// 指向BASEANALOG_STRU结构的指针
	YC_MULTSRC_STRU *fycmultsrcp;
	short		*fycdatap,*ffycdatap;		// 指向遥测量数据区的指针
	uchar       *firstf;
	float		floatrawdata;
    TIME_VAL     ycproctime ;          //遥测时间：秒,微秒 
//printf("\n\####TerminalYcDataInObjTable####\n");

	//if(ycproc_timecount%4==0)changprocflag=0;
	//else changprocflag=1;

	//ycproc_timecount++;

	if (fobjtable.RdbOpenTab(ANALOG_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	fobjtable.RdbLockTabWithRW();
	rcdnums = fobjtable.RdbGetRcdCnt();
    
	NETDATA *netdatapara= compara.netdata[groupno];  //add


	for (i=0; i<rcdnums; i++) 
	{
		// 根据记录号取得BASEANALOG_STRU结构的地址
		fbaseanalogp = (BASEANALOG_STRU	*)fobjtable.rdbgetrcdaddr(i);
		if (fbaseanalogp==NULL)	continue;
		if (fbaseanalogp->groupno != groupno) continue;
//			if(fbaseanalogp->samplemode)continue;
//			if (fbaseanalogp->calcultf || fbaseanalogp->multycf)continue;
		if (fbaseanalogp->calcultf)
		{
			fanalog.StatisticsCalVal(fbaseanalogp);//2004 05 add 
			continue;
		}
		terminalno=fbaseanalogp->terminalno;
		if(terminalno<0 || terminalno>=SCADA_MAX_TERMINALNUMS)continue;
		
		// 取得terminalno的遥测数据起始地址
		//fycdatap=terminalycaddr[terminalno];
		//firstf=(unsigned char *)terminalycfirstaddr[terminalno];
		fycdatap = (short*)(netdatapara->yc+netdatapara->stastartyc[terminalno]*2);
		firstf = (uchar*)(netdatapara->first.ycfirst+netdatapara->stastartyc[terminalno]);
		if(fycdatap==NULL || firstf==NULL)continue;

		/*for(int j=0; j<netdatapara->rtunum; j++)
		{
			if(netdatapara->rtuno[j]==terminalno){
				sortterminalno=j;
				break;
			}
		}*/
	//	if((ycno=fbaseanalogp->ycno)>=netdatapara->ycnum[rtuno[terminalno]])continue;
	//	ffycdatap = (short*)(compara.netdata[groupno]->yc);	// 指向遥测量数据区的首指针
	//	memset(&ycproctime,0,sizeof(TIME_VAL));
	//	ycproctime.tv_sec = netdatapara->yc_time[fycdatap-ffycdatap+ycno*2].tv_sec;
    //    ycproctime.tv_msec = netdatapara->yc_time[fycdatap-ffycdatap+ycno*2].tv_msec;
		//memcpy(&ycproctime,&netdatapara->yc_time[fycdatap-ffycdatap+ycno*2],sizeof(TIME_VAL));
        //ycproctime = netdatapara->yc_time[fycdatap-ffycdatap+ycno*2];
			
	//	if(firstf[ycno])
	//	{
			//if(PROC_RTUYCFLOAT!=1) 
			//{
	//		      fbaseanalogp->rawdata = *((fycdatap + ycno*2));	
	//		      floatrawdata = *((float*)(fycdatap + ycno*2));
				//if(changprocflag==0)
				//{
					//fanalog.RawDataProcessing(fbaseanalogp,fycdatap[ycno]);
					
		//			fanalog.FloatRawDataProcessing(fbaseanalogp,floatrawdata,ycproctime);
				//}
				//else 
				//{
					/*if(fycdatap[ycno]!=fbaseanalogp->rawdata)
						fanalog.RawDataProcessing(fbaseanalogp,fycdatap[ycno]);
					*/
					//if( floatrawdata != fbaseanalogp->value )
				//		fanalog.FloatRawDataProcessing(fbaseanalogp,floatrawdata,ycproctime);
				//}
			//}
			//else {
			//	if(changprocflag==0)
			//		SpecProcRTUYcData(firstf[ycno],fbaseanalogp,fycdatap[ycno],ycproctime);
			//	else {
			//		if(fycdatap[ycno]!=fbaseanalogp->rawdata) 
			//			SpecProcRTUYcData(firstf[ycno],fbaseanalogp,fycdatap[ycno],ycproctime);
			//	}
			//}
	//	}
	//}//for

	//fobjtable.RdbUnlockTab();
	//	if(changprocflag==0) {
		//if (fobjtable.RdbOpenTab(YCMULTISRC_TABNAME)==RDB_OK) 
		//{
		//	float  tmpycvalue;
		//	char   *tmpobjname;

		//	fobjtable.RdbLockTabWithRW();
		//	rcdnums = fobjtable.RdbGetRcdCnt();
		//	for (i=0; i<rcdnums; i++) 
		//	{
				// 根据记录号取得BASEANALOG_STRU结构的地址
		//		fycmultsrcp = (YC_MULTSRC_STRU	*)fobjtable.rdbgetrcdaddr(i);
		//		if (fycmultsrcp==NULL)	continue;
		//		fbaseanalogp=(BASEANALOG_STRU	*)fanalog.GetAnalogAddr(fycmultsrcp->objname);
		//		if (fbaseanalogp==NULL)	continue;
		//		for(int j=0;j<MAX_YCSRC_NUMS;j++)
		//		{
		//			if(fycmultsrcp->ycsrcpara[j].samplemode==MULTSRC_SAMPLING_TERMINAL) 
		//			{
		//				terminalno=fycmultsrcp->ycsrcpara[j].sampgrpno;
		//				if(terminalno<0 || terminalno>=SCADA_MAX_TERMINALNUMS)continue;
						// 取得terminalno的遥测数据起始地址
						//fycdatap=terminalycaddr[terminalno];firstf=terminalycfirstaddr[terminalno];
		//				fycdatap =(short*)(netdatapara->yc+netdatapara->stastartyc[terminalno]*2);;
		//			    firstf = (uchar*)(netdatapara->first.ycfirst[netdatapara->stastartyc[terminalno]]);
		//				if(fycdatap==NULL || firstf==NULL)continue;

						/*for(int k=0; k<netdatapara->rtunum; k++){
							if(netdatapara->rtuno[k]==terminalno){
								sortterminalno=k;
								break;
							}
						}*/

		//				if((ycno=fycmultsrcp->ycsrcpara[j].samporderno)>=netdatapara->ycnum[rtuno[terminalno]])continue;
		//				ffycdatap = (short*)(compara.netdata[groupno]->yc);	// 指向遥测量数据区的首指针
		//				ycproctime = netdatapara->yc_time[fycdatap-ffycdatap+ycno*2];
                       
		//				if(firstf[ycno])
		//				{
		//					floatrawdata = *((float*)(fycdatap + ycno*2));
							//tmpycvalue=fycdatap[ycno] * fycmultsrcp->ycsrcpara[j].coef + fycmultsrcp->ycsrcpara[j].base;
		//					tmpycvalue=floatrawdata * fycmultsrcp->ycsrcpara[j].coef + fycmultsrcp->ycsrcpara[j].base;
							//处理非法标志
		//					if(ISZERO(fbaseanalogp->maxinvld-fbaseanalogp->mininvld)) 
		//					{
		//						fycmultsrcp->ycsrcdata[j].invalidf=0;
		//					}
		//					else	
		//					{
		//						if(tmpycvalue>fbaseanalogp->maxinvld || tmpycvalue<fbaseanalogp->mininvld) 
		//							fycmultsrcp->ycsrcdata[j].invalidf=1;
		//						else 
		//							fycmultsrcp->ycsrcdata[j].invalidf=0;
		//					}
        //                    fycmultsrcp->ycsrcdata[j].initedf=1;
		//					if (ycproctime.tv_sec != fycmultsrcp->ycsrcdata[j].procsecs||ycproctime.tv_msec != fycmultsrcp->ycsrcdata[j].procmsecs )
		//					{
		//						fycmultsrcp->ycsrcdata[j].deadsecs=0;
		//						fycmultsrcp->ycsrcdata[j].deadf=0;
		//						continue;
		//					}
		//					fycmultsrcp->ycsrcdata[j].procsecs = ycproctime.tv_sec;
        //                    fycmultsrcp->ycsrcdata[j].procmsecs = ycproctime.tv_msec;
							// 处理死数标志
							//if(tmpycvalue!=fycmultsrcp->ycsrcdata[j].value)
							//{
								//fycmultsrcp->ycsrcdata[j].deadsecs=0;
								//fycmultsrcp->ycsrcdata[j].deadf=0;
							//}

		//					fycmultsrcp->ycsrcdata[j].value=tmpycvalue;
							//fycmultsrcp->ycsrcdata[j].initedf=1;
							
			//			}
		//			}
		//			else if(fycmultsrcp->ycsrcpara[j].samplemode==MULTSRC_SAMPLING_OBJ) 
		//			{
		//				tmpobjname=((char *)fycmultsrcp) + sizeof(YC_MULTSRC_STRU) + j * OBJECTNAME_LEN;
		//				tmpanap=(BASEANALOG_STRU	*)fanalog.GetAnalogAddr(tmpobjname);
		//				if (tmpanap)
		//				{
		//					fycmultsrcp->ycsrcdata[j].deadsecs=tmpanap->deadsecs;
		//					fycmultsrcp->ycsrcdata[j].deadf=tmpanap->deadf;
		//					fycmultsrcp->ycsrcdata[j].invalidf=tmpanap->invalidf;
		//					fycmultsrcp->ycsrcdata[j].deverror=tmpanap->neterror | tmpanap->terminalerror;
		//					fycmultsrcp->ycsrcdata[j].initedf=1;
		//					if (tmpanap->procsecs== fycmultsrcp->ycsrcdata[j].procsecs && tmpanap->procmsecs == fycmultsrcp->ycsrcdata[j].procmsecs )
		//					{
		//						continue;
		//					}
		//					fycmultsrcp->ycsrcdata[j].procsecs = tmpanap->procsecs;
		//					fycmultsrcp->ycsrcdata[j].procmsecs = tmpanap->procmsecs;
		//					fycmultsrcp->ycsrcdata[j].value=tmpanap->value;
							//fycmultsrcp->ycsrcdata[j].deadsecs=tmpanap->deadsecs;
							//fycmultsrcp->ycsrcdata[j].deadf=tmpanap->deadf;
							//fycmultsrcp->ycsrcdata[j].initedf=1;
							//fycmultsrcp->ycsrcdata[j].invalidf=tmpanap->invalidf;
							//fycmultsrcp->ycsrcdata[j].deverror=tmpanap->neterror | tmpanap->terminalerror;

		//				}
		//				else fycmultsrcp->ycsrcdata[j].initedf=0;
		//			}
		//		}
		//	}//for
//			fobjtable.RdbUnlockTab();
		//}
//	}

	//计算周波
		
	/*int frqrcdno = fobjtable.RdbSearchRcdByKey((void*)SYS_HZ_NAME);
	if(frqrcdno<0)return SCADA_ERR_NOOBJECT;
	sysPlp=(BASEANALOG_STRU*)fobjtable.rdbgetrcdaddr(frqrcdno);
	if(sysPlp==NULL)return(SCADA_ERR_NOOBJECT);
	if(netdatapara->cycle>=0 && (!sysPlp->calcultf))
	{
		if(frqrcdno>=0)	
		{
			ycproctime = netdatapara->cycle_time;
			fanalog.PutRawDataToYcRcdNo(frqrcdno,netdatapara->cycle,ycproctime);
		}
	}

    return(OK);*/
//}*/

// 将FEP服务器实时遥信数据放入实时库相应表中 
int TerminalYxDataInObjTab(int groupno,uchar tabletype,FE_REAL_DATA_NODE * data,int proc_slowyxf)
{
	FE_YX  yxstr;
	yxstr = data->value.yx_value;
	int i;
	//int			i, rcdnums;
	short	    yxno,yxno1;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	BASESTATUS_STRU	*fbasestatusp;	// 指向BASESTATUS_STRU结构的指针       
	SYS_CLOCK yxclock;
	intertime   procsec;
	switch(tabletype)
	{
		case Status_Table:
			if (fobjtable.RdbOpenTab(STATUS_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case DmsStatus_Table:
			if (fobjtable.RdbOpenTab(DMSSTATUS_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case Breaker_Table:
			if (fobjtable.RdbOpenTab(BREAK_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case Switch_Table:
			if (fobjtable.RdbOpenTab(SWITCH_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case ProtSig_Table:
			if (fobjtable.RdbOpenTab(PROT_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
	}

	fobjtable.RdbLockTabWithRW();
	//rcdnums = fobjtable.RdbGetRcdCnt();
	char yxname[OBJECTNAME_LEN];
	if (fstatus.GetYxName(data->rtu_no,data->order,yxname))
	{
		//i = fobjtable.RdbSearchRcdByKey(yxname);
		//fbasestatusp=(BASESTATUS_STRU*)fobjtable.RdbGetRecordAddr(i);
		fbasestatusp=(BASESTATUS_STRU*)fobjtable.RdbSearchRcdAddrByKey(yxname);
		if (fbasestatusp==NULL)
		{
			return 0;
		}
		if (!fbasestatusp->calcultf&&!fbasestatusp->lockfertf&&!fbasestatusp->mansetf)
		{
			yxno= data->order;
			struct tm yxtm;
			yxtm.tm_year=yxstr.modifier.time.year;
			yxtm.tm_mon=yxstr.modifier.time.month;
			yxtm.tm_mday=yxstr.modifier.time.day;
			yxtm.tm_hour=yxstr.modifier.time.hour;
			yxtm.tm_min=yxstr.modifier.time.minute;
			yxtm.tm_sec=yxstr.modifier.time.second;
			yxtm.tm_isdst=-1;
			procsec=(intertime)mktime(&yxtm);
			fbasestatusp->procsecs = procsec;
			fbasestatusp->procmsecs = yxstr.modifier.time.milisecond;
			if (yxstr.processed_yx == 2)
			{
				fbasestatusp->invalidf =1;
			}
			else
			{
				switch(tabletype) 
			   {
				case Status_Table:
					fstatus.RawStatusdataProcessing(fbasestatusp,yxno,yxstr.processed_yx,proc_slowyxf);
					break;
				case DmsStatus_Table:
					fdmsstatus.RawStatusdataProcessing(fbasestatusp,yxno,yxstr.processed_yx,proc_slowyxf);
					break;
				case Breaker_Table:
					fbreak.RawStatusdataProcessing(fbasestatusp,yxno,yxstr.processed_yx,proc_slowyxf);
					break;
				case Switch_Table:
					fswitch.RawStatusdataProcessing(fbasestatusp,yxno,yxstr.processed_yx,proc_slowyxf);
					break;
				case ProtSig_Table:
					fprotsig.RawStatusdataProcessing(fbasestatusp,yxno,yxstr.processed_yx,proc_slowyxf);
					break;
				}
				if (fbasestatusp->changesavef)
				{
					strcpy(fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].name,fbasestatusp->name);
					strcpy(fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].desc,fbasestatusp->describe);
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].value = yxstr.processed_yx;
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].procsecs = fbasestatusp->procsecs;
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].procmsecs = fbasestatusp->procmsecs;
					fstatus.yxdatap->yxwritep = (fstatus.yxdatap->yxwritep+1)%MAX_CHGDATANUM;
					fstatus.yxdatap->yxTrendDBwp = fstatus.yxdatap->yxwritep;
				}
			}
		}
	}
	if (fstatus.GetSubYxName(data->rtu_no,data->order,yxname))
	{
		fbasestatusp=(BASESTATUS_STRU*)fobjtable.RdbSearchRcdAddrByKey(yxname);
		if (fbasestatusp==NULL)
		{
			return 0;
		}
		if (!fbasestatusp->calcultf&&!fbasestatusp->lockfertf&&fbasestatusp->twinyxflag)
		{
			yxno1 = data->order;
			if (yxstr.processed_yx == 2)
			{
				fbasestatusp->invalidf =1;
			}
			else
			{
				switch(tabletype) 
				{
				case Status_Table:
					fstatus.RawStatusdataProcessing(fbasestatusp,yxno1,yxstr.processed_yx,proc_slowyxf);
					break;
				case DmsStatus_Table:
					fdmsstatus.RawStatusdataProcessing(fbasestatusp,yxno1,yxstr.processed_yx,proc_slowyxf);
					break;
				case Breaker_Table:
					fbreak.RawStatusdataProcessing(fbasestatusp,yxno1,yxstr.processed_yx,proc_slowyxf);
					break;
				case Switch_Table:
					fswitch.RawStatusdataProcessing(fbasestatusp,yxno1,yxstr.processed_yx,proc_slowyxf);
					break;
				case ProtSig_Table:
					fprotsig.RawStatusdataProcessing(fbasestatusp,yxno1,yxstr.processed_yx,proc_slowyxf);
					break;
				}

			}
		}
	}
	fobjtable.RdbUnlockTab();
	return(OK);
}



/*int TerminalYxDataInObjTable(int groupno, int start_proc_slowyxf)
{
	TerminalYxDataInObjTab(groupno,ProtSig_Table,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Status_Table,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,DmsStatus_Table,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Breaker_Table,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Switch_Table,start_proc_slowyxf);
//	start_proc_slowyxf=1;
	return(OK);
}*/

//将FEP服务器实时遥信数据放入实时库相应表中 
int TerminalYxDataInObjTable(int groupno, FE_REAL_DATA_NODE* data, int start_proc_slowyxf)
{
	TerminalYxDataInObjTab(groupno,ProtSig_Table,data,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Status_Table,data,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,DmsStatus_Table,data,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Breaker_Table,data,start_proc_slowyxf);
	TerminalYxDataInObjTab(groupno,Switch_Table,data,start_proc_slowyxf);
	return(OK);
}



int TerminalKwhDataInObjTable(int groupno,FE_REAL_DATA_NODE* data)
{
	FE_KWH  kwhstr;
	kwhstr = data->value.kwh_value;
	//int		i, rcdnums;
	short	terminalno, kwhno;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	KWH_STRU		*fkwhp;		// 指向KWH_STRU结构的指针
	if (fobjtable.RdbOpenTab(KWH_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	fobjtable.RdbLockTabWithRW();
	char kwhname[OBJECTNAME_LEN];
	if (fkwh.GetKwhName(data->rtu_no,data->order,kwhname))
	{
		fkwhp = (KWH_STRU*)fobjtable.RdbSearchRcdAddrByKey(kwhname);
		if(fkwhp!=NULL&&!fkwhp->lockfertf&&!fkwhp->mansetf&&!fkwhp->calcultf)
		{
              fkwh.RawKwhProcessing(fkwhp,kwhstr.raw_kwh);
		}
	}
	/*rcdnums = fobjtable.RdbGetRcdCnt();
	for (i=0; i<rcdnums; i++) 
	{
		// 根据记录号取得KWH_STRU结构的地址
		fkwhp = (KWH_STRU*)fobjtable.rdbgetrcdaddr(i);
    	if (fkwhp==NULL||fkwhp->groupno != groupno||fkwhp->terminalno!= data->rtu_no||fkwhp->kwhno!=data->order)
		{
			continue;
		}
		if (fkwhp->calcultf)
		{
			break;
		}
		fkwh.RawKwhProcessing(fkwhp,kwhstr.raw_kwh);

	}//for*/

	fobjtable.RdbUnlockTab();
	
	return(OK);
}


//zxm 20140108 del
/*
int TerminalKwhDataInObjTable(int groupno)
{
	static int prochourkwhflag=0;
	int		i, rcdnums;
	short	terminalno, sortterminalno, kwhno;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	KWH_STRU		*fkwhp;		// 指向KWH_STRU结构的指针
	int		        *fkwhdatap;	// 指向电度量数据区的指针
	uchar           *firstf;

	SYS_CLOCK	     curclock;


	GetCurrentClock(&curclock);
	if(curclock.minute==59 && curclock.second>30) {
		prochourkwhflag++;
	}
	else prochourkwhflag=0;


	if (fobjtable.RdbOpenTab(KWH_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	fobjtable.RdbLockTabWithRW();
	rcdnums = fobjtable.RdbGetRcdCnt();

	NETDATA *netdatapara= compara.netdata[groupno];  //add



	for (i=0; i<rcdnums; i++) 
	{
		// 根据记录号取得KWH_STRU结构的地址
		fkwhp = (KWH_STRU*)fobjtable.rdbgetrcdaddr(i);
    	if (fkwhp==NULL)continue;
		if (fkwhp->groupno != groupno) continue;
//		if(fkwhp->samplemode)continue;

		if (fkwhp->calcultf)continue;
		terminalno=fkwhp->terminalno;
		if(terminalno<0 || terminalno>=SCADA_MAX_TERMINALNUMS)continue;
		
		//fkwhdatap=terminalkwhaddr[terminalno];firstf=terminalkwhfirstaddr[terminalno];
		fkwhdatap = (int*)compara.netdata[groupno]->kwh+netdatapara->stastartkwh[terminalno];
		firstf = (uchar*)(netdatapara->first.kwhfirst+netdatapara->stastartkwh[terminalno]/4);
		if(fkwhdatap==NULL || firstf==NULL)continue;

		/*for(int j=0; j<netdatapara->rtunum; j++){
			if(netdatapara->rtuno[j]==terminalno){
				sortterminalno=j;
				break;
			}
		}*/
		/*int tempkwhrawdata=0;
		if((kwhno=fkwhp->kwhno)>=netdatapara->kwhnum[rtuno[terminalno]])continue;
		if(firstf[kwhno]) 
		{
			tempkwhrawdata=fkwhdatap[kwhno];
			if(fkwhp->sampletype==KWHSAMPLING_HOURCLN) {
#if( ( defined(__sun) && defined(__sparc) ) || defined(_AIX) || defined(__hpux))
				scdhostbyte(RDB_DATATYPE_INT,&tempkwhrawdata);
#endif
				if(prochourkwhflag==1)fkwh.ProcRawData_HourClean(fkwhp, tempkwhrawdata);
			}
			else{ 
//				dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"转换前:\tfkwhdatap[%d]=%d\n",kwhno,tempkwhrawdata);
#if( ( defined(__sun) && defined(__sparc) ) || defined(_AIX) || defined(__hpux))
				scdhostbyte(RDB_DATATYPE_INT,&tempkwhrawdata);
#endif
//				dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"转换后:\tfkwhdatap[%d]=%d\n",kwhno,tempkwhrawdata);
				fkwh.RawKwhProcessing(fkwhp, tempkwhrawdata);
			}
		}

	}//for

	fobjtable.RdbUnlockTab();
	
	return(OK);*/
//}*/


/*
int TerminalDevDataInObjTable(int groupno)
{
	int		i, rcdnums;
	short	terminalno, sortterminalno, devorderno;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	MULTISTATUS_STRU		*fmulstap;		// 指向 MULTISTATUS_STRU结构 的指针
	uchar		*fdevdatap;		// 指向 设备量数据区的指针
	uchar           *firstf;



	if (fobjtable.RdbOpenTab(MULTISTATUS_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	fobjtable.RdbLockTabWithRW();
	rcdnums = fobjtable.RdbGetRcdCnt();

	NETDATA *netdatapara= compara.netdata[groupno];  //add
/*		for(i=0;i<SCADA_MAX_TERMINALNUMS;i++)
	{
		terminaldevaddr[i]=NULL;terminaldevfirstaddr[i]=NULL;
		getTerminalDevaddr(i, &terminaldevaddr[i],&terminaldevfirstaddr[i],netdatapara,groupno);
	}*/
	/*if (rcdnums>0)
	{
		for(i=0;i<SCADA_MAX_TERMINALNUMS;i++)
		{
			terminaldevaddr[i]=NULL;terminaldevfirstaddr[i]=NULL;
			getTerminalDevaddr(i, &terminaldevaddr[i],&terminaldevfirstaddr[i],netdatapara);
		}
	}*/

//	for (i=0; i<rcdnums; i++) 
//	{
		// 根据记录号取得MULTISTATUS_STRU结构的地址
//		fmulstap = (MULTISTATUS_STRU *)fobjtable.rdbgetrcdaddr(i);
//		if (fmulstap==NULL)continue;
//		if (fmulstap->groupno != groupno) continue;
//		if (fmulstap->calcultf)continue;
//		terminalno=fmulstap->terminalno;
//		if(terminalno<0 || terminalno>=SCADA_MAX_TERMINALNUMS)continue;
		
//		fdevdatap=terminaldevaddr[terminalno];firstf=terminaldevfirstaddr[terminalno];
//		if(fdevdatap==NULL || firstf==NULL)continue;

		/*for(int j=0; j<netdatapara->rtunum; j++){
			if(netdatapara->rtuno[j]==terminalno){
				sortterminalno=j;
				break;
			}
		}*/
		
//		if((devorderno=fmulstap->orderno)>=netdatapara->devnum[rtuno[terminalno]])continue;
//		if(firstf[devorderno]) fmultsta.ProcRawData(fmulstap, fdevdatap[devorderno]);
//	}//for

//	fobjtable.RdbUnlockTab();
	
//	return(OK);
//}*/

//////////////////////////////////////

static int MAKE_RTU_EVENT=GetProgramRunPara("maketerminalent",0);

static int  init_terminalchannelstatus_flag=0;

void SetTerminalErrorFlag(int terminalno,int oldstatus,int newstatus)
{
	//if(init_terminalchannelstatus_flag) {
		if(oldstatus==newstatus)return;
	//	if(oldstatus!=TERMINALSTATUS_STOP && newstatus!=TERMINALSTATUS_STOP)return;
	//}
	
	int i,ret,terminalerrflag=0;

	if(newstatus==TERMINALSTATUS_STOP)terminalerrflag=1;
	else terminalerrflag=0;

	Rdb_Tab  rdbtable(MyUserName,MyPassWord);

	ret=rdbtable.RdbOpenTab(ANALOG_TABNAME);
	if(ret==RDB_OK) {
		BASEANALOG_STRU *banap;
		for(banap=(BASEANALOG_STRU *)rdbtable.rdbgetfirstrcdaddr();banap!=NULL;
		   banap=(BASEANALOG_STRU *)rdbtable.rdbgetnextrcdaddr())  {
			if((!banap->calcultf) && banap->terminalno==terminalno)
				banap->terminalerror=terminalerrflag;
		}
	}
	for(i=0;i<YX_RDBTABLENUMS;i++) {
		BASESTATUS_STRU *bstap;
		ret=rdbtable.RdbOpenTab(yx_rdbtablename[i]);
		if(ret==RDB_OK) {
			for(bstap=(BASESTATUS_STRU *)rdbtable.rdbgetfirstrcdaddr();bstap!=NULL;
			   bstap=(BASESTATUS_STRU *)rdbtable.rdbgetnextrcdaddr())  {
				if((!bstap->calcultf) && bstap->terminalno==terminalno)
					bstap->terminalerror=terminalerrflag;
			}
		}
	}

	ret=rdbtable.RdbOpenTab(KWH_TABNAME);
	if(ret==RDB_OK) {
		KWH_STRU *kwhp;
		for(kwhp=(KWH_STRU *)rdbtable.rdbgetfirstrcdaddr();kwhp!=NULL;
		   kwhp=(KWH_STRU *)rdbtable.rdbgetnextrcdaddr())  {
			if((!kwhp->calcultf) && kwhp->terminalno==terminalno)
				kwhp->terminalerror=terminalerrflag;
		}
	}

	ret=rdbtable.RdbOpenTab(YCMULTISRC_TABNAME);
	if(ret==RDB_OK) {
		YC_MULTSRC_STRU *ycmultsrcp;
		for(ycmultsrcp=(YC_MULTSRC_STRU *)rdbtable.rdbgetfirstrcdaddr();ycmultsrcp!=NULL;
			ycmultsrcp=(YC_MULTSRC_STRU *)rdbtable.rdbgetnextrcdaddr())  {
			for(i=0;i<MAX_YCSRC_NUMS;i++) {
				if(ycmultsrcp->ycsrcpara[i].samplemode==MULTSRC_SAMPLING_TERMINAL) {
					if(ycmultsrcp->ycsrcpara[i].sampgrpno==terminalno)ycmultsrcp->ycsrcdata[i].deverror=terminalerrflag;
				}
			}
		}
	}
}



//检查通道主备状态
void CheckChannelStatus()
{
	int i,rcdnums;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	Rdb_Tab	terfobjtable(MyUserName, MyPassWord);
	CHANNEL_STRU		    *pchannel, *tmppchannel;
	TERMINAL_STRU			*terminalstationp;
	uchar                    curchstatus;
	Event                    eventcl;
	if(fobjtable.RdbOpenTab(CHANNEL_TABNAME)!=RDB_OK) return;
	if(terfobjtable.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK) return;
	rcdnums = fobjtable.RdbGetRcdCnt();
	for (i=0; i<rcdnums; i++) 
	{
		pchannel = (CHANNEL_STRU*)fobjtable.rdbgetrcdaddr(i);
		if (pchannel!=NULL && pchannel->useflag)
		{
            scadachanpara.terminaldata[pchannel->rtuno].rtu_no = pchannel->rtuno;
			strcpy(scadachanpara.terminaldata[pchannel->rtuno].chname,pchannel->name);
			if (scadachanpara.terminaldata[pchannel->rtuno].grouppriority>=0)
			{
				//int mainchannelno = scadachanpara.terminaldata[pchannel->rtuno].channel_no;
				tmppchannel = (CHANNEL_STRU*)fobjtable.RdbSearchRcdAddrByKey(scadachanpara.terminaldata[pchannel->rtuno].chname);
				if (tmppchannel!=NULL&&pchannel->useflag)
				{
					if (tmppchannel->status != CHANRUN)
					{
                         scadachanpara.terminaldata[pchannel->rtuno].grouppriority=-1;
                         tmppchannel->mainbak =0;
					}			       
				}
			}
			if (scadachanpara.terminaldata[pchannel->rtuno].grouppriority<0)
			{ 
				if (pchannel->status == CHANRUN)
			    {
				    scadachanpara.terminaldata[pchannel->rtuno].channel_no = pchannel->channelno;
					strcpy(scadachanpara.terminaldata[pchannel->rtuno].chname,pchannel->name);
				    scadachanpara.terminaldata[pchannel->rtuno].grouppriority = pchannel->grouppriority;
			    }
			}
			else
			{	
				if (pchannel->status == CHANRUN)
			    {
				    if (pchannel->grouppriority<scadachanpara.terminaldata[pchannel->rtuno].grouppriority)
					{
						scadachanpara.terminaldata[pchannel->rtuno].channel_no = pchannel->channelno;
							strcpy(scadachanpara.terminaldata[pchannel->rtuno].chname,pchannel->name);
						scadachanpara.terminaldata[pchannel->rtuno].grouppriority = pchannel->grouppriority;
				    }
				}
			}   
		}
	}
	for (i=0;i<MAX_RTU_NUM;i++)
	{
		if (scadachanpara.terminaldata[i].grouppriority>=0)
		{
			pchannel = (CHANNEL_STRU*)fobjtable.RdbSearchRcdAddrByKey(scadachanpara.terminaldata[i].chname);
            pchannel->mainbak = 1;
			terminalstationp =  (TERMINAL_STRU*)terfobjtable.RdbSearchRcdAddrByKey(&(scadachanpara.terminaldata[i].rtu_no));
			if (terminalstationp!=NULL&&terminalstationp->useflag)
			{
				curchstatus = TERMINALSTATUS_MAINCHAN;
				terminalstationp->runchanno = pchannel->channelno;
				if (terminalstationp->status!= curchstatus)
				{
                    eventcl.AddTerminalEvent(terminalstationp->name,terminalstationp->describe,curchstatus,terminalstationp->runchanno);
                    SetTerminalErrorFlag(terminalstationp->terminalno,terminalstationp->status,curchstatus);
                    terminalstationp->status=curchstatus;
				}
			}
		   if (scadachanpara.terminaldata[i].channel_no != scadachanpara.terminaldata[i].last_channel_no)
		   {
			  //tmppchannel = (CHANNEL_STRU*)fobjtable.RdbSearchRcdAddrByKey(&(scadachanpara.terminaldata[i].last_channel_no));
			  if (scadachanpara.terminaldata[i].last_channel_no != -1)
			  {
				  eventcl.AddChannelEvent(pchannel->name,pchannel->describe,pchannel->channelno,pchannel->mainbak);
				  tmppchannel = (CHANNEL_STRU*)fobjtable.RdbSearchRcdAddrByKey(scadachanpara.terminaldata[i].chname);
				  if (tmppchannel!=NULL&&tmppchannel->useflag)
				  {
					  if (tmppchannel->mainbak)
					  {
						  tmppchannel->mainbak =0;
					  }
					  eventcl.AddChannelEvent(tmppchannel->name,tmppchannel->describe,tmppchannel->channelno,tmppchannel->mainbak);
				  }
				 // SwitchChannelEvent(pchannel->channelno, pchannel->mainbak);
				  //SwitchChannelEvent(pchannel->channelno, pchannel->mainbak);
			  }
		   }
		   //scadachanpara.terminaldata[i].last_channel_no = pchannel->channelno;
		}
		else
		{
			terminalstationp =  (TERMINAL_STRU*)terfobjtable.RdbSearchRcdAddrByKey(&(scadachanpara.terminaldata[i].rtu_no));
			if (terminalstationp!=NULL)
			{
			    if (terminalstationp->useflag)
			    {
					curchstatus = TERMINALSTATUS_STOP;
					if (terminalstationp->status!= curchstatus)
					{
					    SetTerminalErrorFlag(terminalstationp->terminalno,terminalstationp->status,curchstatus);
	                    terminalstationp->status=curchstatus;
						eventcl.AddTerminalEvent(terminalstationp->name,terminalstationp->describe,curchstatus,terminalstationp->runchanno);
                         terminalstationp->breaktimes++;
					}
	                terminalstationp->runchanno =-1;
			    }
				else
				{
                   terminalstationp->runchanno =-1;
				}
			}
		}
	}
}

//static int MAKE_DEADDATA_EVENT=GetProgramRunPara("makedeadevent",0);//2004 05 add
int TerminalChannelDataInObjTable(int groupno,FE_CHANNEL_STAT_MAIL* data,char* hostname)
{
	//int		i, rcdnums;
	//unsigned short	channo;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	Rdb_Tab	cfobjtable(MyUserName, MyPassWord);
	CHANNEL_STRU		    *pchannel;
	//TERMINAL_STRU			*terminalstationp;
	//TDSTATUS		        *ptdstatus,*pothertdstatus,*plaststatus;
	//uchar                    curchstatus;
	//Event                    eventcl;
	if(fobjtable.RdbOpenTab(CHANNEL_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	if(cfobjtable.RdbOpenTab(CHANNELNOSORT_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
	//rcdnums = fobjtable.RdbGetRcdCnt();

    
	CHANNELSORT_STRU	*channelsort;

    memset(&channelsort,0,sizeof(CHANNELSORT_STRU));
	channelsort = (CHANNELSORT_STRU *)cfobjtable.RdbSearchRcdAddrByKey(&(data->channel_no));
	if (channelsort==NULL) return(SCADA_ERR_NOOBJECT);

	pchannel =  (CHANNEL_STRU*)fobjtable.RdbSearchRcdAddrByKey(channelsort->objname);
	if(pchannel!=NULL&&pchannel->useflag)
	{
		if (scadachanpara.terminaldata[pchannel->rtuno].grouppriority>=0)
		{
			scadachanpara.terminaldata[pchannel->rtuno].last_channel_no =scadachanpara.terminaldata[pchannel->rtuno].channel_no;
		}
		chan.ProcChanData(pchannel,data->bit_err_rate,data->stat);
		strcpy(pchannel->hostname,hostname);
		pchannel->lastsynctime = data->last_sync_time;
		pchannel->recvcount += data->recv_count;
		pchannel->sendcount+=data->send_count;
		pchannel->synctimesuccess = data->sync_time_success;
	}
	return(OK);
}


///////////////////////////////////////////////////////
static int NO_CLEAR_CHANGEF=GetProgramRunPara("noclearchangf",0);//2004 05 add

void  ProcYXTimerTask(void)
{
	static intertime  lastclearalarmtime;
	intertime  now;
	int i,k,rcdnums,decsecs;
	BASESTATUS_STRU   *bstatusp;
	Rdb_Tab  rdbtable(MyUserName, MyPassWord);

	GetInterTime(&now);
	if(lastclearalarmtime==0 || now<lastclearalarmtime) {
		lastclearalarmtime=now;
		return;
	}
	decsecs=now-lastclearalarmtime;
	lastclearalarmtime=now;
	if(decsecs<=0)return;

	for(k=0;k<YX_RDBTABLENUMS;k++) {
		if(rdbtable.RdbOpenTab(yx_rdbtablename[k])!=RDB_OK) continue;
		rcdnums = rdbtable.RdbGetRcdCnt();
		for (i=0; i<rcdnums; i++) {
			if((bstatusp=(BASESTATUS_STRU *)rdbtable.rdbgetrcdaddr(i))==NULL)break;
			//if(bstatusp->changef) {//2004 05 del
			if(bstatusp->changef&&!NO_CLEAR_CHANGEF) {//2004 05 add 
				if(decsecs>=bstatusp->clearchgtime) {
					bstatusp->changef=0;
					bstatusp->clearchgtime=0;
					if(strstr(bstatusp->name,"Tt") && bstatusp->name[0]=='S') bstatusp->value=0;  //jsm 20040902 ++
				}
				else {
					bstatusp->clearchgtime -= decsecs;
				}
			}
			if(bstatusp->alarmf) {
				if(bstatusp->clearalmtime>0) {
					if(decsecs>=bstatusp->clearalmtime) {
						bstatusp->alarmf=0;
						bstatusp->clearalmtime=0;
					}
					else {
						bstatusp->clearalmtime -= decsecs;
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////
//时段参数表修改触发

#ifdef __unix

void CheckScadaTableUpdate(void) 
{
	static  int registedflag;
	static Rdb_EventWithUser  rdbevent(MyUserName,MyPassWord);
	int entcode;
	char  send_username[100];
	RDB_DATETIME_MS_STRU   entclock;
	RDB_EVENT_LOADTAB_STRU  *loadmsg;
	ScdTimePeriodType  scdtp;


	if(registedflag==0) {
		rdbevent.Register(getpid(),0);
		rdbevent.RdbNotifyEventType(RDB_EVENT_LOADTAB);
		registedflag=1;
	}

	do {
		entcode=rdbevent.RdbGetEvent(send_username,&entclock,(void **)&loadmsg);
		if(entcode==RDB_EVENT_LOADTAB) {
			if(strcmp(loadmsg->tabname,SDPARA_TABNAME)==0 ||
				strcmp(loadmsg->tabname,DAYSDPARA_TABNAME)==0) {
				//创建时段参数
				UpdateTimePeriodParaTab();
				FreshCurrentTimePeriodType();
			}
		}
	} while(entcode>0);
}
#endif

/*
//处理FEP服务器生数据的线程

static int  start_proc_slowyxf=1;

void   proc_fertrawdata(void)
{
	static int  procraw_timecount=0;
	//intertime			curtime;
	//int i;

	NetStatus netstat;  //add
    int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	
	/*GetInterTime(&curtime);
	if(fertrawproc_lasttime<=0 || curtime<fertrawproc_lasttime) {
		fertrawproc_lasttime=curtime;
	}

	fertrawproc_decsecs=CalDecSecs(fertrawproc_lasttime,curtime);
	fertrawproc_lasttime=curtime;*/
	
/*	for(int j=0;j<grpnum;j++)
	{
		NETDATA *netdatapara= compara.netdata[host_group_no[j]];
        int rtuindex;
		for(int i=0; i<netdatapara->rtunum; i++)
		{
			rtuindex = netdatapara->rtuno[i];
			rtuno[rtuindex] = i;
		}
//		printf("===========the %d group rtunum is %d\n",j,compara.netdata[host_group_no[j]]->rtunum);
	
		//for(i=0;i<SCADA_MAX_TERMINALNUMS;i++)
		//{
		//	terminalyxaddr[i]=NULL;terminalyxfirstaddr[i]=NULL;
		//	getTerminalYxaddr(i, &terminalyxaddr[i],&terminalyxfirstaddr[i],netdatapara);
		//	if(terminalyxdelaytime[i]>0)
		//		terminalyxdelaytime[i] -= 3;
		//}

		//for(i=0;i<SCADA_MAX_TERMINALNUMS;i++)
		//{
		//	terminalycaddr[i]=NULL;
		//	terminalycfirstaddr[i]=NULL;
		//	getTerminalYcaddr(i,&terminalycaddr[i],&terminalycfirstaddr[i],netdatapara);
		//}

		//for(i=0;i<SCADA_MAX_TERMINALNUMS;i++)
		//{
		//	terminalkwhaddr[i]=NULL;terminalkwhfirstaddr[i]=NULL;
		//	getTerminalKwhaddr(i, &terminalkwhaddr[i],&terminalkwhfirstaddr[i],netdatapara);
		//}
	
		// 3秒
		if(procraw_timecount%3==0)
		{
			TerminalYxDataInObjTable(host_group_no[j], start_proc_slowyxf);//将FEP服务器实时遥信数据放入实时库相应表中
		}

		TerminalYcDataInObjTable(host_group_no[j]);		//将FEP服务器实时遥测数据放入analog表中

//		ReadYCTimeLimitInObjTable(host_group_no[j]);		
		
		if(procraw_timecount%3==0) {
			TerminalKwhDataInObjTable(host_group_no[j]);//将FEP服务器实时电度数据放入kwh表中
			TerminalChannelDataInObjTable(host_group_no[j]);//将FEP服务器实时通道数据放入channel表中
			TerminalDevDataInObjTable(host_group_no[j]);//将FEP服务器实时多态数据(非电力采样数据)放入multistatus表中
		}
    }
	procraw_timecount++;
}*/


//处理FEP发送的实时数据
static int  start_proc_slowyxf=1;
void   proc_fertdata(void* buf,char *hostname)
{
	 	
	FE_MAIL_STRUCT *feMail;
	feMail = (FE_MAIL_STRUCT*)buf;
	/*static int icount = 0;
	icount++;
	//if( icount % 100 == 0)
	TRACE( "\n eve count = %d,lenth =%d \n",icount,feMail->mail_lenth);*/
    int i,j;
	NetStatus netstat;  
	int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	if (feMail->mail_type == INVALID)
	{
		return;
	}
	else if (feMail->mail_type == REAL_DATA)
	{
		FE_REAL_DATA_NODE *nodeData;
		nodeData = (FE_REAL_DATA_NODE *)feMail->mail_body;
		Rdb_Tab	fobjtable(MyUserName, MyPassWord);
		TERMINAL_STRU			*terminalstationp;
		if(fobjtable.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK) return;
		for (j=0;j<grpnum;j++)
		{
		    for (i=0;i<feMail->sub_information1;i++)
		    {
			    terminalstationp = (TERMINAL_STRU*)fobjtable.RdbSearchRcdAddrByKey(&(nodeData->rtu_no));
				if (terminalstationp!=NULL&&terminalstationp->useflag&&terminalstationp->status == 1&&nodeData->channel_no==terminalstationp->runchanno)
				{
					switch(nodeData->type) 
					{
					case FE_REAL_DATA_NODE::YX_REALDATA:	
						TerminalYxDataInObjTable(host_group_no[j],nodeData, start_proc_slowyxf);//将FEP服务器实时遥信数据放入实时库相应表中
						break;
					case FE_REAL_DATA_NODE::YC_REALDATA:		//  遥测
						TerminalYcDataInObjTable(host_group_no[j],nodeData);//将FEP服务器实时遥测数据放入analog表中
						break;
					case FE_REAL_DATA_NODE::KWH_REALDATA:		//  电度
						TerminalKwhDataInObjTable(host_group_no[j],nodeData);//将FEP服务器实时电度数据放入kwh表中
						break;
					default:
						break;
					}
				}
			
			nodeData++;
		    }
		}
	}
	else if (feMail->mail_type == CHANNEL_STAT )
	{
		FE_CHANNEL_STAT_MAIL *channelData;
		channelData = (FE_CHANNEL_STAT_MAIL *)feMail->mail_body;
		for (j=0;j<grpnum;j++)
		{
			for (i=0;i<feMail->sub_information1;i++)
			{
                TerminalChannelDataInObjTable(host_group_no[j],channelData,hostname);//将FEP服务器实时通道数据放入channel表中
                channelData++;
			}	  
		}
        CheckChannelStatus();//检查通道主备状态
	}
}


static int  DEADLIMIT=GetProgramRunPara("deadlimit",1200);

void proc_ycdata(int groupno)
{
	int	i,j,rcdnums;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	BASEANALOG_STRU	*fbaseanalogp;	// 指向BASEANALOG_STRU结构的指针
	BASEANALOG_STRU	*tmpanap;
	YC_MULTSRC_STRU *fycmultsrcp;
	if (fobjtable.RdbOpenTab(ANALOG_TABNAME)!=RDB_OK) return;
	fobjtable.RdbLockTabWithRW();
	rcdnums = fobjtable.RdbGetRcdCnt();
	for (i=0; i<rcdnums; i++) 
	{
		// 根据记录号取得BASEANALOG_STRU结构的地址
		fbaseanalogp = (BASEANALOG_STRU	*)fobjtable.rdbgetrcdaddr(i);
		if (fbaseanalogp==NULL)	continue;
		if (fbaseanalogp->groupno != groupno) continue;
		if (fbaseanalogp->calcultf)
		{
			fanalog.StatisticsCalVal(fbaseanalogp);
			continue;
		}
		intertime   curtime;
		uchar tmpdeadf=0;
		GetInterTime(&curtime);
		//死数检查
		if((!ISZERO(fbaseanalogp->value))) 
		{
			fbaseanalogp->deadsecs = curtime - fbaseanalogp->procsecs;
			if(fbaseanalogp->deadtimelimit<=120) fbaseanalogp->deadtimelimit=DEADLIMIT;//设置值过小，使用默认值
			if(fbaseanalogp->deadsecs>=fbaseanalogp->deadtimelimit)
			{
				tmpdeadf=1;
			}
		}
		if(tmpdeadf) 
		{
			if(fbaseanalogp->baseobjtype==OBJTYPE_ANGLE || fbaseanalogp->baseobjtype==OBJTYPE_TEMPERATURE ||
				fbaseanalogp->baseobjtype==OBJTYPE_HUMIDITY || fbaseanalogp->baseobjtype==OBJTYPE_DW) {
					tmpdeadf=0;
			}
		}
		if(fbaseanalogp->deadf!=tmpdeadf) 
		{
			fbaseanalogp->deadf=tmpdeadf;
			if(tmpdeadf==0)
			{
			    fbaseanalogp->deadsecs=0;
			}
		}
		if(!ISZERO(fbaseanalogp->maxchange)) //要做突变检查
		{
			float curdata;
			if(fbaseanalogp->slope==0 && fbaseanalogp->intercept==0)
			{
				curdata=fbaseanalogp->rawdata;
			}
			else
			{ 
				if(fbaseanalogp->scale == 0) fbaseanalogp->scale=1;
				curdata = (float)((fbaseanalogp->slope/fbaseanalogp->scale)*fbaseanalogp->rawdata+fbaseanalogp->intercept); //简单工程转换
			}
			if(ABS(curdata)<=fbaseanalogp->zeroval)		//零漂--剔除
			{
				curdata = 0.0;
				fbaseanalogp->ycinconsistf=0;
			}
			if(fbaseanalogp->overinccnt>0)
			{
				fbaseanalogp->overinccnt++;// = curtime-fbaseanalogp->procsecs;
				//SYS_CLOCK *curclockp;
				//memset(curclockp,0,sizeof(SYS_CLOCK));
				//ConvertTime(curtime,curclockp);
				if(fbaseanalogp->overinccnt>8)
				{
					fbaseanalogp->overinccnt=0;
					fbaseanalogp->value=curdata;
					if (fbaseanalogp->polgrade == 3)
					{
						fanalog.YieldOverMutationApEvent(fbaseanalogp);
					}

				}
			}

		}//end突变检查	
	}
	fobjtable.RdbUnlockTab();
	if (fobjtable.RdbOpenTab(YCMULTISRC_TABNAME)==RDB_OK) 
	{
		float  tmpycvalue;
		char   *tmpobjname;
		fobjtable.RdbLockTabWithRW();
		rcdnums = fobjtable.RdbGetRcdCnt();
		for (i=0; i<rcdnums; i++) 
		{
			// 根据记录号取得BASEANALOG_STRU结构的地址
			fycmultsrcp = (YC_MULTSRC_STRU	*)fobjtable.rdbgetrcdaddr(i);
			if (fycmultsrcp==NULL)	continue;
			fbaseanalogp=(BASEANALOG_STRU	*)fanalog.GetAnalogAddr(fycmultsrcp->objname);
			if (fbaseanalogp==NULL)	continue;
			for(j=0;j<MAX_YCSRC_NUMS;j++)
			{
			    if(fycmultsrcp->ycsrcpara[j].samplemode==MULTSRC_SAMPLING_OBJ)
				{
					tmpobjname=((char *)fycmultsrcp) + sizeof(YC_MULTSRC_STRU) + j * OBJECTNAME_LEN;
					tmpanap=(BASEANALOG_STRU*)fanalog.GetAnalogAddr(tmpobjname);
					if (tmpanap)
					{
						fycmultsrcp->ycsrcdata[j].value=tmpanap->value;
						fycmultsrcp->ycsrcdata[j].deadsecs=tmpanap->deadsecs;
						fycmultsrcp->ycsrcdata[j].deadf=tmpanap->deadf;
						fycmultsrcp->ycsrcdata[j].initedf=1;
						fycmultsrcp->ycsrcdata[j].invalidf=tmpanap->invalidf;
						fycmultsrcp->ycsrcdata[j].deverror=tmpanap->neterror | tmpanap->terminalerror;
						fycmultsrcp->ycsrcdata[j].procsecs = tmpanap->procsecs;
						fycmultsrcp->ycsrcdata[j].procmsecs = tmpanap->procmsecs;
					}
					else 
					{
						fycmultsrcp->ycsrcdata[j].initedf=0;
					}
				}
			}
		}//for
		fobjtable.RdbUnlockTab();
	}
}


int YxDataProcessing(int groupno,uchar tabletype)
{
	int	i, rcdnums;
	Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	BASESTATUS_STRU	*fbasestatusp;	// 指向BASESTATUS_STRU结构的指针
	switch(tabletype) {
		case Status_Table:
			if (fobjtable.RdbOpenTab(STATUS_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case DmsStatus_Table:
			if (fobjtable.RdbOpenTab(DMSSTATUS_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case Breaker_Table:
			if (fobjtable.RdbOpenTab(BREAK_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case Switch_Table:
			if (fobjtable.RdbOpenTab(SWITCH_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
		case ProtSig_Table:
			if (fobjtable.RdbOpenTab(PROT_TABNAME)!=RDB_OK) return(SCADA_ERR_OPENTAB);
			break;
	}

	fobjtable.RdbLockTabWithRW();
	rcdnums = fobjtable.RdbGetRcdCnt();
	for (i=0; i<rcdnums; i++) 
	{
		fbasestatusp =(BASESTATUS_STRU*)fobjtable.rdbgetrcdaddr(i);
		if (fbasestatusp==NULL)continue;
		if (fbasestatusp->groupno != groupno) continue;
		if(fbasestatusp->calcultf&&!fbasestatusp->mansetf)
		{
			if (fbasestatusp->procmsecs != fbasestatusp->procsecs)
			{
				fbasestatusp->procmsecs = fbasestatusp->procsecs;
				if (fbasestatusp->changesavef)
				{
					strcpy(fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].name,fbasestatusp->name);
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].value = fbasestatusp->value;
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].procsecs = fbasestatusp->procsecs;
					fstatus.yxdatap->yxData[fstatus.yxdatap->yxwritep].procmsecs = fbasestatusp->procmsecs;
					fstatus.yxdatap->yxwritep = (fstatus.yxdatap->yxwritep+1)%MAX_CHGDATANUM;
					fstatus.yxdatap->yxTrendDBwp = fstatus.yxdatap->yxwritep;
				}
			}
			continue;
		}
	}
	fobjtable.RdbUnlockTab();
	return(OK);

}

void proc_yxdata(int groupno)
{
	YxDataProcessing(groupno,ProtSig_Table);
	YxDataProcessing(groupno,Status_Table);
	YxDataProcessing(groupno,DmsStatus_Table);
	YxDataProcessing(groupno,Breaker_Table);
	YxDataProcessing(groupno,Switch_Table);
}


////////////////////////////////////////////////////
SlowYXLinkTable syxlt;

void  scada_timer_task_1s(void)
{
	//time_t curtime;
	//time(&curtime);
	//printf("proc one time fert raw %d\n", curtime);
	//proc_fertrawdata(); 
	int i;
	NetStatus netstat; 
	int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	for(i=0;i<grpnum;i++)
	{
	    proc_ycdata(host_group_no[i]);
	    proc_yxdata(host_group_no[i]);
	}
	//printf("proc one time slowyx\n");
	syxlt.SlowYXProc();	// 慢遥信处理
	//printf("end of raw data\n");
}

void  scada_timer_task_2s(void)
{
#ifdef __unix
	CheckScadaTableUpdate();
#endif

	FreshCurrentTimePeriodType();
}

void  scada_timer_task_4s(void)
{
	static int timecount;
	static BreakYCInconsist  brkycin;

	// 对电度进行积分
	CalIntegralKwh();
	//遥测多源数据处理
	YCMultiSourceDataProcessing();

	// 计算标准计算公式
//	AnaStandardCal();

	// 进行旁路替代
	if(timecount%2==0) { // 8 秒
		AnaBypassProcessing();
		KwhBypassProcessing(); 
		brkycin.LoopProcBreakYCInconsist();
	}

	//32秒定时处理小车开关状态，标志 
	if(timecount%8==0) { // 32 秒
		init_carbreak_status();
	}

	// 累计运行时间
	fbreak.CalRunTime();
	fanalog.TimerProcess();
	chan.CalRunTime();
	power_terminal.CalRunTime();

	ProcYXTimerTask();

	timecount++;
}

void  scada_timer_task_120s(void)
{
	FreshSysLastTime();
}


void  scada_timer_task_min(void)
{
	SYS_CLOCK   curclock;

	GetCurrentClock(&curclock);
	if(curclock.month==1 && curclock.day==1 &&
		curclock.hour==0 && curclock.day==0) {
		//创建时段参数
		UpdateTimePeriodParaTab();
		FreshCurrentTimePeriodType();
	}

	subst.CalSafeDays(); //计算安全运行日
	//subst.CalSubStYCState();
	//subst.CalSubStYXState();
}

///////////  设置定时任务 //////////////
void SetScadaTimerTask(void)
{
	ScdTimer   scdtimer;

	scdtimer.SetScdTimer(SCDTIMER_INTERV,1,scada_timer_task_1s);
	scdtimer.SetScdTimer(SCDTIMER_INTERV,2,scada_timer_task_2s);
	scdtimer.SetScdTimer(SCDTIMER_INTERV,4,scada_timer_task_4s);
	scdtimer.SetScdTimer(SCDTIMER_INTERV,120,scada_timer_task_120s);
	scdtimer.SetScdTimer(SCDTIMER_ACCURATE,1,scada_timer_task_min);
}


	ScdTimer  scdtimer;

///////////  处理SCADA 定时器 ////////////
#ifdef __unix
void  *scada_timer_scheduler(void *p)
#else
UINT scada_timer_scheduler(void *p) 
#endif
{

	for(;;) {
		scdtimer.ProcTimerTask();
		sleepms(1,0);
	}
	return(NULL);
}

int IsNotifyEvent(short evttype)
{
	Rdb_Tab   RdbTable(MyUserName,MyPassWord);
	NOTIFYEVTTYPE_RCDKEY_STRU notifyetkey;
	int  ret,rcdno;
	ret=RdbTable.RdbOpenTab(NOTIFYEVTTYPE_TABNAME);
	if(ret!=RDB_OK)	{
	   RdbPrintErr ("打开事项确认类型表失败",ret);
	   return(FALSE);
	}

	notifyetkey.eventtypeid=evttype;

	rcdno=RdbTable.RdbSearchRcdByKey(&notifyetkey);
	if(rcdno<0) {
//	   RdbPrintErr ("该事项类型不在事项确认类型表范围内，不加入实时事项通知队列\n",rcdno);
	   return(FALSE);
	}

	RdbTable.RdbCloseTab();
	return TRUE;

}
///////////  处理通知事项队列 ////////////
static int PROCNOTIFYEVENT=GetProgramRunPara("procnotifyevent",0);//2004 05 add
#ifdef __unix
void  *proc_notify_event_scheduler(void *argp)
#else
UINT  proc_notify_event_scheduler(LPVOID pParam)
#endif
{
    int ret;
	EventProc evtproc;
	SYS_EVENT sysevt;
	NetDbg dbg;
	while(1) {
		if(PROCNOTIFYEVENT){
			if(evtproc.Read(&sysevt)){
				SYS_EVENT_EX sysevtex;
				memset(&sysevtex, 0, sizeof(SYS_EVENT_EX));
				memcpy(&sysevtex.evt,&sysevt,sizeof(SYS_EVENT));
				if(IsNotifyEvent(sysevtex.evt.type)){
					dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t读到相关实时通知事项 sort=%d,type=%d,ymd=%d,hmsms=%d\n",sysevtex.evt.sort,sysevtex.evt.type,sysevtex.evt.ymd,sysevtex.evt.hmsms);
					Rdb_Tab  rdbtable(MyUserName,MyPassWord);
					ret=rdbtable.RdbOpenTab(NOTIFYEVTQUERCD_TABNAME);
					if(ret!=RDB_OK) dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t打开notifyevtrcd出错\n");

					int neqmaxrcdnums=rdbtable.RdbGetMaxRcdCnt();
					int rcdnums=rdbtable.RdbGetRcdCnt();

					scdnetbyte(RDB_DATATYPE_SHORT,&sysevtex.evt.sort);
					scdnetbyte(RDB_DATATYPE_SHORT,&sysevtex.evt.type);
					scdnetbyte(RDB_DATATYPE_INT,&sysevtex.evt.ymd);
					scdnetbyte(RDB_DATATYPE_INT,&sysevtex.evt.hmsms);
					scdnetbyte(RDB_DATATYPE_SHORT,&sysevtex.evt.state);
					scdnetbyte(RDB_DATATYPE_SHORT,&sysevtex.evt.state1);
					scdnetbyte(RDB_DATATYPE_SHORT,&sysevtex.evt.state2);
					scdnetbyte(RDB_DATATYPE_FLOAT,&sysevtex.evt.val);
					scdnetbyte(RDB_DATATYPE_FLOAT,&sysevtex.evt.val1);
					scdnetbyte(RDB_DATATYPE_FLOAT,&sysevtex.evt.val2);
					scdnetbyte(RDB_DATATYPE_FLOAT,&sysevtex.evt.val3);

					//压栈处理
					if(rcdnums>=neqmaxrcdnums){
						ret=rdbtable.RdbDelRecord(0,1);
						if(ret!=RDB_OK)dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t压栈删除出错\n");
					}
					ret=rdbtable.RdbAddRecord(&sysevtex);
					if(!ret)dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t加入实时事项通知队列\n");
					if(ret!=RDB_OK) dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t加入实时事项通知队列出错\n");

					if(IsMainServer(sysevtex.evt.grpno))
					{
						RdbBackupTab(MyUserName,MyPassWord, NOTIFYEVTQUERCD_TABNAME);
						dbg.PutDbg(NET_DBGTYPE_RTDB_PROC,"SCADA:\t备份实时事项通知队列表\n");
					}
				}
			}
		}
		sleepms(1,0);
	}
	return(NULL);
}

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
/////////////////////////////
#ifdef __unix
extern void  *timersavehisdataproc(void *argp);
extern void  *proc_timer_task(void *argp);
#endif

//zl20130917
#if defined WIN32
UINT proc_prot_resetting_check(LPVOID pParam)
#else
void * proc_prot_resetting_check(void * argp)
#endif
{
	EventProc m_eventproc;
	unsigned int  count=0;
	unsigned int  timer_cnt = 0;

	for(;;) 
	{
		if( PROT_RESETTING_FLAG > 0 && timer_cnt> PROT_RESETTING_FLAG*60 )
		{
			timer_cnt = 0;

			Rdb_LocalTab rdb;
			RDB_BTBL_TAB_STRU tabInfo;
			void * pTmp = NULL;
			if(rdb.RdbOpenTab("prot") != 0)
				return FALSE;

			rdb.RdbGetTableInfo(&pTmp,&tabInfo);
			if( tabInfo.recordlen != sizeof(PROTSIG_STRU) )
			{
				rdb.RdbCloseTab();
				return FALSE;
			};

			if(pTmp == NULL) 
				return FALSE;

			PROTSIG_STRU *m_pProt = (PROTSIG_STRU *)pTmp;
			count = tabInfo.rcdcount;

			for(int i=0; i<count; i++)
			{
			
				if((m_pProt+i)->basestatus.value == 0)
					continue;

				SYS_EVENT evt;
				memset(&evt,0,sizeof(SYS_EVENT));
				evt.grpno = 1;
				evt.sort = ENTCLASS_PROTECT;
				evt.DiskOut = 1;
				evt.DispOut = 1;
				SYS_TIME time;
				TimeProc timeproc;
				timeproc.GetTime(time);
				evt.type = 3005;
				evt.ymd = time.year*10000 + time.mon*100 + time.day;
				evt.hmsms = time.hour*10000000 + time.min*100000 + time.sec*1000 + time.ms;

				sprintf(evt.text," %s 未复归",(m_pProt+i)->basestatus.describe );
				evt.val = 0.0;
				evt.val1 = 0.0;
				evt.state = 0;
				m_eventproc.Save(&evt,1);
			}
		}
		timer_cnt++;
		sleepms(1,0);
	}
	return 0;
}


bool saveYcData(CHANGE_YCDATA *ycDatap,int num)
{
	int i,j;
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	memset( &func,0, sizeof( func ) );
	CS_DATAFMT      *datafmt=NULL;
	int	ptcnt;
	void *pt = NULL;
	int keyid1, keyid2, keyid3, keyid4;
	SYS_CLOCK   curclock;
	GetCurrentClock(&curclock);
    char tabname[128];
	/*if( dbapi.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}*/
    func.func = GET_TABLE_INFO;
	sprintf(tabname,"ycchange%.4d%.2d%.2d",curclock.year,curclock.month,curclock.day);
	strcpy( func.table, tabname );
	strcpy( func.dbname, "modeldb" );
	datafmt=0;
	if(dbapi.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		//dbapi.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		//dbapi.Close();	
		return FALSE;
	}

	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;

		if( !strcasecmp( datafmt[i].name, "代码" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid1 = i;
		}
		if( !strcasecmp( datafmt[i].name, "值" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid2 = i;
		}
		if( !strcasecmp( datafmt[i].name, "处理时间秒" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid3 = i;
		}
		if( !strcasecmp( datafmt[i].name, "处理时间毫秒" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid4 = i;
		}

	}
	func.func = INS_RECORDERS;
	func.roxnum = num;

	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0,  func.recorderlen * func.roxnum  );
	ptcnt = 0;
	for( j = 0; j < num; j ++ )
	{
		dbapi.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(ycDatap->name ) );
		dbapi.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(&(ycDatap->value)));
		dbapi.FillData( &datafmt[ keyid3 ], pt, ptcnt, (void*)(&(ycDatap->procsecs) )) ;
		dbapi.FillData( &datafmt[ keyid4 ], pt, ptcnt, (void*)(&(ycDatap->procmsecs)) );
		ycDatap++;
	}

	if((retcode = dbapi.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		//dbapi.Close();	
		return FALSE;
	}

	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	//dbapi.Close();

	return TRUE;
}


bool saveYxData(CHANGE_YXDATA *yxDatap,int num)
{
	int i,j;
	CS_RETCODE      retcode = 0; 
	FUNC_STR        func;
	memset( &func,0, sizeof( func ) );
	CS_DATAFMT      *datafmt=NULL;
	int	ptcnt;
	void *pt = NULL;
	int keyid1, keyid2, keyid3, keyid4;
	SYS_CLOCK   curclock;
	GetCurrentClock(&curclock);
    char tabname[128];
	/*if( dbapi.Open( DB_OP_WR, FALSE, FALSE ) < 0 )
	{
		return FALSE;
	}*/
	func.func = GET_TABLE_INFO;
	sprintf(tabname,"yxchange%.4d%.2d%.2d",curclock.year,curclock.month,curclock.day);

	strcpy( func.table, tabname );
	strcpy( func.dbname, "modeldb" );
	datafmt=0;
	if(dbapi.GetTableInfo(&func,&datafmt)==CS_FAIL)
	{
		if(datafmt)	free(datafmt);
		//dbapi.Close();	
		return FALSE;
	}
	if (func.colnum<1 || !datafmt)
	{
		if (datafmt) free(datafmt);
		//dbapi.Close();	
		return FALSE;
	}

	for( i = 0; i < func.colnum; i ++ )
	{
		func.col[i].select = FALSE;
		func.col[i].order_d = -1;
		func.col[i].order = -1;
		func.col[i].condition=FALSE;

		if( !strcasecmp( datafmt[i].name, "代码" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid1 = i;
		}
		if( !strcasecmp( datafmt[i].name, "值" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid2 = i;
		}
		if( !strcasecmp( datafmt[i].name, "处理时间秒" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid3 = i;
		}
		if( !strcasecmp( datafmt[i].name, "处理时间毫秒" ) )
		{
			func.col[i].select = TRUE;
			func.col[i].order_d = 1;
			keyid4 = i;
		}

	}
	func.func = INS_RECORDERS;
	func.roxnum = num;

	pt= malloc( func.recorderlen * func.roxnum );
	memset( pt, 0,  func.recorderlen * func.roxnum  );
	ptcnt = 0;
	for( j = 0; j < num; j ++ )
	{
		dbapi.FillData( &datafmt[ keyid1 ], pt, ptcnt, (void*)(yxDatap->name ) );
		dbapi.FillData( &datafmt[ keyid2 ], pt, ptcnt, (void*)(&(yxDatap->value)));
		dbapi.FillData( &datafmt[ keyid3 ], pt, ptcnt, (void*)(&(yxDatap->procsecs) )) ;
		dbapi.FillData( &datafmt[ keyid4 ], pt, ptcnt, (void*)(&(yxDatap->procmsecs)) );
		yxDatap++;
	}

	if((retcode = dbapi.InsertRcds(&func,datafmt,pt)) != CS_SUCCEED)
	{
		if(datafmt)	free(datafmt);
		if(pt)	free(pt);
		//dbapi.Close();	
		return FALSE;
	}

	if( pt )
	{
		free(pt);
		pt = NULL;
	}
	if( datafmt )
	{
		free(datafmt);
		datafmt = NULL;
	}

	//dbapi.Close();

	return TRUE;
}

static int SAVE_CHANGEDATA = GetProgramRunPara("savechangedata",(int)0);

// 实时数据变化存储线程 zxm20140218
#if defined WIN32
UINT changesaveproc(LPVOID pParam)
#else
void * changesaveproc(void *argp)
#endif
{
	int	ycdatanum,yxdatanum;
	int ycreadp,yxreadp;
	int ycwritep,yxwritep;
	//CHANGE_YCDATA  *tmpYcData = new CHANGE_YCDATA[MAX_CHGDATANUM];
	//CHANGE_YXDATA  *tmpYxData = new CHANGE_YXDATA[MAX_CHGDATANUM];
	intertime   yclasttime, yxlasttime,curtime = 0;
	GetInterTime(&curtime);
	yclasttime = curtime;
	yxlasttime = curtime;
	int ycsave_decsecs =0,yxsave_decsecs =0;
	for (;;) 
	{
		if(SAVE_CHANGEDATA == 1||SAVE_CHANGEDATA == 3)
		{
			GetInterTime(&curtime);
			if(yclasttime<=0 || curtime<yclasttime) 
			{
				yclasttime = curtime;
			}
			ycsave_decsecs = curtime-yclasttime;
			if(ycsave_decsecs<0)ycsave_decsecs=0;
			ycreadp = fanalog.ycdatap->ycreadp;
			ycwritep = fanalog.ycdatap->ycwritep;
			ycdatanum = (ycwritep + MAX_CHGDATANUM - ycreadp)%MAX_CHGDATANUM;
			if (ycsave_decsecs>=200||ycdatanum >=200)
			{
				if (ycwritep>ycreadp)
				{
					saveYcData((CHANGE_YCDATA*)&fanalog.ycdatap->ycData[ycreadp],ycdatanum);
				}
				if (ycwritep<ycreadp)
				{
					//ycdatanum = MAX_CHGDATANUM-ycreadp;
					saveYcData((CHANGE_YCDATA*)&fanalog.ycdatap->ycData[ycreadp],MAX_CHGDATANUM-ycreadp);
					//ycdatanum = ycwritep;
					saveYcData((CHANGE_YCDATA*)&fanalog.ycdatap->ycData[0],ycwritep);
					/*memcpy( (void*)&tmpYcData, (void*)&fanalog.ycdatap->ycData[ycreadp], 
						( MAX_CHGDATANUM-ycreadp ) * sizeof( CHANGE_YCDATA ) );
					memcpy( (void*)&tmpYcData[ MAX_CHGDATANUM-ycreadp], (void*)&fanalog.ycdatap->ycData[0], 
						ycwritep * sizeof( CHANGE_YCDATA ) );
					saveYcData((CHANGE_YCDATA*)tmpYcData,ycdatanum);*/
				}
				fanalog.ycdatap->ycreadp = (ycreadp + ycdatanum)%MAX_CHGDATANUM;
				yclasttime = curtime;
			}

			if(yxlasttime<=0 || curtime<yxlasttime) 
			{
				yxlasttime = curtime;
			}
			yxsave_decsecs = curtime-yxlasttime;
			if(yxsave_decsecs<0)yxsave_decsecs=0;
			yxreadp = fstatus.yxdatap->yxreadp;
			yxwritep = fstatus.yxdatap->yxwritep;
			yxdatanum = (yxwritep + MAX_CHGDATANUM - yxreadp)%MAX_CHGDATANUM;
			if (yxsave_decsecs>=200||yxdatanum >=200)
			{
				if (yxwritep>yxreadp)
				{
					saveYxData((CHANGE_YXDATA*)&fstatus.yxdatap->yxData[yxreadp],yxdatanum);
				}
				if (yxwritep<yxreadp)
				{
					//yxdatanum = MAX_CHGDATANUM-yxreadp;
					saveYxData((CHANGE_YXDATA*)&fstatus.yxdatap->yxData[yxreadp], MAX_CHGDATANUM-yxreadp);
					//yxdatanum = yxwritep;
					saveYxData((CHANGE_YXDATA*)&fstatus.yxdatap->yxData[0],yxwritep);
					/*memcpy( (void*)&tmpYxData, (void*)&fstatus.yxdatap->yxData[yxreadp], 
						( MAX_CHGDATANUM-yxreadp ) * sizeof( CHANGE_YXDATA ) );
					memcpy( (void*)&tmpYxData[ MAX_CHGDATANUM-yxreadp], (void*)&fstatus.yxdatap->yxData[0], 
						yxwritep * sizeof( CHANGE_YXDATA ) );
					saveYxData((CHANGE_YXDATA*)tmpYxData,yxdatanum);*/
				}
				fstatus.yxdatap->yxreadp = (yxreadp + yxdatanum)%MAX_CHGDATANUM;
				yxlasttime = curtime;
			}
		}
		sleepms(2,0);
	}
	//delete tmpYcData;
	//delete tmpYxData;
	return NULL;
}


int TrendDBSaveYcData(char* connection,CHANGE_YCDATA *ycDatap,int num,int histDays,short histDepth)
{
	int i;
	for(i = 0; i < num; ++i)
	{
		TrendDBAPI::DBCreateYcPoints(connection,ycDatap->name,ycDatap->name,ycDatap->unit,histDays,histDepth);
		float ycvalue = ycDatap->value;
		TrendDBAPI::DBSaveYcData(connection,ycvalue,ycDatap->name);
	}
	TrendDBAPI::DBFlushConnectionBuffer(connection);
	return 0;
}


int TrendDBSaveYxData(char* connection,CHANGE_YXDATA *yxDatap,int num,int histDays)
{
	int i;
	for(i = 0; i < num; ++i)
	{
		TrendDBAPI::DBCreateYxPoints(connection,yxDatap->name,yxDatap->name,histDays);
		uchar yxvalue = yxDatap->value;
		TrendDBAPI::DBSaveYxData(connection,yxvalue,yxDatap->name);
	}
	TrendDBAPI::DBFlushConnectionBuffer(connection);
	return 0;
}


// 时序数据库存储线程 zxm20141008
#if defined WIN32
UINT TrendDBsaveproc(LPVOID pParam)
#else
void * TrendDBsaveproc(void *argp)
#endif
{
	if (SAVE_CHANGEDATA == 2||SAVE_CHANGEDATA == 3)
	{
		TrendDBAPI::DBStartupAPI();
		char db[24];
		char nameserver[40];
		char user[24];
		char passwd[24];
		int dataHistDays;
		short datahistDepth;
		/*int i,j;
		CS_RETCODE      retcode = 0; 
		FUNC_STR        func;
		CS_DATAFMT      *datafmt=NULL;
		int				ptcnt = 0;

		printf( "时序数据库参数表" );
		fflush( stdout );

		func.func = GET_TABLE_INFO;
		strcpy(func.table,"时序数据库参数表");
		strcpy(func.dbname,"modeldb");
		char * col_name[] = {"服务名称","服务地址","用户名","密码","历史数据保存天数","数据缓存深度"};

		datafmt = 0;
		void *pt=0;
		if(dbapi.GetTableInfo(&func,&datafmt)==CS_FAIL)
		{
			if(datafmt)	free(datafmt);
			dbapi.Close();	
			return FALSE;
		}
		if (func.colnum<1 || !datafmt)
		{
			if (datafmt) free(datafmt);
			dbapi.Close();	
			return FALSE;
		}

		int   col_num = sizeof(col_name)/sizeof(char*);
		for(i=0;i<func.colnum;i++)
		{
			func.col[i].select = NO;
			func.col[i].order_d = -1;
			func.col[i].order = -1;
			func.col[i].condition=NO;
			for(j=0; j<col_num; j++)
			{
				if(strcmp(datafmt[i].name,col_name[j]))	continue;
				func.col[i].select = YES;
				func.col[i].order_d = j;
			}
			//if ( strcasecmp( datafmt[i].name,"代码") == 0 )
			//{
			//	func.col[i].order = 0 ;
			//}
		}
		func.func = SEL_RECORDERS;
		if((retcode = dbapi.SelectRcds(&func,datafmt,&pt)) != CS_SUCCEED)
		{
			if(datafmt)	free(datafmt);
			if(pt)	free(pt);
			dbapi.Close();	
			return FALSE;
		}

		int count;
		//char tmpbuf[ 256 ];
		for( i = 0; i < func.ret_roxnum; i++  )
		{	
			count = 0;
			dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,db);
			dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,nameserver);
			dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,user);
			dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,passwd);
			dataHistDays = (int)dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
			datahistDepth = (short)dbapi.GetData(i,&func,datafmt,col_name[count++],pt,ptcnt,NULL);
		}
		if (func.roxnum != 0) 
		{
			free(pt);
		}
		if (datafmt) 
		{
			free(datafmt);
		}
		if (func.ret_roxnum==0)
		{
			return FALSE;
		}*/
		int		i, rcdnums;
	    Rdb_Tab	fobjtable(MyUserName, MyPassWord);
	    TRENDDB_STRU		*trenddbp;		
	    if (fobjtable.RdbOpenTab(TRENDDB_TABNAME)!=RDB_OK)
		{
			return FALSE;
		}
	    fobjtable.RdbLockTabWithRW();
	    rcdnums = fobjtable.RdbGetRcdCnt();
    	for (i=0; i<rcdnums; i++) 
	    {
		    trenddbp = (TRENDDB_STRU *)fobjtable.rdbgetrcdaddr(i);
     		if (trenddbp==NULL)continue;
			strcpy(db, trenddbp->db );
			strcpy(nameserver,trenddbp->nameserver);
			strcpy(user, trenddbp->user);
			strcpy(passwd, trenddbp->passwd);
            dataHistDays = trenddbp->datahistdays;
			datahistDepth = trenddbp->datahistdepth;
	    }//for
	    fobjtable.RdbUnlockTab();
		int nRet = TrendDBAPI::DBInitNameServers(nameserver,1,NULL,NULL);
		char connectionId[256];
		sprintf(connectionId, "thr@%s", db);
	
		nRet = TrendDBAPI::DBCreateConnection(connectionId, false,false,  user, passwd);
		if (nRet<0)
		{
			return FALSE;
		}
		int	ycTrendDBnum,yxTrendDBnum;
		int ycTrendDBrp,yxTrendDBrp;
		int ycTrendDBwp,yxTrendDBwp;
		//CHANGE_YCDATA  *DBYcData = new CHANGE_YCDATA[MAX_CHGDATANUM];
		//CHANGE_YXDATA  *DBYxData = new CHANGE_YXDATA[MAX_CHGDATANUM];
		for (;;)
		{
			ycTrendDBrp = fanalog.ycdatap->ycTrendDBrp;
			ycTrendDBwp = fanalog.ycdatap->ycTrendDBwp;
			ycTrendDBnum = (ycTrendDBwp + MAX_CHGDATANUM - ycTrendDBrp)%MAX_CHGDATANUM;
			if (ycTrendDBwp>ycTrendDBrp)
			{
				TrendDBSaveYcData(connectionId,(CHANGE_YCDATA*)&fanalog.ycdatap->ycData[ycTrendDBrp],ycTrendDBnum,dataHistDays,datahistDepth);
			}
			if (ycTrendDBwp<ycTrendDBrp)
			{
				ycTrendDBnum =  MAX_CHGDATANUM-ycTrendDBrp;
	            TrendDBSaveYcData(connectionId,(CHANGE_YCDATA*)&fanalog.ycdatap->ycData[ycTrendDBrp],ycTrendDBnum,dataHistDays,datahistDepth);
				ycTrendDBnum = ycTrendDBwp;
				TrendDBSaveYcData(connectionId,(CHANGE_YCDATA*)&fanalog.ycdatap->ycData[0],ycTrendDBnum,dataHistDays,datahistDepth);
				/*memcpy( (void*)&DBYcData, (void*)&fanalog.ycdatap->ycData[ycTrendDBrp], 
					( MAX_CHGDATANUM-ycTrendDBrp ) * sizeof( CHANGE_YCDATA ) );
				memcpy( (void*)&DBYcData[ MAX_CHGDATANUM-ycTrendDBrp], (void*)&fanalog.ycdatap->ycData[0], 
					ycTrendDBwp * sizeof( CHANGE_YCDATA ) );
				TrendDBSaveYcData(connectionId,(CHANGE_YCDATA*)DBYcData,ycTrendDBnum,dataHistDays,datahistDepth)	;*/
			}
			ycTrendDBnum = (ycTrendDBwp + MAX_CHGDATANUM - ycTrendDBrp)%MAX_CHGDATANUM;
			fanalog.ycdatap->ycTrendDBrp = (ycTrendDBrp + ycTrendDBnum)%MAX_CHGDATANUM;


			yxTrendDBrp = fstatus.yxdatap->yxTrendDBrp;
			yxTrendDBwp = fstatus.yxdatap->yxTrendDBwp;
			yxTrendDBnum = (yxTrendDBwp + MAX_CHGDATANUM - yxTrendDBrp)%MAX_CHGDATANUM;
			if (yxTrendDBwp>yxTrendDBrp)
			{
				TrendDBSaveYxData(connectionId,(CHANGE_YXDATA*)&fstatus.yxdatap->yxData[yxTrendDBrp],yxTrendDBnum,dataHistDays);
			}
			if (ycTrendDBwp<ycTrendDBrp)
			{
				yxTrendDBnum = MAX_CHGDATANUM-yxTrendDBrp;
                TrendDBSaveYxData(connectionId,(CHANGE_YXDATA*)&fstatus.yxdatap->yxData[yxTrendDBrp],yxTrendDBnum,dataHistDays);
				yxTrendDBnum = yxTrendDBwp;
			    TrendDBSaveYxData(connectionId,(CHANGE_YXDATA*)&fstatus.yxdatap->yxData[0],yxTrendDBnum,dataHistDays);
				/*memcpy( (void*)&DBYxData, (void*)&fstatus.yxdatap->yxData[yxTrendDBrp], 
					( MAX_CHGDATANUM-yxTrendDBrp ) * sizeof( CHANGE_YXDATA ) );
				memcpy( (void*)&DBYxData[ MAX_CHGDATANUM-yxTrendDBrp], (void*)&fstatus.yxdatap->yxData[0], 
					yxTrendDBwp * sizeof( CHANGE_YXDATA ) );
				TrendDBSaveYxData(connectionId,(CHANGE_YXDATA*)DBYcData,yxTrendDBnum,dataHistDays);*/
			}
			yxTrendDBnum = (yxTrendDBwp + MAX_CHGDATANUM - yxTrendDBrp)%MAX_CHGDATANUM;
			fstatus.yxdatap->yxTrendDBrp = (yxTrendDBrp + yxTrendDBnum)%MAX_CHGDATANUM;
	        sleepms(2,0);
		}
		TrendDBAPI::DBClose(connectionId);
		//delete DBYcData;
		//delete DBYxData;
		return 0;
	}
	return NULL;
}





#ifdef __unix
int  started_sybase_flag=0;
int main(int argc,char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("scdsvc,1.1");
				printf("scdsvc, 1.1");
				return 0;
			}
		}
	}
	
	uhlong   timecount=0;
	ProcMan  procman;
	procman.StartProc("scdsvc", "SCADA处理主进程","",NULL);

	CProcInfo  p_proc;
	if(p_proc.GetProcNum("scdsvc")>1)
	{
		printf("scdsvc already run!\n");
		return 0;
	}

#ifdef __sun
	sigset(SIGUSR1,usr1_func);
	setpgrp();
#else
	sigset(SIGUSR1,SIG_IGN);
	setsid();
	//setpgid(getpid(),getpid());
#endif

	NetDbg dbg;
	dbg.RegProc("SCADA处理主进程");

    if(!IsScadaServer()){
		printf("\n本机不是SCADA服务器!!!\t请将本机配置为SCADA服务器后运行\n");
		return 0;
	}

	//CreateAllMem(&compara);
	NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	for(int i=0; i<grpnum; i++)
	{
		//CreateShmMem(host_group_no[i],&compara);
	}

	scada_start_init();
	SetScadaTimerTask();

	//  scada service receive mail thread 
	scd_pthread_create(scadamailproc_level0,NULL,100);
	scd_pthread_create(scadamailproc_level1,NULL,100);

	//  YK and UPDOWN process 
	scd_pthread_create(ykupdownproc,NULL,2000);

	//  SG process
	scd_pthread_create(accidentdealproc,NULL,100);

	//  yield power event 
	scd_pthread_create(powereventproc,NULL,100);

	//  data deal timer  scheduler
	scd_pthread_create(scada_timer_scheduler,NULL,100);

	//  notify event proc
	scd_pthread_create(proc_notify_event_scheduler,NULL,100);

	//  Check Parent Thread
	scd_pthread_create(proc_timer_task,NULL,30);

	//  scada service receive mail thread 
	scd_pthread_create(scadamailproc_level2,NULL,100);
	
	//  Check Parent Thread
	scd_pthread_create(changesaveproc,NULL,100);

	//zl20140303,prot reseting warnning
	scd_pthread_create(proc_prot_resetting_check,NULL,30);

	scd_pthread_create(TrendDBsaveproc,NULL,100);
	//启动历史数据库
    DBApi dbapi;
	int m;
//	for(m=0;m<4;m++) {
	dbapi.Open(DB_OP_W,FALSE,TRUE);
//			break;
//		sleepms(2,0);
//	}

	// lhl 20110727 离库运行
// 	if(m>=4) 
// 		return 1;

	started_sybase_flag=1;
	//  save thread
	timersavehisdataproc(NULL);
	dbapi.Close();
	started_sybase_flag=0;

/*
    for (;;) {
		if (!procman.CheckParentProc())	{
			printf("scdsvc exit: CheckParentProc returning 0\n");
			exit(1);
		}
		timecount++;
		sleepms(2,0);
    } 
*/
	dbg.UnRegProc();

    return 0;
}
#endif

