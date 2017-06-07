#ifndef LICENSE_IMP_H
#define LICENSE_IMP_H

#include "ui_license_import.h"
#include "rwxml.h"
#include "license_imp_def.h"
#include "ddbi/ddbi.h"

class License_imp : public QMainWindow , public Ui::License_imp_base_dialog
{
	Q_OBJECT
public:
	License_imp( QWidget* parent = 0 ) ;
protected:

private:
	void get_mac_addrs( void ) ;
	bool save_imp_info( LICENSE_STRU* ) ;

private slots:
	void slot_license_imp( void ) ;
	void slot_license_check( void ) ;
	void slot_choose_file( void ) ;

private:
	QStringList	m_mac_addrs ;
	Ddbi				m_hdbi ;
};

#endif