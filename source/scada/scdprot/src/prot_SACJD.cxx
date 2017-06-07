#include "scadaproc.h"
#include "procthread.h"
#include "prot_SACJD.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					SACJD 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



SACJD_Protect::SACJD_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *SACJD_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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
SACJD_PROTECTACTION	*SACJD_Protect::GetActionPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACJD key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACJD_PROTECTACTION *)table.RdbGetRecordAddr(SACJD_PROTACTION_TABNAME,&key);
}

//取自诊断信息参数
SACJD_PROTECTACTION	*SACJD_Protect::GetDetectPara(short actioncode,short actiontype)
{
	ACTION_KEY_SACJD key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (SACJD_PROTECTACTION *)table.RdbGetRecordAddr(SACJD_PROTDETECT_TABNAME,&key);
}


//解释FEP服务器保护生报文
int SACJD_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到SACJD的保护事件邮件");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	address1=m_Buf[1];	
	
	//主站地址	对应微机保护参数保护参数表中得地址1
	mdlp = GetModulePara(terminalno,protocoltype,address1);			//微机保护参数
	
	//微机保护动作事件
	char text[60];
	sprintf(text,"终端号:%d\t规约类型:%d\t地址:%d\n",terminalno,protocoltype,address1);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
	if(mdlp == NULL)return PROT_ERR_MODULE;
	if (m_Buf[14] == 0x27)
	{
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成SACJD动作事件");
	make_action_apevent(&sys_clock,mdlp,m_Buf);
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存SACJD动作事件");
	ret = save_action_apevent(&sys_clock,mdlp,m_Buf);
	}
	else if (m_Buf[14] == 0x28)
	{
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}

	return ret;
}

//生成保护动作应用事件
void SACJD_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	SACJD_PROTECTACTION *action;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int second,ms,minute,hour;

	if(mdlp == NULL )return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));

	actioncode=event[16];	//动作序号

	ms = event[11]+event[12]*256;
	second = event[10];
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;

	msg.type_value = (int)actioncode;
	make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_SACJD);	//公共信息
	actiontype = event[15];
	if((action = GetActionPara(actioncode,actiontype))==NULL)return;	//取动作信息
	msg.sgflag = action->sgflag;
	strcpy(msg.enttypedescribe,action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//动作事项存盘
int SACJD_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	SACJD_PROTECTACTION *action;
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
	if(mdlp == NULL )return 0;
	

	actioncode=event[16];	//动作序号

	ms = event[11]+event[12]*256;
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;

	actiontype = event[15];//设备类型

	/////////生成存盘事项//////////////////////////////////////////////////
	tmpevent.evt.state = actioncode;	//动作序号
	tmpevent.evt.state = (int)actioncode;
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);	//公共信息
	if((action = GetActionPara(actioncode,actiontype))==NULL)return PROT_ERR_ACTION;	//取动作信息
	sprintf(tmpevent.evt.member1,"%d",action->actioncode);
	strcpy(tmpevent.member_des1,action->actioninfo);
	tmpevent.evt.ToneOut = action->alarmf;
	strcpy(entname,action->entname);
	
	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存SACJD动作事项");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存SACJD动作事项完毕");

	return OK;
}

//自检事项存盘
int SACJD_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	SACJD_PROTECTACTION *detect;
	NetDbg dbg;
	short	actioncode,actiontype;
	SYS_CLOCK  sys_clock;
	int ms,minute,hour;
	if(mdlp == NULL )return 0;

	actioncode=event[16];	//动作序号

	ms = event[11]+event[12]*256;
	minute = event[9];
	hour = event[8];
	//GetCurrentClock(&sys_clock);
	sys_clock.year = event[4]+event[5]*100;
	sys_clock.month = event[6];
	sys_clock.day = event[7];
	sys_clock.hour =hour;
	sys_clock.minute =minute;
	sys_clock.second =event[10];
	sys_clock.msecond =ms;


	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;


	actiontype = event[15];//设备类型
	if((detect = GetDetectPara(actioncode,actiontype))==NULL)
		return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",detect->actioncode);
	strcpy(tmpevent.member_des1,detect->actioninfo);

	yield_entdescribe(detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(detect->entname,&tmpevent);

	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存自检事项");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存自检事项完毕");

	return OK;
}
