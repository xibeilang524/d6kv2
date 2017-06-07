#ifndef _PROT_SACDW_H
#define _PROT_SACDW_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					SACDW						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			SACDW事件信息参数表				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;
		short	moduletype;				//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;					//是否报警	
		uchar	sgflag;					//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} SACDW_PROTECTACTION;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_SACDW;


							//////////////////////////////////////////////////////////////
							/*						SACDW 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class SACDW_Protect : public BaseProtect
{

public:
	SACDW_Protect(void);
	ushort address1;//地址(下面的终端序号)
	int m_GramLength;//报文长度
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	SACDW_PROTECTACTION	*GetActionPara(short actioncode,short actiontype);		//取事件信息参数

	unsigned char m_Buf[1024];
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

		//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
