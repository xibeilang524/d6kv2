#ifndef   _PROTECT_H
#define   _PROTECT_H
#include  <string.h>

#include "scadatime.h"
#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "net/netdef.h"
#include "scd/scddef.h"

#include    "scadaproc.h"
#include    "scadainterface.h"

#define TIMEBYBIN 0
#define TIMEBYBCD 1
#define	BCD1BIN1(a)		(((a)/16)*10 + (a)%16)



//微机保护返回错误定义
#define  PROT_ERR_MAILLEN		   2		//保护邮件长度错误
#define  PROT_ERR_MODULE		   3		//保护模块错误
#define  PROT_ERR_MESSAGE		   4		//无该微机保护信息
#define  PROT_ERR_ACTION		   5		//保护动作信息错误
#define  PROT_ERR_DETECT		   6		//保护自检信息错误
#define  PROT_ERR_TROUBLE		   7		//保护故障信息错误
#define  PROT_ERR_NOTSUPPORT       10		//不支持的保护类型

////////////////////////////////////////////////////////////////////////////////
/////////////////  微机保护的相关实时库表名 //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define  PROTMDL_TABNAME            "protmodule"	//微机保护参数表

//LFP
#define  LFP_PROTMODULE_TABNAME     "lfp_module"	//LFP_模块信息
#define  LFP_PROTACTION_TABNAME     "lfp_action"	//LFP_故障信息
#define  LFP_PROTDETECT_TABNAME     "lfp_detect"	//LFP_自检信息
#define  LFP_PROTEVENT_TABNAME      "lfp_event" 	//LFP_事件信息

//ISA
#define  ISA_PROTACTION_TABNAME     "isa_action"	//ISA_动作信息
#define  ISA_PROTDETECT_TABNAME     "isa_detect"	//ISA_自诊断信息
#define  ISA_PROTTRBL_TABNAME       "isa_trouble"	//ISA_故障信息
#define  ISA_PROTMSR_TABNAME        "isa_measure"	//ISA_测量值信息
#define  ISA_SMALLCURRENT_TABNAME   "isa_smallcurrent" 


//ISA300
#define  ISA300_PROTACTION_TABNAME  "isa300_action"	//ISA300_动作信息
#define  ISA300_PROTDETECT_TABNAME  "isa300_detect"	//ISA300_自诊断信息
#define  ISA300_PROTTRBL_TABNAME    "isa300_trouble"//ISA300_故障信息
#define  ISA300_PROTMSR_TABNAME     "isa300_measure"//ISA300_测量值信息


//CAN2000
#define  CAN2000_PROTACTION_TABNAME  "can_action"	//C2000_动作信息
#define  CAN2000_PROTDETECT_TABNAME  "can_detect"	//C2000_自检信息
#define  CAN2000_PROTMSR_TABNAME     "can_measure"	//C2000_测量类型

//LSAP
#define  LSAP_PROTACTION_TABNAME     "lsap_action"	//LSA_P动作信息
#define  LSAP_PROTDETECT_TABNAME     "lsap_detect"	//LSA_P自诊断信息
#define  LSAP_PROTTRBL_TABNAME       "lsap_trouble"	//LSA_P故障信息
#define  LSAP_PROTMSR_TABNAME        "lsap_measure"	//LSA_P测量值信息
#define  PTOTPRSN_TABNAME			 "lsap_prcsn"   //LSA_P保护精度信息表

//DSA_301
#define  DSA_301_PROTACTION_TABNAME  "dsa301_action"//DSA_301动作信息
#define  DSA_301_PROTDETECT_TABNAME  "dsa301_detect"//DSA_301自诊断信息
#define  DSA_301_PROTTRBL_TABNAME    "dsa301_trouble"//DSA_301故障信息
#define  DSA_301_PROTMSR_TABNAME     "dsa301_measure"//DSA_301测量值信息

//SEL
#define  SEL_PROTACTION_TABNAME     "sel_action"	//SEL_动作信息
#define  SEL_PROTDETECT_TABNAME     "sel_detect"	//SEL保护诊断信息表

//DFP
#define  DFP_PROTACTION_TABNAME     "dfp_action"	//DFP_动作信息
#define  DFP_PROTTRBL_TABNAME       "dfp_trouble"	//DFP保护故障信息表
#define  DFP_PROTDETECT_TABNAME     "dfp_detect"	//DFP_自检信息

