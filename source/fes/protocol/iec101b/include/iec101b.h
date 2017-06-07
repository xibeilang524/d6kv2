/**
@file iec101b.h
@brief 平衡101规约处理头文件

@author libinghong 
@version 1.0.0
@date 2016-08-12
*/

#ifndef IEC101B_H_
#define IEC101B_H_

#ifdef WIN32
#ifdef FES_IEC101B_PROTOCOL_DLL
#define	FES_IEC101B_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_IEC101B_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_IEC101B_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "iec101b_type_def.h"

/**
@brief 平衡101规约子站类

转发101规约类，继承自转发规约基础类
*/
class FES_IEC101B_PROTOCOL_EXPORT Iec101b: public Base_protocol
{
public:
	explicit Iec101b(const int& channel);
	~Iec101b();

	/**
	@brief 规约处理主任务

	调用该函数开始一个101规约任务
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief 报文发送
	*/
	int Iec101b_send_proc(void);

	/**
	@brief 报文接收
	*/
	int Iec101b_receive_proc(void);

	/**
	@brief 重置101规约控制信息

	当新网络连接建立，需重置101规约控制信息
	*/
	virtual void init_protocol(void);

private:
	/**
	@brief 复位链路
	*/
	int reset_link( void ) ;

	/**
	@brief 请求链路状态
	*/
	int request_link_status( void ) ;

    /**
	@brief 响应链路状态
	*/
	int response_link_status( void ) ;

	 /**
	@brief 确认复位链路
	*/
	int confirm_reset_link( void ) ;

	 /**
	@brief 确认
	*/
	int test_link( void ) ;

    /**
	@brief 确认
	*/
	int confirm_link( void ) ;

	/**
	@brief 总召唤
	*/
	int call_all_data( void ) ;

	/**
	@brief 时间同步
	*/
	int sync_time( void ) ;
	/**
	@brief 获取延时
	*/
	int call_delay_time( void ) ;
	/**
	@brief 发送延时
	*/
	int send_delay_time( void ) ;

	/**
	@brief 召唤电度数据
	*/
	int call_kwh_data( void ) ;

	/**
	@brief 处理固定帧
	*/
	int deal_fixed_frame( void ) ;

	/**
	@brief 处理可变帧
	*/
	int deal_unfixed_frame( void ) ;

	/**
	@brief 处理无效帧
	*/
	int deal_error_frame(void);

	//////以下为yx的处理函数，可增加
	/**
	@brief 处理M_SP_NA_1遥信
	*/
	int deal_sequence_M_SP_NA_1(void);
		/**
	@brief 处理M_DP_NA_1遥信
	*/
	int deal_sequence_M_DP_NA_1(void);
		/**
	@brief 处理M_PS_NA_1遥信
	*/
	int deal_sequence_M_PS_NA_1(void);

//////以下为yxsoe的处理函数，可增加
	/**
	@brief M_SP_TA_1遥信SOE
	*/
	int deal_sequence_M_SP_TA_1(void);
		/**
	@brief M_DP_TA_1遥信SOE
	*/
	int deal_sequence_M_DP_TA_1(void);
		/**
	@brief M_SP_TB_1遥信SOE
	*/
	int deal_sequence_M_SP_TB_1(void);
		/**
	@brief M_DP_TB_1遥信SOE
	*/
	int deal_sequence_M_DP_TB_1(void);
	
//////以下为yc的处理函数，可增加

	/**
	@brief 处理M_ME_NA_1 9、11遥测
	*/
	int deal_sequence_M_ME_NA_1(void);
	/**
	@brief 处理M_ME_TA_1遥测
	*/
	int deal_sequence_M_ME_TA_1(void);
	/**
	@brief 处理M_ME_NC_1遥测
	*/
	int deal_sequence_M_ME_NC_1(void);
	/**
	@brief 处理M_ME_ND_1遥测
	*/
	int deal_sequence_M_ME_ND_1(void);
	

	/**
	@brief 处理M_IT_NA_1电度
	*/
	int deal_sequence_M_IT_NA_1(void);
	
	/**
	@brief 处理遥控
	*/
	int proc_yk_frame(void);

	/**
	@brief 发送遥控报文
	@para in  ykno 遥控号
	@para in  bclosed 控合/控分			true：控合 false：控分
	@para in  yk_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	/**
	@brief 发送设点报文
	@para in  ytno 遥调号
	@para in  set_value 设点值			
	@para in  yt_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag);

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd){return 0;};

	virtual int	send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

private:
	unsigned char m_rec_buf[MAX_BUF_SIZE];  ///< 存储一帧完整的待处理报文

	Timer	m_t0_timer;		//超时计时器
	Timer	m_heart_timer;	//心跳计时器

	Timer	m_call_all_data_timer;	//总召计时器
	Timer	m_call_kwh_timer;		//召电度计时
	Timer	m_sync_time_timer;		//对时计时

	unsigned char m_current_ykoder;	//当前下发遥控状态

	const IEC101B_CONFIG* const	m_config_ptr;		//规约本身无法修改配置
	short	m_common_addr;	//公共地址

	bool m_resend_flag;			//重发标志
	int m_link_flag;		//主站链路标志
	bool m_request_link_flag ; //请求链路标志
	bool m_subrequest_link_flag ; //终端请求链路标志
	int m_sublink_flag;		//终端链路标志
	bool m_subreset_link_flag ; //终端请求链路标志
	bool m_reset_link_flag ;//复位链路标志
	bool m_confirm_link_flag ;//确认标志
	int m_call_delaytime_flag ;
	int m_send_delaytime_flag ;
	unsigned int m_t1 ;
	unsigned int m_t3 ;
	unsigned int m_t4 ;
	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;
	bool m_heart_flag;

	unsigned char m_mainstation_FCB ; //主站FCB

	unsigned char m_resend_count ;		 //重发计数
	unsigned char m_resend_limit ;		//重发次数限
};

/**
@brief 获取规约实例的C风格接口
*/
extern "C" FES_IEC101B_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief 读取数据库配置的C风格接口
*/
extern "C" FES_IEC101B_PROTOCOL_EXPORT void get_config(void);

/**
@brief 获取版本的C风格接口
*/
extern "C" FES_IEC101B_PROTOCOL_EXPORT char* get_version(void);

/**
@brief 获取描述信息的C风格接口
*/
extern "C" FES_IEC101B_PROTOCOL_EXPORT char* get_desc(void);

#endif