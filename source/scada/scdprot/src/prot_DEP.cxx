#include "scadaproc.h"
#include "procthread.h"
#include "prot_DEP.h"

				//////////////////////////////////////////////////////////////
				/*					DEP(LSA_P) 微机保护处理类					*/
				//////////////////////////////////////////////////////////////

Rdb_Tab DEP_Protect::rdbtable(MyUserName,MyPassWord);
int DEP_Protect::opentabf;


DEP_Protect::DEP_Protect(void)
{

}

PROTECTMDL *DEP_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
DEP_PROTECTACTION	*DEP_Protect::GetActionPara(short actioncode)
{
	return (DEP_PROTECTACTION *)table.RdbGetRecordAddr(DEP_PROTACTION_TABNAME,&actioncode);
}

//取故障信息参数
DEP_PROTECTTROUBLE	*DEP_Protect::GetTroublePara(short troublecode)
{
	return (DEP_PROTECTTROUBLE *)table.RdbGetRecordAddr(DEP_PROTTRBL_TABNAME,&troublecode);
}

//取测量值信息参数
DEP_PROTECTMEASURE	*DEP_Protect::GetMeasurePara(short measurecode)
{
	return (DEP_PROTECTMEASURE *)table.RdbGetRecordAddr(DEP_PROTMSR_TABNAME,&measurecode);
}

//取自检信息参数
DEP_PROTECTDETECT	*DEP_Protect::GetDetectPara(short detectcode)
{
	return (DEP_PROTECTDETECT *)table.RdbGetRecordAddr(DEP_PROTDETECT_TABNAME,&detectcode);
}


//取保护动作描述
int DEP_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	DEP_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//取保护自检信息描述
int DEP_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	DEP_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//取保护故障描述
int DEP_Protect::GetTroubleInfo(short troubletype,char *info)
{
	DEP_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int DEP_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	GetProtinfoTime(gram+2, TIMEBYBCD, &sys_clock);	//发生时间
	mdlp = GetModulePara(terminalno,protocoltype,gram[11],0);			//微机保护参数

	if(gram[0] == 0x40)		//动作事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else if(gram[0] == 0x51)	//自检事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void DEP_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	DEP_PROTECTACTION *DEP_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DEP);	//公共信息
	msg.type_value = event[3];			//动作类型
	msg.event_int_info[0] = event[0];	//事件编号
	msg.event_int_info[1] = event[1];	//该动作类型累计发生数
	msg.event_int_info[2] = event[2];	//事故类型
	msg.event_int_info[3] = event[3];	//动作类型
	msg.event_int_info[4] = event[4];	//测量值类型
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((DEP_action = GetActionPara(msg.type_value))==NULL)return;	//取动作信息
	msg.sgflag = DEP_action->sgflag;
	strcpy(msg.enttypedescribe,DEP_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int DEP_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN],longdes[120];
	DEP_PROTECTACTION *DEP_action;

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

	tmpevent.evt.state = event[3];			//动作类型-----状态值
	tmpevent.evt.val = (float)(event[5]+event[6]*256)/100.0f;	//浮点值
	if((DEP_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",DEP_action->actioncode);
	strcpy(tmpevent.member_des1,DEP_action->actioninfo);
	tmpevent.evt.ToneOut = DEP_action->alarmf;
	strcpy(entname,DEP_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//故障

	DEP_PROTECTMEASURE *DEP_measure = GetMeasurePara(event[4]);
	if(DEP_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			DEP_measure->measureinfo,(float)tmpevent.evt.val,DEP_measure->measureunit);
		strcat(tmpevent.longdes,longdes);
	}

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
int DEP_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	DEP_PROTECTDETECT *DEP_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = event[0];	//状态值---自检事件类型
	if((DEP_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",DEP_detect->detectcode);
	strcpy(tmpevent.member_des1,DEP_detect->detectinfo);

	yield_entdescribe(DEP_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = DEP_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(DEP_detect->entname,&tmpevent);
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

int DEP_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	clock->year=gram_part[0]+gram_part[1]*256;
	clock->month=gram_part[2];
	clock->day=gram_part[3];
	clock->hour=gram_part[4];
	clock->minute=gram_part[5];
	clock->second=gram_part[6];
	long mSec;
	mSec=gram_part[7]+gram_part[8]*256;
	clock->msecond=mSec/10;
	return OK;
}

