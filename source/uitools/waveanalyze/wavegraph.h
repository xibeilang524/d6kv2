/**
@file wavegraph.h
@brief 曲线画板头文件

@author lujiashun 
@version 1.0.0
@date 2016-11-01
*/
#ifndef WAVEGRAPH_H
#define WAVEGRAPH_H

#include <QColor>
#include <QRect> 
#include <QPoint>
#include <qnamespace.h> 
#include <QFont> 
#include <QCursor>
#include <QList> 
#include <QStringList>
#include <QPixmap>  
#include <QEvent> 
#include <QScrollBar>
#include <QScrollArea>
#include <QWidget>
#include <QFrame>
#include <QToolButton>
#include <QToolTip>

#include "viewintf.h"
#include "mouselinerect.h"
#include "drawchannel.h"

const int MAXPOINTS = 100000;		// 允许的最大点数
const int MAXCHANGES = 100;			// 允许的最大变位次数
const int POINTERHEIGHT = 20;		// 指针高度
const int MINMOUSEPOINTSMOVE = 5;	// 光标控制移动view时的最小象素个数
const int MINMOUSEZOOM = 20;		// 选择框缩放的最小象素数
const int MAXSPLITTERPOS = 100;		// 分割轴最大位置
const int MINSPLITTERPOS = 50;		// 分割轴最小位置
const int OPERATEHINT = 1;			// 光标-操作提示

class Wave_graph;
class Draw_channel;
class Draw_analog;
class Draw_digtal;
/**
@brief 绘制时间线类

*/
class Time_line : public QObject
{
	Q_OBJECT

public:
	Time_line(Wave_graph* owner, QPaintDevice* d);
	~Time_line();

private:
	QColor	m_Color;		// 指针颜色
	QCursor m_Cursor;       // 光标指针, 只负责保存这些信息，由Owner来调用显示
	QString m_strHint;      // 提示信息
	int		m_nSize;
	int		m_nPosition;    // 位置，与首尾垂直
	bool	m_bVisible;     // 是否显示
	bool	m_bShowPointer; // 是否绘制头部指针
	QString	m_strCaption;
	int		m_nTag;

	QPaintDevice* m_Device;

signals:
	void	OnCursor(int positon);	// 移动

protected:
	QRect	m_rctDrawRect;
	Wave_graph* m_pOwner;
	QPainter* m_Paint;

	void	Change();
	void	draw_device(QPaintDevice* dev);
	
public:
	int		hit_test(QPoint pt);
	void	Draw();
	
	QColor	color() { return m_Color; }
	void	set_color(QColor c);
	int		position() { return m_nPosition; }
	void	set_position(int n);
	bool	visible() { return m_bVisible; }
	void	set_visible(bool b);
	bool	show_pointer() { return m_bShowPointer; }
	void	set_show_pointer(bool b);
	int		size() { return m_nSize; }
	void	set_size(int n);
	QString	caption() { return m_strCaption; }
	void	set_caption(QString s);
	QString	hint() { return m_strHint; }
	void	set_hint(const QString& s) { m_strHint = s; }
	int		tag() { return m_nTag; }
	void	set_tag(int n);
	QCursor get_cursor() { return m_Cursor; }
	void	set_cursor(QCursor& c);

};

typedef enum Digtal_Kind{ dkLine = 0, dkRect };
typedef enum Cursor_Action{ caLDrag, caLClick, caDbClick};
typedef enum Mouse_Mode{mmNone, mmCopy, mmCursor, mmHint};


/**
@brief 绘制曲线画板类
*/
class Wave_graph : public QFrame
{
	Q_OBJECT
public:
	Wave_graph(QWidget* parent = 0,  Qt::WindowFlags f=0);
	~Wave_graph();

private:
	QColor		m_backgroud_color;	// 背景色
	int		m_channel_count;			// 通道总数 包含模拟量和开关量
	int		m_analog_count;				// 模拟量通道总数
	int		m_digtal_height;				// 开关量高度
	int		m_line_width;					// 线宽度
	int		m_lock_count;					// 锁计数器
	int		m_show_count;				// 每页显示曲线个数
	int		m_data_count;					// 总点数
	bool		m_right_move_flag;			// 鼠标右键移动view标志
	double	m_fzoom_Xdata;				// X轴缩放比例
	bool		m_lock_line_flag;					//锁定游标标志
	bool		m_mouse_zoom_flag;			// 使用鼠标进行缩放操作标志
	bool		m_copying_flog;					// 正在拷贝标志
	bool		m_lock_all_lines_flag;			// 锁定所有时标线标志 
	int		m_cursor_step;					// 游标快速移动步长
	bool		m_auto_set_flag;					// 曲线显隐时自动设置高度标志
	double	m_fzoom_Xstep;					// 单步缩放倍数
	bool		m_region_select_flag;			// 选择区域标志
	bool		m_auto_set_copy_flag;		// 拷贝区域时自动设置标志
	QString m_str_hint;
	bool		m_bshow_hint;
	
