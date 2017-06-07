/**
@file event_cfg.h
@brief  事项配置项头文件

@author lujiashun
@version 1.2.0
@date 2016-9-22
*/
#ifndef EVENT_CFG_H
#define EVENT_CFG_H

#include <QString>
#include <QColor>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QList>

#include "sysstr.h"

enum EmColoring
{
	EVENTSORTCOLOR = 0,//按事项分类对事项着色
	EVENTTYPECOLOR,//按事项类型进行着色
	EVENTRTUCOLOR,//按事项厂站进行着色
	EVENTLEVELCOLOR
};

//列定义
enum
{
	LIST_COL_SORT,
	LIST_COL_TYPE,
	LIST_COL_TIME,
	LIST_COL_DES,
	LIST_COL_FLOAT_VAL,
	LIST_COL_STATUS_VAL,
	LIST_COL_GROUP_NAME,
	LIST_COL_MEMBER_NAME0,
	LIST_COL_MEMBER_NAME1,
	LIST_COL_MEMBER_NAME2,
	LIST_COL_MEMBER_NAME3,
	LIST_COL_GRAPH_NAME,
	LIST_COL_TONE_INFO,
	LIST_COL_SECU_LEV,
	LIST_COL_PRINT_FLAG,
	LIST_COL_SAVE_FLAG,
	LIST_COL_GRAPH_FLAG,
	LIST_COL_WAV_FLAG,
	LIST_COL_NUM
};

class Event_cfg  
{
public:
	Event_cfg();
	virtual ~Event_cfg();
	
	/**
	@brief 设置着色方式
	*/
	void set_color_type( EmColoring type );

	EmColoring get_color_type();
	/**
	@brief 得到相应列的描述
	*/
	bool get_colname_by_id( uint index, QString &colname );
	/**
	@brief 根据描述获取ID
	*/
	int get_colid_by_colname( const QString &colname );

	//列显示序号
	bool get_col_disp_no( uint index, uint &dispno );
	bool get_col_disp_order( uint index, uint &disporder );
	void set_col_disp_no( uint index, uint dispno );
	bool colIs_disp( uint index );
	void set_col_disp( uint index, bool show = true );
	void set_col_disp( const QString &colname, bool show = true );
	uint get_disp_col_num();
	QString get_col_name( uint dispno );
	bool get_col_id( uint dispno, uint &index );

	//设置事项颜色
	void add_sort_color( uint sort, const QColor &clr );
	void add_type_color( uint type, const QColor &clr );
	void add_rtu_color( QString &rtucode, const QColor &clr );

	//得到事项颜色
	bool get_sort_color( uint sort, QColor &clr );
	bool get_type_color( uint type, QColor &clr );
	bool get_rtu_color( const QString &rtucode, QColor &clr );

	//得到级别颜色
	bool get_level_color( uint level, QColor &clr );

	bool set_level_color( uint level, QColor &clr );

	void set_tone( bool b = true );

	bool get_tone();

	void set_tone_dev( const QString & dev );
	
	QString get_tone_dev();

	void set_keep_unconfirmed_event( bool b = true );

	bool get_keep_unconfirmed_event();

	void set_print( bool b = true );
	
	bool get_print();

	void set_font_size( uint size );

	uint get_font_size();

	void set_event_sound_count( uint type, int soundcount );

	int get_event_sound_count( uint type );

	void set_phone_no( uint type, QString  phoneno );

	QString get_type_phone_no( uint type );

	void set_phone_no_desc(  uint type, QString phoneno, QString desc );
	QString get_send_phone_no( SYS_EVENT* event );

	typedef QMap<QString, QString> PhoneDescMap;
	QMap<int,PhoneDescMap> get_type_to_no_map();


	void set_send_msg( bool b = true );

	bool get_send_msg();

	void set_rcv_msg( bool b);

	bool get_rcv_msg();

	void set_serial_no( QString no );

	QString get_serial_no();

	void set_msg_confirm_time( uint msgconfirmtime);
	
	uint get_msg_confirm_time();

	void set_max_event_num( uint eventnum );

	uint get_max_event_num();

	void set_show_unconfirm( bool b );

	bool get_show_unconfirm();

	// 确认后自动删除
	void set_auto_confirm_del( bool b );

	bool get_auto_confirm_del();

	// 自动匹配
	void set_auto_match( bool b );

	bool get_auto_match();

	QString get_printer();

	void set_printer(const QString & printer);

	void set_dbg_out( bool b );

	bool get_dgb_out();
	
	/**
	@brief 设置显示检修事项
	*/
	void set_show_overhaul_event( bool b );

	bool get_show_overhaul_event();

	void set_report_print( bool b = true );

	bool get_report_print();

	void set_report_print_type( bool b = false );

	int get_report_print_type();

	QMap<QString , QStringList> get_report_map();

	int get_station_count() ;

	void set_report_event_type( QString station, QStringList  eventTypeList ) ;

	QStringList get_report_event_type( QString station ) ;

	void set_report_name( QString station, QStringList  nameList ) ;

	QStringList get_report_name( QString station ) ;

private:
	void create_col_name_map();

private:
	//按事项分类进行着色时，每种分类颜色列表。
	QMap<uint,QColor>		m_event_sort_color;

	//按事项类型进行着色时，每种类型颜色列表
	QMap<uint,QColor>		m_event_type_color;

	//按厂站着色时，每个厂站事项颜色列表
	QMap<QString,QColor>	m_event_rtu_color;

	//按基本着色时，每个事项基本颜色
	QColor					m_level_color[ 8 ];

	//语音报警事项类型对应报警次数
	QMap<int,int>			m_event_sound_count_map;

	//短信报警事项类型对应手机号码
	QMap<int,QString>		m_phone_no_map;

	//短信报警事项类型对应事项对象代码
	QMap<int,PhoneDescMap>		m_type_to_no_map;


	//事项着色类型
	EmColoring				m_color_type;

	//列显示顺序列表
	uint					m_col_id_to_dispno_map[ LIST_COL_NUM ];
	uint					m_col_dispno_to_id_map[ LIST_COL_NUM ];

	QMutex					m_mutex;

	//列显示标志
	bool					m_col_disp_flags[ LIST_COL_NUM ];

	//列序号到列名索引
	QMap<uint,QString>		m_id_to_col_name;
	//列名到列序号索引
	QMap<QString, uint>		m_col_name_to_id;
	
	//是否语音报警
	bool					m_is_tone;

	//语音报警串口设备序号
	QString					m_tone_dev;
	//是否事项打印
	bool					m_is_print;
	//是否保留待确认事项
	bool					m_keep_unconfirmed_event;

	//字体大小
	uint					m_font_size;

	//事项数上限
	uint					m_event_num;

	//只显示待确认事项
	bool					m_is_show_unconfirm_event;

	//是否短信报警
	bool					m_is_send_msg;

	//  是否短信确认[7/10/2012 ljs116] 
	bool					m_is_rcv_msg;
	//  短信确认时间[7/10/2012 ljs116]
	uint					m_msg_confirm_time;

	QString					m_serial_no;

	//确认后自动删除
	bool					m_bauto_confirm_del;

	//需要自动匹配			
	bool					m_bauto_math;

	//打印机
	QString					m_printer;

	bool					m_dgb_out;
	
	bool					m_show_overhaul_event; ///< 是否显示检修事项

	//是否报表打印
	bool					m_is_report_print ;

	int					m_is_vprint ;	///< 是否横向打印
	//报表打印列表
	QMap<QString , QStringList>			m_report_event_type ;
	QMap<QString , QStringList>			m_report_name ;

};

#endif