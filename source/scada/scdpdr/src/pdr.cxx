
#include <memory.h>
#include "procthread.h"
#include "scadainterface.h"
#include "scadaproc.h"
#include "scadahislib.h"
#include "math.h"
#include "pdr.h"
#include "commfunc.h"

extern int pdrgroup_initflag;
extern PDRGROUP_MANINFO  pdrgrp_maninfo[MAXPDRGROUPNUMS];

////////////////////////////////////////////////////////////////////////////////////
void LockObjPdr(uchar zygrpno)
{
	RequireToken(&pdrgrp_maninfo[zygrpno].lockflag,4000);
}

void UnlockObjPdr(uchar zygrpno)
{
	FreeToken(&pdrgrp_maninfo[zygrpno].lockflag);
}

////////////////////////////////////////////////////////////////////////////////////

PdrGroupMan::PdrGroupMan(void)
{
	if(pdrgroup_initflag==0) {
		memset(pdrgrp_maninfo,0,sizeof(pdrgrp_maninfo));
		pdrgroup_initflag=1;
	}
}

PdrGroupMan::~PdrGroupMan(void)
{
}


void PdrGroupMan::ProcAllPdrGroup(void)
{
	int i;
	for(i=0;i<MAXPDRGROUPNUMS;i++) {
		if(pdrgrp_maninfo[i].availflag) {
			pdrgrp_maninfo[i].timecount++;
			if(pdrgrp_maninfo[i].timecount>=pdrgrp_maninfo[i].sampdensity) {
				LockObjPdr(i);
				pdrgrp_maninfo[i].objpdr->FreshObjData();
				pdrgrp_maninfo[i].trigflag=0;
				UnlockObjPdr(i);
				pdrgrp_maninfo[i].timecount=0;
			}
		}
	}
}

void PdrGroupMan::UpdAllPdrPara(void)
{
	int groupnums,i;
	PDRGROUP_STRU *spgi,tmpspgi;

	for(i=0;i<MAXPDRGROUPNUMS;i++) pdrgrp_maninfo[i].initedflag=0;

	Rdb_Tab rdbtable(MyUserName,MyPassWord);
	if(rdbtable.RdbOpenTab(PDRGRP_TABNAME)!=RDB_OK)return;
	groupnums = rdbtable.RdbGetRcdCnt();		//取得分组追忆组个数
	for(i=0;i<groupnums;i++) {
		spgi = (PDRGROUP_STRU *)rdbtable.RdbGetRecordAddr(i);
		if(spgi==NULL)continue;
		tmpspgi=*spgi;
		if(tmpspgi.zygrpno>=0 && tmpspgi.zygrpno<MAXPDRGROUPNUMS) {
			if(!pdrgrp_maninfo[tmpspgi.zygrpno].availflag) {
				pdrgrp_maninfo[tmpspgi.zygrpno].objpdr=new ObjectDataPdr;

				LockObjPdr(tmpspgi.zygrpno);
				strcpy(pdrgrp_maninfo[tmpspgi.zygrpno].groupname,tmpspgi.groupname);
				pdrgrp_maninfo[tmpspgi.zygrpno].sampdensity=tmpspgi.sampledensity;
				pdrgrp_maninfo[tmpspgi.zygrpno].preframenums=tmpspgi.prepoints;
				pdrgrp_maninfo[tmpspgi.zygrpno].bakframenums=tmpspgi.postpoints;
				pdrgrp_maninfo[tmpspgi.zygrpno].maxframenums=tmpspgi.prepoints+tmpspgi.postpoints+5;
				SetPdrPara(&tmpspgi,pdrgrp_maninfo[tmpspgi.zygrpno].maxframenums);
				UnlockObjPdr(tmpspgi.zygrpno);

				pdrgrp_maninfo[tmpspgi.zygrpno].availflag=1;
			}
			else {
				LockObjPdr(tmpspgi.zygrpno);
				strcpy(pdrgrp_maninfo[tmpspgi.zygrpno].groupname,tmpspgi.groupname);
				pdrgrp_maninfo[tmpspgi.zygrpno].sampdensity=tmpspgi.sampledensity;
				pdrgrp_maninfo[tmpspgi.zygrpno].preframenums=tmpspgi.prepoints;
				pdrgrp_maninfo[tmpspgi.zygrpno].bakframenums=tmpspgi.postpoints;
				pdrgrp_maninfo[tmpspgi.zygrpno].maxframenums=tmpspgi.prepoints+tmpspgi.postpoints+5;
				SetPdrPara(&tmpspgi,pdrgrp_maninfo[tmpspgi.zygrpno].maxframenums);
				UnlockObjPdr(tmpspgi.zygrpno);
			}
			pdrgrp_maninfo[tmpspgi.zygrpno].initedflag=1;
		}
	}

	for(i=0;i<MAXPDRGROUPNUMS;i++) {
		if(pdrgrp_maninfo[i].availflag && pdrgrp_maninfo[i].initedflag==0) {
			LockObjPdr(tmpspgi.zygrpno);
			pdrgrp_maninfo[i].objpdr->SetObjDataPara(i,0,0,NULL);
			UnlockObjPdr(tmpspgi.zygrpno);
		}
	}

}

