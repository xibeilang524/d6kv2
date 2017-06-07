#include "scadaproc.h"
#include "procthread.h"
#include "prot_ISA.h"

				//////////////////////////////////////////////////////////////
				/*					ISA(LSA_P) 微机保护处理类					*/
				//////////////////////////////////////////////////////////////

Rdb_Tab ISA_Protect::rdbtable(MyUserName,MyPassWord);
int ISA_Protect::opentabf;


ISA_Protect::ISA_Protect(void)
{
	if(opentabf == 0)
	{
		opentabf=2;
		if(rdbtable.RdbOpenTab(ISA_SMALLCURRENT_TABNAME)!=RDB_OK){opentabf=0;return;}
		opentabf=1;
	}
}

PROTECTMDL *ISA_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2)
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
ISA_PROTECTACTION	*ISA_Protect::GetActionPara(short actioncode)
{
	return (ISA_PROTECTACTION *)table.RdbGetRecordAddr(ISA_PROTACTION_TABNAME,&actioncode);
}

//取故障信息参数
ISA_PROTECTTROUBLE	*ISA_Protect::GetTroublePara(short troublecode)
{
	return (ISA_PROTECTTROUBLE *)table.RdbGetRecordAddr(ISA_PROTTRBL_TABNAME,&troublecode);
}

//取测量值信息参数
ISA_PROTECTMEASURE	*ISA_Protect::GetMeasurePara(short measurecode)
{
	return (ISA_PROTECTMEASURE *)table.RdbGetRecordAddr(ISA_PROTMSR_TABNAME,&measurecode);
}

//取自检信息参数
ISA_PROTECTDETECT	*ISA_Protect::GetDetectPara(short detectcode)
{
	return (ISA_PROTECTDETECT *)table.RdbGetRecordAddr(ISA_PROTDETECT_TABNAME,&detectcode);
}

//取小电流接地装置参数
ISA_SMALLCUR *ISA_Protect::GetSmallCurPara(short terminalno,short lineno,short linetype)
{
	ISA_SMALLCUR *smcurp;

	int	rcdnums = rdbtable.RdbGetRcdCnt();
	for (int i = 0; i<rcdnums; i++) { 
		smcurp = (ISA_SMALLCUR *) rdbtable.rdbgetrcdaddr(i);
		if (smcurp == NULL)	continue;
		if ((smcurp->terminalno == terminalno) && (smcurp->lineno == lineno) && (smcurp->linetype == linetype))
		{
			return(smcurp);
		}
	}
	return(NULL);
}

//取保护动作描述
int ISA_Protect::GetActionInfo(short actiontype,char *name,char *info)
{
	ISA_PROTECTACTION *actionp = GetActionPara(actiontype);
	if(actionp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,actionp->actioninfo);
	if(info) strcpy(info,actionp->actioninfo);
	return(OK);
}

//取保护自检信息描述
int ISA_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	ISA_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}

