#ifndef _PROT_LSA_P_H
#define _PROT_LSA_P_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			LSA_P_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} LSA_P_PROTECTACTION;

										//////////////////////////////////////////////
										/*			LSA_P_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} LSA_P_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			LSA_P_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} LSA_P_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			LSA_P_自诊断信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} LSA_P_PROTECTDETECT;

										//////////////////////////////////////////////
										/*				LSA_P微机保护精度信息			*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	protocoltype;
		ushort	precisiontype;
		char	precisionname[OBJECTNAME_LEN];
		char	precisioninfo[PROTINFO_LEN];
} LSAP_PRECISION;


							//////////////////////////////////////////////////////////////
							/*						LSA_P 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class LSA_P_Protect : public BaseProtect
{
public:
	LSA_P_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	LSA_P_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	LSA_P_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	LSA_P_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	LSA_P_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	int GetTroubleInfo(short troubletype,char *info);//取保护故障描述
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};


#endif