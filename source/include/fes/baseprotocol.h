/**
@file baseprotocol.h
@brief 规约基类头文件

@author zhangyun 
@version 1.0.0
@date 2012-10-25
*/
#ifndef FES_BASE_PROTOCOL_H_
#define FES_BASE_PROTOCOL_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_PROTOCOL_API_EXPORT __declspec(dllexport) 
#else
#define	FES_PROTOCOL_API_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_PROTOCOL_API_EXPORT
#endif

#include "fes/fesshare.h"
#include "fes/fesdev.h"
#include "fes/msgtransceiver.h"
#include "fes/crossplatformtypes.h"
#include "ace/Task.h"

typedef union
{
	float yc_value;
	unsigned char yx_value;
	unsigned long kwh_value;
}real_data_value;

typedef struct
{
	short  	terminalno;             //终端号
	unsigned char protocoltype;     //规约类型  (决定保护信息的解释方法) 0-->LFP, 1-->ISA ,   2-->SEL
	unsigned char gram_len;			//保护报文的长度
	unsigned char gram[256];		//保护报文
}FEP_PROT_STRU;

/////////////////////////////////录波相关结构体/////////////////////////////////////////
#define IEC_101_NOF			2		//标准101文件传输文件名称为16位
#define IEC_101_NOS			1		//标准101文件传输节名称为8位
#define FILE_NAME_LEN		512		//标准101文件传输文件名称为16位

typedef struct
{
	unsigned char STATUS:5;
	unsigned char LFD:1;
	unsigned char FOR:1;
	unsigned char FA:1;
}FILE_SOF;

typedef struct
{
	int  len;
	unsigned short info_addrL;//地址
	unsigned char file_type;
	unsigned char file_name[IEC_101_NOF];	//文件名
	int  time_sec;//1970年1月1日0点0分0秒0毫秒
	int  time_msec;
	FILE_SOF sof;
}WAVE_LIST;

typedef struct
{
	unsigned short info_addr;//文件信息体地址
	int file_lenth;			//文件长度
	int sum_file_lenth;			//已传文件送长度
	int cur_node_lenth;		//当前传送节长度
	int sum_node_lenth;		//已传送当前节长度
	int cur_phase_lenth;	//当前传送段长度
	char save_name[FILE_NAME_LEN];
	unsigned char file_type;
	unsigned char file_name[IEC_101_NOF];
	unsigned char cur_nodename[IEC_101_NOS];	//当前传送节名
}WAVE_LIST_RUN;
//////////////////////////////////////////////////////////////////////////


/**
@brief 规约基类

所有规约都必须继承该类并实现protocol_proc接口
*/
class FES_PROTOCOL_API_EXPORT Base_protocol : public ACE_Task<ACE_MT_SYNCH>
{

	//定义字符长度
protected:
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		REC_BUF = 512,
		REC_LEN = 4096,
		TRN_LEN = REC_LEN
	};

	enum
	{
		NO_NEED = 0,
		NEED_CALL = 1,
		ON_PROCESS,
	};

public:
	/**
	@brief 构造函数
	@param in const int& channel 通道号
	*/
	explicit Base_protocol(const int& channel_no);
	/**
	@brief 析构函数
	*/
	virtual ~Base_protocol();

	/**
	@brief 启动
	*/
	virtual int open (void *args = 0);

	/**
	@brief 停止
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief 规约处理主函数
	*/
	virtual void protocol_proc (void) = 0;


	/**
	@brief 规约处理线程体，必须重新实现
	*/
	virtual int svc(void);

	/**
	@brief 初始化规约实例
	
	可根据实际需要重新实现
	*/
	virtual void init_protocol();
	/**
	@brief 打开通道端口
	@return 失败返回-1
	*/
	int open_dev(void);

	/**
	@brief 关闭通道端口
	*/
	void close_dev(void);

	/**
	@brief 通道是否打开 
	@return true,通道已打开;false,通道未打开
	*/
	bool is_open(void);

	/**
	@brief 关闭通道线程
	*/
	void exit_proc();

