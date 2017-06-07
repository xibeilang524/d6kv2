#ifndef _RDBDEF_H
#define _RDBDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysdef.h"

#ifdef  __unix
#if defined(__alpha)	// compaq unix platform
#define  RDB_PLATFORM_ALPHAUNIX
#define	 RDB_PLATFORM_UNIX
typedef  long             hlong;			// 64 bits
typedef  unsigned long   uhlong;
#endif  // __alpha

#ifdef  _AIX			// ibm unix platform
#define  RDB_PLATFORM_AIXUNIX
#define	 RDB_PLATFORM_UNIX
#define  RDB_NET_ENCODE
#define  RDB_NET_BYTE_ORDER
typedef  long long             hlong;			// 64 bits
typedef  unsigned long long   uhlong;
#endif  // _AIX

#if (defined(__sun) && defined(__sparc))           // sun unix platform
#define  RDB_PLATFORM_SUNUNIX
#define  RDB_PLATFORM_UNIX
#define  RDB_NET_ENCODE
#define  RDB_NET_BYTE_ORDER
typedef  long long             hlong;			// 64 bits
typedef  unsigned long long   uhlong;
#endif  // __sunsparc

#if (defined(__sun) && defined(__i386))
#define  RDB_PLATFORM_SUNUNIX
#define  RDB_PLATFORM_UNIX
typedef  long long             hlong;			// 64 bits
typedef  unsigned long long   uhlong;
#endif // __sunx86

#ifdef __hpux           // HP-UX platform
#define  RDB_PLATFORM_HPUNIX
#define  RDB_PLATFORM_UNIX
#define  RDB_NET_ENCODE
#define  RDB_NET_BYTE_ORDER
typedef  long long             hlong;			// 64 bits
typedef  unsigned long long   uhlong;
#endif  // __hpux

#endif  // __unix

#ifdef __linux
#define  RDB_PLATFORM_LINUX
#define  RDB_PLATFORM_UNIX
typedef  long long             hlong;			// 64 bits
typedef  unsigned long long   uhlong;
#endif



#ifdef WIN32                // windows win32 platform
#define  RDB_PLATFORM_WIN32

typedef  __int64               hlong;
typedef  unsigned __int64     uhlong;

#endif


//typedef  int              hint;			// 32 bits

typedef  unsigned char    uchar;
typedef  unsigned short   ushort;
typedef  unsigned int     uint;
typedef  unsigned int     uhint;


#ifdef RDB_PLATFORM_WIN32

#include <time.h>

typedef  void *             rdbcalleridtype;

#define RDB_EVENT_MESG   (WM_USER+1753)

#else  // unix

#include <unistd.h>
#include <sys/time.h>

typedef  int              rdbcalleridtype;

#endif


#define  RDB_TABNAME_LEN            CODE_LEN   // The length of table simple name 
#define  RDB_TABFULLNAME_LEN        DESC_LEN   // The length of table full name

#define  RDB_COLNAME_LEN            CODE_LEN   // The length of column simple name
#define  RDB_COLFULLNAME_LEN        DESC_LEN   // The length of column full name

#define  RDB_USERNAME_LEN           CODE_LEN   // The length of user simple name
#define  RDB_USERFULLNAME_LEN       DESC_LEN   // The length of user full name

#define  RDB_HOSTNAME_LEN           CODE_LEN   // The length of user simple name


#define  RDB_PASSWD_LEN             PASSWORD_LEN   // The length of user's password

#define  RDB_OBJNAME_LEN			CODE_LEN


#define  RDB_KEYCOL_NUMS            6    // the number of column  in key

#define  RDB_QUICKQUERY_NORMAL		1
#define  RDB_QUICKQUERY_NAME		2
#define  RDB_QUICKQUERY_STR			3

//数据类型定义

