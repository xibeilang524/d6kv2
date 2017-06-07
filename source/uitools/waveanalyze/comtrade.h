/**
@file comtrade.h
@brief comtrade文件处理相关类头文件

@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef COMTRADE_H
#define COMTRADE_H

#include <QList>
#include <QPoint>

#include "viewintf.h"

class Comtrade;

/**
@brief 通道类基类
*/
class Channel : public View_data
{
public:
	Channel(Comtrade* owner);
	// 注：必须声明QChannel析构函数为虚函数，c++标准规定
	virtual ~Channel();

private:
	QString		m_strClassName;		// 类名
	Comtrade*	m_pOwner;			// 控制者
	double*		m_pData;			// 数据指针
	uint		m_nChannelNo;		// 通道号
	QString		m_strChannelName;	// 通道名称
	QString		m_strDevice;		// 被监视回路元件
	QString		m_strPhase;			// 相位特征

public:
	Channel &operator=(const Channel &);
	void	copy_data(Channel* src, QPoint range);
	void	set_size(int size);
	virtual	int	switch_num();
	QString	channel_name();
	int		channel_no();
	void	SetChannelNo(int n);
	int		channel_phase();
	double*	get_data();
	QString	get_device();
	QString	Phase();
	
	// Interface
	double* get_analog_data(int minPos, int maxPos, int& count);
	STATUS* get_digtal_data(int& initData, int& count);
	int		get_all_switch(STATUS* pSta);
	virtual double Value(int idx) { return 0.0; }
	virtual QColor get_color() { return Qt::white; }
	virtual int harm_value() { return 0; }
	virtual QString get_channel_unit() {return "";}

	friend class Analog;
	friend class Digtal;
	friend class Comtrade;
	friend class File_operate;

};
/**
@brief 模拟量通道类
继承自通道基类
*/
class Analog : public Channel
{
public:
	Analog(Comtrade* owner);
	~Analog();

private:
	QString		m_strDataUnit;		// 通道单位
	double		m_fScale;			// 通道乘数
	double		m_fOffset;			// 通道偏移加数
	double		m_fSkew;			// 从抽样周期开始后的通道时滞(us)
	double		m_fMinValue;		// 最小数据值
	double		m_fMaxValue;		// 最大数据值
	double		m_fPrimary;			// 一次变比
	double		m_fSecodary;		// 二次变比
	QString		m_strPs;			// 数值特征符
	uint		m_nHarmValue;		// 谐波次数 0-直流 1-基波 2-二次谐波

public:
	Analog &operator=(const Analog &);
	uint	AnaType();
	double	Value(int n);
	double	MaxValue();
	QColor	GetColor();
	int		HarmValue()					{ return m_nHarmValue; }

	double* get_analog_data(int minPos, int maxPos, int& count);
	//获取通道单位
	QString get_channel_unit() {return m_strDataUnit;}

	friend class File_operate;

};
/**
@brief 开关量通道类
继承自通道基类
*/
class Digtal : public Channel
{
public:
	Digtal(Comtrade* owner);
	~Digtal();

private:
	uint		m_nSwitchNum;			// 跃变次数
	uint		m_nDefaultState;		// 初始状态
	STATUS		m_sChangeArray[255];	// 变位数组

public:
	Digtal &operator=(const Digtal &);
	double		Value(int n);
	int		get_all_switch(STATUS* pSta);
	STATUS* get_digtal_data(int& initData, int& count);

	void	Filtrate();
	int		switch_num()					{ return m_nSwitchNum; }

	friend class File_operate;
	
};


/**
@brief comtrade数据类
*/
class Comtrade : public Gain_data
{
public:
	Comtrade();
	~Comtrade();

private:
	uint				m_nDataCount;		// 数据总点数
	QString		m_strStartTime;		// 采样起始时间
	QString		m_strTriggerTime;	// 故障触发时间
	double*		m_pSampleTime;		// 采样时间指针,不能用PDWORD 
	QString		m_strFaultFile;		// 故障文件名
	QString		m_strStation;		// 变电站名称
	QString		m_strRecorder;		// 设备编号
	QString		m_strRevYear;		// COMTRADE文件版本定义为标准修改年份
	uint		m_nChannelNum;		// 通道总数
	uint		m_nAnaChannelNum;	// 模拟量通道数
	uint		m_nDigChannelNum;	// 数字量通道数
	double		m_fSysFrequency;	// 系统频率
	uint		m_nFrequencyNum;	// 采样频率数目
	QString		m_strFileType;		// 文件类型 BINARY-ASCII
	QString		m_strTimeMult;		// 时间标记乘数系数
	int			m_nFaultFileType;	//

