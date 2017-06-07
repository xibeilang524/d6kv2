#ifndef _PROT_DWK_H
#define _PROT_DWK_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					DWK						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DWK_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
    	char    entname[ENTNAME_LEN];		//报警句
} DWK_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DWK_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} DWK_PROTECTTROUBLE;



							//////////////////////////////////////////////////////////////
							/*						DWK 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DWK_Protect : public BaseProtect
{
public:
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DWK_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	DWK_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数

	int GetActionInfo(short actiontype,char *name,char *info);	//取保护动作描述
	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event,int restoreflag);
};

#endif
