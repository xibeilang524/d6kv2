// QLoginWnd.h: interface for the QLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
#define AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ui_qloginwndbase.h"

#include "scd/scdapi.h"

class QLoginWnd : public QDialog ,public Ui::QLoginWndBase
{
	Q_OBJECT

	int m_userSumNum;
	int m_hostUserNum;

	DISPATCHER_STRU	*m_operDispatcher;
	DISPATCHER_STRU *m_curOperDispatcher;
	USEROPERRIGHT_STRU	*m_operRight;

	int m_userTypes[ 10 ];
	int m_userTypeNum;

	int readUserPara();
public:
	QLoginWnd( QWidget* parent = NULL, const char* name = NULL, Qt::WindowFlags fl = 0 );
	virtual ~QLoginWnd();
public slots:
	virtual void onCbxType( int sel );
    virtual void onBtnOK();

};

#endif // !defined(AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
