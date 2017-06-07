// QLoginWnd.h: interface for the QLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

//#ifndef QLOGINWNDBASE_H
//#define QLOGINWNDBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#if !defined(AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
#define AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_

#include <QtGui>
#include <QVariant>
#include <QDialog>
#include "ui_qloginwnd.h"

#include "scd/scdapi.h"

class QLoginWnd : public QDialog  
{
	Q_OBJECT

	int m_userSumNum;//数据库中所有用户数
	int m_hostUserNum;//本节点用户数

	DISPATCHER_STRU	*m_operDispatcher;
	DISPATCHER_STRU *m_curOperDispatcher;
	USEROPERRIGHT_STRU	*m_operRight;

	int m_userTypes[ 10 ];
	int m_userTypeNum;


	int readUserPara();
public:
	QLoginWnd( QWidget* parent = NULL, const char* name = NULL, Qt::WindowFlags fl = 0 );
	virtual ~QLoginWnd();

	int passtype;
	bool ismonitor;

public slots:
    void onCbxType( int );
    void onBtnOK();

private:
	Ui::QLoginWnd ui;

};

#endif // !defined(AFX_QLOGINWND_H__8FE96AA0_081C_46B3_9D53_30BA1CD2DEEB__INCLUDED_)
