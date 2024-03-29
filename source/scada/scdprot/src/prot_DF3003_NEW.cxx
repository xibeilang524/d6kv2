
///////////////////////////////////////////////////////////////////////////////////
//////////////////////  新DF3003处理类 /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#include "procthread.h"
#include "prot_DF3003_NEW.h"
#include "math.h"
#include <time.h>


DF3003_NEW_Protect::DF3003_NEW_Protect(void)
{
}

PROTECTMDL *DF3003_NEW_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort type2,ushort address1)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) //&& (mdlp->type2==type2) 
			&&(mdlp->address1==address1)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取动作自检信息
DF3003_PROTECTACTION* DF3003_NEW_Protect::GetActionPara(unsigned short funaction,unsigned short actcode)
{
	DF3003_ACTION_KEY key;
	key.funaction = funaction;
	key.actcode = actcode;
	return ((DF3003_PROTECTACTION *)table.RdbGetRecordAddr(DF3003_PROTACTION_TABNAME, &key));
}

//取测量值信息参数
DF3003_PROTECTMEASURE* DF3003_NEW_Protect::GetMeasurePara(unsigned short measurecode)
{
	return ((DF3003_PROTECTMEASURE *)table.RdbGetRecordAddr(DF3003_PROTMEASURE_TABNAME, &measurecode));
}

/*DF3003_PROTECTTROUBLE* DF3003_NEW_Protect::GetTroublePara(unsigned short troublecode)
{
	return ((DF3003_PROTECTTROUBLE *)table.RdbGetRecordAddr(DF3003_PROTTROUBLE_TABNAME, &troublecode));
}
*/
//取保护故障描述
/*int DF3003_NEW_Protect::GetTroubleInfo(ushort troubletype,char *name,char *info)
{
	DF3003_PROTECTTROUBLE *troublep;
	troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return -1;
	if(name)strcpy(name, troublep->troubleinfo);
	if(info)strcpy(info, troublep->troubleinfo);
	return OK;
}*/

//取保护测量值描述
int DF3003_NEW_Protect::GetMeasureInfo(ushort measurecode,char *info,char *unit)
{
	DF3003_PROTECTMEASURE *measurep;
	measurep = GetMeasurePara(measurecode);
	if(measurep == NULL)return -1;

	if(info)strcpy(info, measurep->measureinfo);
	if(unit)strcpy(unit, measurep->measureunit);
	return OK;
}
	
//取时间
int DF3003_NEW_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag)
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
		clock->hour = gram_part[3] & 0x1f;
		clock->minute = gram_part[2] & 0x3f;
		clock->second=(gram_part[1] * 256 + gram_part[0])/1000;
		clock->msecond=(gram_part[1] * 256 + gram_part[0])%1000;
	}

	return OK;
}

//FEP服务器保护生报文解释
int DF3003_NEW_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SYS_CLOCK sys_clock;
	PROTECTMDL	*mdlp;	//微机保护参数指针
	int ret;
	unsigned char *acteventp;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	mdlp = GetModulePara(terminalno,protocoltype,gram[4],gram[3]);	// gram[3]为地址码,gram[4]对于df3300不判
	if(mdlp == NULL) return PROT_ERR_MODULE;


	if(gram[0] == 0x01)		//动作自检信息(df3300不分动作和自检)
	{
		acteventp = gram + 7;		//时间位

		GetProtinfoTime(acteventp, TIMEBYBIN, &sys_clock,1);		//发生时间
		make_action_apevent(&sys_clock, mdlp, gram+5);
		ret = save_detect_apevent(&sys_clock, mdlp, gram+5);

	}
	else if(gram[0] == 0x02)
	{
		acteventp = gram + 11;		//时间位

		GetProtinfoTime(acteventp, TIMEBYBIN, &sys_clock,1);		//发生时间
		make_action_apevent(&sys_clock, mdlp, gram+5);
		ret = save_action_apevent(&sys_clock, mdlp, gram+5);
	}
	else	return PROT_ERR_MESSAGE;
	return ret;
}

void DF3003_NEW_Protect::make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp)
{
	PROTECT_MSG msg;
	APEvent   apevent;	//应用事件处理类
	unsigned short eventinfo = 0;
	DF3003_PROTECTACTION *DF3003_action;

	if(mdlp == NULL || sysclock == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_DF3003_NEW);
	
	msg.type_value = acteventp[0];
	DF3003_action = (DF3003_PROTECTACTION *)GetActionPara(mdlp->type2,msg.type_value);
	if(DF3003_action) {
		strcpy(msg.enttypedescribe,DF3003_action->actioninfo);
		msg.sgflag = DF3003_action->sgflag;
		apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
	}
}

