
///////////////////////////////////////////////////////////////////////////////////
//////////////////////  LFP处理类 /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#include "procthread.h"
#include "prot_LFP.h"
#include "time.h"


PROTECTMDL *LFP_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1)//取微机保护参数表（protmodule）
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

//取LFP_模块信息	"lfp_module"	
LFP_PROTECTMODULE* LFP_Protect::GetLfpMdlPara(unsigned short moduletype)
{
	return ((LFP_PROTECTMODULE *)table.RdbGetRecordAddr(LFP_PROTMODULE_TABNAME,&moduletype)); 
}

//取LFP_故障信息	"lfp_action"
LFP_PROTECTACTION* LFP_Protect::GetActionPara(unsigned short moduletype,unsigned char boardnum,unsigned char actbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = boardnum;
	key.flag2 = actbit;
	return ((LFP_PROTECTACTION *)table.RdbGetRecordAddr(LFP_PROTACTION_TABNAME,&key)); 
}

//取LFP_事件信息	"lfp_event"
LFP_PROTECTEVENT* LFP_Protect::GetEventPara(unsigned short moduletype,unsigned char actbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = actbit;
	return ((LFP_PROTECTEVENT *)table.RdbGetRecordAddr(LFP_PROTEVENT_TABNAME,&key)); 
}

//取LFP_自检信息	"lfp_detect"
LFP_PROTECTDETECT* LFP_Protect::GetDetectPara(unsigned short moduletype,unsigned char flagtype,unsigned char chkbit)
{
	LFP_ACTION_KEY key;
	key.moduletype = moduletype;
	key.flag1 = flagtype;
	key.flag2 = chkbit;
	return ((LFP_PROTECTDETECT *)table.RdbGetRecordAddr(LFP_PROTDETECT_TABNAME,&key)); 
}

//FEP服务器保护生报文解释
int LFP_Protect::protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram)
{
	SYS_CLOCK sys_clock;
	PROTECTMDL	*mdlp;	//微机保护参数指针
	LFP_PROTECTMODULE *m_pLfpMdl; //LFP模块参数指针
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN; 

	mdlp = GetModulePara(terminalno,protocoltype,gram[0]);	// gram[0]为地址码
	if(mdlp == NULL)return PROT_ERR_MODULE;

	m_pLfpMdl = GetLfpMdlPara(mdlp->type1);
	if(m_pLfpMdl == NULL)return PROT_ERR_MODULE;

	if(gram[1] == 0x40)		// 事件信息上送---简单事件报告
	{
		GetProtinfoTime(gram+5, TIMEBYBIN, &sys_clock);		//发生时间
		make_event_apevent(&sys_clock, mdlp, m_pLfpMdl, gram + 13);
		ret = save_event_apevent(&sys_clock, mdlp, m_pLfpMdl, gram + 13);
	}
	else if(gram[1] == 0x46)		//故障信息上送报告--复杂事件报告
	{
		GetProtinfoTime(gram+5, TIMEBYBIN, &sys_clock);	//发生时间
		if(m_pLfpMdl->totrouble)
			actiontype = LFP_ACTTYPE0;
		else if(m_pLfpMdl->prtbd0tz && m_pLfpMdl->prtbd1tz && m_pLfpMdl->circuitval)
			actiontype = LFP_ACTTYPE4;
		else if(m_pLfpMdl->prtbd0tz && m_pLfpMdl->prtbd1tz )
			actiontype = LFP_ACTTYPE3;
		else if(m_pLfpMdl->prtbd0tz && m_pLfpMdl->circuitval )
			actiontype = LFP_ACTTYPE2;
		else if(m_pLfpMdl->prtbd0tz )
			actiontype = LFP_ACTTYPE1;
		make_action_apevent(&sys_clock, mdlp, m_pLfpMdl, gram + 13);
		ret = save_action_apevent(&sys_clock, mdlp, m_pLfpMdl, gram + 13);
	}
	else if(gram[1]==0x47)		//自检报告
	{
		if(m_pLfpMdl->flagtype) { //有标志字节
			GetProtinfoTime(gram+6, TIMEBYBIN, &sys_clock);
			ret = save_detect_apevent(&sys_clock,mdlp,m_pLfpMdl,gram+14,gram[5]);
		}
		else{
			GetProtinfoTime(gram+5, TIMEBYBIN, &sys_clock);
			ret = save_detect_apevent(&sys_clock,mdlp,m_pLfpMdl,gram+13,0);
		}
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}


//形成简单事件报告的应用事件
void LFP_Protect::make_event_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event)
{
	int i , bytenum;
	uchar actbit;
	PROTECT_MSG msg;
	APEvent   apevent;	//应用事件处理类
	unsigned short eventinfo = 0;
	LFP_PROTECTEVENT *lfp_event;

	if(mdlp == NULL || sysclock == NULL || m_pLfpMdl == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_LFP);

	switch (m_pLfpMdl->eventtype)
	{
	case LFP_EVTTYPE0:
		bytenum = 8;
		eventinfo = event[0] ;
		break;
	case LFP_EVTTYPE1:
	case LFP_EVTTYPE4:	
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		break;
	case LFP_EVTTYPE2:
		bytenum = 8;
		eventinfo = event[0];
		msg.event_float_info[0] = (float)(event[2]+event[3]*256)/100.0f;				//故障电流
		break;
	case LFP_EVTTYPE3:
	case LFP_EVTTYPE5:
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		msg.event_float_info[0] = (float)(event[2]+event[3]*256)/100.0f;				//故障电流
		break;
	default:
		return;
	}	 //switch

	i = 0;
	while(i < bytenum)
	{
		if(eventinfo & 0x0001) {
			msg.type_value = i;
			actbit = i;
			lfp_event = (LFP_PROTECTEVENT *)GetEventPara(m_pLfpMdl->moduletype,actbit);
			if(lfp_event) {
				strcpy(msg.enttypedescribe,lfp_event->describe);
				msg.sgflag = lfp_event->sgflag;
				apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
			}
		}
		eventinfo = eventinfo >> 1;
		i++;
	}
} 


