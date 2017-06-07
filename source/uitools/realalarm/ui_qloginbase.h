/********************************************************************************
** Form generated from reading UI file 'qloginbase.ui'
**
** Created: Fri Apr 7 09:21:04 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLOGINBASE_H
#define UI_QLOGINBASE_H

#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_QLoginBase
{
public:
    QGridLayout *gridLayout;
    QLabel *textLabel3;
    QComboBox *m_userTypeList;
    QComboBox *m_userList;
    QLineEdit *m_userPasswd;
    QLabel *textLabel2;
    QLabel *textLabel1;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_cancelBtn;
    QPushButton *m_okBtn;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *QLoginBase)
    {
        if (QLoginBase->objectName().isEmpty())
            QLoginBase->setObjectName(QString::fromUtf8("QLoginBase"));
        QLoginBase->resize(281, 183);
        QLoginBase->setModal(true);
        gridLayout = new QGridLayout(QLoginBase);
        gridLayout->setSpacing(10);
        gridLayout->setContentsMargins(20, 20, 20, 20);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel3 = new QLabel(QLoginBase);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textLabel3->sizePolicy().hasHeightForWidth());
        textLabel3->setSizePolicy(sizePolicy);
        textLabel3->setWordWrap(false);

        gridLayout->addWidget(textLabel3, 2, 0, 1, 1);

        m_userTypeList = new QComboBox(QLoginBase);
        m_userTypeList->setObjectName(QString::fromUtf8("m_userTypeList"));

        gridLayout->addWidget(m_userTypeList, 0, 1, 1, 1);

        m_userList = new QComboBox(QLoginBase);
        m_userList->setObjectName(QString::fromUtf8("m_userList"));

        gridLayout->addWidget(m_userList, 1, 1, 1, 1);

        m_userPasswd = new QLineEdit(QLoginBase);
        m_userPasswd->setObjectName(QString::fromUtf8("m_userPasswd"));
        m_userPasswd->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_userPasswd, 2, 1, 1, 1);

        textLabel2 = new QLabel(QLoginBase);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        sizePolicy.setHeightForWidth(textLabel2->sizePolicy().hasHeightForWidth());
        textLabel2->setSizePolicy(sizePolicy);
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 0, 1, 1);

        textLabel1 = new QLabel(QLoginBase);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        sizePolicy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
        textLabel1->setSizePolicy(sizePolicy);
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_cancelBtn = new QPushButton(QLoginBase);
        m_cancelBtn->setObjectName(QString::fromUtf8("m_cancelBtn"));

        horizontalLayout->addWidget(m_cancelBtn);

        m_okBtn = new QPushButton(QLoginBase);
        m_okBtn->setObjectName(QString::fromUtf8("m_okBtn"));

        horizontalLayout->addWidget(m_okBtn);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

        QWidget::setTabOrder(m_userPasswd, m_okBtn);
        QWidget::setTabOrder(m_okBtn, m_cancelBtn);
        QWidget::setTabOrder(m_cancelBtn, m_userTypeList);
        QWidget::setTabOrder(m_userTypeList, m_userList);

        retranslateUi(QLoginBase);

        QMetaObject::connectSlotsByName(QLoginBase);
    } // setupUi

    void retranslateUi(QDialog *QLoginBase)
    {
        QLoginBase->setWindowTitle(QApplication::translate("QLoginBase", "\347\224\250\346\210\267\347\231\273\351\231\206", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("QLoginBase", "\347\224\250\346\210\267\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("QLoginBase", "\347\224\250\346\210\267\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QLoginBase", "\347\224\250\346\210\267\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        m_cancelBtn->setText(QApplication::translate("QLoginBase", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        m_okBtn->setText(QApplication::translate("QLoginBase", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QLoginBase: public Ui_QLoginBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLOGINBASE_H