static int  SWITCH_PDR_MODE=GetProgramRunPara("swpdrmode",0);//刀闸是否追忆


inline int PdrAnaParaSelect(PDRGROUP_STRU *spgi,BASEANALOG_STRU *anap)
{
	int	i,findflag;
	findflag=0;

	for(i=0;i<ZY_SELSUBST_NUMS;i++) {
		if(spgi->selsubst[i][0]!='\0') break;
	}

	if(i>=ZY_SELSUBST_NUMS)findflag=1;
	else {
		for(i=0;i<ZY_SELSUBST_NUMS;i++) {
			if(spgi->selsubst[i][0]!='\0' && strcmp(spgi->selsubst[i],anap->substation)==0) {
				findflag=1;
				break;
			}
		}
	}
	if(!findflag) return(0);

	findflag=0;
	for(i=0;i<ZY_SELVOLT_NUMS;i++) {
		if(spgi->selvolt[i]>0)break;
	}
	if(i>=ZY_SELVOLT_NUMS) findflag=1;
	else {
		for(i=0;i<ZY_SELVOLT_NUMS;i++) {
//			if(spgi->selvolt[i]>0 && spgi->selvolt[i]==((ANALOG_STRU *)anap)->voltgrade) {
			if(spgi->selvolt[i]>0) {
				findflag=1;
				break;
			}
		}
	}
	return(findflag);
}

inline int PdrStaParaSelect(PDRGROUP_STRU *spgi,BASESTATUS_STRU *stap)
{
	int	i,findflag;
	findflag=0;

	for(i=0;i<ZY_SELSUBST_NUMS;i++) {
		if(spgi->selsubst[i][0]!='\0') break;
	}

	if(i>=ZY_SELSUBST_NUMS)findflag=1;
	else {
		for(i=0;i<ZY_SELSUBST_NUMS;i++) {
			if(spgi->selsubst[i][0]!='\0' && strcmp(spgi->selsubst[i],stap->substation)==0) {
				findflag=1;
				break;
			}
		}
	}
	if(!findflag) return(0);

	findflag=0;
	for(i=0;i<ZY_SELVOLT_NUMS;i++) {
		if(spgi->selvolt[i]>0)break;
	}
	if(i>=ZY_SELVOLT_NUMS) findflag=1;
	else {
		for(i=0;i<ZY_SELVOLT_NUMS;i++) {
//			if(spgi->selvolt[i]>0 && spgi->selvolt[i]==stap->voltgrade) {
			if(spgi->selvolt[i]>0) {
				findflag=1;
				break;
			}
		}
	}
	return(findflag);
}

