/********************************************************************************
** Form generated from reading UI file 'qloginwnd.ui'
**
** Created: Fri Apr 7 09:20:13 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLOGINWND_H
#define UI_QLOGINWND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QLoginWnd
{
public:
    QLabel *textLabel1;
    QLabel *textLabel2;
    QLabel *textLabel3;
    QComboBox *m_cbxType;
    QComboBox *m_cbxName;
    QLineEdit *m_edtPassword;
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    void setupUi(QDialog *QLoginWnd)
    {
        if (QLoginWnd->objectName().isEmpty())
            QLoginWnd->setObjectName(QString::fromUtf8("QLoginWnd"));
        QLoginWnd->resize(282, 162);
        textLabel1 = new QLabel(QLoginWnd);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setGeometry(QRect(20, 20, 80, 21));
        textLabel2 = new QLabel(QLoginWnd);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setGeometry(QRect(20, 55, 80, 21));
        textLabel3 = new QLabel(QLoginWnd);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setGeometry(QRect(20, 88, 80, 21));
        m_cbxType = new QComboBox(QLoginWnd);
        m_cbxType->setObjectName(QString::fromUtf8("m_cbxType"));
        m_cbxType->setGeometry(QRect(120, 20, 150, 22));
        m_cbxName = new QComboBox(QLoginWnd);
        m_cbxName->setObjectName(QString::fromUtf8("m_cbxName"));
        m_cbxName->setGeometry(QRect(120, 54, 150, 22));
        m_edtPassword = new QLineEdit(QLoginWnd);
        m_edtPassword->setObjectName(QString::fromUtf8("m_edtPassword"));
        m_edtPassword->setGeometry(QRect(120, 89, 150, 20));
        m_btnOK = new QPushButton(QLoginWnd);
        m_btnOK->setObjectName(QString::fromUtf8("m_btnOK"));
        m_btnOK->setGeometry(QRect(40, 130, 75, 23));
        m_btnCancel = new QPushButton(QLoginWnd);
        m_btnCancel->setObjectName(QString::fromUtf8("m_btnCancel"));
        m_btnCancel->setGeometry(QRect(180, 130, 75, 23));

        retranslateUi(QLoginWnd);
        QObject::connect(m_btnOK, SIGNAL(clicked()), QLoginWnd, SLOT(onBtnOK()));
        QObject::connect(m_btnCancel, SIGNAL(clicked()), QLoginWnd, SLOT(reject()));

        QMetaObject::connectSlotsByName(QLoginWnd);
    } // setupUi

    void retranslateUi(QDialog *QLoginWnd)
    {
        QLoginWnd->setWindowTitle(QApplication::translate("QLoginWnd", "\347\224\250\346\210\267\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QLoginWnd", "\346\223\215\344\275\234\345\221\230\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("QLoginWnd", "\346\223\215\344\275\234\345\221\230\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("QLoginWnd", "\346\223\215\344\275\234\345\221\230\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        m_btnOK->setText(QApplication::translate("QLoginWnd", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        m_btnCancel->setText(QApplication::translate("QLoginWnd", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QLoginWnd: public Ui_QLoginWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLOGINWND_H
