/**
@file event_real_wnd.h
@brief 智能告警主窗口类头文件

@author lujiashun
@version 1.2.0
@date 2016-9-23
*/
#ifndef EVENT_REAL_WND_H
#define EVENT_REAL_WND_H

#include <QMainWindow>
#include <QList>
#include <QIcon>
#include <QFont>
#include <QProcess>

#include "scd/scdapi.h"
#include "net/netapi.h"
#include "event_table.h"
#include "event_para.h"
#include "event_cfg.h"
#include "event_sound.h"
#include "read_event_proc.h"
#include <qstatusbar.h> 
//#ifdef _WIN32
#include "event_msg.h"
#include "globaldef.h"
//#endif

#include "event_print.h"
#include "title_frame.h"
#include "tool_button_frame.h"


#define DATABUFLEN	10240

class QTabWidget;
class QToolButton;
class QSplitter;
class QFrame;

class Event_real_wnd : public QMainWindow  
{
	Q_OBJECT
public:
	Event_real_wnd( QWidget * parent = 0, const char * name = 0, Qt::WindowFlags f = /*WType_TopLevel | */Qt::WindowStaysOnTopHint );
	virtual ~Event_real_wnd();

// 	void customEvent(QCustomEvent *);

	QTabWidget * m_tab_widget;
	QWidget* m_centre_widget;
	QFrame* m_tab_frame;
	QSplitter  * m_splitter;
	QVBoxLayout* m_vbox_layout;
	QVBoxLayout* m_vtab_layout;
	QLabel* m_status_label;


	Title_frame	*m_title_frame;
	Tool_button_frame* m_tool_button_frame;

	//页面列表
	QList<Event_table*>	m_page_list;

	//窗口列表
	QList<Event_table*>	m_wnd_list;

	//界面设置按钮
	QToolButton	*m_ui_set_btn;

	//系统设置按钮
	QToolButton *m_sys_set_btn;

	//系统退出按钮
	QToolButton *m_sys_exit_btn;

	//事项确认按钮
	QToolButton *m_confirm_event_btn;

	//事项确认按钮
	QToolButton *m_all_confirm_event_btn;

	//语音确认按钮
	QToolButton *m_confirm_snd_btn;

	//事项删除按钮
	QToolButton *m_delete_btn;

	//暂停按钮
	QToolButton *m_pause_btn;

	//窗口置顶窗口按钮
	QToolButton *m_topmost_btn;

	//测试语音按钮
	QToolButton *m_test_wav_btn;
	
	//数据库读写类
	Event_para		m_db_para;

	//配置管理类
	Event_cfg		m_event_cfg;

	//事项语音报警类
	Event_sound		m_event_sound;

	//短信报警类
//#ifdef _WIN32
	Event_msg		m_event_msg;
//#endif

	//注册是否成功
	bool			m_breg_successed;

	//当前用户
	USEROPERRIGHT_STRU m_cur_user;

	DISPATCHER_STRU    m_cur_oper_man;

	QMutex	m_mutex;

	//所有实时事项指针列表
	Sys_event_list		m_real_event_list;

	//发送了短信的事项列表
	Sys_event_list		m_event_msg_list;

	//打印列表
	QList<Rsys_event *> m_print_list;
	
	//初始化标题栏和按钮栏
	void init_title_and_toolbtn_frame( void );

	//短信内容对比
	void confirm_msg_event( Sys_event_list & eventlist );

	//事项确认
	void confirm_event( Sys_event_list & eventlist );

	//事项删除
	void delete_event( Sys_event_list & eventlist );

	//向其他节点发送确认事项消息
	void send_msg( Sys_event_list & eventlist );
	
	//添加实时页
	void add_page( Event_table *table );

	void set_page_name( Event_table *table, const QString &title );

	//添加窗口
	void add_wnd( Event_table *table );

	//删除实时页
	void del_page( uint index );

	//删除窗口
	void del_wnd( uint index );

	bool m_bnew_event( Rsys_event*eve );

	//刷新所有页
	void refresh_table();

	void save_event();
	void load_event();

	bool show_dock_menu( const QPoint & globalPos );

	void closeEvent( QCloseEvent *e );
	void timerEvent ( QTimerEvent * e );
	void showEvent( QShowEvent * e);

	bool get_pause();

	//读实时事项类
	Read_event_proc	m_read_event_proc;

	//打印实时事项类
	Event_print m_print;

	bool m_bclose_wnd;

	void statics_events();

	ProcMan & GetProcMan() {return m_proc_man;}

	int  m_page_count;
	int m_current_table_index;
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
// 	bool winEvent(MSG *message, long *result); 
public slots:

	void slot_exit();

	void slot_sys_set();

	void slot_ui_set();

	void slot_top_most();

	void slot_confirm_event();

	void slot_confirm_all_event();

	void slot_confirm_snd();

	void slot_delete_event();

	void slot_pause_btn();

	void slot_show_page( int );

	void slot_test_wav_btn();

	void slot_stop_thread();

	void slot_show_help();

	void slot_custom_event( Rsys_event * ,int);

	// add by chenkai 2013年3月12日 11:23:21
// 	void add(SYS_EVENT *e ,int) ;

private:
	//读初始化文件
	void read_ini( );

	//写初始化文件
	void write_ini( );

	bool reg_proc();

	void unreg_proc();

	void create_socket();

	//重实时库中读出没有被确认的事项
	void read_unconfirm_event();

	//接收事项确认报文
	void on_read_data();

	// 事项列表滚动
	void roll_event_list();

	// 退出清理资源
	void onclose();

	Event_table * get_foucs_table();

	void region(const QPoint &cursorPoint);

private:
	ProcMan			m_proc_man;
	NetDbg			m_net_dbg;

	//checkparent定时器
	int				m_check_timer;
	//接收确认报文定时器
	int				m_recv_data_timer;
	//删除事项定时器
	int				m_del_event_timer;
	//打印定时器
	int				m_print_event_timer;

	//发送确认报文socket
	int				m_event_confirm_socket;

	//是否暂停
	bool			m_bpause;

	//事项确认报文缓冲
	char			m_recv_data_buf[ DATABUFLEN ];
	//事项确认报文缓冲写指针
	uint			m_write_point;
	//事项确认报文缓冲读指针
	uint			m_read_point;
	SYS_EVENT       m_sub_SYSEVENT;
	QIcon			m_new_event_icon;
	QIcon			m_no_new_event_icon;
	QFont			m_cur_font;

	QPoint drag_position;
	Direction dir;

	bool m_bleft_press_down;

};

#endif // !defined(AFX_QEVENTREALWND_H__562B2EBC_1D29_4AB4_8744_99B943BE2074__INCLUDED_)
