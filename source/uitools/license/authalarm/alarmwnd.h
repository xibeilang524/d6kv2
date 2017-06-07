#ifndef ALARMWND_H
#define ALARMWND_H

#include "ui_alarm.h"

#include <QMutex>
#include <QLibrary>
#include <QTimer>
#include <QSystemTrayIcon>

#include "ddbi/ddbi.h"

typedef struct _LICENSE_STRU
{
	short no ;
	short period ;
	char file_gen_time[ 24 ] ;
	char file_imp_time[ 24 ] ;
	char mac_addr[ 24 ] ;
	short expired_flag ;
}LICENSE_STRU ;


#define FillGdbRecord( record,column,item )\
	record[column] = item ;\
	column ++ ;\

class alarm_wnd : public QMainWindow , public Ui::alarmBaseWnd
{
	Q_OBJECT
public:
	alarm_wnd( QWidget* parent = 0 ) ;

protected:

private:
	void ini_alarm_wnd( void ) ;
	void get_mac_addrs( void ) ;
	bool save_license_info( LICENSE_STRU* pLicense_stru ) ;

private:
	Ddbi				m_hdbi ;
	int					m_wnd_height ;
	int					m_wnd_width ;
	bool					m_bSystem_tray ;
	QStringList	m_mac_addrs ;
	QTimer*			m_pCheck_timer ;
	QSystemTrayIcon * m_pSystem_tray ;

private slots:
	void slot_period_check( void ) ;
	void slot_tray_clicked( void ) ;
};

#endif