//取保护故障描述
int ISA_Protect::GetTroubleInfo(short troubletype,char *info)
{
	ISA_PROTECTTROUBLE *troublep = GetTroublePara(troubletype);
	if(troublep == NULL)return SCADA_ERR_NOOBJECT;
	if(info) strcpy(info,troublep->troubleinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int ISA_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;

	if(gram_len<5) return PROT_ERR_MAILLEN;

	gram[4]=gram[3];
	GetProtinfoTime(gram+4, TIMEBYBCD, &sys_clock);	//发生时间
	mdlp = GetModulePara(terminalno,protocoltype,gram[0],gram[12]);			//微机保护参数

	if(gram[1] == 0x40)		//动作事件
	{
//zl		if(gram[12] == 0 && gram[15] == 0xff)		//小电流接地事件
		if( (gram[13] == 0 && gram[14] == 0) || (gram[13] == 0 && gram[14]== 0xff) )		//小电流接地事件
		{
			ISA_SMALLCUR *smcurp;
//zl			if(gram[16] == 0xff) 
			if(gram[13] == 0xff) 
				return PROT_ERR_MESSAGE;
//zl			if(gram[17] == 0xff)			//"线路号" 0xFF表示纯母线接地
			if(gram[14] == 0xff)			//"线路号" 0xFF表示纯母线接地
			{
//zl				smcurp = GetSmallCurPara(terminalno,gram[16],ISA_SMALLCUR_LINETYPE_MX);
				smcurp = GetSmallCurPara(terminalno,gram[15],ISA_SMALLCUR_LINETYPE_MX);
				make_smallcurrent_apevent(&sys_clock,smcurp);
				save_smallcurrent_event(&sys_clock,smcurp);
			}
//zl			else smcurp = GetSmallCurPara(terminalno,gram[17],ISA_SMALLCUR_LINETYPE_XL);
			else 
			{
				smcurp = GetSmallCurPara(terminalno,gram[15],ISA_SMALLCUR_LINETYPE_XL);
				make_smallcurrent_apevent(&sys_clock,smcurp);
				save_smallcurrent_event(&sys_clock,smcurp);
			}
		}
		else	//微机保护动作事件
		{
			if(mdlp == NULL)return PROT_ERR_MODULE;
			make_action_apevent(&sys_clock,mdlp,gram+13);
			ret = save_action_apevent(&sys_clock,mdlp,gram+13);
		}
	}
	else if(gram[1] == 0x51)	//自检事件
	{
		if(mdlp == NULL)return PROT_ERR_MODULE;
		ret = save_detect_apevent(&sys_clock,mdlp,gram+13);
	}
	else return PROT_ERR_MESSAGE;
	return ret;
}

//生成保护动作应用事件
void ISA_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	ISA_PROTECTACTION *isa_action;

	if(mdlp == NULL || sysclock == NULL)return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_ISA);	//公共信息
	msg.type_value = event[3];			//动作类型
	msg.event_int_info[0] = event[0];	//事件编号
	msg.event_int_info[1] = event[1];	//该动作类型累计发生数
	msg.event_int_info[2] = event[2];	//事故类型
	msg.event_int_info[3] = event[3];	//动作类型
	msg.event_int_info[4] = event[4];	//测量值类型
	msg.event_float_info[0] = (float)(event[5]+event[6]*256)/100.0f;
	
	if((isa_action = GetActionPara(msg.type_value))==NULL)return;	//取动作信息
	msg.sgflag = isa_action->sgflag;
	strcpy(msg.enttypedescribe,isa_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int ISA_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN],longdes[120];
	ISA_PROTECTACTION *isa_action;

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
	if((isa_action = GetActionPara(event[3])) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",isa_action->actioncode);
	strcpy(tmpevent.member_des1,isa_action->actioninfo);
	tmpevent.evt.ToneOut = isa_action->alarmf;
	strcpy(entname,isa_action->entname);

	GetTroubleInfo(event[2], tmpevent.member_des2);	//故障

	ISA_PROTECTMEASURE *ISA_measure = GetMeasurePara(event[4]);
	if(ISA_measure)
	{
		sprintf(longdes,"%s:%4.2f%s",
			ISA_measure->measureinfo,(float)tmpevent.evt.val,ISA_measure->measureunit);
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
int ISA_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	ISA_PROTECTDETECT *isa_detect;

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
	if((isa_detect = GetDetectPara(event[0]))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",isa_detect->detectcode);
	strcpy(tmpevent.member_des1,isa_detect->detectinfo);

	yield_entdescribe(isa_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = isa_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(isa_detect->entname,&tmpevent);
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

//生成小电流接地故障事件
void ISA_Protect::make_smallcurrent_apevent(SYS_CLOCK *sysclock, ISA_SMALLCUR *smallcurp)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	int LSAP_FLAG=0;

	if(sysclock == NULL || smallcurp == NULL)return;
	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,NULL,2,PROTTYPE_ISA);
	strcpy(msg.name,smallcurp->name);
	strcpy(msg.event_char_info,"小电流接地");
	apevent.AddApEvent(APEVENT_PROT_TROUBLE,&msg,sizeof(PROTECT_MSG));
}


//小电流接地事件存盘
void ISA_Protect::save_smallcurrent_event(SYS_CLOCK *sysclock, ISA_SMALLCUR *smallcurp)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];

	entname[0] = 0;
	int LSAP_FLAG=0;

	if(smallcurp == NULL || sysclock == NULL)return;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, NULL, 2);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = smallcurp->alarmf;

	strcpy(tmpevent.evt.member0,smallcurp->name);
	strcpy(tmpevent.member_des0,smallcurp->describe);

	yield_entdescribe(smallcurp->entname,&tmpevent);
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
}	


