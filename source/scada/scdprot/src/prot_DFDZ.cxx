#ifdef __alpha
#include<time.h>
#endif
#include "scadaproc.h"
#include "procthread.h"
#include "prot_DFDZ.h"
#include "math.h"


				//////////////////////////////////////////////////////////////
				/*					DFDZ 微机保护处理类					*/
				//////////////////////////////////////////////////////////////



DFDZ_Protect::DFDZ_Protect(void)
{
	m_GramLength=0;
	memset(m_Buf,0,sizeof(char)*1024);
}

PROTECTMDL *DFDZ_Protect::GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort address3,ushort address4)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->address1==address1)
			&& (mdlp->address2==address2) && (mdlp->address3==address3) && (mdlp->address4==address4))
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//取保护动作参数
DFDZ_PROTECTACTION	*DFDZ_Protect::GetActionPara(short actiontype,short actioncode)
{
	ACTION_KEY_DFDZ key;
	key.moduletype = actiontype;
	key.actioncode = actioncode;
	return (DFDZ_PROTECTACTION *)table.RdbGetRecordAddr(DFDZ_PROTACTION_TABNAME,&key);
}


//取自检信息参数
DFDZ_PROTECTDETECT	*DFDZ_Protect::GetDetectPara(short detectcode)
{
	return (DFDZ_PROTECTDETECT *)table.RdbGetRecordAddr(DFDZ_PROTDETECT_TABNAME,&detectcode);
}

//取量测信息参数
DFDZ_PROTECTMEASURE	*DFDZ_Protect::GetMeasurePara(short measurecode)
{
	return (DFDZ_PROTECTMEASURE *)table.RdbGetRecordAddr(DFDZ_PROTMEASURE_TABNAME,&measurecode);
}

//取保护自检信息描述
int DFDZ_Protect::GetDetectInfo(short detecttype,char *name,char *info)
{
	DFDZ_PROTECTDETECT *detectp = GetDetectPara(detecttype);
	if(detectp == NULL) return SCADA_ERR_NOOBJECT;
	if(name) strcpy(name,detectp->detectinfo);
	if(info) strcpy(info,detectp->detectinfo);
	return(OK);
}


//解释FEP服务器保护生报文
int DFDZ_Protect::protectinfo(short terminalno, unsigned char protocoltype, unsigned char gram_len, unsigned char *gram)
{
	SYS_CLOCK  sys_clock;
	PROTECTMDL	*mdlp;		//微机保护参数指针
	int ret;
	NetDbg dbg;
	dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到DFDZ的保护事件邮件");
	if(gram_len<10) return PROT_ERR_MAILLEN;

	int i,index =0;
	for (i=0;i<gram_len;i++)
	{
		m_Buf[i] = gram[i];
	}

	uchar eventnums = m_Buf[0];				//事件的数目
	m_address1=m_Buf[1];					//地址	对应微机保护参数保护参数表中得地址1
	m_address2=m_Buf[2];					//地址	对应微机保护参数保护参数表中得地址1
	m_address3=m_Buf[3];					//地址	对应微机保护参数保护参数表中得地址1
	m_address4=m_Buf[4];					//地址	对应微机保护参数保护参数表中得地址1
	mdlp = GetModulePara(terminalno,protocoltype,m_address1,m_address2,m_address3,m_address4);			//微机保护参数

	if(mdlp == NULL) return PROT_ERR_MODULE;
	

	index = 5;
	for (i=0;i<eventnums;i++)
	{
		if (m_Buf[index+3] == 0x0b)//动作事件
		{
			//微机保护动作事件
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入生成动作事件");
			char text[60];
			sprintf(text,"终端号:%d\t规约类型:%d\t地址:%d %d %d %d",terminalno,protocoltype,m_address1,m_address2,m_address3,m_address4);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,text);
			GetProtinfoTime(m_Buf+index+4, TIMEBYBIN, &sys_clock);		//发生时间
			make_action_apevent(&sys_clock,mdlp,m_Buf+index);
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存动作事件");
			ret = save_action_apevent(&sys_clock,mdlp,m_Buf+index);
			uchar measurenum=m_Buf[index+8];
			index += 8;
			index += 1;
			index += measurenum *5;//如果是整形就是3个 ，浮点型为5个
		}
		else if (m_Buf[index+3] == 0x0a)//自检事件------异常事件
		{
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"进入保存自检事件");
			GetProtinfoTime(m_Buf+index+4, TIMEBYBIN, &sys_clock);		//发生时间
			ret = save_detect_apevent(&sys_clock,mdlp,m_Buf+index);
			index += 8;
		}
		else return PROT_ERR_MESSAGE;
	}


	return ret;
}