protected:
	/**
	@brief 接收数据到接收缓冲区

	@return 有数据返回字节数，无数据返回0，失败返回-1
	*/
	int recv_msg(void);

	/**
	@brief 把发送缓冲区的数据发送出去

	@return 成功返回0，失败返回-1
	*/
	int send_msg(void);

	/**
	@brief 把生成的单个事项放入全局缓存
	*/
	void buf_event(FE_DATA_EVENT_NODE& event);

	/**
	@brief 把生成的单个生数据放入全局缓存
	*/
 	void buf_fe_raw(FE_REAL_DATA_NODE& fe_real_data);

	/**
	@brief 把生成的遥控返校信息放入全局缓存
	*/
 	void buf_yk_check(FE_YK_CHECK_NODE& fe_yk_check_data);
	/**
	@brief 设置时间

	时间会被写入缓冲区，主GPS通道会以一定的时间间隔全网对时
	*/
	virtual void set_time(const unsigned short& year, 
						  const unsigned char& month,
						  const unsigned char& day, 
						  const unsigned char& hour, 
						  const unsigned char& minute, 
						  const unsigned char& second, 
						  const unsigned short& milisecond);
	/**
	@brief 检查设置的时的合理性，同时做一些相应的处理
	*/
	 virtual int check_time_ok(const unsigned short& year, 
							   const unsigned char& month, 
							   const unsigned char& day, 
							   const unsigned char& hour, 
							   const unsigned char& minute, 
							   const unsigned char& second, 
							   const unsigned short& milisecond);

	/**
	@brief 发送遥控返回邮件

	没有特殊需要不需要重新实现
	*/
	virtual void send_yk_ret(const int& rtu_no, const int& ykno, const int& yk_ret, const char* msg);

	/**
	@brief 生成电力事项并放入缓存

	没有特殊需要不需要重新实现
	*/
	void make_event(const FETIME& soe_time, const int& data_no, const unsigned char& yx_value);

	/**
	@brief 生成遥测实时数据点，并放入缓存

	没有特殊需要不需要重新实现
	*/
	void make_yc_real_data(const int& data_no, const float& yc_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief 生成遥信实时数据点，并放入缓存

	没有特殊需要不需要重新实现
	*/

	void make_yx_real_data(const int& data_no, const unsigned char& yx_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief 生成遥脉实时数据点，并放入缓存

	没有特殊需要不需要重新实现
	*/
	void make_kwh_real_data(const int& data_no, const unsigned long& kwh_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief 邮件处理函数

	没有特殊需要不需要重新实现
	*/
	void check_event(void);

	/**
	@brief 发送遥控报文
	@para in  ykno 遥控号
	@para in  bclosed 控合/控分			true：控合 false：控分
	@para in  yk_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag) = 0;

	virtual int send_yt_frame(unsigned char ytno, float set_value,unsigned char yt_flag)=0;

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)=0;

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf)=0;

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) = 0;

protected:
	unsigned char m_read_buf[REC_LEN];		///<读缓冲区
	unsigned char m_write_buf[TRN_LEN];		///<写缓冲区
	int  m_read_lenth;				///<接收区报文长度
	int	 m_write_lenth;				///<发送区报文长度
	
	const int	m_channel_no;			///<通道号
	int	m_rtu_no;				///<终端号
	CHANPARA* 	m_chanpara;				///通道参数的指针
	CHANDATA* 	m_chandata;				///通道状态的指针

	Dev_base*	m_dev_ptr;				///<通讯设备

	unsigned char*		m_yx_ptr;		///本地缓存的遥信值，1个字节一个值，0为合，1为分，未刷新的值为UCHAR_MAX。或者是ACE_Numeric_Limits<T>.max()，下同
	float*				m_yc_ptr;		///<本地缓存的遥测值，未刷新的值为FLT_MAX
	unsigned long*		m_kwh_ptr;		///<本地缓存的电度值，未刷新的值未UINT_MAX 

	int8u	m_call_all_data_flag;		//总召唤标志: 0:无需召唤 1：需要召唤 2:正在召唤
	int8u	m_call_kwh_data_flag;		//召唤电度标志0:无需召唤 1：需要召唤 2:正在召唤
	int8u	m_sync_time_flag;			//召唤电度标志0:无需对时 1：需要召唤 2:对时结束
	int8u   m_rtu_init_flag;			//终端初始化标志
	int8u	m_check_public_key ;		//校验公钥标志0:无需校验 1：需要校验 2:正在校验
	int8u	m_refresh_public_key ;		//更新公钥标志0:无需更新 1：需要更新 2:正在更新

	int m_current_ykno;		//当前遥控地址

	Msg_transceiver*	m_msg_tran_instance; ///<报文转发服务实例指针

	bool m_nEnd;
};

#endif