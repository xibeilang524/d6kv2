/********************************************************************************
** Form generated from reading UI file 'rdb_scada_dialog_base.ui'
**
** Created: Fri Apr 7 09:20:30 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RDB_SCADA_DIALOG_BASE_H
#define UI_RDB_SCADA_DIALOG_BASE_H

#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Rdb_scada_dialog_base
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *textLabel4;
    QComboBox *m_pRdbtable_combobox;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *textLabel4_2;
    QComboBox *m_pField_combobox;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *m_pSel_all_button;
    QPushButton *m_pSel_none_button;
    QSpacerItem *horizontalSpacer;
    QListWidget *m_pCol_list;
    QHBoxLayout *horizontalLayout;
    QPushButton *m_pSave_button;
    QPushButton *m_pCancel_button;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *Rdb_scada_dialog_base)
    {
        if (Rdb_scada_dialog_base->objectName().isEmpty())
            Rdb_scada_dialog_base->setObjectName(QString::fromUtf8("Rdb_scada_dialog_base"));
        Rdb_scada_dialog_base->resize(420, 450);
        verticalLayout = new QVBoxLayout(Rdb_scada_dialog_base);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        textLabel4 = new QLabel(Rdb_scada_dialog_base);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        horizontalLayout_3->addWidget(textLabel4);

        m_pRdbtable_combobox = new QComboBox(Rdb_scada_dialog_base);
        m_pRdbtable_combobox->setObjectName(QString::fromUtf8("m_pRdbtable_combobox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pRdbtable_combobox->sizePolicy().hasHeightForWidth());
        m_pRdbtable_combobox->setSizePolicy(sizePolicy);
        m_pRdbtable_combobox->setMinimumSize(QSize(269, 0));

        horizontalLayout_3->addWidget(m_pRdbtable_combobox);

        horizontalSpacer_3 = new QSpacerItem(13, 14, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        textLabel4_2 = new QLabel(Rdb_scada_dialog_base);
        textLabel4_2->setObjectName(QString::fromUtf8("textLabel4_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textLabel4_2->sizePolicy().hasHeightForWidth());
        textLabel4_2->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(textLabel4_2);

        m_pField_combobox = new QComboBox(Rdb_scada_dialog_base);
        m_pField_combobox->setObjectName(QString::fromUtf8("m_pField_combobox"));
        sizePolicy.setHeightForWidth(m_pField_combobox->sizePolicy().hasHeightForWidth());
        m_pField_combobox->setSizePolicy(sizePolicy);
        m_pField_combobox->setMinimumSize(QSize(269, 0));

        horizontalLayout_4->addWidget(m_pField_combobox);

        horizontalSpacer_4 = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        m_pSel_all_button = new QPushButton(Rdb_scada_dialog_base);
        m_pSel_all_button->setObjectName(QString::fromUtf8("m_pSel_all_button"));

        horizontalLayout_2->addWidget(m_pSel_all_button);

        m_pSel_none_button = new QPushButton(Rdb_scada_dialog_base);
        m_pSel_none_button->setObjectName(QString::fromUtf8("m_pSel_none_button"));

        horizontalLayout_2->addWidget(m_pSel_none_button);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        m_pCol_list = new QListWidget(Rdb_scada_dialog_base);
        m_pCol_list->setObjectName(QString::fromUtf8("m_pCol_list"));

        verticalLayout->addWidget(m_pCol_list);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_pSave_button = new QPushButton(Rdb_scada_dialog_base);
        m_pSave_button->setObjectName(QString::fromUtf8("m_pSave_button"));

        horizontalLayout->addWidget(m_pSave_button);

        m_pCancel_button = new QPushButton(Rdb_scada_dialog_base);
        m_pCancel_button->setObjectName(QString::fromUtf8("m_pCancel_button"));

        horizontalLayout->addWidget(m_pCancel_button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Rdb_scada_dialog_base);

        QMetaObject::connectSlotsByName(Rdb_scada_dialog_base);
    } // setupUi

    void retranslateUi(QDialog *Rdb_scada_dialog_base)
    {
        Rdb_scada_dialog_base->setWindowTitle(QApplication::translate("Rdb_scada_dialog_base", "SCADA\345\256\236\346\227\266\350\241\250\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("Rdb_scada_dialog_base", "\345\256\236\346\227\266\350\241\250\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel4_2->setText(QApplication::translate("Rdb_scada_dialog_base", "\345\237\237    \345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        m_pSel_all_button->setText(QApplication::translate("Rdb_scada_dialog_base", "\345\205\250\351\200\211", 0, QApplication::UnicodeUTF8));
        m_pSel_none_button->setText(QApplication::translate("Rdb_scada_dialog_base", "\345\205\250\344\270\215\351\200\211", 0, QApplication::UnicodeUTF8));
        m_pSave_button->setText(QApplication::translate("Rdb_scada_dialog_base", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        m_pCancel_button->setText(QApplication::translate("Rdb_scada_dialog_base", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Rdb_scada_dialog_base: public Ui_Rdb_scada_dialog_base {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RDB_SCADA_DIALOG_BASE_H