void PdrGroupMan::SetPdrPara(PDRGROUP_STRU *spgi,int maxfnums)
{
	int i,ret,rcdcnt;
	char *tmpobjname;
	short tmpobjtype;
	OBJDATA_PARA  *tmpobjparap;
	Rdb_Tab  analogtbl(MyUserName, MyPassWord);
	Rdb_Tab  breaktab(MyUserName, MyPassWord);
	Rdb_Tab  switchtab(MyUserName, MyPassWord);
	short tmpzygrpno=0;//2004 05 add

	if(spgi->zycondition) {
		Rdb_PreMalloc snpobjmalloc(1000,sizeof(OBJDATA_PARA));
		BASEANALOG_STRU	*tmpanap;
		BASESTATUS_STRU	*tmpstap;

		analogtbl.RdbOpenTab(ANALOG_TABNAME);
		rcdcnt = analogtbl.RdbGetRcdCnt();
		for(i=0;i<rcdcnt;i++) {
			tmpanap=(BASEANALOG_STRU *)analogtbl.RdbGetRecordAddr(i);
			if(tmpanap==NULL)continue;
			if(PdrAnaParaSelect(spgi,tmpanap)) {
				tmpobjparap=(OBJDATA_PARA  *)snpobjmalloc.get_nextunitaddr();
				strcpy(tmpobjparap->objname,tmpanap->name);
				tmpobjparap->objtype=tmpanap->baseobjtype;
				tmpobjparap->tabtype=PDROBJ_TBLTYPE_ANALOG;
				tmpobjparap->tabrcdno=i;
			}
		}

		breaktab.RdbOpenTab(BREAK_TABNAME);
		rcdcnt = breaktab.RdbGetRcdCnt();
		for(i=0;i<rcdcnt;i++) {
			tmpstap=(BASESTATUS_STRU *)breaktab.RdbGetRecordAddr(i);
			if(tmpstap==NULL)continue;
			if(PdrStaParaSelect(spgi,tmpstap)) {
				tmpobjparap=(OBJDATA_PARA  *)snpobjmalloc.get_nextunitaddr();
				strcpy(tmpobjparap->objname,tmpstap->name);
				tmpobjparap->objtype=tmpstap->baseobjtype;
				tmpobjparap->tabtype=PDROBJ_TBLTYPE_BREAK;
				tmpobjparap->tabrcdno=i;
			}
		}
		switchtab.RdbOpenTab(SWITCH_TABNAME);
/*		if(spgi->selswitchf) {
			rcdcnt = switchtab.RdbGetRcdCnt();
			for(i=0;i<rcdcnt;i++) {
				tmpstap=(BASESTATUS_STRU *)switchtab.RdbGetRecordAddr(i);
				if(tmpstap==NULL)continue;
				if(PdrStaParaSelect(spgi,tmpstap)) {
					tmpobjparap=(OBJDATA_PARA  *)snpobjmalloc.get_nextunitaddr();
					strcpy(tmpobjparap->objname,tmpstap->name);
					tmpobjparap->objtype=tmpstap->baseobjtype;
					tmpobjparap->tabtype=PDROBJ_TBLTYPE_SWITCH;
					tmpobjparap->tabrcdno=i;
				}
			}
		}*/
		tmpobjparap=(OBJDATA_PARA *)snpobjmalloc.get_mallocinfo(&rcdcnt);
		if(rcdcnt>0 && tmpobjparap!=NULL) {
			pdrgrp_maninfo[spgi->zygrpno].objpdr->SetObjDataPara(spgi->zygrpno,
											maxfnums,rcdcnt,tmpobjparap);
		}
	}
	else {
		Rdb_SQL rsql;
		rsql.RdbSelectFrom(PDRPARA_TABNAME);
		rsql.RdbSelectField("objname");
		rsql.RdbSelectField("baseobjtype");
		tmpzygrpno =(short)spgi->zygrpno;
		rsql.RdbWhereCond("zygrpno",RDB_DATATYPE_SHORT,&tmpzygrpno);
		rsql.RdbOrderBy("objname");
		ret=rsql.RdbSelCmd(MyUserName,MyPassWord);
		if(ret!=0)return;
		rcdcnt=rsql.RdbGetRcdCnt();
		if(rcdcnt<=0)return;

		Rdb_Malloc  partmalloc(rcdcnt * sizeof(OBJDATA_PARA));
		tmpobjparap=(OBJDATA_PARA  *)partmalloc.GetAddr();

		analogtbl.RdbOpenTab(ANALOG_TABNAME);
		breaktab.RdbOpenTab(BREAK_TABNAME);
		switchtab.RdbOpenTab(SWITCH_TABNAME);

		for(i=0;i<rcdcnt;i++) {
			tmpobjname = rsql.RdbGetString(i,"objname");
			if(tmpobjname == NULL)return;
			tmpobjtype = rsql.RdbGetVal_int(i,"baseobjtype");
			strcpy(tmpobjparap[i].objname,tmpobjname);
			tmpobjparap[i].objtype=tmpobjtype;
			if(IS_YC_TYPE(tmpobjtype)) {
				tmpobjparap[i].tabrcdno=analogtbl.RdbSearchRcdByKey(tmpobjname);
				tmpobjparap[i].tabtype=PDROBJ_TBLTYPE_ANALOG;
			}
			else if(IS_BREAK_TYPE(tmpobjtype)){
				tmpobjparap[i].tabrcdno=breaktab.RdbSearchRcdByKey(tmpobjname);
				tmpobjparap[i].tabtype=PDROBJ_TBLTYPE_BREAK;
			}
			else if(IS_SWITCH_TYPE(tmpobjtype)){
				tmpobjparap[i].tabrcdno=switchtab.RdbSearchRcdByKey(tmpobjname);
				tmpobjparap[i].tabtype=PDROBJ_TBLTYPE_SWITCH;
			}
		}
		pdrgrp_maninfo[spgi->zygrpno].objpdr->SetObjDataPara(spgi->zygrpno,
										maxfnums,rcdcnt,tmpobjparap);
	}

}