	QList<Analog*> m_pAnaChannels;
	QList<Digtal*> m_pDigChannels;
	SAMPLE*		m_pSampleInfo;		// 采样信息结构数组

	bool		m_bResetTime;		// 组织采样信息标志

protected:
	double	GetTime(int n);
	SAMPLE	GetSampleByPos(int pos);
	int		GetSampleIndexByPos(int pos);
	int		MaxAnalogNo();
	int		MaxDigtalNo();

	Channel* AddItem(bool b = true);

public:
	void	Clear();
	void	Delete(int n);
	void	DeleteRef(Channel* chl);
	
	Channel* Add(bool b = true);
	Channel* Insert(int n, bool b = true);
	Channel* channel(int n);
	Channel* ChannelByNo(int n, bool b = true);

	QList<Analog*> AnaChannels()	{ return m_pAnaChannels; }
	QList<Digtal*> DigChannels()	{ return m_pDigChannels; }

	int		CalCount();
	void	SetSampleTime(double* p);
	void	AddSample(SAMPLE sam);

	Analog* analog(int n);
	Digtal* digtal(int n);
	int		IndexOf(Channel* channel);
	bool	CanAddData(Comtrade& src);
	void	Notify(void* ptr, ListNotify action);

	void	ResetSample(bool b = false);
	void	FiltrateDigtals();
	void	SetFaultFile(QString s)		{ m_strFaultFile = s; }

	int		ChannelNum()				{ return m_nChannelNum; }
	int		AnaChannelNum()				{ return m_nAnaChannelNum; }
	int		DigChannelNum()				{ return m_nDigChannelNum; }

	// Interface(QGainData)
	QString fault_file()					{ return m_strFaultFile; }
	// 文件类型
	QString file_fype();
	// 通道总数
	int Count(int n = 2);
	// 通道类型（模拟量）
	uint channel_type(int n);
	// 通道谐波次数（模拟量）
	uint channel_harm_value(int n);
	// 通道相位（模拟量）
	uint channel_phase(int n);
	QString start_time()					{ return m_strStartTime; }
	QString trigger_time()				{ return m_strTriggerTime; }
	int sys_frequency()					{ return m_fSysFrequency; }
	// 时标是否正确
	bool pos_is_correct(double timeData);
	int pos(double timeData);
	int data_count() { return m_nDataCount; } 
	// pos位置处的周波点数
	int cycle_count(int pos);	
	// 获得一个周波数据，与时段相关
	double* get_cycle_data(int n, int pos);
	double* get_analog_data(int n, int pos, int count);
	STATUS* get_digtal_data(int& initData, int& count, int n);
	// 获得通道从pos位置开始的数据指针
	double* get_data(int n, int pos);
	int sample_count()					{ return m_nFrequencyNum; }
	int get_samples(int ns, int ne, SAMPLE* sam);
	// 获得完整的采样信息
	void get_full_sample(SAMPLE* sam);
	// 获得Pos位置的时标
	virtual double get_sample_time(int pos);
	virtual double get_sample_time(int pos, int zero);
	// 获得最大采样周波数
	int max_cycle_count();
	// 获得最大变位数
	int max_switch_num();
	// 获得信息字符串
	QString get_info_str(int n);
	// 获得故障时刻位置
	int get_fault_pos();
	// 获得指定位置的采样时标数据指针
	double* get_pos_sample(int pos);
	// 获取通道单位
	QString get_channel_unit(int index);

	// propertys
	void	SetDataCount(int n);
	void	SetStartTime(QString s);
	void	SetTriggerTime(QString s);
	double	SampleTime(int n);
	double* PSampleTime()				{ return m_pSampleTime; }

	// 是否故障
	bool	IsFault();

	friend class Analog;
	friend class Digtal;
	friend class File_operate;

};

#endif