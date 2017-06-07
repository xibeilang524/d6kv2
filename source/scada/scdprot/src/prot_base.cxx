#include  "net/netdef.h"
#include  "rdb/rdbapi.h"
#include  "protect.h"
#include  "prot_ISA.h"
#include  "prot_ISA300.h"
#include  "prot_LFP.h"
#include  "prot_DFP.h"
#include  "prot_SEL.h"
#include  "prot_FWB.h"
#include  "prot_DF3003.h"
#include  "prot_CAN2000.h"
#include  "prot_WBX.h"
#include  "prot_DFP500.h"
#include  "prot_LSA_P.h"
#include  "prot_DSA301.h"
#include  "prot_CSC2000.h"
#include  "prot_FBZ.h"
#include  "prot_DWK.h"
#include  "procthread.h"
#include  "prot_DSA301.h"
#include  "prot_ZDB.h"
#include  "prot_DF3003_NEW.h"
#include  "prot_PS.h"
#include  "prot_DEP.h"
#include  "prot_NEWZDB.h"
#include  "prot_NSR.h"
#include  "prot_SACDW.h"
#include  "prot_SACJD.h"
#include  "prot_DF33.h"	//add 2006 09
#include  "prot_NZ103.h"//add 2006 11  for 南自网络103
#include  "prot_RCS_103.h"//add 2006 12  for 南瑞 RCS 103
#include  "prot_DMP.h"	//add 2007 05 for 力导DMP
#include  "prot_DFDZ.h" //add 2007 11 for 东方电子
#include  "prot_AHJY.h"//add 2007 12 for 安徽继远综合自动化

	//////////////////////////////////////////////////////////////////////////////////
	/*																				*/
	/*									微机保护处理基类							*/
	/*																				*/
	//////////////////////////////////////////////////////////////////////////////////

Rdb_QuickQuery BaseProtect::table(MyUserName,MyPassWord);
Rdb_Tab BaseProtect::promdltab;
int BaseProtect::opentabf;

BaseProtect::BaseProtect()
{
	if(opentabf==0)
	{
		opentabf=2;
		if(promdltab.RdbLogin(MyUserName,MyPassWord)!=RDB_OK){opentabf=0;return;}
		if(promdltab.RdbOpenTab(PROTMDL_TABNAME)!=RDB_OK){opentabf=0;return;}
		opentabf=1;
	}
}
							//////////////////////////////////////////
							/*				时间转换处理			*/
							//////////////////////////////////////////
int BaseProtect::GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock,uchar chbyte,uchar nomsflag)
{
	char timeprtstr[128];

	if(nomsflag) 
		sprintf(timeprtstr,"ProtTime%d: 世纪:%.2x 时间:%.2x-%.2x-%.2x %.2x:%.2x:%.2x\n",
			binorbcd,chbyte,gram_part[0],gram_part[1],gram_part[2],gram_part[3],gram_part[4],
			gram_part[5]);
	else
		sprintf(timeprtstr,"ProtTime%d: 世纪:%.2x 时间:%.2x-%.2x-%.2x %.2x:%.2x:%.2x %.2x-%.2x\n",
			binorbcd,chbyte,gram_part[0],gram_part[1],gram_part[2],gram_part[3],gram_part[4],
			gram_part[5],gram_part[6],gram_part[7]);
	RdbDebugMesg(timeprtstr);

	if(binorbcd==TIMEBYBIN){ // 二进制时间格式
		if(chbyte==0xFF) {
			if(gram_part[0] < 98)
				clock->year = gram_part[0] + 2000;
			else
				clock->year = gram_part[0] + 1900;
		}
		else clock->year = gram_part[0] + chbyte * 256;

		clock->month=gram_part[1];
		clock->day=gram_part[2];
		clock->hour=gram_part[3];
		clock->minute=gram_part[4];
		clock->second=gram_part[5];
		if(nomsflag==0)clock->msecond=gram_part[6]+gram_part[7]*256;
		else clock->msecond=0;
	}
	else {	//TIMEBYBCD      BCD码时间格式
		if(chbyte==0xFF) {
			if(BCD1BIN1(gram_part[0]) < 98)
				clock->year = BCD1BIN1(gram_part[0]) + 2000;
			else
				clock->year = BCD1BIN1(gram_part[0]) + 1900;
		}
		else clock->year = BCD1BIN1(gram_part[0]) + BCD1BIN1(chbyte) * 100;

		clock->month=BCD1BIN1(gram_part[1]);
		clock->day=BCD1BIN1(gram_part[2]);
		clock->hour=BCD1BIN1(gram_part[3]);
		clock->minute=BCD1BIN1(gram_part[4]);
		clock->second=BCD1BIN1(gram_part[5]);
		if(nomsflag==0) clock->msecond=BCD1BIN1(gram_part[6])+BCD1BIN1(gram_part[7])*100;
		else clock->msecond=0;
	}
	return OK;
}

							//////////////////////////////////////////////////
							/*			按不同索引方式取微机保护参数		*/
							//////////////////////////////////////////////////