/////////////////////////////////////////////////////////////////////////////////
int ObjectDataPdr::opentabf;
Rdb_Tab	ObjectDataPdr::anatab,ObjectDataPdr::brktab,ObjectDataPdr::swtab;

ObjectDataPdr::ObjectDataPdr(void)
{
	if(!opentabf) {
		opentabf=2;
		anatab.RdbLogin(MyUserName,MyPassWord);
		anatab.RdbOpenTab(ANALOG_TABNAME);
		brktab.RdbLogin(MyUserName,MyPassWord);
		brktab.RdbOpenTab(BREAK_TABNAME);
		swtab.RdbLogin(MyUserName,MyPassWord);
		swtab.RdbOpenTab(SWITCH_TABNAME);
		opentabf=1;
	}
	lockflag=0;
	pdrobjnums=0;
	maxframenums=0;
	curframeno=0;
	objdatapara=NULL;
	objdata=NULL;
	quality=NULL;
}

ObjectDataPdr::~ObjectDataPdr(void)
{
	InitObjDataPdr();
}

void ObjectDataPdr::InitObjDataPdr(void)
{
	pdrobjnums=0;
	maxframenums=0;
	curframeno=0;
	if(objdatapara) {
		delete []objdatapara;objdatapara=NULL;
	}
	if(objdata) {
		delete []objdata;objdata=NULL;
	}
	if(quality) {
		delete []quality;quality=NULL;
	}
}

