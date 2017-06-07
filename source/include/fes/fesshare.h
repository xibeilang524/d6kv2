#ifndef FES_SHARE_H_
#define FES_SHARE_H_

#include "fes/fesscdshm.h"
#include <time.h>

const char* const DDEFAULT_ENV = "NBENV";

enum
{
	MAX_FE_SRV_NUM = 16,		///<前置机最大数量
	MAX_PROTOCOL_NUM = 128,		///<最大规约数量
	MAX_GPS_NUM = 4,			///<最大GPS数量
	MAX_RTU_NUM = 16385,		///<最大终端数量
	MAX_CHAN_NUM = MAX_RTU_NUM,	///<最大通道数量

	MAX_YC_NUM = 0x200000,		///<最大遥测点数
	MAX_YX_NUM =  MAX_YC_NUM,	///<最大遥信点数
	MAX_KWH_NUM = 0x80000,		///<最大电度数
};

enum
{
	DEFAULT_GRP_NO = 1,			///<默认组号
	CODE_LENTH = 24,			///<代码长度
	SHORTCHAR_LEN = 24,			///短字符长度
	SHORT_DESC_LENTH = 64,		///<短描述长度
	LONG_DESC_LENTH = 255,		///<长描述长度
	DEV_NAME_LEN = 64,			///<设备地址长度
	REC_LEN = 4096,				///通道接收缓冲区长度
	TRN_LEN = REC_LEN,			///通道发送缓冲区长度
	MSG_MNG_PORT = 7899			///发送报文端口号
};

/**
@brief 设备类型
*/
enum DEVTYPE
{
	SERIAL,		///<串口设备
	TCP_CLIENT,	///<TCP客户端
	TCP_SERVER,	///<TCP服务器
	MOXA,		///<MOXA设备
	UDP			///<UDP设备
};


/**
@brief 通道状态
*/
enum CHANSTAT
{
	CHANUNCHECK, ///<不检查
	CHANSEARCH,	 ///<通道搜索
	CHANRUN,	 ///<通道运行
	CHANSTOP,	 ///<通道停止
	CHANRATE,	 ///<通道误码
	CHANUNSYN	 ///<通道失步
};

/**
@brief 调试类型
*/
enum DEBUGTYPE
{
	COLLECTDBG,		///<采集调试
	PROTODBG,		///<规约调试
	PORTWATCHDBTG   ///<端口值班调试
};

/**
@brief 遥控返回状态
*/
enum YKRET
{
	SUCCESS,		///<成功
	MSG_FAIL,		///<报文失败
	ENCRYPT_FAIL   ///<加密校验失败
};



/**
@brief 前置系统信息
*/
typedef struct  
{
	int  	host_no;			///<节点号
	int		master_host_no;		///<主前置节点号

	int		protocol_num;		///<规约数量
	int		gps_num;			///<GPS的数量
	int		max_chan_no;		///<最大通道号
	int		max_rtu_no;			///<最大终端号
	time_t	curr_time;			///<当前时间
}FE_SYS_DATA;

/**
@brief 通道参数结构
使用时需初始化，且需将 default_attend_host 置为-1，保证所有通道循环时判断依据正确
*/
typedef struct
{
	int		order;						///<通道序号
	char	desdribe[SHORT_DESC_LENTH];	///<通道名
	char	protocol[CODE_LENTH];		///<规约类型

	unsigned char	gps;				///<GPS标志，0为非GPS，1为GPS
	unsigned char	gps_priority;		///<本通道在GPS_INFO数组中的序号
	unsigned char	check;				///<通道检验标志，0不检验，1检验
	unsigned char	valid;				///<通道使用标识，0为不使用，1为使用
	
	int	common_addr;       		///<公共地址
	int	master_addr;		///<主站地址

	int	last_channel;		///<主备关系中的上一个通道号，-1代表没有了(现作为组号,即终端序号)
	int	next_channel;		///<主备关系中的下一个通道号，-1代表没有了

	int child_rtu;			///<组号-RTU号
	unsigned char	dev_type;			///<设备类型

	//设备相关的参数
	char	local_dev[DEV_NAME_LEN];		///<本地设备地址，对TCP server可用
	short	local_port;						///<本地端口号，对TCP server和UDP可用
	char	dev_name[DEV_NAME_LEN];			///<设备地址
	short	port;							///<端口，TCP server仅使用该字段，不使用back_up_port
	char	backup_dev_name[DEV_NAME_LEN];	///<备设备地址
	short	back_up_port;					///<备端口号

	short	band;									///<波特率
	unsigned char	check_bit;						///<校验位
	unsigned char	stop_bit;						///<停止位
	unsigned char	data_bit;						///<数据位
	unsigned char	flow;							///<流控制

	//通道状态相关的参数
	unsigned char	initial_stat;		///<初始通道状态
	short		stop_time_limit;		///<通道停止时限
	short		bit_error_rate;			///<误码率标准

	short	yc_num;		///<遥测个数，下挂终端测点个数之和，下同
	short	yx_num;		///<遥信个数
	short	kwh_num;	///<电度个数

	short	default_attend_host;	///<默认值班机节点，参数表中如果填写-1，则由系统自动计算默认值班机节点
}CHANPARA;

