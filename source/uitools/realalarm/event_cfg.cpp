#include "event_cfg.h"
#include "globaldef.h"


Event_cfg::Event_cfg()
{
	int i ;
	for( i = 0; i < LIST_COL_NUM; i ++ )
	{
		m_col_id_to_dispno_map[ i ] = i;
		m_col_dispno_to_id_map[ i ] = i;
	}

	for( i = 0; i < LIST_COL_NUM; i ++ )
	{
		m_col_disp_flags[ i ] = true;
	}

	m_is_tone = true;
	m_keep_unconfirmed_event = true;
	m_is_print = false;
	m_printer = QObject::tr("LPT1");
	m_is_show_unconfirm_event = false;
	m_is_send_msg = false;
	m_is_rcv_msg = false;
	m_bauto_confirm_del = false;
	m_bauto_math = false;
	m_dgb_out = false;
	m_show_overhaul_event = false;
	m_is_report_print = false ;
	m_is_vprint = 0 ;

// 	m_serial_no = 0;
	m_msg_confirm_time = 5;	// 短信确认时间*min [7/10/2012 ljs116]

	m_font_size = 16;

	m_color_type = EVENTTYPECOLOR;

	m_event_num = 1000;

	create_col_name_map();
}

Event_cfg::~Event_cfg()
{

}

bool Event_cfg::get_colname_by_id( uint index, QString &colname )
{

	bool ret = false;

	m_mutex.lock();

	if( m_id_to_col_name.find( index ) == m_id_to_col_name.end() )
	{
		colname = m_id_to_col_name[ index ];
		ret = true;
	}

	m_mutex.unlock();

	return ret;

}

int Event_cfg::get_colid_by_colname( const QString &colname )
{
	int index = -1;

	m_mutex.lock();

	if( m_col_name_to_id.find( colname ) != m_col_name_to_id.end() )
	{
		index = m_col_name_to_id[ colname ];
	}

	m_mutex.unlock();

	return index;
}

	

bool Event_cfg::get_col_disp_no( uint index, uint &dispno )
{
	bool ret ;

	m_mutex.lock();

	if( index < 0 && index >= LIST_COL_NUM )
	{
		ret = false;
	}
	else
	{
		dispno = m_col_id_to_dispno_map[ index ];
		ret = true;
	}

	m_mutex.unlock();

	
	return ret;
}

bool Event_cfg::get_col_disp_order( uint index, uint &disporder )
{
	bool ret ;

	m_mutex.lock();

	if( index < 0 && index >= LIST_COL_NUM )
	{
		ret = false;
	}
	else
	{
		ret = m_col_disp_flags[ index ];
		if( ret )
		{
			disporder = 0;
			for( uint i = 0; i < m_col_id_to_dispno_map[ index ]; i ++ )
			{
				if( m_col_disp_flags[ m_col_dispno_to_id_map[ i ] ] )
					disporder ++;
			}
		}
	}

	m_mutex.unlock();

	
	return ret;
}

void Event_cfg::set_col_disp_no( uint index, uint dispno )
{
	m_mutex.lock();

	m_col_id_to_dispno_map[ index ] = dispno;

	m_col_dispno_to_id_map[ dispno ] = index;

	m_mutex.unlock();
}

bool Event_cfg::colIs_disp( uint index )
{
	bool ret;

	m_mutex.lock();

	if( index >= LIST_COL_NUM )
	{
		ret = false;
	}
	else
	{
		ret = m_col_disp_flags[ index ];
	}

	m_mutex.unlock();
	
	return ret ;
}

void Event_cfg::set_col_disp( uint index, bool show )
{
	if( index >= 0 && index < LIST_COL_NUM )
		m_col_disp_flags[ index ] = show;
}

void Event_cfg::set_col_disp( const QString &colname, bool show )
{
	int index = get_colid_by_colname( colname );

	if( index >= 0 && index < LIST_COL_NUM )
		m_col_disp_flags[ index ] = show;
}

void Event_cfg::add_sort_color( uint sort, const QColor &clr )
{
	m_mutex.lock();

	m_event_sort_color[ sort ] = clr;

	m_mutex.unlock();
}

void Event_cfg::add_type_color( uint type, const QColor &clr )
{
	m_mutex.lock();

	m_event_type_color[ type ] = clr;

	m_mutex.unlock();
}

void Event_cfg::add_rtu_color( QString &rtucode, const QColor &clr )
{
	m_mutex.lock();

	m_event_rtu_color[ rtucode ] = clr;

	m_mutex.unlock();
}