int ObjectDataPdr::SetObjDataPara(uchar grpno,int maxfnums,int objnums,OBJDATA_PARA *inpara)
{
	int i,initflag=0;

	if(grpno<0 || grpno>=MAXPDRGROUPNUMS)return(OK);

	if(objnums<=0 || maxfnums<=0 || inpara==NULL) {
		InitObjDataPdr();
		return(OK);
	}
	zygrpno=grpno;

	if(pdrobjnums>0 && maxframenums>0) {
		if(objnums!=pdrobjnums) initflag=1;
		else if(maxframenums!=maxfnums)initflag=1;
		else {
			for(i=0;i<objnums;i++) {
				objdatapara[i].objtype=inpara[i].objtype;
				if(strcmp(inpara[i].objname,objdatapara[i].objname))
					initflag=1;
				else if(inpara[i].tabtype!=objdatapara[i].tabtype)
					initflag=1;
				else if(inpara[i].tabrcdno!=objdatapara[i].tabrcdno)
					initflag=1;
			}
		}
	}
	else initflag=1;

	if(initflag==0) {
		for(i=0;i<objnums;i++) {
			objdatapara[i].objtype=inpara[i].objtype;
		}
	}
	else {
		InitObjDataPdr();
		pdrobjnums=objnums;
		maxframenums=maxfnums;
		curframeno=0;
		objdatapara=new OBJDATA_PARA[pdrobjnums];
		memcpy(objdatapara,inpara,pdrobjnums * sizeof(OBJDATA_PARA));
		objdata= new float[pdrobjnums * maxframenums];
		memset(objdata,0,pdrobjnums * maxframenums * sizeof(float));
		quality= new uchar[pdrobjnums * maxframenums];
		memset(quality,0,pdrobjnums * maxframenums * sizeof(uchar));
	}
	return(OK);
}

void	ObjectDataPdr::FreshObjData(void)
{
	BASEANALOG_STRU	*tmpanap;
	BASESTATUS_STRU	*tmpstap;
	int i,tmpno,dataokf;

	if(pdrobjnums<=0)return;

	tmpno=curframeno;
	for(i=0;i<pdrobjnums;i++) {
		dataokf=0;
		if(objdatapara[i].tabrcdno>=0) {
			if(objdatapara[i].tabtype==PDROBJ_TBLTYPE_ANALOG) {
				tmpanap=(BASEANALOG_STRU	*)anatab.RdbGetRecordAddr(objdatapara[i].tabrcdno);
				if(tmpanap) {
					objdata[tmpno]  =tmpanap->value;
					quality[tmpno] = (tmpanap->mansetf<<7) | (tmpanap->terminalerror<<6) | tmpanap->deadf;
					dataokf=1;
				}
			}
			else if(objdatapara[i].tabtype==PDROBJ_TBLTYPE_BREAK) {
				tmpstap=(BASESTATUS_STRU *)brktab.RdbGetRecordAddr(objdatapara[i].tabrcdno);
				if(tmpstap) {
					objdata[tmpno]  =(float)tmpstap->value;
					quality[tmpno] = (tmpstap->mansetf<<7) | (tmpstap->terminalerror<<6) | tmpstap->changef;
					dataokf=1;
				}
			}
			else if(objdatapara[i].tabtype==PDROBJ_TBLTYPE_SWITCH) {
				tmpstap=(BASESTATUS_STRU *)swtab.RdbGetRecordAddr(objdatapara[i].tabrcdno);
				if(tmpstap) {
					objdata[tmpno]=(float)tmpstap->value;
					quality[tmpno]=(tmpstap->mansetf<<7) | (tmpstap->terminalerror<<6) | tmpstap->changef;
					dataokf=1;
				}
			}
		}
		if(dataokf==0) {
			objdata[tmpno]=(float)0.0;
			quality[tmpno]=255;
		}
		tmpno+=maxframenums;
	}
	if((curframeno+1)>=maxframenums)curframeno=0;
	else curframeno++;
}


void makefilename(uchar zygrpno,int timeflag,char *namebuf)
{
	char filename[128];
	sprintf(filename,"追忆%d_%d.dat",zygrpno,timeflag);
	char *sacenv=getenv(NBENV);
	if(sacenv==NULL) {
		strcpy(namebuf,filename);
	}
	else {
		strcpy(namebuf,sacenv);
	#ifdef WIN32
		strcat(namebuf,"\\procscript\\");
	#else
		strcat(namebuf,"/procscript/");
	#endif
		strcat(namebuf,filename);
	}
}


