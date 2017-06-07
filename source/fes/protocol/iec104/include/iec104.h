/**
@file iec104.h
@brief 104规约处理头文件

@author lujiashun 
@version 1.0.0
@date 2016-08-30
*/

#ifndef IEC104_H_
#define IEC104_H_

#ifdef WIN32
#ifdef FES_IEC104_PROTOCOL_DLL
#define	FES_IEC104_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_IEC104_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_IEC104_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "iec104_type_def.h"

/**
@brief 104规约子站类

转发104规约类，继承自转发规约基础类
*/
class FES_IEC104_PROTOCOL_EXPORT Iec104: public Base_protocol
{
public:

	enum NET_RECV///<接收信息
	{
		RCV_INVALID_MESSAGE = -1,///<104报文非法，简单的判断帧长度是否合法
		RCV_NO_MESSAGE = 0,///<没有接收到报文，网络空闲
		RCV_OK = 1,///<接收到报文
	};

	enum INVALID_TIME///< 非法报文的允许出错次数
	{
		MAX_INVALID_MESSAGE_TIMES = 10,
	};

	enum RECV_STEP///<网络接收
	{
		RECV_APCI = 0,
		RECV_ASDU = 1,
	};

	enum FRAME_TYPE///<帧类别
	{
		INVALID_FRAME = -1,///<非法帧

		I_FRAME = 0,	///< I帧
		S_FRAME = 1,
		U_FRAME_STARTDT = 2,
		U_FRAME_TESTFR = 3,
		U_FRAME_STOPDT = 4,
		U_FRAME_TESTFR_ACK = 5,
	};

	enum SIZE_104///< 104规约中长度定义
	{
		MAX_BUF_SIZE = 255,///<缓冲区长度
		MAX_LENGTH = 253,
		MAX_ASDU_INF = 244,
		APDU_INFORM_LEN = 9,
		US_FRAME_LENGTH = 6,
		US_DATA_LENGTH  = 4,
		CTRL_FRAME_LEN  = 16,
	};

	enum  ///< 响应总召唤时一封报文中的信息点最大数
	{
		YC_FRAME_NUM = 24, 
		YC_CHG_FRAME_NUM = 24,
		YX_FRAME_NUM = 64,
		PI_FRAME_NUM = 20,

		WAVE_FILE_MAX =	64,
	};


public:
	explicit Iec104(const int& channel);
	~Iec104();

	/**
	@brief 规约处理主任务

	调用该函数开始一个104规约任务
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief 报文发送
	*/
	int Iec104_send_proc(void);

	/**
	@brief 判断帧类别
	辨别104规约报文的类别，S帧、U帧还是I帧，并根据情况校准接收序号
	*/
	FRAME_TYPE judge_frame();

	/**
	@brief 重置104规约控制信息

	当新网络连接建立，需重置104规约控制信息
	*/
	virtual void init_protocol(void);

	//对于以下函数，需要注意过程总发生网络错误或者任何需要关闭连接的错误，返回-1
	/**
	@brief 发送测试帧
	*/
	int send_test_fr(void);
	
	/**
	@brief 处理I帧报文
	*/
	int deal_i_frame(void);
	
	/**
	@brief 处理S帧报文
	*/
	int deal_s_frame(void);
	
	/**
	@brief 响应子站测试报文
	*/
	int deal_test_fr(void);
	
	/**
	@brief 处理子站测试响应报文
	*/
	int deal_test_ack(void);
	
	/**
	@brief 确认激活传输启动,并开始进行总召
	*/
	int deal_start_dt(void);
	
	/**
	@brief 激活传输启动
	*/
	int send_start_DT(void);

	/**
	@brief 确认停止传输启动
	*/
	int deal_stop_dt(void);
	
	/**
	@brief 停止传输启动
	*/
	int send_stop_DT(void);
	
	/**
	@brief 处理无效帧
	*/
	int deal_error_frame(void);

	/**
	@brief 发送S帧
	*/
	int send_s_frame(void);
	
	/**
	@brief 总召唤
	*/
	int call_all_data(void);
	
	/**
	@brief 召唤电度
	*/
	int call_pi_data(void);
	
	/**
	@brief 时间同步
	*/
	int sync_time(void);
	

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
	@brief 处理M_ME_TE_1遥测
	*/
	int deal_sequence_M_ME_TE_1(void);
	/**
	@brief 处理M_ME_TF_1遥测
	*/
	int deal_sequence_M_ME_TF_1(void);
	

	/**
	@brief 处理M_IT_NA_1电度
	*/
	int deal_sequence_M_IT_NA_1(void);

	/**
	@brief 处理M_IT_TB_1电度
	*/
	int deal_sequence_M_IT_TB_1(void);
	
	/**
	@brief 处理遥控
	*/
	int proc_yk_frame(void);

	int deal_prot_frame(void);

	/**
	@brief 发送遥控报文
	@para in  ykno 遥控号
	@para in  bclosed 控合/控分			true：控合 false：控分
	@para in  yk_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);


	/**
	@brief 处理设点
	*/
	int proc_yt_frame(void);

	/**
	@brief 发送设点报文
	@para in  ytno 遥调号
	@para in  set_value 设点值			
	@para in  yt_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yt_frame(unsigned char ykno,float set_value,unsigned char yt_flag);

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd);

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf);

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

	int proc_wave_frame();
private:
	unsigned char m_apdu_buf[MAX_BUF_SIZE];  ///< 存储一帧完整的待处理报文

	int m_msg_invalid_times;///< 接收到非法报文的次数

	int m_i_frame_count;///< I帧计数
	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;

	Timer	m_t0_timer;		//超时计时器
	Timer	m_t1_timer;
	Timer	m_t2_timer;
	Timer	m_t3_timer;

	Timer	m_call_all_data_timer;	//总召计时器
	Timer	m_call_kwh_timer;		//召电度计时
	Timer	m_sync_time_timer;		//对时计时

	bool	m_bt1_timeout;			//t1超时判断条件，即发送了I帧或U帧
	bool	m_bt2_timeout;			//t2超时判断条件，即接收了I帧
	bool	m_bstartdt_flag;		//链路初始化标志
	bool	m_btestfr_flag;			//测试帧发送标志
	bool	m_bSFrame_flag;				//S帧发送标志

	int m_v_recv;///< V(R)   接收计数器
	int m_v_send;///< V(S)   发送计数器
	int m_v_ack;///<  Ack    确认对方已正确接收到这个数字的I格式帧

	int m_send_yx_index;///< 当前发送的遥信索引
	int m_send_yc_index;///< 当前发送的遥测索引

	int m_send_pi_index;///< 当前发送的电度索引
	int m_send_chg_yc_index;  ///<当前发送的变位遥测索引

	short	m_common_addr;	//公共地址

	unsigned char m_current_ykoder;	//当前下发遥控状态

	WAVE_LIST m_wave_file_list[WAVE_FILE_MAX];
	WAVE_LIST_RUN m_cur_wave_file;
	unsigned char m_Manual_wave;	//是否手动召唤录波
	unsigned char m_wave_num_pos;

	const IEC104_CONFIG* const	m_config_ptr;		//规约本身无法修改配置

};

/**
@brief 获取规约实例的C风格接口
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief 读取数据库配置的C风格接口
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT void get_config(void);

/**
@brief 获取版本的C风格接口
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT char* get_version(void);

/**
@brief 获取描述信息的C风格接口
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT char* get_desc(void);


#endif
