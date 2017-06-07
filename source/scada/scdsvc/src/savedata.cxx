
#include <stdio.h>
#include <stdlib.h>
#include "scadainterface.h"
#include "scadahislib.h"
#include "scadaproc.h"
#include "procthread.h"
#include "rdb/rdbapi.h"
#include "db/dbapi.h"
#include "scd/cmddef.h"

#ifdef WIN32
#include <io.h>
#endif

#ifdef __linux
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define MAXRCDNUM		65534

extern DBApi dbapi;

static int KG_SAVE_INTERVAL=GetProgramRunPara("kgsaveinterval",(int)0);
static int DZ_SAVE_INTERVAL=GetProgramRunPara("dzsaveinterval",(int)0);
static int HISFILE_SAVEDAYS = GetProgramRunPara("hisfilesavedays",(int)0);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// 计划数据处理类 //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
static int DEAL_PJDATA=GetProgramRunPara("dealpjdata",0);//2004 07 add
int scdproject_getcurdata(SYS_CLOCK *clockp,int initf=0);

///////////////   处理计划数据   ///////////////
//取指定时间点的计划数据, 并将计划数据放在ANALOG_STRU结构中
int scdproject_getcurdata(SYS_CLOCK *clockp,int initf)
{
    int     ret,tmpmin;
    ANALOG_STRU  *analogp;
    float   floatdata;
    Rdb_Tab  memtab(MyUserName,MyPassWord);
    Hisproject  hispj;

    if (DEAL_PJDATA==0) return 0;//2004 07 add

    ret=memtab.RdbOpenTab(ANALOG_TABNAME);

    if (ret==OK)
    {
        tmpmin=clockp->hour*60 + clockp->minute;

        for (analogp=(ANALOG_STRU *)memtab.rdbgetfirstrcdaddr();analogp!=NULL;analogp=(ANALOG_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (analogp->quotatime>0)
            {
                if ((tmpmin%analogp->quotatime)==0 || initf==1)
                {
                    ret=hispj.read_pjdata_minute(clockp,analogp->baseanalog.name,&floatdata);

                    if (ret==OK)
                    {
                        analogp->quotadata=floatdata;
                        ret=hispj.read_pjdata_minute(clockp,analogp->baseanalog.name,&floatdata,HISDATA_UPPERTDDATA);

                        if (ret==OK) analogp->quotaup=floatdata;
                        else analogp->quotaup=0.0;

                        ret=hispj.read_pjdata_minute(clockp,analogp->baseanalog.name,&floatdata,HISDATA_LOWERTDDATA);

                        if (ret==OK) analogp->quotadown=floatdata;
                        else analogp->quotadown=0.0;
                    }
                    else
                    {
                        analogp->quotadata=0.0;
                        analogp->quotaup=0.0;
                        analogp->quotadown=0.0;
                    }
                }
            }
        }

        memtab.RdbCloseTab();
    }

    return(OK);
}

//拷贝clockp指定的日期的计划数据到clockp+daynums日之内的任一天
int scdproject_copy(SYS_CLOCK *clockp,int daynums)
{
    int PROJECT_NOT_COPY=GetProgramRunPara("jhnotcopy",1);

    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();
    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    if (!IsMainServer(savegroupno))return(OK);	// 如果不是主服务器

    if (PROJECT_NOT_COPY)return(OK);

    int     i,ret,srcpjdatanums,tmppjdatanums,updatanums,downdatanums;

    ANALOG_STRU  *analogp;

    SCD_PJDATA *srcpjdata,*tmppjdata,*uppjdata,*downpjdata;

    SYS_CLOCK  tmpclock;

    Rdb_Tab  memtab(MyUserName,MyPassWord);

    Hisproject  hispj,tmppj,uppjcl,downpjcl;

    ret=memtab.RdbOpenTab(ANALOG_TABNAME);

    if (ret==OK)
    {
        for (analogp=(ANALOG_STRU *)memtab.rdbgetfirstrcdaddr();analogp!=NULL;analogp=(ANALOG_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (analogp->quotatime>0)
            {
                srcpjdatanums=hispj.read_pjdata_day(clockp,analogp->baseanalog.name,&srcpjdata);
                updatanums=uppjcl.read_pjdata_day(clockp,analogp->baseanalog.name,&uppjdata,HISDATA_UPPERTDDATA);
                downdatanums=downpjcl.read_pjdata_day(clockp,analogp->baseanalog.name,&downpjdata,HISDATA_LOWERTDDATA);

                if (srcpjdatanums>0)
                {
                    tmpclock=*clockp;

                    for (i=0;i<daynums;i++)
                    {
                        IncDecDays(&tmpclock,1);
                        tmppjdatanums=tmppj.read_pjdata_day(&tmpclock,analogp->baseanalog.name,&tmppjdata);

                        if (tmppjdatanums<=0)
                        {
                            hispj.dump_pjdata(&tmpclock,analogp->baseanalog.name,srcpjdata,srcpjdatanums);

                            if (updatanums>0)uppjcl.dump_pjdata(&tmpclock,analogp->baseanalog.name,uppjdata,updatanums,HISDATA_UPPERTDDATA);

                            if (downdatanums>0)downpjcl.dump_pjdata(&tmpclock,analogp->baseanalog.name,downpjdata,downdatanums,HISDATA_LOWERTDDATA);
                        }
                    } //for
                } // if(srcpjdatanums>0)
            }  // if quotatime>0
        } // for

        memtab.RdbCloseTab();
    }

    return(OK);
}


//恢复计划数据
int scdproject_restore(void)
{
    intertime   lasttime=0,curtime;
    SYS_CLOCK   lastclock,curclock,tmpclock;
    int daynums;

    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();
    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    if (!IsMainServer(savegroupno))return(OK);	// 如果不是主服务器

    lasttime=GetSysLastTime();

    GetInterTime(&curtime);

    if (lasttime>0 && (curtime-lasttime)>120)
    {
        ConvertTime(lasttime,&lastclock);
        ConvertTime(curtime,&curclock);
        lastclock.hour=0;
        lastclock.minute=0;
        lastclock.second=0;
        curclock.hour=0;
        curclock.minute=0;
        curclock.second=0;
        daynums=(int)(CalDecSecs(&lastclock,&curclock)/(24*3600));

        if (daynums>0)
        {
            tmpclock=lastclock;
            IncDecDays(&tmpclock,1);
            scdproject_copy(&tmpclock,daynums);
        }
    }

    return(OK);
}

////////////////////////////////////////////////////////////////////////////
////////////////// 读取历史库容量 //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

char  *hisdbserver_name[]={"dbserver1","dbserver2"};
char  *hisdb_libdes[]={"历史数据库","追忆数据库"};


void  SendHisDBUsageEvent(char *dbsrvname,char *dbsrvdes,int libno,int total,int used)
{
    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();

    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    if (!IsMainServer(savegroupno))return;

    if (libno<0 || libno>=2)return;

    int  HISDBUSAGE_EVTMODE=GetProgramRunPara("hdbevtmode",2);

    EventProc		proc;

    SYS_CLOCK       curclock;

    SYS_EVENT	tmpevent;


    memset((char *)&tmpevent,0,sizeof(tmpevent));

    tmpevent.sort=ENTCLASS_DEVICE;	//事件分类号

    tmpevent.type=ENTTYPE_SYS_DISK;	// 事项类型

    GetCurrentClock(&curclock);

    tmpevent.ymd=curclock.year*10000+curclock.month*100+curclock.day;		//2、年月日

    tmpevent.hmsms=curclock.hour*10000000+curclock.minute*100000+
                   curclock.second*1000+curclock.msecond;	//3、时分秒毫秒

    tmpevent.DispOut=1;

    if (HISDBUSAGE_EVTMODE==0)
        tmpevent.DiskOut=0;
    else if (HISDBUSAGE_EVTMODE==1)
        tmpevent.DiskOut=1;
    else if (HISDBUSAGE_EVTMODE==2)
    {
        tmpevent.ToneOut=1;
    }
    else if (HISDBUSAGE_EVTMODE==3)
    {
        tmpevent.DiskOut=1;
        tmpevent.ToneOut=1;
    }

    strcpy(tmpevent.Object,dbsrvname);

    sprintf(tmpevent.text,"%s %s剩余容量不足(总容量:%dM,已使用:%dM)",dbsrvdes,
            hisdb_libdes[libno],total,used);

    proc.Save(&tmpevent,1);
}

void read_hisdb_diskusage(void)
{
    /*	int  READ_HDB_USAGE=GetProgramRunPara("rdhdbusage",0);

    	if(READ_HDB_USAGE)return;

    	Rdb_QuickQuery   rdbsp(MyUserName, MyPassWord);
    	int i;
    	float  tmplimit;
    	char   dbsrvdes[200];

    	FUNC_STR   tmpfunc;

    	for(i=0;i<2;i++) {
    		if(rdbsp.RdbGetRecordNo("hisdbusage",hisdbserver_name[i])<0)continue;

    //		if(i==0)tmpfunc.serverno=SERVER_1;
    //		else tmpfunc.serverno=SERVER_2;
    		int ret=dbapi.dbfcommand(DB_GET_SPACE,(void *)&tmpfunc);
    		if(ret==CS_SUCCEED) {
    			rdbsp.RdbSetColValInt("hisdbusage",hisdbserver_name[i],"dbtotal",tmpfunc.col[0].val.intvalue);
    			rdbsp.RdbSetColValInt("hisdbusage",hisdbserver_name[i],"dbused",tmpfunc.col[1].val.intvalue);
    			if(tmpfunc.col[0].val.intvalue>0)rdbsp.RdbSetColValFloat("hisdbusage",hisdbserver_name[i],"dbcent",(float)tmpfunc.col[1].val.intvalue/(float)tmpfunc.col[0].val.intvalue);
    			rdbsp.RdbSetColValInt("hisdbusage",hisdbserver_name[i],"zytotal",tmpfunc.col[2].val.intvalue);
    			rdbsp.RdbSetColValInt("hisdbusage",hisdbserver_name[i],"zyused",tmpfunc.col[3].val.intvalue);
    			if(tmpfunc.col[2].val.intvalue>0)rdbsp.RdbSetColValFloat("hisdbusage",hisdbserver_name[i],"zycent",(float)tmpfunc.col[3].val.intvalue/(float)tmpfunc.col[2].val.intvalue);

    			dbsrvdes[0]='\0';
    			rdbsp.RdbGetExpColVal("hisdbusage",hisdbserver_name[i],"describe",RDB_DATATYPE_STRING,dbsrvdes,OBJDESCRIBE_LEN);
    			tmplimit=rdbsp.RdbGetColValFloat("hisdbusage",hisdbserver_name[i],"dblimit");
    			if(!ISZERO(tmplimit)) {
    				if(tmpfunc.col[0].val.intvalue>0) {
    					if(((float)tmpfunc.col[1].val.intvalue/(float)tmpfunc.col[0].val.intvalue)>tmplimit) {
    						SendHisDBUsageEvent(hisdbserver_name[i],dbsrvdes,0,tmpfunc.col[0].val.intvalue,
    												tmpfunc.col[1].val.intvalue);
    					}
    				}
    			}
    			tmplimit=rdbsp.RdbGetColValFloat("hisdbusage",hisdbserver_name[i],"zylimit");
    			if(!ISZERO(tmplimit)) {
    				if(tmpfunc.col[2].val.intvalue>0) {
    					if(((float)tmpfunc.col[3].val.intvalue/(float)tmpfunc.col[2].val.intvalue)>tmplimit) {
    						SendHisDBUsageEvent(hisdbserver_name[i],dbsrvdes,1,tmpfunc.col[2].val.intvalue,
    												tmpfunc.col[3].val.intvalue);
    					}
    				}
    			}
    		}
    	}*/
}

////////////////////////////////////////////////////////////////////////////
////////////////// 历史库定时器 ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

class  HDBTimer
{
    int lasttimeval;
    int secondnums;

public:
    HDBTimer(int secs)
    {
        lasttimeval=-1;
        secondnums=secs;
    }

    int TimerOK(void)
    {
        int curtimeval,okflag=0;
        SYS_CLOCK  curclock;

        GetCurrentClock(&curclock);
        curtimeval=(curclock.hour * 3600 + curclock.minute * 60 + curclock.second)/secondnums;

        if (curtimeval!=lasttimeval && lasttimeval>=0)
        {
            okflag=1;
        }

        lasttimeval=curtimeval;

        return(okflag);
    }

    int GetTimeVal(void)
    {
        return(lasttimeval);
    }
};


////////////////////////////////////////////////////////////////////////////
////////////////// 历史库存盘任务管理 //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

typedef struct
{
    intertime	proc_time;
    char		savedelayf;
    int			mainbackflag;
    int			savetype;
    uchar		pulseflag;
    char		tabname[150];
    int			funccode;
    int			date;
    int			time;
    int			rownums;
    char		savefilename[150];
}  HDB_SAVETASK;


int SaveHDBData(HDB_SAVETASK  *hdbsavetaskp);

class HDBSaveTaskMan
{
    static ComnLinkTable<HDB_SAVETASK>		savetask;

public:
    void AddHDBSaveTask(int	savetype,char *tabname,int pulseflag,int	funccode,int date,int time,
                        int	rownums,char *savefilename);
    void AddHDBSaveTask(HDB_SAVETASK *hdbstp);
    void ProcAllHDBSaveTask(void);
};


ComnLinkTable<HDB_SAVETASK>		HDBSaveTaskMan::savetask;

void HDBSaveTaskMan::AddHDBSaveTask(int	savetype,char *tabname,int pulseflag,int	funccode,int date,
                                    int time,int  rownums,char *savefilename)
{
    HDB_SAVETASK  tmphdbstask;

    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();
    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    memset(&tmphdbstask,0,sizeof(tmphdbstask));

    strcpy(tmphdbstask.tabname,tabname);
    strcpy(tmphdbstask.savefilename,savefilename);
    GetInterTime(&tmphdbstask.proc_time,NULL);
    tmphdbstask.mainbackflag=IsMainServer(savegroupno);
    tmphdbstask.savetype=savetype;
    tmphdbstask.pulseflag=pulseflag;
    tmphdbstask.funccode=funccode;
    tmphdbstask.date=date;
    tmphdbstask.time=time;
    tmphdbstask.rownums=rownums;
    tmphdbstask.savedelayf=0;
    savetask.AddNode(&tmphdbstask);
}

void HDBSaveTaskMan::AddHDBSaveTask(HDB_SAVETASK *hdbstp)
{
    savetask.AddNode(hdbstp);
}

int  HISDBDATA_BACK_DELAY=GetProgramRunPara("hdbbkdelay",(24*3600));

void HDBSaveTaskMan::ProcAllHDBSaveTask(void)
{
    static int  hidbdata_okflag=0;  //表示历史库的好坏状态 1--好，0--坏
    int  backdelaytime=MIN(HISDBDATA_BACK_DELAY,(24*3600)); // 存盘不成功后保留的时间

    COMN_POSITION  curposition,nextposition;
    HDB_SAVETASK  *staskp,tmpstask;
    intertime	curtime;
    int			ret,delfileflag,saveflag,delnodeflag,curmbflag;

    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();
    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    for (;;)
    {
        saveflag=0;
        delfileflag=0;
        savetask.LockLink();
        curposition=savetask.GetFirstNode();

        while (curposition)
        {
            staskp=savetask.NodeData(curposition);
            nextposition=savetask.GetNextNode(curposition);
            delnodeflag=1;
            GetInterTime(&curtime,NULL);

            if (staskp->savedelayf)  //延迟存盘
            {
                if (ABS(curtime-staskp->proc_time)>backdelaytime)  //超过时间
                {
                    tmpstask=*staskp;
                    saveflag=2; // 最后存一次
                }
                else if (hidbdata_okflag)  //历史库好，尝试存盘
                {
                    tmpstask=*staskp;
                    saveflag=1; // 尝试存盘
                }
                else delnodeflag=0;
            }
            else   // 处理正常存盘任务
            {
                switch (staskp->mainbackflag)
                {

                case 0: // 备机存盘任务
                    curmbflag=IsMainServer(savegroupno);

                    if (curmbflag)  //本机当前为主机
                    {
                        if (ABS(curtime-staskp->proc_time)<15)
                        {
                            delnodeflag=0;
                        }
                        else    // 延迟15秒保存
                        {
                            tmpstask=*staskp;
                            saveflag=1;
                        }
                    }
                    else   //本机当前仍为备机
                    {
                        if (ABS(curtime-staskp->proc_time)<55)
                        {
                            delnodeflag=0;
                        }
                        else    // 延迟55秒删除
                        {
                            tmpstask=*staskp;
                            delfileflag=1;
                        }
                    }

                    break;

                default: // 主机存盘任务
                    tmpstask=*staskp;
                    saveflag=1;
                    break;
                }
            }

            if (delnodeflag)savetask.DelNode(curposition,0);

            curposition=nextposition;

            if (saveflag || delfileflag)break;
        }

        savetask.UnlockLink();

        if (saveflag)  //存盘
        {
            ret=SaveHDBData(&tmpstask);
            //置历史库状态标志

            if (ret==OK)hidbdata_okflag=1;
            else hidbdata_okflag=0;

            if (saveflag==2)  //最后存盘，删除文件
            {
                remove(tmpstask.savefilename);
            }
            else if (ret==SCADA_ERR_ADDRCD || ret==SCADA_ERR_OPENTAB)
            {
                //历史库原因存盘不成功,重新加入延迟存盘
                tmpstask.savedelayf=1;
                AddHDBSaveTask(&tmpstask);
            } // 存盘成功或别的原因导致存盘不成功
            else remove(tmpstask.savefilename);
        }
        else if (delfileflag)
        {
            remove(tmpstask.savefilename);
        }
        else break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// 分钟历史数据存储类 //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

class Hisdata
{
    Rdb_PreMalloc   *datamalloc;
    Rdb_PreMalloc   *daydatamalloc;
    Rdb_PreMalloc   *daystatmalloc;

public:
    Hisdata(void);
    ~Hisdata(void);
    void datainit(void);
    int storedata(int pulsef,char *objname,float value,int rawval,int flag);
    int dumpdata(int pulsef,SYS_CCLOCK *datatime,int freedataf);

    void daydatainit(void);
    int storedaydata(int pulsef,char *objname,double value,int rawval,int flag,int odate=0,int otime=0,int osec=0);
    int dumpdaydata(int pulsef,SYS_CCLOCK *datatime);

    int storedaystat(SYS_CLOCK  *clockp);
    int dumpdaystat(void);
};

//pulsef为存盘标志:0为遥测；1为电度；2为遥信

Hisdata::Hisdata(void)
{
    datamalloc=new Rdb_PreMalloc(1000,sizeof(HIS_DATA));
    daydatamalloc=new Rdb_PreMalloc(1000,sizeof(HIS_DATA));
    daystatmalloc = NULL;
}

Hisdata::~Hisdata(void)
{
    if (datamalloc!=NULL)		delete datamalloc;

    if (daydatamalloc!=NULL)	delete daydatamalloc;

    if (daystatmalloc!=NULL)    delete daystatmalloc;
}

void Hisdata::datainit(void)
{
    if (datamalloc) datamalloc->freespace();
}

void Hisdata::daydatainit(void)
{
    if (daydatamalloc) daydatamalloc->freespace();
}


// 将定时存盘数据转成存盘格式存入内存（脉冲值标志，对象名，值，质量标志）
int Hisdata::storedata(int pulsef,char *objname,float value,int rawval,int flag)
{
    HIS_DATA  *sdiskdata;

    sdiskdata=(HIS_DATA  *)datamalloc->get_nextunitaddr();
    strcpy(sdiskdata->name,objname);

    if (pulsef==0)
    {
        sdiskdata->val.ycdata=(float)value;
        sdiskdata->rawval.ycdata=(int)rawval;
    }
    else if (pulsef==1)
    {
        sdiskdata->val.kwhdata=(float)value;
        sdiskdata->rawval.kwhdata=rawval;
    }
    else if (pulsef==2 || pulsef==3)
    {
        sdiskdata->val.yxdata=(uchar)value;
        sdiskdata->rawval.yxdata=(uchar)rawval;
    }

    sdiskdata->flag=flag;

    sdiskdata->ms=0;
    return(OK);
}


// 转存数据，即将数据存入历史库（脉冲值标志，时间，数据释放标志）
int Hisdata::dumpdata(int pulsef,SYS_CCLOCK *datatime,int freedataf)
{
    HIS_DATA        *sdiskdata;
    int             ret,tmppulseflag,rownums;
    SYS_CLOCK		sysclock;
    static  int     datafileno=0;

    sdiskdata=(HIS_DATA *)datamalloc->get_mallocinfo(&rownums);
 
	RdbDebugMesg("\n get_mallocinfo() rownums=%d , sdiskdata=%s \n",rownums,sdiskdata);

    if (sdiskdata==NULL || rownums<=0)	return(OK);

    sdiskdata->date=datatime->date;

    sdiskdata->time=datatime->time;

    char tmpfilename[64],savefilename[200];

    if (pulsef == 0) sprintf(tmpfilename,"YCDATASAVE%d",datafileno);
    else if (pulsef == 1) sprintf(tmpfilename,"KWHDATASAVE%d",datafileno);
    else if (pulsef == 2) sprintf(tmpfilename,"BREAKDATASAVE%d",datafileno);
    else if (pulsef == 3) sprintf(tmpfilename,"SWITCHDATASAVE%d",datafileno);

    char *sacenv=getenv(NBENV);

    if (sacenv==NULL)
    {
        strcpy(savefilename,tmpfilename);
    }
    else
    {
        strcpy(savefilename,sacenv);
#ifdef WIN32
        strcat(savefilename,"\\procscript\\");
#else
        strcat(savefilename,"/procscript/");
#endif
        strcat(savefilename,tmpfilename);
    }

    datafileno++;

    datafileno = datafileno%3000;

    FILE    *savefp=fopen(savefilename,"wb");

    
    if (savefp==NULL)
    {
	    if (freedataf) datamalloc->freespace();
        return -1;
    }
    //char buf[10000];
     //setvbuf(savefp,buf,0,sizeof(buf));
    ret=fwrite(sdiskdata,sizeof(HIS_DATA),rownums,savefp);

    fclose(savefp);

    if (freedataf) datamalloc->freespace();

    if (ret!=rownums)
    {
        remove(savefilename);
        return(-1);
    }
	
	//文件保存成功，备份
	char folder_name[200];
	char dir_name[200];
	memset(folder_name,0,sizeof(char)*200);
	memset(dir_name,0,sizeof(char)*200);

	if( HISFILE_SAVEDAYS > 0)
	{

		SYS_CLOCK	     fileclock;
		GetCurrentClock(&fileclock);

		char savefilename_bk[200];
		char tmpfilename_bk[64];
		memset(savefilename_bk,0,sizeof(char)*200);
		memset(tmpfilename_bk,0,sizeof(char)*64);
		
		if (pulsef == 0) strcpy(tmpfilename_bk,"YCDATASAVE");
		else if (pulsef == 1) strcpy(tmpfilename_bk,"KWHDATASAVE");
		else if (pulsef == 2) strcpy(tmpfilename_bk,"BREAKDATASAVE");
		else if (pulsef == 3) strcpy(tmpfilename_bk,"SWITCHDATASAVE");

		char *file_env=getenv(NBENV);
		if (file_env==NULL)
		{
			strcpy(savefilename_bk,tmpfilename_bk);
		}
		else
		{
			strcpy(savefilename_bk,sacenv);
#ifdef WIN32
			strcat(savefilename_bk,"\\procscript\\hisbak\\");
#else
			strcpy(dir_name,savefilename_bk);
			strcat(dir_name,"/procscript/hisbak");
			strcat(savefilename_bk,"/procscript/hisbak/");
			
#endif
			strcpy(folder_name,savefilename_bk);

			strcat(savefilename_bk,tmpfilename_bk);
		}


		char timeID[128];
		memset(timeID,0,sizeof(char)*128);
		sprintf(timeID,"-%4d-%02d-%02d-%02d-%02d",fileclock.year,fileclock.month,fileclock.day,fileclock.hour,fileclock.minute);

		strcat(savefilename_bk,timeID);		
		FILE    *file_bk_fp=fopen(savefilename_bk,"wb");
		if ( file_bk_fp )
		{
			ret=fwrite(sdiskdata,sizeof(HIS_DATA),rownums,file_bk_fp);
			fclose(file_bk_fp);
		}
	}

	////////////////////

	static int del_count = 0;
	if( HISFILE_SAVEDAYS > 0)
	{
		del_count++;
		//10个存盘周期，检查一下过期文件
		if( del_count%10 == 0)
		{
			long int sec_limit = HISFILE_SAVEDAYS * 3600 * 24;
			char checkfile_name[200];
			strcpy(checkfile_name,folder_name);
			strcat(checkfile_name,"*SAVE*");					//欲查找的文件，支持通配符	                                  

 #ifdef WIN32
			long handle;              //用于查找的句柄
			struct _finddata_t fileinfo;       
			//文件信息的结构体
			handle=_findfirst(checkfile_name,&fileinfo);       //第一次查找
			char delfile_name[200];

			time_t file_cur;
			time(&file_cur); 
			long int file_save_sec = 0;
			if(-1 != handle)
			{
				file_save_sec = file_cur - fileinfo.time_create;
				if( sec_limit < file_save_sec )
				//if( 300 < file_save_sec )   //5分钟
				{
					strcpy(delfile_name,folder_name);
					strcat(delfile_name,fileinfo.name);
					remove(delfile_name);
				}

				while(!_findnext(handle,&fileinfo))         //循环查找其他符合的文件，直到找不到其他的为止
				{
					file_save_sec = file_cur - fileinfo.time_create;
					if( sec_limit < file_save_sec )
					//if( 300 < file_save_sec )
					{
						strcpy(delfile_name,folder_name);
						strcat(delfile_name,fileinfo.name);
						remove(delfile_name);
					}
				}
				_findclose(handle);                                    
			}
#endif

#ifdef __linux
			DIR *dir;//声明一个句柄
			if ( dir = opendir(dir_name) )  
			{  
			    struct dirent* fileinfo;                    //文件信息的结构体
			    char delfile_name[200];

			    time_t file_cur;
			    time(&file_cur); 
			    long int file_save_sec = 0;

			    while ( (fileinfo = readdir(dir)) != NULL )  
			    {  
				    struct stat file_stat;
                    if (strcmp(fileinfo->d_name, ".") == 0 ||
                                strcmp(fileinfo->d_name, "..") == 0)
                   {
                        continue;  
					}
					strcpy(delfile_name,folder_name);
					strcat(delfile_name,fileinfo->d_name);
				    if ( lstat(delfile_name,&file_stat) >= 0)  
				    {  
						file_save_sec = file_cur - file_stat.st_ctime;
						if( sec_limit < file_save_sec )
						{
							remove(delfile_name);
						}
				    }  
			   }  	
			   closedir(dir); 
	       }               
#endif
		}	
	}

	/////////////////////


    HDBSaveTaskMan   hdbstask;

    char tabname[128];


    SYS_CCLOCK tmpdatatime;
    tmpdatatime=*datatime;
    tmpdatatime.time=0;
    ConvertClock(&tmpdatatime,&sysclock);


    // 脉冲值和非脉冲值打开不同的表，并置标志
    if (pulsef==0)
    {
#ifdef TABLEPERDAY
		sprintf(tabname,"yc%.4d%.2d%.2d",sysclock.year,sysclock.month,sysclock.day);
	#pragma message("Compile TABLEPERDAY scdsvc...")
#else
		sprintf(tabname,"yc%.4d%.2d",sysclock.year,sysclock.month);
#endif
        tmppulseflag=NOTPULSE;
    }
    else if (pulsef==1)
    {
        sprintf(tabname,"kwh%.4d%.2d",sysclock.year,sysclock.month);
        tmppulseflag=ISPULSE;
    }
    else if (pulsef==2)
    {
        sprintf(tabname,"yx%.4d%.2d",sysclock.year,sysclock.month);
        tmppulseflag=2;
    }
    else if (pulsef==3)
    {
        sprintf(tabname,"yx%.4d%.2d",sysclock.year,sysclock.month);
        tmppulseflag=3;
    }

    hdbstask.AddHDBSaveTask(1,tabname,tmppulseflag,INS_MIN_HISDATA,datatime->date,

                            datatime->time,rownums,savefilename);

    return(OK);
}



// 将日存盘数据存入内存（脉冲标志，对象名，值，）
int Hisdata::storedaydata(int pulsef,char *objname,double value,int rawval,int flag,int odate,
                          int otime,int osec)
{
    HIS_DATA  *sdiskdata;

    sdiskdata=(HIS_DATA  *)daydatamalloc->get_nextunitaddr();

    strcpy(sdiskdata->name,objname);

    if (pulsef)
    {
        sdiskdata->val.kwhdata=(float)value;
        sdiskdata->rawval.kwhdata=rawval;
    }
    else
    {
        sdiskdata->val.ycdata=(float)value;
        sdiskdata->rawval.ycdata=(int)rawval;
    }

    sdiskdata->flag=flag;

    sdiskdata->odate=odate;
    sdiskdata->otime=otime;
    sdiskdata->oflag=osec;
    return(OK);
}

// 转存日存盘数据（脉冲值标志，时间）
int Hisdata::dumpdaydata(int pulsef,SYS_CCLOCK *datatime)
{
    HIS_DATA        *sdiskdata;
    int             ret,tmppulseflag,rownums;
    SYS_CLOCK		sysclock;
    static  int     daydatafileno=0;

    sdiskdata=(HIS_DATA *)daydatamalloc->get_mallocinfo(&rownums);

    if (sdiskdata==NULL || rownums==0)	return(OK);

    sdiskdata->date=datatime->date;

    sdiskdata->time=datatime->time;

    char tmpfilename[64],savefilename[200];

    if (pulsef) sprintf(tmpfilename,"KWHDAYDATASAVE%d",daydatafileno);
    else sprintf(tmpfilename,"YCDAYDATASAVE%d",daydatafileno);

//	sprintf(tmpfilename,"DAYDATASAVE%d",daydatafileno);
    char *sacenv=getenv(NBENV);

    if (sacenv==NULL)
    {
        strcpy(savefilename,tmpfilename);
    }
    else
    {
        strcpy(savefilename,sacenv);
#ifdef WIN32
        strcat(savefilename,"\\procscript\\");
#else
        strcat(savefilename,"/procscript/");
#endif
        strcat(savefilename,tmpfilename);
    }

    daydatafileno++;

    daydatafileno = daydatafileno%100;

    FILE    *savefp=fopen(savefilename,"wb");

    if (savefp==NULL)
    {
        daydatamalloc->freespace();
        return -1;
    }

    ret=fwrite(sdiskdata,sizeof(HIS_DATA),rownums,savefp);

    fclose(savefp);

    daydatamalloc->freespace();

    if (ret!=rownums)
    {
        remove(savefilename);
        return(-1);
    }

    HDBSaveTaskMan   hdbstask;

    char tabname[128];


    SYS_CCLOCK tmpdatatime;
    tmpdatatime=*datatime;
    tmpdatatime.time=0;
    ConvertClock(&tmpdatatime,&sysclock);

    if (pulsef)
    {
        sprintf(tabname,"daykwh%.4d%.2d",sysclock.year,sysclock.month);
        tmppulseflag=ISPULSE;
    }
    else
    {
        sprintf(tabname,"dayyc%.4d%.2d",sysclock.year,sysclock.month);
        tmppulseflag=NOTPULSE;
    }

    hdbstask.AddHDBSaveTask(2,tabname,tmppulseflag,INS_MIN_HISDATA,datatime->date,

                            0,rownums,savefilename);

    return(OK);
}



// 将日统计存盘数据存入内存（脉冲标志，对象名，值，）
int Hisdata::storedaystat(SYS_CLOCK  *curclock)
{
    int odate;
    Rdb_Tab breaktab,terminaltab,chtab,anatab;

    breaktab.RdbLogin(MyUserName, MyPassWord);
    terminaltab.RdbLogin(MyUserName, MyPassWord);
    chtab.RdbLogin(MyUserName, MyPassWord);
    anatab.RdbLogin(MyUserName, MyPassWord);

    if (breaktab.RdbOpenTab(BREAK_TABNAME)!=RDB_OK)return -1;

    if (terminaltab.RdbOpenTab(TERMINAL_TABNAME)!=RDB_OK)return -1;

    if (chtab.RdbOpenTab(CHANNEL_TABNAME)!=RDB_OK)return -1;

    if (anatab.RdbOpenTab(ANALOG_TABNAME)!=RDB_OK)return -1;


    odate=curclock->year *10000 + curclock->month*100 + curclock->day;

    int breaknum = breaktab.RdbGetRcdCnt();

    int terminalnum =   terminaltab.RdbGetRcdCnt();

    int chnum  =   chtab.RdbGetRcdCnt();

    int storercdnum = breaknum + terminalnum + chnum;

    if (daystatmalloc==NULL)
    {
        daystatmalloc = new Rdb_PreMalloc(storercdnum, sizeof(DEVSTATSAVE));

        if (daystatmalloc==NULL)	return -1;
    }
    else daystatmalloc->freespace();

    DEVSTATSAVE *statsave;

    for (BREAK_STRU *breaks=(BREAK_STRU *)breaktab.rdbgetfirstrcdaddr();breaks!=NULL;
            breaks=(BREAK_STRU *)breaktab.rdbgetnextrcdaddr())
    {
        if (breaks->basestatus.name[0]=='\0')continue;

        statsave = (DEVSTATSAVE *)daystatmalloc->get_nextunitaddr();

        if (statsave == NULL)  break;

        memset(statsave,0,sizeof(DEVSTATSAVE));

        statsave->savedate = odate;

        strcpy(statsave->name, breaks->basestatus.name);

        statsave->devtype = HISDEV_BREAK;

        statsave->runtime = breaks->dayruntime;

        statsave->lastdevstatus=breaks->basestatus.value;
    }


    for (TERMINAL_STRU *rtus=(TERMINAL_STRU *)terminaltab.rdbgetfirstrcdaddr();rtus!=NULL;
            rtus=(TERMINAL_STRU *)terminaltab.rdbgetnextrcdaddr())
    {
        if (rtus->name[0]=='\0')continue;

        if (rtus->useflag)
        {
            statsave = (DEVSTATSAVE *)daystatmalloc->get_nextunitaddr();

            if (statsave == NULL)break;

            memset(statsave,0,sizeof(DEVSTATSAVE));

            statsave->savedate = odate;

            strcpy(statsave->name, rtus->name);

            statsave->devtype = HISDEV_TERMINAL; //TERMINAL运行情况

            statsave->runtime = rtus->dayruntime;

            statsave->lastdevstatus=rtus->status;
			statsave->statistic[0] = rtus->breaktimes;
        }
    }


    for (CHANNEL_STRU *chs=(CHANNEL_STRU *)chtab.rdbgetfirstrcdaddr();chs!=NULL;
            chs=(CHANNEL_STRU *)chtab.rdbgetnextrcdaddr())
    {
        if (chs->name[0]=='\0')continue;

        if (chs->useflag)
        {
            statsave = (DEVSTATSAVE *)daystatmalloc->get_nextunitaddr();

            if (statsave == NULL)break;

            memset(statsave,0,sizeof(DEVSTATSAVE));

            statsave->savedate = odate;

            strcpy(statsave->name, chs->name);

            statsave->devtype = HISDEV_CHAN; //通道运行情况

            statsave->runtime = chs->dayruntime;

            statsave->statistic[0] = chs->breaktimes; //故障次数
			statsave->statistic[1] = chs->sendcount;
			statsave->statistic[2] = chs->recvcount;

            statsave->lastdevstatus=chs->status;
        }
    }

    for (BASEANALOG_STRU *analogs=(BASEANALOG_STRU *)anatab.rdbgetfirstrcdaddr();analogs!=NULL;
            analogs=(BASEANALOG_STRU *)anatab.rdbgetnextrcdaddr())
    {
        if (analogs->name[0]=='\0')continue;

        if (analogs->olgrade)
        {
            statsave = (DEVSTATSAVE *)daystatmalloc->get_nextunitaddr();

            if (statsave == NULL)  break;

            memset(statsave,0,sizeof(DEVSTATSAVE));

            statsave->savedate = odate;

            strcpy(statsave->name, analogs->name);

            statsave->devtype = HISDEV_ANALOG_OL;

            statsave->lastdevstatus=analogs->olgrade;
        }
    }

    Rdb_Tab rdbtablecl;

    rdbtablecl.RdbLogin(MyUserName, MyPassWord);

    HOSTSTATUS_STRU  *dfhostp;

    if (rdbtablecl.RdbOpenTab("hoststatus")==RDB_OK)
    {
        for (dfhostp=(HOSTSTATUS_STRU *)rdbtablecl.rdbgetfirstrcdaddr();dfhostp!=NULL;
                dfhostp=(HOSTSTATUS_STRU *)rdbtablecl.rdbgetnextrcdaddr())
        {
            if (dfhostp->name[0]=='\0')continue;

            statsave = (DEVSTATSAVE *)daystatmalloc->get_nextunitaddr();

            if (statsave == NULL)break;

            memset(statsave,0,sizeof(DEVSTATSAVE));

            statsave->savedate = odate;

            strcpy(statsave->name, dfhostp->name);

            statsave->devtype = HISDEV_HOST; //通道运行情况

            statsave->lastdevstatus=dfhostp->hoststatus;
        }
    }

    return OK;
}

// 转存日存盘数据（开关统计量、TERMINAL统计量)时间
int Hisdata::dumpdaystat(void)
{
    int ret,rownums=0;

    if (daystatmalloc==NULL) return -1;

    DEVSTATSAVE *sdiskdata=(DEVSTATSAVE *)daystatmalloc->get_mallocinfo(&rownums);

    if (sdiskdata==NULL || rownums==0)
    {
        delete daystatmalloc;
        daystatmalloc=NULL;
        return -1;
    }

    static  int     devstatfileno=0;

    char tmpfilename[64],savefilename[200];

    sprintf(tmpfilename,"DEVSTATSAVE%d",devstatfileno);
    char *sacenv=getenv(NBENV);

    if (sacenv==NULL)
    {
        strcpy(savefilename,tmpfilename);
    }
    else
    {
        strcpy(savefilename,sacenv);
#ifdef WIN32
        strcat(savefilename,"\\procscript\\");
#else
        strcat(savefilename,"/procscript/");
#endif
        strcat(savefilename,tmpfilename);
    }

    devstatfileno++;

    devstatfileno = devstatfileno%100;

    FILE    *savefp=fopen(savefilename,"wb");

    if (savefp==NULL)
    {
        delete daystatmalloc;
        daystatmalloc=NULL;
        return -1;
    }

    ret=fwrite(sdiskdata,sizeof(DEVSTATSAVE),rownums,savefp);

    fclose(savefp);

    delete daystatmalloc;
    daystatmalloc=NULL;

    if (ret!=rownums)
    {
        remove(savefilename);
        return(-1);
    }

    HDBSaveTaskMan   hdbstask;

    hdbstask.AddHDBSaveTask(3,DEVSTAT_TABNAME,0,0,0,0,rownums,savefilename);

    return(OK);
}


/////////////////////////////////////////////////////////////////////////////////
//////////////////// 处理分钟级的数据定时存盘 ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//int  SAVE_KWHRAWDATA=GetProgramRunPara("savekwhrawd",0);


int  proc_mindatasave(SYS_CLOCK  *curclock)
{
    int			ret,retcode,tmpsaveflag;
    Hisdata		hisdata;
    Rdb_Tab	    memtab(MyUserName,MyPassWord);
    Kwh			savekwh;
    SYS_CCLOCK	curccclock,tmpcclock;
//	VoltLimit   voltlimit;
//	VOLTLIMIT_STRU  *vlimitparap;
    BASEANALOG_STRU	*analogp;
    KWH_STRU			*kwhp;
    BREAK_STRU *breakp;
    SWITCH_STRU *switchp;
    float tmp;
    int i;

    // 时间转换
    ConvertClock(curclock,&curccclock);
    hisdata.datainit();
    ret=memtab.RdbOpenTab(ANALOG_TABNAME);		// 打开模拟量表

    if (ret==RDB_OK)
    {
        memtab.RdbLockTabWithRW();
        i=0;

        for (analogp=(BASEANALOG_STRU *)memtab.rdbgetfirstrcdaddr(); analogp!=NULL; analogp=(BASEANALOG_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (analogp->name[0]=='\0')continue;

            //if (analogp->savestyle==TIMER_NOSAVE || analogp->savedensity<=0)continue;
            if (analogp->savedensity<=0)continue;

            if ( curccclock.time%analogp->savedensity==0)
            {
                i++;
                /*if(analogp->savestyle==TIMER_SAVE_AVERAGE)	{
                	SetTimerSavingFlag(1);  // timer_saving_flag=1;
                	tmpint1=analogp->sumtimes;
                	if(tmpint1>0)tmp=(float)analogp->sumvalue/((float)tmpint1);
                	else   tmp=analogp->value;
                	analogp->sumtimes=0;
                	analogp->sumvalue=0.0;
                }
                else tmp=analogp->value;*/
                tmp=analogp->value;//只存瞬时值
                tmpsaveflag=HISDATA_REAL;

                if (analogp->mansetf)		// 人工置数
                    tmpsaveflag=HISDATA_LOCALSET;
                else if (analogp->terminalerror)
                    tmpsaveflag=HISDATA_TERMINALERROR;
                else if (analogp->deadf)  // 死数
                    tmpsaveflag=HISDATA_DEADVAL;
                else if (IS_YC_OVER_LIMIT(analogp))
                {
                    if (analogp->olgrade==ANAOL_HIGH || analogp->olgrade==ANAOL_HIHI)
                        tmpsaveflag=HISDATA_OVERHIGHTLMT;
                    else if (analogp->olgrade==ANAOL_LOW || analogp->olgrade==ANAOL_LOLO)
                        tmpsaveflag=HISDATA_OVERLOWLMT;
                }

                /*
                  采用电压限值表
                  else if(IS_VOLT_TYPE(analogp->baseobjtype) && (!ISZERO(tmp))) {
                					vlimitparap=voltlimit.GetVoltLimit(((ANALOG_STRU *)analogp)->voltlmt);
                					if(vlimitparap) {
                						if(tmp>vlimitparap->upvolt)tmpsaveflag=HISDATA_OVERVOLUP;
                						else if(tmp<vlimitparap->downvolt)tmpsaveflag=HISDATA_OVERVOLDOWN;
                					}
                				}
                */
//改为由模拟量表中上下限值判断越限
                else if (IS_VOLT_TYPE(analogp->baseobjtype) && (!ISZERO(tmp)))
                {
                    if ((analogp->highlimit > analogp->lowlimit) && (analogp->highlimit!=0) && (analogp->lowlimit!=0))
                    {
                        if (tmp>analogp->highlimit)tmpsaveflag=HISDATA_OVERVOLUP;
                        else if (tmp<analogp->lowlimit)tmpsaveflag=HISDATA_OVERVOLDOWN;
                    }
                }

                hisdata.storedata(0,analogp->name,tmp,analogp->rawdata,tmpsaveflag);
            }//if
        } // for

        memtab.RdbUnlockTab();
    }//if RDB_OK

    SetTimerSavingFlag(0); //timer_saving_flag=0;

    if (curccclock.time==0)
    {	// 如果是0:0
        tmpcclock=curccclock;
        tmpcclock.date--;
        tmpcclock.time=24*60;
        retcode=hisdata.dumpdata(0,&tmpcclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 遥测  %d分钟数据错误 ret=%d\n",tmpcclock.time,retcode);
    }
    else
    {
        retcode=hisdata.dumpdata(0,&curccclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 遥测  %d分钟数据错误 ret=%d\n",curccclock.time,retcode);
    }

///////////////////////////////////////电度量存盘//////////////////////////////////////////////////
    hisdata.datainit();

    ret=memtab.RdbOpenTab(KWH_TABNAME);

    if (ret==RDB_OK)
    {
        memtab.RdbLockTabWithRW();
        i=0;

        for (kwhp=(KWH_STRU *)memtab.rdbgetfirstrcdaddr(); kwhp!=NULL; kwhp=(KWH_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (kwhp->name[0]=='\0')continue;

            if (kwhp->savedensity<=0) continue;

            if (curccclock.time%kwhp->savedensity==0)
            {
                i++;
                hisdata.storedata(1,kwhp->name,savekwh.GetSaveKwhVal(kwhp),kwhp->rawdata,HISDATA_REAL);

                //if (!kwhp->calcultf && !kwhp->integralf)
				if (!kwhp->integralf)//zxm 修改  计算量存表码 20160131
                {
                    hisdata.storedata(1,kwhp->name,kwhp->tabcode,kwhp->rawdata,HISDATA_KWHTABCODE);//存表码
                }

                /*			if(SAVE_KWHRAWDATA) {
                				hisdata.storedata(1,kwhp->name,(double)kwhp->rawdata,HISDATA_KWHRAWDATA);
                			}*/
            }
        }//for

        memtab.RdbUnlockTab();
    }

    if (curccclock.time==0)
    {
        tmpcclock=curccclock;
        tmpcclock.date--;
        tmpcclock.time=24*60;
        retcode=hisdata.dumpdata(1,&tmpcclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 电度  %d分钟数据错误 ret=%d\n",tmpcclock.time,retcode);
    }
    else
    {
        retcode=hisdata.dumpdata(1,&curccclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 电度  %d分钟数据错误 ret=%d\n",curccclock.time,retcode);
    }

///////////////////////////////////////开关量存盘//////////////////////////////////////////////////
    if (KG_SAVE_INTERVAL>=0)
    {
        if (KG_SAVE_INTERVAL==0 && curccclock.time!=0) return(OK);

        hisdata.datainit();

        ret=memtab.RdbOpenTab(BREAK_TABNAME);		// 打开开关量表

        if (ret==RDB_OK)
        {
            memtab.RdbLockTabWithRW();
            i=0;

            for (breakp=(BREAK_STRU *)memtab.rdbgetfirstrcdaddr(); breakp!=NULL; breakp=(BREAK_STRU *)memtab.rdbgetnextrcdaddr())
            {
                if (breakp->basestatus.name[0]=='\0')continue;

                if (KG_SAVE_INTERVAL==0)
                {
                    i++;
                    tmpsaveflag=HISDATA_REAL;

                    if (breakp->basestatus.mansetf) tmpsaveflag=HISDATA_LOCALSET;
                    else if (breakp->basestatus.terminalerror)	tmpsaveflag=HISDATA_TERMINALERROR;

                    hisdata.storedata(2,breakp->basestatus.name,breakp->basestatus.value,0,tmpsaveflag);
                }
                else if ( curccclock.time%KG_SAVE_INTERVAL==0)
                {
                    i++;
                    tmpsaveflag=HISDATA_REAL;

                    if (breakp->basestatus.mansetf) tmpsaveflag=HISDATA_LOCALSET;
                    else if (breakp->basestatus.terminalerror)	tmpsaveflag=HISDATA_TERMINALERROR;

                    hisdata.storedata(2,breakp->basestatus.name,breakp->basestatus.value,0,tmpsaveflag);
                }
            } // for

            memtab.RdbUnlockTab();
        }//if RDB_OK

        SetTimerSavingFlag(0); //timer_saving_flag=0;

        retcode=hisdata.dumpdata(2,&curccclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 开关  %d分钟数据错误 ret=%d\n",curccclock.time,retcode);
    }

///////////////////////////////////////刀闸量存盘//////////////////////////////////////////////////

    if (DZ_SAVE_INTERVAL>=0)
    {
        if (DZ_SAVE_INTERVAL==0 && curccclock.time!=0) return(OK);

        hisdata.datainit();

        ret=memtab.RdbOpenTab(SWITCH_TABNAME);		// 打开刀闸量表

        if (ret==RDB_OK)
        {
            memtab.RdbLockTabWithRW();
            i=0;

            for (switchp=(SWITCH_STRU *)memtab.rdbgetfirstrcdaddr(); switchp!=NULL; switchp=(SWITCH_STRU *)memtab.rdbgetnextrcdaddr())
            {
                if (switchp->basestatus.name[0]=='\0')continue;

                if (DZ_SAVE_INTERVAL==0)
                {
                    i++;
                    tmpsaveflag=HISDATA_REAL;

                    if (switchp->basestatus.mansetf) tmpsaveflag=HISDATA_LOCALSET;
                    else if (switchp->basestatus.terminalerror)	tmpsaveflag=HISDATA_TERMINALERROR;

                    hisdata.storedata(2,switchp->basestatus.name,switchp->basestatus.value,0,tmpsaveflag);
                }
                else if ( curccclock.time%DZ_SAVE_INTERVAL==0)
                {
                    i++;
                    tmpsaveflag=HISDATA_REAL;

                    if (switchp->basestatus.mansetf) tmpsaveflag=HISDATA_LOCALSET;
                    else if (switchp->basestatus.terminalerror)	tmpsaveflag=HISDATA_TERMINALERROR;

                    hisdata.storedata(3,switchp->basestatus.name,switchp->basestatus.value,0,tmpsaveflag);
                }
            } // for

            memtab.RdbUnlockTab();
        }//if RDB_OK

        SetTimerSavingFlag(0); //timer_saving_flag=0;

        retcode=hisdata.dumpdata(3,&curccclock,1);

        if (retcode!=OK) RdbDebugMesg("\n 存 刀闸  %d分钟数据错误 ret=%d\n",curccclock.time,retcode);
    }

    return(OK);
}


/////////////////////////////////////////////////////////////////////////////////
//////////////////// 处理分钟级的统计数据定时存盘 ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

static int proc_jigu_sdkwh_flag=-1;

int  proc_daydatasave(SYS_CLOCK  *curclock)
{
    int			ret,tmpint1,tmpint2;
    float 		tmpfloat;
    Hisdata		hisdata;
    SYS_CCLOCK	curccclock;
    BASEANALOG_STRU  *analogp;
    Rdb_Tab	memtab(MyUserName, MyPassWord);

    // 取昨天日期
    ConvertClock(curclock,&curccclock);
    curccclock.date--;

    hisdata.daydatainit();
    ret=memtab.RdbOpenTab(ANALOG_TABNAME);

    if (ret==RDB_OK)
    {
        memtab.RdbLockTabWithRW();

        for (analogp=(BASEANALOG_STRU *)memtab.rdbgetfirstrcdaddr(); analogp!=NULL; analogp=(BASEANALOG_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (analogp->name[0]=='\0')continue;

            //if (analogp->savestyle==TIMER_NOSAVE || analogp->savedensity<=0)continue;
            if (analogp->savedensity<=0)continue;

            //最大最小值
            hisdata.storedaydata(0,analogp->name,analogp->maxvalue,analogp->maxrawdata,HISDATA_REALMAX,curccclock.date,
                                 (int)(analogp->maxhour*60+analogp->maxminute),analogp->maxsecond);

            hisdata.storedaydata(0,analogp->name,analogp->minvalue,analogp->minrawdata,HISDATA_REALMIN,curccclock.date,
                                 (int)(analogp->minhour*60+analogp->minminute),analogp->minsecond);

            // 计算合格率
            if (IS_VOLT_TYPE(analogp->baseobjtype))
            {
                tmpint2=24*3600-analogp->dstoptime;

                if (tmpint2>0)    // 停运一天的量不存合格率
                {
                    tmpint1=24*3600-analogp->overlimittimeh-analogp->overlimittimel
                            -analogp->overlimittimehh-analogp->overlimittimell-analogp->dstoptime;

                    if (tmpint1<0)tmpint1=0;

                    if (tmpint1==0)tmpfloat=0.0;
                    else tmpfloat=(((float)tmpint1/(float)tmpint2));//合格率

                    hisdata.storedaydata(0,analogp->name,tmpfloat,0,HISDATA_REALHGL,curccclock.date);
                }
            }

            /*			else {
            				tmpfloat=(float)(((float)(24*3600-analogp->deadtime))/(24.0*3600.0));
            				hisdata.storedaydata(0,analogp->name,tmpfloat,0,HISDATA_REALHGL,curccclock.date);
            			}*/


            //存储平均值
            tmpint1=analogp->daysumtimes;

            if (tmpint1>0) tmpfloat = (float)(analogp->daysumvalue)/(float)(tmpint1);
            else tmpfloat =analogp->value;

            hisdata.storedaydata(0,analogp->name,tmpfloat,0, HISDATA_AVERAGE,curccclock.date);

            //存负荷率
            if (!ISZERO(analogp->maxvalue))
            {
                if (IS_PYC_TYPE(analogp->baseobjtype) || IS_QYC_TYPE(analogp->baseobjtype))
                    hisdata.storedaydata(0, analogp->name, ABS(tmpfloat/analogp->maxvalue),0, HISDATA_PQRATE,curccclock.date);
            }

            //存越限次数
            if (analogp->polgrade>=1)
            {
                hisdata.storedaydata(0,analogp->name, analogp->overlimitcounth,0,  HISDATA_OVHCOUNT,curccclock.date);
                hisdata.storedaydata(0,analogp->name, analogp->overlimitcountl,0,  HISDATA_DOWNLCOUNT,curccclock.date); //一级越限

                //存越限时间
                hisdata.storedaydata(0,analogp->name, analogp->overlimittimeh,0, HISDATA_OVHTIME,curccclock.date);
                hisdata.storedaydata(0,analogp->name, analogp->overlimittimel,0, HISDATA_DOWNLTIME,curccclock.date);

                if (analogp->polgrade>1)
                {
                    hisdata.storedaydata(0,analogp->name, analogp->overlimitcounthh,0, HISDATA_OVHHCOUNT,curccclock.date);//二级越限
                    hisdata.storedaydata(0,analogp->name, analogp->overlimitcountll,0, HISDATA_DOWNLLCOUNT,curccclock.date);
                    hisdata.storedaydata(0,analogp->name, analogp->overlimittimehh,0, HISDATA_OVHHTIME,curccclock.date);
                    hisdata.storedaydata(0,analogp->name, analogp->overlimittimell,0, HISDATA_DOWNLLTIME,curccclock.date);
                }
            }

            // 存日死数时间
            hisdata.storedaydata(0,analogp->name, analogp->deadtime,0, HISDATA_DAYDEADTIME,curccclock.date);

            //  存日停运时间
            if (IS_VOLT_TYPE(analogp->baseobjtype))
            {
                hisdata.storedaydata(0,analogp->name, analogp->dstoptime,0, HISDATA_DAYSTOPTIME,curccclock.date);
            }
        }

        memtab.RdbUnlockTab();

        memtab.RdbCloseTab();
    }

    ret=hisdata.dumpdaydata(0,&curccclock);

    if (ret!=OK) RdbDebugMesg("\n 存 YC 日实时统计数据错误 ret=%d\n",ret);


    KWH_STRU *kwhp;

    hisdata.daydatainit();

    ret=memtab.RdbOpenTab(KWH_TABNAME);

    if (ret==RDB_OK)
    {
        if (proc_jigu_sdkwh_flag<0)
        {
            proc_jigu_sdkwh_flag=0;

            if (memtab.RdbGetColNo("daysdkwh5")>0)proc_jigu_sdkwh_flag=1;
        }

        memtab.RdbLockTabWithRW();

        for (kwhp=(KWH_STRU *)memtab.rdbgetfirstrcdaddr(); kwhp!=NULL; kwhp=(KWH_STRU *)memtab.rdbgetnextrcdaddr())
        {
            if (kwhp->name[0]=='\0')continue;

            if (kwhp->savedensity<=0) continue;

            for (char i=0; i<SDTYPENUMS; i++)
                hisdata.storedaydata(1, kwhp->name, kwhp->daysdkwh[i],0, HISDATA_DAYKWHSD1+i); //日时段电量

            if (proc_jigu_sdkwh_flag)
                hisdata.storedaydata(1, kwhp->name, kwhp->daysdkwh5,0, HISDATA_DAYKWHSD1+4); //日脊谷时段电量

            hisdata.storedaydata(1, kwhp->name, kwhp->daykwh,0, HISDATA_DAYKWHTTL); //日总电量
        }

        memtab.RdbUnlockTab();

        memtab.RdbCloseTab();
    }

    ret=hisdata.dumpdaydata(1,&curccclock); //存日统计电量

    if (ret!=OK) RdbDebugMesg("\n 存 KWH 日实时统计数据错误 ret=%d\n",ret);



    SYS_CLOCK   tmpclock;

    tmpclock=*curclock;

    IncDecDays(&tmpclock,-1);

    if (hisdata.storedaystat(&tmpclock)==OK) hisdata.dumpdaystat();

    return(OK);
}


////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 分钟定时存历史数据  //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#ifdef _AIX
void         KwhDayInit(void);
#endif

#ifdef __linux
void         KwhDayInit(void);
#endif
//#ifdef __unix
void  *timer_proc_min_hisdata(void *p)
//#else
//UINT timer_proc_min_hisdata(LPVOID)
//#endif
{
    SYS_CLOCK   curclock;
    Analog		analog;
    Break		breaker;
    Terminal    terminal;
//    Ftu         ftu;
    Channel     channel;
    Substation  subst;
    HDBTimer    hdbtimer(60);

    for (;;)
    {
        if (hdbtimer.TimerOK())
        {					// 1 minute timer
            GetCurrentClock(&curclock);
            // 分钟级数据存盘
            proc_mindatasave(&curclock);
            // 如果是 0:0，日统计数据存盘并清零

            if (curclock.hour==0 && curclock.minute==0)
            {
                // 日统计数据存盘
                proc_daydatasave(&curclock);
                // 清零
                analog.DayInit();
                breaker.DayInit();
                KwhDayInit();
                terminal.DayInit();
//                ftu.DayInit();
                channel.DayInit();
                KwhDayInit();
            }
        }

        sleepms(2,0);
    }

    return(NULL);
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////// 秒级曲线 ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


#define SECCURVEPARA_TABNAME    "securvepara"

typedef struct
{
    char  name[OBJECTNAME_LEN];
    ushort   savedensity;
    float    decvalue;
} SECCURVE_PARA;



typedef struct
{
    char   sname[12];
    ushort date;
    ushort time;
    uchar  second;
    uchar  flag;
    float  data;
} SECDATA_STRU;

class SecHisData
{
    Rdb_PreMalloc  sechdatamalloc;

public:
    SecHisData(void)
    {
        sechdatamalloc.set_mallocpara(100,sizeof(SECDATA_STRU));
    }

    void secdatainit(void);
    int storesecdata(char *objname,int date,int time,int second,int flag,float data);
    int dumpsecdata(SYS_CCLOCK *datatime);
};

void SecHisData::secdatainit(void)
{
    sechdatamalloc.freespace();
}

int SecHisData::storesecdata(char *objname,int date,int time,int second,int flag,float data)
{
    SECDATA_STRU  *sdiskdata;

    sdiskdata=(SECDATA_STRU  *)sechdatamalloc.get_nextunitaddr();
    strcpy(sdiskdata->sname,objname);
    sdiskdata->date=date;
    sdiskdata->time=time;
    sdiskdata->second=second;
    sdiskdata->data=data;
    sdiskdata->flag=flag;
    return(OK);
}


int SecHisData::dumpsecdata(SYS_CCLOCK *datatime)
{
    SECDATA_STRU        *sdiskdata;
    int             ret,rownums;
    SYS_CLOCK		sysclock;
    static  int     secdatafileno=0;

    sdiskdata=(SECDATA_STRU *)sechdatamalloc.get_mallocinfo(&rownums);

    if (sdiskdata==NULL || rownums<=0)	return(OK);

    char tmpfilename[64],savefilename[200];

    sprintf(tmpfilename,"SECDATASAVE%d",secdatafileno);

    char *sacenv=getenv(NBENV);

    if (sacenv==NULL)
    {
        strcpy(savefilename,tmpfilename);
    }
    else
    {
        strcpy(savefilename,sacenv);
#ifdef WIN32
        strcat(savefilename,"\\procscript\\");
#else
        strcat(savefilename,"/procscript/");
#endif
        strcat(savefilename,tmpfilename);
    }

    secdatafileno++;

    secdatafileno = secdatafileno%3000;

    FILE    *savefp=fopen(savefilename,"wb");

    if (savefp==NULL)
    {
        sechdatamalloc.freespace();
        return -1;
    }

    ret=fwrite(sdiskdata,sizeof(SECDATA_STRU),rownums,savefp);

    fclose(savefp);

    sechdatamalloc.freespace();

    if (ret!=rownums)
    {
        remove(savefilename);
        return(-1);
    }

    HDBSaveTaskMan   hdbstask;

    char tabname[128];


    SYS_CCLOCK tmpdatatime;
    tmpdatatime=*datatime;
    tmpdatatime.time=0;
    ConvertClock(&tmpdatatime,&sysclock);

    // 脉冲值和非脉冲值打开不同的表，并置标志
    sprintf(tabname,"sdata%.4d%.2d",sysclock.year,sysclock.month);

    hdbstask.AddHDBSaveTask(4,tabname,0,INS_MIN_HISDATA,datatime->date,
                            datatime->time,rownums,savefilename);

    return(OK);
}


////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 处理秒级实时数据  ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#define  SECCURVE_SECOND_PERIOD   5


SecHisData		sechisdata;

typedef struct
{
    SECCURVE_PARA  secuvpara;
    float          curvalue;
    uchar		   initflag;
} SECCURVE_PARADATA;

class SecRTData
{
    static int					opentabflag;
    static Rdb_Tab				secuvparatab;
    static int					pararcdnums;
    static SECCURVE_PARADATA	*secuv_paradata;
    static int					paraupdtime;

public:
    SecRTData(void);
    void UpdatePara(void);
    void SetInitFlag(int initflag);
    void FreshData(SYS_CCLOCK  *curccclock,uchar  cursec);
};

int  SecRTData::opentabflag=0;
Rdb_Tab SecRTData::secuvparatab;
int  SecRTData::pararcdnums=0;
int  SecRTData::paraupdtime=0;
SECCURVE_PARADATA  *SecRTData::secuv_paradata=NULL;

SecRTData::SecRTData(void)
{
    if (opentabflag==0)
    {
        int ret;

        if ((ret=secuvparatab.RdbLogin(MyUserName,MyPassWord))!=RDB_OK)return;

        if ((ret=secuvparatab.RdbOpenTab(SECCURVEPARA_TABNAME))!=RDB_OK)return;

        opentabflag=1;

        int maxrcdnums=secuvparatab.RdbGetMaxRcdCnt();

        if (maxrcdnums<=0)return;

        secuv_paradata=new SECCURVE_PARADATA[maxrcdnums];

        memset(secuv_paradata,0,maxrcdnums * sizeof(SECCURVE_PARADATA));

        UpdatePara();

        secuvparatab.GetStructTime(&paraupdtime);
    }
}

void SecRTData::UpdatePara(void)
{
    if (opentabflag==0)return;

    int i;

    pararcdnums=secuvparatab.RdbGetRcdCnt();

    SECCURVE_PARA	*tmpsecparap;

    for (i=0;i<pararcdnums;i++)
    {
        tmpsecparap=(SECCURVE_PARA  *)secuvparatab.RdbGetRecordAddr(i);

        if (tmpsecparap==NULL)return;

        secuv_paradata[i].secuvpara=*tmpsecparap;

        if (secuv_paradata[i].secuvpara.savedensity<=0)continue;

        if (secuv_paradata[i].secuvpara.savedensity%SECCURVE_SECOND_PERIOD!=0)
        {
            secuv_paradata[i].secuvpara.savedensity = secuv_paradata[i].secuvpara.savedensity
                    + SECCURVE_SECOND_PERIOD-secuv_paradata[i].secuvpara.savedensity%SECCURVE_SECOND_PERIOD;
        }
    }
}

void SecRTData::SetInitFlag(int initflag)
{
    if (opentabflag==0)return;

    int i;

    for (i=0;i<pararcdnums;i++)
    {
        secuv_paradata[i].initflag=initflag;
    }
}


int  seccurve_allsaveflag=GetProgramRunPara("secuvalls",0);

void SecRTData::FreshData(SYS_CCLOCK  *curccclock,uchar  cursec)
{
    if (opentabflag==0)return;

    int			i,tmpsecs;

    Analog		analogcl;

    float tmpfloat=(float)0.0;

    intertime  tmptime;

    secuvparatab.GetStructTime(&tmptime);

    if (tmptime!=paraupdtime)
    {
        UpdatePara();
        SetInitFlag(1);
        paraupdtime=tmptime;
    }

    tmpsecs=curccclock->time * 60 + cursec;

    for (i=0;i<pararcdnums;i++)
    {
        if (secuv_paradata[i].secuvpara.name[0]=='\0')continue;

        if (secuv_paradata[i].secuvpara.savedensity<=0)continue;

        if (tmpsecs%secuv_paradata[i].secuvpara.savedensity==0)
        {
            if (analogcl.GetAnaValue(secuv_paradata[i].secuvpara.name,&tmpfloat)!=OK)continue;

            if (secuv_paradata[i].initflag || seccurve_allsaveflag)
            {
                secuv_paradata[i].curvalue=tmpfloat;
                sechisdata.storesecdata(secuv_paradata[i].secuvpara.name,curccclock->date,curccclock->time,
                                        cursec,0,secuv_paradata[i].curvalue);
            }
            else
            {
                if (ABS(tmpfloat-secuv_paradata[i].curvalue)>secuv_paradata[i].secuvpara.decvalue)
                {
                    secuv_paradata[i].curvalue=tmpfloat;
                    sechisdata.storesecdata(secuv_paradata[i].secuvpara.name,curccclock->date,curccclock->time,
                                            cursec,0,secuv_paradata[i].curvalue);
                }
            }

            secuv_paradata[i].initflag=0;
        }//if
    } // for
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 处理秒级历史数据定时存盘  ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void  *timer_proc_sec_hisdata(void *p)
{
    SYS_CLOCK    curclock;
    SYS_CCLOCK	 curccclock;
    int			ret;
    HDBTimer    hdbtimer(5);
    int			lasttimevalue=-1,tmpinitflag;
    int         mainbackflag=0;
    SecRTData  secrtdata;
    NetStatus netstat;
    int *host_group_no=netstat.GetHostGrpEnm();
    int grpnum=netstat.GetHostGrpNum();
    int savegroupno;

    for (int j=0;j<grpnum;j++)
    {
        if (IsMainServer(host_group_no[j]))
        {
            savegroupno=host_group_no[j];
            break;
        }
    }

    if (IsMainServer(savegroupno))mainbackflag=1;
    else mainbackflag=0;

    for (;;)
    {
        if (hdbtimer.TimerOK())
        {
            GetCurrentClock(&curclock);
            curclock.second -= (curclock.second%SECCURVE_SECOND_PERIOD);
            ConvertClock(&curclock,&curccclock);

            if (curccclock.time==0 && curclock.second==0)
            {
                curccclock.date--;
                curccclock.time=1440;
            }

            tmpinitflag=0;

            if (lasttimevalue<0)tmpinitflag=1;
            else if (lasttimevalue!=curccclock.time)
            {
                if (curccclock.time==1440) tmpinitflag=1;
                else if (curccclock.time==0) tmpinitflag=1;
            }

            lasttimevalue=curccclock.time;

            if (IsMainServer(savegroupno))
            {
                if (mainbackflag==0)tmpinitflag=1;

                mainbackflag=1;
            }
            else mainbackflag=0;

            if (tmpinitflag)secrtdata.SetInitFlag(1);

            secrtdata.FreshData(&curccclock,curclock.second);

            if (curclock.second>=(60-SECCURVE_SECOND_PERIOD))
            {
                ret=sechisdata.dumpsecdata(&curccclock);
                sechisdata.secdatainit();
            }
            else if (curccclock.time==1440)
            {
                ret=sechisdata.dumpsecdata(&curccclock);
                sechisdata.secdatainit();
            }
        }

        sleepms(1,0);
    }

    return(NULL);
}


////////////////////////////////////////////////////////////////////////////////////
////////////////////////  文件 --------- 》 DB /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

int  SaveHisDataToDB(HDB_SAVETASK  *hdbsavetaskp)
{
    CS_RETCODE      retcode;
    FUNC_STR        func;
    CS_DATAFMT      *datafmt=NULL;
    HIS_DATA        *sdiskdata;
    NetDbg		    dbginfo;
    int             ret;
    FILE			*readfp;

    Rdb_Malloc     sdiskmalloc(sizeof(HIS_DATA) * hdbsavetaskp->rownums);
    sdiskdata=(HIS_DATA *)sdiskmalloc.GetAddr();

    readfp=fopen(hdbsavetaskp->savefilename,"rb");

    if (readfp==NULL) return -1;

    ret=fread(sdiskdata,sizeof(HIS_DATA),hdbsavetaskp->rownums,readfp);

    fclose(readfp);

    if (ret!=hdbsavetaskp->rownums)return -1;


    strcpy(func.dbname,"hisdb"); // 服务器名

    strcpy(func.table,hdbsavetaskp->tabname);

    func.his.pulseflag=hdbsavetaskp->pulseflag;

    func.func=INS_MIN_HISDATA;

    func.roxnum=hdbsavetaskp->rownums;

    func.his.date=hdbsavetaskp->date;

    func.his.time=hdbsavetaskp->time;

/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.date);

    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.time);

    scdnetbyte(RDB_DATATYPE_SHORT,&func.his.pulseflag);

    for (int i=0; i<hdbsavetaskp->rownums; i++)
    {
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->date);
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->time);
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->ms);

        if (hdbsavetaskp->pulseflag==NOTPULSE)
        {
            scdnetbyte(RDB_DATATYPE_FLOAT,&sdiskdata->val.ycdata);
            scdnetbyte(RDB_DATATYPE_SHORT,&sdiskdata->rawval.ycdata);
        }
        else
        {
            scdnetbyte(RDB_DATATYPE_FLOAT,&sdiskdata->val.kwhdata);
            scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->rawval.kwhdata);
        }

        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->odate);

        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->otime);
        sdiskdata ++;
    }

    sdiskdata-=hdbsavetaskp->rownums;

#endif
  */

	//解决oracle最多插入65534条记录的问题 20110804 by lhl
//	int start = GetTickCount();
// 	if( func.roxnum > MAXRCDNUM )
// 	{
// 		int insertrcdnum = 0;
// 		while( insertrcdnum < hdbsavetaskp->rownums )
// 		{
// 			if( hdbsavetaskp->rownums - insertrcdnum > MAXRCDNUM )
// 			{
// 				func.roxnum = MAXRCDNUM;
// 			}
// 			else
// 			{
// 				func.roxnum = hdbsavetaskp->rownums - insertrcdnum;
// 			}
// 			retcode=dbapi.InsertHisdata(&func,&sdiskdata[ insertrcdnum ] );// 插入定时存盘数据
// 			if( retcode != CS_SUCCEED )
// 				break;
// 
// 			insertrcdnum += func.roxnum;
// 		}
// 	}
// 	else
	{
		retcode=dbapi.InsertHisdata(&func,(HIS_DATA *)sdiskdata);// 插入定时存盘数据
	}

// 	char sqlstr[ 128 ];
// 	FUNC_STR tmpfunc;
// 	memcpy( &tmpfunc, &func, sizeof( FUNC_STR ) );
// 	strcpy( tmpfunc.table, "yctmp" );
// 	retcode=dbapi.InsertHisdata(&tmpfunc,(HIS_DATA *)sdiskdata);// 插入定时存盘数据
// 
// 	if( retcode == CS_SUCCEED )
// 	{
// 		tmpfunc.func = ISQL_COMMAND;
// 		sprintf( sqlstr, "insert/*+append*/ into %s select * from yctmp", func.table );
// 		retcode = dbapi.IsqlCommand( &func, sqlstr );
// 		sprintf( sqlstr, "truncate table yctmp" );
// 		dbapi.IsqlCommand( &func, sqlstr );
// 	}
// 
// 	int end = GetTickCount();
// 
// 	int spendtime = end - start;

    if (datafmt!=NULL)free(datafmt);// 释放数据格式

    if (retcode==CS_SUCCEED)return(OK);
    else
    {
        dbginfo.PutDbg(NET_DBGTYPE_SCADA,"SCADA:\t函数InsertHisdata插入记录失败!\t表名：%s\r\n",func.table);
        return(SCADA_ERR_ADDRCD);
    }
}

int  SaveHisDayDataToDB(HDB_SAVETASK  *hdbsavetaskp)
{
    CS_RETCODE      retcode;
    FUNC_STR        func;
    CS_DATAFMT      *datafmt=NULL;
    HIS_DATA        *sdiskdata;
    NetDbg		    dbginfo;
    int             ret;
    FILE			*readfp;

    Rdb_Malloc     sdiskmalloc(sizeof(HIS_DATA) * hdbsavetaskp->rownums);
    sdiskdata=(HIS_DATA *)sdiskmalloc.GetAddr();

    readfp=fopen(hdbsavetaskp->savefilename,"rb");

    if (readfp==NULL) return -1;

    ret=fread(sdiskdata,sizeof(HIS_DATA),hdbsavetaskp->rownums,readfp);

    fclose(readfp);

    if (ret!=hdbsavetaskp->rownums)return -1;



    strcpy(func.dbname,"hisdb"); // 服务器名

    strcpy(func.table,hdbsavetaskp->tabname);

    func.his.pulseflag=hdbsavetaskp->pulseflag;

    func.func=INS_STAT_HISDATA;

    func.roxnum=hdbsavetaskp->rownums;

    func.his.date=hdbsavetaskp->date;

    func.his.time=0;
/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.date);

    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.time);

    scdnetbyte(RDB_DATATYPE_SHORT,&func.his.pulseflag);

    for (int i=0; i<hdbsavetaskp->rownums; i++)
    {
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->date);
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->time);
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->ms);

        if (hdbsavetaskp->pulseflag==NOTPULSE)
        {
            scdnetbyte(RDB_DATATYPE_FLOAT,&sdiskdata->val.ycdata);
            scdnetbyte(RDB_DATATYPE_SHORT,&sdiskdata->rawval.ycdata);
        }
        else
        {
            scdnetbyte(RDB_DATATYPE_FLOAT,&sdiskdata->val.kwhdata);
            scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->rawval.kwhdata);
        }

        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->odate);

        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->otime);
        sdiskdata ++;
    }

    sdiskdata-=hdbsavetaskp->rownums;