void	ObjectDataPdr::SavePdrData(PDR_SAVETASK *pstask,int savemode)
{
	int i,totalnums=0,prefnums,bakfnums,frameno,ret;
	FILE *outfp;
	SPdrDescribe	tmpspdrdes;

	prefnums=pstask->preframenums;
	bakfnums=pstask->bakframenums;
	frameno=pstask->trigframeno;
	
	if(prefnums<0 || bakfnums<0 || frameno<0)return;
	if((prefnums+bakfnums)>=maxframenums)return;

	int preframeno=(maxframenums+frameno-prefnums)%maxframenums;
	int bakframeno=(maxframenums+frameno+bakfnums)%maxframenums;
	Rdb_Malloc		savedatamalloc(pdrobjnums * sizeof(SPdrSaveData));
	SPdrSaveData	*tmpsdata=(SPdrSaveData	*)savedatamalloc.GetAddr();
	char filename[200];

	makefilename(zygrpno,pstask->trigtime,filename);
	outfp=fopen(filename,"wb");
	if(outfp==NULL)return;

	int tmpframeno,tmpno,lasttmpno,lastframeno,savenums,changef;
	int frameid=-prefnums;

	lastframeno=tmpframeno=preframeno;
	while(tmpframeno!=bakframeno) {
		tmpno=tmpframeno;
		lasttmpno=lastframeno;
		savenums=0;
		for(i=0;i<pdrobjnums;i++) {
			changef=0;
			if(savemode==1)changef=1;
			else if(tmpframeno==preframeno) changef=1;
			else if(objdata[lasttmpno]!=objdata[tmpno])changef=1;

			if(changef) {
				if(objdatapara[i].objname[0]!='\0') {
					tmpsdata[savenums].zygrpno=zygrpno;
					tmpsdata[savenums].frameno=frameid;
					tmpsdata[savenums].timeflag=pstask->trigtime;
					strcpy(tmpsdata[savenums].objname,objdatapara[i].objname);
					tmpsdata[savenums].value=objdata[tmpno];
					tmpsdata[savenums].baseobjtype=objdatapara[i].objtype;
					tmpsdata[savenums].qualityf=quality[tmpno];
					savenums++;
				}
			}

			tmpno+=maxframenums;
			lasttmpno+=maxframenums;
		}

		if(savenums>0) {
			ret=fwrite(tmpsdata,sizeof(SPdrSaveData),savenums,outfp);
			if(ret>0)totalnums+=ret;
		}

		lastframeno=tmpframeno;
		frameid++;
		tmpframeno++;
		if(tmpframeno>=maxframenums)tmpframeno=0;
	}

	fclose(outfp);


	memset(&tmpspdrdes,0,sizeof(tmpspdrdes));
	strcpy(tmpspdrdes.breakername,pstask->triggername);
	strcpy(tmpspdrdes.cause,pstask->cause);
	tmpspdrdes.timeflag=pstask->trigtime;
	tmpspdrdes.ymd=pstask->ymd;
	tmpspdrdes.hmsms=pstask->hmsms;
	tmpspdrdes.zygrpno=pstask->zygrpno;
	tmpspdrdes.causetype=pstask->causetype;
	tmpspdrdes.sampledensity=pdrgrp_maninfo[zygrpno].sampdensity;
	tmpspdrdes.preframenums=prefnums;
	tmpspdrdes.postframenums=bakfnums;

	PdrDBTaskMan  pdbtask;
	pdbtask.AddPdrDBTask(&tmpspdrdes,totalnums,filename);
}

/////////////////////////////////////////////////////////////////
ComnLinkTable<PDR_SAVETASK>		PdrSaveTaskMan::savetask;

PdrSaveTaskMan::PdrSaveTaskMan(void)
{
}

PdrSaveTaskMan::~PdrSaveTaskMan(void)
{
}

