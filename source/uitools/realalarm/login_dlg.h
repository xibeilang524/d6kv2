// login_dlg.h: interface for the QLoginDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QLOGINDLG_H__9F6BCD44_B76A_4E9E_B3E8_60D6835FC92A__INCLUDED_)
#define AFX_QLOGINDLG_H__9F6BCD44_B76A_4E9E_B3E8_60D6835FC92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_qloginbase.h"
#include "scd/scdapi.h"

class Login_dlg : public QDialog, public Ui::QLoginBase  
{
	Q_OBJECT
public:
	int m_userSumNum;
	int m_hostUserNum;
	
	DISPATCHER_STRU	*m_operDispatcher;
	DISPATCHER_STRU *m_curOperDispatcher;
	USEROPERRIGHT_STRU	*m_operRight;
	USEROPERRIGHT_STRU	*m_curRight;
	
	int m_userTypes[ 10 ];
	int m_userTypeNum;
	
	int readUserPara();
public:
	Login_dlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
	virtual ~Login_dlg();

	virtual int exec();
	void getUserRight( USEROPERRIGHT_STRU& userright );
	void getOperInfo( DISPATCHER_STRU & operinfo );

public slots:
	void onUserTypeChanged( int item );
	void onOk();



private:

	DISPATCHER_STRU    m_curOperMan;
	USEROPERRIGHT_STRU m_curPower;

	bool readFile( char *filename, void ** buf );
	bool readFile( char *filename, void * buf, int len );
};

#endif // !defined(AFX_QLOGINDLG_H__9F6BCD44_B76A_4E9E_B3E8_60D6835FC92A__INCLUDED_)
