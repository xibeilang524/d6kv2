#ifndef _RDBAPI_H
#define _RDBAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "rdbdef.h"
#include "db/dbapi.h"

#ifdef  RDB_NET_ENCODE
#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else
void
#endif
RdbNetbyte(int type,void *addr,int nhf);

#else

#define  RdbNetbyte(type,addr,nhf)

#endif


/**************内存管理****************/

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Malloc {
	void *buff;
public:
	Rdb_Malloc(int nbyte) {
		buff=malloc(nbyte);
	}
	~Rdb_Malloc(void) {
		free(buff);
	}
	void *GetAddr(void) {return(buff);}
	operator void *() {return(buff);}
};




////////////////////
#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
DatatypeLen(int datatype);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
DatatypeAlign(int datatype,int off);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
RdbDataCompare(int datatype,void *data1,void *data2);


#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_SwapinVal(VTVALUE_UNION  *vtvalu,uchar  valtype,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_SwapoutVal(VTVALUE_UNION *vtvalu,uchar  valtype,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_SwapoutExpVal(VTVALUE_UNION *vtvalu,uchar expvaltype,uchar valtype,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_SwapinDecVal(VTVALUE_UNION *vtvalu,uchar  valtype,char *decval);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_SwapoutDecVal(VTVALUE_UNION *vtvalu,uchar  valtype,char *decval,int decstrlen,int dotnums=4);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_DispVal(VTVALUE_UNION *vtvalu,uchar  valtype);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_Union_CompVal(uchar  valtype1,VTVALUE_UNION *vtvalu1,
                  uchar  valtype2,VTVALUE_UNION *vtvalu2);    



#ifdef RDB_PLATFORM_WIN32
void	 __declspec(dllexport)
#else
void 
#endif
Valtype_DispVal(RDB_VALUETYPE_STRU *vtval);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_ValInfo(RDB_VALUETYPE_STRU *vtval,uchar  *vtype);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_SwapinVal(RDB_VALUETYPE_STRU *vtval,uchar  valtype,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_SwapinDecVal(RDB_VALUETYPE_STRU *vtval,uchar  valtype,char *decval);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_SwapoutDecVal(RDB_VALUETYPE_STRU *vtvalu,char *decval,int decstrlen,int dotnums=4);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_SwapoutVal(RDB_VALUETYPE_STRU *vtval,uchar  *valtype,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_SwapoutExpVal(RDB_VALUETYPE_STRU *vtval,uchar valtype,int slen,void *valaddr);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
Valtype_CmpVal(RDB_VALUETYPE_STRU *vtval1,RDB_VALUETYPE_STRU  *vtval2);    


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Vartype_value  {
    uchar           valuetype;
    VTVALUE_UNION   value;
    short           stringlen;
    char            *stringval;
	void			MallocString(int inlen);
  public:
	   Rdb_Vartype_value(void) {stringval=NULL;}
	  ~Rdb_Vartype_value(void) {if(stringval!=NULL)free(stringval);}
   int  valueinfo(uchar  *vtype)  {
          *vtype=valuetype;
          if(valuetype!=RDB_DATATYPE_STRING)return(DatatypeLen(valuetype));
          else return(stringlen);
          }
   int  RdbSwapinVal(uchar  valtype,void *valaddr);
   int  RdbSwapinDecVal(uchar  valtype,char *decval);
   int  RdbSwapoutDecVal(char *decval,int decstrlen,int dotnums=4);
   int  RdbSwapoutVal(uchar  *valtype,void *valaddr);
   int  RdbSwapoutExpVal(uchar valtype,int slen,void *valaddr);
   int  RdbSwapVal(VTVALUE_UNION *vtvalu);
   char *RdbGetString(void);
   void RdbDispVal(void);
#ifdef RDB_PLATFORM_WIN32
	friend int __declspec(dllexport)
#else
	friend int
#endif
	vtcl_cmpvalue(Rdb_Vartype_value *vtval1,Rdb_Vartype_value *vtval2);    
#ifdef RDB_PLATFORM_WIN32
	friend int __declspec(dllexport)
#else
	friend int
#endif
	Valuetype_SwapVal(Rdb_Vartype_value *srcvtval,Rdb_Vartype_value *objvtval);    
#ifdef RDB_PLATFORM_WIN32
	friend int __declspec(dllexport)
#else
	friend int
#endif
	Valuetype_SwapVal(Rdb_Vartype_value *vtvalcl,RDB_VALUETYPE_STRU  *vtvalstru);
#ifdef RDB_PLATFORM_WIN32
	friend int __declspec(dllexport)
#else
	friend int
#endif
	Valuetype_SwapVal(RDB_VALUETYPE_STRU  *vtvalstru,Rdb_Vartype_value *vtvalcl);
  };



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_PreMalloc {
  int  unitsize;
  int  maxnums;
  int  curnums;
  int  pernums;
  int  setzerof;
  void *memaddr;
public :
  Rdb_PreMalloc(int pn,int usize,int szerof=0);
  Rdb_PreMalloc(void);
  ~Rdb_PreMalloc();
  void    set_mallocpara(int pn,int usize,int szerof=0);
  void   *get_nextunitaddr(void);
  void   *get_multunitaddr(int un);
  void   *get_mallocinfo(int *nums=NULL) {
            if(nums!=NULL)*nums=curnums;
            return(memaddr);
            };
  void    freespace(void) {
            if(memaddr!=NULL)free(memaddr);
            memaddr=NULL;
            curnums=maxnums=0;
            };
  };




#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_MultiTypeSave {
    int				valuenums;
	Rdb_Vartype_value  *storevtval;
	uchar          *availflag;
  public:
    Rdb_MultiTypeSave(void) {
		storevtval=NULL;
		availflag=NULL;
		valuenums=0;
	}
    Rdb_MultiTypeSave(int nums) {
		storevtval=NULL;
		availflag=NULL;
		valuenums=0;
		SetDataNums(nums);
	}
   ~Rdb_MultiTypeSave(void);
   void SetDataNums(int nums);
	int    RdbGetValNums(void) {
		return(valuenums);
	}
	int     IsAvailable(int valno);
	int		RdbGetValType(int valno,Rdb_Vartype_value *val);
	int		RdbSetValType(int valno,Rdb_Vartype_value *val);
    int     RdbGetValInfo(int valno,uchar *type,int  *len=NULL);
    int		RdbGetVal(int valno,uchar *type,void *valaddr);
	int     RdbSetVal(int valno,uchar type,void *valaddr);
    int     RdbGetVal_int(int valno);
  float     RdbGetVal_float(int valno);
  double    RdbGetVal_double(int valno);
   hlong    RdbGetVal_long(int valno);
  uhlong    RdbGetVal_ulong(int valno);
    char   *RdbGetString(int valno);
  };



///////////////// Date and Time Process ////////////////////////

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
RdbSleepms(int  msecs);

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
RdbSleepms(int  secs,int msecs);



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Time {
public:
	void RdbGetIntertime(intertime  *intertimep,unsigned  short  *msecp=NULL);
	void RdbConvertTime(intertime intime,RDB_DATETIME_MS_STRU  *clockp);
	void RdbConvertClock(RDB_DATETIME_MS_STRU  *clockp,intertime *intimep);
	void RdbGetCurClock(RDB_DATETIME_MS_STRU  *curclockp);
	void RdbGetCurClock(RDB_DATE_YMD_STRU   *curdatep);
	void RdbGetCurClock(RDB_TIME_HMS_STRU   *curtimep);
	int  RdbGetWeekday(int year,int month,int day);
	int  RdbCalDecSecs(intertime starttime,intertime endtime)  {
                return(endtime-starttime);
	}
	int  RdbCalDecSecs(RDB_DATETIME_MS_STRU  *startclock,RDB_DATETIME_MS_STRU  *endclock);
	int  RdbCalDecSecs(RDB_TIME_HMS_STRU   *starttime,RDB_TIME_HMS_STRU  *endtime);
	void RdbIncDecSecs(RDB_DATETIME_MS_STRU   *clockp,int  secs);
	void RdbIncDecDays(RDB_DATETIME_MS_STRU   *clockp,int  days);
	void RdbIncDecDays(RDB_DATE_YMD_STRU    *clockp,int  days);
	void RdbDispTime(char *printstr=NULL);
};

////////////////////////////////////////

#define  RDB_BPTREE_ORDER       32

#define  RDB_BPTNODE_BASE      2
#define  RDB_BPTNODE_TOP      1
#define  RDB_BPTNODE_BRANCH    0

#define  RDB_BPTNODE_CHGMAX     1
#define  RDB_BPTNODE_CHGOVER    2
#define  RDB_BPTNODE_CHGSPLIT   3


#define  BptKeyAlign(off,alig)    (((off)%(alig))==0 ? (off):(off)+(alig)-(off)%(alig))

// The B+ tree created by the program is composed of control area 
// and node area. a node record is composed of pointer to parent
// tree,pointer to child tree  and key info. 
// The node record  format is below:
//
// plink   chlink1 link2 ... linkn childno nodetype  key1 key2 .... keyn

// RDB_BPTREE_CTRL_STRU : declare the structure of  control area 
//		 about B+ tree.

typedef struct  {
   int   maxnodenums;      // the maximum of node record.
   int   noderoot;         // point to the root of the B+ tree's node.
   int   layernums;        // the layer number of the B+ tree
   int   freenodelink;     // point to free node record
   int   nextnodelink;     // point to unmalloced node record
   int   keyinfolen;       // the byte number of a key
   int   keyoff[RDB_BPTREE_ORDER];     // the  offset of key in a record
   int   nodercdlen;
   ushort  lockcount;
   uchar   lock_flag;
  } RDB_BPTREE_CTRL_STRU;


// RDB_BPTREE_LINK_STRU : declare the structure of  node area 
//		         about B+ tree.

typedef struct  {
	 int    plink;   // parent link
     int    chlink[RDB_BPTREE_ORDER];
	 unsigned char  keynums;
     char   nodetype;
  }  RDB_BPTREE_LINK_STRU;


// function: get infomation of node record about B+ tree
// input: key len ,key align len,the pointer to key offset
// return: node record length

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
BptreeTotalNode(int maxrcdnums);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
BptreeTotalMemLen(int maxrcdnums,int keyinfolen,int keyalign);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
BptreeNodeStructInfo(int keyinfolen,int keyalign,int *keyoff=NULL);




#define  BNODEADDR(a)  (bptnode+(a) * bptctrl->nodercdlen)
#define  KeyCopy(o,s)  memcpy(o,s,bptctrl->keyinfolen)
#define  KeyAddr(a,b)  ((char *)(a)+bptctrl->keyoff[(b)])
#define  KeyMove(n,ol,sl)	{\
							KeyCopy(KeyAddr(n,ol),KeyAddr(n,sl)); \
							(n)->chlink[ol]=(n)->chlink[sl];\
						}