bool Event_cfg::get_sort_color( uint sort, QColor &clr )
{
	bool ret;


	m_mutex.lock();

	if( m_event_sort_color.find( sort ) == m_event_sort_color.end() )
	{
		ret = false;
	}
	else
	{
		clr = m_event_sort_color[ sort ];
		ret = true;
	}

	m_mutex.unlock();

	return ret;
}

bool Event_cfg::get_type_color( uint type, QColor &clr )
{
	bool ret ;

	m_mutex.lock();

	if( m_event_type_color.find( type ) == m_event_type_color.end() )
	{
		ret = false;
	}
	else
	{
		clr = m_event_type_color[ type ];
		ret = true;
	}
	
	m_mutex.unlock();

	return ret;
}

bool Event_cfg::get_rtu_color( const QString &rtucode, QColor &clr )
{
	bool ret;

	m_mutex.lock();

	if( m_event_rtu_color.find( rtucode ) 
		== m_event_rtu_color.end() )
	{
		ret = false;
	}
	else
	{
		clr = m_event_rtu_color[  rtucode  ];
		ret = true;
	}

	m_mutex.unlock();

	return ret;
}

bool Event_cfg::get_level_color( uint level, QColor &clr )
{
	clr = m_level_color[ level ];
	return true;
}
bool Event_cfg::set_level_color( uint level, QColor &clr )
{
	m_level_color[ level ] = clr;
	return true;
}

void Event_cfg::create_col_name_map()
{
	m_id_to_col_name[ LIST_COL_SORT ] = QObject::tr( "事件分类名" );
	m_col_name_to_id[ QObject::tr( "事件分类名" ) ] = LIST_COL_SORT;

	m_id_to_col_name[ LIST_COL_TYPE ] = QObject::tr( "事项类型" );
	m_col_name_to_id[ QObject::tr( "事项类型" ) ] = LIST_COL_TYPE;

	m_id_to_col_name[ LIST_COL_TIME ] = QObject::tr( "事项时间" );
	m_col_name_to_id[ QObject::tr( "事项时间" ) ] = LIST_COL_TIME;

	m_id_to_col_name[ LIST_COL_DES ] = QObject::tr( "事项描述" );
	m_col_name_to_id[ QObject::tr( "事项描述" ) ] = LIST_COL_DES;

	m_id_to_col_name[ LIST_COL_FLOAT_VAL ] = QObject::tr( "当前值" );
	m_col_name_to_id[ QObject::tr( "当前值" ) ] = LIST_COL_FLOAT_VAL;

	m_id_to_col_name[ LIST_COL_STATUS_VAL ] = QObject::tr( "状态值" );
	m_col_name_to_id[ QObject::tr( "状态值" ) ] = LIST_COL_STATUS_VAL;

	m_id_to_col_name[ LIST_COL_GROUP_NAME ] = QObject::tr( "对象组名" );
	m_col_name_to_id[ QObject::tr( "对象组名" ) ] = LIST_COL_GROUP_NAME;

	m_id_to_col_name[ LIST_COL_MEMBER_NAME0 ] = QObject::tr( "对象名0" );
	m_col_name_to_id[ QObject::tr( "对象名0" ) ] = LIST_COL_MEMBER_NAME0;

	m_id_to_col_name[ LIST_COL_MEMBER_NAME1 ] = QObject::tr( "对象名1" );
	m_col_name_to_id[ QObject::tr( "对象名1" ) ] = LIST_COL_MEMBER_NAME1;

	m_id_to_col_name[ LIST_COL_MEMBER_NAME2 ] = QObject::tr( "对象名2" );
	m_col_name_to_id[ QObject::tr( "对象名2" ) ] = LIST_COL_MEMBER_NAME2;

	m_id_to_col_name[ LIST_COL_MEMBER_NAME3 ] = QObject::tr( "对象名3" );
	m_col_name_to_id[ QObject::tr( "对象名3" ) ] = LIST_COL_MEMBER_NAME3;

	m_id_to_col_name[ LIST_COL_GRAPH_NAME ] = QObject::tr( "推图名" );
	m_col_name_to_id[ QObject::tr( "推图名" ) ] = LIST_COL_GRAPH_NAME;

	m_id_to_col_name[ LIST_COL_TONE_INFO ] = QObject::tr( "语音描述" );
	m_col_name_to_id[ QObject::tr( "语音描述" ) ] = LIST_COL_TONE_INFO;

	m_id_to_col_name[ LIST_COL_SECU_LEV ] = QObject::tr( "安全级" );
	m_col_name_to_id[ QObject::tr( "安全级" ) ] = LIST_COL_SECU_LEV;

	m_id_to_col_name[ LIST_COL_PRINT_FLAG ] = QObject::tr( "是否打印" );
	m_col_name_to_id[ QObject::tr( "是否打印" ) ] = LIST_COL_PRINT_FLAG;

	m_id_to_col_name[ LIST_COL_SAVE_FLAG ] = QObject::tr( "是否存盘" );
	m_col_name_to_id[ QObject::tr( "是否存盘" ) ] = LIST_COL_SAVE_FLAG;

	m_id_to_col_name[ LIST_COL_GRAPH_FLAG ] = QObject::tr( "是否推图" );
	m_col_name_to_id[ QObject::tr( "是否推图" ) ] = LIST_COL_GRAPH_FLAG;

	m_id_to_col_name[ LIST_COL_WAV_FLAG ] = QObject::tr( "是否报警" );
	m_col_name_to_id[ QObject::tr( "是否报警" ) ] = LIST_COL_WAV_FLAG;
}


