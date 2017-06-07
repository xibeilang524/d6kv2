#ifndef _PROT_ZDB_H
#define _PROT_ZDB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					ZDB						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			ZDB动作信息参数表				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actiontype;						//模块类型
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} ZDB_PROTECTACTION;

										//////////////////////////////////////////////
										/*			ZDB自检信息参数表			*/
										//////////////////////////////////////////////
typedef	struct {
		short	detectcode;						//序号
		char	detectinfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警
   		char    entname[ENTNAME_LEN];		//报警事项句
} ZDB_PROTECTDETECT;

										//////////////////////////////////////////////
										/*			ZDB测量类型参数表			*/
										//////////////////////////////////////////////
typedef	struct {
		short	measurecode;					//序号
		char	measureinfo[PROTINFO_LEN];		//描述
		float   measurepoint;					//系数
		char	measureunit[PROT_UNIT_LEN];		//单位
} ZDB_PROTECTMEASURE;

typedef struct 
{
		ushort   moduletype;
		ushort	actioncode;
} ACTION_KEY_ZDB;



							//////////////////////////////////////////////////////////////
							/*						ZDB 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class ZDB_Protect : public BaseProtect
{

public:
	ZDB_Protect(void);
	ushort address1,address2;//主站地址，从站地址
	int m_GramLength;//报文长度
	unsigned char m_Buf[1024];
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	ZDB_PROTECTACTION	*GetActionPara(short actiontype,short actioncode);		//取保护动作参数
	ZDB_PROTECTDETECT	*GetDetectPara(short detecttype);		//取自检信息参数
	ZDB_PROTECTMEASURE	*GetMeasurePara(short measurecode);		//取量测信息参数

	int GetDetectInfo(short detecttype,char *name,char *info);	//取保护自检信息描述

	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//时间转换
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
