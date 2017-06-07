#ifndef _PROT_NEWZDB_H
#define _PROT_NEWZDB_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "scadatime.h"
#include "protect.h"
		
									//////////////////////////////////////////////////////
									/*					NEWZDB						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*			NEWZDB事件信息参数表				*/
										//////////////////////////////////////////////
typedef	struct {
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} NEWZDB_PROTECTACTION;


										//////////////////////////////////////////////
										/*			IEC103事件信息参数表				*/
										//////////////////////////////////////////////
typedef	struct {
		short devicetype;						//装置类型
		short	actioncode;						//序号
		char	actioninfo[PROTINFO_LEN];		//描述
		uchar   alarmf;							//是否报警	
		uchar	sgflag;							//是否判事故
    	char    entname[ENTNAME_LEN];		//报警句
} IEC103_PROTECTACTION;
	
typedef struct {
		ushort devicetype;						//装置类型
		ushort  actioncode;						//序号
	} IEC103_ACTION_KEY;

						//////////////////////////////////////////////////////////////
							/*						NEWZDB 微机保护处理类			*/
							//////////////////////////////////////////////////////////////
class NEWZDB_Protect : public BaseProtect
{

public:
	NEWZDB_Protect(void);
	ushort address1,address2;//主站地址，从站地址
	int m_GramLength;//报文长度
	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1,ushort address2);
	NEWZDB_PROTECTACTION	*GetActionPara(short actioncode);		//取事件信息参数
	IEC103_PROTECTACTION	*GetIEC103ActionPara(unsigned short devicetype,unsigned short actioncode);

	unsigned char m_Buf[1024];
	//解释FEP服务器保护生报文
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	//时间转换
	int GetProtinfoTime(uchar *gram_part,uchar binorbcd,SYS_CLOCK *clock);
	//生成应用事件
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);

	//生成存盘事件
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, unsigned char *event);
};

#endif
