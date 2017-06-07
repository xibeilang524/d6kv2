#ifndef	NET_STR_H
#define	NET_STR_H

#include <time.h>
#include "netdef.h"

//extern IMEXPORT parent_id parent;
/*
#if defined (__unix)
struct
{
#if defined(__alpha)
	mtyp_t mtype;
#elif defined(__sun)
	long mtype;
#endif
	DATETIME data;
}msg_buf;
#endif
*/
/*--------系统信息-------*/

typedef struct
{
	floats cpu_load,net_load;			//使用率
	int32u disk_all,disk_occu,mem_all,mem_occu,db_all,db_occu,net_base,net_rate;
}HOST_SYS_INFO;

/*-----------------------*/

typedef struct
{
	char hostname[HOSTNAMELEN];
	char name[CODE_LEN];
	struct _host_neta_netb_t
	{
		struct in_addr ip_addr;
		int8u new_flag;
		int8u cur_flag;
		int8u active;
		int8u count;
		int8u new_flag_bak;
		int8u count_bak;
	}net_a,net_b,net_a_v,net_b_v;
	int8u grpno;
	int8u atbtime;
	int8u stoptime;
	int8u type;
	int8u vstate;
	int8u rstate;
	int8u area3flag;
	int8bit attr1,attr2,attr3;
	HOST_SYS_INFO	sys_info;
}HOST;

typedef struct _mail_head_t
{
	char host[HOSTNAMELEN];
	char user[USER_NAME_LEN];
	MAILPROCTYPE pid;
	_mail_head_t(void) {}
}MAIL_HEAD;

typedef struct _head_t
{
	MAIL_HEAD	src;
	int8u		grpno;
	int16u		cmdtype;
	int8u		userdata1,userdata2;
	int32u		len;
	_head_t()	{;}
}HEAD;

struct COVER
{
	MAIL_HEAD	src,trg;
	int			type,len,count,flag,rx_type;
};

struct GRAM_HEAD
{
	int8u  grpno;
	int16u type;
	int16u netno;
	char srcname[CODE_LEN];
	char trgname[CODE_LEN];
	char username[USER_NAME_LEN];
	int16u cmdtype;
	int32u len;
};

/*----------------------------报文类型-------------------------*/

/*---------注册报文----------*/

struct REG_GRAM
{
	HEAD head;
	int32u time;
};

//平安报文
struct ATB_GRAM
{
	HEAD head;
	int32u time;
};

/*----------状态报文-----------*/

struct STATE_GRAM
{
	HEAD head;
	struct stategram_state_t
	{
		int8u neta_f;
		int8u netb_f;
		int8u rstate;
		int8u res;
	}state[HOST_MAX];
	struct server_state_t
	{
		char main_server[HOSTNAMELEN];
	}server_state[RTDB_MAX];
};

/*--------设备信息报文---------*/

struct DEVICEINFO_GRAM
{
	HEAD head;
	char name[CODE_LEN];
	int8u status;
};

/*--------校时报文--------------*/

struct DATETIME
{
	int16u year,mon,day;
	int16u hour,min,sec;
	int16u ms,res;
};

struct SYNCTIME_GRAM
{
	HEAD head;
	DATETIME data;
};

/*--------切换主机报文-----------*/

struct SWITCH_HOST
{
	int16u type;
	int16u host;
};

/*--------系统信息报文----------*/

struct HOST_SYS_GRAM
{
	HEAD head;
	HOST_SYS_INFO info;
};

//////////////////////////////////////////

struct EventStruct
{
	int num;
	SYS_EVENT* buf;
};

/*-------------------------------------------------------------------*/

struct NODE_INFO
{
	int flag;
	int type;
	int netno;
	char srcname[CODE_LEN];
	char trgname[CODE_LEN];
	char username[USER_NAME_LEN];
	char src_proc[PROC_NAME_LEN];
	char trg_proc[PROC_NAME_LEN];
	int cmdtype;
	time_t start_time; //最近登陆时间
	int total_sum;
	int total_len;
	
	int total_recv_sum;
	int total_recv_len;
	int recv_err_sum;
	int recv_err_len;
	int recv_ok_sum;
	int recv_ok_len;

	int total_send_sum;
	int total_send_len;
	int send_err_sum;
	int send_err_len;
	int send_ok_sum;
	int send_ok_len;

	int trg_pid;
	int src_pid;
};

/*-----------------------------网络数据库定义-----------------------------*/
struct PORT_DB
{
	int8u   portnum;
	struct _port
	{
		int16u	port;
		int8s	type;
	}port[PORT_NUM_MAX];
};

struct NET_DB
{
	int16u	hostnum;
	int8s	data_net,cmd_net;
	int32u	time;
	HOST	host[GROUP_MAX*HOST_MAX];
};

