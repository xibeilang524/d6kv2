#include "scadaproc.h"
#include "procthread.h"
#include "prot_DF33.h"

				//////////////////////////////////////////////////////////////
				/*					DF33 微机保护处理类					*/
				//////////////////////////////////////////////////////////////

Rdb_Tab DF33_Protect::rdbtable(MyUserName,MyPassWord);
int DF33_Protect::opentabf;


DF33_Protect::DF33_Protect(void)
{

}

PROTECTMDL *DF33_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort address3,ushort address4)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1) && (mdlp->address2==address2) && (mdlp->address3 == address3) && (mdlp->address4 == address4)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取保护动作参数
DF33_PROTECTACTION	*DF33_Protect::GetActionPara(short type,short actioncode)
{
	KEY_DF33 key;
	key.devicetype = type;
	key.code = actioncode;
	return (DF33_PROTECTACTION *)table.RdbGetRecordAddr(DF33_PROTACTION_TABNAME,&key);
}

//取故障信息参数
DF33_PROTECTTROUBLE	*DF33_Protect::GetTroublePara(short troublecode)
{
	return (DF33_PROTECTTROUBLE *)table.RdbGetRecordAddr(DF33_PROTTRBL_TABNAME,&troublecode);
}

//取测量值信息参数
DF33_PROTECTMEASURE	*DF33_Protect::GetMeasurePara(short measurecode)
{
	return (DF33_PROTECTMEASURE *)table.RdbGetRecordAddr(DF33_PROTMSR_TABNAME,&measurecode);
}


//取保护动作描述
int DF33_Protect::GetActionInfo(short type,int actiontype,char *name,char *info)
{
	DF33_PROTECTACTION *actionp = GetActionPara(type,actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}


//取保护故障描述
int DF33_Protect::GetTroubleInfo(short type,short troubletype,char *info)
{
	DF33_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}

//取时间
int DF33_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag)
{
	SYS_CLOCK sysclock;
	struct tm tim;
	struct tm *tim1;
	time_t tt;
	if(clock == NULL)return -1;
	GetCurrentClock(&sysclock);
	if(flag==0)
	{
		tim.tm_year=sysclock.year-1900;
		tim.tm_mon=sysclock.month-1;
		tim.tm_mday=sysclock.day;
		tim.tm_hour=gram_part[7] & 0x1f;
		tim.tm_min=gram_part[6] & 0x3f;
		tim.tm_sec=(gram_part[4] * 256 + gram_part[5])/1000;
		tim.tm_isdst=0;
		tt=mktime(&tim);
		tt=tt+(gram_part[0] * 256 + gram_part[1])/1000;
		tim1=localtime(&tt);
		clock->year=((uchar) tim1->tm_year)+1900;
        clock->month=(uchar)tim1->tm_mon+1;
        clock->day=(uchar)tim1->tm_mday;
        clock->hour=(uchar) tim1->tm_hour;
        clock->minute=(uchar) tim1->tm_min;
        clock->second=(uchar) tim1->tm_sec;
		clock->msecond=((gram_part[7] * 256 + gram_part[6])%1000+gram_part[3] * 256 + gram_part[2])%1000;
	}
	else if(flag==1)
	{
		clock->year = sysclock.year;
		clock->month = sysclock.month;
		clock->day = sysclock.day;
		clock->hour = gram_part[0] & 0x1f;
		clock->minute = gram_part[1] & 0x3f;
		clock->second=gram_part[3] ;
		clock->msecond=gram_part[5] * 256 + gram_part[4];
	}

	return OK;
}

//解释FEP服务器保护生报文
int DF33_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp = NULL;		//微机保护参数指针
	int ret=OK;

	if(gram_len<16) return PROT_ERR_MAILLEN;



	if(gram[0] == 0XCC)		//事件
	{
		ushort address1 = gram[2];
		ushort address2 = gram[3];
		ushort address3 = gram[4];
		ushort address4 = gram[5];
		RdbDebugMesg("终端号  %d 地址1 %d 地址2 %d 地址3 %d 地址4 %d \n",terminalno,address1,address2,address3,address4);
		mdlp = GetModulePara(terminalno,protocoltype,address1,address2,address3,address4);			//微机保护参数
		if (mdlp == NULL) 
			return PROT_ERR_MODULE;
		GetProtinfoTime(gram+14, TIMEBYBIN, &sys_clock,1);	//发生时间
		if (gram[7] == 2)//动作
		{
			RdbDebugMesg("动作处理");
			make_action_apevent(&sys_clock,mdlp,gram+12);
			save_action_apevent(&sys_clock,mdlp,gram+12);
		}
		else if (gram[7] == 1)//故障
		{
			RdbDebugMesg("故障处理");
			save_trouble_apevent(&sys_clock,mdlp,gram+12);
		}
	}
	else 
		return PROT_ERR_MESSAGE;

	return ret;
}

