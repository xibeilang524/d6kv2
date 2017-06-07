/********************************************************************************
** Form generated from reading UI file 'PB_Dialog.ui'
**
** Created: Fri Apr 7 09:20:49 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PB_DIALOG_H
#define UI_PB_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
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

class Ui_pb_Dialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QRadioButton *chkMD;
    QRadioButton *chkBD;
    QGroupBox *groupBox_3;
    QCheckBox *chkYT;
    QCheckBox *chkMT;
    QCheckBox *chkET;
    QCheckBox *chkDT;
    QCheckBox *chk_PT;
    QGroupBox *groupBox_2;
    QLabel *textLabel3;
    QDateEdit *dtST;
    QLabel *textLabel4;
    QDateEdit *dtET;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QLineEdit *txtExpFile;
    QPushButton *btnExp;
    QLabel *textLabel1;
    QWidget *tab2;
    QLabel *textLabel1_2;
    QPushButton *btnImp;
    QLineEdit *txtImpFile;
    QCheckBox *chkImpAll;
    QWidget *tab3;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAbout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *pb_Dialog)
    {
        if (pb_Dialog->objectName().isEmpty())
            pb_Dialog->setObjectName(QString::fromUtf8("pb_Dialog"));
        pb_Dialog->resize(446, 356);
        layoutWidget = new QWidget(pb_Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(1, 6, 436, 346));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 43));
        groupBox->setMaximumSize(QSize(16777215, 43));
        chkMD = new QRadioButton(groupBox);
        chkMD->setObjectName(QString::fromUtf8("chkMD"));
        chkMD->setGeometry(QRect(15, 15, 54, 18));
        chkMD->setChecked(true);
        chkBD = new QRadioButton(groupBox);
        chkBD->setObjectName(QString::fromUtf8("chkBD"));
        chkBD->setGeometry(QRect(100, 15, 60, 16));

        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(0, 81));
        groupBox_3->setMaximumSize(QSize(16777215, 81));
        chkYT = new QCheckBox(groupBox_3);
        chkYT->setObjectName(QString::fromUtf8("chkYT"));
        chkYT->setGeometry(QRect(216, 46, 75, 20));
        chkYT->setChecked(true);
        chkMT = new QCheckBox(groupBox_3);
        chkMT->setObjectName(QString::fromUtf8("chkMT"));
        chkMT->setGeometry(QRect(122, 46, 75, 20));
        chkMT->setChecked(true);
        chkET = new QCheckBox(groupBox_3);
        chkET->setObjectName(QString::fromUtf8("chkET"));
        chkET->setGeometry(QRect(313, 45, 89, 20));
        chkET->setChecked(true);
        chkDT = new QCheckBox(groupBox_3);
        chkDT->setObjectName(QString::fromUtf8("chkDT"));
        chkDT->setGeometry(QRect(20, 46, 78, 20));
        chkDT->setChecked(true);
        chk_PT = new QCheckBox(groupBox_3);
        chk_PT->setObjectName(QString::fromUtf8("chk_PT"));
        chk_PT->setGeometry(QRect(20, 20, 66, 20));
        chk_PT->setChecked(true);

        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 58));
        groupBox_2->setMaximumSize(QSize(16777215, 58));
        textLabel3 = new QLabel(groupBox_2);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setGeometry(QRect(15, 25, 57, 16));
        textLabel3->setWordWrap(false);
        dtST = new QDateEdit(groupBox_2);
        dtST->setObjectName(QString::fromUtf8("dtST"));
        dtST->setGeometry(QRect(70, 20, 110, 22));
        dtST->setCurrentSection(QDateTimeEdit::YearSection);
        textLabel4 = new QLabel(groupBox_2);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setGeometry(QRect(200, 25, 56, 16));
        textLabel4->setWordWrap(false);
        dtET = new QDateEdit(groupBox_2);
        dtET->setObjectName(QString::fromUtf8("dtET"));
        dtET->setGeometry(QRect(255, 20, 110, 22));

        verticalLayout->addWidget(groupBox_2);

        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        txtExpFile = new QLineEdit(tab1);
        txtExpFile->setObjectName(QString::fromUtf8("txtExpFile"));
        txtExpFile->setGeometry(QRect(65, 60, 331, 21));
        txtExpFile->setDragEnabled(false);
        txtExpFile->setReadOnly(true);
        btnExp = new QPushButton(tab1);
        btnExp->setObjectName(QString::fromUtf8("btnExp"));
        btnExp->setGeometry(QRect(395, 60, 29, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/OPEN.xpm"), QSize(), QIcon::Normal, QIcon::Off);
        btnExp->setIcon(icon);
        textLabel1 = new QLabel(tab1);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setGeometry(QRect(10, 60, 58, 21));
        textLabel1->setWordWrap(false);
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        textLabel1_2 = new QLabel(tab2);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setGeometry(QRect(5, 60, 58, 21));
        textLabel1_2->setWordWrap(false);
        btnImp = new QPushButton(tab2);
        btnImp->setObjectName(QString::fromUtf8("btnImp"));
        btnImp->setGeometry(QRect(390, 60, 29, 25));
        btnImp->setIcon(icon);
        txtImpFile = new QLineEdit(tab2);
        txtImpFile->setObjectName(QString::fromUtf8("txtImpFile"));
        txtImpFile->setGeometry(QRect(60, 60, 331, 21));
        txtImpFile->setDragEnabled(false);
        txtImpFile->setReadOnly(true);
        chkImpAll = new QCheckBox(tab2);
        chkImpAll->setObjectName(QString::fromUtf8("chkImpAll"));
        chkImpAll->setGeometry(QRect(5, 35, 281, 20));
        chkImpAll->setChecked(true);
        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QString::fromUtf8("tab3"));
        groupBox_4 = new QGroupBox(tab3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(0, 5, 431, 76));
        groupBox_4->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->addTab(tab3, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonAbout = new QPushButton(layoutWidget);
        buttonAbout->setObjectName(QString::fromUtf8("buttonAbout"));

        horizontalLayout->addWidget(buttonAbout);

        horizontalSpacer = new QSpacerItem(183, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOk = new QPushButton(layoutWidget);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));

        horizontalLayout->addWidget(buttonOk);

        buttonCancel = new QPushButton(layoutWidget);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));

        horizontalLayout->addWidget(buttonCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(pb_Dialog);
        QObject::connect(buttonOk, SIGNAL(clicked()), pb_Dialog, SLOT(begin()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), pb_Dialog, SLOT(exit()));
        QObject::connect(buttonAbout, SIGNAL(clicked()), pb_Dialog, SLOT(about()));
        QObject::connect(tabWidget, SIGNAL(currentChanged(int)), pb_Dialog, SLOT(tab_selChanged(int)));
        QObject::connect(btnImp, SIGNAL(clicked()), pb_Dialog, SLOT(importDb()));
        QObject::connect(btnExp, SIGNAL(clicked()), pb_Dialog, SLOT(exportDb()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(pb_Dialog);
    } // setupUi

    void retranslateUi(QDialog *pb_Dialog)
    {
        pb_Dialog->setWindowTitle(QApplication::translate("pb_Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("pb_Dialog", "\346\225\260\346\215\256\345\272\223\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        chkMD->setText(QApplication::translate("pb_Dialog", "\344\270\273\345\272\223", 0, QApplication::UnicodeUTF8));
        chkBD->setText(QApplication::translate("pb_Dialog", "\345\244\207\345\272\223", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("pb_Dialog", "\346\225\260\346\215\256\345\272\223\350\241\250\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        chkYT->setText(QApplication::translate("pb_Dialog", "\345\271\264\346\225\260\346\215\256\350\241\250", 0, QApplication::UnicodeUTF8));
        chkMT->setText(QApplication::translate("pb_Dialog", "\346\234\210\346\225\260\346\215\256\350\241\250", 0, QApplication::UnicodeUTF8));
        chkET->setText(QApplication::translate("pb_Dialog", "\344\272\213\351\241\271\350\256\260\345\275\225\350\241\250", 0, QApplication::UnicodeUTF8));
        chkDT->setText(QApplication::translate("pb_Dialog", "\346\227\245\346\225\260\346\215\256\350\241\250", 0, QApplication::UnicodeUTF8));
        chk_PT->setText(QApplication::translate("pb_Dialog", "\345\217\202\346\225\260\350\241\250", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("pb_Dialog", "\346\227\245\346\234\237\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("pb_Dialog", "\345\274\200\345\247\213\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        dtST->setDisplayFormat(QApplication::translate("pb_Dialog", "yyyy-MM-dd", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("pb_Dialog", "\347\273\223\346\235\237\346\227\245\346\234\237", 0, QApplication::UnicodeUTF8));
        btnExp->setText(QString());
        textLabel1->setText(QApplication::translate("pb_Dialog", "\350\276\223\345\207\272\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("pb_Dialog", "\345\257\274\345\207\272", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("pb_Dialog", "\350\276\223\345\207\272\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        btnImp->setText(QString());
        chkImpAll->setText(QApplication::translate("pb_Dialog", "\345\257\274\345\205\245\346\211\200\346\234\211(\346\226\207\344\273\266\344\270\255\346\211\200\346\234\211\350\241\250\351\203\275\345\260\206\350\242\253\345\257\274\345\205\245\345\210\260\346\225\260\346\215\256\345\272\223\344\270\255)", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("pb_Dialog", "\345\257\274\345\205\245", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("pb_Dialog", "\346\270\205\351\231\244\351\200\211\351\241\271", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("pb_Dialog", "\346\270\205\351\231\244", 0, QApplication::UnicodeUTF8));
        buttonAbout->setText(QApplication::translate("pb_Dialog", "\345\205\263\344\272\216(&A)", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("pb_Dialog", "\345\274\200\345\247\213(&O)", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("pb_Dialog", "\351\200\200\345\207\272(&Q)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class pb_Dialog: public Ui_pb_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PB_DIALOG_H
