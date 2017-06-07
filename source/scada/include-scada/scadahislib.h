#ifndef  _SCADA_HISLIB_H
#define  _SCADA_HISLIB_H

#include "db/dbapi.h"
#include "rdb/rdbapi.h"
#include "scd/saveflag.h"
#include "scadatime.h"
#include "scadaproc.h"


typedef struct 
{
    int   savedate; 
    char  name[OBJECTNAME_LEN];
	short devtype; //设备类型
    short lastdevstatus;
    int   runtime;
    int   statistic[4];
} DEVSTATSAVE;


#if defined(WIN32)
_SCD_DLLPORT 
#endif
int  BulkCopyIn(char *tabname,void *rcdbuff,int  rcdnums);


#define  HTMLOADFLAG_ALWAYSLOAD        1
#define  HTMLOADFLAG_FIRSTLOAD         2

#define  HTMLOADMODE_REQLOAD        0
#define  HTMLOADMODE_FIRSTLOAD      1



//////////////////// 历史数据操作类 ///////////////////////////////

#define  HISDATA_TABTYPE_MINUTE            0
#define  HISDATA_TABTYPE_DAY               1
#define  HISDATA_TABTYPE_MONTH             2
#define  HISDATA_TABTYPE_YEAR              3


class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
HisdataOper {
public:
	int				curpulsef;
	int				curdatanums;
	HIS_DATA       *curhisdata;

	HisdataOper(void);
	~HisdataOper(void);
	int readdata(int pulsef,int tabtype,SYS_CLOCK *clockp,char *objname,int flag);
	int binddata(int rcdno,int *time,double *data,int *flag);
	int bindodata(int rcdno,int *odate,int *otime,int *oflag);
	int upddata(int pulsef,int tabtype,SYS_CLOCK *clockp,char *objname,double data,int flag,int odate,int otime,int oflag);
	int deldata(int pulsef,int tabtype,SYS_CLOCK *clockp,char *objname,int flag);
	int insertdata(int pulsef,int tabtype,SYS_CLOCK *clockp,char *objname,double data,short rawdata,int flag,int odate,int otime,int oflag);
	int readmindata(int pulsef,SYS_CLOCK *clockp,char *objname,int flag,double *data,int *dataflag=NULL);
};


///////////////   历史库计划处理  /////////////

typedef struct {
   short  time;
   uchar  flag;
   float  data;
 } SCD_PJDATA;


int 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
ConvertProjectData(SCD_PJDATA *srcpjdata,int srcnums,SCD_PJDATA *objpjdata,int objpjtime);
  
class 
#if defined(WIN32)
_SCD_DLLPORT 
#endif
Hisproject {
    Rdb_PreMalloc   pjmalloc;
 public:
    Hisproject(void);
    int  read_pjdata_day(SYS_CLOCK  *clockp,char *objname,SCD_PJDATA **pjdata,int saveflag=HISDATA_PLAN);
    int  read_pjdata_minute(SYS_CLOCK  *clockp,char *objname,float *pjdata,int saveflag=HISDATA_PLAN);
    int  malloc_pjdata(SYS_CLOCK  *clockp,char *objname,int savetime,int saveflag=HISDATA_PLAN);
    int  dump_pjdata(SYS_CLOCK  *clockp,char *objname,SCD_PJDATA *pjdat,int nums,int saveflag=HISDATA_PLAN);
  };
#endif