//生成保护动作应用事件
void DF33_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	DF33_PROTECTACTION *DF33_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DF33);	//公共信息

	short actionno = event[1]*256 + event[0];
	if((DF33_action = GetActionPara(mdlp->type1,actionno))==NULL)return;	//取动作信息

	msg.type_value = (short)actionno;			//动作类型
	
	msg.sgflag = DF33_action->sgflag;
	strcpy(msg.enttypedescribe,DF33_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}

void  DF33_Protect::SunFloat(uchar *source, uchar *des)
{
	des[3] = source[0];
	des[2] = source[1];
	des[1] = source[2];
	des[0] = source[3];

}

//动作事项存盘
int DF33_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN],longdes[120];
	DF33_PROTECTACTION *DF33_action;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	short actionno = event[1]*256 + event[0];

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////

	tmpevent.evt.state = actionno;			//动作类型-----状态值
	short type1 = mdlp->type1;
	RdbDebugMesg("动作处理 --- %d",type1);
	
	if((DF33_action = GetActionPara(type1,actionno)) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",DF33_action->actioncode);
	strcpy(tmpevent.member_des1,DF33_action->actioninfo);
	tmpevent.evt.ToneOut = DF33_action->alarmf;
	strcpy(entname,DF33_action->entname);


	uchar measurenum = event[8];
	if (measurenum>8) measurenum = 6;

	ushort measureno[8];
	int i=0;

	for (i=0;i<4;i++) 
		measureno[i] = event[10+i];
	for (i=0;i<4;i++) 
		measureno[i+4] = event[15+i];

	for (i=0;i<measurenum;i++)
	{
		DF33_PROTECTMEASURE *DF33_measure = GetMeasurePara(measureno[i]);
		
		if(DF33_measure)
		{
			float value;
#ifdef __sun
			SunFloat( (uchar*)(event+20+i*5), (uchar*)&value  );
#else
			memcpy(&value,event+20+i*5,4);//一般的浮点算法
#endif
			sprintf(longdes,"%s:%.2f %s",
				DF33_measure->measureinfo,value,DF33_measure->measureunit);
			strcat(tmpevent.longdes,longdes);
		}
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

//故障事项存盘
int DF33_Protect::save_trouble_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN],longdes[120];
	DF33_PROTECTTROUBLE *DF33_trouble;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_ACTION;

	short troubleno = event[1]*256 + event[0];

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////

	tmpevent.evt.state = troubleno;			////故障类型-----状态值
	if((DF33_trouble = GetTroublePara(troubleno)) == NULL) return PROT_ERR_ACTION;	//取动作信息
	RdbDebugMesg("故障%s",DF33_trouble->troubleinfo);
	sprintf(tmpevent.evt.member1,"%d",DF33_trouble->troublecode);
	strcpy(tmpevent.member_des1,DF33_trouble->troubleinfo);
	tmpevent.evt.ToneOut = 1;

	strcpy(entname,"PCProtChg");


	uchar measurenum = event[8];
	if (measurenum>8) measurenum = 8;

	ushort measureno[8];
	int i=0;

	for (i=0;i<4;i++) 
		measureno[i] = event[10+i];
	for (i=0;i<4;i++) 
		measureno[i+4] = event[15+i];

	for (i=0;i<measurenum;i++)
	{
		DF33_PROTECTMEASURE *DF33_measure = GetMeasurePara(measureno[i]);
		
		if(DF33_measure)
		{
			float value;
#ifdef __sun
			SunFloat( (uchar*)(event+20+i*5), (uchar*)&value  );
#else
			memcpy(&value,event+20+i*5,4);//一般的浮点算法
#endif
			sprintf(longdes,"%s:%.2f %s",
				DF33_measure->measureinfo,value,DF33_measure->measureunit);
			strcat(tmpevent.longdes,longdes);
		}
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
		RdbDebugMesg("生成事项 %d",grp[no]);
		save_event(&tmpevent,1);
	}//for--no
	return OK;
}

