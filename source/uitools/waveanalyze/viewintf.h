/**
@file viewintf.h
@brief 数据接口类头文件

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/

#ifndef VIEWINTF_H
#define VIEWINTF_H

#include <QString>
#include <QColor>

const QString CHANNELCLASS[] = {"Channel", "Analog", "Digtal"};


typedef	enum ListNotify{lnAdded, lnDeleted, lnExtracted};

/** 
@brief 开关量变位结构
	有几种状态就有几个值，之前是有变位才有值
	为了在绘制开关量曲线时方便绘制结尾位置
*/
typedef struct tagStatus 
{
	int position;		// 变位位置
	bool change;		// 状态，True-↑, False-↓
	int endPosition;	//变位结束位置
} STATUS;

typedef struct tagSample
{
	int count;
	int begin;
	int end;
	double frequency;
} SAMPLE;

/**
@brief 通道类接口类
*/
class View_data
{
public:
	// 获得模拟量数据
	virtual double* get_analog_data(int minPos, int maxPos, int& count) = 0;
	// 获得开关量数据
	virtual STATUS* get_digtal_data(int& initData, int& count) = 0;
	// 获得通道名称
	virtual QString channel_name() = 0;
	// 获得通道索引（唯一）
	virtual int channel_no() = 0;
	// 获得通道相别
	virtual int channel_phase() = 0;
	// 获得通道变位次数
	virtual int switch_num() = 0;
	// 获得所有变位信息
	virtual int get_all_switch(STATUS* pSta) = 0;
	// 获得通道某点的值
	virtual double Value(int idx) = 0;
	// 获得通道颜色
	virtual QColor get_color() = 0;
	// 获得通道谐波次数
	virtual int harm_value() = 0;
	// 获取通道单位
	virtual QString get_channel_unit() = 0;
};


/**
@brief 数据获取接口类
*/
class Gain_data
{
public:
	// 故障文件名称
	virtual QString fault_file() = 0;
	// 文件类型
	virtual QString file_fype() = 0;
	// 通道总数
	virtual int Count(int n = 2) = 0;
	// 通道类型（模拟量）
	virtual uint channel_type(int n) = 0;
	// 通道谐波次数（模拟量）
	virtual uint channel_harm_value(int n) = 0;
	// 通道相位（模拟量）
	virtual uint channel_phase(int n) = 0;
	virtual QString start_time() = 0;
	virtual QString trigger_time() = 0;
	virtual int sys_frequency() = 0;

	// 时标是否正确
	virtual bool pos_is_correct(double timeData) = 0;
	virtual	int pos(double timeData) = 0;
	virtual int data_count() = 0;
	// pos位置处的周波点数
	virtual int cycle_count(int pos) = 0;
	
	// 获得一个周波数据，与时段相关
	virtual double* get_cycle_data(int n, int pos) = 0;
	virtual double* get_analog_data(int n, int pos, int count) = 0;
	virtual STATUS* get_digtal_data(int& initData, int& count, int n) = 0;
	// 获得通道从pos位置开始的数据指针
	virtual double* get_data(int n, int pos) = 0;

	virtual int sample_count() = 0;
	virtual int get_samples(int ns, int ne, SAMPLE* sam) = 0;
	// 获得完整的采样信息
	virtual void get_full_sample(SAMPLE* sam) = 0;
	// 获得Pos位置的时标
	virtual double get_sample_time(int pos) = 0;
	// 获得最大采样周波数
	virtual int max_cycle_count() = 0;
	// 获得最大变位数
	virtual int max_switch_num() = 0;
	// 获得信息字符串
	virtual QString get_info_str(int n) = 0;
	// 获得故障时刻位置
	virtual int get_fault_pos() = 0;
	// 获得指定位置的采样时标数据指针
	virtual double* get_pos_sample(int pos) = 0;
	
};

// 当前数据对象
Gain_data* DataObject();

#endif