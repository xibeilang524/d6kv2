#include "procthread.h"
#include "prot_DFP.h"

///////////////////////////////////////////////////////////////////////////////////
//////////////////////  DFP处理类 /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


DFP_Protect::DFP_Protect(void)
{
	return;
}

PROTECTMDL *DFP_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)
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

DFP_PROTECTACTION	*DFP_Protect::GetActionPara(uchar actbit)
{
	return ((DFP_PROTECTACTION *)table.RdbGetRecordAddr(DFP_PROTACTION_TABNAME,&actbit));
}

DFP_PROTECTDETECT	*DFP_Protect::GetDetectPara(uchar boardtype,uchar chkbit)
{
	struct ACTION_KEY{
		uchar boardtype;
		uchar chkbit;
	}action_key;
	action_key.boardtype = boardtype;
	action_key.chkbit = chkbit;
	return ((DFP_PROTECTDETECT *)table.RdbGetRecordAddr(DFP_PROTDETECT_TABNAME,&action_key));
}

int DFP_Protect::GetActionInfo(uchar actbit,char *name,char *info)
{
	DFP_PROTECTACTION *actionp;
	actionp=GetActionPara(actbit);
	if(actionp==NULL) return SCADA_ERR_NOOBJECT;
	if(name!=NULL)strcpy(name,actionp->actioninfo);
	if(info!=NULL)strcpy(info,actionp->actioninfo);
	return(OK);
}

int DFP_Protect::GetDetectInfo(uchar boardtype,uchar chkbit,char *name,char *info)
{
	DFP_PROTECTDETECT *detectp;
	detectp=GetDetectPara(boardtype,chkbit);
	if(detectp==NULL) return SCADA_ERR_NOOBJECT;
	if(name!=NULL)strcpy(name,detectp->detectinfo);
	if(info!=NULL)strcpy(info,detectp->detectinfo);
	return(OK);
}

int DFP_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SYS_CLOCK sys_clock;
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	PROTECTMDL	*mdlp = GetModulePara(terminalno,protocoltype,gram[0]);			//gram[0]为地址码
	if(mdlp==NULL) return PROT_ERR_MODULE;

	if(gram[2]==0x45){	//保护跳闸报告
		GetProtinfoTime(gram+7, TIMEBYBCD, &sys_clock);
		sys_clock.msecond = 0;
		make_action_apevent(&sys_clock, mdlp, gram+25 );
		ret = save_action_apevent(&sys_clock, mdlp, gram+25 );
	}
	else if(gram[2]==0x46){			//自检报告
		GetProtinfoTime(gram+5, TIMEBYBCD, &sys_clock);
		sys_clock.msecond = 0;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+11);
	}
	else if(gram[2]==0x49){			//硬件故障报告
		GetProtinfoTime(gram+7, TIMEBYBCD, &sys_clock);
		sys_clock.msecond = 0;
		ret = save_trouble_apevent(&sys_clock,mdlp,gram+6);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}


void DFP_Protect::make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)		//形成动作事件
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	unsigned short eventinfo;
	uchar actbit;
	int i,j;

	short troubletype;
	DFP_PROTECTACTION *dfp_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DFP);
	
	msg.event_float_info[0] = (float)(event[16] + event[17]*256);				//重合闸时间
	msg.event_float_info[1] = (float)(event[20] + event[21]*256)/10.0f;			//测距值
	msg.event_float_info[2] = (float)(event[22] + event[23]*256)/100.0f;		//最大零序电流
	msg.event_float_info[3] = (float)(event[24] + event[25]*256)/100.0f;		//最大相电流
	troubletype = event[26];
	for(i=0;i<3;i++)
	{
        msg.clock.msecond = event[i*4+2]+event[i*4+3]*256;
		eventinfo = event[i*4] + event[i*4+1]*256;		//gram[25+i*4]+gram[26+i*4]*256;
		for(j=0;j<16;j++)
		{
			if(eventinfo & 0x0001) {
				eventinfo = eventinfo >> 1;
				actbit = j;
			}
			else {
				eventinfo = eventinfo >>1;
				continue;
			}
			msg.type_value = actbit;

			dfp_action = (DFP_PROTECTACTION *)GetActionPara(actbit);
			if(dfp_action == NULL) continue;
			strcpy(msg.enttypedescribe,dfp_action->actioninfo);
			msg.sgflag = dfp_action->sgflag;
			apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
		}
	}
	return;
}

