#ifndef OPTIONSIMPL_H
#define OPTIONSIMPL_H

#include "ui_options.h"

const double AUTOPLAYTIME[] =
	{ 0.3, 0.5, 0.7, 1.0 };

// 本选项不进行保存，只支持在线操作
class Dlg_option_impl : public QDialog,public Ui::QDlgOption
{
    Q_OBJECT

public:
    Dlg_option_impl( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~Dlg_option_impl();

	int filter();
	void set_filter(int n);

protected slots:
	void slot_harm_button_click(bool checked);

};

/* 全局属性 */
class Propertys
{
public:
	Propertys();
	~Propertys();
	bool Config();

	int Filter()				{ return m_nFilter; }
	int DiffValue()				{ return m_nDiffValue; }
	bool FixedHarm()			{ return m_bFixedHarm; }
	int FixedHarmValue()		{ return m_nFixedHarmValue; }
	int FloatFormatValue()		{ return m_nFloatFormatValue; }
	int AutoPlayTime()			{ return m_nAutoPlayTime; }

private:
	int		m_nFilter;				// 滤波方式
	int		m_nDiffValue;			// 差分点数
	bool	m_bFixedHarm;			// 固定最大谐波次数标志
	int		m_nFixedHarmValue;		// 谐波计算最大值, 可限制为50
	int		m_nFloatFormatValue;	// 浮点数格式化方式
	int		m_nAutoPlayTime;		// 游标自动演示时间间隔

	friend class Dlg_option_impl;

};

#endif