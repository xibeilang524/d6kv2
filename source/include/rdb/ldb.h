#ifndef  __ldb_h
#define  __ldb_h

#include  "rdbapi.h"

#define RDBUSERTYPE_NORMAL		  0
#define RDBUSERTYPE_ROOT		  1
#define RDBUSERTYPE_ONLYREAD	  2
#define RDBUSERTYPE_INTERUSER	  3

typedef struct {
	char username[RDB_USERNAME_LEN];
	int  login_user_no;
	char groupname[RDB_USERNAME_LEN];
	uchar usertype;
}  RDB_USER_HANDLE_STRU;




/////////////////// Dictionary Manager ///////////


#define  RDB_MAX_TABLE_NUMS				800		// maxium number of table

#define  RDB_MAX_COLUMN_NUMS			18000   // maxium number of column

#define  RDB_LOAD_TRIG_NUMS				3

typedef struct {
	int  table_count;		// the used table count
    int  column_count;		// the used column count
}  RDB_DICTMAN_STRU;


#define RDBDIC_TABTYPE_REAL     0
#define RDBDIC_TABTYPE_MIRROR   1
#define RDBDIC_TABTYPE_HALT     2

typedef struct {
	uchar  tabletype; // 0: 实表  1: 镜像表  2: 不属于本机的表
	short  groupno;                          //rcd group no
	char   name[RDB_TABNAME_LEN];			// table simple  name
	char   full_name[RDB_TABFULLNAME_LEN];    // table full  name
	short  table_id;                         // the number of table ID 
	char   lib_name[RDB_TABNAME_LEN];
	char   server_name[RDB_USERNAME_LEN];
	char   owner_name[RDB_USERNAME_LEN];     // table's owner simple name
//	uchar   group_read;					// table's opration right
//	uchar   group_write;
//	uchar   world_read;
//	uchar   world_write;
	int   max_rcdcount;                     // the maxium of record number in the table
	int   cur_rcdcount;						// current record number
	int   record_len;						// the record lenght 
	int   column_count;
    int   open_count;						// total number of opening the table
	uchar  key_flag;						// key avail flag
	uchar  key_column_nums;					// key  column number
	int   key_column[RDB_KEYCOL_NUMS];	// column orderno of key
	uchar  key_datatype[RDB_KEYCOL_NUMS];
	int   key_offset[RDB_KEYCOL_NUMS];
	int   key_rcdlen;						// key record length
	int   key_border;
	int   start_column_no;					//  start column no
	int   data_seg_no;	// table data and key  segment order number
    int   data_start_off;   // table data start offset in RDB
	int   data_mem_len;     // table data length
	int   key_seg_no;
	int   key_start_off;    // table key start offset in RDB
	int   key_mem_len;      // table key length
	uchar  backup_mode;
	char    para_table_name[RDB_TABFULLNAME_LEN];
	char    data_table_name[RDB_TABFULLNAME_LEN];
	char    trig_table_name[RDB_LOAD_TRIG_NUMS][RDB_TABFULLNAME_LEN];
	uchar		backuptime;
	intertime  lastbackuptime;
	intertime  lastloadtime;
	intertime  laststructtime;
	ushort lockcount;
	uchar  lock_flag;
	uchar  havemirror;
	uchar	ismainservflag;
	uchar	requiredinitflag;
	char   parawhere[255];
	char   datawhere[255];
}  RDB_TAB_STRU;


typedef struct {
	char name[RDB_COLNAME_LEN];		// column simple name
	char full_name[RDB_COLFULLNAME_LEN];	// column full name
	int    offset;						// column offset in record
	ushort data_len;						// column data length
	uchar  data_type;					// column data type
	uchar  position;	
	uchar  bit_nums;
	uchar  backup_type;
	uchar  eventflag;
	short  rawcolno;
	uchar  havemirror;
}  RDB_COL_STRU;

#define COLUMN_BACKUP_STATIC        1
#define COLUMN_BACKUP_DYNAMIC		2


inline void SetTableStructTime(RDB_TAB_STRU *rdbtblp) {
	Rdb_Time   settbl_rdbtime;
	settbl_rdbtime.RdbGetIntertime(&rdbtblp->laststructtime);
}

