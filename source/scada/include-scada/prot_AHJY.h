#ifndef _PROT_AHJY_H
#define _PROT_AHJY_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"

									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			AHJY动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		char    entname[ENTNAME_LEN];		//报警句
} AHJY_PROTECTACTION;

							//////////////////////////////////////////////////////////////
							/*						AHJY 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class AHJY_Protect : public BaseProtect
{
public:
	AHJY_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	AHJY_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
