#ifndef _PROT_FWB_H
#define _PROT_FWB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
/*
	FWB保护诊断信息较简单，因而可在程序中硬编码解释；
	FWB保护动作信息与模块有关，因而在动作信息表中含有模块类型

#define PDEVTYPE_FWB_START				PDEVTYPE_FWB_1200
#define PDEVTYPE_FWB_1200				0		//3130
#define PDEVTYPE_FWB_1201				1
#define PDEVTYPE_FWB_1210				2		//3131
#define PDEVTYPE_FWB_1220				3
#define PDEVTYPE_FWB_1221				4		//3121
#define PDEVTYPE_FWB_1222				5		//3122
#define PDEVTYPE_FWB_1230				6		//3160
#define PDEVTYPE_FWB_1240				7
#define PDEVTYPE_FWB_END				PDEVTYPE_FWB_1240
*/
									//////////////////////////////////////////////////////
									/*					FWB								*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			FWB_动作信息(fwb_action)		*/
										//////////////////////////////////////////////
typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY1;


typedef	struct {
		ushort	moduletype;						//模块类型
		ushort	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} FWB_PROTECTACTION;

										//////////////////////////////////////////////
										/*			FWB_诊断信息(fwb_detect)		*/
										//////////////////////////////////////////////
typedef	struct {
		uchar	chkbit;							//码位
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警句
} FWB_PROTECTDETECT;

							//////////////////////////////////////////////////////////////
							/*						FWB 微机保护处理类					*/
							//////////////////////////////////////////////////////////////
class FWB_Protect : public BaseProtect
{
public:
	FWB_Protect(void){};

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	FWB_PROTECTACTION	*GetActionPara(unsigned short moduletype,short actioncode);		//取保护动作参数
	FWB_PROTECTDETECT	*GetDetectPara(unsigned char chkbit);		//取自检信息参数

	int GetActionInfo(unsigned short moduletype, short actioncode,char *name,char *info);	//取保护动作描述
	int GetDetectInfo(unsigned char chkbit,char *name,char *info);	//取保护自检信息描述

    int GetFWBProtinfoTime(uchar *gram_part, SYS_CLOCK *clock);
    int GetTime(SYS_CLOCK *clock); 

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
	
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