void PdrSaveTaskMan::AddPdrSaveTask(uchar zygrpno,int trigtime,char *triggername,
									SYS_CLOCK *entclock,int causetype,char *cause,
									int prefnums,int bakfnums)
{
	if(zygrpno<0 || zygrpno>=MAXPDRGROUPNUMS)return;

	//////// 检查是否有重复的追忆任务 ///////////////
	COMN_POSITION  curposition,nextposition;
	PDR_SAVETASK  *staskp;
	int findflag=0;

	savetask.LockLink();
	curposition=savetask.GetFirstNode();
	while(curposition) {
		staskp=savetask.NodeData(curposition);
		nextposition=savetask.GetNextNode(curposition);
		if(staskp->zygrpno==zygrpno) {
			if(ABS(staskp->trigtime-trigtime)<pdrgrp_maninfo[zygrpno].sampdensity) {
				findflag=1;
				break;
			}
		}
		curposition=nextposition;
	}
	savetask.UnlockLink();
	if(findflag)return;
	///////////////////////////////////////////////

	int tmpint;
	PDR_SAVETASK  tmppdrstask;

	if(prefnums<0 || bakfnums<0)return;
	if(prefnums==0 && bakfnums==0)return;
	if((prefnums+bakfnums)>=pdrgrp_maninfo[zygrpno].maxframenums)return;

	memset(&tmppdrstask,0,sizeof(tmppdrstask));

	strcpy(tmppdrstask.triggername,triggername);
	strcpy(tmppdrstask.cause,cause);
	tmppdrstask.zygrpno=zygrpno;
	tmppdrstask.trigtime=trigtime;
	tmppdrstask.causetype=causetype;
	tmppdrstask.preframenums=prefnums;
	tmppdrstask.bakframenums=bakfnums;
	tmppdrstask.trigframeno=pdrgrp_maninfo[zygrpno].objpdr->GetCurFrameNo();
	tmppdrstask.ymd=entclock->year * 10000 + entclock->month * 100 + entclock->day;
	tmppdrstask.hmsms=entclock->hour * 10000000 + entclock->minute * 100000 
						+ entclock->second * 1000 + entclock->msecond;

	pdrgrp_maninfo[zygrpno].trigflag=0;
	tmpint=pdrgrp_maninfo[zygrpno].maxframenums;
	tmppdrstask.lastframeno=(tmpint+tmppdrstask.trigframeno+bakfnums)%tmpint;
	savetask.AddNode(&tmppdrstask);
}


static int PDRDATA_SAVEMODE=GetProgramRunPara("pdrsavemode",0);//0.首帧数据全存，其余各帧只存相对于前一帧变化的数据;1.所有帧数据全存

void PdrSaveTaskMan::ProcAllSaveTask(void)
{
	COMN_POSITION  curposition,nextposition;
	PDR_SAVETASK  *staskp;

	savetask.LockLink();
	curposition=savetask.GetFirstNode();
	while(curposition) {
		staskp=savetask.NodeData(curposition);
		nextposition=savetask.GetNextNode(curposition);
		if(staskp->lastframeno==pdrgrp_maninfo[staskp->zygrpno].objpdr->GetCurFrameNo()) {
				LockObjPdr(staskp->zygrpno);
				pdrgrp_maninfo[staskp->zygrpno].objpdr->SavePdrData(staskp,PDRDATA_SAVEMODE);
				UnlockObjPdr(staskp->zygrpno);
				savetask.DelNode(curposition,0);
		}
		curposition=nextposition;
	}
	savetask.UnlockLink();
}

/////////////////////////////////////////////////////////////////////////
ComnLinkTable<PDR_DBTASK>		PdrDBTaskMan::pdrdbtask;

PdrDBTaskMan::PdrDBTaskMan(void)
{
}

PdrDBTaskMan::~PdrDBTaskMan(void)
{
}

void PdrDBTaskMan::AddPdrDBTask(SPdrDescribe *pdrdesp,int datanums,char *sfname)
{
	PDR_DBTASK  tmpdbtask;

	tmpdbtask.pdrdes=*pdrdesp;
	tmpdbtask.datanums=datanums;
	strcpy(tmpdbtask.savefname,sfname);

	pdrdbtask.AddNode(&tmpdbtask);
}

#define PDR_PERSAVEDATA_NUMS   512