//ISA，LSAP，DFP500，DSA301的保护模块，只根据TERMINALNO，ADDRESS1，ADDRESS2
PROTECTMDL *BaseProtect::GetModulePara(ushort terminalno,ushort address1,ushort address2)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->address1==address1) && (mdlp->address2==address2)) 
		{
			if(mdlp->prottype==PROTTYPE_ISA || mdlp->prottype==PROTTYPE_ISA300 || 
				mdlp->prottype==PROTTYPE_LSA_P || mdlp->prottype==PROTTYPE_DFP500 ||
				mdlp->prottype==PROTTYPE_DSA_301)
			return(mdlp);
		}
	}
	return(NULL);
}

PROTECTMDL *BaseProtect::GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort type2,ushort type3,ushort type4,ushort address1,ushort address2,ushort address3,ushort address4)
{
	PROTECTMDL *mdlp;
	int	rcdnums=promdltab.RdbGetRcdCnt();
	for (int i=0; i<rcdnums; i++) { 
		mdlp=(PROTECTMDL *)promdltab.rdbgetrcdaddr(i);
		if (mdlp==NULL)	continue;
		if ((mdlp->terminalno==terminalno) && (mdlp->prottype==prottype) && (mdlp->type1==type1) &&
			(mdlp->type2==type2) && (mdlp->type3==type3) && (mdlp->type4==type4) &&	(mdlp->address1==address1) &&
			(mdlp->address2==address2) &&(mdlp->address3==address3) && (mdlp->address4==address4)) 
		{
			return(mdlp);
		}
	}
	return(NULL);
}

//形成公用存盘事件信息
void BaseProtect::make_common_saveinfo(SCDEVENT *tmpevent, SYS_CLOCK *sysclock, PROTECTMDL *mdlp, int entflag)
{
	SUBSTATION_STRU *m_pSubStation; //substation
	if(tmpevent == NULL)return;
	tmpevent->evt.sort = ENTCLASS_PROTECT;				//事件分类号
	if(entflag == 0) tmpevent->evt.type = ENTTYPE_PROTECT;				//事件类型
	else if(entflag == 1) tmpevent->evt.type = ENTTYPE_PROTECT_DETECT;
	else tmpevent->evt.type = ENTTYPE_RELAYFAULT;

	//add 2007 04 begin
	if ((mdlp->type3 != 0) && (mdlp->prottype != PROTTYPE_FORTYTWO))
	{//类型3不为零表示以服务器收到的事项的机器时间作为事项的时间，报文上送的时间无效
		SYS_CLOCK curclock;
		GetCurrentClock(&curclock);
		sysclock = &curclock;
	}
	//add 2007 04 end


	if(sysclock) {
		tmpevent->evt.ymd = sysclock->year*10000 + sysclock->month*100 + sysclock->day;	//年月日
		tmpevent->evt.hmsms = sysclock->hour*10000000 + sysclock->minute*100000				//时分秒毫秒
										+ sysclock->second*1000 + sysclock->msecond;
	}

	if(mdlp) {
		strcpy(tmpevent->evt.Object,mdlp->substation);					//组名
		m_pSubStation=(SUBSTATION_STRU *)table.RdbGetRecordAddr(STATION_TABNAME,mdlp->substation);	//组描述
		if(m_pSubStation!=NULL)strcpy(tmpevent->group_des,m_pSubStation->describe);
		strcpy(tmpevent->evt.member0,mdlp->name);										//对象名0
		strcpy(tmpevent->member_des0,mdlp->describe);												//对象名0描述
		tmpevent->evt.security = mdlp->securegrade;											//安全级
	}
	return;
}

//形成公用应用事件信息
void BaseProtect::make_common_apentinfo(PROTECT_MSG *msg, SYS_CLOCK *sysclock, PROTECTMDL *mdlp, int entflag,int protocoltype)
{
	if(msg == NULL || sysclock == NULL)return;
	msg->clock = *sysclock;
	if(mdlp) {
		strcpy(msg->name,mdlp->name);
		strcpy(msg->substation,mdlp->substation);
	}
	msg->protocoltype = protocoltype;
	msg->eventtype = entflag;
	return;
}

