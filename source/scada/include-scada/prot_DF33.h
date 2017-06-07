#ifndef _PROT_DF33_H
#define _PROT_DF33_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_DF33 PROTTYPE_FORTYTWO

	
									//////////////////////////////////////////////////////
									/*					DF33						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			DF33_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		uchar devicetype;						//装置类型
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    char    entname[ENTNAME_LEN];		//报警句
} DF33_PROTECTACTION;

										//////////////////////////////////////////////
										/*			DF33_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} DF33_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			DF33_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} DF33_PROTECTMEASURE;


typedef struct 
{
		short   devicetype;
		short	code;
} KEY_DF33;

							//////////////////////////////////////////////////////////////
							/*						DF33 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class DF33_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	DF33_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2,ushort,ushort);
	DF33_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//取保护动作参数
	DF33_PROTECTTROUBLE	*GetTroublePara(short troubletype);	//取故障信息参数
	DF33_PROTECTMEASURE  *GetMeasurePara(short measuretype);	//取测量值信息参数

	int GetActionInfo(short type1,int actiontype,char *name,char *info);	//取保护动作描述
	int GetTroubleInfo(short type1,short troubletype,char *info);//取保护故障描述
	
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_trouble_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *actevent);	
	void SunFloat(uchar *source, uchar *des);
	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