#endif
*/
	//解决oracle最多插入65534条记录的问题 20110804 by lhl
// 	if( func.roxnum > MAXRCDNUM )
// 	{
// 		int insertrcdnum = 0;
// 		while( insertrcdnum < hdbsavetaskp->rownums )
// 		{
// 			if( hdbsavetaskp->rownums - insertrcdnum > MAXRCDNUM )
// 			{
// 				func.roxnum = MAXRCDNUM;
// 			}
// 			else
// 			{
// 				func.roxnum = hdbsavetaskp->rownums - insertrcdnum;
// 			}
// 			retcode=dbapi.InsertHisdata(&func,&sdiskdata[ insertrcdnum ] );
// 			if( retcode != CS_SUCCEED )
// 				break;
// 			
// 			insertrcdnum += func.roxnum;
// 		}
// 	}
// 	else
	{
		retcode=dbapi.InsertHisdata(&func,(HIS_DATA *)sdiskdata);
	}

    if (datafmt!=NULL)free(datafmt);

    if (retcode==CS_SUCCEED)return(OK);
    else
    {
        dbginfo.PutDbg(NET_DBGTYPE_SCADA,"SCADA:\t函数InsertHisdata失败!\t表名：%s\r\n",func.table);
        return(SCADA_ERR_ADDRCD);
    }
}

