#include "customFunctionDialog.h"

#include <QAbstractButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialogButtonBox>

CustomFunctionDialog::CustomFunctionDialog( QWidget * parent /*= 0*/, Qt::WindowFlags f /*= 0 */ )
{
	init();
}

CustomFunctionDialog::~CustomFunctionDialog()
{

}

void CustomFunctionDialog::init()
{
	if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("CustomFunctionDialog"));
    resize(375, 231);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
	
    gridLayout->addWidget(label, 0, 0, 1, 1);
	
    funcNameEdit = new QLineEdit(this);
    funcNameEdit->setObjectName(QString::fromUtf8("funcNameEdit"));
	
    gridLayout->addWidget(funcNameEdit, 0, 1, 1, 1);
	
    label_2 = new QLabel(this);
    label_2->setObjectName(QString::fromUtf8("label_2"));
	
    gridLayout->addWidget(label_2, 1, 0, 1, 1);
	
    funcDefEdit = new QTextEdit(this);
    funcDefEdit->setObjectName(QString::fromUtf8("funcDefEdit"));
	
    gridLayout->addWidget(funcDefEdit, 1, 1, 1, 1);
	
    label_3 = new QLabel(this);
    label_3->setObjectName(QString::fromUtf8("label_3"));
	
    gridLayout->addWidget(label_3, 2, 0, 1, 1);
	
    funcHelpEdit = new QTextEdit(this);
    funcHelpEdit->setObjectName(QString::fromUtf8("funcHelpEdit"));
	
    gridLayout->addWidget(funcHelpEdit, 2, 1, 1, 1);
	
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));

    gridLayout->addWidget(buttonBox, 3, 1, 1, 1);

	setWindowTitle(tr(""));
	label->setText(tr("函数名"));
	label_2->setText(tr("函数定义"));
	label_3->setText(tr("函数帮助"));

	
}

void CustomFunctionDialog::buttonClicked( QAbstractButton* button )
{
	QDialogButtonBox::ButtonRole role=buttonBox->buttonRole(button);
	if(role==QDialogButtonBox::RejectRole)
		reject();
	else if(role==QDialogButtonBox::AcceptRole)
		accept();
}