//生成保护动作应用事件
void DFDZ_Protect::make_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	APEvent   apevent;	//应用事件处理类
	PROTECT_MSG msg;
	DFDZ_PROTECTACTION *DFDZ_action;

	if(mdlp == NULL || sysclock == NULL) return;

	memset(&msg,0,sizeof(PROTECT_MSG));
	make_common_apentinfo(&msg,sysclock,mdlp,0,PROTTYPE_FIFTYZERO);	//公共信息
	short inf = event[0] + event[1]*256;
	msg.type_value = inf;			//动作类型
	
	if((DFDZ_action = GetActionPara(mdlp->type1,inf))==NULL)return;	//取动作信息
	msg.sgflag = DFDZ_action->sgflag;
	strcpy(msg.enttypedescribe,DFDZ_action->actioninfo);		//事件类型描述
	apevent.AddApEvent(APEVENT_PROT,&msg,sizeof(PROTECT_MSG));
}


//动作事项存盘
int DFDZ_Protect::save_action_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN],longdes[120];
	int i = 0;

	entname[0] = 0;

	DFDZ_PROTECTACTION *DFDZ_action;
	DFDZ_PROTECTMEASURE *DFDZ_measure;

	if(mdlp == NULL || sysclock == NULL) return PROT_ERR_ACTION;

	short inf = event[0] +event[1]*256;

	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 0);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	/////////生成存盘事项//////////////////////////////////////////////////

	tmpevent.evt.state = inf;			//动作类型-----状态值

	if((DFDZ_action = GetActionPara(mdlp->type1,inf)) == NULL) return PROT_ERR_ACTION;	//取动作信息

	sprintf(tmpevent.evt.member1,"%d",DFDZ_action->actioncode);
	strcpy(tmpevent.member_des1,DFDZ_action->actioninfo);
	tmpevent.evt.ToneOut = DFDZ_action->alarmf;
	strcpy(entname,DFDZ_action->entname);

	uchar measurenum = event[8];

	if (measurenum>3) 
		measurenum = 3;//事项描述太长有问题

	for(i=0;i<measurenum;i++)
	{
		short measureno = event[5*i +9];
		DFDZ_measure = GetMeasurePara(measureno);
		if(DFDZ_measure){
			float measureval =0;
			char ttt[4];
#if( ( defined(__sun) && defined(__sparc) ) || defined(_AIX) || defined(__hpux))	
			ttt[0] = event[9+ 5*i +4];
			ttt[1] = event[9+ 5*i +3];
			ttt[2] = event[9+ 5*i +2];
			ttt[3] = event[9+ 5*i +1];
#else
			ttt[0] = event[9+ 5*i +1];
			ttt[1] = event[9+ 5*i +2];
			ttt[2] = event[9+ 5*i +3];
			ttt[3] = event[9+ 5*i +4];
#endif

			memcpy(&measureval,ttt,4);
			tmpevent.evt.val = measureval;	//测量值
			sprintf(longdes,"%s:%4.2f%s",
				DFDZ_measure->measureinfo,(float)tmpevent.evt.val,DFDZ_measure->measureunit);
			if((strlen(tmpevent.longdes)+strlen(longdes))>=120) break;
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
	}
	return OK;
}

//自检事项存盘
int DFDZ_Protect::save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event)
{
	SCDEVENT tmpevent;	//存盘事件
	char entname[ENTNAME_LEN];
	DFDZ_PROTECTDETECT *DFDZ_detect;

	entname[0] = 0;

	if(mdlp == NULL || sysclock == NULL)return PROT_ERR_DETECT;

	short inf = event[0] + event[1]*256 ;	//INF值---自检事件类型
	memset(&tmpevent,0,sizeof(SCDEVENT));
	make_common_saveinfo(&tmpevent, sysclock, mdlp, 1);
	tmpevent.evt.DiskOut = 1;			//存盘标志
	tmpevent.evt.PrintOut = 1;			//打印标志
	tmpevent.evt.DispOut = 1;			//登录标志
	tmpevent.evt.GraphOut = 0;			//推图标志
	tmpevent.evt.ToneOut = 0;

	tmpevent.evt.state = inf;
	if((DFDZ_detect = GetDetectPara(inf))==NULL)return PROT_ERR_DETECT;
	sprintf(tmpevent.evt.member1,"%d",DFDZ_detect->detectcode);
	strcpy(tmpevent.member_des1,DFDZ_detect->detectinfo);

	yield_entdescribe(DFDZ_detect->entname,&tmpevent);
	tmpevent.evt.ToneOut = DFDZ_detect->alarmf;
	if(tmpevent.evt.ToneOut)yield_snddescribe(DFDZ_detect->entname,&tmpevent);
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


int DFDZ_Protect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock)
{
	if(clock == NULL)return -1;
	uchar hour,minute,second;
	short ms;
	ms = (gram_part[0] + gram_part[1]*256)%1000;
	second = (gram_part[0] + gram_part[1]*256)/1000;
	minute = gram_part[2];
	hour = gram_part[3];

	SYS_CLOCK sysclock;
	GetCurrentClock(&sysclock);	
	clock->year = sysclock.year;
	clock->month = sysclock.month;
	clock->day = sysclock.day;
	clock->hour=hour;
	clock->minute=minute;
	clock->second=second;
	clock->msecond = ms;
	return OK;
}