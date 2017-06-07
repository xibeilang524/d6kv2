#ifndef _PROT_LFP_H
#define _PROT_LFP_H

#include "scd/cmddef.h"
#include "rdb/rdbapi.h"
#include "protect.h"
#define LFP_EVTTYPE0					0
#define LFP_EVTTYPE1					1
#define LFP_EVTTYPE2					2
#define LFP_EVTTYPE3					3
#define LFP_EVTTYPE4					4
#define LFP_EVTTYPE5					5

#define LFP_ACTTYPE0					0
#define LFP_ACTTYPE1					1
#define LFP_ACTTYPE2					2
#define LFP_ACTTYPE3					3
#define LFP_ACTTYPE4					4

									//////////////////////////////////////////////////////
									/*						LFP						*/
									//////////////////////////////////////////////////////
										//////////////////////////////////////////////
										/*				LFP_模块信息(lfp_module)	*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//模块类型
		char	describe[OBJDESCRIBE_LEN];		//描述
		char	prtboard0[OBJDESCRIBE_LEN];		//保护板0名
		char	prtboard1[OBJDESCRIBE_LEN];		//保护板1名
		char	prtboard2[OBJDESCRIBE_LEN];		//保护板2名
		uchar   eventtype;						//事件报告类型
		uchar	flagtype;						//有无标志字节
		uchar   totrouble;						//同故障事件
		uchar	prtbd0tz;						//保护板0跳闸
		uchar	prtbd1tz;						//保护板1跳闸
		uchar   circuitval;						//重合闸电流等
} LFP_PROTECTMODULE;	
										//////////////////////////////////////////////
										/*				LFP_事件信息(lfp_event)		*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//模块类型
		uchar	actbit;							//码位
		char	describe[OBJDESCRIBE_LEN];		//描述
		uchar   alarmf;							//报警
		uchar	sgflag;							//事故
   		char    entname[ENTNAME_LEN];		//报警句
} LFP_PROTECTEVENT;
										//////////////////////////////////////////////
										/*				LFP_故障信息(lfp_action)	*/
										//////////////////////////////////////////////
typedef struct
{
		ushort	moduletype;						//模块类型
		uchar	boardnum;						//保护板号
		uchar	actbit;							//码位
		char	describe[OBJDESCRIBE_LEN];		//描述
		uchar   alarmf;							//报警
		uchar	sgflag;							//事故
   		char    entname[ENTNAME_LEN];		//报警句
} LFP_PROTECTACTION;

										//////////////////////////////////////////////
										/*				LFP_自检信息(lfp_detect)	*/
										//////////////////////////////////////////////
typedef	struct {
		ushort	moduletype;						//模块类型
		uchar   flagtype;						//标志类型
		uchar	chkbit;							//码位
		char	describe[OBJDESCRIBE_LEN];		//描述
		uchar   alarmf;							//报警
   		char    entname[ENTNAME_LEN];		//事项句
} LFP_PROTECTDETECT;


typedef struct {
		ushort moduletype;						//模块类型
		uchar  flag1;							//标志1
		uchar  flag2;							//标志2
	} LFP_ACTION_KEY;


							//////////////////////////////////////////////////////////////
							/*						LFP 微机保护处理类					*/
							//////////////////////////////////////////////////////////////
class LFP_Protect : public BaseProtect
{
public:
	int actiontype;
	LFP_Protect(void){};

	PROTECTMDL *GetModulePara(ushort terminalno,ushort prottype,ushort address1);
	int protectinfo(short terminalno,unsigned char protocoltype,unsigned char gram_len,unsigned char *gram);

	LFP_PROTECTMODULE  *GetLfpMdlPara(unsigned short moduletype);
	LFP_PROTECTEVENT  *GetEventPara(unsigned short moduletype,unsigned char actbit);
	LFP_PROTECTACTION *GetActionPara(unsigned short moduletype,unsigned char boardnum,unsigned char actbit);
	LFP_PROTECTDETECT *GetDetectPara(unsigned short moduletype,unsigned char flagtype,unsigned char chkbit);

	void make_event_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);	//生成事件信息--简单故障报告
	void make_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);	//生成故障信息--复杂故障报告
	int save_event_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);		//存盘事件信息--简单故障报告
	int save_action_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event);		//存盘故障信息--复杂故障报告
	int save_detect_apevent(SYS_CLOCK *sysclock,PROTECTMDL *mdlp, LFP_PROTECTMODULE *m_pLfpMdl, unsigned char *event, int m_Flag);		//保存自检事件
};

#endif