//SBC
#define  SBC_PROTACTION_TABNAME		"sbc_action"    //SBC_动作信息

//FWB
#define  FWB_PROTACTION_TABNAME		"fwb_action"    //FWB_动作信息
#define  FWB_PROTDETECT_TABNAME		"fwb_detect"	//FWB_诊断信息

//DF3003
#define DF3003_PROTACTION_TABNAME	"df3003_action"	//DF3003_动作自检信息
#define DF3003_PROTMEASURE_TABNAME	"df3003_measure"//DF3003_量测信息
#define DF3003_PROTTROUBLE_TABNAME	"df3003_trouble"//DF3003_故障信息

//WBX
#define WBX_PROTACTION_TABNAME	    "wbx_action"	//WBX_事件信息

//DFP500
#define  DFP500_PROTACTION_TABNAME  "dfp500_action"	 //DFP500_动作信息
#define  DFP500_PROTDETECT_TABNAME  "dfp500_detect"	 //DFP500_自诊断信息
#define  DFP500_PROTTRBL_TABNAME    "dfp500_trouble" //DFP500_故障信息
#define  DFP500_PROTMSR_TABNAME     "dfp500_measure" //DFP500_测量值


//CSC2000
#define  CSC2000_PROTACTION_TABNAME "csc2000_action"	//CSC2000_动作信息
#define  CSC2000_PROTDETECT_TABNAME "csc2000_detect"	//CSC2000_自诊断信息
#define  CSC2000_PROTTRBL_TABNAME   "csc2000_trouble"	//CSC2000_故障信息
#define  CSC2000_PROTMSR_TABNAME    "csc2000_measure"	//CSC2000_测量值信息


//FBZ
#define  FBZ_PROTACTION_TABNAME     "fbz_action"	//FBZ_动作信息
#define  FBZ_PROTDETECT_TABNAME     "fbz_detect"	//FBZ_自诊断信息
#define  FBZ_PROTTRBL_TABNAME       "fbz_trouble"	//FBZ_故障信息
#define  FBZ_PROTMSR_TABNAME        "fbz_measure"	//FBZ_测量值信息

//DWK
#define  DWK_PROTACTION_TABNAME     "dwk_action"	//DWK_动作信息
#define  DWK_PROTTRBL_TABNAME       "dwk_trouble"	//DWK_故障信息

//ZDB
#define  ZDB_PROTACTION_TABNAME     "zdb_action"	//ZDB动作信息
#define  ZDB_PROTDETECT_TABNAME     "zdb_detect"	//ZDB自诊断信息
#define  ZDB_PROTMEASURE_TABNAME		"zdb_measure"	//ZDB量测参数

//PS6000
#define PS6000_PROTACTION_TABNAME		"ps6000_action"			//PS6000动作信息

//许继DEP
#define  DEP_PROTACTION_TABNAME     "dep_action"	//DEP_动作信息
#define  DEP_PROTDETECT_TABNAME     "dep_detect"	//DEP_自诊断信息
#define  DEP_PROTTRBL_TABNAME       "dep_trouble"	//DEP_故障信息
#define  DEP_PROTMSR_TABNAME        "dep_measure"	//DEP_测量值信息 

//SACDW
#define  SACDW_PROTACTION_TABNAME     "sacdw_action"	//SACDW动作信息
//SACJD
#define  SACJD_PROTACTION_TABNAME     "sacjd_action"	//SACJD动作信息
#define  SACJD_PROTDETECT_TABNAME     "sacjd_detect"	//SACJD自诊断信息


//NSR	//add 2004 12
#define  NSR_PROTACTION_TABNAME     "nsr_action"	//NSR_动作信息
#define  NSR_PROTDETECT_TABNAME     "nsr_detect"	//NSR_自诊断信息
#define  NSR_PROTTRBL_TABNAME       "nsr_trouble"	//NSR_故障信息
#define  NSR_PROTMSR_TABNAME        "nsr_measure"	//NSR_测量值信息

//NEWZDB
#define  NEWZDB_PROTACTION_TABNAME     "newzdb_action"	//NEWZDB动作信息

#define  IEC103_PROTACTION_TABNAME     "iec103_action"	//IEC103动作信息

