#ifndef _PROT_DMP_H
#define _PROT_DMP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_DMP PROTTYPE_FORTYEIGHT
	
									//////////////////////////////////////////////////////
									/*					DMP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DMP_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//装置地址
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    char    entname[ENTNAME_LEN];		//报警句
} DMP_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DMP_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DMP_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DMP_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DMP_PROTECTMEASURE;

typedef struct 
{
		short   devicetype;
		short	code;
} KEY_DMP;

							//////////////////////////////////////////////////////////////
							/*						DMP 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DMP_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DMP_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	DMP_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//取保护动作参数
	DMP_PROTECTTROUBLE	*GetTroublePara(short type1,short troubletype);	//取故障信息参数
	DMP_PROTECTMEASURE  *GetMeasurePara(short type1,short measuretype);	//取测量值信息参数
	
	int GetActionInfo(short type1,short actiontype,char *name,char *info);	//取保护动作描述
	int GetTroubleInfo(short type1,short troubletype,char *info);//取保护故障描述
	
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