/**
@brief 密钥参数结构
*/
typedef struct
{
	unsigned char oper_key_no ;//待操作的公钥序号 1 - 4
	unsigned char cur_key_no[ 4 ] ;//当前公钥序号 1 - 50
	unsigned char new_key_no[ 4 ] ;//新公钥序号，仅用于更新公钥操作 1 - 50
}ENCRYPT_KEYNO;

/**
@brief 终端参数结构
*/
typedef struct
{
	int		order;				///<终端序号
	int		last_channel;		///<主通道号

	int		last_rtu;			///上一个RTU，-1代表没有上一个
	int		next_rtu;			///<下一个RTU，如果为通道的第一个rtuno代表没有了

	char	rtu_code[CODE_LENTH];		///<终端代码
	char	desdribe[SHORT_DESC_LENTH];	///<终端名

	short	yc_num;		///<遥测个数
	short	yx_num;		///<遥信个数
	short	kwh_num;	///<电度个数

	unsigned char	valid;			///<终端使用标识,0为不使用，1为使用

	ENCRYPT_KEYNO encry_key_info;//密钥信息
}RTUPARA;


/**
@brief 通道数据结构
使用时需初始化，且需将 default_attend_host 置为-1，保证所有通道循环时判断依据正确
*/
typedef struct
{
	CHANSTAT	stat;					///<通道状态
	time_t		last_recv_time;			///<上次收到数据的时间
	time_t		last_connected_time;	///<上次连接成功的时间
 	time_t      last_up_time;			///<上次运行时间
 	time_t		last_down_time;			///<上次停止时间  

	time_t		last_sync_time;			///<上次对时成功时间

	int			frame_check_err;		///<一帧报文中的误码个数
	int			frame_corr_cnt;			///<总正确报文的个数
	int			frame_err_cnt;			///<总错误报文的个数
	float		bit_err_rate;			///<误码率

	unsigned char		master_flag;			///主通道标志
	unsigned char		auto_switch;			///<主站自动判优标志，0为用户强制主备，1为主站自动判优
	short		curr_attend_host;		///<当前值班机

	unsigned char		sync_time_success;	///<对时成功标志

	float			recv_count;		///<通道接收字节数
	float			send_count;		///<通道发送字节数

}CHANDATA;


/**
@brief GPS数据结构
*/
typedef struct
{
	time_t	update_time;	///<GPS数据更新的时间
	time_t	last_check_time;///<<上一次全网对时的时间
	int		channel;		///<GPS通道号
	FETIME	time;			///GPS提供的时间
	int		attend_host_no;	///GPS被值班的主机
}GPS_INFO;



/**
@brief 通道状态
*/
typedef struct
{
	short		channel_no;		///<通道序号
	CHANSTAT	stat;			///<通道状态
	float		bit_err_rate;	///<误码率
	int			master;			///<主通道标志
}FE_CHAN_STAT;

/**
@brief 报文结构体
*/
typedef struct
{
	enum
	{
		MAX_MSGLENTH = 512
	};
	int channel_no;						//通道号
	FETIME time;						//报文时间
	bool  recv_flag;					//接收标志 1接收，0发送;
	short lenth;						//报文长度
	unsigned char msg[MAX_MSGLENTH];	//报文帧
}FE_MSG;

//结构体排序不要随意变动,牵涉字节对齐
struct YC_PARA_DB
{
	unsigned short  yc_no;//遥测序号
	double slope;	//斜率
	double ordinate;//截距
	unsigned short  ter_no;//终端序号
};

struct YX_PARA_DB
{
	unsigned char negate;	//取反，0为不取反，1为取反
	unsigned short ter_no;//终端序号
	unsigned short  yx_no;//遥信序号

};

struct KWH_PARA_DB
{
	unsigned short  kwh_no;//电度序号
	double full_code;	//满码
	double slope;		//斜率 = 倍率/标度
	unsigned short  ter_no;//终端序号

};

#endif