#define  TwinKeyMove(on,ol,sn,sl) {\
							KeyCopy(KeyAddr(on,ol),KeyAddr(sn,sl)); \
							(on)->chlink[ol]=(sn)->chlink[sl];\
						}

// the class of  B+ tree 
#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Bptree {
    RDB_BPTREE_CTRL_STRU  *bptctrl;    // address of control area
    char  *bptnode;     // address of node area
	int   RdbSplitInsertNode(RDB_BPTREE_LINK_STRU *insnodep,void *keydp,int datano,int *overf,void *overkey1,void *overkey2,int *overnodeno2);
	int   RdbDirectInsertNode(RDB_BPTREE_LINK_STRU *insnodep,void *keydp,int datano,int *maxflag,void  *maxkey);
	int   RdbProcInsert(void *keydp,int nodeno,RDB_BPTREE_LINK_STRU *insnodep,int *overf,void *overkey1,int *overnodeno1,void *overkey2,int *overnodeno2);
	int   RdbMallocFreeNode(void);
	int   RdbMallocNodeRecord(int nodetype);   // malloc one record from node area
	void  RdbInitNodeInfo(RDB_BPTREE_LINK_STRU *bnp,int nodetype);
	void  RdbLockBptreeNoAccess(void);
	void  RdbLockBptreeStructWithRW(void);
	void  RdbUnlockBptree(void);
  public:
   // constructor
	void  RdbBindInfo(void *addr);
	// Initialize whole B+ tree
    void  RdbInitBptree(int maxnodenums,int keylen,int keyalign);
	// empty all node node of B+ tree
    void  RdbEmptyBptree(void);
	// the function of comparing key
    virtual int   RdbCompareKey(void *keydp1,void *keydp2);
	// the find function
    int   RdbBptreeFind(void *keydp);
    int   RdbBptreeInsert(void *keydp,int datano);
    };




/////////////////////////////////////

class Rdb_Shm {
public:
	int   RdbCheckShm(int shmkey);
	int   RdbCreateShm(int  shmkey,int  shmlen);
	void *RdbMappingShm(int shmkey,int  shmlen);
	int   RdbUnmapShm(void *shmaddr);
	int   RdbDelShm(int shmkey);
	void *RdbCreateAndMapping(int shmkey,int  shmlen,int *createf=NULL);
};

////////////////////////////////////////////////
#define  RDB_LINE_MAXWORDLEN	1024

#define  RDB_LINE_STRTYPE		    0
#define  RDB_LINE_NAMETYPE		    1
#define  RDB_LINE_IDIGITTYPE		2
#define  RDB_LINE_FDIGITTYPE		3
#define  RDB_LINE_SYMBOLTYPE		4

#define  RDB_LINE_EOLTYPE			254
#define  RDB_LINE_ERR				255


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_LineAnalysis {
	int  linepoint;
	int  lastlinepoint;
	char *linep;
public:
	char word[RDB_LINE_MAXWORDLEN];
	Rdb_LineAnalysis(void) {
		linep=NULL;linepoint=0;lastlinepoint=0;
	}
	Rdb_LineAnalysis(char *lp) {
		InLine(lp);
	}
	void InLine(char *lp) {
		linep=lp;linepoint=0;lastlinepoint=0;
	}
	int GetWord(void);
	int UngetWord(void);
};

///////////////////////////////////////////////

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int
#endif
RdbGetFileLen(char *filename);

#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else 
void
#endif
RdbDebugMesg(char *fmt,...);

#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else 
void
#endif
RdbPrintErr(int errcode,char *errdesc);//取错误描述

#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else 
void
#endif
RdbPrintErr(char *promptaddr,int errcode);//显示错误

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickSort{
	char  *rcdbuf;
	int rcdlen;
	int rcdnums;
	void  *tempbuf;
	int ProcQuickSort(int lp,int rp);
public:
	int  descentf;
	void InitQuickSortSrch(void *buf,int rlen,int nums,int descf=0) 
	{
		descentf=descf;
		rcdbuf=(char *)buf;
		rcdlen=rlen;
		rcdnums=nums;
		tempbuf=NULL;
	}
	// comparing function which compare between keys
	virtual  int QSSCompareKey(void *rcd1,void *rcd2);
	// quick sort function
	int QuickSort(void);
	// binary search function
	int SearchBin(void *keydp);
};


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickSortSrch : public  Rdb_QuickSort {
	int key_cmp_type;
	int key_col_nums;
	int key_col_offset[15];
	unsigned char key_col_datatype[15];
public:
	Rdb_QuickSortSrch(void) {
		InitQuickSortSrch(NULL,0,0,0);
	}
	Rdb_QuickSortSrch(void *buf,int rlen,int nums,int descf=0) {
		InitQuickSortSrch(buf,rlen,nums,descf);
	}
	void QSSInitCompare(int col_nums,int *offset,unsigned char *datatype);
	virtual  int QSSCompareKey(void *rcd1,void *rcd2);
};





