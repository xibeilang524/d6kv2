/**
@file mainformimpl.h
@brief 程序主窗口头文件

@author lujiashun 
@version 1.0.0
@date 2016-11-01
*/
#ifndef MAINFORMIMPL_H
#define MAINFORMIMPL_H

#include <QTimer>
 #include <QDockWidget>
#include <QGridLayout>
#include <QSpinBox>

#include "comtrade.h"
#include "wavegraph.h"
#include "analyzeutils.h"
#include "wavetab.h"
#include "valueformimpl.h"
// #include "vectorsimpl.h"

#include "ui_mainform.h"

class Wave_caption;

class Main_form_impl : public QMainWindow,public Ui::QMainForm
{
    Q_OBJECT

public:
    Main_form_impl( QWidget* parent = 0);
    ~Main_form_impl();

protected:
	Dlg_value_impl* m_value;	///< 曲线信息框
// 	QDlgVectorImpl* m_pVectors;	///<矢量图
	QTimer*		m_timer_ptr;
	QWaveTabWidget*	m_tab_wave;	///<主对话框tab
	QWaveBar*	m_status_bar;		///<状态栏
	int		m_auto_play_interval;		///<自动演示时间间隔 单位毫秒
	QPixmap*	m_pix_lock[2];

	QDockWidget* m_dock_value;	
	QDockWidget* m_dock_vector;

// 	QGridLayout *QMainFormLayout;
	QVBoxLayout  *QMainFormLayout;

protected:
	/**
	@brief 绘制游标线
	*/
	void add_lines(Wave_graph* graph, int pos,Wave_caption* caption);
	/**
	@brief 打开comtrade文件，并创建comtrade对象
	*/
	Comtrade* create_comtrade(const QString& fileName, bool bshow = true);
	/**
	@brief 创建图形信息，包含图形和标题
	*/
    Wave_graph* create_graph(const QString& caption = QString::null);
	/**
	@brief 获取当前tab页的图形
	*/
	Wave_graph*	current_wave();
	/**
	@brief 获取当前tab页的分析模块信息
	*/
	QAnalyze* current_analyze();
	/**
	@brief 获取当前tab页的comtrade文件信息
	*/
	Comtrade* current_comtrade();
	/**
	@brief 获取当前tab页的录波标题头信息
	*/
	Wave_caption* current_caption();

	void set_zoom_box();

	/**
	@brief 设置action按钮是否可以使用
	*/
	void set_action_enable(bool enable = true);
	/**
	@brief 当前选中的通道号
	*/
	int	selected_channel();
	/**
	@brief 更新锁定游标
	*/
	void update_lockline(Wave_graph* graph);
	/**
	@brief 更新Data关联的Analyze
	*/
	void update_analyze(Comtrade* c);

	/**
	@brief 显示算法
	*/
	void show_math_type();
	/**
	@brief 显示起始时间
	*/
	void show_start_time();
	/**
	@brief 显示游标与DeltaT之间的差值
	*/
	void update_time_delta(int pos1, int pos2);
	/**
	@brief 显示/隐藏 矢量图 
	功能暂时未实现
	*/
	void view_vectors(int page);
	/**
	@brief 创建dockwidget
	@para in str dockwidget的name
	@return 创建成功的dockwidget
	*/
	QDockWidget* create_dock_widget(const QString& str,QMainWindow* mainWin);

	/**
	@brief 显示/隐藏 工具栏
	@para in bar 工具栏名称
	@para in b	 true:显示  false:隐藏
	*/
	void view_toolbar(QToolBar* bar, bool b);
	/**
	@brief 重写resizeEvent() 处理外边框改变时的情况
	*/
	virtual void resizeEvent( QResizeEvent * event );
public:
	Gain_data*	Data();

	/**
	@brief 打开录波文件
	*/
	void open_comtrade_file(const QString& fileName);
	/**
	@brief 新建录波文件
	在已经打开一个录波文件的情况下，再新打开一个录波文件
	*/
	void new_comtrade_file(const QString& fileName);
		/**
	@brief 关闭录波文件
	*/
void close_comtrade_file(int idx, bool ask = true);

public slots:
	// file
    void slot_file_open();	///<打开文件
	void slot_file_new();	///<新建文件
	void slot_file_close();	///<关闭文件
	void slot_file_print();	///<打印文件
	void slot_exit();	///<退出程序
	void slot_file_property();		///<文件属性
	void slot_file_view();		///<文件列表
	void slot_down_load_file();		///< 下载录波文件 FTP对话框

	// view
	void slot_view_fullscreen();			///<全屏
	void slot_view_show_all();			///<显示所有曲线
	void slot_view_auto_set();			///<波形图自动设置
	void slot_view_draw1();				///<按像素绘制
	void slot_view_draw2();				///<压缩/拉伸至一页
	void slot_view_auto_play();			///<自动演示
	void slot_view_to_here();			///<计时起点至此

	// toolbar
	void slot_view_mainbar(bool isToggled);		///<查看主工具栏
	void slot_view_anabar(bool isToggled);			///<查看分析工具栏
	void slot_view_editbar(bool isToggled);		///<查看图形编辑工具栏
	void slot_visible_changed1(bool visible);		///<主工具栏状态改变
	void slot_visible_changed2(bool visible);		///<分析工具栏状态改变
	void slot_visible_changed3(bool visible);		///<图形编辑工具栏状态改变

	// analyze
	void slot_analyze_value();			///<曲线信息
	void slot_analyze_harm();			///<谐波棒图 
	void slot_analyze_vector();			///<向量图
	void slot_analyze_delete();			///<删除曲线
	void slot_analyze_harm_wave();			///<谐波棒图

	// setting
	void slot_set_zero();		///<相对时标零点
	void slot_set_fault();		///<计时起点
	void slot_set_property();		///<波形图属性
	void slot_set_options();		///<选项

	// help
	void slot_help_about();			///<关于

    // edit
	void slot_edit_zoom_YIn();			///<纵向放大
	void slot_edit_zoom_Yout();		///<纵向缩小
	void slot_edit_lock();		///<锁定游标
	void slot_edit_home();	///<游标移动至波形首部
	void slot_edit_end();	///<游标移动至波形尾部
	void slot_edit_move_up();	///<前移一格
	void slot_edit_move_down();	///<后移一格
	void slot_edit_page_up();	///<前移1/3页
	void slot_edit_page_down();	///<后移1/3页
	void slot_edit_show_all();	///<所有通道显示在一页

	// timeline	
	void slot_SG_time_line1(int position);
	void slot_SG_time_line2(int position);
	void slot_SG_time_line3(int position);

	// popup menu
	void slot_wave_menu();
	void slot_caption_menu();

	// caption menu
	void slot_caption_delete();
	void slot_caption_auto_position();
	void slot_caption_auto_scale();
	void slot_caption_harm();
	void slot_caption_harm_wave();
	void slot_caption_property();
	void slot_caption_same_position();
	void slot_caption_same_scale();

	void slot_zoom_actived(int index);
	void slot_wave_zoom(double f);
	void slot_time_out();
	void slot_select(int count, QColor* arr);

	void slot_table_changed(QWidget* w);
	void slot_item_close(int index);
	void slot_item_property(int index);

	void slot_value_dock(bool visible);
	void slot_vector_dock(bool visible);

	void slot_resize_widget(int,int);

private:
		QSpinBox* m_x_edit_zoom;		///<x轴比例系数spinbox

};

#endif