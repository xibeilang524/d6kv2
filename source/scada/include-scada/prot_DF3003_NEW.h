#ifndef _PROT_DF3003_NEW_H
#define _PROT_DF3003_NEW_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
#include "prot_DF3003.h"






							//////////////////////////////////////////////////////////////
							/*						DF3003 微机保护处理类					*/
							//////////////////////////////////////////////////////////////
class DF3003_NEW_Protect : public BaseProtect
{
public:
	DF3003_NEW_Protect(void);
	float Dword2Float(long TempDword);
	long Dword2Val_Sun(long *TempDword);//Sun机器下的浮点算法

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type2,ushort address1);
	DF3003_PROTECTACTION	*GetActionPara(ushort funaction,ushort actcode);
	DF3003_PROTECTMEASURE  *GetMeasurePara(ushort measurecode);		//取测量值信息参数
//	DF3003_PROTECTTROUBLE	*GetTroublePara(ushort troublecode);	//取故障信息参数

	int GetActionInfo(ushort actiontype,char *name,char *info);
	int GetMeasureInfo(ushort measuretype,char *info,char *unit);	//取保护测量值描述
//	int GetTroubleInfo(ushort troublecode,char *name,char *info);//取保护故障描述

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
	int  protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *acteventp);
};
 

#endif