int  SaveDevStatToDB(HDB_SAVETASK  *hdbsavetaskp)
{
    FILE		*readfp;
    int			ret;

    Rdb_Malloc   sdiskmalloc(sizeof(DEVSTATSAVE) * hdbsavetaskp->rownums);
    DEVSTATSAVE *sdiskdata=(DEVSTATSAVE *)sdiskmalloc.GetAddr();

    readfp=fopen(hdbsavetaskp->savefilename,"rb");

    if (readfp==NULL) return -1;

    ret=fread(sdiskdata,sizeof(DEVSTATSAVE),hdbsavetaskp->rownums,readfp);

    fclose(readfp);

    if (ret!=hdbsavetaskp->rownums)return -1;
/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
	for(int i=0; i<hdbsavetaskp->rownums; i++)
    {
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->savedate);
        scdnetbyte(RDB_DATATYPE_SHORT,&sdiskdata->devtype);
        scdnetbyte(RDB_DATATYPE_SHORT,&sdiskdata->lastdevstatus);
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->runtime);
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->statistic[0]);
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->statistic[1]);
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->statistic[2]);
        scdnetbyte(RDB_DATATYPE_INT,&sdiskdata->statistic[3]);
        sdiskdata ++;
    }

    sdiskdata-=hdbsavetaskp->rownums;

