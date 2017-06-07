#include "alarmwnd.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QFocusEvent>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>

extern int live_flag ;

alarm_wnd::alarm_wnd( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	setupUi( this ) ;

	m_hdbi.setDB( "modeldb" ) ;
	m_hdbi.setUser( "modeldb" ) ;
	m_hdbi.setPass( "supernb" ) ;
	m_hdbi.createDBI( Ddbi::GD_DBI_HIS ) ;

	ini_alarm_wnd() ;
}

void alarm_wnd::ini_alarm_wnd( void )
{
	setMouseTracking( true ) ;
	setWindowFlags( Qt::WindowStaysOnTopHint | Qt::Popup ) ;
	setWindowModality( Qt::ApplicationModal ) ;
	setWindowFlags( windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint) ) ;

	QDesktopWidget* desktopWidget = QApplication::desktop() ;
	QRect clientRect = desktopWidget->availableGeometry() ;
	QRect applicationRect = desktopWidget->screenGeometry() ;
	int height = applicationRect.height() ;
	int width = applicationRect.width() ;
	m_wnd_height = height / 2 ;
	m_wnd_width = width / 2 ;
	setGeometry( m_wnd_width / 2 ,  m_wnd_height / 2 , m_wnd_width , m_wnd_height ) ; //设置窗体固定大小

	m_pCheck_timer = new QTimer();
	connect( m_pCheck_timer , SIGNAL( timeout() ) , this , SLOT( slot_period_check() ) ) ;
	m_pCheck_timer->start( 10 * 60 * 1000 ) ;

	m_pSystem_tray = new QSystemTrayIcon();
	m_pSystem_tray ->setToolTip( tr( "授权告警" ) ) ;
	m_pSystem_tray ->setIcon( QIcon( ":res/alarm.png" ) ) ;
	m_pSystem_tray->show() ;
	m_bSystem_tray = false ;
	connect( m_pSystem_tray , SIGNAL( messageClicked() ) , this , SLOT( on_tray_clicked() ) ) ;

	slot_period_check() ;
}

void alarm_wnd::slot_period_check( void )
{
	if ( !live_flag )
	{
		exit(  0 ) ;
	}
	int success = m_hdbi.startdb( Ddbi::G_DB_RW ) ;
	if ( !success )
	{
		return ;
	}

	char sqlw[ 128 ] ;
	sprintf( sqlw, "select 组号,授权周期,文件生成时间,\
				   文件导入时间,MAC地址,是否过期 from 授权权限参数表 where 组号=0" ) ;

	m_hdbi.selectFrom( "授权权限参数表" ) ;

	GdbRecords recs ;
	if( !m_hdbi.readSQL( recs , sqlw ) )
		return ;

	if( recs.recnum == 0 )
		return ;

	int res_count = recs.recnum ;
	LICENSE_STRU* pLicense_stru = new LICENSE_STRU[ res_count ] ;
	memset( pLicense_stru , 0 , sizeof( LICENSE_STRU ) * res_count ) ;

	for( int i = 0 ; i < res_count ; i ++)
	{
		int col = 0 ;
		pLicense_stru[ i ].no = recs[ i ][ col ++ ].toInt() ;
		pLicense_stru[ i ].period = recs[ i ][ col ++ ].toInt() ;
		strcpy( pLicense_stru[ i ].file_gen_time, _Q2C( recs[ i ][ col ++ ].toString() ) ) ;
		strcpy( pLicense_stru[ i ].file_imp_time, _Q2C( recs[ i ][ col ++ ].toString() ) ) ;
		strcpy( pLicense_stru[ i ].mac_addr, _Q2C( recs[ i ][ col ++ ].toString() ) ) ;
		pLicense_stru[ i ].expired_flag = recs[ i ][ col ++ ].toInt() ;
	}

	if ( pLicense_stru[ 0 ].period == -1 )
	{
		return ;
	}
	if ( pLicense_stru[ 0 ].expired_flag == 1 )//授权已过期
	{
		setGeometry( m_wnd_width / 2 ,  m_wnd_height / 2 , m_wnd_width , m_wnd_height ) ; //设置窗体固定大小
		show() ;
		return ;
	}
	QDateTime current_time = QDateTime::currentDateTime() ;
	QDateTime imp_time = QDateTime::fromString( _C2Q( pLicense_stru[ 0 ].file_imp_time ) ,  "yyyy-MM-dd hh:mm:ss" ) ;
	QDateTime deadline_time = imp_time.addDays( pLicense_stru[ 0 ].period ) ;
	int run_secs = current_time.secsTo( deadline_time ) ;
	if ( run_secs >= 0  &&  run_secs <= 10 * 24 * 60 * 60 )
	{
		int dead_day = current_time.daysTo( deadline_time ) - 1 ;
		int dead_hour = run_secs / 60 / 60 - dead_day * 24 ;
		int dead_min = run_secs / 60 - dead_day * 24 * 60 - dead_hour * 60 ;
		m_pSystem_tray->showMessage( tr( "D6000友情提示" ) , \
			tr( "D6000授权还剩%1天%2小时%3分钟,即将过期！" ).arg( dead_day ).arg( dead_hour ).arg( dead_min ) ,\
			QSystemTrayIcon::Information , 600000 ) ;
	}
	else if ( run_secs < 0 )
	{
		setGeometry( m_wnd_width / 2 ,  m_wnd_height / 2 , m_wnd_width , m_wnd_height ) ; //设置窗体固定大小
		pLicense_stru[ 0 ].expired_flag = 1 ;
		save_license_info( pLicense_stru ) ;
		show() ;
	}
	else
	{
		hide() ;
	}
}

