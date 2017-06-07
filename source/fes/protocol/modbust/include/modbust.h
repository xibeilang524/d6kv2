/**
@file modbust.h
@brief modbus带时标规约处理头文件

@author lujishun 
@version 1.0.0
@date 2016-3-22
*/

#ifndef MODBUST_H_
#define MODBUST_H_

#ifdef WIN32
#ifdef FES_MODBUST_PROTOCOL_DLL
#define	FES_MODBUST_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_MODBUST_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_MODBUST_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "modbust_type_def.h"


/**
@brief modbust规约子站类

转发modbus规约类，继承自转发规约基础类
*/
class FES_MODBUST_PROTOCOL_EXPORT Modbust: public Base_protocol
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
// 		MAX_BUF_SIZE = 255,///<缓冲区长度
		MAX_BUF_SIZE = 512,///<缓冲区长度
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
	};


public:
	explicit Modbust(const int& channel);
	~Modbust();

	/**
	@brief 规约处理主任务

	调用该函数开始一个modbus规约任务
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief 报文发送
	*/
	int modbust_send_proc(void);

	/**
	@brief 判断帧类别
	辨别modbus规约报文的类别，S帧、U帧还是I帧，并根据情况校准接收序号
	*/
	FRAME_TYPE judge_frame();

	/**
	@brief 重置modbus规约控制信息

	当新网络连接建立，需重置modbus规约控制信息
	*/
	virtual void init_protocol(void);

	//对于以下函数，需要注意过程总发生网络错误或者任何需要关闭连接的错误，返回-1
	
	/**
	@brief 发送召唤遥测帧报文
	*/
	int call_yc_data(void);

	/**
	@brief 发送召唤遥信帧报文
	*/
	int call_yx_data(void);

	/**
	@brief 发送召唤遥脉帧报文
	*/
	int call_ym_data(void);
	
	/**
	@brief 处理M_ME_TA_1 0x10遥测
	*/
	int deal_sequence_M_ME_NA_1(void);

	/**
	@brief 处理M_IT_NA_1 0x10 电度
	*/
	int deal_sequence_M_IT_NA_1(void);

	/**
	@brief 处理M_SP_NA_1 0x0f遥信
	*/
	int deal_sequence_M_SP_NA_1(void);


	/**
	@brief 处理可变桢数据
	*/
	int deal_i_frame(void);
	
	/**
	@brief 处理无效帧
	*/
	int deal_error_frame(void);

	/**
	@brief 确认
	*/
	int confirm_link(void);

	/**
	@brief modbus CRC检验计算
	*/
	unsigned short CRC16 ( unsigned char *arr_buff, int len);

		/**
	@brief 发送遥控报文
	@para in  ykno 遥控号
	@para in  bclosed 控合/控分			true：控合 false：控分
	@para in  yk_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	virtual int send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag){return 0;};

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd){return 0;};

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

private:
	unsigned char m_apdu_buf[MAX_BUF_SIZE];  ///< 存储一帧完整的待处理报文

	int m_msg_invalid_times;///< 接收到非法报文的次数

	int m_i_frame_count;///< I帧计数
	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;

	bool	m_confirm_link_flag ;	//确认标志

	int8u	m_call_yc_data_flag;		//召唤遥测标识：0:无需召唤 1：需要召唤 2:正在召唤
	int8u	m_call_yx_data_flag;		//召唤遥信标识：0:无需召唤 1：需要召唤 2:正在召唤
	int8u	m_call_ym_data_flag;		//召唤遥脉标识：0:无需召唤 1：需要召唤 2:正在召唤

	int m_v_recv;///< V(R)   接收计数器
	int m_v_send;///< V(S)   发送计数器
	int m_v_ack;///<  Ack    确认对方已正确接收到这个数字的I格式帧

	int m_send_yx_index;///< 当前发送的遥信索引
	int m_send_yc_index;///< 当前发送的遥测索引

	int m_send_pi_index;///< 当前发送的电度索引
	int m_send_chg_yc_index;  ///<当前发送的变位遥测索引

	short	m_common_addr;	//公共地址

	const MODBUST_CONFIG* const	m_config_ptr;		//规约本身无法修改配置
};

/**
@brief 获取规约实例的C风格接口
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief 读取数据库配置的C风格接口
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT void get_config(void);

/**
@brief 获取版本的C风格接口
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT char* get_version(void);

/**
@brief 获取描述信息的C风格接口
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT char* get_desc(void);


#endif