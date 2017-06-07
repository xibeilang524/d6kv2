#ifndef _SCADA_SERVICE_H
#define _SCADA_SERVICE_H

#include "scd/cmddef.h"
#include "scadatime.h"

//////////////////////////////////////////////////////////
/*														*/
/*			微机保护应用事件结构定义(通用)				*/
/*														*/
//////////////////////////////////////////////////////////
typedef struct
{
	SYS_CLOCK   clock;							//微机保护事件发生时间
	char    name[OBJECTNAME_LEN];				//代码
    char    substation[OBJECTNAME_LEN];			//站所线系代码
   	short   protocoltype;						//保护规约类型
	short	eventtype;							//保护事件类型--动作事件、或自检信息,或纯故障报告
	short	type_value;							//保护动作类型值、或自检类型值
	uchar	sgflag;								//保护动作是否判事故
	char	enttypedescribe[OBJDESCRIBE_LEN];	//事件类型描述
	int		event_int_info[8];					//微机保护上送的其他整数信息
	float	event_float_info[8];				//微机保护上送的其他浮点数信息
	char	event_char_info[32];				//微机保护上送的其他字符串信息
}PROTECT_MSG;


///////////////////////////////////////////////////////////////////////////////
/////////////////////// 与事项有关的表 ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


// 事项句表结构
typedef	struct {
		char  entidxname[ENTNAME_LEN];		// 事项句名
		char  entexpression[ENTDESC_LEN];	// 事项表达式
		char  sndexpression[ENTDESC_LEN];	// 语音表达式
} ENTEXPRESSION;

/*
// 事项结构
typedef	struct {
		SYS_EVENT	sysevent;
		char		valuedes[OBJDESCRIBE_LEN];
		char		groupdes[OBJDESCRIBE_LEN];
		char		memberdes0[OBJDESCRIBE_LEN];
		char		memberdes1[OBJDESCRIBE_LEN];
		char		memberdes2[OBJDESCRIBE_LEN];
		char		memberdes3[OBJDESCRIBE_LEN];
		char		memberdes4[OBJDESCRIBE_LEN];
		char		memberdes5[OBJDESCRIBE_LEN];
		char		otherdes0[OBJDESCRIBE_LEN];
		char		otherdes1[OBJDESCRIBE_LEN];
		char		otherdes2[OBJDESCRIBE_LEN];
		char		otherdes3[OBJDESCRIBE_LEN];
		char		otherdes4[OBJDESCRIBE_LEN];
		char		otherdes5[OBJDESCRIBE_LEN];
		char		longdes[120];
} SCDEVENT;
*/

//沿用系统定义
#define SCDEVENT SYS_EVENT_EX

//事项类型表结构
typedef struct {
    short eventsortid; //分类号
    short eventtypeid; //类型号
    char  eventname[OBJECTNAME_LEN]; //事项名
    char  describecode[OBJECTNAME_LEN]; //事项句名
} EVENTTYPE;

//事项状态描述表结构
typedef struct {
//	short eventclassid;
    short eventtypeid;
    short statevalue;
    char  statedescribe[OBJDESCRIBE_LEN];
} EVENTSTATEDESC;

/*
//事项模式表
typedef struct {
    short  mode;
    char   describe[OBJDESCRIBE_LEN];
	uchar  notmakef;
	uchar  security;
	uchar  savediskf;
	uchar  printf;
	uchar  logf;
	uchar  alarmf;
	uchar  popgraf;
	char   entname[ENTNAME_LEN];	//  事项句
	char   almgraname[GRAP_LEN];	//  推图名
} EVENTMODE;
*/

/******************************************************************/
/*********************  事项处理方式参数表  ***********************/
/******************************************************************/
typedef struct {
	    char    name[OBJECTNAME_LEN];//代码
        char    describe[OBJDESCRIBE_LEN];//描述
		uchar	jgprintf;//警告打印
		uchar	jglogf;//警告显示
		uchar	jgalarmf;//警告音响
		uchar	jgalarmmodef;//警告音响方式
		uchar	jgpopgraf;//警告推图
		uchar	jgsavef;//警告存盘
		uchar	jgconfirmf;//警告确认方式

		uchar	jbprintf;//警报打印
		uchar	jblogf;//警报显示
		uchar	jbalarmf;//警报音响
		uchar	jbalarmmodef;//警报音响方式
		uchar	jbpopgraf;//警报推图
		uchar	jbsavef;//警报存盘
		uchar	jbconfirmf;//警报确认方式
		char    almtelno[OBJECTNAME_LEN];//报警电话
		uchar	eventlevel;
}  EVENTDEALMODE_STRU;


//event control struct
typedef	struct  {
		int  staval;						// 状态值
		char describe[OBJDESCRIBE_LEN];		// 描述
} SCDENTDEFSTR;

 
// 事项类

class 
#ifdef RDB_PLATFORM_WIN32
 __declspec(dllexport)
#endif
ScdComnEvent: public NetDbg{
	protected:
		static int			opentabf;
		static	Rdb_Tab		eventquetab;		// 事项队列表
		static	Rdb_Tab		entexpressiontab;	// 事项句表
		static	Rdb_Tab		enttypetab;			// 事项类型表
		static	Rdb_Tab		entstatusdestab;	// 事项状态描述表结构	
		static	Rdb_Tab		eventdealmodetab;	// 事项处理方式表结构
		char	*curexpression;					// 当前表达式指针
	public:
		ScdComnEvent(void);
		~ScdComnEvent(void);
		// 取得分裂的字串
		int		get_splitdefstr(char *srcstr,char* keystr,SCDENTDEFSTR *defstr);
		// 取得事项文字表达式
		char	*get_entexpression(char  *entidxname);
		// 取得事项语音表达式
		char	*get_sndexpression(char  *entidxname);
		// 产生事项文字描述
		int		yield_entdescribe(char* entname,SCDEVENT  *eventp,char firstflag=0);
		// 产生事项语音描述
		int		yield_snddescribe(char* entname,SCDEVENT  *eventp);
		// 根据事项类型、事项状态值取得事项状态描述
		int		GetStavalueDes(short eventtype, short stavalue, char *description);
		// 根据事项类型取得事项句名
		int		Get_EventExpressName(short eventtype, char *eventname);
		// 根据事项句中的关键字取得声音文件名
		int		GetToneFileName(SYS_EVENT *eventp, char *keystr, char *filename);
// 根据事项模式
//		EVENTMODE *GetEventMode(short mode);
		EVENTDEALMODE_STRU *GetEventDealMode(char* name);
		//
		int		save_event(SCDEVENT *eventp,int nums);
		int		add_userdef_event(short groupno,short entsort,short enttype,short stval,short stval1,float floatval,char *group_des,char * value_des,char *member_name0,char *member_des0,char *member_des1,char *member_des2,char *otherdes0,char *otherdes1,char *entname);
};

///////////////////////////////////////////////////////////////////////
#endif
	
