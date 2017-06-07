/**
@file analyzeutils.h
@brief 分析模块：
负责管理曲线信息、矢量图、谐波棒图、特性图相关联的数据信息，以及曲线合成 

@author lujiashun 
@version 1.0.0
@date 2016-11-07
*/

#ifndef ANALYZEUTILS_H
#define ANALYZEUTILS_H

#include "comtrade.h"
#include "channelunit.h"
#include "complexs.h"
#include "selectdetailimpl.h"

class QAnalyze
{
public:
	QAnalyze();
	QAnalyze(Comtrade* c);
	~QAnalyze();

	void SetComtrade(Comtrade* c);
	QString FaultFile();
	int CursorPos()					{ return m_nCursorPos; }
	void SetCursorPos(int pos)		{ m_nCursorPos = pos; }
	Channels* AnaChannels()		{ return m_pAnaChannels; }
	Channels* DigChannels()		{ return m_pDigChannels; }
	QDetails* Details(int index);
	Comtrade* comtrade()			{ return m_Comtrade; }

	// 曲线信息
	int ValuePage()					{ return m_nValuePage; }
	void SetValuePage(int n);
	bool ShowAllSwitch()			{ return m_bShowAllSwitch; }
	void SetShowAllSwitch(bool b)	{ m_bShowAllSwitch = b; }
	bool ShowNoSwitch()				{ return m_bShowNoSwitch; }
	void SetShowNoSwitch(bool b)	{ m_bShowNoSwitch = b; }
	int AverCount()					{ return m_nAverCount; }
	void SetAverCount(int n)		{ m_nAverCount = n; }
	QString CalAnaData(int ch, int pos, int idx);
	QString CalDigData(int ch, int pos, int idx);
	QString CalHarmData(int ch, int pos, int idx);
	int GetDetailIndex(QDetails* det, int idx);
	void GetSwitchInfo(int count, bool* args);

	// 矢量分析
	int VectorPage()				{ return m_nVectorPage; }
	int VectorItemIndex()			{ return m_nHarmValueIndex; }
	void SetVectorItemIndex(int n)	{ m_nHarmValueIndex = n; }
	View_data* HarmChannel()		{ return m_pHarmChannel; }
	void SetHarmChannel(View_data* p)	{ m_pHarmChannel = p; }
	int HarmStyle()					{ return m_nHarmStyle; }
	void SetHarmStyle(int n)		{ m_nHarmStyle = n; }
	int HarmValueIndex()			{ return m_nHarmValueIndex; }
	void SetHarmValueIndex(int n)	{ m_nHarmValueIndex = n; }
	bool AddVector();
	Complex VectorValue(int idx, int pos);
	bool AddHarm();
	double HarmValue(int idx, int pos);
	int SelectOneChannel(const QString& str);

private:
	Comtrade* m_Comtrade;
	Channels* m_pAnaChannels;
	Channels* m_pDigChannels;
	int		m_nCursorPos;			// 记忆上次游标的位置

	// 曲线信息
	int		m_nValuePage;			// 处于激活状态的曲线信息页面
	bool	m_bShowAllSwitch;		// 显示所有变位信息标志
	bool	m_bShowNoSwitch;		// 显示无变位通道标志
	QDetails* m_pDetails[3];		// 表格显示信息
	int		m_nAverCount;			// 计算平均值点数	

    // 矢量/谐波
	int		m_nVectorPage;			// 处于激活状态的矢量/谐波页面
	int		m_nVectorIndex;			// 矢量图基准
	int		m_nHarmStyle;			// 谐波显示模式
	int		m_nHarmValueIndex;		// 谐波值显示模式
	View_data*	m_pHarmChannel;		// 谐波通道索引

protected:
	void Init();
    void InitData();
	void InitAnaDetail();
	void InitDigDetail();

	int MaxHarmValue();
	int VectorIndex(int idx);
	void AddHarmBar(int idx, const QString& str);
	void InitHarmBarDetail();
	
};

#endif