#define RDB_DATATYPE_UCHAR          0     //无符号字符型
#define RDB_DATATYPE_CHAR           1     //有符号字符型
#define RDB_DATATYPE_USHORT         2     //无符号短整数型
#define RDB_DATATYPE_SHORT          3     //有符号短整数型
#define RDB_DATATYPE_UINT           4     //无符号整数型
#define RDB_DATATYPE_INT            5     //有符号整数型
#define RDB_DATATYPE_ULONG          6     //无符号长整数型
#define RDB_DATATYPE_LONG           7     //有符号长整数型(8 byte long)
#define RDB_DATATYPE_FLOAT          8     //单精度浮点型
#define RDB_DATATYPE_DOUBLE         9     //双精度浮点型
#define RDB_DATATYPE_STRING         10    //字符串型


#define RDB_DATATYPE_LASTDEF	RDB_DATATYPE_STRING	

#define  RDB_OK      0


#define  BorderAlign(off,alig)    ((((off)%(alig))==0) ? (off):((off)+(alig)-(off)%(alig)))

#define  RDB_MAX_STRVAL_LEN    256


#define RDB_DATA_FROM_NET_TO_HOST   0
#define RDB_DATA_FROM_HOST_TO_NET   1


/////////////////////////////////////////////////////
typedef union {
		 uchar   ucharval;
		 char	 charval;
		 ushort	 ushortval;
		 short	 shortval;
         uint    uintval;
         int     intval;
         uhlong  ulongval;
         hlong   longval;
         float   floatval;
         double  doubleval;
}  VTVALUE_UNION;


typedef struct {
    uchar           valuetype;
    short           stringlen;
	char			memalign[4];
    VTVALUE_UNION   value;
    char            stringval[RDB_MAX_STRVAL_LEN];
} RDB_VALUETYPE_STRU;




///////////////// Date and Time Process ////////////////////////
typedef struct {
      short  year;
      unsigned char  month;
      unsigned char  day;
} RDB_DATE_YMD_STRU;

typedef struct {
      unsigned char  hour;
      unsigned char  minute;
      unsigned char  second;
} RDB_TIME_HMS_STRU;

typedef struct {
      unsigned  short   year;
      unsigned char    month;
      unsigned char    day;
      unsigned char    wday;
      unsigned char    hour;
      unsigned char    minute;
      unsigned char    second;
      unsigned  short   msecond;
} RDB_DATETIME_MS_STRU;


typedef  int   intertime;




#define  RDBRUNSTATE_SCADATRAIN     1   // 进入scada培训态


#define  RDBSERVNAME_SCDPAS        "SCADA+PAS"
#define  RDBSERVNAME_PASSCD        "PAS+SCADA"
#define  RDBSERVNAME_OPTSCD        "OPT+SCADA"
#define  RDBSERVNAME_SCADA         "SCADA"
#define  RDBSERVNAME_PAS           "PAS"
#define  RDBSERVNAME_OPTICKET      "OPTICKET"
#define  RDBSERVNAME_ALLHOST       "ALLHOST"
#define  RDBSERVNAME_SCDTRAIN      "TRAIN"


inline int is_same_rdbserver(char *tabservname,char *netservname)
{
	if(strcmp(tabservname,netservname)==0)return(1);
	else if(strcmp(tabservname,RDBSERVNAME_SCDPAS)==0 
				|| strcmp(tabservname,RDBSERVNAME_PASSCD)==0) {
		if(strcmp(netservname,RDBSERVNAME_SCADA)==0 
			|| strcmp(netservname,RDBSERVNAME_PAS)==0)return(1);
		else return(0);
	}
	else if(strcmp(tabservname,RDBSERVNAME_OPTSCD)==0) {
		if(strcmp(netservname,RDBSERVNAME_SCADA)==0 
			|| strcmp(netservname,RDBSERVNAME_OPTICKET)==0)return(1);
		else return(0);
	}
	else if(strcmp(tabservname,RDBSERVNAME_ALLHOST)==0)return(1);

	return(0);
}