inline void rdbcolumn_columninfo(RDB_COL_STRU *rdbcolumn,RDB_BTBL_COL_STRU *colinfo) {
		strcpy(colinfo->name,rdbcolumn->name);
		strcpy(colinfo->full_name,rdbcolumn->full_name);
		colinfo->offset=rdbcolumn->offset;
		colinfo->unitlen=rdbcolumn->data_len;
		colinfo->datatype=rdbcolumn->data_type;
		colinfo->position=rdbcolumn->position;
		colinfo->bitnums=rdbcolumn->bit_nums;
}

//////////////// Data segment manager ///////////

#define   RDB_MAX_DATASEG_NUMS       32      // data share memory segment number

#define   RDB_DATASEG_TYPE_NORMAL    0
#define   RDB_DATASEG_TYPE_INDEX     1
#define   RDB_DATASEG_TYPE_SPEC1     2

typedef  struct {
	int   segment_nums;
	struct {
	   int segment_id;
	   int dataseg_type;
	   int  max_data_len;		// maxium length of database data
	   int  spare_data_off;		// spare space offset 
       int  spare_data_len;		// spare space length
	   }  segment_ctrl[RDB_MAX_DATASEG_NUMS];
} RDB_DATASEGMAN_STRU;

#define  RDB_DATASEG_MEMID             3735210  // to RDB_DATASEG_MEMID+RDB_MAX_DATASEG_NUMS





///// class Rdb_DataSegmentManager : process Dorbase data segment ////////

#define  RDB_DATASEG_MALLOC_MODE_SMALL		1
#define  RDB_DATASEG_MALLOC_MODE_LARGE		2

void SetDataSegMallocMode(int mode);

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_DataSegmentManager {
	static RDB_DATASEGMAN_STRU   *dataseg_man;
	static char  *dataseg_addr[RDB_MAX_DATASEG_NUMS];
	int   MappingInit(void);
public:
	Rdb_DataSegmentManager(void);
	int   MappingDataSeg(void);
	int   MallocMem(int len,int dataseg_type,int *offset);
    void *GetMemAddr(int segno,int offset);
	void  DisplayMemInfo(void);
};


///////// class RdbUserConf: process Dorbase user config //////

#define  RDB_MAX_USER_NUMS			   1000


typedef struct {
	int user_count;
}  RDB_USERMAN_STRU;

typedef struct {
	char  name[RDB_USERNAME_LEN];           // user simple name
	char  full_name[RDB_USERFULLNAME_LEN];   // user full name
	char  group_name[RDB_USERNAME_LEN];      // group simple name 
	char  password[RDB_PASSWD_LEN];        // user's password
	uchar usertype;
	uchar reserve[3];
}  RDB_USER;



#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_UserManager {
	static RDB_USERMAN_STRU  *user_man;
	static RDB_USER      *user_addr;
	static Rdb_Bptree         user_name_bptree;
public:
	Rdb_UserManager(void);
	RDB_USER   *SearchUser(char *name,int *login_no=NULL);
	int AddUser(RDB_USER *userp);
	int RdbGetUserInfo(char *name,char *full_name=NULL,char *group_name=NULL,uchar *usertype=NULL);
	int CheckPassword(char *name,char *password,int *user_no=NULL);
};


///// class Rdb_DictManager : process Dorbase local dictionary


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_DictManager {
	static RDB_DICTMAN_STRU *dict_man;
	static RDB_TAB_STRU   *table_addr;
	static RDB_COL_STRU   *column_addr;
	static Rdb_Bptree  table_name_bptree;
	static Rdb_Bptree  table_id_bptree;
public:
	Rdb_DictManager(void);
	int			RdbGetTabNums(void);
	int		AddTableStruct(RDB_TAB_STRU *addtable);
	RDB_COL_STRU   *Rdb_MallocColumn(int  columnnums,int *start_column_no);
	RDB_TAB_STRU    *GetTableStruct(int table_no);
	RDB_TAB_STRU    *SearchTable(char *name,int *table_no=NULL);
	RDB_TAB_STRU    *SearchTable(int tabid,int *table_no=NULL);
	RDB_COL_STRU   *SearchColumn(char *tabname,char *columnname);
	RDB_COL_STRU   *GetColumnStruct(int column_no);
	void	CreateMirrorColInfo(void);
	void	FreshMainServFlag(void);
	void DisplayDictionaryInfo(void);
};



///// class Rdb_LdbManager: manager local dictionary,data,key etc./// 


