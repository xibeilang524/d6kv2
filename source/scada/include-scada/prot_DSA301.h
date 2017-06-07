#ifndef _PROT_DSA301_H
#define _PROT_DSA301_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
									//////////////////////////////////////////////////////
									/*					DSA_301						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DSA_301动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} DSA_301_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DSA_301故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DSA_301_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DSA_301测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DSA_301_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			DSA_301自诊断信息			*/
										/////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} DSA_301_PROTECTDETECT;



							//////////////////////////////////////////////////////////////
							/*						DSA_301 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DSA_301_Protect : public BaseProtect
{
public:
	DSA_301_Protect(void);

	uchar m_Buf[256];		//200707
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	DSA_301_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	DSA_301_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	DSA_301_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	DSA_301_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
