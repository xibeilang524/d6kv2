/**
@file modbusdaqo.h
@brief 大全modbus规约处理头文件

@author chenkai 
@version 1.0.0
@date 2016-07-21
*/

#ifndef MODBUSDAQO_H_
#define MODBUSDAQO_H_

#ifdef WIN32
#ifdef FES_MODBUSDAQO_PROTOCOL_DLL
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "fes/fesshare.h"
#include "modbusdaqo_type_def.h"

#include <QTime>
/**
@brief 大全modbus规约子站类

大全modbus规约类，继承自转发规约基础类
*/
class FES_MODBUSDAQO_PROTOCOL_EXPORT modbusdaqo: public Base_protocol
{
	enum SIZE_MODBUSDAQO
	{
		MAX_BUF_SIZE = 1024,///<缓冲区长度
	};
public:
	explicit modbusdaqo(const int& channel);
	~modbusdaqo();

	/**
	@brief 规约处理主任务

	调用该函数开始一个modbus规约任务
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief 报文发送
	*/
	int modbusdaqo_send_proc(void);
	/**
	@brief 判断帧类别,返回帧类别
	校验报文，并复制完整报文待处理
	*/
	TYPE_IDENTIFICATION calc_frame();

	/**
	@brief 判断帧类别,返回帧类别
	校验报文，并复制完整报文待处理
	*/
	int proc_modbusdaqo_frame(TYPE_IDENTIFICATION frametype);

	/**
	@brief 重置modbus规约控制信息

	当新网络连接建立，需重置modbus规约控制信息
	*/
	virtual void init_protocol(void);

	//对于以下函数，需要注意过程总发生网络错误或者任何需要关闭连接的错误，返回-1		
	/**
	@brief 处理遥信报文
	*/
	int deal_modbusdaqo_code1( void );
	/**

	@brief 处理遥测报文
	*/
	int deal_modbusdaqo_code3( void );

	/**
	@brief 召唤遥信报文
	*/
	int call_modbusdaqo_code1(void);
	
	/**
	@brief 召唤遥测报文
	*/
	int call_modbusdaqo_code3(void);

	/**
	@brief 发送遥控报文
	@para in  ykno 遥控号
	@para in  bclosed 控合/控分			true：控合 false：控分
	@para in  yk_flag 选择/执行/取消：	0：选择 1：执行 2：取消
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	virtual int send_yt_frame(unsigned char ykno, float ytvalue,unsigned char yk_flag){ return 0;};

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd){return 0;};

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) ;

	int deal_yk_frame();

private:
	unsigned char m_modbusdaqo_buf[MAX_BUF_SIZE];  ///< 存储一帧完整的待处理报文

	int m_yc_all_num ;
	int m_yx_all_num ;
	int m_acc_handle_flag ;
	int m_protocol_flag ;
	int m_unit_flag ;
	bool m_call_yc_flag ;
	bool m_call_yx_flag ;

	QTime m_timeout_timer;
	QTime m_yc_timer ;
	QTime m_yx_timer ;

	enum
	{
		NO_NEED = 0,
		NEED_CALL = 1,
		ON_PROCESS,
	};

	short	m_common_addr;	//公共地址
	short	m_master_addr;		//主站地址

	int m_current_ykno;		//当前遥控地址
	unsigned char m_current_ykoder;	//当前下发遥控状态

	const MODBUSDAQO_CONFIG* const	m_config_ptr;		//规约本身无法修改配置
};

/**
@brief 获取规约实例的C风格接口
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief 读取数据库配置的C风格接口
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT void get_config(void);

/**
@brief 获取版本的C风格接口
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT char* get_version(void);

/**
@brief 获取描述信息的C风格接口
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT char* get_desc(void);


#endif