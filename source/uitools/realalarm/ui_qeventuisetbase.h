/********************************************************************************
** Form generated from reading UI file 'qeventuisetbase.ui'
**
** Created: Fri Apr 7 09:21:04 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QEVENTUISETBASE_H
#define UI_QEVENTUISETBASE_H

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
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QEventUISetBase
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *m_tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *textLabel1;
    QComboBox *m_pageType;
    QHBoxLayout *horizontalLayout;
    QLabel *textLabel1_2;
    QLineEdit *m_pageNameEdit;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *m_delPageBtn;
    QPushButton *m_modPageBtn;
    QPushButton *m_addPageBtn;
    QTreeWidget *m_pageList;
    QSpacerItem *verticalSpacer_2;
    QWidget *TabPage;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *textLabel5;
    QComboBox *m_eventPageCB;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QTreeWidget *m_eventTypeListView;
    QVBoxLayout *verticalLayout;
    QTreeWidget *m_rtuList;
    QTreeWidget *m_eventLevelList;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_cancelBtn;
    QPushButton *m_saveBtn;

    void setupUi(QDialog *QEventUISetBase)
    {
        if (QEventUISetBase->objectName().isEmpty())
            QEventUISetBase->setObjectName(QString::fromUtf8("QEventUISetBase"));
        QEventUISetBase->resize(428, 417);
        QEventUISetBase->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(QEventUISetBase);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(15, 10, 15, 15);
        m_tabWidget = new QTabWidget(QEventUISetBase);
        m_tabWidget->setObjectName(QString::fromUtf8("m_tabWidget"));
        m_tabWidget->setEnabled(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(9);
        gridLayout->setVerticalSpacing(6);
        gridLayout->setContentsMargins(0, 10, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textLabel1 = new QLabel(tab);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
        textLabel1->setSizePolicy(sizePolicy);
        textLabel1->setWordWrap(false);

        horizontalLayout_2->addWidget(textLabel1);

        m_pageType = new QComboBox(tab);
        m_pageType->setObjectName(QString::fromUtf8("m_pageType"));

        horizontalLayout_2->addWidget(m_pageType);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textLabel1_2 = new QLabel(tab);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        horizontalLayout->addWidget(textLabel1_2);

        m_pageNameEdit = new QLineEdit(tab);
        m_pageNameEdit->setObjectName(QString::fromUtf8("m_pageNameEdit"));

        horizontalLayout->addWidget(m_pageNameEdit);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(10);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        m_delPageBtn = new QPushButton(tab);
        m_delPageBtn->setObjectName(QString::fromUtf8("m_delPageBtn"));

        horizontalLayout_5->addWidget(m_delPageBtn);

        m_modPageBtn = new QPushButton(tab);
        m_modPageBtn->setObjectName(QString::fromUtf8("m_modPageBtn"));

        horizontalLayout_5->addWidget(m_modPageBtn);

        m_addPageBtn = new QPushButton(tab);
        m_addPageBtn->setObjectName(QString::fromUtf8("m_addPageBtn"));

        horizontalLayout_5->addWidget(m_addPageBtn);


        gridLayout->addLayout(horizontalLayout_5, 4, 0, 1, 1);

        m_pageList = new QTreeWidget(tab);
        m_pageList->setObjectName(QString::fromUtf8("m_pageList"));
        m_pageList->header()->setDefaultSectionSize(80);

        gridLayout->addWidget(m_pageList, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 3, 0, 1, 1);

        m_tabWidget->addTab(tab, QString());
        TabPage = new QWidget();
        TabPage->setObjectName(QString::fromUtf8("TabPage"));
        verticalLayout_2 = new QVBoxLayout(TabPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        textLabel5 = new QLabel(TabPage);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        sizePolicy.setHeightForWidth(textLabel5->sizePolicy().hasHeightForWidth());
        textLabel5->setSizePolicy(sizePolicy);
        textLabel5->setWordWrap(false);

        horizontalLayout_3->addWidget(textLabel5);

        m_eventPageCB = new QComboBox(TabPage);
        m_eventPageCB->setObjectName(QString::fromUtf8("m_eventPageCB"));

        horizontalLayout_3->addWidget(m_eventPageCB);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        m_eventTypeListView = new QTreeWidget(TabPage);
        m_eventTypeListView->setObjectName(QString::fromUtf8("m_eventTypeListView"));

        horizontalLayout_6->addWidget(m_eventTypeListView);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_rtuList = new QTreeWidget(TabPage);
        m_rtuList->setObjectName(QString::fromUtf8("m_rtuList"));

        verticalLayout->addWidget(m_rtuList);

        m_eventLevelList = new QTreeWidget(TabPage);
        m_eventLevelList->setObjectName(QString::fromUtf8("m_eventLevelList"));

        verticalLayout->addWidget(m_eventLevelList);


        horizontalLayout_6->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(10);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        m_cancelBtn = new QPushButton(TabPage);
        m_cancelBtn->setObjectName(QString::fromUtf8("m_cancelBtn"));

        horizontalLayout_4->addWidget(m_cancelBtn);

        m_saveBtn = new QPushButton(TabPage);
        m_saveBtn->setObjectName(QString::fromUtf8("m_saveBtn"));

        horizontalLayout_4->addWidget(m_saveBtn);


        verticalLayout_2->addLayout(horizontalLayout_4);

        m_tabWidget->addTab(TabPage, QString());

        gridLayout_2->addWidget(m_tabWidget, 0, 0, 1, 1);


        retranslateUi(QEventUISetBase);

        m_tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QEventUISetBase);
    } // setupUi

    void retranslateUi(QDialog *QEventUISetBase)
    {
        QEventUISetBase->setWindowTitle(QApplication::translate("QEventUISetBase", "\345\256\236\346\227\266\344\272\213\351\241\271\347\225\214\351\235\242\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QEventUISetBase", "\347\261\273  \345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        m_pageType->clear();
        m_pageType->insertItems(0, QStringList()
         << QApplication::translate("QEventUISetBase", "\351\241\265\351\235\242", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventUISetBase", "\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8)
        );
        textLabel1_2->setText(QApplication::translate("QEventUISetBase", "\345\220\215  \347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        m_delPageBtn->setText(QApplication::translate("QEventUISetBase", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        m_modPageBtn->setText(QApplication::translate("QEventUISetBase", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        m_addPageBtn->setText(QApplication::translate("QEventUISetBase", "\345\242\236\345\212\240", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pageList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("QEventUISetBase", "\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("QEventUISetBase", "\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab), QApplication::translate("QEventUISetBase", "\351\241\265\351\235\242\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("QEventUISetBase", "\351\241\265\351\235\242\357\274\232", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = m_eventTypeListView->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("QEventUISetBase", "\344\272\213\351\241\271\345\210\206\347\261\273", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = m_rtuList->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("QEventUISetBase", "\345\216\202\347\253\231", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = m_eventLevelList->headerItem();
        ___qtreewidgetitem3->setText(0, QApplication::translate("QEventUISetBase", "\347\272\247\345\210\253", 0, QApplication::UnicodeUTF8));
        m_cancelBtn->setText(QApplication::translate("QEventUISetBase", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        m_saveBtn->setText(QApplication::translate("QEventUISetBase", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(TabPage), QApplication::translate("QEventUISetBase", "\344\272\213\351\241\271\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QEventUISetBase: public Ui_QEventUISetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QEVENTUISETBASE_H