//形成简单事件报告的存盘事项
int LFP_Protect::save_event_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp,LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event)
{
	int i, bytenum ;
	uchar actbit;
	SCDEVENT tmpevent;	//存盘事件
	unsigned short eventinfo;
	LFP_PROTECTEVENT *lfp_event;

	if(mdlp == NULL || sysclock == NULL || m_pLfpMdl == NULL)return PROT_ERR_ACTION;
	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	switch (m_pLfpMdl->eventtype)
	{
	case LFP_EVTTYPE0:
		bytenum = 8;
		eventinfo = event[0] ;
		break;
	case LFP_EVTTYPE1:
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		break;
	case LFP_EVTTYPE2:
		bytenum = 8;
		eventinfo = event[0];
		tmpevent.evt.val = (float)(event[2]+event[3]*256)/100.0f;				//故障电流
		sprintf(tmpevent.otherdes1,"故障电流:%6.2fA",(float)tmpevent.evt.val);
		strcpy(tmpevent.member_des2,"选相:");
		if(event[1] & 0x01) strcat(tmpevent.member_des2,"A");
		if(event[1] & 0x02) strcat(tmpevent.member_des2,"B");
		if(event[1] & 0x04) strcat(tmpevent.member_des2,"C");
		if(((event[1] & 0x01)==0) && ((event[1] & 0x02)==0) && ((event[1] & 0x04)==0))
			strcat(tmpevent.member_des2,"/");
		break;
	case LFP_EVTTYPE3:
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		tmpevent.evt.val = (float)(event[2]+event[3]*256)/100.0f;				//故障电流
		sprintf(tmpevent.otherdes1,"故障电流:%6.2fA",(float)tmpevent.evt.val);
		strcpy(tmpevent.member_des2,"选相:");
		if(event[4] & 0x01) strcat(tmpevent.member_des2,"A");
		if(event[4] & 0x02) strcat(tmpevent.member_des2,"B");
		if(event[4] & 0x04) strcat(tmpevent.member_des2,"C");
		if(((event[4] & 0x01)==0) && ((event[4] & 0x02)==0) && ((event[4] & 0x04)==0))
			strcat(tmpevent.member_des2,"/");
		break;
	case LFP_EVTTYPE4:	
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		tmpevent.evt.val = (float)(event[2]+event[3]*256)/100.0f;				//故障电流Ia
		tmpevent.evt.val1 = (float)(event[4]+event[5]*256)/100.0f;				//故障电流Ib
		tmpevent.evt.val2 = (float)(event[6]+event[7]*256)/100.0f;				//故障电流Ic
		sprintf(tmpevent.otherdes1,"故障电流Ia:%6.2fA",(float)tmpevent.evt.val);
		sprintf(tmpevent.otherdes2,"故障电流Ib:%6.2fA",(float)tmpevent.evt.val1);
		sprintf(tmpevent.otherdes3,"故障电流Ic:%6.2fA",(float)tmpevent.evt.val2);
		break;
	case LFP_EVTTYPE5:
		bytenum = 16;
		eventinfo = event[0] + event[1]*256;
		tmpevent.evt.val = (float)(event[2]+event[3]*256)/100.0f;				//故障电流Imax
		tmpevent.evt.val1 = (float)(event[4]+event[5]*256)/100.0f;				//t1
		tmpevent.evt.val2 = (float)(event[6]+event[7]*256)/100.0f;				//t2
		sprintf(tmpevent.otherdes1,"故障最大相电流I:%6.2fA",(float)tmpevent.evt.val);
		sprintf(tmpevent.otherdes2,"第一次跳闸时间:%4.0fms",(float)tmpevent.evt.val1);
		sprintf(tmpevent.otherdes3,"第二次跳闸时间:%4.0fms",(float)tmpevent.evt.val2);
		break;
	default:
		return PROT_ERR_ACTION;
	}	 //switch

	i = 0;
	while(i < bytenum)
	{
		if(eventinfo & 0x0001) {
			tmpevent.evt.state = i;
			actbit = i;
			lfp_event = (LFP_PROTECTEVENT *)GetEventPara(m_pLfpMdl->moduletype,actbit);
			if(lfp_event) {
				tmpevent.evt.ToneOut = lfp_event->alarmf;
				strcpy(tmpevent.member_des1,lfp_event->describe);			//对象名1描述---动作描述
				yield_entdescribe(lfp_event->entname,&tmpevent);			// 产生事项文字描述
				if(tmpevent.evt.ToneOut)yield_snddescribe(lfp_event->entname,&tmpevent);				// 产生事项语音描述
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
		eventinfo = eventinfo >> 1;
		i++;
	}	//while
	return OK;
} 

void LFP_Protect::make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	unsigned short eventinfo;
	int i,j,prtbdnum = 0;
	uchar boardnum,actbit;
	LFP_PROTECTACTION *lfp_action;

	if(mdlp == NULL || sysclock == NULL || m_pLfpMdl == NULL)return;

	if(actiontype == LFP_ACTTYPE0)
	{
		make_event_apevent(sysclock,mdlp,m_pLfpMdl,event);
		return;
	}

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_LFP);

	switch(actiontype)
	{
	case LFP_ACTTYPE1:
		prtbdnum = 1;
		break;
	case LFP_ACTTYPE2:
		prtbdnum = 1;
		msg.event_int_info[0] = event[12];											//重合闸值		
		msg.event_float_info[0] = (float)(event[14] + event[15]*256);				//重合闸时间
		msg.event_float_info[1] = (float)(event[16] + event[17]*256)/100.0f;		//测距值
		msg.event_float_info[2] = (float)(event[18] + event[19]*256)/100.0f;		//最大短路电流
		msg.event_float_info[3] = (float)(event[20] + event[21]*256)/100.0f;		//最大零序电流
		break;
	case LFP_ACTTYPE3:
		prtbdnum = 2;
		break;
	case LFP_ACTTYPE4:	
		prtbdnum = 2;
		msg.event_int_info[0] = event[24];											//重合闸值		
		msg.event_float_info[0] = (float)(event[26] + event[27]*256);				//重合闸时间
		msg.event_float_info[1] = (float)(event[28] + event[29]*256)/100.0f;		//测距值
		msg.event_float_info[2] = (float)(event[30] + event[31]*256)/100.0f;		//最大短路电流
		msg.event_float_info[3] = (float)(event[32] + event[33]*256)/100.0f;		//最大零序电流
		break;
	default:
		return;
	}

	for(i=0;i<prtbdnum*3;i++)
	{
		if(actiontype == LFP_ACTTYPE3)eventinfo = event[i*4+2] + event[i*4+3]*256;
		else eventinfo = event[i*4] + event[i*4+1]*256;
		j=0;
		while (j < 16)
		{
			if(eventinfo & 0x0001) {
				boardnum = i/3;
				msg.type_value = j;
				actbit = j;
				lfp_action = (LFP_PROTECTACTION *)GetActionPara(m_pLfpMdl->moduletype,boardnum,actbit);
				if(lfp_action) {
					strcpy(msg.enttypedescribe,lfp_action->describe);
					msg.sgflag = lfp_action->sgflag;
					apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
				}
			}
			eventinfo = eventinfo >>1;
			j++;
		}	//while
	} //for--i
	return;
}