uint Event_cfg::get_disp_col_num()
{
	uint num = 0;
	uint i;

	m_mutex.lock();

	for( i = 0; i < LIST_COL_NUM; i ++ )
	{
		if( m_col_disp_flags[ i ] )
			num ++;
	}

	m_mutex.unlock();

	return num;
}

QString Event_cfg::get_col_name( uint dispno )
{
	return m_id_to_col_name[ m_col_dispno_to_id_map[ dispno ] ];
}

bool Event_cfg::get_col_id( uint dispno, uint &index )
{
	if( dispno < 0 || dispno >= LIST_COL_NUM )
		return false;

	m_mutex.lock();

	index = m_col_dispno_to_id_map[ dispno ];

	m_mutex.unlock();

	return true;
}

void Event_cfg::set_event_sound_count( uint type, int soundcount )
{
	m_event_sound_count_map[ type ] = soundcount;
}

int Event_cfg::get_event_sound_count( uint type )
{
	if( m_event_sound_count_map.find( type ) == 
		m_event_sound_count_map.end() )
		return 0;

	return m_event_sound_count_map[ type ];
}

void Event_cfg::set_phone_no( uint type, QString  phoneno )
{
	QString phoneno_old = get_type_phone_no( type );

	m_phone_no_map[ type ] = phoneno;

	QStringList phoneno_list = phoneno.split(",");
	if (phoneno.isEmpty())	//无任何电话号码类型删除
	{
		m_type_to_no_map.remove(type);
	}
	else if(phoneno_old.isEmpty())	//类型新增一个
	{	
		PhoneDescMap phone_to_desc;
		
		for (int i = 0;i<phoneno_list.size();i++)
		{
			phone_to_desc[phoneno_list.at(i)] = QString();
		}
		if (!m_type_to_no_map.contains(type))
		{
			m_type_to_no_map[type] = phone_to_desc;
		}

	}
	else		//修改
	{
		PhoneDescMap& phone_to_desc = m_type_to_no_map[type];

		PhoneDescMap phonedesc_temp(m_type_to_no_map[type]);

		phone_to_desc.clear();	//清空

		for (int i = 0;i<phoneno_list.size();i++)
		{
			if (phonedesc_temp.contains(phoneno_list.at(i)))
			{
				phone_to_desc[phoneno_list.at(i)] = phonedesc_temp[phoneno_list.at(i)];
			}
			else
			{
				phone_to_desc[phoneno_list.at(i)] = QString();
			}
		}
	}

}

QString Event_cfg::get_type_phone_no( uint type )
{
	QString phoneno;

	if( m_phone_no_map.find( type ) != m_phone_no_map.end() )
	{
		phoneno = m_phone_no_map[ type ];
	}

	return phoneno;
}

void Event_cfg::set_phone_no_desc( uint type, QString phoneno, QString desc )
{
	if (!m_type_to_no_map.contains(type))
	{
		PhoneDescMap phonedesc_temp;
		m_type_to_no_map[type] = phonedesc_temp;
	}
	PhoneDescMap& phone_to_desc = m_type_to_no_map[type];
	phone_to_desc[phoneno] = desc;
}

QString Event_cfg::get_send_phone_no( SYS_EVENT* event )
{
	QString sendphoneno;
	uint type = event->type;
	QString phoneno =get_type_phone_no(type);
	QStringList phoneno_list = phoneno.split(",");


	PhoneDescMap phone_to_desc = m_type_to_no_map[type];
	QString phoneno_temp;
	QString desc;
	for (int i = 0;i<phoneno_list.size();i++)
	{
		desc = phone_to_desc[phoneno_list.at(i)];

		if (desc.contains(event->member0) || desc.isEmpty() )	//若包含此对象,或者无赛选
		{
			if (!sendphoneno.isEmpty())
			{
				sendphoneno.append(",");
			}
			sendphoneno.append(phoneno_list.at(i));
		}
	}

	return sendphoneno;
}