inline void get_default_rdbserver(char *servname,char *defaultservname)
{
	if(strcmp(servname,RDBSERVNAME_SCDPAS)==0)
		strcpy(defaultservname,RDBSERVNAME_SCADA);
	else if(strcmp(servname,RDBSERVNAME_PASSCD)==0)
		strcpy(defaultservname,RDBSERVNAME_PAS);
	else if(strcmp(servname,RDBSERVNAME_OPTSCD)==0)
		strcpy(defaultservname,RDBSERVNAME_OPTICKET);
	else if(strcmp(servname,RDBSERVNAME_ALLHOST)==0)
		strcpy(defaultservname,RDBSERVNAME_SCADA);
	else 
		strcpy(defaultservname,servname);
}




/////////////////////////////////////////////
#define  RDB_EVENT_MSGLEN           128

#define  RDB_EVENT_NULL             0
#define  RDB_EVENT_ALL              1

#define  RDB_EVENT_UPDCOL			20
#define  RDB_EVENT_LOADTAB			21
#define  RDB_EVENT_BKUPTAB			22
#define  RDB_EVENT_UPDRCD			23
#define  RDB_EVENT_SQL_UPD			24
#define  RDB_EVENT_SQL_DEL			25
#define  RDB_EVENT_SQL_INS			26
#define  RDB_EVENT_LOADMODE			27

typedef struct {
	char  tabname[RDB_TABNAME_LEN];
}  RDB_EVENT_BKUPTAB_STRU;

typedef struct {
	char  tabname[RDB_TABNAME_LEN];
	int   rcdno;
}   RDB_EVENT_BKUPRCD_STRU;


typedef struct {
	char  hostname[RDB_HOSTNAME_LEN];
	char  username[RDB_USERNAME_LEN];
	char  tabname[RDB_TABNAME_LEN];
}  RDB_EVENT_LOADTAB_STRU;


typedef struct {
	char  hostname[RDB_HOSTNAME_LEN];
	char  username[RDB_USERNAME_LEN];
}  RDB_EVENT_LOADMODE_STRU;


typedef struct {
	char  hostname[RDB_HOSTNAME_LEN];
	char  username[RDB_USERNAME_LEN];
	char  tabname[RDB_TABNAME_LEN];
	char  SqlCmd[RDB_EVENT_MSGLEN-50];
}  RDB_EVENT_UPD_STRU;

typedef struct {
	char  hostname[RDB_HOSTNAME_LEN];
	char  username[RDB_USERNAME_LEN];
	char  tabname[RDB_TABNAME_LEN];
	char  objectname[RDB_OBJNAME_LEN];
	char  columnname[RDB_COLNAME_LEN];
	uchar  datatype;
	union {
		VTVALUE_UNION   updvalue;
		char			updstring;
	} update_value;
}  RDB_EVENT_UPDCOL_STRU;

typedef struct {
        int       eventcode;
        RDB_DATETIME_MS_STRU eventclock;
        short     eventmsglen;
		char	  username[RDB_USERNAME_LEN];
        char      eventmsg[RDB_EVENT_MSGLEN];
} RDB_EVENT_STRU;


////////////////////////////////////////////////////////////////////////////////////
//#define  MAXCONDCOUNT       6

#define  LOGIC_OPER_AND                 0        //与
#define  LOGIC_OPER_OR                  1        //或

#define  RELATION_OPER_EQU              0        //等于
#define  RELATION_OPER_MORETHAN         1        //大于
#define  RELATION_OPER_LESSTHAN         2        //小于
#define  RELATION_OPER_MOREOREQU        3        //大于等于
#define  RELATION_OPER_LESSOREQU        4        //小于等于
#define  RELATION_OPER_NOTEQU           5        //不等于


typedef struct {
     uchar           logicalop;
     uchar           relationop;
	 char			 memalign[4];
     char            name[RDB_COLNAME_LEN];
     RDB_VALUETYPE_STRU   conditionval;
} RDB_COND_STRU;



typedef  struct {
	char  lib_name[RDB_TABNAME_LEN];
	char  name[RDB_TABNAME_LEN];			   // 表代码
	char  full_name[RDB_TABFULLNAME_LEN];      // 表描述
	int   tableid;                             // 表标识 
	int   rcdcount;						       // 记录个数
	int   recordlen;						   // 记录长度（字节) 
	int   columnnums;                          //表的列数
}  RDB_BTBL_TAB_STRU; // *******表信息******** //