/////////////////// User Manager //////////////////////////////////////////////////

typedef void *  HUSER;


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Userlogin {
public:
	HUSER   RdbLogin(char *username,char *password);
	void	RdbLogout(HUSER  huser);
	void	RdbGetGrpName(HUSER huser,char *group_name);
	int     RdbGetUserInfo(char *name,char *full_name=NULL,char *group_name=NULL,uchar *usertype=NULL);
};




//////////////////////////////////////////////////////////////////////////////////

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_EventWithUser {
	HUSER huser;
	int   caller_no;
	hlong  event_message[RDB_EVENT_MSGLEN/sizeof(hlong)+1];
public:
	Rdb_EventWithUser(void);
	Rdb_EventWithUser(char *username,char *password);
	~Rdb_EventWithUser(void);
	int   Register(rdbcalleridtype  callerid,int noticef=1,char *username=NULL,char *password=NULL);//注册
	void  RdbUnReg(void);//注销
	void  RdbNotifyEventType(int eventtype);//设置需通知的事件类型
	int   RdbGetEvent(char *sender,RDB_DATETIME_MS_STRU *evttime,void **mesg,int *length=NULL);//获取事件
};//本地事件

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_EventNoUser {
	void  *sysctrl;
	hlong event_message[RDB_EVENT_MSGLEN/sizeof(hlong)+1];
public:
	Rdb_EventNoUser(void);
	~Rdb_EventNoUser(void);
	int RdbGetEvent(char *sender,RDB_DATETIME_MS_STRU *evttime,void **mesg,int *length=NULL);//获取事件
};


#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else
void
#endif
RdbPutEvent(char *username,int event_code,void *mesg,int len);


////////////////// process  base table  ///////////////////////

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Column {
	int curdataoff,maxtypelen;
	int curposition,lastcolumntype;
public:
	Rdb_Column(void) {
		Initializtion();
	};
	void Initializtion(void) {
		curdataoff=0;maxtypelen=1;
		curposition=0;lastcolumntype=0;
	}
	int AddColumn(int datatype,int unitlen,int bitnums,int *offset,uchar *position);
	int TotalLength(void);
	int Border(void) {return(maxtypelen);}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Rdb_MallocColumn {
	RDB_BTBL_COL_STRU *column;
public:
	Rdb_MallocColumn(int colnums) {
		if(colnums>0)
			column=(RDB_BTBL_COL_STRU *)malloc(colnums * sizeof(RDB_BTBL_COL_STRU));
		else 
			column=NULL;
	}
	~Rdb_MallocColumn(void) {
		if(column)free(column);
	}
	RDB_BTBL_COL_STRU  &operator[](int n) {return(column[n]);}
};


/*****************************************/
/*****************************************/
/*************本地实时库接口**************/
/*****************************************/
/*****************************************/

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
CalRcdInfo(RDB_BTBL_TAB_STRU *tp,RDB_BTBL_COL_STRU *fp);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
CalRcdInfo(int colnums,RDB_BTBL_COL_STRU *fp);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
CalRcdLen(int colnums,RDB_BTBL_COL_STRU *fp);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
GetColInfoVal(char *rcdaddr,RDB_BTBL_COL_STRU *colinfop,Rdb_Vartype_value *gvtval);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
GetColInfoVal(char *rcdaddr,RDB_BTBL_COL_STRU *colinfop,RDB_VALUETYPE_STRU *gvtval);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
SetColInfoVal(char *rcdaddr,RDB_BTBL_COL_STRU *colinfop,Rdb_Vartype_value *pvtval);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
SetColInfoVal(char *rcdaddr,RDB_BTBL_COL_STRU *colinfop,RDB_VALUETYPE_STRU *pvtval);

#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
RecordCopy(int colnums,RDB_BTBL_COL_STRU *srccolp,char *srcrcdaddr,
                RDB_BTBL_COL_STRU *descolp,char *desrcdaddr);



#ifdef RDB_PLATFORM_WIN32
int    __declspec(dllexport)
#else
int   
#endif
CheckColMatch(int colnums,RDB_BTBL_COL_STRU *srccolp,
						RDB_BTBL_COL_STRU *descolp);


/////////////////////////


typedef  struct {
	int 			colno;
	int				retcode;
	Rdb_Vartype_value	fieldvalue;
}  RDB_FLDNAME_VAL_CL_STRU;



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_BaseTab {
   RDB_BTBL_TAB_STRU	 *tableinfo;
   RDB_BTBL_COL_STRU	  *columninfo;
   RDB_BTBL_COL_STRU	  *srchcolumninfo;
   char				  *frcdaddr;
   int					currcdno;
   char				*currcdaddr;
	RDB_BTBL_COL_STRU  *RdbGetColumnInfo(char *colname);
	RDB_BTBL_COL_STRU  *RdbGetColumnInfo(int  columnno);
 public:
	 Rdb_BaseTab(void) {
		 tableinfo=NULL;
		 columninfo=NULL;
		 srchcolumninfo=NULL;
		 frcdaddr=NULL;
		 currcdno=0;
		 currcdaddr=NULL;
	 };

	~Rdb_BaseTab(void) {
		if(srchcolumninfo!=NULL)free(srchcolumninfo);
	};
virtual int		RdbReadRight(void) {return(1);}
virtual int		RdbWriteRight(void) {return(0);}
   void         RdbBindTabInfo(RDB_BTBL_TAB_STRU *tp,RDB_BTBL_COL_STRU *fp,void *faddr);
virtual int	    RdbGetColBaseValue(int rcdno,RDB_BTBL_COL_STRU *columninfop,Rdb_Vartype_value *gvtval);
virtual int	    RdbSetColBaseValue(int rcdno,RDB_BTBL_COL_STRU *columninfop,Rdb_Vartype_value *pvtval);

   void         RdbGetTableInfo(void  **raddr,RDB_BTBL_TAB_STRU *tab_info);  // 取表的首地址,记录个数,记录长度
	int			RdbGetRcdLen(void);
    int         RdbGetAllColumnInfo(RDB_BTBL_COL_STRU *colinfaddr);//取全部列信息
	int			RdbGetAllColumnInfo(RDB_BTBL_COL_STRU **col);
    int         RdbGetSelectColumnInfo(int columnnums,RDB_BTBL_COL_STRU *colinfaddr);    // 取指定列信息
    int         RdbGetColumnInfo(char *colname,RDB_BTBL_COL_STRU *colinfaddr);    // 按单个列信息
    int         RdbGetColumnInfo(int colno,RDB_BTBL_COL_STRU *colinfaddr);    // 按单个列信息
    int         RdbGetColumnInfo(char *colname,uchar *type,int  *len);  // 取域的数据类型和长度
    int         RdbGetColumnInfo(int colno,uchar *type,int  *len);  // 取域的数据类型和长度	
	int			RdbGetColNo(char *colname);
	void       *RdbGetColumnAddr(int rcdno,int colno);                        // 取记录中某列的数据地址
   void        *RdbGetColumnAddr(int rcdno,char *colname);                   // 取域的地址
virtual int		RdbGetRcdCnt(void);
virtual void   *RdbGetRecordAddr(int rcdno);

	int		    rdbgetrcdcnt(void) { return(RdbGetRcdCnt());};   // 取记录个数
   char        *rdbgetfirstrcdaddr(void);
   char        *rdbgetlastrcdaddr(void);
   char        *rdbgetnextrcdaddr(void);
   void        *rdbgetrcdaddr(int rcdno){return(RdbGetRecordAddr(rcdno));};// 取记录地址


	int			RdbGetColumnValue(int rcdno,char *colname,Rdb_Vartype_value *gvtval);//取列值
    int         RdbGetColumnValue(int rcdno,char *colname,RDB_VALUETYPE_STRU *gvtval);  
    int         RdbGetColumnValue(int rcdno,int colno,Rdb_Vartype_value *gvtval);
    int         RdbGetColumnValue(int rcdno,int colno,uchar *type,void *valaddr);                 // 按记录号,域号取域值
    int         RdbGetColumnValue(int rcdno,int colno,uchar exptype,int slen,void *valaddr);      // 按记录号,域号取域值
	int         RdbGetColumnValue(int rcdno,char *colname,uchar exptype,int slen,void *valaddr); // 按记录号,域名取域值
    int         RdbGetColumnValue(int rcdno,char *colname,uchar *type,void *valaddr); // 按记录号,域名取域值

    int			RdbGetMultiColumnVal(int rcdno,int nums,RDB_FLDNAME_VAL_CL_STRU  *fnvcl);// get mult column value

    float       RdbGetVal_float(int rcdno,int colno);
    double      RdbGetVal_double(int rcdno,int colno);
    int         RdbGetVal_int(int rcdno,int colno);
    uint        RdbGetVal_uint(int rcdno,int colno);
    hlong       RdbGetVal_long(int rcdno,int colno);
    uhlong      RdbGetVal_ulong(int rcdno,int colno);
    char       *RdbGetString(int rcdno,int colno);

    float       RdbGetVal_float(int rcdno,char *fldname);//按单精度浮点取域值
    double      RdbGetVal_double(int rcdno,char *fldname);//按双精度浮点取域值
    int         RdbGetVal_int(int rcdno,char *fldname);//按有符号整数型取域值
    uint        RdbGetVal_uint(int rcdno,char *fldname);//按无符号整数型取域值
    hlong       RdbGetVal_long(int rcdno,char *fldname);//按有符号长整型取域值
    uhlong      RdbGetVal_ulong(int rcdno,char *fldname);//按无符号长整数型取域值
    char       *RdbGetString(int rcdno,char *fldname);//按符号串型取域值
	

	int		    RdbSetColVal(int rcdno,char *colname,Rdb_Vartype_value *pvtval);//修改列值
	int		    RdbSetColVal(int rcdno,char *colname,RDB_VALUETYPE_STRU *pvtval);
	int		    RdbSetColVal(int rcdno,char *colname,uchar type,void *valaddr);
	int		    RdbSetColVal(int rcdno,int colno,Rdb_Vartype_value *pvtval);
	int		    RdbSetColVal(int rcdno,int colno,uchar type,void *valaddr);
	
	int		    RdbCompareAccordingCond(int rcdno,int condnums,RDB_COND_STRU *condinfop);
	int		    RdbSearchRcdAccordingCond(char *fldname1,uchar valtype1,void *valaddr1,...);//按条件查找记录
	int		    RdbGetRcdCntAccordingCond(int comdnums,RDB_COND_STRU *condinfop);

virtual int     RdbDispTabStruct(void);
    int         RdbDispRcd(void);
};//二维表基类 包含了所有对二维表的操作





//////////////
#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_KeyBptree : public Rdb_Bptree {
	int key_cmp_type;
	int key_col_nums;
	int key_col_offset[RDB_KEYCOL_NUMS];
	uchar key_col_datatype[RDB_KEYCOL_NUMS];
public:
	void QSSInitCompare(int key_flag,int col_nums,int *offset,uchar *datatype);
	int  RdbCompareKey(void *keydp1,void *keydp2);
};