#endif
*/

    BulkCopyIn(DEVSTAT_TABNAME, sdiskdata, hdbsavetaskp->rownums);
    return 0;
}

int  SaveSecHisDataToDB(HDB_SAVETASK  *hdbsavetaskp)
{
    CS_RETCODE      retcode;
    FUNC_STR        func;
    CS_DATAFMT      *datafmt=NULL;
    SECDATA_STRU        *sdiskdata;
    NetDbg		dbginfo;
    int             ret;
    FILE			*readfp;

    Rdb_Malloc     sdiskmalloc(sizeof(SECDATA_STRU) * hdbsavetaskp->rownums);
    sdiskdata=(SECDATA_STRU *)sdiskmalloc.GetAddr();

    readfp=fopen(hdbsavetaskp->savefilename,"rb");

    if (readfp==NULL) return -1;

    ret=fread(sdiskdata,sizeof(SECDATA_STRU),hdbsavetaskp->rownums,readfp);

    fclose(readfp);

    if (ret!=hdbsavetaskp->rownums)return -1;


    strcpy(func.dbname,"hisdb"); // 服务器名

    strcpy(func.table,hdbsavetaskp->tabname);

//	func.func=SDATE_BLKIN;
    func.func=BLK_IN;

    func.roxnum=hdbsavetaskp->rownums;

    SYS_CCLOCK  tmpcclock;

    SYS_CLOCK	tmpclock;

    tmpcclock.date=hdbsavetaskp->date;

    tmpcclock.time=0;

    ConvertClock(&tmpcclock,&tmpclock);

    func.his.year=tmpclock.year;

    func.his.month=tmpclock.month;

    func.his.date=tmpclock.day;

    func.his.time=sdiskdata[0].time;

    func.his.otime=sdiskdata[hdbsavetaskp->rownums-1].time;

/*#if(defined(__sun) && defined(__sparc) || defined(_AIX))
    scdnetbyte(RDB_DATATYPE_SHORT,&func.his.year);

    scdnetbyte(RDB_DATATYPE_SHORT,&func.his.month);

    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.date);

    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.time);

    scdnetbyte(RDB_DATATYPE_USHORT,&func.his.otime);

    for (int i=0; i<hdbsavetaskp->rownums; i++)
    {
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->date);
        scdnetbyte(RDB_DATATYPE_USHORT,&sdiskdata->time);
        scdnetbyte(RDB_DATATYPE_FLOAT,&sdiskdata->data);
        sdiskdata ++;
    }

    sdiskdata-=hdbsavetaskp->rownums;

#endif
*/

    retcode=dbapi.BlkCopyIn(&func,datafmt,(SECDATA_STRU *)sdiskdata,1);		// 插入定时存盘数据

    if (datafmt!=NULL)free(datafmt);		// 释放数据格式

    if (retcode==CS_SUCCEED)return(OK);
    else
    {
        dbginfo.PutDbg(NET_DBGTYPE_SCADA,"SCADA:\tSaveSecHisDataToDB函数BlkCopyIn插入记录失败!\t表名：%s\r\n",func.table);
        return(SCADA_ERR_ADDRCD);
    }
}