typedef struct {
	short  groupno;                          //rcd group no
	char   name[RDB_TABNAME_LEN];			 // table simple  name
	char   full_name[RDB_TABFULLNAME_LEN];   // table full  name
	short  table_id;                         // the number of table ID 
	char   lib_name[RDB_TABNAME_LEN];
	char   server_name[RDB_USERNAME_LEN];
	char   owner_name[RDB_USERNAME_LEN];     // table's owner simple name
//	uchar   group_read;					     // table's opration right
//	uchar   group_write;
//	uchar   world_read;
//	uchar   world_write;
	int     max_rcdcount;                    // the maxium of record number in the table
	char    key_column_name[RDB_KEYCOL_NUMS][RDB_COLNAME_LEN];
	char    para_table_name[RDB_TABFULLNAME_LEN];
	char    data_table_name[RDB_TABFULLNAME_LEN];
	char    trig_table_name[RDB_LOAD_TRIG_NUMS][RDB_TABFULLNAME_LEN];
	uchar	backuptime;
//	uchar	querymode;
	char    parawhere[255];
	char    datawhere[255];
}  RDB_TAB_MODE_STRU;


typedef struct {
	char name[RDB_COLNAME_LEN];		// column simple name
	char full_name[RDB_COLFULLNAME_LEN];	// column full name
	char table_name[RDB_TABNAME_LEN];
	ushort data_len;						// column data length
	uchar  data_type;					// column data type
	uchar  bit_nums;
	uchar  backup_type;
	short  col_order;
	short  disp_order;
	uchar  dispflag;
	uchar  eventflag;
}  RDB_COL_MODE_STRU;


typedef struct {
	char  name[RDB_USERNAME_LEN];           // user simple name
	char  full_name[RDB_USERFULLNAME_LEN];   // user full name
	char  group_name[RDB_USERNAME_LEN];      // group simple name 
	char  password[RDB_PASSWD_LEN];        // user's password
}  RDB_USER_MODE;

/*
typedef struct {
	short   mirrortype;
	char	tabname[RDB_TABNAME_LEN];
	char	columnname[RDB_COLNAME_LEN];
}  RDB_MIRROR_TABMODE;


typedef struct {
	char	hostname[RDB_HOSTNAME_LEN];
	short   mirrortype;
}  RDB_NODE_MIRRORTAB;
*/

#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_LdbManager : public Rdb_DictManager,Rdb_DataSegmentManager,Rdb_UserManager {
public:
//	Rdb_LdbManager(void);
	// add a table
	int  AddTable(uchar tabletype,RDB_TAB_MODE_STRU *instblp,int columnnums,RDB_COL_MODE_STRU *inscolumnp);
	int	 AddUserMode(RDB_USER_MODE *insuser,uchar usertype);
};


/////////////////////////////////////////////////////////////////////////////////////////////

#define  MIRROR_TASK_QUEUE_NUMS         100000

#define  MIRROR_TASKID_REQINITTAB		1
#define  MIRROR_TASKID_INITTAB			2
#define  MIRROR_TASKID_SYNCRECORD		3
#define  MIRROR_TASKID_SYNCCOLUMN		4
#define  MIRROR_TASKID_ADDRCD			5


typedef struct {
	uchar  crquememflag;
	uchar  lockflag;
	int quesegno;
	int queoffset;
	int put_que_no;
	int get_que_no;
	uchar  crmcolinfoflag;
	int colsegno;
	int coloffset;
	unsigned int  mirrorfreshcount;
	uchar  reqinitflag;
} RDB_MIRRORTASK_CTRL_STRU;

typedef struct {
	int   rcdno;
	short tableno;
	short colno;
	uchar taskid;
	uchar procflag;
} RDB_MIRRORTASK_STRU;



#ifdef RDB_PLATFORM_WIN32
RDB_MIRRORTASK_CTRL_STRU __declspec(dllexport) *
#else
RDB_MIRRORTASK_CTRL_STRU	*
#endif
GetMirrorTaskCtrlAddr(void);


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_MirrorTask {
	static RDB_MIRRORTASK_CTRL_STRU	*mquectrl;
	static RDB_MIRRORTASK_STRU		*mquestru;
	void LockTaskQue(void);
	void UnlockTaskQue(void);
	void CreateMirrorMem(void);
	void MapMirrorMem(void);
public:
	Rdb_MirrorTask(void);

	int GetMirrorTask(RDB_MIRRORTASK_STRU *mtp);
	int PutMirrorTaskInQue(uchar taskid,short tableno,int rcdno,int colno);
};

#ifdef RDB_PLATFORM_WIN32
int  __declspec(dllexport)
#else
int
#endif
SetMirrorTask(uchar taskid,short tableno,int rcdno,int colno);