typedef struct {
	int		ctrltabno;
	int		tabid;
	uchar   tabletype;
	uchar   havemirror;
} RDB_MIRROR_TAB_STRU;

typedef struct {
	short  rawcolno;
	uchar  havemirror;
} RDB_MIRROR_COL_STRU;

	


class 
#ifdef RDB_PLATFORM_WIN32
 __declspec(dllexport)
#endif
Rdb_LocalTab : public Rdb_BaseTab {
	void				*table_ctrladdr;
	int					 max_rcdcount;
	uchar				 selfunlockf;
	RDB_BTBL_TAB_STRU	 table_info;
	RDB_BTBL_COL_STRU	*column_info;
	RDB_MIRROR_TAB_STRU	 mrtable_info;
	RDB_MIRROR_COL_STRU	*mrcolumn_info;
	char				*table_addr;
	char				 tablegrpname[RDB_USERNAME_LEN];
	char				 key_flag;
	Rdb_KeyBptree		 rdb_key_bptree;
	void				 PickUpKey(char *rcdbuf,char *key);
	void				 HashInKey(char  *key,char  *rcdbuf);
	void				*GetAbsRcdAddr(int rcdno);
	int                  GetTabRcdCnt(void);
	int					 UpdTabRcdCnt(int  rcdcnt);
	int                  DelTabRcd(int rcdno,int nums);
	int					 OpenTabForCtrlAddr(void *ctrladdr,int ctrltabno);
public:
	Rdb_LocalTab(void);
	~Rdb_LocalTab(void);
virtual void RdbSetRight(void);
virtual int	 RdbWriteRight(void) {return(1);}
virtual	int	 RdbOpenTab(char *tabname);
	int		 RdbOpenTabForId(int tabid);
	int		 RdbCloseTab(void);             //关闭表
	void     RdbSetSelfUnlock(void);
	void     RdbUnsetSelfUnlock(void);
	void	 RdbLockTabNoAccess(void);     //封锁对本表的所有访问
	void	 RdbLockTabWithRW(void);       //封锁对本表记录的增加及删除操作，但不封锁读写操作
	void	 RdbUnlockTab(void);           //解锁
	int		 RdbGetKeyInfo(int *key_flag,int *key_col_nums,int *key_col_offset,uchar *key_col_datatype);
	int		 RdbGetKeyType(void);
	int      RdbGetColValByKey(void *keydp,char *col_name,Rdb_Vartype_value *gval);
	int      RdbSetColValByKey(void *keyaddrp,char *col_name,Rdb_Vartype_value *pval);
	int      RdbSearchRcdByKey(void *keyaddrp);   //按关键字查找记录
	void    *RdbSearchRcdAddrByKey(void *keydp,int *rcdno=NULL);
	int      RdbAddRecord(void *rcdaddr);       //增加一个记录
	int      RdbAddEmptyRecordsToTail(int rcdnums);      //尾追多个记录
	int      RdbAddRecordByKey(void *keyaddrp);   //按关键字增加记录
	int      RdbTruncateTailRecords(int rcdnums);      //尾部删除多个记录
	int      RdbDelRecordByKey(void *keyaddrp);   //按关键字删除记录
	int      RdbDelRecord(int rcdno,int nums);//删除记录
	int      RdbTruncateAllRecords(void);            //删除所有记录
	int		 RdbGetRcdCnt(void);
	int		 RdbGetMaxRcdCnt(void);
	int      RdbGetUserRight(char *user_name,uchar *read_right,uchar *write_right);
	int      RdbGetUserRight(HUSER huser,uchar *read_right,uchar *write_right);
	int		 RdbRefreshKeyIndex(void);
	int		 RdbRefreshKeyIndex_SetRcdCount(int rcdnums);
	int      RdbDelRcdFromDataPartion(int rcdno,int nums);

	void     SetStructTime(void);
	int      GetStructTime(intertime *intime);
	int      IsStructChanged(intertime intime);

	void     UpdNoticeTable(void);
	void	 UpdNoticeRecord(int rcdno);
	void	 UpdNoticeRecord(void *rcdaddr);
	void	 UpdNoticeColumn(int rcdno,int colno);
	void	 UpdNoticeColumn(void *rcdaddr,int colno);
	void	 UpdNoticeColumn(int rcdno,char *colname);
	void	 UpdNoticeColumn(void *coladdr);
	void	 UpdNoticeAllColumn(int colno);
	void	 UpdNoticeAllColumn(char *colname);
	void	 UpdNoticeAllColumn(void *coladdr);

	friend class Rdb_MirrorNetData;
};






