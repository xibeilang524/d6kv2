#ifndef _PROT_DFP500_H
#define _PROT_DFP500_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			DFP500_动作信息(dfp500_action)	*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} DFP500_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DFP500_故障信息	(dfp500_trouble)*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DFP500_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DFP500_测量值信息(dfp500_measure)*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DFP500_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*	DFP500_自诊断信息(dfp500_detect)	*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} DFP500_PROTECTDETECT;


							//////////////////////////////////////////////////////////////
							/*						DFP500 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DFP500_Protect : public BaseProtect
{
public:
	DFP500_Protect(void);
	
	PROTECTMDL	*GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);			
	DFP500_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	DFP500_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	DFP500_PROTECTMEASURE   *GetMeasurePara(short measuretype);	//取测量值信息参数
	DFP500_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	int GetProtInfoTime(uchar *gram_part, SYS_CLOCK *clock);
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
