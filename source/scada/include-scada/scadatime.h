#ifndef _SCADA_TIME_H
#define _SCADA_TIME_H

#include <stdlib.h>
#include "scd/cmddef.h"


typedef struct {
      short  year;
      unsigned char  month;
      unsigned char  day;
    } SYS_DATE;

typedef struct {
      unsigned char  hour;
      unsigned char  minute;
      unsigned char  second;
    } SYS_HMS;

typedef struct {
      unsigned  short  year;
      unsigned char    month;
      unsigned char    day;
      unsigned char    wday;
      unsigned char    hour;
      unsigned char    minute;
      unsigned char    second;
      unsigned  short  msecond;
    } SYS_CLOCK;


#define  CCCLOCK_RESOLUTE      60
#define  DAYS_1900TO1970       25567


typedef struct {
      unsigned  short date;     // from 1970.1.1 
      unsigned  short time;     // from midnight
    } SYS_CCLOCK;


//typedef  int   intertime;

#ifdef WIN32
_SCD_DLLPORT 
#endif
void GetInterTime(intertime  *intertimep,unsigned  short  *msecp=NULL);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void ConvertTime(intertime intime,SYS_CLOCK  *clockp);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void ConvertTime(intertime intime,SYS_CCLOCK  *cclockp);


#ifdef WIN32
_SCD_DLLPORT 
#endif
void ConvertClock(SYS_CCLOCK  *ccclockp,SYS_CLOCK  *clockp);


#ifdef WIN32
_SCD_DLLPORT 
#endif
void ConvertClock(SYS_CLOCK  *clockp,intertime *intimep);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void ConvertClock(SYS_CLOCK  *clockp,SYS_CCLOCK  *cclockp);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void GetCurrentClock(SYS_CLOCK  *curclockp);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void GetCurrentClock(SYS_CCLOCK *curcclockp);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void GetCurrentClock(SYS_DATE   *curdatep);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void GetCurrentClock(SYS_HMS   *curtimep);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int  GetWeekday(int year,int month,int day);


#ifdef WIN32
_SCD_DLLPORT 
#endif
int  GetYearday(int year,int month,int day);


#ifdef WIN32
_SCD_DLLPORT 
#endif
int  sleepms(int  msecs);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int  sleepms(int  secs,int msecs);

#ifdef WIN32
_SCD_DLLPORT 
#endif
inline int  CalDecSecs(intertime starttime,intertime endtime)  {return(endtime-starttime);}

#ifdef WIN32
_SCD_DLLPORT 
#endif
int  CalDecSecs(SYS_CLOCK  *startclock,SYS_CLOCK  *endclock);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int  CalDecSecs(SYS_CCLOCK *startclock,SYS_CCLOCK *endclock);

#ifdef WIN32
_SCD_DLLPORT 
#endif
int  CalDecSecs(SYS_HMS   *starttime,SYS_HMS  *endtime);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void IncDecSecs(SYS_CLOCK   *clockp,int  secs);

#ifdef WIN32
_SCD_DLLPORT 
#endif
void IncDecDays(SYS_CLOCK   *clockp,int  days);


#ifdef WIN32
_SCD_DLLPORT 
#endif
void IncDecDays(SYS_DATE    *clockp,int  days);


#ifdef WIN32
_SCD_DLLPORT 
#endif
void IncDecDays(SYS_CCLOCK  *clockp,int  days);


#ifdef WIN32
_SCD_DLLPORT 
#endif
void  DispTime(char *printstr=NULL);

// end of define by Xuehua Hu

#endif
