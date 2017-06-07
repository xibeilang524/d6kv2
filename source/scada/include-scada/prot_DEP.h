#ifndef _PROT_DEP_H
#define _PROT_DEP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					DEP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DEP_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} DEP_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DEP_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DEP_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DEP_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DEP_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			DEP_自诊断信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} DEP_PROTECTDETECT;
									
							//////////////////////////////////////////////////////////////
							/*						DEP 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DEP_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DEP_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	DEP_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	DEP_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	DEP_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	DEP_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	int GetActionInfo(short actiontype,char *name,char *info);	//取保护动作描述
	int GetDetectInfo(short detecttype,char *name,char *info);	//取保护自检信息描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述
	//时间转换
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