int SaveHDBData(HDB_SAVETASK  *hdbsavetaskp)
{
    int ret=-1;

    switch (hdbsavetaskp->savetype)
    {

    case 1:
        ret=SaveHisDataToDB(hdbsavetaskp);

        if (ret!=OK) RdbDebugMesg("\n 存数据入%s(date:%d time:%d)错误 ret=%d\n",
                                      hdbsavetaskp->tabname,hdbsavetaskp->date,hdbsavetaskp->time,ret);

        break;

    case 2:
        ret=SaveHisDayDataToDB(hdbsavetaskp);

        if (ret!=OK) RdbDebugMesg("\n 存数据入%s(date:%d)错误 ret=%d\n",
                                      hdbsavetaskp->tabname,hdbsavetaskp->date,ret);

        break;

    case 3:
        ret=SaveDevStatToDB(hdbsavetaskp);

        if (ret!=OK) RdbDebugMesg("\n SaveDevStatToDB执行错误 ret=%d\n",ret);

        break;

    case 4:
        ret=SaveSecHisDataToDB(hdbsavetaskp);

        break;

    default:
        break;
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////////////
///////////////////// 历史数据定时存盘  ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#ifdef __unix
void  *timersavehisdataproc(void *argp)
#else
UINT  timersavehisdataproc(LPVOID)
#endif
{
    static int pjcopyflag=0;    // 用于确保计划拷贝正确执行
    static SYS_CLOCK   lastpjreadclock;  // 用于确保读取计划值正确执行
    int   tmpmins,i;
    SYS_CLOCK   curclock;
    HDBTimer    hdbtimer(60);
    HDBSaveTaskMan  hdbstaskman;

    scdproject_restore();
    GetCurrentClock(&curclock);
    lastpjreadclock=curclock;
    scdproject_getcurdata(&curclock,1);

    scd_pthread_create(timer_proc_min_hisdata,NULL,100);

    int  seccurve_startflag=GetProgramRunPara("secuvstart",0);

    if (seccurve_startflag)
        scd_pthread_create(timer_proc_sec_hisdata,NULL,100);

    read_hisdb_diskusage();

    for (;;)
    {
        hdbstaskman.ProcAllHDBSaveTask();

        if (hdbtimer.TimerOK())
        {					// 1 minute timer
            GetCurrentClock(&curclock);

            //利用pjcopyflag防止计划拷贝多次

            if (curclock.hour==0 && curclock.minute<10)
            {
                if (pjcopyflag==0)
                {
                    scdproject_copy(&curclock,1);
                    pjcopyflag=1;
                }
            }
            else pjcopyflag=0;

            if (curclock.minute%10==7)
                read_hisdb_diskusage();

            //防止某分钟任务太多，分钟定时延迟超过一分钟，导致某分钟未读取计划值
            tmpmins=CalDecSecs(&lastpjreadclock,&curclock);

            tmpmins/=60;

            if (tmpmins>=2)
            {
                for (i=0;i<tmpmins-1;i++)
                {
                    IncDecSecs(&lastpjreadclock,60);
                    scdproject_getcurdata(&lastpjreadclock);
                }
            }

            scdproject_getcurdata(&curclock);

            lastpjreadclock=curclock;
        }

        sleepms(2,0);
    }

    return(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////
///////////////////// UNIX  ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#ifdef __unix

//int  started_sybase_flag=0;

void  *proc_timer_task(void *argp)
#else
UINT  proc_timer_task(LPVOID pParam)
#endif
{

    ProcMan  procman;

    while (1)
    {
        if (!procman.CheckParentProc())
        {
            printf("exit : CheckParentProc returning 0\n");
            exit(1);
            /*
            #ifdef __unix
                       	  exit(1);
            #else
            			  PostMessage(WM_CLOSE);
            #endif
            */
        }

        sleepms(2,0);
    }

    return(NULL);
}


/*
#ifdef __unix
int main()
{
	ProcMan  procman;
	procman.StartProc("scdsave","存盘处理进程","",NULL);

	if (!procman.CheckParentProc()) {
       	 printf("scdsave exit : CheckParentProc returning 0\n");
         exit(1);
	}

#ifdef __sun
	sigset(SIGUSR1,usr1_func);
	setpgid(getpid(),getpid());
#else
	sigset(SIGUSR1,SIG_IGN);
	setpgid(getpid(),getpid());
#endif

	NetDbg dbg;
	dbg.RegProc("存盘处理进程");

	scd_pthread_create(proc_timer_task,NULL,30);

	//启动历史数据库
    DBApi dbapi;
	int m;
	for(m=0;m<4;m++) {
		if(dbapi.Open(DB_OP_W,FALSE,TRUE)==CS_SUCCEED)break;
		sleepms(2,0);
	}
	if(m>=4)return 1;

	started_sybase_flag=1;
	timersavehisdataproc(NULL);
	dbapi.Close();
	started_sybase_flag=0;
	dbg.UnRegProc();
    return 1;
}
#endif
*/
