#include "scadaproc.h"
#include "procthread.h"
#include "prot_NSR.h"

				//////////////////////////////////////////////////////////////
				/*					NSR(LSA_P) 微机保护处理类					*/
				//////////////////////////////////////////////////////////////


NSR_Protect::NSR_Protect(void)
{

}

PROTECTMDL *NSR_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) 
			&& (mdlp->address2==address2)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取保护动作参数
NSR_PROTECTACTION	*NSR_Protect::GetActionPara(short actioncode)
{
	return (NSR_PROTECTACTION *)table.RdbGetRecordAddr(NSR_PROTACTION_TABNAME,&actioncode);
}

//取故障信息参数
NSR_PROTECTTROUBLE	*NSR_Protect::GetTroublePara(short troublecode)
{
	return (NSR_PROTECTTROUBLE *)table.RdbGetRecordAddr(NSR_PROTTRBL_TABNAME,&troublecode);
}

//取测量值信息参数
NSR_PROTECTMEASURE	*NSR_Protect::GetMeasurePara(short measurecode)
{
	return (NSR_PROTECTMEASURE *)table.RdbGetRecordAddr(NSR_PROTMSR_TABNAME,&measurecode);
}

//取自检信息参数
NSR_PROTECTDETECT	*NSR_Protect::GetDetectPara(short detectcode)
{
	return (NSR_PROTECTDETECT *)table.RdbGetRecordAddr(NSR_PROTDETECT_TABNAME,&detectcode);
}

//取保护动作描述
int NSR_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	NSR_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//取保护自检信息描述
int NSR_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	NSR_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//取保护故障描述
int NSR_Protect::GetTroubleInfo(short troubletype,char *info)
{
	NSR_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int NSR_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;

	if(gram_len<24) return PROT_ERR_MAILLEN;

	GetProtinfoTime(gram+12, TIMEBYBCD, &sys_clock);	//发生时间
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[1]);			//微机保护参数

	if(gram[8] == 0x70)		//动作事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+20);
		ret = save_action_apevent(&sys_clock,mdlp,gram+20);
		
	}
	else if(gram[8] == 0x97)	//自检事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+20);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void NSR_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	NSR_PROTECTACTION *NSR_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_NSR);	//公共信息
	msg.type_value = event[0]+event[1]*256;			//动作类型
	msg.event_float_info[0] = (float)(event[2]+event[3]*256);
	
	if((NSR_action = GetActionPara(msg.type_value))==NULL)return;	//取动作信息
	msg.sgflag = NSR_action->sgflag;
	strcpy(msg.enttypedescribe,NSR_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int NSR_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	NSR_PROTECTACTION *NSR_action;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////

	tmpevent.evt.state = event[0]+event[1]*256;			//动作类型-----状态值
	tmpevent.evt.val = (float)(event[2]+event[3]*256);	
	if((NSR_action = GetActionPara(tmpevent.evt.state)) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",NSR_action->actioncode);
	strcpy(tmpevent.member_des1,NSR_action->actioninfo);
	tmpevent.evt.ToneOut = NSR_action->alarmf;
	strcpy(entname,NSR_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//故障

/*	NSR_PROTECTMEASURE *NSR_measure = GetMeasurePara(event[4]);
	if(NSR_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			NSR_measure->measureinfo,(float)tmpevent.evt.val,NSR_measure->measureunit);
		strcat(tmpevent.longdes,longdes);
	}
*/
	yield_entdescribe(entname,&tmpevent);
	if (tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	return OK;
}

//自检事项存盘
int NSR_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	NSR_PROTECTDETECT *NSR_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[0]+event[1]*256;	//状态值---自检事件类型
	if((NSR_detect = GetDetectPara(tmpevent.evt.state))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",NSR_detect->detectcode);
	strcpy(tmpevent.member_des1,NSR_detect->detectinfo);

	yield_entdescribe(NSR_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = NSR_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(NSR_detect->entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}//for--no
	return OK;
}

int NSR_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	if (gram_part == NULL) return -1;
	clock->year=2000+gram_part[0];
	clock->month=gram_part[1];
	clock->day=gram_part[2];
	clock->hour=gram_part[3];
	clock->minute=gram_part[4];
	clock->second=gram_part[5];
	clock->msecond=gram_part[6]+gram_part[7]*256;
	return OK;
}