void PdrDBTaskMan::SaveDataInDB(PDR_DBTASK *dbtaskp)
{
	NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
	int grpnum=netstat.GetHostGrpNum();
	if(IsMainServer(host_group_no[0]))
	{
		int pendingcount,ret;
		SPdrSaveData spsd[PDR_PERSAVEDATA_NUMS];

		if(dbtaskp->pdrdes.causetype>=2) {
			Rdb_HisLib  hislibcl;
			char  tmpwhere[1000];

			sprintf(tmpwhere,"开关名=\'%s\' and 追忆组=%d and 时标=%d",
				dbtaskp->pdrdes.breakername,dbtaskp->pdrdes.zygrpno,
				dbtaskp->pdrdes.timeflag);
			hislibcl.DelRcds("hisdb","pdrdescribe",tmpwhere);

			sprintf(tmpwhere,"时标=%d and 追忆组=%d",dbtaskp->pdrdes.timeflag,
				dbtaskp->pdrdes.zygrpno);
			hislibcl.DelRcds("hisdb","pdrdata",tmpwhere);
		}

		FILE *fp=fopen(dbtaskp->savefname,"rb");
		if(fp==NULL)return;


		while(!feof(fp))
		{
			pendingcount = fread(spsd,sizeof(SPdrSaveData),PDR_PERSAVEDATA_NUMS, fp);
			if(pendingcount<=0)break;

/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
			for(int i=0; i<PDR_PERSAVEDATA_NUMS; i++){
				scdnetbyte(RDB_DATATYPE_SHORT,&spsd[i].zygrpno);
				scdnetbyte(RDB_DATATYPE_SHORT,&spsd[i].frameno);
				scdnetbyte(RDB_DATATYPE_INT,&spsd[i].timeflag);
				scdnetbyte(RDB_DATATYPE_FLOAT,&spsd[i].value);
				scdnetbyte(RDB_DATATYPE_SHORT,&spsd[i].baseobjtype);
			}
#endif*/

			if((ret=BulkCopyIn(PDRDATA_TABNAME, spsd,pendingcount))!=OK) {
				sleepms(2,0);
				if((ret=BulkCopyIn(PDRDATA_TABNAME, spsd,pendingcount))!=OK) {
					RdbDebugMesg("存 时标=%d,追忆组=%d的追忆数据失败,ret=%d\n",
						dbtaskp->pdrdes.timeflag,dbtaskp->pdrdes.zygrpno,ret);
				}
			}
			sleepms(1,0);
		}//while

		fclose(fp);
/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
		scdnetbyte(RDB_DATATYPE_INT,&dbtaskp->pdrdes.timeflag);
		scdnetbyte(RDB_DATATYPE_INT,&dbtaskp->pdrdes.ymd);
		scdnetbyte(RDB_DATATYPE_INT,&dbtaskp->pdrdes.hmsms);
		scdnetbyte(RDB_DATATYPE_SHORT,&dbtaskp->pdrdes.zygrpno);
#endif*/		
		if((ret=BulkCopyIn(PDRDESC_TABNAME,&dbtaskp->pdrdes,1))!=OK) {
			sleepms(2,0);
			if((ret=BulkCopyIn(PDRDESC_TABNAME,&dbtaskp->pdrdes,1))!=OK) {
				RdbDebugMesg("存 时标=%d,追忆组=%d的追忆描述失败,ret=%d\n",
					dbtaskp->pdrdes.timeflag,dbtaskp->pdrdes.zygrpno,ret);
			}
		}
    }

	remove(dbtaskp->savefname);
}

void PdrDBTaskMan::ProcAllDBTask(void)
{
	COMN_POSITION  curposition,nextposition;
	int  procokflag=0;
	PDR_DBTASK  *dbtaskp;
	PDR_DBTASK  tmpdbtask;

	for(;;) {
		procokflag=0;
		pdrdbtask.LockLink();
		curposition=pdrdbtask.GetFirstNode();
		while(curposition) {
			dbtaskp=pdrdbtask.NodeData(curposition);
			nextposition=pdrdbtask.GetNextNode(curposition);
			tmpdbtask=*dbtaskp;
			procokflag=1;
			pdrdbtask.DelNode(curposition,0);
			curposition=nextposition;
			break;
		}
		pdrdbtask.UnlockLink();
		if(procokflag==0)break;
		SaveDataInDB(&tmpdbtask);
	}
}


///////////////////////////////////////////