struct RTDB_DB
{
	int8u  grpnum;
	struct grp_t
	{
//		int8u  grpno;
		int16u dbnum;
		struct dt_t
		{
			char dbname[RTDB_NAME_LEN];
			char dbdesc[DESC_LEN];
			char node[SERVER_NODE_MAX][HOSTNAMELEN];
			char mirror[HOST_MAX][HOSTNAMELEN];
			char main_node[HOSTNAMELEN];
			int8u mirrornum;
			int16u procnum,ok,nodenum;
			struct srvproc_t
			{
				char name[PROC_NAME_LEN];
				char ch_name[PROC_NAME_LEN];
				char par1[PROC_NAME_LEN];
				char par2[PROC_NAME_LEN];
				char par3[PROC_NAME_LEN];
				char par4[PROC_NAME_LEN];
				char par5[PROC_NAME_LEN];
				int8u start_time;			//启动次数
				int8u flag;					//正常计数
				int8u auto_start;			//启动方式
				int8u curtime;				//当前启动方式
				int8u starthour;			//定时启动小时
				int8u startmin;				//定时启动分钟
				int8u startsec;
				int8u startflag;			//启动标志
				int8u res;					//保留
			}srvpro[SRV_PROC_MAX];
		}db[RTDB_MAX];
	}grp[GROUP_MAX];
};		//一组实时库

//邮件库
struct IMEXPORT MAIL_DB
{
	int writep,count;
	char lock;
	struct mail_t
	{
		COVER cover;
		char data[MAIL_LEN_MAX];
	}mail[MAIL_NUM_MAX];
};

struct IMEXPORT PROC_DB
{
	struct proc_db_proc_t
	{
		MAILPROCTYPE pid;
		int16u flag,ProcCount;
		int32u start_time;					//进程标志
		char ch_name[PROC_NAME_LEN];		//进程汉字名
		char asc_name[PROC_NAME_LEN];		//进程ascii名
		char usr_name[USER_NAME_LEN];		//用户名
		char host_name[HOSTNAMELEN];		//节点名
	}proc[PROC_MAX],netbus;
};

struct IMEXPORT AUTO_DB
{
	int16u num,restart_flag;
	struct autodb_data_t
	{
		char name[PROC_NAME_LEN];
		char ch_name[PROC_NAME_LEN];
		char par1[PROC_NAME_LEN];
		char par2[PROC_NAME_LEN];
		char par3[PROC_NAME_LEN];
		char par4[PROC_NAME_LEN];
		char par5[PROC_NAME_LEN];
		int8u start_time;
		int8u auto_start;
		int8u starthour;
		int8u startmin;
		int8u startsec;
	}data[AUTO_PROC_MAX+1];
};

//事项库
struct IMEXPORT EVENT_DB
{
	int32u writep;
	int32u num;
	char lock;
	SYS_EVENT event[EVENT_NUM_MAX];
};

struct IMEXPORT LINK_DB
{
	int16u  sum;
	NODE_INFO node_info[LINK_MAX];
};

struct IMEXPORT DBG_PROC
{
	int32u	pid;
	char	proc_asc[DESC_LEN];				//进程名称
	int8u	flag;							//使用标志
	int8u   typenum;
	int16u	writep;
	struct _type_t
	{
		int16s	type;
		char	desc[DESC_LEN];
	}type_desc[DEBUG_MAX_TYPE];
	struct _info_t
	{
		int16s	type;
		char	desc[DEBUG_MAX_LEN];
	}data[DEBUG_MAX_LINE];

};

struct IMEXPORT DBG_DB
{
	int16u		proc_num;
	DBG_PROC	dbg_proc[DEBUG_MAX_PROC];
};

struct  IMEXPORT SYS_TIME
{
	int16s	year;			//etc 2002
	char	mon;
	char	day;
	char	hour;
	char    min;
	char    sec;
	int16s	ms;

	void operator = (struct SYS_TIME &time)
	{
		year = time.year;
		mon = time.mon;
		day = time.day;
		hour = time.hour;
		min = time.min;
		sec = time.sec;
		ms = time.ms;
	}
	struct SYS_TIME operator + (int32s err)			//ms err>0
	{
		short ms_t,carry=0;
		short val = ms + (err%1000);
		ms_t = val%1000;
		carry = val/1000;

		struct tm tim;
		tim.tm_year = year - 1900;
        tim.tm_mon = mon - 1;
        tim.tm_mday = day;
        tim.tm_hour = hour;
        tim.tm_min = min;
        tim.tm_sec = sec;
        tim.tm_isdst = -1;
		time_t tmp_t = mktime(&tim) + err/1000 + carry;
		struct tm *result = localtime(&tmp_t);

		struct SYS_TIME time;
		time.year = result->tm_year + 1900;
		time.mon = result->tm_mon + 1;
		time.day = result->tm_mday;
		time.hour = result->tm_hour;
		time.min = result->tm_min;
		time.sec = result->tm_sec;
		time.ms = ms_t;
		return time;
	} 
	struct SYS_TIME operator - (int32s err)			//ms err>0
	{
		short ms_t,carry = 0;
		short val = ms - (err%1000);
		if(val<0)
		{
			ms_t = val + 1000;
			carry = 1;
		}
		else
			ms_t = val;