int LFP_Protect::save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	unsigned short eventinfo;						//事件信息
	LFP_PROTECTACTION *lfp_action;
	int i,j,prtbdnum = 0,ret;
	uchar boardnum,actbit;
	unsigned short eventtime;						//事件相对时间
	struct tm tim;
	struct tm *tim1;
	time_t tt;
	SYS_CLOCK eventclock;

	if(mdlp == NULL || sysclock == NULL || m_pLfpMdl ==	NULL)return PROT_ERR_TROUBLE;

	if(actiontype == LFP_ACTTYPE0)
	{
		ret = save_event_apevent(sysclock,mdlp,m_pLfpMdl,event);
		return ret;
	}

	memset(&tmpevent,0,sizeof(SCDEVENT));

	switch(actiontype)
	{
	case LFP_ACTTYPE1:
		prtbdnum = 1;
		break;
	case LFP_ACTTYPE2:
		prtbdnum = 1;
		if(event[12] & 0x01)strcpy(tmpevent.member_des3,"重合闸动作");											//重合闸值		
		else strcpy(tmpevent.member_des3,"重合闸未动作");
		tmpevent.evt.val = (float)(event[14] + event[15]*256);				//重合闸时间
		tmpevent.evt.val1 = (float)(event[16] + event[17]*256)/100.0f;		//测距值
		tmpevent.evt.val2 = (float)(event[18] + event[19]*256)/100.0f;		//最大短路电流
		tmpevent.evt.val3 = (float)(event[20] + event[21]*256)/100.0f;		//最大零序电流
		sprintf(tmpevent.otherdes0,"重合闸时间:%4.0fms",(float)tmpevent.evt.val);
		sprintf(tmpevent.otherdes1,"测距值:%6.2fkm",(float)tmpevent.evt.val1);
		sprintf(tmpevent.otherdes2,"最大短路电流:%6.2fA",(float)tmpevent.evt.val2);
		sprintf(tmpevent.otherdes3,"I0max:%6.2fA",(float)tmpevent.evt.val3);

		strcpy(tmpevent.member_des2,"选相:");
		if(event[22] & 0x01) strcat(tmpevent.member_des2,"A");
		if(event[22] & 0x02) strcat(tmpevent.member_des2,"B");
		if(event[22] & 0x04) strcat(tmpevent.member_des2,"C");
		if(((event[22] & 0x01)==0) && ((event[22] & 0x02)==0) && ((event[22] & 0x04)==0))
			strcat(tmpevent.member_des2,"/");
		break;
	case LFP_ACTTYPE3:
		prtbdnum = 2;
		break;
	case LFP_ACTTYPE4:	
		prtbdnum = 2;
		if(event[24] & 0x01)strcpy(tmpevent.member_des3,"重合闸动作");											//重合闸值		
		else strcpy(tmpevent.member_des3,"重合闸未动作");
		tmpevent.evt.val = (float)(event[26] + event[27]*256);				//重合闸时间
		tmpevent.evt.val1 = (float)(event[28] + event[29]*256)/100.0f;		//测距值
		tmpevent.evt.val2 = (float)(event[30] + event[31]*256)/100.0f;		//最大短路电流
		tmpevent.evt.val3 = (float)(event[32] + event[33]*256)/100.0f;		//最大零序电流
		sprintf(tmpevent.otherdes0,"重合闸时间:%4.0fms",(float)tmpevent.evt.val);
		sprintf(tmpevent.otherdes1,"测距值:%6.2fkm",(float)tmpevent.evt.val1);
		sprintf(tmpevent.otherdes2,"最大短路电流:%6.2fA",(float)tmpevent.evt.val2);
		sprintf(tmpevent.otherdes3,"I0max:%6.2fA",(float)tmpevent.evt.val3);

		strcpy(tmpevent.member_des2,"选相:");
		if(event[34] & 0x01) strcat(tmpevent.member_des2,"A");
		if(event[34] & 0x02) strcat(tmpevent.member_des2,"B");
		if(event[34] & 0x04) strcat(tmpevent.member_des2,"C");
		if(((event[34] & 0x01)==0) && ((event[34] & 0x02)==0) && ((event[34] & 0x04)==0))
			strcat(tmpevent.member_des2,"/");
		break;
	default:
		return PROT_ERR_TROUBLE;
	}

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志

	for(i=0;i<prtbdnum*3;i++)
	{
		if(actiontype == LFP_ACTTYPE3){
			eventinfo = event[i*4+2] + event[i*4+3]*256;
			eventtime = event[i*4] + event[i*4+1]*256;
		}
		else{
			eventinfo = event[i*4] + event[i*4+1]*256;
			eventtime = event[i*4+2] + event[i*4+3]*256;
		}

		tim.tm_year=sysclock->year-1900;
		tim.tm_mon=sysclock->month-1;
		tim.tm_mday=sysclock->day;
		tim.tm_hour=sysclock->hour;
		tim.tm_min=sysclock->minute;
		tim.tm_sec=sysclock->second;
		tim.tm_isdst=0;
		tt=mktime(&tim);
		tt=tt+(eventtime+sysclock->msecond)/1000;
		tim1=localtime(&tt);
		eventclock.year=((uchar) tim1->tm_year)+1900;
		eventclock.month=(uchar)tim1->tm_mon+1;
		eventclock.day=(uchar)tim1->tm_mday;
		eventclock.hour=(uchar) tim1->tm_hour;
		eventclock.minute=(uchar) tim1->tm_min;
		eventclock.second=(uchar) tim1->tm_sec;
		eventclock.msecond=(eventtime+sysclock->msecond)%1000;
		make_common_saveinfo(&tmpevent, &eventclock, mdlp, 0);

		j = 0;
		while(j < 16)
		{
			if(eventinfo & 0x0001) {
				boardnum = i/3;
				actbit = j;
				lfp_action = (LFP_PROTECTACTION *)GetActionPara(m_pLfpMdl->moduletype,boardnum,actbit);
				if(lfp_action) {
					tmpevent.evt.ToneOut = lfp_action->alarmf;
					strcpy(tmpevent.member_des1, lfp_action->describe);							//对象名1描述---动作类型描述
					tmpevent.evt.state = j;
					yield_entdescribe(lfp_action->entname,&tmpevent);
					if(tmpevent.evt.ToneOut)yield_snddescribe(lfp_action->entname,&tmpevent);
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
			eventinfo = eventinfo >> 1;
			j++;
		}	//while
	} //for--i
	return OK;
}

int LFP_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event,int m_Flag)	//保存自检事件
{ 
	SCDEVENT tmpevent;	//存盘事件
	int i = 0;
	uchar chkbit;
	unsigned short eventinfo;
	LFP_PROTECTDETECT *lfp_detect;

	if(mdlp == NULL || sysclock == NULL || m_pLfpMdl == NULL)return PROT_ERR_DETECT;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);

	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	eventinfo = event[1] + event[2]*256;

	while(i < 16)
	{
		if(eventinfo & 0x0001) {
			tmpevent.evt.state = i;
			chkbit = i;
			lfp_detect = GetDetectPara(m_pLfpMdl->moduletype,m_Flag,chkbit);
			if(lfp_detect) {
				tmpevent.evt.ToneOut = lfp_detect->alarmf;		//音响报警标志
				strcpy(tmpevent.member_des1,lfp_detect->describe);			//对象名1描述---动作描述
				yield_entdescribe(lfp_detect->entname,&tmpevent);
				if(tmpevent.evt.ToneOut)yield_snddescribe(lfp_detect->entname, &tmpevent);
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
		eventinfo = eventinfo >> 1;
		i++;
	}
	return OK;
}

