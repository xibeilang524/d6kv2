#include "scadaproc.h"
#include "procthread.h"
#include "prot_DWK.h"

				//////////////////////////////////////////////////////////////
				/*					DWK(LSA_P) 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



PROTECTMDL *DWK_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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
DWK_PROTECTACTION	*DWK_Protect::GetActionPara(short actioncode)
{
	return (DWK_PROTECTACTION *)table.RdbGetRecordAddr(DWK_PROTACTION_TABNAME,&actioncode);
}

//取故障信息参数
DWK_PROTECTTROUBLE	*DWK_Protect::GetTroublePara(short troublecode)
{
	return (DWK_PROTECTTROUBLE *)table.RdbGetRecordAddr(DWK_PROTTRBL_TABNAME,&troublecode);
}


//取保护动作描述
int DWK_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	DWK_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}


//取保护故障描述
int DWK_Protect::GetTroubleInfo(short troubletype,char *info)
{
	DWK_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int DWK_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	GetProtinfoTime(gram+3, TIMEBYBIN, &sys_clock,-1,1);	//发生时间
	mdlp = GetModulePara(terminalno,protocoltype,gram[0]);			//微机保护参数

	if(gram[1] == 0x21)		//动作报告
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_action_apevent(&sys_clock,mdlp,gram+10);
	}
	else if(gram[1] == 0x20)	//故障报告
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_trouble_apevent(&sys_clock,mdlp,gram+10,0);
	}
	else if(gram[1] == 0x22)	//故障解除报告
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_trouble_apevent(&sys_clock,mdlp,gram+10,1);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}


//动作事项存盘
int DWK_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	DWK_PROTECTACTION *dwk_action;

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

	int tmpactioncode=event[0] + event[1] * 256;
	tmpevent.evt.state = tmpactioncode;			//动作类型-----状态值
	if((dwk_action = GetActionPara(tmpactioncode)) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",dwk_action->actioncode);
	strcpy(tmpevent.member_des1,dwk_action->actioninfo);
	tmpevent.evt.ToneOut = dwk_action->alarmf;
	strcpy(entname,dwk_action->entname);

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
int DWK_Protect::save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event,int restoreflag)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	DWK_PROTECTTROUBLE *dwk_trouble;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 2);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	int tmptroublecode=event[0] + event[1] * 256;
	tmpevent.evt.state = tmptroublecode;	//状态值---自检事件类型
	if((dwk_trouble = GetTroublePara(tmptroublecode))==NULL)return PROT_ERR_TROUBLE;
	sprintf(tmpevent.evt.member1,"%d",dwk_trouble->troublecode);
	strcpy(tmpevent.member_des1,dwk_trouble->troubleinfo);

	tmpevent.evt.val1 = (float)restoreflag;
	if(restoreflag)strcpy(tmpevent.otherdes0,"故障解除");
	else 	if(restoreflag)strcpy(tmpevent.otherdes0,"故障");


	strcpy(entname,dwk_trouble->entname);
	yield_entdescribe(entname,&tmpevent);
	tmpevent.evt.ToneOut = dwk_trouble->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(entname,&tmpevent);
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