//动作事项存盘
int DF3003_NEW_Protect::save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp)
{
	SCDEVENT tmpevent;	//存盘事件
	DF3003_PROTECTACTION *DF3003_action;
	int i=0,measurenum;
	char measureinfo[PROTINFO_LEN],measureunit[PROT_UNIT_LEN],longdes[120];	
	long TempDwordVal;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	tmpevent.evt.state = acteventp[0];
	DF3003_action = (DF3003_PROTECTACTION *)GetActionPara(mdlp->type2,acteventp[0]);
	if(DF3003_action == NULL) return PROT_ERR_ACTION;
	tmpevent.evt.ToneOut = DF3003_action->alarmf;
	sprintf(tmpevent.evt.member1,"%d",DF3003_action->actcode);
	strcpy(tmpevent.member_des1,DF3003_action->actioninfo);

	measurenum = acteventp[10];
	for(i=0;i<measurenum;i++)
	{
		GetMeasureInfo(acteventp[11+i*5], measureinfo, measureunit);
//		tmpevent.evt.val = (float)Dword2Float(acteventp[12+i*5]+ acteventp[13+i*5]*256+(acteventp[14+i*5] + acteventp[15+i*5]*256)*256*256); 值运算于机型无关
		#ifdef __sun
		TempDwordVal = Dword2Val_Sun((long *)(acteventp + 12 + i*5));//Sun机器下的浮点算法
		#else
		memcpy(&TempDwordVal,acteventp + 12 + i*5,4);//一般的浮点算法
		#endif
		tmpevent.evt.val = (float)Dword2Float(TempDwordVal);

		sprintf(longdes,"%s:%6.2f%s",measureinfo,(float)tmpevent.evt.val,measureunit);
		if((strlen(tmpevent.longdes)+strlen(longdes))>=120) break;
		strcat(tmpevent.longdes,longdes);
	}

	yield_entdescribe(DF3003_action->entname,&tmpevent);
	if(tmpevent.evt.ToneOut)yield_snddescribe(DF3003_action->entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}
	return OK;
}


int DF3003_NEW_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp)	//保存自检事件
{
	SCDEVENT tmpevent;	//存盘事件
	DF3003_PROTECTACTION *DF3003_action;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	tmpevent.evt.state = acteventp[0];
	DF3003_action = (DF3003_PROTECTACTION *)GetActionPara(mdlp->type2,acteventp[0]);
	if(DF3003_action == NULL) return PROT_ERR_DETECT;
	tmpevent.evt.ToneOut = DF3003_action->alarmf;
	sprintf(tmpevent.evt.member1,"%d",DF3003_action->actcode);
	strcpy(tmpevent.member_des1,DF3003_action->actioninfo);

	yield_entdescribe(DF3003_action->entname,&tmpevent);
	if(tmpevent.evt.ToneOut)yield_snddescribe(DF3003_action->entname,&tmpevent);
	//2003 03
	NetStatus netcfg;
	int groupnum=netcfg.GetHostGrpNum();
	int *grp = netcfg.GetHostGrpEnm();
	for(int no=0;no<groupnum;no++)
	{
		tmpevent.evt.grpno=grp[no];
		save_event(&tmpevent,1);
	}
	return OK;
}

float DF3003_NEW_Protect::Dword2Float(long TempDword)//IEEE标准754R32.23格式将4字节转为浮点形
{
	float TempFloat=0;
	//公式(1-2S)*(1+M*2E-23)*2E(E-127)
	//=(1-2S)*(1+F)*2E(E-127)
	//=(1-2S)*(1+F)*2EE/2E127
	//=(1-2S)*(1+F)*L
	int S=!((TempDword&0X80000000)==0);//符号位
	int M=TempDword&0X007FFFFF;
	float F=(float)M/((float)0X800000);//或M*pow(2,-23)
	int E=(TempDword>>23)&0XFF;		//去掉符号位,取出指数
	E -=127;
	double aa = 2;
	float L=(float)pow(aa,E);
	TempFloat=(1-2*S)*(1+F)*L;
	return TempFloat;
}

long DF3003_NEW_Protect::Dword2Val_Sun(long *TempDword)//Sun机器下的浮点算法
{
	unsigned char DesDword[4];
	unsigned char *OriDowrd=(unsigned char *)TempDword;
	DesDword[0]=OriDowrd[3];
	DesDword[1]=OriDowrd[2];
	DesDword[2]=OriDowrd[1];
	DesDword[3]=OriDowrd[0];
	return *((long*) DesDword);
}