#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_Tab : public Rdb_LocalTab {
	HUSER  login_user;
	uchar  read_right;
	uchar  write_right;
	void   RdbSetRight(void);
public:
    Rdb_Tab(void);
	Rdb_Tab(char *username,char *password);
    ~Rdb_Tab(void);
	int		RdbReadRight(void) {return(read_right);};
	int		RdbWriteRight(void) {return(write_right);};
	int     RdbLogin(char *username,char *password); //用户登录
	void    RdbLogout(void);                       //退出用户登录
	int		RdbOpenTab(char *tabname);         //打开表
};  //本地表访问



//////////////////////////////////////////


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickQuery {
	HUSER    huser;
	void    *lasttablep;
	char	 lasttablename[RDB_TABNAME_LEN];
	int      RdbAddKeyTab(char *tabname);
	int      RdbReadRight(Rdb_LocalTab *rtable);
	int      RdbWriteRight(Rdb_LocalTab *rtable);
 public:
    Rdb_QuickQuery(void);
	Rdb_QuickQuery(char *username,char *password);
    ~Rdb_QuickQuery(void);
	int			RdbLogin(char *username,char *password);
	void        RdbLogout(void);
    Rdb_LocalTab	*GetTableClass(char *tabname);
    void	   *RdbGetRecordAddr(char *tabname,void *keyaddr);    //取记录地址
    int			RdbGetRecordNo(char *tabname,void *keyaddr);       //取记录号
	void	   *RdbGetRecordAddrNo(char *tabname,void *keyaddr,int *rcdno);
virtual  int	RdbGetColumnValueBase(char *tabname,void *keyaddr,char  *colname,Rdb_Vartype_value *getval);        // get one column value
virtual  int	RdbSetColumnValueBase(char *tabname,void *keyaddr,char  *colname,Rdb_Vartype_value *setval);  // set one column value
	int			RdbGetColumnValue(char *tabname,void *keyaddr,char  *colname,Rdb_Vartype_value *getval) { // get one column value
					return(RdbGetColumnValueBase(tabname,keyaddr,colname,getval));
				};
    int			RdbGetColumnValue(char *tabname,void *keyaddr,char  *colname,RDB_VALUETYPE_STRU *getval);        // 取列值
    int			RdbGetColumnValue(char *tabname,void *keyaddr,char  *colname,uchar *valtype,void *valaddr); // 取列值
    int			RdbGetExpColVal(char *tabname,void *keyaddr,char  *colname,uchar valtype,void *valaddr,int maxstrlen=0); // 按任意指定类型取列值
    float       RdbGetColValFloat(char *tabname,void *keyaddr,char  *colname);     //按单精度浮点类型取值
    double		RdbGetColValDouble(char *tabname,void *keyaddr,char  *colame);     //按双精度浮点类型取值
	int			RdbGetColValInt(char *tabname,void *keyaddr,char  *colname);        //按整型取值
    hlong       RdbGetColValLong(char *tabname,void *keyaddr,char  *colname);       //按有符号长整型取值
    uhlong		RdbGetColValULong(char *tabname,void *keyaddr,char  *colname);       //按无符号长整型取值

	int			RdbSetColVal(char *tabname,void *keyaddr,char  *colname,Rdb_Vartype_value *setval) {  //修改列值
					return(RdbSetColumnValueBase(tabname,keyaddr,colname,setval));			
				};
    int			RdbSetColVal(char *tabname,void *keyaddr,char  *colname,uchar valtype,void *valaddr);     // set one column value
	int			RdbSetMultiColVal(char *tabname,void *keyaddr,char  *colname,uchar valtype,void *valaddr,char  *columnname1,...); //修改多个列值
    //按指定类型修改列值
	int		    RdbSetColValFloat(char *tabname,void *keyaddr,char  *colname,float value);
    int			RdbSetColValDouble(char *tabname,void *keyaddr,char  *colname,double value);
    int			RdbSetColValInt(char *tabname,void *keyaddr,char  *colname,int value);
    
	int         RdbAddRecord(char *tabname,void *keyaddr);   //增加记录
    int			RdbDelRecord(char *tabname,void *keyaddr);//删除记录
    int			RdbTruncateAllRecords(char *tabname);//删除所有记录

	int			IsStructChanged(char *tabname,intertime intime);
  };//本地快速访问 包含了本地快速访问所有的接口函数


//////////////////////////////////////////////////////////////////////////////////////////////////////
// 优化的批量快速查询
typedef struct {
     char   tabname[RDB_TABNAME_LEN];
     char   fldname[RDB_COLNAME_LEN];
} RDB_OPTIMIZE_FLD_STRU;

typedef struct {
	intertime		updtime;
	Rdb_LocalTab   *rdbtabcl;
	short			colno;
	int 			rcdno;
} RDB_OPTIMIZE_QUERY_STRU;

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_OptimizeQuickQuery  {
	int				loginret;
	Rdb_QuickQuery  rdbspquery;
	int				querynums;
	int				keylen;
	RDB_OPTIMIZE_FLD_STRU  *optm_fielfinfo;
	void			*key_addr;
	RDB_OPTIMIZE_QUERY_STRU *optm_query_result;
	int ResetQueryPara(int nums,RDB_OPTIMIZE_FLD_STRU *optmfieldp,int klen,void *keyaddr);
public:
	Rdb_OptimizeQuickQuery(void);
	~ Rdb_OptimizeQuickQuery(void);
	int RdbLogin(char *username,char *password);
	int RdbSetQueryPara(int nums,RDB_OPTIMIZE_FLD_STRU *optmfieldp,int klen,void *keyaddr);
	RDB_OPTIMIZE_QUERY_STRU *GetQueryResult(void);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	 int     orderno;
     uchar   datatype;
     short   len;
} RDB_VALUE_STRU;


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_ArrayValue {
     char   *buffer;
     int    bufflen;
     int    valnums;
	 int    dataoff;
	 Rdb_PreMalloc  databufmalloc;
	 Rdb_PreMalloc  valinfomalloc;
  public:
     Rdb_ArrayValue(void) {
        buffer=NULL;
		valnums=0;
		dataoff=0;
		valinfomalloc.set_mallocpara(500,sizeof(RDB_VALUE_STRU));
		databufmalloc.set_mallocpara(2000,1);
     };
    ~Rdb_ArrayValue(void) {
       if(buffer!=NULL)free(buffer);
	 };
	void ArrayInit(void) {
		valinfomalloc.freespace();
		databufmalloc.freespace();
	    if(buffer!=NULL)free(buffer);
        buffer=NULL;
		valnums=0;
		dataoff=0;
	}
     void   RdbSetArrayVal(int orderno,Rdb_Vartype_value *val);
     int    RdbBuffInfo(void **buffaddr);
  };





