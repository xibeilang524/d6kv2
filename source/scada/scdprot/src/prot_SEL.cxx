///////////////////////////////////////////////////////////////////////////////////
//////////////////////  SEL处理类 /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#include "procthread.h"
#include "prot_SEL.h"

Rdb_Tab SEL_Protect::protacttab;
int SEL_Protect::opentabf;

SEL_Protect::SEL_Protect(void)
{
	if(opentabf==0)
	{
		opentabf=2;
		if(protacttab.RdbLogin(MyUserName,MyPassWord)!=RDB_OK){opentabf=0;return;}
		if(protacttab.RdbOpenTab(SEL_PROTACTION_TABNAME)!=RDB_OK){opentabf=0;return;}
		opentabf=1;
	}
	return;
}

PROTECTMDL *SEL_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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

int SEL_Protect::GetActionType(short devtype,short moduletype,char *action_string,char *target,ushort *actiontype, int totalact_nums)
{
	int devno;
	char event[16];
	char *str, *str1, *str2, *str3, *str4;
	devno = devtype;
	moduletype = moduletype;
	strcpy(event,action_string);
	strcpy(target,target);
	if(devno == PDEVTYPE_SEL_251)	//SEL251
	{
		str=strstr(event,"T");
		str1=strstr(event,"TR");
		str2=strstr(event,"DT");
		str3=strstr(event,"ET");
		if(str && (!str1) && (!str2) && (!str3))	{*actiontype=0; return(OK);}

		switch(moduletype)	//模块类型（保护类型）
		{
		case PTYPE_MTBACK_110KV:	//主变后备保护(110KV侧)		
			str=strstr(event,"ER");
			str1=strstr(event,"A");
			str2=strstr(event,"B");
			str3=strstr(event,"C");
			str4=strstr(event,"T");
			if(str || ( (str1 || str2 || str3) && (!str4)))	{*actiontype=1; return(OK);}

			str=strstr(event,"ET");
			if(str)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_MTBACK_35KV:		//主变后备保护(35KV侧)	
			str=strstr(event,"DT");
			if(str)	{*actiontype=1; return(OK);}

			str=strstr(event,"ER");
			if(str)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_MTBACK_10KV:		//主变后备保护(10KV侧)	
			str=strstr(event,"ER");
			if(str)	{*actiontype=1; return(OK);}

			str=strstr(event,"DT");
			if(str)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_SUBSECTION:		//分段保护	
			str=strstr(event,"ER");
			if(str)	{*actiontype=1; return(OK);}
			break;
		default:
			return(-1);
			break;
		}
	}
	else if(devno==PDEVTYPE_SEL_551)	//SEL551
	{
		switch(moduletype)	//模块类型（保护类型）
		{
		case PTYPE_MTMIDZERO:	//主变中型点零序保护	
			str=strstr(event,"ER1");
			if(str)	{*actiontype=0; return(OK);}
			str=strstr(event,"ER2");
			if(str)	{*actiontype=1; return(OK);}
			str=strstr(event,"TRIP");
			if(str && target[0]==0)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_MTZTYPE:		//主变Z型变保护
			str=strstr(event,"ER1");
			if(str)	{*actiontype=0; return(OK);}
			str=strstr(event,"ER2");
			if(str)	{*actiontype=1; return(OK);}
			str=strstr(event,"TRIP");
			str1=strchr(target,'N');
			if(str && str1)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_CAPACITOR_I:		//电容器电流保护
			str=strstr(event,"ER1");
			if(str)	{*actiontype=0; return(OK);}
			str=strstr(event,"ER2");
			if(str)	{*actiontype=1; return(OK);}
			str=strstr(event,"TRIP");
			if(str && target[0]==0)	{*actiontype=2; return(OK);}
			break;
		case PTYPE_LINE:		//线路保护
			if(totalact_nums==0){
				str=strstr(event,"ER1");
				if(str)	{*actiontype=0; return(OK);}
				str=strstr(event,"ER2");
				if(str)	{*actiontype=1; return(OK);}
				str=strstr(event,"TRIP");
				str1=strchr(target,'N');
				if(str && str1)	{*actiontype=5; return(OK);}
			}
			else if(totalact_nums==1){
				str=strstr(event,"ER1");
				if(str)	{*actiontype=2; return(OK);}
				str=strstr(event,"ER2");
				if(str)	{*actiontype=3; return(OK);}
				str=strstr(event,"TRIP");
				if(str && target[0]==0)	{*actiontype=4; return(OK);}
				str1=strchr(target,'N');
				if(str && str1)	{*actiontype=6; return(OK);}
			}
			break;
		default:
			return(-1);
			break;
		}
	}
	else if(devno==PDEVTYPE_SEL_287)	//SEL287
	{
		str=strstr(event,"ET1");
		if(str)	{*actiontype=0; return(OK);}
		str=strstr(event,"ET2");
		if(str)	{*actiontype=1; return(OK);}
		str=strstr(event,"ER");
		if(str)	{*actiontype=2; return(OK);}
	}
	else if(devno==PDEVTYPE_SEL_387)	//SEL387
	{
		str=strstr(event,"TRIP1");
		if(str)	{*actiontype=0; return(OK);}
		str=strstr(event,"TRIP2");
		if(str)	{*actiontype=1; return(OK);}
		str=strstr(event,"TRIP3");
		if(str)	{*actiontype=2; return(OK);}
		str=strstr(event,"MER");
		if(str)	{*actiontype=3; return(OK);}
	}
	return -1;
}


