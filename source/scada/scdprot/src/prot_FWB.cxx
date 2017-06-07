
///////////////////////////////////////////////////////////////////////////////////
//////////////////////  FWB处理类 /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#include "procthread.h"
#include "prot_FWB.h"


PROTECTMDL *FWB_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) &&(mdlp->address1==address1)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取动作信息
FWB_PROTECTACTION* FWB_Protect::GetActionPara(unsigned short moduletype, short actioncode)
{
	ACTION_KEY1 key;
	key.moduletype = moduletype;
	key.actioncode = actioncode;
	return ((FWB_PROTECTACTION *)table.RdbGetRecordAddr(FWB_PROTACTION_TABNAME,&key)); 
}

//取自检信息
FWB_PROTECTDETECT* FWB_Protect::GetDetectPara(unsigned char chkbit)
{
	return ((FWB_PROTECTDETECT *)table.RdbGetRecordAddr(FWB_PROTDETECT_TABNAME,&chkbit)); 
}


int  FWB_Protect::GetFWBProtinfoTime(uchar *gram_part,SYS_CLOCK *clock)
{      
	GetCurrentClock(clock);
    clock->msecond=gram_part[0]+gram_part[1]*256;
    return OK;
}

int FWB_Protect::GetTime(SYS_CLOCK *clock)
{
   GetCurrentClock(clock);
   return OK;
}


//FEP服务器保护生报文解释
int FWB_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SYS_CLOCK sys_clock;
	PROTECTMDL	*mdlp;	//微机保护参数指针
	int ret ;

	if(gram_len<5) return PROT_ERR_MAILLEN;

    mdlp = GetModulePara(terminalno,protocoltype,gram[2]);  // gram[2]为地址码
    if(mdlp == NULL)return PROT_ERR_MODULE;

	if(gram[4] == 0)		// 故障事件上送---简单事件报告
	{       
		GetFWBProtinfoTime(gram+7,&sys_clock);		//发生时间
		make_action_apevent(&sys_clock,mdlp,gram+5);
		ret = save_action_apevent(&sys_clock,mdlp,gram+5);
	}
	else if(gram[4] == 0x0f)		//自检报告
	{
		GetTime(&sys_clock);
		ret = save_detect_apevent(&sys_clock,mdlp,gram+6);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}


//生成保护动作应用事件
void FWB_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	FWB_PROTECTACTION *fwb_action;

	if(mdlp == NULL || sysclock == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FWB);	//公共信息
	msg.type_value = event[0];			//动作类型
	msg.event_float_info[0] = 0.0f;
	
	if((fwb_action = GetActionPara(mdlp->type2, msg.type_value))==NULL)return;	//取动作信息
	msg.sgflag = fwb_action->sgflag;
	strcpy(msg.enttypedescribe,fwb_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int FWB_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];

	entname[0] = 0;
	FWB_PROTECTACTION *fwb_action;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////

	tmpevent.evt.state = event[0];			//动作类型-----状态值
	tmpevent.evt.val = 0.0f;	//浮点值
	fwb_action = GetActionPara(mdlp->type2,event[0]);	//取动作信息
	if(fwb_action) {
		sprintf(tmpevent.evt.member1,"%d",fwb_action->actioncode);
		strcpy(tmpevent.member_des1,fwb_action->actioninfo);
		tmpevent.evt.ToneOut = fwb_action->alarmf;
		strcpy(entname,fwb_action->entname);
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


int FWB_Protect::save_detect_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)	//保存自检事件
{ 
	SCDEVENT tmpevent;	//存盘事件
	int i = 0,bitnums;
	unsigned short eventinfo;
	uchar chkbit;
	FWB_PROTECTDETECT *fwb_detect;
         
	chkbit=-1;
    bitnums=5;
	eventinfo = event[0];
	for(i=0;i<bitnums;i++)
	{
		if(eventinfo & 0x01) {
			eventinfo = eventinfo>>1;
			chkbit = i;
		}
		else {
			eventinfo = eventinfo>>1;
			continue;
		}
		fwb_detect = GetDetectPara(chkbit);
		if(fwb_detect == NULL) continue ;
        memset(&tmpevent,0,sizeof(SCDEVENT));
        make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);

        tmpevent.evt.DiskOut = 1;                  //存盘标志
        tmpevent.evt.PrintOut = 1;                 //打印标志
        tmpevent.evt.DispOut = 1;                  //登录标志
		tmpevent.evt.ToneOut = fwb_detect->alarmf;		//音响报警标志

//		sprintf(tmpevent.evt.member1,"%d",fwb_detect->detectcode);//对象名1--动作类型
		strcpy(tmpevent.member_des1,fwb_detect->detectinfo);	//对象名1描述---动作描述
		yield_entdescribe(fwb_detect->entname,&tmpevent);
		if(tmpevent.evt.ToneOut)yield_snddescribe(fwb_detect->entname, &tmpevent);
		//2003 03
		NetStatus netcfg;
		int groupnum=netcfg.GetHostGrpNum();
		int *grp = netcfg.GetHostGrpEnm();
		for(int no=0;no<groupnum;no++)
		{
			tmpevent.evt.grpno=grp[no];
			save_event(&tmpevent,1);
		}//for--no
	}
	return OK;
}

