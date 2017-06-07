/**
@file femailstruct.h
@brief 前置邮件结构设计
@author hongxiang 
@version 1.0.0
@date 2013-11-28
*/
#ifndef FES_MAIL_STRUCT_H_
#define FES_MAIL_STRUCT_H_

#include "fes/fesshare.h"
#include "fes/fesdatashm.h"

/**
@brief 前置邮件类型
*/
enum MAIL_TYPE
{
	INVALID,		//邮件无效
	YK,				//遥控邮件
	CALLING,		//召唤邮件
	CHANNEL_STAT,	//通道状态邮件
	PORT_WATCH,		//端口值班邮件
	REAL_DATA,		//实时数据邮件
	LOAD_PARA,		//参数加载
	SET_POINT,		//设点邮件
	PROT,			//保护邮件
	WAVE,			//录波邮件
	SEND_YX,   //转发遥信
};


/**
@brief 前置邮件结构
*/
struct FE_MAIL_STRUCT
{
	MAIL_TYPE mail_type;		//邮件类型
	int mail_lenth;				//邮件长度
	int sub_information1;		//附属数据1，比如遥控邮件中需要有个终端号
	int sub_information2;		//附属数据2，保留
	unsigned char mail_body[1];	//可变长的邮件体

	/**
	@brief 计算邮件的长度
	*/
	int get_lenth(void)
	{
		return sizeof(FE_MAIL_STRUCT);
	}
};


/**
@brief 遥控邮件结构
*/
struct FE_YK_MAIL
{
	int yk_flag; //标识位：0：选择 1：执行 2：取消
	int	channel_no;//通道号
	int rtu_no;	//终端号
	int yk_no;	//遥控号
	int action;	//遥控类别：scada下发命令：0x33为分，0xCC为合；转到规约线程：0:分，1:合
};

/**
@brief 设点邮件结构
*/
struct FE_SETPOINT_MAIL
{
	int yt_flag; //标识位：0：选择 1：执行 2：取消
	int	channel_no;//通道号
	int rtu_no;	//终端号
	int yt_no;	//遥控号
	float setvalue;	//设点值
};

/**
@brief 设点邮件结构
*/
struct FE_PROT_MAIL
{
	int	channel_no;//通道号
	int rtu_no;	//终端号
	int prot_type;//保护规约类型
	int cmd_lenth;//命令长度
	int exe_flag; //标识位：0：预置 1：执行 2：取消
	unsigned char prot_buf[1];	//可变长的邮件体
};

/**
@brief 遥控邮件结构
*/
struct FE_TRANS_YX_MAIL
{
	int channel_no;//通道号
	int rtu_no;	//终端号
	int yx_no;	//遥信号
	int yx_value;	//遥信值
};

/**
@brief 保护规约类型定义
目前定义这么多的类型，需要使用的话将数字变成有意义的英文说明，
目前标为数字的都是没有使用的保护规约类型
*/
enum PROT_PROTOCOL_TYPE
{
	FE_PROTTYPE_0,   
	FE_PROTTYPE_1,     
	FE_PROTTYPE_2,         
	FE_PROTTYPE_3,         
	FE_PROTTYPE_4,         
	FE_PROTTYPE_5,         
	FE_PROTTYPE_6,         
	FE_PROTTYPE_7,		    
	FE_PROTTYPE_8,	    	
	FE_PROTTYPE_9,			
	FE_PROTTYPE_10,		
	FE_PROTTYPE_11,		
	FE_PROTTYPE_12,        
	FE_PROTTYPE_13,		
	FE_PROTTYPE_14,		
	FE_PROTTYPE_15,		
	FE_PROTTYPE_16,		
	FE_PROTTYPE_17,
	FE_PROTTYPE_18,		
	FE_PROTTYPE_19,		
	FE_PROTTYPE_20,		
	FE_PROTTYPE_21,		
	FE_PROTTYPE_22,		
	FE_PROTTYPE_23,		
	FE_PROTTYPE_24,		
	FE_PROTTYPE_25,		
	FE_PROTTYPE_26,	
	FE_PROTTYPE_27,		
	FE_PROTTYPE_28,
	FE_PROTTYPE_29,		
	FE_PROTTYPE_30,		
	FE_PROTTYPE_31,		
	FE_PROTTYPE_32,		
	FE_PROTTYPE_33,
	FE_PROTTYPE_34,		
	FE_PROTTYPE_35,	
	FE_PROTTYPE_IEC103,			//标准103   				
	FE_PROTTYPE_37,				
	FE_PROTTYPE_38,				
	FE_PROTTYPE_39,			
	FE_PROTTYPE_40,			
	FE_PROTTYPE_41,			
	FE_PROTTYPE_42,			
	FE_PROTTYPE_43,			
	FE_PROTTYPE_44,			
	FE_PROTTYPE_45,			
	FE_PROTTYPE_46,			
	FE_PROTTYPE_47,			
	FE_PROTTYPE_48,			
	FE_PROTTYPE_49,			
	FE_PROTTYPE_50,			
	FE_PROTTYPE_51,			
	FE_PROTTYPE_52,			
	FE_PROTTYPE_53,			
	FE_PROTTYPE_54,			
	FE_PROTTYPE_ISA103,				//ISA扩展103规约
	FE_PROTTYPE_56,			
	FE_PROTTYPE_57,			
	FE_PROTTYPE_58,			
	FE_PROTTYPE_59,			
	FE_PROTTYPE_60,			
};

/**
@brief 召唤邮件结构
*/
struct FE_CALLING_MAIL
{
	enum CALL_TYPE		//召唤类别
	{
		CALL_ALL,		//总召
		RTU_INIT,		//终端初始化
		CHANNEL_INIT,	//通道初始化
		CHECK_TIME,		//对时
		CALL_KWH,		//电度
		CHECK_PUBLIC_KEY ,	//检查公钥
		REFRESH_PUBLIC_KEY	//更新公钥
	};
	CALL_TYPE call_type;
	int channel_no;		//通道号
	int rtu_no;			//终端号
};

/**
@brief 通道状态邮件结构
*/
typedef struct
{
	short		channel_no;		///<通道序号
	short		rtu_no;			///<通道监管的首个终端序号
	CHANSTAT	stat;			///<通道状态
	float		bit_err_rate;	///<误码率
	int			master;			///<主通道标志

	int		last_sync_time;			///<上次对时成功时间
	float			recv_count;		///<通道接收字节数
	float			send_count;		///<通道发送字节数
	unsigned char		sync_time_success;	///<对时成功标志


}FE_CHANNEL_STAT_MAIL;

#endif;