SEL_PROTECTACTION	*SEL_Protect::GetActionPara(ushort actiontype,ushort circletype)
{
	struct ACTION_KEY{
		ushort actiontype;
		ushort circletype;
	}action_key;
	action_key.actiontype=(ushort)actiontype;
	action_key.circletype=(ushort)circletype;
	
	int rcdno=protacttab.RdbSearchRcdByKey(&action_key);
	return((SEL_PROTECTACTION*)protacttab.RdbGetRecordAddr(rcdno));
}

int SEL_Protect::GetActionInfo(ushort actiontype,ushort moduletype,char *name,char *info)
{
	SEL_PROTECTACTION *actionp;
	actionp=GetActionPara(actiontype,moduletype);
	if(actionp==NULL) return SCADA_ERR_NOOBJECT;
	if(name!=NULL) strcpy(name,actionp->actioninfo);
	if(info!=NULL) strcpy(info,actionp->actioninfo);
	return(OK);
}


int SEL_Protect::GetProtinfoTime(uchar *gram_part, SYS_CLOCK *clock)
{	//高位在前,binary
	//月，日，年，时，分，秒，毫秒
	if(gram_part[3] < 98)
		clock->year = gram_part[5] + 2000;
	else
		clock->year = gram_part[5] + 1900;
	clock->month=gram_part[1];
	clock->day=gram_part[3];
	clock->hour=gram_part[7];
	clock->minute=gram_part[9];
	clock->second=gram_part[11];
	clock->msecond=gram_part[12]*256+gram_part[13];
	return OK;
}

//解释FEP服务器保护生报文
int SEL_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SCDEVENT tmpevent;	//存盘事件
	APEvent   apevent;
	PROTECT_MSG msg;
	char    target[24];				/*		面板指示灯		*/
	char    action_string[12];		/*		动作代码		*/
	SYS_CLOCK sys_clock;
	ushort actiontype;
    SEL_PROTECTACTION	*actp;
    char entname[ENTNAME_LEN];
    entname[0] = 0;

    int		FaultLocation = 0;			/*		故障定位		*/
	unsigned short totalact_nums = 0;	/*		事故跳闸次数	*/
	int		value = 0;					/*			值			*/

	if(gram_len<5) return PROT_ERR_MAILLEN;

	GetProtinfoTime(&gram[4], &sys_clock);
	PROTECTMDL	*mdlp=GetModulePara(terminalno,protocoltype,gram[0],gram[1]); //gram[0]地址1,gram[1]地址2
							//地址对应模块信息

	if(mdlp==NULL)return PROT_ERR_MODULE;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,&sys_clock,mdlp,0,PROTTYPE_SEL);	//公共信息

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, &sys_clock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	switch(gram[2])
	{
	case PDEVTYPE_SEL_251:
		FaultLocation = gram[24]*256+gram[25];	// SEL 故障定位（Fault Location）
		totalact_nums = gram[26]*256+gram[27];	//SEL Shot Number
		value = gram[28]*256+gram[29];			//SEL---Fault Current
		//gram[30]*256+gram[31];		// Group 定值组号
		strncpy(target,(char *)&gram[32],sizeof(target)-1);		//面板指示
		break;
	case PDEVTYPE_SEL_551:		//SEL551
		totalact_nums = gram[26]*256+gram[27];	//SEL Shot Number
		value = gram[28]*256+gram[29];			//SEL---Fault Current
		strncpy(target,(char *)&gram[30],sizeof(target)-1);		//面板指示
		break;
	case PDEVTYPE_SEL_287:		//SEL287
		strncpy(target,(char *)&gram[24],sizeof(target)-1);		//面板指示
		break;
	case PDEVTYPE_SEL_387:		//SEL387
		//(gram[26]*256+gram[27])	--> Group 定值组号
		strncpy(target,(char *)&gram[28],sizeof(target)-1);		//面板指示
		break;
	default:
		return PROT_ERR_MESSAGE;
	}
	if(GetActionType(gram[2], mdlp->type2, action_string, target, &actiontype, totalact_nums)==OK)
	{
		actp = GetActionPara(actiontype,gram[2]);
		if(actp) {
			msg.event_int_info[0] = FaultLocation;
			msg.event_int_info[1] = totalact_nums;
			msg.event_int_info[2] = value;
			msg.sgflag = actp->sgflag;
			tmpevent.evt.val = (float)FaultLocation;
			tmpevent.evt.val1 = (float)totalact_nums;
			tmpevent.evt.val2 = (float)value;

		   sprintf(tmpevent.evt.member1,"%d",actp->actioncode);
		   strcpy(tmpevent.member_des1,actp->actioninfo);

		   strcpy(msg.enttypedescribe,actp->actioninfo);
		   tmpevent.evt.ToneOut=actp->alarmf;
		   strcpy(entname,actp->entname);
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
		apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
	}
	else return PROT_ERR_MESSAGE;
	return OK;
}




