#ifndef _PROT_ROAD_H
#define _PROT_ROAD_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
	
#define PROTTYPE_ROAD PROTTYPE_FORTYONE
#define TYPE_140	  140
#define TYPE_141	  141
	
									//////////////////////////////////////////////////////
									/*					ROAD						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ROAD_动作信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//装置地址
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    char    entname[ENTNAME_LEN];		//报警句
} ROAD_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ROAD_故障信息				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//装置地址
		short	troublecode;					//序号
		char	troubleinfo[PROTINFO_LEN];		//描述
} ROAD_PROTECTTROUBLE;
										//////////////////////////////////////////////
										/*			ROAD_测量值信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//装置地址
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		char	measureunit[PROT_UNIT_LEN];		//单位
} ROAD_PROTECTMEASURE;
										//////////////////////////////////////////////
										/*			ROAD_自诊断信息			*/
										//////////////////////////////////////////////
typedef	struct {
		short	devicetype;						//装置地址
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   	char    entname[ENTNAME_LEN];		//报警事项句
} ROAD_PROTECTDETECT;

typedef struct 
{
		short   devicetype;
		short	code;
} KEY_ROAD;

							//////////////////////////////////////////////////////////////
							/*						ROAD 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class ROAD_Protect : public BaseProtect
{
	static Rdb_Tab rdbtable;
	static int opentabf;
public:
	ROAD_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	ROAD_PROTECTACTION	*GetActionPara(short type1,short actiontype);		//取保护动作参数
	ROAD_PROTECTTROUBLE	*GetTroublePara(short type1,short troubletype);	//取故障信息参数
	ROAD_PROTECTMEASURE  *GetMeasurePara(short type1,short measuretype);	//取测量值信息参数
	ROAD_PROTECTDETECT	*GetDetectPara(short type1,short detecttype);		//取自检信息参数

	int GetActionInfo(short type1,short actiontype,char *name,char *info);	//取保护动作描述
	int GetDetectInfo(short type1,short detecttype,char *name,char *info);	//取保护自检信息描述
	int GetTroubleInfo(short type1,short troubletype,char *info);//取保护故障描述
	
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	int  GetProtinfoTime(uchar *gram_part, uchar binorbcd, SYS_CLOCK *clock, uchar flag);
};

#endif