typedef struct  {
	int   tabid;
	int	  rcdno;
	uchar taskid;
	short colnums;
	int   rcdnums;
	int   rcdlen;
	char  reservd[4]; //8 byte 对齐
} RDB_MIRRORNET_DATAHEAD_STRU;

typedef struct  {
	int   offset;
	short colno;
	uchar datatype;	 //8 byte 对齐
} RDB_MIRRORNET_COL_STRU;


#ifdef RDB_PLATFORM_WIN32
class __declspec(dllexport)
#else
class
#endif
Rdb_MirrorNetData {
	static RDB_MIRRORTASK_CTRL_STRU	*mtaskctrl;
	static char 			*netdata_addr;
	void			*GetMirrorNetData_SyncRecord(RDB_MIRRORTASK_STRU  *mtaskp,int *datalen);
	int				PutMirrorNetData_SyncRecord(RDB_MIRRORNET_DATAHEAD_STRU  *tmpdatahead,void *data,int datalen);
	void			*GetMirrorNetData_SyncColumn(RDB_MIRRORTASK_STRU  *mtaskp,int *datalen);
	int				PutMirrorNetData_SyncColumn(RDB_MIRRORNET_DATAHEAD_STRU  *tmpdatahead,void *data,int datalen);
public:
	Rdb_MirrorNetData(void);
	void			*GetMirrorNetData_ReqInitMirrorTable(int *datalen);
	int				PutMirrorNetData_ReqInitMirrorTable(RDB_MIRRORNET_DATAHEAD_STRU  *tmpdatahead,void *data,int datalen);
	void *GetMirrorNetData(RDB_MIRRORTASK_STRU  *mtaskp,int *datalen);
	int   PutMirrorNetData(void *data,int datalen);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	char  tabname[RDB_TABNAME_LEN];
	char  colname[RDB_COLNAME_LEN];
}	RDB_MIRROR_COLMAN_STRU;


class Rdb_MirrorColKeyBptree : public Rdb_Bptree {
public:
	int RdbCompareKey(void *keydp1,void *keydp2) 
	{
		int ret;
		ret=strcmp(((RDB_MIRROR_COLMAN_STRU *)keydp1)->tabname,((RDB_MIRROR_COLMAN_STRU *)keydp2)->tabname);
		if(ret!=0)return(ret);
		return(strcmp(((RDB_MIRROR_COLMAN_STRU *)keydp1)->colname,((RDB_MIRROR_COLMAN_STRU *)keydp2)->colname));
	}
};

class Rdb_MirrorColManager {
	static int  mcolbptree_initflag;
	static RDB_MIRRORTASK_CTRL_STRU	*mtaskctrl;
	static Rdb_MirrorColKeyBptree  mirror_col_bptree;
public:
	Rdb_MirrorColManager(void);
	int		InsertMirrorColInfo(char *tabname,char *colname,int colno);
	int		GetMirrorColNo(char *tabname,char *colname);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////


#define RDBMEM_DATASEG_MAN_OFFNO			0
#define RDBMEM_USER_MAN_OFFNO				1
#define RDBMEM_USER_OFFNO					2
#define RDBMEM_USER_BPTREE_OFFNO			3
#define RDBMEM_DICT_MAN_OFFNO				4
#define RDBMEM_COLUMN_OFFNO					5
#define RDBMEM_TABLE_OFFNO					6
#define RDBMEM_TABNAME_BPTREE_OFFNO			7
#define RDBMEM_TBLID_BPTREE_OFFNO			8
#define RDBMEM_MIRROR_TASK_OFFNO			9
#define RDBMEM_EVENT_SYSCTRL_OFFNO			10


#define  RDB_SERVER_MEMID		    3735200
#define  RDB_SERVER_MEMOFF_NUMS     50

typedef struct {
	uchar      havemirror;
	int		   reserved[40];
}  RDB_SERVER_MEMHEAD_STRU;



class  Rdb_SrvMemManager {
	static  char  *serv_mem_addr;
	static  RDB_SERVER_MEMHEAD_STRU  *memhead;
	static  int   mem_offset[RDB_SERVER_MEMOFF_NUMS];
	static  int   mem_length[RDB_SERVER_MEMOFF_NUMS];
	static  int   mem_total_length;
	int		CreateMem(void);
	int		MappingMem(void);
public:
	int     error_ret;
	Rdb_SrvMemManager(int initf=0);
	void *GetMemAddr(int offset_no);
};

#endif