int DFP_Protect::save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)			//保存动作事件
{
	SCDEVENT tmpevent;	//存盘事件
	unsigned short eventinfo;
	uchar actbit;
	int i,j;
	DFP_PROTECTACTION *dfp_action;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	tmpevent.evt.val = (float)(event[16] + event[17]*256);				//重合闸时间
	tmpevent.evt.val1 = (float)(event[20] + event[21]*256)/10.0f;		//测距值
	if(event[21] & 0x80)
		tmpevent.evt.val2 = (float)(event[22] + event[23]*256)/100.0f;		//最大零序电流FMT1格式
	else
		tmpevent.evt.val2 = (float)(event[22] + event[23]*256)/1000.0f;		
	if(event[25] & 0x80)
		tmpevent.evt.val3 = (float)(event[24] + event[25]*256)/100.0f;		//最大相电流FMT1格式
	else
		tmpevent.evt.val3 = (float)(event[24] + event[25]*256)/1000.0f;		
	sprintf(tmpevent.otherdes0,"重合闸时间:%4.0fms",(float)tmpevent.evt.val);
	sprintf(tmpevent.otherdes1,"测距值:%4.2fkm",(float)tmpevent.evt.val1);
	sprintf(tmpevent.otherdes2,"I0max:%4.2fA",(float)tmpevent.evt.val2);
	sprintf(tmpevent.otherdes3,"最大相电流:%4.2fA",(float)tmpevent.evt.val3);

	strcpy(tmpevent.member_des2,"选相:");
    if(event[26] & 0x80)
    {
      if((event[26] & 0x10) && (event[26] & 0x20) && (event[26] & 0x40)) 
                  strcat(tmpevent.member_des2,"ABC");
	  else if(event[26] & 0x40) strcat(tmpevent.member_des2,"AB");
	  else if(event[26] & 0x20) strcat(tmpevent.member_des2,"BC");
	  else if(event[26] & 0x10) strcat(tmpevent.member_des2,"CA");
	  else strcat(tmpevent.member_des2,"/");
    }
    else if(event[26] & 0x08)
    {
	  if(event[26] & 0x04) strcat(tmpevent.member_des2,"A");
	  else if(event[26] & 0x02) strcat(tmpevent.member_des2,"B");
	  else if(event[26] & 0x01) strcat(tmpevent.member_des2,"C");
	  else strcat(tmpevent.member_des2,"/");
    }
	else strcat(tmpevent.member_des2,"/");

	for(i=0;i<3;i++)
	{
        tmpevent.evt.hmsms = (tmpevent.evt.hmsms /1000)*1000 + event[i*4+2]+event[i*4+3]*256;
		eventinfo = event[i*4] + event[i*4+1]*256;
		for(j=0;j<16;j++)
		{
			if(eventinfo & 0x0001)
			{
				eventinfo = eventinfo >> 1;
				actbit = j;
			}
			else {
				eventinfo = eventinfo >>1;
				continue;
			}
			tmpevent.evt.state = actbit;
			dfp_action = (DFP_PROTECTACTION *)GetActionPara(actbit);
			if(dfp_action == NULL) continue;
			tmpevent.evt.ToneOut = dfp_action->alarmf;
			strcpy(tmpevent.member_des1,dfp_action->actioninfo);
			yield_entdescribe(dfp_action->entname,&tmpevent);
			if(tmpevent.evt.ToneOut)yield_snddescribe(dfp_action->entname,&tmpevent);
			//2003 03
			NetStatus netcfg;
			int groupnum=netcfg.GetHostGrpNum();
			int *grp = netcfg.GetHostGrpEnm();
			for(int no=0;no<groupnum;no++)
			{
				tmpevent.evt.grpno=grp[no];
				save_event(&tmpevent,1);
			}//for--no
		}	//for--j
	}	//for--i
	return OK;
}


int DFP_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)	//保存自检事件
{
	SCDEVENT tmpevent;	//存盘事件
	unsigned short eventinfo,val;
	uchar boardtype,chkbit;
	int i,j;
	DFP_PROTECTDETECT *dfp_detect;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	for(i=0; i<3; i++)
	{
		val = event[i*4] + event[i*4+1]*256;
		eventinfo = event[i*4+2] + event[i*4+3]*256;
		j=0;
		while(j<16)
		{
			if(val & 0x0001) //对应自检项有效位为1
			{
				if(eventinfo & 0x0001)
				{
					//生成事件
					chkbit = j ;
					boardtype = i;
					dfp_detect = (DFP_PROTECTDETECT *)GetDetectPara(boardtype,chkbit);
					if(dfp_detect){
					tmpevent.evt.ToneOut = dfp_detect->alarmf;		//音响报警标志
					strcpy(tmpevent.member_des1,dfp_detect->detectinfo);			//对象名1描述---动作描述
					yield_entdescribe(dfp_detect->entname,&tmpevent);
					if(tmpevent.evt.ToneOut)yield_snddescribe(dfp_detect->entname,&tmpevent);
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
				}
			}
			val = val >> 1;
			eventinfo = eventinfo >> 1;
			j++;
		}	//while
	}	//for
	return OK;
}

int DFP_Protect::save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)	//保存硬件故障报告
{
	SCDEVENT tmpevent;	//存盘事件
	unsigned short eventinfo,val;
	uchar troublebit,boardtype;
	int j;
	DFP_PROTECTDETECT *dfp_trouble;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_TROUBLE;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	val = event[7] + event[8]*256;
	eventinfo = event[9] + event[10]*256;

	j=0;
	while(j<16)
	{
		if(val & 0x0001) //对应故障项有效位为1
		{
			if(eventinfo & 0x0001)
			{
				//生成事件
				troublebit = j ;
				boardtype = event[0];
				dfp_trouble = (DFP_PROTECTDETECT *)GetDetectPara(boardtype,troublebit);
				if(dfp_trouble){
					tmpevent.evt.ToneOut = dfp_trouble->alarmf;
					strcpy(tmpevent.member_des1,dfp_trouble->detectinfo);
					yield_entdescribe(dfp_trouble->entname,&tmpevent);
					if(tmpevent.evt.ToneOut)yield_snddescribe(dfp_trouble->entname,&tmpevent);
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
			}	
		}	
		val = val >> 1;
		eventinfo = eventinfo >> 1;
		j++;
	}	//while
	return OK;
}
