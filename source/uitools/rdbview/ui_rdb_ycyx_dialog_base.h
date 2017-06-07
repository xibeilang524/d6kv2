/********************************************************************************
** Form generated from reading UI file 'rdb_ycyx_dialog_base.ui'
**
** Created: Fri Apr 7 09:20:29 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RDB_YCYX_DIALOG_BASE_H
#define UI_RDB_YCYX_DIALOG_BASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Rdb_ycyx_dialog_base
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox1;
    QLabel *textLabel2_2;
    QCheckBox *m_pRefresh_checkbox;
    QLabel *textLabel2;
    QCheckBox *m_pStation_checkbox;
    QSpinBox *m_pTime_spinbox;
    QGroupBox *groupBox2;
    QCheckBox *m_pYcmanset_checkbox;
    QCheckBox *m_pYcxs_checkbox;
    QCheckBox *m_pYcky_checkbox;
    QCheckBox *m_pYccal_checkbox;
    QGroupBox *groupBox3;
    QCheckBox *m_pYxmanset_checkbox;
    QCheckBox *m_pKwhshow_checkbox;
    QCheckBox *m_pYxcal_checkbox;
    QPushButton *m_pOk_button;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_pCancel_button;

    void setupUi(QDialog *Rdb_ycyx_dialog_base)
    {
        if (Rdb_ycyx_dialog_base->objectName().isEmpty())
            Rdb_ycyx_dialog_base->setObjectName(QString::fromUtf8("Rdb_ycyx_dialog_base"));
        Rdb_ycyx_dialog_base->resize(402, 430);
        Rdb_ycyx_dialog_base->setMinimumSize(QSize(402, 430));
        Rdb_ycyx_dialog_base->setMaximumSize(QSize(402, 430));
        gridLayout = new QGridLayout(Rdb_ycyx_dialog_base);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox1 = new QGroupBox(Rdb_ycyx_dialog_base);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        textLabel2_2 = new QLabel(groupBox1);
        textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));
        textLabel2_2->setGeometry(QRect(360, 60, 20, 21));
        textLabel2_2->setWordWrap(false);
        m_pRefresh_checkbox = new QCheckBox(groupBox1);
        m_pRefresh_checkbox->setObjectName(QString::fromUtf8("m_pRefresh_checkbox"));
        m_pRefresh_checkbox->setEnabled(true);
        m_pRefresh_checkbox->setGeometry(QRect(20, 60, 190, 21));
        textLabel2 = new QLabel(groupBox1);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setGeometry(QRect(210, 60, 80, 21));
        textLabel2->setWordWrap(false);
        m_pStation_checkbox = new QCheckBox(groupBox1);
        m_pStation_checkbox->setObjectName(QString::fromUtf8("m_pStation_checkbox"));
        m_pStation_checkbox->setEnabled(true);
        m_pStation_checkbox->setGeometry(QRect(20, 30, 190, 21));
        m_pTime_spinbox = new QSpinBox(groupBox1);
        m_pTime_spinbox->setObjectName(QString::fromUtf8("m_pTime_spinbox"));
        m_pTime_spinbox->setGeometry(QRect(280, 60, 71, 20));
        m_pTime_spinbox->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(groupBox1, 0, 0, 1, 3);

        groupBox2 = new QGroupBox(Rdb_ycyx_dialog_base);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        m_pYcmanset_checkbox = new QCheckBox(groupBox2);
        m_pYcmanset_checkbox->setObjectName(QString::fromUtf8("m_pYcmanset_checkbox"));
        m_pYcmanset_checkbox->setEnabled(true);
        m_pYcmanset_checkbox->setGeometry(QRect(20, 50, 190, 21));
        m_pYcxs_checkbox = new QCheckBox(groupBox2);
        m_pYcxs_checkbox->setObjectName(QString::fromUtf8("m_pYcxs_checkbox"));
        m_pYcxs_checkbox->setEnabled(true);
        m_pYcxs_checkbox->setGeometry(QRect(210, 20, 170, 21));
        m_pYcky_checkbox = new QCheckBox(groupBox2);
        m_pYcky_checkbox->setObjectName(QString::fromUtf8("m_pYcky_checkbox"));
        m_pYcky_checkbox->setEnabled(true);
        m_pYcky_checkbox->setGeometry(QRect(210, 50, 170, 21));
        m_pYccal_checkbox = new QCheckBox(groupBox2);
        m_pYccal_checkbox->setObjectName(QString::fromUtf8("m_pYccal_checkbox"));
        m_pYccal_checkbox->setEnabled(true);
        m_pYccal_checkbox->setGeometry(QRect(20, 20, 180, 21));

        gridLayout->addWidget(groupBox2, 1, 0, 1, 3);

        groupBox3 = new QGroupBox(Rdb_ycyx_dialog_base);
        groupBox3->setObjectName(QString::fromUtf8("groupBox3"));
        m_pYxmanset_checkbox = new QCheckBox(groupBox3);
        m_pYxmanset_checkbox->setObjectName(QString::fromUtf8("m_pYxmanset_checkbox"));
        m_pYxmanset_checkbox->setEnabled(true);
        m_pYxmanset_checkbox->setGeometry(QRect(20, 50, 280, 21));
        m_pKwhshow_checkbox = new QCheckBox(groupBox3);
        m_pKwhshow_checkbox->setObjectName(QString::fromUtf8("m_pKwhshow_checkbox"));
        m_pKwhshow_checkbox->setEnabled(true);
        m_pKwhshow_checkbox->setGeometry(QRect(20, 80, 300, 21));
        m_pYxcal_checkbox = new QCheckBox(groupBox3);
        m_pYxcal_checkbox->setObjectName(QString::fromUtf8("m_pYxcal_checkbox"));
        m_pYxcal_checkbox->setEnabled(true);
        m_pYxcal_checkbox->setGeometry(QRect(20, 20, 300, 21));

        gridLayout->addWidget(groupBox3, 2, 0, 1, 3);

        m_pOk_button = new QPushButton(Rdb_ycyx_dialog_base);
        m_pOk_button->setObjectName(QString::fromUtf8("m_pOk_button"));

        gridLayout->addWidget(m_pOk_button, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(258, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 1, 1, 1);

        m_pCancel_button = new QPushButton(Rdb_ycyx_dialog_base);
        m_pCancel_button->setObjectName(QString::fromUtf8("m_pCancel_button"));

        gridLayout->addWidget(m_pCancel_button, 3, 2, 1, 1);


        retranslateUi(Rdb_ycyx_dialog_base);

        QMetaObject::connectSlotsByName(Rdb_ycyx_dialog_base);
    } // setupUi

    void retranslateUi(QDialog *Rdb_ycyx_dialog_base)
    {
        Rdb_ycyx_dialog_base->setWindowTitle(QApplication::translate("Rdb_ycyx_dialog_base", "\347\233\270\345\205\263\346\230\276\347\244\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        groupBox1->setTitle(QApplication::translate("Rdb_ycyx_dialog_base", "\345\256\236\346\227\266\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        textLabel2_2->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\347\247\222", 0, QApplication::UnicodeUTF8));
        m_pRefresh_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\345\256\232\346\227\266\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\345\210\267\346\226\260\345\221\250\346\234\237\357\274\232", 0, QApplication::UnicodeUTF8));
        m_pStation_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\276\347\244\272\346\214\211\345\216\202\347\253\231\346\216\222\345\272\217", 0, QApplication::UnicodeUTF8));
        groupBox2->setTitle(QApplication::translate("Rdb_ycyx_dialog_base", "\345\256\236\346\227\266\351\201\245\346\265\213\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        m_pYcmanset_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\351\224\201\345\256\232\346\240\207\345\277\227", 0, QApplication::UnicodeUTF8));
        m_pYcxs_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\351\201\245\346\265\213\347\263\273\346\225\260", 0, QApplication::UnicodeUTF8));
        m_pYcky_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\351\201\245\346\265\213\345\217\257\347\226\221", 0, QApplication::UnicodeUTF8));
        m_pYccal_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\350\256\241\347\256\227\346\240\207\345\277\227", 0, QApplication::UnicodeUTF8));
        groupBox3->setTitle(QApplication::translate("Rdb_ycyx_dialog_base", "\345\256\236\346\227\266\347\224\265\345\272\246/\351\201\245\344\277\241\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        m_pYxmanset_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\351\224\201\345\256\232\346\240\207\345\277\227", 0, QApplication::UnicodeUTF8));
        m_pKwhshow_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\347\224\265\345\272\246\346\230\276\347\244\272\346\226\271\345\274\217(\351\200\211\344\270\255\344\270\272\346\230\276\347\244\272\350\241\250\347\240\201)", 0, QApplication::UnicodeUTF8));
        m_pYxcal_checkbox->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\346\230\257\345\220\246\346\230\276\347\244\272\350\256\241\347\256\227\346\240\207\345\277\227", 0, QApplication::UnicodeUTF8));
        m_pOk_button->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        m_pCancel_button->setText(QApplication::translate("Rdb_ycyx_dialog_base", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Rdb_ycyx_dialog_base: public Ui_Rdb_ycyx_dialog_base {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RDB_YCYX_DIALOG_BASE_H
