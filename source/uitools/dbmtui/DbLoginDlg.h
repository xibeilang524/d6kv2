// DbLoginDlg.h: interface for the DbLoginDlg class.
//
//////////////////////////////////////////////////////////////////////
#ifndef PBLOGINDLG_H
#define PBLOGINDLG_H

#include <QDialog>

namespace Ui {class LoginDialog;}

class DbLoginDlg : public QDialog  
{
	Q_OBJECT
public:
	DbLoginDlg(QWidget *parent = 0);
	virtual ~DbLoginDlg();
public:
	virtual void accept();
    virtual void reject();
public:
	QString m_user;
	QString m_pwd;
private:
	Ui::LoginDialog *login_ui;
};

#endif
