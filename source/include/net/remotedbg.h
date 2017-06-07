#if !defined (_REMOTE_DBG_HEAD)
#define _REMOTE_DBG_HEAD

#include "netapi.h"

#define MAX_REMOTE_DBG_NUM		2

/*---------------------------------------*/

typedef struct _dbg_proc_info
{
	int32u	pid;
	int8u   regidx;
	char	proc_asc[DESC_LEN];				//½ø³ÌÃû³Æ
	int8u   typenum;
	struct _type
	{
		int16s	type;
		char	desc[DESC_LEN];
	}type[DEBUG_MAX_TYPE];
}DbgProcInfo;

typedef struct _remote_dbg_proc
{
	int16u			proc_num;
	DbgProcInfo*	proc;
}REMOTE_DBG_PROC;

/*---------------------------------------*/

typedef	struct _dbg_proc_data
{
	char	type[DESC_LEN];
	char	desc[DEBUG_MAX_LEN];
}DbgProcData;

typedef struct _remote_dbg_data
{
	int8u			proc_idx;
	DbgProcData		data;
}REMOTE_DBG_DATA;

/*---------------------------------------*/

typedef struct _remote_dbg_sel
{
	int8u	proc_idx;
	int16s	typenum;
	int16s* type;
}REMOTE_DBG_SEL;

#endif
