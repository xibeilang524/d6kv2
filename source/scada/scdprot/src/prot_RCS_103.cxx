#include "scadaproc.h"
#include "procthread.h"
#include "prot_RCS_103.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					RCS_103 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



RCS_103_Protect::RCS_103_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *RCS_103_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			)
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取保护动作参数
RCS_103_PROTECTACTION	*RCS_103_Protect::GetActionPara(short actiontype,short actioncode)
{
	ACTION_KEY_RCS_103 key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (RCS_103_PROTECTACTION *)table.RdbGetRecordAddr(RCS_103_PROTACTION_TABNAME,&key);
}


//取自检信息参数
RCS_103_PROTECTDETECT	*RCS_103_Protect::GetDetectPara(short detecttype,short detectcode)
{
	ACTION_KEY_RCS_103 key;
	key.moduletype = detecttype;
	key.actioncode = detectcode;

	return (RCS_103_PROTECTDETECT *)table.RdbGetRecordAddr(RCS_103_PROTACTION_TABNAME,&key);
}


//取保护自检信息描述
int RCS_103_Protect::GetDetectInfo(short detecttype,short detectcode,char *name,char *info)
{
	RCS_103_PROTECTDETECT *detectp = GetDetectPara(detecttype,detectcode);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int RCS_103_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;

	int index =0;

	for (int i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}


	m_GramLength=m_Buf[3+index];									

	address1 = m_Buf[0+index];						//地址1	
	address2 = m_Buf[10+index];

	if (protocoltype == PROTTYPE_FORTYNINE)
	{//串口103
		address1 = m_Buf[5];
		address2 = m_Buf[9];
		index = 5;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到串口103的保护事件邮件");
		mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//微机保护参数
		if(m_Buf[1+index] == 0x02)		//动作事件
		{
			//微机保护动作事件
			GetProtinfoTime(m_Buf+12+index, TIMEBYBCD, &sys_clock);	//发生时间
			RdbDebugMesg("LFP_103邮件时间为 %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成动作事件");
			char text[60];
			if(mdlp == NULL) return PROT_ERR_MODULE;
			sprintf(text,"终端号:%d\t规约类型:%d\t",terminalno,protocoltype);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
			make_LFPaction_apevent(&sys_clock,mdlp,m_Buf);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存动作事件");
			ret = save_LFPaction_apevent(&sys_clock,mdlp,m_Buf);

		}
		else if(m_Buf[1+index] == 0x01)	//自检事件------异常事件
		{
			GetProtinfoTime(m_Buf+8+index, TIMEBYBCD, &sys_clock);	//发生时间
			RdbDebugMesg("LFP_103邮件时间为 %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
			if(mdlp == NULL) return PROT_ERR_MODULE;
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存自检事件");
			ret = save_LFPdetect_apevent(&sys_clock,mdlp,m_Buf);
		}	

		return ret;

	}

	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到RCS_103的保护事件邮件");

	mdlp = GetModulePara(terminalno,protocoltype,address1,address2);			//微机保护参数
	

	if(m_Buf[1+index] == 0x02)		//动作事件
	{
		//微机保护动作事件
		GetProtinfoTime(m_Buf+12+index, TIMEBYBCD, &sys_clock);	//发生时间
		RdbDebugMesg("RCS_103邮件时间为 %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成动作事件");
		char text[60];
		if(mdlp == NULL) return PROT_ERR_MODULE;
		sprintf(text,"终端号:%d\t规约类型:%d\t",terminalno,protocoltype);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
		make_action_apevent(&sys_clock,mdlp,m_Buf);
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存动作事件");
		ret = save_action_apevent(&sys_clock,mdlp,m_Buf);

	}
	else if(m_Buf[1+index] == 0x01)	//自检事件------异常事件
	{
		GetProtinfoTime(m_Buf+8, TIMEBYBCD, &sys_clock);	//发生时间
		RdbDebugMesg("RCS_103邮件时间为 %d-%d-%d %d-%d\n",sys_clock.year,sys_clock.month,sys_clock.day,sys_clock.hour,sys_clock.minute);
		if(mdlp == NULL) return PROT_ERR_MODULE;
		dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存自检事件");
		ret = save_detect_apevent(&sys_clock,mdlp,m_Buf);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void RCS_103_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	RCS_103_PROTECTACTION *RCS_103_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[6]	;			//动作类型

	if (event[7]&0x01 == 1) return;

	if((RCS_103_action = GetActionPara(mdlp->type1,msg.type_value))==NULL)return;	//取动作信息

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYSEVEN);	//公共信息

	msg.sgflag = RCS_103_action->sgflag;
	strcpy(msg.enttypedescribe,RCS_103_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int RCS_103_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	RCS_103_PROTECTACTION *RCS_103_action;
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
	tmpevent.evt.state = event[6];				//动作类型


	if (event[7]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;
	
	if((RCS_103_action = GetActionPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//取动作信息

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//公共信息
	sprintf(tmpevent.evt.member1,"%d",RCS_103_action->actioncode);

	strcpy(tmpevent.member_des1,RCS_103_action->actioninfo);
	tmpevent.evt.ToneOut = RCS_103_action->alarmf;
	strcpy(entname,RCS_103_action->entname);
			
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
int RCS_103_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	RCS_103_PROTECTDETECT *RCS_103_detect;
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

	tmpevent.evt.state = event[6];				//状态值---自检事件类型

	if (event[7]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;

	if((RCS_103_detect = GetDetectPara(mdlp->type1,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",RCS_103_detect->detectcode);

	strcpy(tmpevent.member_des1,RCS_103_detect->detectinfo);

	yield_entdescribe(RCS_103_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = RCS_103_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(RCS_103_detect->entname,&tmpevent);
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


int RCS_103_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;

	time_t ltime;
	struct tm *curtime;
	time(&ltime);
	curtime = localtime(&ltime);
	uchar i=0;
	unsigned short ms=0;
	ms = gram_part[i] + gram_part[i+1]*256;

	clock->year=curtime->tm_year + 1900;
	clock->month=curtime->tm_mon+1;
	clock->day=curtime->tm_mday;
	clock->msecond=ms%1000;
	clock->second=(uchar)(ms/1000);
	clock->minute=gram_part[i+2]&0x3f;
	clock->hour=gram_part[i+3]&0x1f;
	return OK;
}


//取LFP_故障信息	"lfp_action"
LFP_PROTECTACTION* RCS_103_Protect::GetLFPActionPara(short moduletype,unsigned char boardnum,short actbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = boardnum;
	key.flag2 = (unsigned char )actbit;
	return ((LFP_PROTECTACTION *)table.RdbGetRecordAddr(LFP_PROTACTION_TABNAME,&key)); 
}


//取LFP_自检信息	"lfp_detect"
LFP_PROTECTDETECT* RCS_103_Protect::GetLFPDetectPara(short moduletype,unsigned char flagtype,short chkbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = flagtype;
	key.flag2 = (unsigned char )chkbit;
	return ((LFP_PROTECTDETECT *)table.RdbGetRecordAddr(LFP_PROTDETECT_TABNAME,&key)); 
}

//生成保护动作应用事件
void RCS_103_Protect::make_LFPaction_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	LFP_PROTECTACTION *lfp_action;

	if(mdlp == NULL || sysclock == NULL)return;
	
	memset(&msg,0,sizeof(PROTECT_MSG));
	msg.type_value = event[11]	;			//动作类型

	if (event[12]&0x01 == 1) return;

	if((lfp_action = GetLFPActionPara(mdlp->type1,0,msg.type_value))==NULL)return;	//取动作信息

	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FORTYNINE);	//公共信息

	msg.sgflag = lfp_action->sgflag;
	strcpy(msg.enttypedescribe,lfp_action->describe);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int RCS_103_Protect::save_LFPaction_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	LFP_PROTECTACTION *lfp_action;
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
	tmpevent.evt.state = event[11];				//动作类型


	if (event[12]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;
	
	if((lfp_action = GetLFPActionPara(mdlp->type1,0,tmpevent.evt.state))==NULL)return PROT_ERR_ACTION;	//取动作信息

	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);	//公共信息
	sprintf(tmpevent.evt.member1,"%d",lfp_action->actbit);

	strcpy(tmpevent.member_des1,lfp_action->describe);
	tmpevent.evt.ToneOut = lfp_action->alarmf;
	strcpy(entname,lfp_action->entname);
			
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
int RCS_103_Protect::save_LFPdetect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	LFP_PROTECTDETECT *lfp_detect;
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

	tmpevent.evt.state = event[11];				//状态值---自检事件类型

	if (event[12]&0x01==1) return PROT_ERR_TROUBLE;

	tmpevent.evt.val = 1;

	if((lfp_detect = GetLFPDetectPara(mdlp->type1,0,tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;

	sprintf(tmpevent.evt.member1,"%d",lfp_detect->chkbit);

	strcpy(tmpevent.member_des1,lfp_detect->describe);

	yield_entdescribe(lfp_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = lfp_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(lfp_detect->entname,&tmpevent);
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
