// logindialog.cpp: implementation of the LoginDialog class.
//
//////////////////////////////////////////////////////////////////////

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QMap>

#include "logindialog.h"

#include "net/netapi.h"
#include "uitools/dmessagebox.h"
#include "datacurveapp.h"
#include "auth.h"

static const int LOGIN_FAIL_LIMIT = 3;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LoginDialog::LoginDialog(QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	init(this);
}

LoginDialog::~LoginDialog()
{
	
}

void LoginDialog::init(QWidget* parent)
{
	parent->setWindowTitle(tr("操作员登录"));

	lblUserStyle=new QLabel(tr("操作员类型"),parent);
	lblUserStyle->setObjectName(QString::fromUtf8("lblUserStyle"));

	boxUserStyle=new QComboBox(parent);
	boxUserStyle->setObjectName(QString::fromUtf8("boxUserStyle"));
	boxUserStyle->setEditable(false);
	initUserStyle();
	connect(boxUserStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(boxUserStyleIdxChanged(int)));

	lblUser=new QLabel(tr("操作员名称"),parent);
	lblUser->setObjectName(QString::fromUtf8("lblUser"));
	
	boxUser=new QComboBox(parent);
	boxUser->setObjectName(QString::fromUtf8("boxUser"));
	boxUser->setEditable(false);

	lblPwd=new QLabel(tr("操作员密码"),parent);
	lblPwd->setObjectName(QString::fromUtf8("lblPwd"));
	
	editPwd=new QLineEdit(this);
	editPwd->setObjectName(QString::fromUtf8("editPwd"));
	editPwd->setEchoMode(QLineEdit::Password);
	editPwd->setFocus();

	buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Vertical,parent);
	buttonBox->setObjectName(QString::fromUtf8("editPwd"));
	connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

	QGridLayout *gridLayout=new QGridLayout;
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

	gridLayout->addWidget(lblUserStyle,0,0);
	gridLayout->addWidget(boxUserStyle,0,1);
	gridLayout->addWidget(lblUser,1,0);
	gridLayout->addWidget(boxUser,1,1);
	gridLayout->addWidget(lblPwd,2,0);
	gridLayout->addWidget(editPwd,2,1);
	
	QHBoxLayout *hLayout=new QHBoxLayout;
	hLayout->addLayout(gridLayout);
	hLayout->addWidget(buttonBox);


	parent->setLayout(hLayout);
	if(boxUserStyle->count()>0)
		boxUserStyleIdxChanged(0);
}

void LoginDialog::boxUserStyleIdxChanged( int index )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>qApp;
	if(index<0)	return;

	char hostname[HOSTNAMELEN];
	_get_host_name(hostname);
	QString thishost=C2G(hostname);
	boxUser->clear();

	uint usertype=boxUserStyle->itemData(index).toUInt();

	for(int i=0;i<myApp->userparas.size();i++)
	{
		if(myApp->userparas[i].type==usertype)
		{
			for(int j=0;j<myApp->rightparas.size();j++)
			{
				if((myApp->userparas[i].username==myApp->rightparas[j].username) && ((myApp->rightparas[j].hostname)==thishost))
				{	
					boxUser->addItem(myApp->userparas[i].userdesc,QVariant(myApp->userparas[i].username));
					break;
				}
			}

		}
	}

}

void LoginDialog::buttonClicked( QAbstractButton* button )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>qApp;
	RightPara &rightparas=(static_cast<DataCurveApp*>qApp)->rightparas;

	QDialogButtonBox::ButtonRole role=buttonBox->buttonRole(button);
	if(role==QDialogButtonBox::RejectRole)
	{
		username="";
		password="";
		reject();
		return;
	}
	else if(role==QDialogButtonBox::AcceptRole)
	{
		QString selUser,selPwd;
		username=boxUser->itemData(boxUser->currentIndex()).toString();
		password=editPwd->text().trimmed();
		//后门
		if(password.compare(MANPWD,Qt::CaseInsensitive)==0)
		{
			username=MANUSER;
			password=MANPWD;
			accept();
			return;
		}

		static int login_fail_count = 0;

		for(int i=0;i<rightparas.size();i++)
		{
			if((rightparas[i].username==username) && (0 == user_auth(G2C(username), G2C(password), G2C(rightparas[i].password))))
			{
				if(!myApp->checkLoginUserRight(BROWER_HISTORY_CURVE,&(rightparas[i])))
				{
					DMessageBox::information(this,tr("登录提示"),tr("登录用户没有浏览历史曲线数据的权限，请向管理员申请或重新登录"),
						QMessageBox::Ok,QMessageBox::NoButton);
					return;
				}
				accept();
				return;
			}
		}

		login_fail_count++;
		
		if (login_fail_count < LOGIN_FAIL_LIMIT)
		{
			DMessageBox::warning(this,tr("警告"), tr("密码错误！！！\n请重新输入密码"));
			editPwd->clear();
		}
		else
		{
			DMessageBox::warning( this, tr( "警告" ), tr( "鉴别失败次数过多，请联系管理员重置口令" ));

			//I really hate to do that....please forgive me
			exit(-1);
		}
		
		return;
	}
	return;
}

void LoginDialog::initUserStyle()
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>qApp;
	NetStatus netstat;
	char hostname[HOSTNAMELEN];
	_get_host_name(hostname);
	QString thishost=C2G(hostname);
	
	
	boxUserStyle->clear();
	
	QMap<int,QString> mapType;
	for(int i=0;i<myApp->userparas.size();i++)
	{
		for(int j=0;j<myApp->rightparas.size();j++)
		{
			if((myApp->userparas[i].username==myApp->rightparas[j].username) && ((myApp->rightparas[j].hostname)==thishost))
			{	
				if (!mapType.contains(myApp->userparas[i].type))
				{
					mapType[myApp->userparas[i].type] = myApp->userparas[i].typedesc;
				}
				break;
			}
		}
	}

	QList<int> keyIndex = mapType.keys();
	for (int k=0;k<keyIndex.count();k++)
	{
// 		boxUserStyle->addItem(mapType.value(keyIndex[k]),QVariant(k));
		boxUserStyle->addItem(mapType.value(keyIndex[k]),QVariant(keyIndex[k]));
	}

}

void LoginDialog::updUser( int userStyle )
{
	
}




