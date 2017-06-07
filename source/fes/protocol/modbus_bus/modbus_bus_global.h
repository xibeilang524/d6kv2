#ifndef MODBUS_BUS_GLOBAL_H
#define MODBUS_BUS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MODBUS_BUS_LIB
# define MODBUS_BUS_EXPORT Q_DECL_EXPORT
#else
# define MODBUS_BUS_EXPORT Q_DECL_IMPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"



#define 	CODE_VAL           0x03    //获得当前监测仪内部一个或多个当前寄存器值
#define  	CODE_GetSetVAL     0x04    //获得当前监测仪内部设置寄存器值
#define  	CODE_SETVAL        0x10    //修改当前监测仪内部设置寄存器值

#define     CODE_ERROR_OPER    0x01    //收到错误操作功能码回复
#define     CODE_ERROR_ILLEGAL 0x02    //收到非法寄存器操作或者数据长度超长
#define     CODE_ERROR_COMM_1  0xFF    //对应地址 CG2361S 仪表和传感器通讯出问题
#define     CODE_ERROR_COMM_2  0xFE    //CG2361C 和相应地址的 CG2361S 通讯故障
#define     CODE_ERROR_COMM_3  0xFD    //CG2361M 和 CG2361C 通讯故障
#define     CODE_ERROR_COMM_4  0xFC    //CG2361C 设置的测温点小于 CG2361M 的设置值

#define     PER_MSG_GetValNum  120     //每帧报文最大获取报文数
#define     MAX_SET_VAL_NUM    4       //最大配置寄存器数目

enum FRAME_TYPE///<帧类别
{
	INVALID_FRAME = -1,///<非法帧
};


enum
{
	BIT_ADDRESS = 0,
	BIT_FUNCODE = 1,
	BIT_DATA_LEN = 2,
	BIT_DATA_HEAD = 3,
};

enum
{
	MAX_BUFF_LENGTH = 512 ,
};

enum
{
	ANALOG_TYPE,
	BINARY_TYPE,
	KWH_TYPE,
};

typedef unsigned char   BIT08;  //08位
typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位


#define MYLO8(w)            ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)            ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

//每个装置的基本属性
class CDeviceData
{
public:
	int m_nDeviceID;    //装置ID
	int m_nDeviceAddr;  //装置地址
	//装置命令
public:
	vector<int> m_arrAnalogStartAddr;//起始地址数组(将所有点按点号排序后，每帧请求PER_FRAME_*个数据，每帧第一个数据的起始地址)
	vector<int> m_arrAnalogCountPerFrame;
	//当前装置没有遥信，可不配置
	vector<int> m_arrBinaryStartAddr;
	vector<int> m_arrBinaryCountPerFrame;
public:
	int m_nErrorCount;              //当前错误数目
	int m_nLastHaltTime;            //挂起时间
	bool  m_bIsHalt;                //装置是否挂起
};


#endif // MODBUS_BUS_GLOBAL_H
