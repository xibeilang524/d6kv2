/********************************************************************************
** Form generated from reading UI file 'QusercfgDlg.ui'
**
** Created: Fri Apr 7 09:20:12 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUSERCFGDLG_H
#define UI_QUSERCFGDLG_H

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
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_QUsercfgDlg
{
public:
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *m_cbxRoletype;
    QRadioButton *m_rdbtnGrplder;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_2;
    QLineEdit *m_editCode;
    QLineEdit *m_editRoledesc;
    QLineEdit *m_editPassword;
    QLineEdit *m_editDesc;
    QPushButton *m_btOk;
    QPushButton *m_btCancel;
    QLabel *label_5;

    void setupUi(QDialog *QUsercfgDlg)
    {
        if (QUsercfgDlg->objectName().isEmpty())
            QUsercfgDlg->setObjectName(QString::fromUtf8("QUsercfgDlg"));
        QUsercfgDlg->resize(495, 221);
        label_3 = new QLabel(QUsercfgDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 120, 54, 13));
        label_4 = new QLabel(QUsercfgDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(240, 80, 80, 13));
        m_cbxRoletype = new QComboBox(QUsercfgDlg);
        m_cbxRoletype->setObjectName(QString::fromUtf8("m_cbxRoletype"));
        m_cbxRoletype->setGeometry(QRect(330, 40, 121, 22));
        m_rdbtnGrplder = new QRadioButton(QUsercfgDlg);
        m_rdbtnGrplder->setObjectName(QString::fromUtf8("m_rdbtnGrplder"));
        m_rdbtnGrplder->setGeometry(QRect(330, 120, 89, 18));
        label = new QLabel(QUsercfgDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 40, 54, 13));
        label_6 = new QLabel(QUsercfgDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(260, 40, 60, 13));
        label_2 = new QLabel(QUsercfgDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 80, 54, 13));
        m_editCode = new QLineEdit(QUsercfgDlg);
        m_editCode->setObjectName(QString::fromUtf8("m_editCode"));
        m_editCode->setGeometry(QRect(110, 40, 111, 20));
        m_editRoledesc = new QLineEdit(QUsercfgDlg);
        m_editRoledesc->setObjectName(QString::fromUtf8("m_editRoledesc"));
        m_editRoledesc->setGeometry(QRect(330, 80, 121, 20));
        m_editPassword = new QLineEdit(QUsercfgDlg);
        m_editPassword->setObjectName(QString::fromUtf8("m_editPassword"));
        m_editPassword->setGeometry(QRect(110, 120, 111, 20));
        m_editDesc = new QLineEdit(QUsercfgDlg);
        m_editDesc->setObjectName(QString::fromUtf8("m_editDesc"));
        m_editDesc->setGeometry(QRect(110, 80, 111, 20));
        m_btOk = new QPushButton(QUsercfgDlg);
        m_btOk->setObjectName(QString::fromUtf8("m_btOk"));
        m_btOk->setGeometry(QRect(94, 180, 81, 31));
        m_btCancel = new QPushButton(QUsercfgDlg);
        m_btCancel->setObjectName(QString::fromUtf8("m_btCancel"));
        m_btCancel->setGeometry(QRect(284, 180, 81, 31));
        label_5 = new QLabel(QUsercfgDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 150, 381, 21));

        retranslateUi(QUsercfgDlg);
        QObject::connect(m_btCancel, SIGNAL(clicked()), QUsercfgDlg, SLOT(reject()));
        QObject::connect(m_btOk, SIGNAL(clicked()), QUsercfgDlg, SLOT(onBtOK()));

        QMetaObject::connectSlotsByName(QUsercfgDlg);
    } // setupUi

    void retranslateUi(QDialog *QUsercfgDlg)
    {
        QUsercfgDlg->setWindowTitle(QApplication::translate("QUsercfgDlg", "\347\224\250\346\210\267\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QUsercfgDlg", "\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("QUsercfgDlg", "\350\247\222\350\211\262\347\261\273\345\210\253\346\217\217\350\277\260\357\274\232", 0, QApplication::UnicodeUTF8));
        m_rdbtnGrplder->setText(QApplication::translate("QUsercfgDlg", "\346\230\257\345\220\246\347\273\204\351\225\277", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QUsercfgDlg", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("QUsercfgDlg", "\350\247\222\350\211\262\347\261\273\345\210\253\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QUsercfgDlg", "\346\217\217\350\277\260\357\274\232", 0, QApplication::UnicodeUTF8));
        m_editDesc->setText(QString());
        m_btOk->setText(QApplication::translate("QUsercfgDlg", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        m_btCancel->setText(QApplication::translate("QUsercfgDlg", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("QUsercfgDlg", "\350\257\264\346\230\216\357\274\232\345\257\206\347\240\201\345\277\205\351\241\273\345\214\205\345\220\253\345\244\247\345\206\231\345\255\227\346\257\215\343\200\201\345\260\217\345\206\231\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\357\274\214\344\270\224\344\270\215\345\276\227\345\260\221\344\272\2168\344\275\215\343\200\202", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QUsercfgDlg: public Ui_QUsercfgDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUSERCFGDLG_H
