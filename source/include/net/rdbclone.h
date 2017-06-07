#if !defined(RDB_MIRROR_HEAD)
#define RDB_MIRROR_HEAD

#include "sysdef.h"
#include "rdb/ldb.h"
#include "rdb/rdbapi.h"

#define MAX_VAL_LEN			12
#define MIRROR_MAX			(GROUP_MAX*RTDB_MAX*(SERVER_NODE_MAX-1))

#define OBJ_STATE_NOSTART	0
#define OBJ_STATE_START		1
#define OBJ_STATE_RUN		2
#define OBJ_STATE_STOP		3
#define OBJ_STATE_EXIT		4


#define RDB_MEM_COL_PARA	1	
typedef struct {
	int		grpno;
	char	type[CODE_LEN];
	char	table[CODE_LEN];
	char    paracol;
//	int16u  streamlen;
//	void*   streamval;  //rcdno(int16u),colno(int16u),val;......;rcdno,colno,val
	int		oldrcdnum;
	void*   oldval;
} MIRROR_DATA;

typedef struct _mirror_table_para
{
	int		rcdnum;
	MIRROR_DATA *pData;
}MIRROR_TABLE_PARA;

#define ALL_DATA_FUNC		1
#define STREAM_DATA_FUNC	2
#define TABLE_ALLDATA_FUNC	3
#define NULL_DATA_FUNC      4

typedef struct _rdb_mirror_head
{
	int16u  func;
	char	table[CODE_LEN];
	int16u  rcd_len;
	int16u	col_num;
	int32u  rcd_num;
}MIRROR_HEAD;

#endif