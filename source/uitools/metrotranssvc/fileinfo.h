#ifndef FILEINFO_H
#define FILEINFO_H
#include <QListWidget>

#include "db/dbapi.h"
#include "rdb/rdbapi.h"

#include "globaldef.h"


//触发条件表
typedef struct
{
	int	No;									//序号
	char	code[CODE_LEN];			//遥信代码
	unsigned char	state;				//遥信状态
	int	blockcond_no;					//闭锁条件号
	int	action_no;						//动作组号
	unsigned char real_state;	
}TriggerCond;

//闭锁条件表
typedef struct
{
	int	block_no;							//闭锁条件号
	int	No;									//序号
	char	code[CODE_LEN];			//遥信代码
	char	table[CODE_LEN];			//遥信表代码
	unsigned char	state;				//遥信状态
	unsigned char real_state;	
}BlockCond;

//动作表
typedef struct
{
	int	groupno;							//组号
	int	No;									//序号
	int	oper_type;						//操作类型(0:yk   1:yx)
	char	code[CODE_LEN];			//遥信代码
	char	table[CODE_LEN];			//遥信表代码
	unsigned char	state;				//遥信状态
	unsigned char real_state;
	char msg[255];
}ActionInfo;


class File_info
{
public:
	int Init();

	int read_triggercond_para();									//读取触发条件表
	int read_blockcond_para(int blockcondno);			//读取闭锁条件表
	int read_action_para();												//读取动作表

	int read_action_realstate();		//读动作列表当前实时值
	int read_blockcond_realstate();		//读闭锁列表的当前实时值

	int get_actionlist_by_actionno(int actionno);		//获取当前触发条件的的控制序列，返回序列数量
	File_info();
	virtual ~File_info();

	TriggerCond  *m_faultcond_list;			//触发条件列表
	BlockCond  *m_blockcond_list;			//闭锁条件列表
	ActionInfo  *m_action_list;					//动作列表
	int		m_triggercond_num;		//触发条件数量
	int		m_action_num;				//序列控制数量
	int		m_blockcond_num;			//闭锁条件数量
	ActionInfo  *m_pControlList;			//组号的控制序列
	int	m_pControlNum ;
};
#endif