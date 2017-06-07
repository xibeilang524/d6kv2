#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QComboBox;
class QLineEdit;
class QLabel;

class LoginDialog : public QDialog  
{
	Q_OBJECT
public:
	LoginDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~LoginDialog();
public:

private slots:
	void boxUserStyleIdxChanged(int index);
	void buttonClicked(QAbstractButton* button);
private:
	void init(QWidget* parent);
	void initUserStyle();
	void updUser(int userStyle);
public:
	QString username;
	QString password;
private:
	QComboBox *boxUserStyle,*boxUser;
	QLineEdit *editPwd;
	QLabel    *lblUserStyle,*lblUser,*lblPwd;
	QDialogButtonBox *buttonBox;
};

#endif 
