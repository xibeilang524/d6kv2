#include "scadaproc.h"
#include "procthread.h"
#include "prot_SACDW.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					SACDW 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



SACDW_Protect::SACDW_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *SACDW_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) )
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取事件信息参数
SACDW_PROTECTACTION	*SACDW_Protect::GetActionPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACDW key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACDW_PROTECTACTION *)table.RdbGetRecordAddr(SACDW_PROTACTION_TABNAME,&key);
}


//解释FEP服务器保护生报文
int SACDW_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到SACDW的保护事件邮件");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	address1=m_Buf[0];										//主站地址	对应微机保护参数保护参数表中得地址1
	mdlp = GetModulePara(terminalno,protocoltype,address1);			//微机保护参数
	
	//微机保护动作事件
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成SACDW动作事件");
	char text[60];
	sprintf(text,"终端号:%d\t规约类型:%d\t地址:%d\n",terminalno,protocoltype,address1);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
	if(mdlp == NULL)return PROT_ERR_MODULE;
	make_action_apevent(&sys_clock,mdlp,m_Buf);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存SACDW动作事件");
	ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	return ret;
}

//生成保护动作应用事件
void SACDW_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	SACDW_PROTECTACTION *action;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int ms,minute,hour;

	if(mdlp == NULL )return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));

	actioncode=event[1];	//动作序号
	ms = event[3]+event[4]*256;
	minute = event[5];
	hour = event[6];
	GetCurrentClock(&sys_clock);
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =ms/1000;
	sys_clock.msecond =ms%1000;

	msg.type_value = (int)actioncode;
	make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_SACDW);	//公共信息
	actiontype = mdlp->type1;
	if((action = GetActionPara(actioncode,actiontype))==NULL)return;	//取动作信息
	msg.sgflag = action->sgflag;
	strcpy(msg.enttypedescribe,action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//动作事项存盘
int SACDW_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	SACDW_PROTECTACTION *action;
	short	actioncode,actiontype;
	int actionnum = 0;//动作个数
	SYS_CLOCK  sys_clock;
	NetDbg dbg;

	entname[0] = 0;
	if(mdlp == NULL )return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	int ms,minute,hour;
	ms = event[3]+event[4]*256;
	minute = event[5];
	hour = event[6];
	GetCurrentClock(&sys_clock);
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =ms/1000;
	sys_clock.msecond =ms%1000;
	/////////生成存盘事项//////////////////////////////////////////////////
	tmpevent.evt.state = event[1];	//动作序号
	actioncode=event[1];	//动作序号
	tmpevent.evt.state = (int)actioncode;
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//公共信息
	actiontype = mdlp->type1;
	if((action = GetActionPara(actioncode,actiontype))==NULL)return PROT_ERR_ACTION;	//取动作信息
	sprintf(tmpevent.evt.member1,"%d",action->actioncode);
	strcpy(tmpevent.member_des1,action->actioninfo);
	tmpevent.evt.ToneOut = action->alarmf;
	strcpy(entname,action->entname);
	
	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存SACDW动作事项");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存SACDW动作事项完毕");

	return OK;
}