typedef struct  {
	char   name[RDB_COLNAME_LEN];		    // 列代码
	char   full_name[RDB_COLFULLNAME_LEN];	// 列描述
	int    offset;
	ushort unitlen;						    // 数据长度
	uchar  datatype;					    // 数据类型
	uchar  position;
	uchar  bitnums;
	uchar  keyflag;
}  RDB_BTBL_COL_STRU; // *******列信息********  //



/////////////////////////////////////////////////////////////
typedef  struct {
	char			fldname[RDB_COLFULLNAME_LEN];
	int				retcode;
	RDB_VALUETYPE_STRU	fieldvalue;
}  RDB_FLDNAME_VAL_STRU;
	



typedef  struct {
	 int    groupno;
	 char   tabname[RDB_TABNAME_LEN];
     char   objname[RDB_OBJNAME_LEN];
     char   fldname[RDB_COLNAME_LEN];
} RDB_FIELD_STRU;

typedef  struct {
	 int    orderno;
	 int    groupno;
     char   tabname[RDB_TABNAME_LEN];
     char   objname[RDB_OBJNAME_LEN];
     char   fldname[RDB_COLNAME_LEN];
} RDB_FIELD_SRV_STRU;



//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/////////////// //////////rdb error code define //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
#define  RDB_ERR_BASE           -300

#define  RDB_ERR_NOTAB			(RDB_ERR_BASE-1)
#define  RDB_ERR_NOCOL		    (RDB_ERR_BASE-2)
#define  RDB_ERR_NORCD		    (RDB_ERR_BASE-3)
#define  RDB_ERR_NORIGHT		(RDB_ERR_BASE-4)
#define  RDB_ERR_MALLOC		    (RDB_ERR_BASE-5)
#define  RDB_ERR_ADDTAB		    (RDB_ERR_BASE-6)
#define  RDB_ERR_ADDUSER		(RDB_ERR_BASE-7)
#define  RDB_ERR_NOUSER			(RDB_ERR_BASE-8)
#define  RDB_ERR_OPENTAB		(RDB_ERR_BASE-9)
#define  RDB_ERR_ADDRCD			(RDB_ERR_BASE-10)
#define  RDB_ERR_DELRCD			(RDB_ERR_BASE-11)
#define  RDB_ERR_LOGIN			(RDB_ERR_BASE-12)
#define  RDB_ERR_SQLWHR		    (RDB_ERR_BASE-13)
#define  RDB_ERR_NET			(RDB_ERR_BASE-14)
#define  RDB_ERR_BKUP			(RDB_ERR_BASE-15)
#define  RDB_ERR_COLMATCH		(RDB_ERR_BASE-16)
#define  RDB_ERR_HISSQL		    (RDB_ERR_BASE-17)
#define  RDB_ERR_EVENT			(RDB_ERR_BASE-18)
#define  RDB_ERR_PARA			(RDB_ERR_BASE-19)
#define  RDB_ERR_TABEXIST		(RDB_ERR_BASE-20)
#define  RDB_ERR_DATATYPE		(RDB_ERR_BASE-21)
#define  RDB_ERR_NETDATA		(RDB_ERR_BASE-22)
#define  RDB_ERR_BPTREE		    (RDB_ERR_BASE-23)
#define  RDB_ERR_HISBULKIN		(RDB_ERR_BASE-24)
#define  RDB_ERR_VERSION 		(RDB_ERR_BASE-25)
#define  RDB_ERR_CMD  		    (RDB_ERR_BASE-26)
#define  RDB_ERR_PASSWD 		(RDB_ERR_BASE-27)
#define  RDB_ERR_NAMERULE		(RDB_ERR_BASE-28)
#define  RDB_ERR_NOTPOWDEV		(RDB_ERR_BASE-29)
#define  RDB_ERR_BPTFAULT		(RDB_ERR_BASE-30)

/////////////////////////////////////////////////////////////////

#endif