void Event_cfg::set_report_event_type( QString station, QStringList  eventTypeList )
{
	m_report_event_type[ station ] = eventTypeList;
}

void Event_cfg::set_report_name( QString station, QStringList  nameList )
{
	m_report_name[ station ] = nameList;
}

QStringList Event_cfg::get_report_event_type( QString station )
{
	QStringList eventTypeList;

	if( m_report_event_type.find( station ) != m_report_event_type.end() )
	{
		eventTypeList = m_report_event_type[ station ];
	}

	return eventTypeList;
}

QStringList Event_cfg::get_report_name( QString station )
{
	QStringList nameList;

	if( m_report_name.find( station ) != m_report_name.end() )
	{
		nameList = m_report_name[ station ];
	}

	return nameList;
}

int Event_cfg::get_station_count()
{
	return m_report_event_type.count() ;
}

void Event_cfg::set_color_type(EmColoring type)
{
	m_color_type = type;
}

EmColoring Event_cfg::get_color_type()
{
	return m_color_type;
}

void Event_cfg::set_tone( bool b /*= true */ )
{
	m_is_tone = b;
}

bool Event_cfg::get_tone()
{
	return m_is_tone;
}

void Event_cfg::set_tone_dev( const QString & dev )
{
	m_tone_dev = dev;
}

QString Event_cfg::get_tone_dev()
{
	return m_tone_dev;
}

void Event_cfg::set_keep_unconfirmed_event( bool b /*= true */ )
{
	m_keep_unconfirmed_event = b;
}

bool Event_cfg::get_keep_unconfirmed_event()
{
	return m_keep_unconfirmed_event;
}

void Event_cfg::set_print( bool b /*= true */ )
{
	m_is_print = b;
}

bool Event_cfg::get_print()
{
	return m_is_print;
}

void Event_cfg::set_font_size( uint size )
{
	m_font_size = size;
}

uint Event_cfg::get_font_size()
{
	return m_font_size;
}

void Event_cfg::set_send_msg( bool b /*= true */ )
{
	m_is_send_msg = b;
}

bool Event_cfg::get_send_msg()
{
	return m_is_send_msg;
}

void Event_cfg::set_rcv_msg( bool b )
{
	m_is_rcv_msg = b;
}

bool Event_cfg::get_rcv_msg()
{
	return m_is_rcv_msg;
}

void Event_cfg::set_serial_no( QString no )
{
	m_serial_no = no;
}

QString Event_cfg::get_serial_no()
{
	return m_serial_no;
}

void Event_cfg::set_msg_confirm_time( uint msgconfirmtime )
{
	m_msg_confirm_time = msgconfirmtime;
}

uint Event_cfg::get_msg_confirm_time()
{
	return m_msg_confirm_time;
}

void Event_cfg::set_max_event_num( uint eventnum )
{
	m_event_num = eventnum;
}

uint Event_cfg::get_max_event_num()
{
	return m_event_num;
}

void Event_cfg::set_show_unconfirm( bool b )
{
	m_is_show_unconfirm_event = b;
}

bool Event_cfg::get_show_unconfirm()
{
	return m_is_show_unconfirm_event;
}

void Event_cfg::set_auto_confirm_del( bool b )
{
	m_bauto_confirm_del = b;
}

bool Event_cfg::get_auto_confirm_del()
{
	return m_bauto_confirm_del;
}

void Event_cfg::set_auto_match( bool b )
{
	m_bauto_math = b;
}

bool Event_cfg::get_auto_match()
{
	return m_bauto_math;
}

QString Event_cfg::get_printer()
{
	return m_printer;
}

void Event_cfg::set_printer( const QString & printer )
{
	m_printer = printer;
}

void Event_cfg::set_dbg_out( bool b )
{
	m_dgb_out = b;
}

bool Event_cfg::get_dgb_out()
{
	return m_dgb_out;
}

void Event_cfg::set_show_overhaul_event( bool b )
{
	m_show_overhaul_event = b;
}

bool Event_cfg::get_show_overhaul_event()
{
	return m_show_overhaul_event;
}

void Event_cfg::set_report_print( bool b /*= true */ )
{
	m_is_report_print = b;
}

bool Event_cfg::get_report_print()
{
	return m_is_report_print;
}

void Event_cfg::set_report_print_type( bool b /*= false */ )
{
	if ( b)
	{
		m_is_vprint = 1 ;
	}
	else
	{
		m_is_vprint = 0;
	}
}

int Event_cfg::get_report_print_type()
{
	return m_is_vprint;
}

QMap<QString , QStringList> Event_cfg::get_report_map()
{
	return m_report_event_type ;
}

QMap<int,Event_cfg::PhoneDescMap> Event_cfg::get_type_to_no_map()
{
	return m_type_to_no_map;
}