#define  RDB_GET_FLDNAME_STREAM		0
#define  RDB_GET_FLDNAME_POLLING	1
#define  RDB_GET_FLDLINE_POLLING	2

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickQueryServer  {
	struct RDB_FIELD_SPLIT_STRU {
		int  startno;
		int		nums;
	};
	int						 fieldsplitnums;
	RDB_FIELD_SPLIT_STRU    *fieldsplit;
	int						 vtvalnums;
	int						 lastgetvalno;
	RDB_FLDNAME_VAL_CL_STRU	*lastvtval[2];

	Rdb_OptimizeQuickQuery   optrdbspquery;
	Rdb_ArrayValue   arrvalue;
	int	GetFieldForNetPolling_Name(char *hostname,char *username,char *password,
						int para_len,void *para_buf,
						int *ret_data_len,void **ret_data_buf);
	int	GetFieldForNetPolling_Line(char *hostname,char *username,char *password,
						int para_len,void *para_buf,
						int *ret_data_len,void **ret_data_buf);

	int GetSplitFieldValue(int fspn,RDB_FIELD_SPLIT_STRU *fspp,
							RDB_OPTIMIZE_QUERY_STRU *fisp,RDB_FLDNAME_VAL_CL_STRU *fnvcp);
	int	GetFieldForNetStream_Name(char *hostname,char *username,char *password,
						int para_len,void *para_buf,
						int *ret_data_len,void **ret_data_buf);
public:
	Rdb_QuickQueryServer(void);
	~Rdb_QuickQueryServer(void);
	int	GetFieldForNet(char *hostname,char *username,char *password,
						int method,int para_len,void *para_buf,
						int *ret_data_len,void **ret_data_buf);
	int	GetChangedFieldForNet(char *hostname,char *username,char *password,
						int method,int para_len,void *para_buf,
						int *ret_data_len,void **ret_data_buf);
	int SetFieldForNet(char *hostname,char *username,char *password,
						int method,int para_len,void *para_buf,
						int data_len,void *data_buf);
};


//////////////////////////////////////////////////////////////////

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_MultiTypeValue {
	RDB_VALUE_STRU *valinfo;
	char     *databuf;
    int      *offsetinfo;
	int		  malloc_databuf_len;
	int		  datalen;
    int       valuenums;
	int       maxparano;
	int      *paraorderno;
  public:
    Rdb_MultiTypeValue(void);
   ~Rdb_MultiTypeValue(void);
   void    RdbInitVal(void);
	int    RdbBindVal(void *badr,int blen);
	int    RdbGetValNums(void) {
		return(valuenums);
	}//取数据个数
	int	   RdbGetValtype(int valno,Rdb_Vartype_value *val);
   void	  *RdbGetValInfo(int valno,uchar *type,int  *len=NULL,int *orderno=NULL);//取数据信息
	int	   RdbGetValOrderno(int valno);//取数据的参数序号
	int	   RdbGetParaValno(int  orderno);
    int	   RdbGetVal(int valno,uchar *type,void *valaddr);//取数据
    int    RdbGetMultiVal(int startvalno,int valnums,uchar *type,void *valaddr);//取连续的多个数据
    int    RdbGetVal_int(int valno=0);
  float    RdbGetVal_float(int valno=0);//按单精度浮点类型取数据
 double    RdbGetVal_double(int valno=0);//按双精度浮点类型取数据
  hlong    RdbGetVal_long(int valno=0);//按有符号长整数型取数据
 uhlong    RdbGetVal_ulong(int valno=0);//按无符号长整数型取数据
   char   *RdbGetString(int maxlen,int valno=0);//获取字符串数据的地址
  };//复合数据分析




#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_ServerField {
	Rdb_PreMalloc  finfomalloc;
	char server_name[RDB_USERNAME_LEN];
public:
	int groupno;
	Rdb_ServerField(void) {
		finfomalloc.set_mallocpara(1000,sizeof(RDB_FIELD_SRV_STRU));
	};
	~Rdb_ServerField(void) {
		return;
	};
	RDB_FIELD_SRV_STRU *GetFieldInfo(int *nums) {
		return((RDB_FIELD_SRV_STRU *)finfomalloc.get_mallocinfo(nums));
	};
	void FreeFieldInfo(void)	{
		finfomalloc.freespace();
	};
	char *ServerName(void) {
		return(server_name);
	};
	void RdbAddFieldInfo(int orderno,RDB_FIELD_STRU *fldinfo);
	int  RdbGetSendInfo(void **buff);
};
	

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_SplitField
{
	Rdb_PreMalloc     serv_field_info;
	Rdb_ServerField  *GetServerInfo(int groupno,char *serv_name);
	void      FreeServerFieldInfo();
public:
	Rdb_SplitField(void);
	~Rdb_SplitField(void);
	int RdbInputFieldInfo(int groupno,int nums,RDB_FIELD_STRU *fldinfo);
	int RdbGetSrvNums(void);
	int RdbGetSrvFieldInfo(int groupno,int serv_no,char *server_name,int *fldnums,RDB_FIELD_SRV_STRU **fldinfo);
	int RdbGetSrvFieldInfo(int groupno,int serv_no,char *server_name);
	int RdbGetSrvSendInfo(int groupno,int serv_no,char *server_name,int *len,void **buff);
};



//////////////////////////////////////////////////////////////////////////
#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickPolling : public Rdb_SplitField {
	int          updfieldnums;
	Rdb_ArrayValue   updarrvalue;
	Rdb_PreMalloc	 updfieldinfo;
public:
	Rdb_QuickPolling(void) {
		updfieldnums=0;
		updfieldinfo.set_mallocpara(500,sizeof(RDB_FIELD_STRU));
		MSetFieldInit();
	}
	int RdbGetFieldValue(char *username,char *password,
		int nums,RDB_FIELD_STRU *fieldinfo,Rdb_MultiTypeValue *getvalue);//取值

	int RdbSetFieldValue(int groupno,char *username,char *password,char *tabname,
		char *objname,char *fldname,uchar valtype,void *valaddr);//修改值

	int MSetFieldInit(void) {
		updfieldnums=0;
		updarrvalue.ArrayInit();
		updfieldinfo.freespace();
		return(RDB_OK);
	}
    int RdbMSetFieldVal(int groupno,char *table_name,char *objname,char *fldname,uchar valtype,void *valaddr);
    int RdbMSetFieldExec(int groupno,char *username,char *password);
};//问答方式 包含了问答方式所有的接口函数


typedef  struct win_link{
	int groupno;
	int winno;
	uchar  stopflag;
	uchar  getflag;
	Rdb_SplitField   *spinfo;
	Rdb_QuickQueryServer	 *localspserv;
} RDB_WIN_LINK_STRU;

typedef  struct stream_link{
	int groupno;
	char   server_name[RDB_USERNAME_LEN];
	void  *serv_stream;
} RBD_STREAM_LINK_STRU;



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_QuickStream {
	int grpnum,group_no[GROUP_MAX];
    Rdb_PreMalloc        winmalloc;
	Rdb_PreMalloc		 streamalloc;
	RDB_WIN_LINK_STRU	*GetWinLink(int groupno,int winno);
	void			     FreeWinLink(void);
	void                *GetStreamLink(int groupno,char *server_name);
	void		   	     FreeStreamLink(void);
public:
	Rdb_QuickStream(void);
	~Rdb_QuickStream(void);
	int RdbSetStreamPara(char *username,char *password,int freshperiod,
			int paranums,RDB_FIELD_STRU *fldaddr,int winno
#ifdef RDB_PLATFORM_WIN32
			,rdbcalleridtype winhandle
#endif
		,void (STDCALL *func)(void *buf,int ord,int len,int num) = NULL	);//设置刷新参数

	int RdbGetFieldValue(int win_no,Rdb_MultiTypeValue *getvalue);//取值
	int RdbGetFieldValue(int win_no,Rdb_MultiTypeSave *getvalue);
   void RdbStopFresh(int winno=0xff);//停止刷新
};//流方式


