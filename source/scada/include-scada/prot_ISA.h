#ifndef _PROT_ISA_H
#define _PROT_ISA_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					ISA						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ISA_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} ISA_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ISA_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} ISA_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			ISA_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} ISA_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			ISA_自诊断信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} ISA_PROTECTDETECT;


										//////////////////////////////////////////////////
										/*	ISA微机保护小电流接地选线装置参数  	*/
										//////////////////////////////////////////////////
#define ISA_SMALLCUR_LINETYPE_MX 0				//ISA小电流接地母线类型
#define ISA_SMALLCUR_LINETYPE_XL 1				//ISA小电流接地线路类型

typedef struct {
		char name[OBJECTNAME_LEN];				//ISA小电流接地线路代码
		char describe[PROTINFO_LEN];			//ISA小电流接地线路描述
		short   terminalno;						//终端序号
		ushort  lineno;							//线路号(一般为0-10)
		ushort  linetype;						//线路类型(母线/线路)
		uchar alarmf;
		char	entname[ENTNAME_LEN];		//报警事项句
}ISA_SMALLCUR;


							//////////////////////////////////////////////////////////////
							/*						ISA 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class ISA_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	ISA_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	ISA_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	ISA_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	ISA_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	ISA_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数
	ISA_SMALLCUR        *GetSmallCurPara(short terminalno,short lineno,short linetype);	//取小电流接地装置参数

	int GetActionInfo(short actiontype,char *name,char *info);	//取保护动作描述
	int GetDetectInfo(short detecttype,char *name,char *info);	//取保护自检信息描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	void make_smallcurrent_apevent(SYS_CLOCK *sysclock,ISA_SMALLCUR *smallcurp);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	void save_smallcurrent_event(SYS_CLOCK *sysclock,ISA_SMALLCUR *smallcurp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
