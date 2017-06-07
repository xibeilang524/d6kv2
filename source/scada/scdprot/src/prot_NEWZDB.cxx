#include "scadaproc.h"
#include "procthread.h"
#include "prot_NEWZDB.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					NEWZDB 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



NEWZDB_Protect::NEWZDB_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *NEWZDB_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			&&(mdlp->address2==address2))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取事件信息参数
NEWZDB_PROTECTACTION	*NEWZDB_Protect::GetActionPara(short actioncode)
{
	return (NEWZDB_PROTECTACTION *)table.RdbGetRecordAddr(NEWZDB_PROTACTION_TABNAME,&actioncode);
}

//取动作自检信息
IEC103_PROTECTACTION* NEWZDB_Protect::GetIEC103ActionPara(unsigned short devicetype,unsigned short actioncode)
{
	IEC103_ACTION_KEY key;
	key.devicetype = devicetype;
	key.actioncode = actioncode;
	return ((IEC103_PROTECTACTION *)table.RdbGetRecordAddr(IEC103_PROTACTION_TABNAME, &key));
}

//解释FEP服务器保护生报文
int NEWZDB_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到NEWZDB的保护事件邮件");
	if(gram_len<30) return PROT_ERR_MAILLEN;

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	m_GramLength=m_Buf[3];									//报文长度(用来计算量测值的个数)
//	GetProtinfoTime(gram+24, TIMEBYBCD, &sys_clock);	//发生时间
	address2=m_Buf[4]+m_Buf[5]*256;						//从站地址	对应微机保护参数保护参数表中得地址2
	address1=m_Buf[6]+m_Buf[7]*256;						//主站地址	对应微机保护参数保护参数表中得地址1
	mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//微机保护参数
	

	if(m_Buf[16] == 0xa3)		//动作事件
	{
		//微机保护动作事件
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成NEWZDB动作事件");
		char text[60];
		sprintf(text,"终端号:%d\t规约类型:%d\t主站地址:%d\t从站地址:%d\t",terminalno,protocoltype,address1,address2);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,m_Buf+20);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存NEWZDB动作事件");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf+20);

	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void NEWZDB_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	NEWZDB_PROTECTACTION *newzdb_action;
	short	actioncode;
	int actionnum = 0;//动作个数
	SYS_CLOCK  sys_clock;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	actionnum = event[0];
	
	if (actionnum<=0) return;
	for (int i=0;i<actionnum;i++)//生成actionnum个保护动作应用事件
	{

		actioncode=event[i*10+2];	//动作序号
		msg.type_value = (int)actioncode;
		GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//发生时间
		make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_NEWZDB);	//公共信息

		if((newzdb_action = GetActionPara(actioncode))==NULL)return;	//取动作信息
		msg.sgflag = newzdb_action->sgflag;
		strcpy(msg.enttypedescribe,newzdb_action->actioninfo);		//事件类型描述
		apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
		
	}
}


//动作事项存盘
int NEWZDB_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	NEWZDB_PROTECTACTION *newzdb_action;
	IEC103_PROTECTACTION *iec103_action;//200507
	short	actioncode;
	int actionnum = 0;//动作个数
	SYS_CLOCK  sys_clock;
	NetDbg dbg;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////
	actionnum = event[0];
	if (actionnum<=0) return PROT_ERR_ACTION;

	tmpevent.evt.state = event[2];	//动作序号

	if (mdlp->type1 ==0)
	{
		for (int i=0;i<actionnum;i++)//生成actionnum个保护动作应用事件
		{
			actioncode=event[i*10+2];	//动作序号
			tmpevent.evt.state = (int)actioncode;
			GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//发生时间
			make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//公共信息

			if((newzdb_action = GetActionPara(actioncode))==NULL)return PROT_ERR_ACTION;	//取动作信息
			sprintf(tmpevent.evt.member1,"%d",newzdb_action->actioncode);
			strcpy(tmpevent.member_des1,newzdb_action->actioninfo);
			tmpevent.evt.ToneOut = newzdb_action->alarmf;
			strcpy(entname,newzdb_action->entname);
			
			yield_entdescribe(entname,&tmpevent);
			if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
			//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存NEWZDB动作事项");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存NEWZDB动作事项完毕");
		}
	}
	else
	{
		for (int i=0;i<actionnum;i++)//生成actionnum个保护动作应用事件
		{
			actioncode=event[i*10+2];	//动作序号
			tmpevent.evt.state = (int)actioncode;
			GetProtinfoTime(event+4+i*10, TIMEBYBCD, &sys_clock);	//发生时间
			make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//公共信息

			if((iec103_action = GetIEC103ActionPara(mdlp->type1,actioncode))==NULL)return PROT_ERR_ACTION;	//取动作信息
			sprintf(tmpevent.evt.member1,"%d",iec103_action->actioncode);
			strcpy(tmpevent.member_des1,iec103_action->actioninfo);
			tmpevent.evt.ToneOut = iec103_action->alarmf;
			strcpy(entname,iec103_action->entname);
			
			yield_entdescribe(entname,&tmpevent);
			if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
			//2003 03
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存NEWZDB动作事项");
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存NEWZDB动作事项完毕");
		}
	}
	return OK;
}



int NEWZDB_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	time_t Sec;
	Sec=gram_part[0]+(gram_part[1]+(gram_part[2]+gram_part[3]*256)*256)*256;
	struct tm *time;
	time=gmtime(&Sec);
	clock->year=time->tm_year+1900;
	clock->month=time->tm_mon+1;
	clock->day=time->tm_mday;
	clock->hour=time->tm_hour;
	clock->minute=time->tm_min;
	clock->second=time->tm_sec;
	long mSec;
	mSec=gram_part[4]+(gram_part[5]+(gram_part[6]+gram_part[7]*256)*256)*256;
	clock->msecond=mSec/1000;
	return OK;
}
