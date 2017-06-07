/**
@file fesdatashm.h
@brief 前置生数据共享内存头文件

提供前置生数据共享内存的创建、申请、访问接口

@author zhangyun 
@version 1.0.0
@date 2013-11-08
*/
#ifndef FES_DATA_SHM_H_
#define FES_DATA_SHM_H_

#ifdef WIN32
#ifdef FESSHM_DLL
#define	FES_DATA_EXPORT __declspec(dllexport) 
#else
#define	FES_DATA_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_DATA_EXPORT
#endif 

#include "fes/fesshare.h"

struct buddy;
class Fes_shm;



/**
@brief 遥测参数
*/
typedef struct
{
	float slope;	//斜率
	float ordinate;	//截距 

	void set_default(void)
	{
		slope = 1;
		ordinate = 0;
	};
}YC_PARA;

/**
@brief 遥信参数
*/
typedef struct
{
	unsigned char negate;	//取反，0为不取反，1为取反
	void set_default(void)
	{
		negate = 0;
	};
}YX_PARA;

/**
@brief 电度参数
*/
typedef struct
{
	float full_code;	//满码
	float slope;		//斜率 = 倍率/标度

	void set_default(void)
	{
		slope = 1;
		full_code = FLOAT_DEFAULT;
	};
}KWH_PARA;

/**
@brief 前置数据控制段，每个通道所属数据的起始地址
*/
typedef struct 
{
	int channel_yc_begin[MAX_CHAN_NUM];
	int channel_yx_begin[MAX_CHAN_NUM];
	int channel_kwh_begin[MAX_CHAN_NUM];
	
	int rtu_yc_begin[MAX_RTU_NUM];
	int rtu_yx_begin[MAX_RTU_NUM];
	int rtu_kwh_begin[MAX_RTU_NUM];
}FES_DATA_CRL_SEG;

/**
@brief 前置数据生数据段，包括遥测、遥信、电度
*/
typedef struct  
{
	FE_YC yc[MAX_YC_NUM];
	FE_YX yx[MAX_YX_NUM];
	FE_KWH kwh[MAX_KWH_NUM];
}FES_DATA_SEG;

/**
@brief 前置数据参数段
*/
typedef struct  
{
	YC_PARA yc[MAX_YC_NUM];
	YX_PARA yx[MAX_YX_NUM];
	KWH_PARA kwh[MAX_KWH_NUM];
}FES_DATA_PARA_SEG;



/**
@brief 前置数据共享内存结构
*/
typedef struct
{
	FES_DATA_CRL_SEG ctrl_seg;
	FES_DATA_PARA_SEG para_seg;
	FES_DATA_SEG data_seg;
}FE_DATA;

/**
@brief 前置生数据共享内存访问类

提供了前置生数据共享内存的申请、回收，在申请回收的时候不会移动其它通道的数据
除非空间不够，所以此处空间都设置的很大

块是分配的最小单位，即使只申请0个遥测，也会分配1块给它

考虑到以后的可能会有非基础类型的数据作为FE_DATA的成员了，此处放弃使用模板类做
进一步封装
*/
class FES_DATA_EXPORT Fes_data_shm
{
public:
	enum
	{
		FES_DATA_SHM_KEY = 6666777,		///<共享内存key
		FES_DATA_SHM_LEN = 0x8000000,	///<共享内存长度
		YC_BLOCK_SIZE = 100,				///<遥测块大小，
		YX_BLOCK_SIZE = YC_BLOCK_SIZE,	///<遥信块大小
		KWH_BLOCK_SIZE = 20,				///<电度块大小
	};
public:
	~Fes_data_shm();
	/**
	@brief 构造函数，初始化成员，不予显式调用
	*/
	Fes_data_shm();

	/**
	@brief 获取通道的遥测起始
	*/
	FE_YC* channel_yc(const int& channel) const;
	const FE_YC* const_channel_yc(const int& channel) const;
	/**
	@brief 获取通道的遥信起始
	*/
	FE_YX* channel_yx(const int& channel) const;
	const FE_YX* const_channel_yx(const int& channel) const;
	/**
	@brief 获取通道的电度起始
	*/
	FE_KWH* channel_kwh(const int& channel) const;
	const FE_KWH* const_channel_kwh(const int& channel) const;
	/**
	@brief 获取终端的遥测起始
	@return 通道的相对偏移地址
	*/
	int rtu_yc(const int& rtu) const;
	const int const_rtu_yc(const int& rtu) const;
	/**
	@brief 获取终端的遥信起始
	@return 通道的相对偏移地址
	*/
	int rtu_yx(const int& rtu) const;
	const int const_rtu_yx(const int& rtu) const;
	/**
	@brief 获取终端的电度起始
	@return 通道的相对偏移地址
	*/
	int rtu_kwh(const int& rtu) const;
	const int const_rtu_kwh(const int& rtu) const;

	/**
	@brief 获取YC的斜率
	*/
	const float const_yc_slope(const int& channel, const int& datano) const;
	/**
	@brief 获取YC的截取
	*/
	const float const_yc_ordinate(const int& channel, const int& datano) const;
	/**
	@brief 获取YX的取反标志
	*/
	const unsigned char const_yx_negate(const int& channel, const int& datano) const;
	/**
	@brief 获取KWH的满码值
	*/
	const float const_kwh_full_code(const int& channel, const int& datano) const;
	/**
	@brief 获取KWH的斜率
	*/
	const float const_kwh_slope(const int& channel, const int& datano) const;

private:
	Fes_data_shm(const Fes_data_shm&);
	Fes_data_shm& operator = (const Fes_data_shm&);

protected:
	FE_DATA*	m_fe_data;		//生数据地址
};

#endif