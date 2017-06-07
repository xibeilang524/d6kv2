/********************************************************************************
** Form generated from reading UI file 'qphonenoedit.ui'
**
** Created: Fri Apr 7 09:21:04 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QPHONENOEDIT_H
#define UI_QPHONENOEDIT_H

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
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_phoneNoEditDlg
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *m_phoneNo;
    QTreeWidget *m_desc_tree;
    QVBoxLayout *verticalLayout;
    QPushButton *m_addbutton;
    QPushButton *m_delete_button;
    QPushButton *m_load_button;
    QTreeWidget *m_selected_tree;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_okBtn;
    QPushButton *m_cancelBtn;

    void setupUi(QDialog *phoneNoEditDlg)
    {
        if (phoneNoEditDlg->objectName().isEmpty())
            phoneNoEditDlg->setObjectName(QString::fromUtf8("phoneNoEditDlg"));
        phoneNoEditDlg->resize(530, 458);
        phoneNoEditDlg->setMinimumSize(QSize(530, 458));
        phoneNoEditDlg->setMaximumSize(QSize(530, 458));
        gridLayout = new QGridLayout(phoneNoEditDlg);
        gridLayout->setContentsMargins(20, 20, 20, 20);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(20);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(phoneNoEditDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        m_phoneNo = new QComboBox(phoneNoEditDlg);
        m_phoneNo->setObjectName(QString::fromUtf8("m_phoneNo"));

        horizontalLayout->addWidget(m_phoneNo);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 3);

        m_desc_tree = new QTreeWidget(phoneNoEditDlg);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("\345\276\205\351\200\211\344\273\243\347\240\201"));
        m_desc_tree->setHeaderItem(__qtreewidgetitem);
        m_desc_tree->setObjectName(QString::fromUtf8("m_desc_tree"));

        gridLayout->addWidget(m_desc_tree, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_addbutton = new QPushButton(phoneNoEditDlg);
        m_addbutton->setObjectName(QString::fromUtf8("m_addbutton"));

        verticalLayout->addWidget(m_addbutton);

        m_delete_button = new QPushButton(phoneNoEditDlg);
        m_delete_button->setObjectName(QString::fromUtf8("m_delete_button"));

        verticalLayout->addWidget(m_delete_button);

        m_load_button = new QPushButton(phoneNoEditDlg);
        m_load_button->setObjectName(QString::fromUtf8("m_load_button"));

        verticalLayout->addWidget(m_load_button);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 1);

        m_selected_tree = new QTreeWidget(phoneNoEditDlg);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        m_selected_tree->setHeaderItem(__qtreewidgetitem1);
        m_selected_tree->setObjectName(QString::fromUtf8("m_selected_tree"));

        gridLayout->addWidget(m_selected_tree, 1, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(20);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        m_okBtn = new QPushButton(phoneNoEditDlg);
        m_okBtn->setObjectName(QString::fromUtf8("m_okBtn"));

        horizontalLayout_2->addWidget(m_okBtn);

        m_cancelBtn = new QPushButton(phoneNoEditDlg);
        m_cancelBtn->setObjectName(QString::fromUtf8("m_cancelBtn"));

        horizontalLayout_2->addWidget(m_cancelBtn);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 3);


        retranslateUi(phoneNoEditDlg);

        QMetaObject::connectSlotsByName(phoneNoEditDlg);
    } // setupUi

    void retranslateUi(QDialog *phoneNoEditDlg)
    {
        phoneNoEditDlg->setWindowTitle(QApplication::translate("phoneNoEditDlg", "\347\237\255\344\277\241\345\221\212\350\255\246", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("phoneNoEditDlg", "             \346\211\213\346\234\272\347\237\255\344\277\241\345\217\267\347\240\201:", 0, QApplication::UnicodeUTF8));
        m_addbutton->setText(QApplication::translate("phoneNoEditDlg", ">>", 0, QApplication::UnicodeUTF8));
        m_delete_button->setText(QApplication::translate("phoneNoEditDlg", "<<", 0, QApplication::UnicodeUTF8));
        m_load_button->setText(QApplication::translate("phoneNoEditDlg", "\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
        m_okBtn->setText(QApplication::translate("phoneNoEditDlg", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        m_cancelBtn->setText(QApplication::translate("phoneNoEditDlg", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class phoneNoEditDlg: public Ui_phoneNoEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPHONENOEDIT_H
