#ifndef _PROT_SEL_H
#define _PROT_SEL_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"


									//////////////////////////////////////////////////
									/*				S E L模块定义					*/
									//////////////////////////////////////////////////


#define PDEVTYPE_SEL_251				0
#define PDEVTYPE_SEL_501				1
#define PDEVTYPE_SEL_587				2
#define PDEVTYPE_SEL_551				3
#define PDEVTYPE_SEL_287				4
#define PDEVTYPE_SEL_387				5
#define PDEVTYPE_SEL_351				6


// 模块类型（保护类型）定义

#define PTYPE_MTMAIN					1	//主变主保护
#define PTYPE_MTBACK_110KV				2	//主变后备保护(110KV侧)
#define PTYPE_MTBACK_35KV				3	//主变后备保护(35KV侧)
#define PTYPE_MTBACK_10KV				4	//主变后备保护(10KV侧)
#define PTYPE_MTMIDZERO					5	//主变中型点零序保护
#define PTYPE_MTZTYPE					6	//主变Z型变保护
#define PTYPE_SUBSECTION				7	//分段保护
#define PTYPE_BUS_35KV					8	//35KV母线保护
#define PTYPE_CAPACITOR_V				9	//电容器电压保护
#define PTYPE_CAPACITOR_I				10	//电容器电流保护
#define PTYPE_LINE						11	//线路保护
				

									//////////////////////////////////////////////////
									/*					S E L						*/
									//////////////////////////////////////////////////
										//////////////////////////////////////////
										/*			SEL_动作信息(sel_action)	*/
										//////////////////////////////////////////
typedef	struct {
		short	actioncode;		//序号
		short	actiontype;		//动作类型
		short	circletype;		//接线类型
		char	actioninfo[PROTINFO_LEN];	//描述
		uchar   alarmf;			//报警
		uchar	sgflag;			//事故
    	char    entname[ENTNAME_LEN];	//报警句
} SEL_PROTECTACTION;


							//////////////////////////////////////////////////////////////
							/*						SEL 微机保护处理类					*/
							//////////////////////////////////////////////////////////////
class SEL_Protect : public BaseProtect
{
	static Rdb_Tab protacttab;
	static int opentabf;
public:
	SEL_Protect(void);

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort adddress1,ushort address2);
	int GetActionType(short devtype,short moduletype,char *action_string,char *target,ushort *actiontype,int totalnums);
	SEL_PROTECTACTION	*GetActionPara(ushort actiontype,ushort circletype);
	int GetActionInfo(ushort actiontype,ushort circletype,char *name,char *info);

	int  GetProtinfoTime(uchar *gram_part, SYS_CLOCK *clock);
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);
};
 

#endif
