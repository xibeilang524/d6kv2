/********************************************************************************
** Form generated from reading UI file 'qloginwndbase.ui'
**
** Created: Fri Apr 7 09:20:26 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QLOGINWNDBASE_H
#define UI_QLOGINWNDBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_QLoginWndBase
{
public:
    QGridLayout *gridLayout;
    QComboBox *m_cbxType;
    QLabel *textLabel1;
    QComboBox *m_cbxName;
    QLabel *textLabel2;
    QLabel *textLabel3;
    QLineEdit *m_edtPassword;
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    void setupUi(QDialog *QLoginWndBase)
    {
        if (QLoginWndBase->objectName().isEmpty())
            QLoginWndBase->setObjectName(QString::fromUtf8("QLoginWndBase"));
        QLoginWndBase->resize(293, 131);
        QLoginWndBase->setMinimumSize(QSize(293, 131));
        QIcon icon;
        icon.addFile(QString::fromUtf8("png/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        QLoginWndBase->setWindowIcon(icon);
        gridLayout = new QGridLayout(QLoginWndBase);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_cbxType = new QComboBox(QLoginWndBase);
        m_cbxType->setObjectName(QString::fromUtf8("m_cbxType"));
        m_cbxType->setFocusPolicy(Qt::StrongFocus);

        gridLayout->addWidget(m_cbxType, 0, 1, 1, 2);

        textLabel1 = new QLabel(QLoginWndBase);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

        m_cbxName = new QComboBox(QLoginWndBase);
        m_cbxName->setObjectName(QString::fromUtf8("m_cbxName"));

        gridLayout->addWidget(m_cbxName, 1, 1, 1, 2);

        textLabel2 = new QLabel(QLoginWndBase);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 0, 1, 1);

        textLabel3 = new QLabel(QLoginWndBase);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout->addWidget(textLabel3, 2, 0, 1, 1);

        m_edtPassword = new QLineEdit(QLoginWndBase);
        m_edtPassword->setObjectName(QString::fromUtf8("m_edtPassword"));
        m_edtPassword->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(m_edtPassword, 2, 1, 1, 2);

        m_btnOK = new QPushButton(QLoginWndBase);
        m_btnOK->setObjectName(QString::fromUtf8("m_btnOK"));

        gridLayout->addWidget(m_btnOK, 4, 0, 1, 1);

        m_btnCancel = new QPushButton(QLoginWndBase);
        m_btnCancel->setObjectName(QString::fromUtf8("m_btnCancel"));

        gridLayout->addWidget(m_btnCancel, 4, 2, 1, 1);

        QWidget::setTabOrder(m_edtPassword, m_btnOK);
        QWidget::setTabOrder(m_btnOK, m_btnCancel);
        QWidget::setTabOrder(m_btnCancel, m_cbxType);
        QWidget::setTabOrder(m_cbxType, m_cbxName);

        retranslateUi(QLoginWndBase);

        QMetaObject::connectSlotsByName(QLoginWndBase);
    } // setupUi

    void retranslateUi(QDialog *QLoginWndBase)
    {
        QLoginWndBase->setWindowTitle(QApplication::translate("QLoginWndBase", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QLoginWndBase", "\346\223\215\344\275\234\345\221\230\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("QLoginWndBase", "\346\223\215\344\275\234\345\221\230\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("QLoginWndBase", "\346\223\215\344\275\234\345\221\230\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        m_btnOK->setText(QApplication::translate("QLoginWndBase", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        m_btnCancel->setText(QApplication::translate("QLoginWndBase", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QLoginWndBase: public Ui_QLoginWndBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QLOGINWNDBASE_H