void alarm_wnd::get_mac_addrs( void )
{
	m_mac_addrs.clear() ;
	QString strMac ;
	//获取所有网卡信息
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces() ;
	for ( int i = 0 ; i < ifaces.count() ; i++ )
	{
		QNetworkInterface iface = ifaces.at( i ) ;

		strMac = iface.hardwareAddress();
		if ( m_mac_addrs.indexOf( strMac ) == -1 )
		{
			m_mac_addrs.append( strMac ) ;
		}
	}
}

void alarm_wnd::slot_tray_clicked( void )
{
	m_bSystem_tray = true ;
}

bool alarm_wnd::save_license_info( LICENSE_STRU* pLicense_stru )
{
	int success = m_hdbi.startdb( Ddbi::G_DB_RW ) ;
	if ( !success )
	{
		return false ;
	}
	int i = 0,col = 0;
	QString strCode = "" ;
	GdbRecord record ;
	record.allocMem( 6 ) ;
	char sqlw[ 128 ] ;
	sprintf( sqlw, "select 组号,授权周期,文件生成时间,\
				   文件导入时间,MAC地址,是否过期 from 授权权限参数表 where 组号=0" ) ;

	m_hdbi.selectFrom( "授权权限参数表" ) ;

	GdbRecords recs ;
	if( !m_hdbi.readSQL( recs , sqlw ) || recs.recnum == 0 )
	{
		col = 0 ;
		FillGdbRecord( record,col,pLicense_stru->no ) ;
		FillGdbRecord( record,col,pLicense_stru->period ) ;
		FillGdbRecord( record,col,pLicense_stru->file_gen_time ) ;
		FillGdbRecord( record,col,pLicense_stru->file_imp_time ) ;
		FillGdbRecord( record,col,pLicense_stru->mac_addr ) ;
		FillGdbRecord( record,col,pLicense_stru->expired_flag ) ;

		if( m_hdbi.insertARecord( record ) )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
	else
	{
		col = 0 ;
		FillGdbRecord( record,col,pLicense_stru->no ) ;
		FillGdbRecord( record,col,pLicense_stru->period ) ;
		FillGdbRecord( record,col,pLicense_stru->file_gen_time ) ;
		FillGdbRecord( record,col,pLicense_stru->file_imp_time ) ;
		FillGdbRecord( record,col,pLicense_stru->mac_addr ) ;
		FillGdbRecord( record,col,pLicense_stru->expired_flag ) ;

		if( m_hdbi.updateARecord( recs[0] , record ) )
		{
			return true ;
		}
		else
		{
			return false ;
		}
	}
}