	bool	m_redraw_flag;						// 是否需要重绘标志
	uint	m_time_to_draw;						// ms计数，每次绘制所需时间

	Time_line*			m_pdouble_click_line;		// 鼠标双击时确定位置的时间线
	Draw_channel*	m_selected_channel;			// 当前选中的通道
	Mouse_Mode		m_mouse_mode;			// 鼠标操作模式
	Cursor_Action		m_cursor_move;			// 游标移动方式
	Cursor_Action		m_zero_move;				// 零点移动
	Digtal_Kind			m_digtal_kind;			// 开关量变位样式

	QStringList		m_lstZoom;				//已不需要改变量
	
	QList<Draw_channel*>	m_lst_channel;	// 曲线列表
	QList<Time_line*>	m_lst_cursor;	// 游标列表
	QList<Time_line*>	m_lst_lock_lines;	// 锁定的时标列表
	 
	QPoint	m_pt_selected_region;				// 选择区域的范围
	bool	m_bPrinting;						// 打印中
	QPoint	m_pt_print;						// 打印起始点，在执行缩放操作时会出错!	

	QScrollBar*	m_pHbar;
	QScrollBar* m_pVbar;
	QToolButton* m_pbutton;
	QCursor*	m_pleft_hand;
//	QWaveTip*	m_pTip;

signals:
	void signal_move(int n);						// 曲线移动事件
	void signal_zoom(double f);					// 缩放事件
	void signal_copy();							// 拷贝图形事件
	void signal_unselect();						// 取消选择事件
	void signal_select(int count, QColor* arr);	// 通道被选择事件
	void signal_delete(void* ptr);				// 删除通道事件
	void signal_add_select(void* ptr);			// 增加选择通道事件
	void signal_show(int count, bool* arr);		// 通道显/隐事件
	void signal_auto_hint(const QString& hint);	// 通道显示/隐藏事件
	void signal_wave_menu();

protected:
	QColor		m_clr_old;							//	
	int			m_last_show_count;			//
	int			m_last_horz_pos;					// 记忆上次水平滚动栏位置
	int			m_last_vert_pos;					// 记忆上次垂直滚动栏位置
	QPoint		m_pt_mouse;						// 鼠标位置
	bool			m_bleft_pressed;					//
	bool			m_bright_pressed;				//
	uint			m_mouse_points_move;		// 移动view最小象素
	int			m_lock_delta;						// 处于锁定状态的时标线间隔
	QPixmap	m_pixmap;							// 内存位图
	Time_line*		m_cursor_line;				// 游标线，暂时固定为第一条添加的时间线
	Time_line*		m_selected_line;			// 当前被操作的时间线  
	Mouse_line*	m_mouse_line;			// 橡皮筋

protected:
	QPainter*	get_mem_painter();

	void	load_cursors();
	void	init_data();
	void	move_cursor_line(int x);
	void	move_lock_line();

	bool	locked();
	void	set_locking(bool	updateing);
	void	copy_select_rect();
	void	line_zoom();
	void	on_up_mouse(Wave_graph* p, QRect r);

	virtual void render(QPaintDevice* pd);

	int		footer();						// 时间线绘制底部位置
	int		max_footer(bool all = true);		// 最低部的位置
	int		lefter();						// 图形绘制起始位置
	int		draw_count();
	int		cursor_range(bool h = false);
	int		analog_height();
	bool		can_zoom_in();
	int		get_default_analog_height();
	void		update_horz(int pos = 0)	;
	virtual void show_channel(int channel, bool show);
	int		channel_position(int channel);
	double	get_scale(int index);
	Mouse_Mode mouse_mode();
	void		set_mouse_mode(Mouse_Mode mm);
	
	inline	int	get_delta();
	bool		move_selected_channels(int delta);
	Time_line* get_select_cursor(QPoint pt);

	inline void update_mouse_mode();

	void	draw_lines();

	/*  关于VertScrollBar滚动栏
	   1，滚动区域：0～曲线区域-widget高度；
	   2，singleStep: 单位模拟量曲线高度；
	   3，pageStep: widget高度；
 	*/
	void	create_scroll_bars();
	void	update_scroll_bars();
	inline	QRect graph_rect();
	int Hbar_height();	// 水平滚动栏高度
	int Vbar_width();		// 垂直滚动栏宽度

protected:
	// mouse
	void	mousePressEvent( QMouseEvent* );
	void	mouseMoveEvent ( QMouseEvent* );
	void	mouseReleaseEvent ( QMouseEvent* );
	void	mouseDoubleClickEvent ( QMouseEvent* );
	void	wheelEvent ( QWheelEvent* );

	// keyboard
	void	keyPressEvent ( QKeyEvent* );

	// draw
	void	paintEvent ( QPaintEvent* );
	void	resizeEvent ( QResizeEvent* );

	// menu
	void contextMenuEvent ( QContextMenuEvent* e );

protected slots:
	// ScrollBar and ToolButton
	void	slot_Hscroll(int);
	void	slot_Vscroll(int);
	void	slot_btn_clicked();

