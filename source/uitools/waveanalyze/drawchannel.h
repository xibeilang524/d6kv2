/**
@file drawchannnel.h
@brief 绘制通道头文件

@author lujiashun 
@version 1.0.0
@date 2016-11-08
*/
#ifndef DRAWCHANNEL_H
#define DRAWCHANNEL_H

#include <QFont>
#include <QRect>
#include <QFontMetrics>

#include "viewintf.h"
#include "wavegraph.h"
#include "comtrade.h"

const int AMARGIN = 2;				// 边缘
// const int DIGTALHEIGHT = 15;		// 开关量默认高度	 
const int DIGTALHEIGHT = 40;		// 开关量默认高度	 
const QString DRAWCLASS[] = {"QDrawChannel", "QDrawAnalog", "QDrawDigtal"};

class Wave_graph;

/**
@brief 绘制通道曲线基类

*/
class Draw_channel
{
public:
	Draw_channel(Wave_graph* owner);
	virtual ~Draw_channel();

private:
	View_data*		m_pdata;				///< 关联数据接口
	Comtrade*	m_pcomtrade;
	bool		m_bselected;					///< 被选择
	int		m_left_margin;				///< 左边缘
	bool		m_bshow_border;			///< 显示边框
	bool		m_bshow_caption;			///< 显示名称
	bool		m_bvisible;						///< 是否显示
	QString	m_caption;					///< 名称
	QFont		m_font;						///< 名称字体
	bool			m_bshow_pointer;		///< 显示指示针, 指针应该可以有几种绘制方法，指明当前的状态
	QColor		m_color;						///< 颜色
	QRect		m_draw_rect;				///< 绘制区域
	double		m_max_value;				///< 最大值
	int			m_position;					///< 位置，根据绘制区域而定
	QString	m_str_hint;					///<
	bool			m_bauto_size;				///< 自动设置纵向比例标志

	QString	m_str_class_name;	///< 类名

protected:
	Wave_graph* m_owner_ptr;
	QRect		m_caption_rect;

	virtual void change();
	virtual void rect_update() = 0;
	virtual QRect get_select_rect(int rightPos) = 0;
	/**
	@brief 绘制直线封装函数
	@para in  move  moveTo/lineTo     true:moveTo  false:lineTo
	*/
	void	draw_line(QPainter* p, QPainterPath &pa,QPointF pt, bool print, bool move);
	/**
	@brief 改变标题头rect的大小
	*/
	void	change_caption_rect();
	/**
	@brief 绘制录波曲线基础函数
	@para in  xref  x轴比例系数  
	@para in  lefter  x轴比例系数  
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, int drawCount, bool print = FALSE) = 0;

public:
	/**
	@brief 绘制标题头内容
	@para in p 画板
	@para in  rightPos 绘制区域最右端
	@para in  index 绘制的通道号
	@para in  currentPos 当前鼠标所在位置x轴坐标
	*/
	void   draw_caption(QPainter* p, int rightPos,int index,int currentPos=0);
	/**
	@brief 绘制标题头右侧箭头
	@para in p 画板
	@para in  rightPos 绘制区域最右端
	*/
	void   draw_pointer(QPainter* p, int rightPos);
	/**
	@brief 判断pt是否在标题头矩形框内部
	*/
	int    hit_test(QPoint pt);
	QPoint caption_size();
	void   copy_font(QFont& ft);
	void   set_max_value(double f);
	void   set_view_data(View_data* data, double maxData = 0);
	/**
	@brief 获取类名
	*/
	QString class_name();
	/**
	@brief 设置comtrade数据
	*/
	void   set_comtrade_data(Comtrade* c);		///<设置comtrade

public:
	int		position();
	void	set_position(int n );
	QColor	color(); 
	void	set_color(QColor c);
	bool	show_pointer();
	void	set_show_pointer(bool b);
	bool	visible();
	void	set_visible(bool b);
	QString caption();
	void	set_caption(const QString& s);
	QRect	caption_rect();
	QFont&	font();
	void	set_font(QFont& f);
	bool	show_border();
	void	set_show_border(bool b);
	bool	show_caption();
	void	set_show_caption(bool b);
	int		left_margin();
	void	set_left_margin(int n);
	QRect	draw_rect();
	void	set_draw_rect(QRect& r);
	bool	selected();
	void    set_selected(bool b);
	QString hint();
	void	set_hint(QString s);
	bool	auto_size();
	void	set_auto_size(bool b);

	friend class Draw_analog;
	friend class Draw_digtal;
	friend class Wave_graph;
	
};

/**
@brief 绘制模拟量曲线类
继承自Draw_channel
*/
class Draw_analog : public Draw_channel
{
public:
	Draw_analog(Wave_graph* owner);
	~Draw_analog() {}

private:
	double	m_fYref;		///< 纵向比例

protected:
	double	m_fmin_Yref;		///< 纵向最小比例
	double	m_fmax_Yref;		///< 纵向最大比例
	virtual void rect_update();
	virtual QRect get_select_rect(int rightPos);
	/**
	@brief 自动根据分配高度和最大值来设置纵向比例；并设置比例极限值 
	*/
	void	auto_set_Yref();

public:
	/**
	@brief 绘制曲线
	@para in xref x轴比例系数
	@para in lefter x轴偏移量
	@para in drawCount 绘制曲线的数据点数
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, 
		int drawCount, bool print = FALSE);
	/**
	@brief 获取Y轴比例
	*/
	double  Yref();
	/**
	@brief 设置Y轴比例
	*/
	void	set_Yref(double f);
	/**
	@brief 获取Y轴最小比例
	*/
	double	min_Yref();
	/**
	@brief 获取Y轴最大比例
	*/
	double	max_Yref();
	/**
	@brief 设置 自动设置纵向比例标志
	*/
	void	set_auto_size(bool b);
	/**
	@brief 获取边框
	*/
	int get_margin(double* pp, double& fmax, double& fmin, 
		int& counter, int nunit, int count);
};


/**
@brief 绘制开关量曲线类
继承自Draw_channel
*/
class Draw_digtal : public Draw_channel
{
public:
	Draw_digtal(Wave_graph* owner);
	~Draw_digtal() {}

private:
	int		m_nheight;			///< 开关状态＝1时的高度
	bool		m_is_rect_flag;		///< 绘制变位矩形标志

protected:
	virtual void rect_update();
	virtual QRect get_select_rect(int rightPos);
	QRectF	 print_rect(QRectF r, bool print);
	/**
	@brief 根据pos点获取该点的状态和序号
	用来获取一条开关量波形的起始点的状态和开关变位的序号
	以及终点的状态和序号
	@para in pos 为当前波的点号
	@para in nInit 初始状态 0 or 1
	@para in sta 当前波的所有状态信息
	@para in count 状态的数量
	@para in state 获取的当前pos点的状态值 0 or 1
	@para in index 获取的当前状态值在sts中的序号
	*/
	void switch_info(int pos, int nInit, const STATUS* sta, 
		int count,int& state, int& index);

public:
	/**
	@brief 绘制开关量主函数
	*/
	virtual void draw_wave(QPainter* p, double xref, int lefter, 
		int drawCount, bool print = FALSE);
	/**
	@brief 获取绘制1的高度
	*/
	int		height();
	/**
	@brief 获取是否绘制矩形
	*/
	bool		is_rect();
	/**
	@brief 设置是否绘制矩形
	*/
	void	set_is_rect(bool b);
};



#endif