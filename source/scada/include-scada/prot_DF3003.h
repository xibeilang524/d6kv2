#ifndef _PROT_DF3003_H
#define _PROT_DF3003_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"

									//////////////////////////////////////////////////
									/*					DF3003						*/
									//////////////////////////////////////////////////
										//////////////////////////////////////////
										/*	DF3003_动作自检(df3003_action)		*/
										//////////////////////////////////////////


typedef struct {
		ushort funaction;						//功能类型
		ushort  actcode;						//序号
	} DF3003_ACTION_KEY;

typedef	struct {
		ushort	funaction;						//功能类型
		ushort	actcode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//报警标志
		uchar	sgflag;							//事故标志
    	char    entname[ENTNAME_LEN];		//报警句
} DF3003_PROTECTACTION;


										//////////////////////////////////////////////
										/*			DF3003_量测信息	(df3003_measure)*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DF3003_PROTECTMEASURE;


										//////////////////////////////////////////////
										/*			DF3003_故障信息(df3003_trouble)	*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DF3003_PROTECTTROUBLE;


							//////////////////////////////////////////////////////////////
							/*						DF3003 微机保护处理类					*/
							//////////////////////////////////////////////////////////////
class DF3003_Protect : public BaseProtect
{
public:
	DF3003_Protect(void);
	float Dword2Float(long TempDword);
	long Dword2Val_Sun(long *TempDword);//Sun机器下的浮点算法

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type2,ushort address1);
	DF3003_PROTECTACTION	*GetActionPara(ushort funaction,ushort actcode);
	DF3003_PROTECTMEASURE  *GetMeasurePara(ushort measurecode);		//取测量值信息参数
	DF3003_PROTECTTROUBLE	*GetTroublePara(ushort troublecode);	//取故障信息参数

	int GetActionInfo(ushort actiontype,char *name,char *info);
	int GetMeasureInfo(ushort measuretype,char *info,char *unit);	//取保护测量值描述
	int GetTroubleInfo(ushort troublecode,char *name,char *info);//取保护故障描述

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
	int  protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
};
 

#endif