	// elastic，橡皮筋
	void	slot_elastic_move(QRect rct);
	void	slot_elastic_up(QRect rct);
	
public:
	// lock design pattern
	void	lock();
	void	unlock();

	// channel
	int	indexOf(Draw_channel* channel);
	Draw_channel* add(bool b = true);
	void	insert(int index, Draw_channel* channel);
	void	delete_channel(int index);
	void	Clear();
	void	notify(void* ptr, ListNotify action);

	// lines
	Time_line*	add_cursor(bool key = false);	// 添加游标
	void	delete_cursor(int index);			// 删除游标
	int		cursor_index(Time_line* line);
	int		line_count();
	void	add_lock_line(Time_line* line);		// 添加被锁定游标
	void	delete_lock_line(Time_line* line);	// 删除被锁定游标
	void	lock_lines();						// 锁定游标
	void	set_line_pos(Time_line* line, int x);	// 设置游标位置
	int	position_to_dot(int pos);				// 象素位置转换为点数
	void	update_line_pos(Time_line* line);

	// draw and arrange
	void	set_client_rect(QRect r);
	virtual void render();
	void	print(QPainter* p, int left, int ncount);
	void	alloc_rect();
	void	realloc_rect();
	void	auto_set_rect();
	virtual void auto_position(int channel);
	virtual void auto_position(Draw_channel* channel);
	void	render_caption(bool full = true);
	void	add_select_channel(Draw_channel* channel);
	bool	is_selected(int index);
	void	do_selected(int index, bool b);
	virtual bool locate_channel(int index);
	virtual bool locate_channel(Draw_channel* channel);
	void	select_channel();
	virtual void show_channel();
	void	select_region(QPainter* p);
	void	do_select_region(QPoint pt);

	int	wave_height();
	void	set_item_color(QColor c, int index = 1);	// 设置群颜色
	void	set_selected_item_scale(double y);
	void	scrollY(int delta);						// 曲线绘制位置偏移
	void	clear_intf();							// 清空对数据接口指针的引用
	void	update_vert(bool all = true, int h = 0);
    void	zoom_config();							// 配置缩放下拉列表

	// operating
	void	fill_in();
	void	expand();
	void	cursor_left(int delta = 1);
	void	cursor_right(int delta = 1);
	void	cursor_home();
	void	cursor_end();
	void	cursor_up();
	void	cursor_down();
	void	page_up();
	void	page_down();
	void	page_home();
	void	page_end();

	void	move_channel(Draw_channel* channel, int pos);
	bool	channel_move(Draw_channel* channel, int delta, bool bLock = false);
	void	unselect_all();
	void	show_all(bool b = true);
	void	copy_graph(bool b = true);
//	void	EraseMemLine();

	// tooltip
	QRect	tip(const QPoint& pos, Time_line* line);

	// propertys
	Draw_channel*	channels(int index);
//	void	SetChannel(QDrawChannel* channel);
	Time_line*	cursors(int index);
//	void	SetCursors(QTimeLine* line);
	Time_line*	lock_cursor(int index);
	Draw_channel*	 selected_channel();
	void	set_selected_channel(Draw_channel* channel);
	bool	checked(int index);
	void	set_checked(int index, bool b);
	int		count();
	int		analog_count();
	int		cursor_count();
	int		lock_cursor_count();
	int		digtal_height();
	void		set_digtal_height(int n);
	int		line_width();
	void		set_line_width(int n);
	double		zoom_Xdata();
	void		set_zoom_Xdata(double f);
	bool		mouse_zoom();
	void		set_mouse_zoom(bool b);
	Digtal_Kind	digtal_kind();
	void		set_digtal_kind(Digtal_Kind dk);
	bool		lock_line();
	QStringList* zoom_list();
	bool		copying();
	void		set_copying(bool b);
	int		default_analog_height();
	bool		lock_all_lines();
	void		set_lock_all_lines(bool b);
	Time_line* cursor_line();
	void		set_cursor_line(Time_line* line);
	Time_line* double_click_line();
	void	set_double_click_line(Time_line* line);
	int		cursor_step();
	void		set_cursor_step(int n);
	double	zoom_Xstep();
	void	set_zoom_Xstep(double f);
	bool	auto_set();
	void	set_auto_set(bool b);
	bool	region_select();
	void	set_region_select(bool b);
	double	min_selected_scale();
	double	max_selected_scale();
	double	min_selected_scale1();
	double	max_selected_scale1();
	bool	auto_set_copy();
	void	set_auto_set_copy(bool b);
	bool	printing();
	void	set_printing(bool b);
	QPoint	print_point();
	void	set_print_point(QPoint pt);
	QPoint	selected_region();
	QColor	get_color();
	void	set_color(QColor c);
	bool	show_hint();
	void	set_show_hint(bool b);
    
	int		show_count();
	void		set_show_count(int n);
	int		data_count();
	void		set_data_count(int n);

	friend class Draw_channel;
	friend class Time_line;	

};

#endif