/**
@file rdbview_wnd.h
@brief rdbview主窗口

@author chenkai
@version 1.2.0
@date 2016-10-31
*/
#include "data_info.h"
#include "rdb_table.h"
#include "rdb/rdbapi.h"

#include <QMainWindow>
#include <QToolButton>
#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTreeWidget>
#include <QTableWidget>

class Rdbview_wnd : public QMainWindow
{
	Q_OBJECT
public:
	Rdbview_wnd( QWidget* parent = 0 ) ;
	~Rdbview_wnd() ;
protected:
private:
	void ini_rdbview_wnd( void ) ;
	void ini_rdbview_toolbar( void ) ;
	void ini_show_cfg( void ) ;
	void ini_status_bar( void ) ;

	void refresh_rdb_tree( void ) ;
	bool get_terminal_info( void ) ;
	bool get_station_info( void ) ;
	bool get_rtdb_table_info( void ) ;
	bool get_rtdb_terminal_info( void ) ;
	bool get_table_col_info( int cur_sel ) ;
	bool get_yc_data( void ) ;
	bool get_yx_data( void ) ;
	bool get_kwh_data( void ) ;
	bool get_real_table( QString table_name_str , short rtu_no ) ;
	bool get_cur_computer_no( void ) ;
	bool fill_data( QString table_name_str , short rtu_no , bool flag ) ;
	bool fill_empty_data( QString table_name_str , short rtu_no ) ;
	int get_min_yx_type( int brind , int swind , int prind , int stind , int dmsdiagnoseind ) ;
	int get_one_real_data_val( short data_type , short bit_count , short pos , char* val , char* msg_str ) ;
	void get_yxdata_by_table( REALDATA **Yx , short &num , char *tablename ) ;
	void float_to_asc( double dv , unsigned char pointnum , char * asc ) ;
	void set_total_row_col_num( int total_row ,  int total_col ) ;
	short get_terminal_no( QString table_str ) ;
	QString get_rtdb_table_name( QString rtdb_table_describe_str ) ;

	bool readFile( char *filename , void ** buf ) ;
	bool readFile( char *filename , void * buf , int len ) ;
	bool call_real_data( QString table_name_str , short rtu_no ) ;

	int read_stationarea_data( void );

	void rsql_handling( void ) ;
	void error_handling( void ) ;
	void update_table( void ) ;

	void analyze_select( void ) ;
	void analyze_help( void ) ;
	void analyze_update( void ) ;
	void analyze_delete( void ) ;
	void analyze_insert( void ) ;
	int analyze_where( char* select_table_name ) ;
	int analyze_orderby( void ) ;
	int gen_selected_rcds( void ) ;
	void gen_table_info( void ) ;
	void gen_table_struct( void ) ;

private slots:
	void slot_sql_checkbox( int ) ;
	void slot_change_button_type( void ) ;
	void slot_timer( void ) ;
	void slot_read_data( void ) ;
	void slot_file_out( void ) ;
	void slot_scada_set( void ) ;
	void slot_ycyx_set( void ) ;
	void slot_read_data_info( void ) ;
	void slot_select_change( QTreeWidgetItem* pItem ) ;
	void slot_sql_connect( void ) ;
	void slot_sql_disconnect( void ) ;
	void slot_sql_run( void ) ;
	void slot_rdb_select_change( void ) ;
	void slot_sql_change( void ) ;

private:
	QToolButton*	m_pFileout_button ;
	QToolButton*	m_pReaddata_button ;
	QToolButton*	m_pYcyx_display_set_button ;
	QToolButton*	m_pScada_display_set_button ;
	QToolButton*	m_pRead_data_info_button ;
	QToolButton*	m_pRdb_connect_button ;
	QToolButton*	m_pRdb_disconnect_button ;
	QToolButton*	m_pRdb_sqlrun_button ;
	QToolButton*	m_pButton_type_button ;
	QFrame*			m_pToolbar_widget ;
	QCheckBox*		m_pIsSql_checkbox ;
	QComboBox*	m_pRdb_select_combobox ;
	QFrame*			m_pNormal_frame ;
	QFrame*			m_pSql_frame ;
	QWidget*			m_pCentral_widget ;
	QTreeWidget*	m_pRdb_tree ;
	Rdb_table*			m_pNormal_table ;
	Rdb_table*			m_pSql_table ;
	QTextEdit*			m_pSql_textedit ;
	QTimer*				m_pFresh_timer ;
	QLabel*				m_pRead_info_label ;
	QLabel*				m_pStatus_label ;
	QLabel*				m_pSelect_label ;
	QStatusBar*		m_pStatus_bar ;
	QMap<int , PSTATIONAREAPARA>	m_station_area;
	bool						m_bRd_file ;
	unsigned int    m_yc_num ;    //遥测个数
	unsigned int    m_yx_num ;    //遥信个数
	unsigned int    m_yk_num ;    //遥控个数
	unsigned int    m_cal_num ;   //计算量个数
	bool m_bGet_dict ;
	bool m_bIs_connected ;
	char* m_pSql_statement ;
	char   m_rsql_username[ 50 ] ;
	char   m_rsql_password[ 50 ] ;
	RDBTYPE m_rdb_type ;
	Rdb_SQL*			m_pRsql_local,* m_pLocal_dict;   //this memeber variable is used to access local node rdb
	Rdb_SqlCli*		m_pRsql_server,* m_pSer_dict;  //this member variable is used to access server's rdb
	Rdb_LineAnalysis    m_gettoken ;
	RetInfo m_rdb_ret_info ;
	int m_err_code ;
	bool m_bButton_type ;
};