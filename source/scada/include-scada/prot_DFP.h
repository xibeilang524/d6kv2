#ifndef _PROT_DFP_H
#define _PROT_DFP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
										//////////////////////////////////////////////
										/*			DFP_动作信息(dfp_action)		*/
										//////////////////////////////////////////////
typedef	struct {
		uchar	actbit;			//码位
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;			//报警
		uchar	sgflag;			//事故
    	char    entname[ENTNAME_LEN];		//报警句
} DFP_PROTECTACTION;
										//////////////////////////////////////////
										/*		DFP_自检信息(dfp_detect)		*/
										//////////////////////////////////////////
typedef	struct {
		uchar	boardtype;  //板类型
		uchar   chkbit;		//码位
		char	detectinfo[PROTINFO_LEN];	//描述
		uchar   alarmf;		//报警
   		char    entname[ENTNAME_LEN];	//报警句
} DFP_PROTECTDETECT;

							//////////////////////////////////////////////////////////////
							/*						DFP 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DFP_Protect : public BaseProtect
{
public:
	DFP_Protect(void);
	
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DFP_PROTECTACTION	*GetActionPara(uchar actbit);
	DFP_PROTECTDETECT	*GetDetectPara(uchar boardtype,uchar chkbit);
	
	int GetActionInfo(uchar actbit,char *name,char *info);
	int GetTroubleInfo(short troubletype,char *name,char *info);
	int GetDetectInfo(uchar boardtype,uchar chkbit,char *name,char *info);

	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);		//形成动作事件;
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);			//保存动作事件;
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);	//保存自检事件;
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);	//保存硬件故障报告
};

#endif
