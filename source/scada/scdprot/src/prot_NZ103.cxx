#include "scadaproc.h"
#include "procthread.h"
#include "prot_NZ103.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					NZ103 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



NZ103_Protect::NZ103_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *NZ103_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取保护动作参数
NZ103_PROTECTACTION	*NZ103_Protect::GetActionPara(short devicetype,short actioncode)
{
	ACTION_KEY_NZ103 key;
	key.moduletype = devicetype;
	key.actioncode = actioncode;
	return (NZ103_PROTECTACTION *)table.RdbGetRecordAddr(NZ103_PROTACTION_TABNAME,&key);
}


//取自检信息参数
NZ103_PROTECTDETECT	*NZ103_Protect::GetDetectPara(short devicetype,short detectcode)
{
	ACTION_KEY_NZ103 key;
	key.moduletype = devicetype;
	key.actioncode = detectcode;
	return (NZ103_PROTECTDETECT *)table.RdbGetRecordAddr(NZ103_PROTDETECT_TABNAME,&key);
}



//取保护自检信息描述
int NZ103_Protect::GetDetectInfo(short devicetype,short detecttype,char *name,char *info)
{
	NZ103_PROTECTDETECT *detectp = GetDetectPara(devicetype,detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int NZ103_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到NZ103的保护事件邮件");

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	m_GramLength=m_Buf[3];									
	GetProtinfoTime(m_Buf, TIMEBYBCD, &sys_clock);	//发生时间
	RdbDebugMesg("NZ103邮件时间为 %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);

	address1=1;						//地址1	

	mdlp = GetModulePara(terminalno,protocoltype,address1);			//微机保护参数
	

	if(m_Buf[0] == 0x04)		//动作事件
	{
		//微机保护动作事件
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成动作事件");
		char text[60];
		if(mdlp == NULL) return PROT_ERR_MODULE;
		sprintf(text,"终端号:%d\t规约类型:%d\t",terminalno,protocoltype);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		make_action_apevent(&sys_clock,mdlp,m_Buf);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存动作事件");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	}
	else if(m_Buf[0] == 0x05)	//自检事件------异常事件
	{
		if(mdlp == NULL) return PROT_ERR_MODULE;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存自检事件");
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void NZ103_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	NZ103_PROTECTACTION *NZ103_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[3]	;			//动作类型
	if((NZ103_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//取动作信息

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYFIVE);	//公共信息

	msg.sgflag = NZ103_action->sgflag;
	strcpy(msg.enttypedescribe,NZ103_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));

}


//动作事项存盘
int NZ103_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	NZ103_PROTECTACTION *NZ103_action;
	NetDbg dbg;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	//make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////
	tmpevent.evt.state = event[3];		//动作类型
	tmpevent.evt.val = event[4];		//
	
	if((NZ103_action = GetActionPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//取动作信息

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//公共信息
	sprintf(tmpevent.evt.member1,"%d",NZ103_action->actioncode);

	strcpy(tmpevent.member_des1,NZ103_action->actioninfo);
	tmpevent.evt.ToneOut = NZ103_action->alarmf;
	strcpy(entname,NZ103_action->entname);
			

	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存动作事项");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"保存动作事项完毕");

	return OK;
}

//自检事项存盘
int NZ103_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	NZ103_PROTECTDETECT *NZ103_detect;
	NetDbg dbg;
	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[3];				//状态值---自检事件类型

	tmpevent.evt.val = event[4];

	if((NZ103_detect = GetDetectPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",NZ103_detect->detectcode);

	strcpy(tmpevent.member_des1,NZ103_detect->detectinfo);

	yield_entdescribe(NZ103_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = NZ103_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(NZ103_detect->entname,&tmpevent);
		//2003 03
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"开始保存自检事项");
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"结束保存自检事项");

	return OK;
}


int NZ103_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;

	uchar i=5;
	clock->year=gram_part[i++]+1900;
	clock->month=gram_part[i++];
	clock->day=gram_part[i++];
	clock->hour=gram_part[i++];
	clock->minute=gram_part[i++];
	clock->second=gram_part[i++];
	clock->msecond=gram_part[i]+gram_part[i+1]*256;
	return OK;
}