//////////////////////////////////////////////////////////////////////////////////////////////

#define RDB_SQLCMD_NULL			0
#define RDB_SQLCMD_SELECT		1
#define RDB_SQLCMD_UPDATE		2
#define RDB_SQLCMD_INSERT		3
#define RDB_SQLCMD_DELETE		4
#define RDB_SQLCMD_GETCOL		10
#define RDB_SQLCMD_GETKEY		11


#define   RDB_SQL_ASCEND        0
#define   RDB_SQL_DESCEND       1




typedef struct {
	ushort   commandcode;
	uchar    version;
	uchar    subversion;
	uchar    reserve[4];
}	RDB_SQL_CTRLHEAD_STRU;


typedef struct {
   char     tabname[RDB_TABNAME_LEN];
   int      colnums;
   short	condnums;
   uchar    ordertype;
   short	ordercolnums;
} RDB_SQL_TAB_STRU;

typedef struct {
   char     selcolname[RDB_COLNAME_LEN];
} RDB_SQL_SELCOL_STRU;

typedef struct {
  char  updcolname[RDB_COLNAME_LEN];
  RDB_VALUETYPE_STRU  updvalue;
} RDB_SQL_UPDCOL_STRU;


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_BaseRdbSql {
	int			  order_type;
	Rdb_PreMalloc     selinfomalloc;
	Rdb_PreMalloc     orderinfomalloc;
	Rdb_PreMalloc     updinfomalloc;
	Rdb_PreMalloc	  whereinfomalloc;
	int		 GetWhereStatement(char *statement,int len);
 public:
	int		 command_type;
    char     rsql_table_name[RDB_TABNAME_LEN];
   Rdb_BaseRdbSql(void);
  ~Rdb_BaseRdbSql(void);
   void		 RdbClearSQLCmd(void);
   int       RdbGetSelectInfo(RDB_SQL_SELCOL_STRU **selcolumn);
   int		 RdbGetOrderType(void);
   int		 RdbGetOrderInfo(RDB_SQL_SELCOL_STRU **ordercolumn);
   int       RdbGetUpdateInfo(RDB_SQL_UPDCOL_STRU **updcolumn);
   int		 RdbGetWhereInfo(RDB_COND_STRU **condinfop);
   int		 RdbGetSQLStatement(char *statement,int len);

   //////// select /////////
   int       RdbSelectFrom(char *tabname);               //指定要选择的表名
   int       RdbSelectField(char *fldname);              //指定要选择的域名
   int		 RdbSetOrdertype(int ordertype);
   int		 RdbOrderBy(char *attrname);

   /////// update /////////
   int       RdbUpdTab(char *tabname);                  //指定要修改的表名
   int       RdbUpdFld(char *fldname,RDB_VALUETYPE_STRU *); 
   int       RdbUpdFld(char *fldname,uchar valtype,void *valaddr);//设置要修改的域及其值

   /////// insert ////////
   int		 RdbInserTab(char *tabname);
   int		 RdbSetValues(char *fldname,uchar valtype,void *valaddr);
   int       RdbSetValues(char *fldname,RDB_VALUETYPE_STRU *);

   ////// where  /////////
   int		 RdbClearWhere(void);
   int       RdbValTypeWhere(char *fldname,RDB_VALUETYPE_STRU *vtval,
                           uchar relationop=RELATION_OPER_EQU,uchar logicalop=LOGIC_OPER_AND);
   int       RdbWhereCond(char *fldname,uchar valtype,void *valaddr,
                           uchar relationopertype=RELATION_OPER_EQU,uchar logicopertype=LOGIC_OPER_AND);//设置选择或修改的条件
};



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_SQL : public Rdb_BaseTab,public Rdb_BaseRdbSql {
   RDB_BTBL_TAB_STRU    table_info;
   RDB_BTBL_COL_STRU   *column_info;
   Rdb_PreMalloc          selmalloc;
   int           errcode;
  
  protected:
	int	select_base(char *username,char *password,
				RDB_SQL_TAB_STRU  *seltable,RDB_SQL_SELCOL_STRU  *selcolumn,
				RDB_COND_STRU  *condinfop,RDB_SQL_SELCOL_STRU  *ordercol);
	int update_base(char *username,char *password,
				RDB_SQL_TAB_STRU  *updtable,RDB_SQL_UPDCOL_STRU  *updcolumn,
				RDB_COND_STRU  *condinfop);
	int insert_base(char *username,char *password,
				RDB_SQL_TAB_STRU  *instable,RDB_SQL_UPDCOL_STRU  *inscolumn);
	int delete_base(char *username,char *password,
				RDB_SQL_TAB_STRU  *deltable,RDB_COND_STRU  *condinfop);
  public:
   Rdb_SQL(void);
  ~Rdb_SQL(void);
   int  RdbSelCmd(char *username,char *password);//执行选择
   int  RdbUpdCmd(char *username,char *password);//执行修改
   int  RdbDelCmd(char *username,char *password,char *tabname);//执行删除
   int  RdbInsCmd(char *username,char *password);//执行插入
}; //本地SQL访问 包含了本地SQL访问的接口函数


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_SqlSrv : public Rdb_SQL {
	RDB_BTBL_TAB_STRU   tmptableinfo;
	RDB_BTBL_COL_STRU  *tmpcolumninfo;
	RDB_BTBL_COL_STRU  netgetcolinfo;
	char	 keycolname[RDB_KEYCOL_NUMS][RDB_COLNAME_LEN];
	int  SelectTableForNet(uchar version,uchar  subversion,
					   char *hostname,char *username,char *password,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2,
                       void **data_addr1,int *data_len1,
					   void **data_addr2,int *data_len2,
					   void **data_addr3,int *data_len3
					   );
	int UpdateTableForNet(uchar version,uchar  subversion,
					   char *hostname,char *username,char *password,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2
					   );
	int DeleteTableForNet(uchar version,uchar  subversion,
					   char *hostname,char *username,char *password,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2
					   );
	int InsertTableForNet(uchar version,uchar  subversion,
					   char *hostname,char *username,char *password,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2
					   );
	int GetColumnInfoForNet(uchar version,uchar  subversion,char *tabname,
						char *colname,RDB_BTBL_COL_STRU *colinfop);
	int GetKeyInfoForNet(uchar version,uchar  subversion,char *tabname);
   public:
	Rdb_SqlSrv(void);
	~Rdb_SqlSrv(void);
	int  SelectForNet(char *hostname,char *username,char *password,
					   void *ctrlheadp,int ctrlhead_len,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2,
                       void **data_addr1,int *data_len1,
					   void **data_addr2,int *data_len2,
					   void **data_addr3,int *data_len3);
    int  UpdateForNet(char *hostname,char *username,char *password,
					   void *ctrlheadp,int ctrlhead_len,
					   void *para_addr1,int para_len1,
					   void *para_addr2,int para_len2);
};



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_SqlCli : public Rdb_BaseTab,public Rdb_BaseRdbSql {
    RDB_BTBL_TAB_STRU    table_info;
    RDB_BTBL_COL_STRU   *column_info;
	void			*mem_addr;
   public:
    Rdb_SqlCli(void);
   ~Rdb_SqlCli(void);
    int    RdbSelCmd(int groupno,char *username,char *password);//执行选择
    int    RdbUpdCmd(int groupno,char *username,char *password);//执行修改
    int    RdbDelCmd(int groupno,char *username,char *password,char *tabname);
    int    RdbInsCmd(int groupno,char *username,char *password);

	// this function fetch column information through network
	int    RdbGetColInfo(int groupno,char *tabname,char *colname,RDB_BTBL_COL_STRU *cinfo);
	int    RdbGetKeyInfo(int groupno,char *tabname,char *keyname);
};//远程SQL访问接口

