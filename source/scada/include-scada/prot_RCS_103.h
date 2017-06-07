#ifndef _PROT_RCS_103_H
#define _PROT_RCS_103_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
#include "prot_LFP.h"
									//////////////////////////////////////////////////////
									/*					RCS_103						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			RCS_103动作信息参数表				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//模块类型
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} RCS_103_PROTECTACTION;

										//////////////////////////////////////////////
										/*			RCS_103自检信息参数表			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detecttype;						//模块类型
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
		uchar	sgflag;							//是否判事故
   		char    entname[ENTNAME_LEN];		//报警事项句
} RCS_103_PROTECTDETECT;


typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_RCS_103;



							//////////////////////////////////////////////////////////////
							/*						RCS_103 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class RCS_103_Protect : public BaseProtect
{

public:
	RCS_103_Protect(void);
	ushort address1,address2;//主站地址，从站地址
	int m_GramLength;//报文长度
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	RCS_103_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//取保护动作参数
	RCS_103_PROTECTDETECT	*GetDetectPara(short detecttype,short detectcode);		//取自检信息参数

	LFP_PROTECTACTION *GetLFPActionPara(short moduletype,unsigned char boardnum,short actbit);
	LFP_PROTECTDETECT *GetLFPDetectPara(short moduletype,unsigned char flagtype,short chkbit);


	int GetDetectInfo(short,short detecttype,char *name,char *info);	//取保护自检信息描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//时间转换
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成应用事件
	void make_LFPaction_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_LFPaction_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_LFPdetect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