//ROAD //add 2006 04 for 铁路事业部
#define  ROAD_PROTACTION_TABNAME     "road_action"	//ROAD_动作信息
#define  ROAD_PROTDETECT_TABNAME     "road_detect"	//ROAD_自诊断信息
#define  ROAD_PROTTRBL_TABNAME       "road_trouble"	//ROAD_故障信息
#define  ROAD_PROTMSR_TABNAME        "road_measure"	//ROAD_测量值信息

//DF33 //add 2006 09 for 山东平阴
#define  DF33_PROTACTION_TABNAME     "df33_action"	//DF33_动作信息
#define  DF33_PROTTRBL_TABNAME       "df33_trouble"	//DF33_故障信息
#define  DF33_PROTMSR_TABNAME        "df33_measure"	//DF33_测量值信息

//NZ103 //add 2006 11 for 网络103
#define  NZ103_PROTACTION_TABNAME     "nz103_action"		//南自103动作信息
#define  NZ103_PROTDETECT_TABNAME     "nz103_detect"		//南自103故障信息

//RCS_103 //add 2006 12 for 南瑞 RCS 103
#define  RCS_103_PROTACTION_TABNAME     "rcs103_action"		//南自103动作信息
#define  RCS_103_PROTDETECT_TABNAME     "rcs103_detect"		//南自103故障信息


//DMP  //add 2007 05 for 力导DMP
#define  DMP_PROTACTION_TABNAME     "dmp_action"	//DMP_动作信息
#define  DMP_PROTTRBL_TABNAME       "dmp_trouble"	//DMP_故障信息
#define  DMP_PROTMSR_TABNAME        "dmp_measure"	//DMP_测量值信息

//DFDZ  //add 2007 11 for 东方电子
#define  DFDZ_PROTACTION_TABNAME     "dfdz_action"	//DFDZ_动作信息
#define  DFDZ_PROTDETECT_TABNAME     "dfdz_trouble"	//DFDZ_故障信息
#define  DFDZ_PROTMEASURE_TABNAME    "dfdz_measure"	//DFDZ_测量值信息

//安徽继远公司综合自动化保护 2007 11
#define  AHJY_PROTACTION_TABNAME	"ahjy_action"	//AHJY_动作信息
////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
/*														*/
/*					微机保护参数表(通用)				*/
/*														*/
//////////////////////////////////////////////////////////
typedef struct {
	char    name[OBJECTNAME_LEN];		//代码
    char    describe[OBJDESCRIBE_LEN];	//描述
    int     securegrade;				//安全级
	char	substation[OBJECTNAME_LEN];	//站所线系代码
    short   terminalno;					//终端序号	
    ushort  prottype;					//保护规约类型
	ushort  type1;						//类型1 
	ushort  type2;						//类型2 
	ushort  type3;						//类型3 
	ushort  type4;						//类型4 
	ushort  address1;					//地址1 
	ushort  address2;					//地址2 
	ushort  address3;					//地址3 
	ushort  address4;					//地址4 
}PROTECTMDL;




//////////////////////////////////////////
/*				微机保护处理基类		*/
//////////////////////////////////////////
class BaseProtect : public Event
{
public:
		static Rdb_QuickQuery table;
		static Rdb_Tab promdltab;
        static int opentabf;
public:
		BaseProtect(void);
		int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock,uchar chbyte=0xFF,uchar nomsflag=0);	//时间处理函数
		PROTECTMDL *GetModulePara(ushort terminalno,ushort address1,ushort address2);
		PROTECTMDL	*GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort type2,ushort type3,ushort type4,ushort address1,
			ushort address2,ushort address3,ushort address4);
				//按终端号、规约类型、类型1、类型2、类型3、类型4、地址1、地址2、地址3、地址4取微机保护参数
					
		void make_common_saveinfo(SCDEVENT *,SYS_CLOCK *,PROTECTMDL *,int );
		void make_common_apentinfo(PROTECT_MSG *,SYS_CLOCK *,PROTECTMDL *,int ,int );
};


/********************************************************************/
/*						微机保护原始报文处理类						*/
/********************************************************************/
class Protect_Scdservice_serv : public Scdservice_serv {
   public:
	 Protect_Scdservice_serv(char *hm,char *um,char *pd,hlong  pid):Scdservice_serv(hm,um,pd,pid)
	 	{};
	 int  yieldprotectinfo(SCADA_PROT_STRU* protectp,int  datalen);
};

#endif