/////////////////////////

#ifdef RDB_PLATFORM_WIN32
void __declspec(dllexport)
#else
void
#endif
DispRdbSysInfo(void);


///////////////

typedef  struct  {
	int   table_count;
} RDB_TAB_LOCATOR_STRU;

typedef  struct  {
	char  table_name[RDB_TABNAME_LEN];
	char  table_desc[RDB_TABFULLNAME_LEN];
	char  server_name[RDB_USERNAME_LEN];
} RDB_TAB_LOCATION_STRU;


int  RdbCreateTableLocation(void);

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_LocateTable {
	static RDB_TAB_LOCATOR_STRU  *rdblocation_man;
	static RDB_TAB_LOCATION_STRU  *rdblocation;
	static Rdb_Bptree		location_bptree;
public:
	Rdb_LocateTable(void);
	int  RdbAddLocationInfo(char *tabname,char *desc,char *servername);
	int  RdbGetServerName(char *tabname,char *servername);
	int  RdbGetTabNums(void);
	int  RdbGetTableInfo(int tableno,char *tabname,char *description,char *servername);
	int  RdbGetTableInfo(char *tabname,char *description,char *servername);
};



////////////////////////////////////////////////

#ifdef RDB_PLATFORM_WIN32
class  __declspec(dllexport)
#else 
class   
#endif
Rdb_HisLib  {
	RDB_BTBL_TAB_STRU  table_info;
	RDB_BTBL_COL_STRU *column_info;
	char   curlibname[RDB_TABFULLNAME_LEN];
	char   curtablename[RDB_TABFULLNAME_LEN];
	void   *hislib_func;
	void   *hislib_datafmt;
public:
	DBApi dbapi;
	Rdb_HisLib(void);
	~Rdb_HisLib(void);
	int  GetHisTabInfo(char *libname,char *tabname,
		 RDB_BTBL_TAB_STRU **table_infop=NULL,RDB_BTBL_COL_STRU **col_infop=NULL);
	int  GetHisSelColumnInfo(int colnums,RDB_BTBL_COL_STRU *colp);
	int  BlkCpOut(char *libname,char *tabname,void **rcdbuff);
	int  BlkCpIn(char *libname,char *tabname,void *rcdbuff,int  rcdnums);
	int  SqlCmd(char *sql_str);
	int  SqlCmd(char *sql_str,int dbhostflag);
	int  DelRcds(char *libname,char *tabname,char *wherestr=NULL);
	int  SelRcds(char *libname,char *tabname,void **rcdbuff,char *wherestr=NULL);
	int  UpdRcds(char *libname,char *tabname,int nums,RDB_FLDNAME_VAL_STRU *updvalue,char *wherestr);
	int  InsRcds(char *libname,char *tabname,int nums,RDB_FLDNAME_VAL_STRU *insertvalue);
};



#ifdef RDB_PLATFORM_WIN32
class  __declspec(dllexport)
#else 
class   
#endif
Rdb_HisTab : public Rdb_BaseTab, public Rdb_HisLib {
	 RDB_BTBL_TAB_STRU  newtable_info;
	 RDB_BTBL_COL_STRU *newcolumn_info;
     int             currcdno;
     char           *frcdaddr;
     char           *currcdaddr;
  public:
     Rdb_HisTab(void);
    ~Rdb_HisTab(void);
     int    OpenTab(char *libname,char *tabname,char *wherestr=NULL);
     int    OpenTab(char *libname,char *tabname,int colnums,char colname[][RDB_COLFULLNAME_LEN],char *wherestr=NULL);
     int    CloseTab(void);
};



#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int   
#endif
HisBulkCopyOut(char *libname,char *tabname,void **rcdbuff);

////////////////////////////////////////////////////////  

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int   
#endif
SetStartRDB_CallNums(int setcallnums);


#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int   
#endif
Start_RDB(void); //启动实时数据库

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int   
#endif
MapAllRdbShm(void);


#ifdef RDB_PLATFORM_WIN32
int   __declspec(dllexport)
#else
int 
#endif
RdbBackupTab(char *user_name,char *password,char *tabname);//备份一个表

#ifdef RDB_PLATFORM_WIN32
int   __declspec(dllexport)
#else
int 
#endif
RdbBackupRcd(char *user_name,char *password,char *tabname,int rcdno);//备份表中的一个记录

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else
int 
#endif
RdbLoadTab(char *host_name,char	*user_name,char *password,char	*tabname);//动态装载实时表

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else 
int   
#endif
RdbReloadUser(void);//重新装载口令表

#ifdef RDB_PLATFORM_WIN32
int	 __declspec(dllexport)
#else
int 
#endif
RdbLoadDbMode(char *host_name,char *user_name,char *password,int	load_type);


#ifdef RDB_PLATFORM_WIN32
int  	 __declspec(dllexport)  
#else
int  
#endif
GetMirrorDataForNet(void **data,int *datalen);

#ifdef RDB_PLATFORM_WIN32
int  	 __declspec(dllexport)  
#else
int 
#endif
SetMirrorDataForNet(void *data,int datalen);



#ifdef RDB_PLATFORM_WIN32
void  	 __declspec(dllexport)  
#else
void  
#endif
NetCheckFreshRdbMainFlag(void);


// state: 0 正常 RDBRUNSTATE_SCADATRAIN scada培训态
#ifdef RDB_PLATFORM_WIN32
int  	 __declspec(dllexport)  
#else
int 
#endif
SetRdbRunstate(int state);

#ifdef RDB_PLATFORM_WIN32
int  	 __declspec(dllexport)  
#else
int 
#endif
GetRdbRunState(void);


/*
/////////////////////////////////////////////////////////////////////////////////
///////////////  与电力设备对象的接口 ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#include "dev.h"

//////////////////////////////////////////////////////////////////////////
////// 用设备对象结构RDB_DEV_FIELD和数据库对象结构RDB_FIELD_STRU  ////////////
////// 的混合结构进行实时库的POLLING方式访问                 ////////////
/////////////////////////////////////////////////////////////////////////
#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport) 
#else
class 
#endif
Rdb_MixPolling : public Rdb_QuickPolling {
public:
	int   GetMixFieldValue(char *username,char *password,int groupno,int nums, 
		RDB_MIX_FIELD *mixfield,Rdb_MultiTypeValue *getvalue);
};


//////////////////////////////////////////////////////////////////////////
////// 用设备对象结构RDB_DEV_FIELD和数据库对象结构RDB_FIELD_STRU  ////////////
////// 的混合结构进行实时库的Stream方式访问                 ////////////
/////////////////////////////////////////////////////////////////////////
#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_MixStream : public Rdb_QuickStream {
public:
	int   SetMixStreamPara(char *username,char *password,int groupno,
			int period,int nums, RDB_MIX_FIELD *mixfield,int winno
#ifdef RDB_PLATFORM_WIN32
			,rdbcalleridtype winhandle
#endif
		);
};
*/


#endif


