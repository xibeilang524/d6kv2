// DbLoginDlg.cpp: implementation of the DbLoginDlg class.
//
//////////////////////////////////////////////////////////////////////
#include <QString>
#include <QMessageBox>
#include <QDebug>


#include "DbLoginDlg.h"
#include "ui_Login_Dialog.h"
#include "uitools/dmessagebox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DbLoginDlg::DbLoginDlg(QWidget *parent)
:QDialog(parent),m_user(tr("")),m_pwd(tr(""))
{
	login_ui = new Ui::LoginDialog;
	login_ui->setupUi(this);
}

DbLoginDlg::~DbLoginDlg()
{
	delete login_ui;
	login_ui=NULL;
}

void DbLoginDlg::accept()
{
	m_user=login_ui->txtUsername->text().trimmed();
	m_pwd=login_ui->txtPassword->text().trimmed();
	if(m_user.isEmpty() || m_pwd.isEmpty())
	{
// 		QMessageBox msgBox;
// 		msgBox.setText(tr("用户名和密码不能为空，请重新输入！"));
// 		msgBox.exec();
		DMessageBox::information( this , tr( "提示" ) ,  tr("用户名和密码不能为空，请重新输入！") ) ;
		return;
	}
	done(0);
}

void DbLoginDlg::reject()
{
	done(1);
}