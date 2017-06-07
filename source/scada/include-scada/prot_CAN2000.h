#ifndef _PROT_CAN2000_H
#define _PROT_CAN2000_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
									//////////////////////////////////////////////////////
									/*					C2000						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			C2000_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		    //报警句
} CAN2000_PROTECTACTION;

										//////////////////////////////////////////////
										/*			C2000_测量类型			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		float   measurepoint;					//系数
		char	measureunit[PROT_UNIT_LEN];		//单位
} CAN2000_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			C2000_自检信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//自检故障信息描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} CAN2000_PROTECTDETECT;


							//////////////////////////////////////////////////////////////
							/*						C2000 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class CAN2000_Protect : public BaseProtect
{
public:
	CAN2000_Protect(void);
	
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort type1,ushort address1);
	CAN2000_PROTECTACTION	*GetActionPara(short actiontype);		//取保护动作参数
	CAN2000_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数
	CAN2000_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event,int measurenum);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
