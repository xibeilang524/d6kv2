#ifndef LICENSE_GEN_H
#define LICENSE_GEN_H

#include "ui_licensegen.h"

class DLicense_gen : public QMainWindow , public Ui::LicGenBaseWnd
{
	Q_OBJECT
public:
	DLicense_gen( QWidget* parent = 0 ) ;
protected:

private:
	void ini_main_window( void ) ;
	void gen_license( void ) ;
	void check_license( void ) ;
private:

private slots:
	void on_change_widget( int ) ;
	void on_ok_btn( void ) ;
	void on_cancel_btn( void ) ;
	void on_get_macaddr( void ) ;
	void on_get_genpath( void ) ;
	void on_get_checkpath( void ) ;
};

#endif