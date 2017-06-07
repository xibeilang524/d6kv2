/********************************************************************************
** Form generated from reading UI file 'DD_Dialog.ui'
**
** Created: Fri Apr 7 09:20:49 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DD_DIALOG_H
#define UI_DD_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dd_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *chkMD;
    QRadioButton *chkBD;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QCheckBox *chk_PT;
    QCheckBox *chk_MDL;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *chkMT;
    QCheckBox *chkMCT;
    QCheckBox *chkYT;
    QCheckBox *chkET;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *textLabel3;
    QDateEdit *dtST;
    QLabel *textLabel4;
    QDateEdit *dtET;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *textLabel1;
    QLineEdit *txtExpFile;
    QPushButton *btnExp;
    QLabel *textLabel1_3;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab2;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *chkImpAll;
    QHBoxLayout *horizontalLayout_5;
    QLabel *textLabel1_2;
    QLineEdit *txtImpFile;
    QPushButton *btnImp;
    QLabel *textLabel1_4;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAbout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *dd_Dialog)
    {
        if (dd_Dialog->objectName().isEmpty())
            dd_Dialog->setObjectName(QString::fromUtf8("dd_Dialog"));
        dd_Dialog->resize(498, 506);
        verticalLayout = new QVBoxLayout(dd_Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(dd_Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 80));
        groupBox->setMaximumSize(QSize(16777215, 80));
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        chkMD = new QRadioButton(groupBox);
        chkMD->setObjectName(QString::fromUtf8("chkMD"));
        chkMD->setChecked(true);

        horizontalLayout_3->addWidget(chkMD);

        chkBD = new QRadioButton(groupBox);
        chkBD->setObjectName(QString::fromUtf8("chkBD"));

        horizontalLayout_3->addWidget(chkBD);

        horizontalSpacer_3 = new QSpacerItem(351, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(dd_Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        groupBox_3->setMinimumSize(QSize(0, 100));
        groupBox_3->setMaximumSize(QSize(16777215, 100));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        chk_PT = new QCheckBox(groupBox_3);
        chk_PT->setObjectName(QString::fromUtf8("chk_PT"));
        chk_PT->setChecked(true);

        gridLayout->addWidget(chk_PT, 0, 0, 1, 1);

        chk_MDL = new QCheckBox(groupBox_3);
        chk_MDL->setObjectName(QString::fromUtf8("chk_MDL"));
        chk_MDL->setChecked(false);

        gridLayout->addWidget(chk_MDL, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(303, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 3);

        chkMT = new QCheckBox(groupBox_3);
        chkMT->setObjectName(QString::fromUtf8("chkMT"));
        chkMT->setChecked(true);

        gridLayout->addWidget(chkMT, 1, 0, 1, 1);

        chkMCT = new QCheckBox(groupBox_3);
        chkMCT->setObjectName(QString::fromUtf8("chkMCT"));
        chkMCT->setChecked(true);

        gridLayout->addWidget(chkMCT, 1, 1, 1, 1);

        chkYT = new QCheckBox(groupBox_3);
        chkYT->setObjectName(QString::fromUtf8("chkYT"));
        chkYT->setChecked(true);

        gridLayout->addWidget(chkYT, 1, 2, 1, 1);

        chkET = new QCheckBox(groupBox_3);
        chkET->setObjectName(QString::fromUtf8("chkET"));
        chkET->setChecked(true);

        gridLayout->addWidget(chkET, 1, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(137, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 4, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(dd_Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(0, 100));
        groupBox_2->setMaximumSize(QSize(16777215, 100));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textLabel3 = new QLabel(groupBox_2);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        horizontalLayout_2->addWidget(textLabel3);

        dtST = new QDateEdit(groupBox_2);
        dtST->setObjectName(QString::fromUtf8("dtST"));

        horizontalLayout_2->addWidget(dtST);

        textLabel4 = new QLabel(groupBox_2);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        horizontalLayout_2->addWidget(textLabel4);

        dtET = new QDateEdit(groupBox_2);
        dtET->setObjectName(QString::fromUtf8("dtET"));

        horizontalLayout_2->addWidget(dtET);

        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(3, 1);

        verticalLayout->addWidget(groupBox_2);

        tabWidget = new QTabWidget(dd_Dialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(0, 70));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        verticalLayout_2 = new QVBoxLayout(tab1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        textLabel1 = new QLabel(tab1);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        horizontalLayout_4->addWidget(textLabel1);

        txtExpFile = new QLineEdit(tab1);
        txtExpFile->setObjectName(QString::fromUtf8("txtExpFile"));
        txtExpFile->setDragEnabled(false);
        txtExpFile->setReadOnly(true);

        horizontalLayout_4->addWidget(txtExpFile);

        btnExp = new QPushButton(tab1);
        btnExp->setObjectName(QString::fromUtf8("btnExp"));
        btnExp->setStyleSheet(QString::fromUtf8("#btnExp{\n"
"background-image: url(:/res/background_normal.png);\n"
"border: 0px;\n"
"min-width: 58px;\n"
"min-height: 41px;\n"
"max-width: 58px;\n"
"max-height: 41px;\n"
"font-size: 14px;\n"
"border-radius: 3px;\n"
"}\n"
"#btnExp:hover\n"
"{\n"
"background-image: url(:/res/background_hover.png);\n"
"border: 0px;\n"
"}\n"
"#btnExp:pressed\n"
"{\n"
"background-image: url(:/res/background_press.png);\n"
"border: 0px;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnExp->setIcon(icon);

        horizontalLayout_4->addWidget(btnExp);


        verticalLayout_2->addLayout(horizontalLayout_4);

        textLabel1_3 = new QLabel(tab1);
        textLabel1_3->setObjectName(QString::fromUtf8("textLabel1_3"));
        textLabel1_3->setWordWrap(false);

        verticalLayout_2->addWidget(textLabel1_3);

        verticalSpacer_2 = new QSpacerItem(20, 19, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        verticalLayout_3 = new QVBoxLayout(tab2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        chkImpAll = new QCheckBox(tab2);
        chkImpAll->setObjectName(QString::fromUtf8("chkImpAll"));
        chkImpAll->setChecked(true);

        verticalLayout_3->addWidget(chkImpAll);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        textLabel1_2 = new QLabel(tab2);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        horizontalLayout_5->addWidget(textLabel1_2);

        txtImpFile = new QLineEdit(tab2);
        txtImpFile->setObjectName(QString::fromUtf8("txtImpFile"));
        txtImpFile->setDragEnabled(false);
        txtImpFile->setReadOnly(true);

        horizontalLayout_5->addWidget(txtImpFile);

        btnImp = new QPushButton(tab2);
        btnImp->setObjectName(QString::fromUtf8("btnImp"));
        btnImp->setStyleSheet(QString::fromUtf8("#btnImp{\n"
"background-image: url(:/res/background_normal.png);\n"
"border: 0px;\n"
"min-width: 58px;\n"
"min-height: 41px;\n"
"max-width: 58px;\n"
"max-height: 41px;\n"
"font-size: 14px;\n"
"border-radius: 3px;\n"
"}\n"
"#btnImp:hover\n"
"{\n"
"background-image: url(:/res/background_hover.png);\n"
"border: 0px;\n"
"}\n"
"#btnImp:pressed\n"
"{\n"
"background-image: url(:/res/background_press.png);\n"
"border: 0px;\n"
"}"));
        btnImp->setIcon(icon);

        horizontalLayout_5->addWidget(btnImp);


        verticalLayout_3->addLayout(horizontalLayout_5);

        textLabel1_4 = new QLabel(tab2);
        textLabel1_4->setObjectName(QString::fromUtf8("textLabel1_4"));
        textLabel1_4->setWordWrap(false);

        verticalLayout_3->addWidget(textLabel1_4);

        verticalSpacer_3 = new QSpacerItem(20, 24, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QString::fromUtf8("tab3"));
        horizontalLayout_6 = new QHBoxLayout(tab3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label = new QLabel(tab3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_6->addWidget(label);

        tabWidget->addTab(tab3, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonAbout = new QPushButton(dd_Dialog);
        buttonAbout->setObjectName(QString::fromUtf8("buttonAbout"));

        horizontalLayout->addWidget(buttonAbout);

        horizontalSpacer = new QSpacerItem(183, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOk = new QPushButton(dd_Dialog);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));

        horizontalLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(dd_Dialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout->addWidget(buttonCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(dd_Dialog);
        QObject::connect(buttonAbout, SIGNAL(clicked()), dd_Dialog, SLOT(about()));
        QObject::connect(buttonOk, SIGNAL(clicked()), dd_Dialog, SLOT(begin()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), dd_Dialog, SLOT(exit()));
        QObject::connect(btnExp, SIGNAL(clicked()), dd_Dialog, SLOT(exportDb()));
        QObject::connect(tabWidget, SIGNAL(currentChanged(int)), dd_Dialog, SLOT(tab_selChanged(int)));
        QObject::connect(btnImp, SIGNAL(clicked()), dd_Dialog, SLOT(importDb()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(dd_Dialog);
    } // setupUi

    void retranslateUi(QDialog *dd_Dialog)
    {
        dd_Dialog->setWindowTitle(QApplication::translate("dd_Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("dd_Dialog", "\346\225\260\346\215\256\345\272\223\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        chkMD->setText(QApplication::translate("dd_Dialog", "\344\270\273\345\272\223", 0, QApplication::UnicodeUTF8));
        chkBD->setText(QApplication::translate("dd_Dialog", "\345\244\207\345\272\223", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("dd_Dialog", "\346\225\260\346\215\256\345\272\223\350\241\250\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        chk_PT->setText(QApplication::translate("dd_Dialog", "\345\217\202\346\225\260\350\241\250", 0, QApplication::UnicodeUTF8));
        chk_MDL->setText(QApplication::translate("dd_Dialog", "\346\250\241\345\236\213\345\244\207\344\273\275", 0, QApplication::UnicodeUTF8));
        chkMT->setText(QApplication::translate("dd_Dialog", "\346\234\210\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        chkMCT->setText(QApplication::translate("dd_Dialog", "\346\234\210\347\273\237\350\256\241\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        chkYT->setText(QApplication::translate("dd_Dialog", "\345\271\264\346\225\260\346\215\256\350\241\250", 0, QApplication::UnicodeUTF8));
        chkET->setText(QApplication::translate("dd_Dialog", "\344\272\213\351\241\271\350\256\260\345\275\225\350\241\250", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("dd_Dialog", "\346\227\245\346\234\237\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("dd_Dialog", "\345\274\200\345\247\213\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("dd_Dialog", "\347\273\223\346\235\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("dd_Dialog", "\350\276\223\345\207\272\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        btnExp->setText(QString());
        textLabel1_3->setText(QApplication::translate("dd_Dialog", "\346\263\250\357\274\232\350\267\257\345\276\204\344\270\255\344\270\215\350\246\201\345\214\205\345\220\253\344\270\255\346\226\207\345\255\227\347\254\246", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("dd_Dialog", "\345\244\207\344\273\275", 0, QApplication::UnicodeUTF8));
        chkImpAll->setText(QApplication::translate("dd_Dialog", "\345\257\274\345\205\245\346\211\200\346\234\211\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("dd_Dialog", "\350\276\223\345\205\245\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        btnImp->setText(QString());
        textLabel1_4->setText(QApplication::translate("dd_Dialog", "\346\263\250\357\274\232\346\201\242\345\244\215\345\211\215\350\257\267\345\205\210\346\270\205\351\231\244\345\267\262\346\234\211\346\250\241\345\236\213\344\270\216\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("dd_Dialog", "\346\201\242\345\244\215", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("dd_Dialog", "\350\257\267\351\200\211\346\213\251\351\234\200\350\246\201\346\270\205\351\231\244\347\232\204\346\225\260\346\215\256\345\272\223\350\241\250\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("dd_Dialog", "\346\270\205\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonAbout->setText(QApplication::translate("dd_Dialog", "\345\205\263\344\272\216(&A)", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("dd_Dialog", "\345\274\200\345\247\213(&O)", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("dd_Dialog", "\351\200\200\345\207\272(&Q)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dd_Dialog: public Ui_dd_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DD_DIALOG_H
