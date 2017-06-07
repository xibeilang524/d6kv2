#include "scadaproc.h"
#include "procthread.h"
#include "prot_WBX.h"

				//////////////////////////////////////////////////////////////
				/*					WBX 微机保护处理类					*/
				//////////////////////////////////////////////////////////////

WBX_Protect::WBX_Protect(void)
{

}

PROTECTMDL *WBX_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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

//取保护动作参数
WBX_PROTECTACTION	*WBX_Protect::GetActionPara(ushort actiontype,ushort actioncode)
{
	struct ACTION_KEY{
		ushort actiontype;
		ushort actioncode;
	}actionkey;
	actionkey.actiontype = actiontype;
	actionkey.actioncode = actioncode;
	return (WBX_PROTECTACTION *)table.RdbGetRecordAddr(WBX_PROTACTION_TABNAME,&actionkey);
}


//解释FEP服务器保护生报文
int WBX_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[5] = gram[4];
	GetProtinfoTime(gram+5, TIMEBYBCD, &sys_clock);	//发生时间
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[3]);			//微机保护参数
			//gram[0]保护地址，gram[3]CPU号
	if(gram[1] == 0xb0)		//动作事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		make_action_apevent(&sys_clock,mdlp,gram+13);
		ret = save_action_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void WBX_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	WBX_PROTECTACTION *WBX_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_WBX);	//公共信息
	msg.type_value = event[0];			//动作类型
	
	if((WBX_action = GetActionPara(event[0],event[1]))==NULL)return;	//取动作信息
	msg.sgflag = WBX_action->sgflag;
	strcpy(msg.enttypedescribe,WBX_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}

//动作事项存盘
int WBX_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	int i = 0;

	entname[0] = 0;

	WBX_PROTECTACTION *WBX_action;

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

	WBX_action = GetActionPara(event[0],event[1]);	//取动作信息
	if(WBX_action) {
		sprintf(tmpevent.evt.member1,"%d",WBX_action->actioncode);
		strcpy(tmpevent.member_des1,WBX_action->actioninfo);
		tmpevent.evt.ToneOut = WBX_action->alarmf;
		strcpy(entname,WBX_action->entname);
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
	}
	return OK;
}