		struct tm tim;
		tim.tm_year = year - 1900;
        tim.tm_mon = mon - 1;
        tim.tm_mday = day;
        tim.tm_hour = hour;
        tim.tm_min = min;
        tim.tm_sec = sec;
        tim.tm_isdst = 0;
		time_t tmp_t = mktime(&tim) - err/1000 - carry;
		struct tm *result = localtime(&tmp_t);

		struct SYS_TIME time;
		time.year = result->tm_year + 1900;
		time.mon = result->tm_mon + 1;
		time.day = result->tm_mday;
		time.hour = result->tm_hour;
		time.min = result->tm_min;
		time.sec = result->tm_sec;
		time.ms = ms_t;
		return time;
	} 
	int32s operator - (struct SYS_TIME &time)
	{
		short ms_t,carry = 0;
		short val = ms - (time.ms%1000);
		if(val<0)
		{
			ms_t = val + 1000;
			carry = 1;
		}
		else
			ms_t = val;

		struct tm tim1,tim2;
		tim1.tm_year = year - 1900;
        tim1.tm_mon = mon - 1;
        tim1.tm_mday = day;
        tim1.tm_hour = hour;
        tim1.tm_min = min;
        tim1.tm_sec = sec;
        tim1.tm_isdst = 0;
		time_t tmp_t1 = mktime(&tim1);

		tim2.tm_year = time.year - 1900;
        tim2.tm_mon = time.mon - 1;
        tim2.tm_mday = time.day;
        tim2.tm_hour = time.hour;
        tim2.tm_min = time.min;
        tim2.tm_sec = time.sec;
        tim2.tm_isdst = 0;
		time_t tmp_t2 = mktime(&tim2);

		return ( (tmp_t1 - tmp_t2 - carry)*1000 + ms_t );
	}
};

struct IMEXPORT TIME_DB
{
	SYS_TIME	start;
	int8u		checkmode[GROUP_MAX];
	int32s		err;		//time = GPS - locatetime;	ms	
	SYS_TIME	time;		//GPS time;
};

struct IMEXPORT DEAD_DB
{
	int16u	wp,rp,lockf;
	char	buf[DEADDATA_TXSIZE];
};

struct IMEXPORT DEADBUF_HEAD
{
	char syn[6];
	char flag;				//是否成功发送
	int8u count;			//发送次数
	int8u type;
	int32u len;
	union addr_des
	{
		struct des_addr
		{
			struct in_addr addr;
			int    port;
		} socket_addr;

		struct des_srv
		{
			short	grpno;
			short	srvno;
			int	port;
		} srv;
	}des;
};

typedef struct {
		int16u	writep;
		int16u	readp;
		int16u	read_lock;
		int8u	write_lock;
		int16u	bound;
		int16u	sum;
		SYS_EVENT event[SND_EVENT_MAX];
}SND_EVENT_BUF;

typedef struct {
        int16u	writep;
		int16u	readp;
		int16u	read_lock;
		int8u	write_lock;
		int16u	bound;
		int16u	sum;
        HIS_EVENT event[GEN_EVENT_MAX];
}GEN_EVENT_SAVE;

typedef struct {
        int16u	writep;
		int16u	readp;
		int16u	read_lock;
		int8u	write_lock;
		int16u	bound;
		int16u	sum;
        HIS_EVENT event[OPT_EVENT_MAX];
}OPT_EVENT_SAVE;

typedef struct 
{
	SND_EVENT_BUF   sndevt;
	GEN_EVENT_SAVE  genevt;
	OPT_EVENT_SAVE  optevt;
}HISEVT_DB;

typedef struct _grp_vrt_db
{
	int8u num;
	struct GrpVrt
	{
		int8u grpno;
		int8u grpno_vrt;
	}grp_vrt[GROUP_MAX];
}GRPVRT_DB;

typedef struct
{
	PORT_DB			portdb;
	HISEVT_DB		hisevtdb;
	NET_DB			netdb;
	RTDB_DB			rtdbdb;
	MAIL_DB			maildb;
	PROC_DB			procdb;
	AUTO_DB			autodb;
	EVENT_DB		eventdb;
	LINK_DB			linkdb;
	DBG_DB			dbgdb;
	TIME_DB			timedb;
	DEAD_DB			deaddb;
	GRPVRT_DB		grpvrtdb;
	char			hostname[CODE_LEN];
}NET_SHM_DB;

/*--------------------------zip struct ------------------------------*/

typedef struct {
	char *stream;
	unsigned char mask;
	short int rack;
} BIT_STREAM;


typedef struct {
	short int code_value;
	short int parent_code;
	char character;
} DICTIONARY;

/*--------------------------------------------------------------------*/

extern char *raw_title[];
extern int   raw_loc[] ;
extern int   raw_select[];

extern IMEXPORT char *srv_title[];
extern IMEXPORT int srv_loc[] ;
extern IMEXPORT int   srv_select[];
extern IMEXPORT LINK_DB  *p_netinfo;

//网络信息定义结束
//////////////////////////////////////////////////////////////////////////
#if defined(__unix)
int SingleRun(char*);
#endif

#endif		//all of end
