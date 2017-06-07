#ifndef _PROT_FBZ_H
#define _PROT_FBZ_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					FBZ						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			FBZ_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} FBZ_PROTECTACTION;

										//////////////////////////////////////////////
										/*			FBZ_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} FBZ_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			FBZ_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
		uchar	dotnums;		//小数位
} FBZ_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			FBZ_自诊断信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} FBZ_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						FBZ 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class FBZ_Protect : public BaseProtect
{
public:
	FBZ_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	FBZ_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	FBZ_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	FBZ_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	FBZ_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	int GetActionInfo(short actiontype,char *name,char *info);	//取保护动作描述
	int GetDetectInfo(short detecttype,char *name,char *info);	//取保护自检信息描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_trouble_apevent(SYS_CLOCK *sysclock, PROTECTMDL *mdlp, unsigned char *event,int measurenums);
};

#endif