//处理FEP服务器保护邮件
int Protect_Scdservice_serv::yieldprotectinfo(SCADA_PROT_STRU* protectp,int  datalen)
{
	int ret;
	scdhostbyte(RDB_DATATYPE_SHORT,&protectp->terminalno);
	NetDbg dbg;

	if (protectp->protocoltype == PROTTYPE_ISA)
	{
		PROTECTMDL	*mdlp=NULL;		//微机保护模块参数指针
		BaseProtect     c_rxprot;
		mdlp = c_rxprot.GetModulePara(protectp->terminalno,protectp->gram[0],protectp->gram[12]);			//微机保护参数
		if(mdlp!=NULL)  protectp->protocoltype = (uchar)mdlp->prottype;
	}

	switch(protectp->protocoltype)
	{
	case PROTTYPE_CAN2000:
		{
			CAN2000_Protect prot;
			RdbDebugMesg("收到CAN2000微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF3003:
		{
			DF3003_Protect prot;
			RdbDebugMesg("收到DF3003微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DFP:
		{
			DFP_Protect prot;
			RdbDebugMesg("收到DFP微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DFP500:
		{
			DFP500_Protect prot;
			RdbDebugMesg("收到DFP500微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DSA_301:
		{
			DSA_301_Protect prot;
			RdbDebugMesg("收到DSA_301微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FWB:
		{
			FWB_Protect prot;
			RdbDebugMesg("收到FWB微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ISA:
		{
			ISA_Protect prot;
			RdbDebugMesg("收到ISA微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ISA300:
		{
			ISA300_Protect prot;
			RdbDebugMesg("收到ISA300微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_LFP:
		{
			LFP_Protect prot;
			RdbDebugMesg("收到LFP微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_LSA_P:
		{
			LSA_P_Protect prot;
			RdbDebugMesg("收到LSA_P微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;

	case PROTTYPE_SEL:
		{
			SEL_Protect prot;
			RdbDebugMesg("收到SEL微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_WBX:
		{
			WBX_Protect prot;
			RdbDebugMesg("收到WBX微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_CSC2000:
		{
			CSC2000_Protect prot;
			RdbDebugMesg("收到CSC2000微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FBZ:
		{
			FBZ_Protect prot;
			RdbDebugMesg("收到FBZ微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DWK:
		{
			DWK_Protect prot;
			RdbDebugMesg("收到DWK电压无功综合装置邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_ZDB://2003 03
		{
			ZDB_Protect prot;
			RdbDebugMesg("收到ZDB微机保护邮件\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到ZDB微机保护邮件");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_NEWZDB://2004 08
		{
			NEWZDB_Protect prot;
			RdbDebugMesg("收到NEWZDB微机保护邮件\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到NEWZDB微机保护邮件");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF3003_NEW://2003 03
		{
			DF3003_NEW_Protect prot;
			RdbDebugMesg("收到DF3003_NEW微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DEP://2004 06
		{
			DEP_Protect prot;
			RdbDebugMesg("收到DEP微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_PS://2004 05
		{
			PS_Protect prot;
			RdbDebugMesg("收到PS6000微机保护邮件\n");
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"收到PS6000微机保护邮件");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_SACDW:
		{
			SACDW_Protect prot;
			RdbDebugMesg("收到SACDW微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_DF33://add 2006 09 平阴 
		{
			DF33_Protect prot;
			RdbDebugMesg("收到DF33微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYFIVE://add 2006 11  for 南自网络103
		{
			NZ103_Protect prot;
			RdbDebugMesg("收到NZ103微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYSEVEN://add 2006 12  for 南瑞 RCS网络103
	case PROTTYPE_FORTYNINE://add 2007 06  for 南瑞 串口103
		{
			RCS_103_Protect prot;
			RdbDebugMesg("收到RCS103微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FORTYEIGHT://add 2007 05  for 力导DMP
		{
			DMP_Protect prot;
			RdbDebugMesg("收到DMP微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FIFTYZERO://add 2007 11  for 东方电子
		{
			DFDZ_Protect prot;
			RdbDebugMesg("收到东方电子微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_FIFTYONE://add 2007 12  for 安徽继远综合自动化
		{
			AHJY_Protect prot;
			RdbDebugMesg("收到安徽继远综合自动化微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_SACJD:
		{
			SACJD_Protect prot;
			RdbDebugMesg("收到SACJD微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	case PROTTYPE_NSR:
		{
			NSR_Protect prot;
			RdbDebugMesg("收到NSR微机保护邮件\n");
			ret = prot.protectinfo(protectp->terminalno,protectp->protocoltype,protectp->gram_len,protectp->gram);
		}
		break;
	default:
		{
			dbg.PutDbg(NET_DBGTYPE_COPYSCR,"无法识别的规约类型");
			ret = PROT_ERR_NOTSUPPORT;   
			break;
		}
	}
	return ret;
}


