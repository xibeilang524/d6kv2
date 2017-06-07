// QUsercfgDlg.h: interface for the QLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

//#ifndef QLOGINWNDBASE_H
//#define QLOGINWNDBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#if !defined(AFX_QUSERCFGDLG_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
#define AFX_QUSERCFGDLG_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_

#include "QZonePara.h"
#include <QtGui>
#include <QVariant>
#include <QDialog>
#include "ui_QusercfgDlg.h"

#include "scd/scdapi.h"

class QUsercfgDlg : public QDialog  
{
	Q_OBJECT

	int m_roleNum;	// ljs [4/16/2012]

	DISPATCHER_STRU	*m_operDispatcher;
	DISPATCHER_STRU *m_curOperDispatcher;
	USEROPERRIGHT_STRU	*m_operRight;

	int m_userTypes[ 10 ];
	int m_userTypeNum;

public:
	QUsercfgDlg( QWidget* parent = NULL, const char* name = NULL, Qt::WindowFlags fl = 0 );
	
	virtual ~QUsercfgDlg();

	void refreshUserCfg(PUSERPARA pModify);

	PUSERPARA pUser;

	QComboBox *m_roltype;

public slots:
    virtual void onBtOK();
private:
	Ui::QUsercfgDlg ui;
	QString m_strCurrentPassword;

};

#endif // !defined(AFX_QUSERCFGDLG_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
