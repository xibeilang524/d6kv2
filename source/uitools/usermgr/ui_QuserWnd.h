/********************************************************************************
** Form generated from reading UI file 'QuserWnd.ui'
**
** Created: Fri Apr 7 09:20:12 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUSERWND_H
#define UI_QUSERWND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuserWnd
{
public:
    QAction *actionLogin;
    QAction *actionExit;
    QAction *actionSave;
    QAction *actionLoad;
    QWidget *centralwidget;
    QGridLayout *gridLayout_7;
    QTabWidget *m_tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QTreeWidget *m_roleTW;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_rolelAll;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *m_rolelNotAll;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *roleGroupBox;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *m_addRoleBtn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *m_delRoleBtn;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *m_appBtn;
    QSpacerItem *horizontalSpacer_7;
    QTableWidget *m_roleTable;
    QWidget *tab_2;
    QGridLayout *gridLayout_5;
    QTableWidget *m_userTbWidget;
    QSpacerItem *horizontalSpacer_8;
    QSplitter *splitter;
    QPushButton *m_addUserBtn;
    QPushButton *m_delUserBtn;
    QPushButton *m_modUserBtn;
    QSpacerItem *horizontalSpacer_9;
    QWidget *tabpage;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_6;
    QTreeWidget *m_authTW;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *m_selAllAuth;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *m_selEptAuth;
    QSpacerItem *horizontalSpacer_11;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *m_addauthBtn;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *m_delauthBtn;
    QSpacerItem *horizontalSpacer_15;
    QPushButton *m_aplauthBtn;
    QSpacerItem *horizontalSpacer_16;
    QTreeWidget *m_userTW;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QuserWnd)
    {
        if (QuserWnd->objectName().isEmpty())
            QuserWnd->setObjectName(QString::fromUtf8("QuserWnd"));
        QuserWnd->resize(700, 565);
        actionLogin = new QAction(QuserWnd);
        actionLogin->setObjectName(QString::fromUtf8("actionLogin"));
        actionLogin->setCheckable(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLogin->setIcon(icon);
        actionExit = new QAction(QuserWnd);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionExit->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon1);
        actionSave = new QAction(QuserWnd);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSave->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/dbsave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionLoad = new QAction(QuserWnd);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionLoad->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/dbupdate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad->setIcon(icon3);
        centralwidget = new QWidget(QuserWnd);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_7 = new QGridLayout(centralwidget);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        m_tabWidget = new QTabWidget(centralwidget);
        m_tabWidget->setObjectName(QString::fromUtf8("m_tabWidget"));
        m_tabWidget->setStyleSheet(QString::fromUtf8(""));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        m_roleTW = new QTreeWidget(groupBox_2);
        m_roleTW->setObjectName(QString::fromUtf8("m_roleTW"));

        gridLayout_4->addWidget(m_roleTW, 0, 0, 1, 5);

        horizontalSpacer = new QSpacerItem(32, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 0, 1, 1);

        m_rolelAll = new QPushButton(groupBox_2);
        m_rolelAll->setObjectName(QString::fromUtf8("m_rolelAll"));

        gridLayout_4->addWidget(m_rolelAll, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(33, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        m_rolelNotAll = new QPushButton(groupBox_2);
        m_rolelNotAll->setObjectName(QString::fromUtf8("m_rolelNotAll"));

        gridLayout_4->addWidget(m_rolelNotAll, 1, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(32, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 1, 4, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 0, 1, 1);

        roleGroupBox = new QGroupBox(tab);
        roleGroupBox->setObjectName(QString::fromUtf8("roleGroupBox"));
        gridLayout = new QGridLayout(roleGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_4 = new QSpacerItem(7, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 1, 0, 1, 1);

        m_addRoleBtn = new QPushButton(roleGroupBox);
        m_addRoleBtn->setObjectName(QString::fromUtf8("m_addRoleBtn"));

        gridLayout->addWidget(m_addRoleBtn, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 1, 2, 1, 1);

        m_delRoleBtn = new QPushButton(roleGroupBox);
        m_delRoleBtn->setObjectName(QString::fromUtf8("m_delRoleBtn"));

        gridLayout->addWidget(m_delRoleBtn, 1, 3, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 1, 4, 1, 1);

        m_appBtn = new QPushButton(roleGroupBox);
        m_appBtn->setObjectName(QString::fromUtf8("m_appBtn"));

        gridLayout->addWidget(m_appBtn, 1, 5, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 1, 6, 1, 1);

        m_roleTable = new QTableWidget(roleGroupBox);
        if (m_roleTable->columnCount() < 2)
            m_roleTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_roleTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_roleTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        m_roleTable->setObjectName(QString::fromUtf8("m_roleTable"));
        m_roleTable->setSelectionMode(QAbstractItemView::SingleSelection);
        m_roleTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(m_roleTable, 0, 0, 1, 7);


        gridLayout_3->addWidget(roleGroupBox, 0, 1, 1, 1);

        m_tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_5 = new QGridLayout(tab_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        m_userTbWidget = new QTableWidget(tab_2);
        if (m_userTbWidget->columnCount() < 6)
            m_userTbWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        m_userTbWidget->setHorizontalHeaderItem(5, __qtablewidgetitem7);
        m_userTbWidget->setObjectName(QString::fromUtf8("m_userTbWidget"));

        gridLayout_5->addWidget(m_userTbWidget, 0, 0, 1, 3);

        horizontalSpacer_8 = new QSpacerItem(203, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_8, 1, 0, 1, 1);

        splitter = new QSplitter(tab_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        m_addUserBtn = new QPushButton(splitter);
        m_addUserBtn->setObjectName(QString::fromUtf8("m_addUserBtn"));
        splitter->addWidget(m_addUserBtn);
        m_delUserBtn = new QPushButton(splitter);
        m_delUserBtn->setObjectName(QString::fromUtf8("m_delUserBtn"));
        splitter->addWidget(m_delUserBtn);
        m_modUserBtn = new QPushButton(splitter);
        m_modUserBtn->setObjectName(QString::fromUtf8("m_modUserBtn"));
        splitter->addWidget(m_modUserBtn);

        gridLayout_5->addWidget(splitter, 1, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(202, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_9, 1, 2, 1, 1);

        m_tabWidget->addTab(tab_2, QString());
        tabpage = new QWidget();
        tabpage->setObjectName(QString::fromUtf8("tabpage"));
        horizontalLayout = new QHBoxLayout(tabpage);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        m_authTW = new QTreeWidget(tabpage);
        m_authTW->setObjectName(QString::fromUtf8("m_authTW"));

        gridLayout_6->addWidget(m_authTW, 0, 0, 1, 5);

        horizontalSpacer_10 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_10, 1, 0, 1, 1);

        m_selAllAuth = new QPushButton(tabpage);
        m_selAllAuth->setObjectName(QString::fromUtf8("m_selAllAuth"));

        gridLayout_6->addWidget(m_selAllAuth, 1, 1, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_12, 1, 2, 1, 1);

        m_selEptAuth = new QPushButton(tabpage);
        m_selEptAuth->setObjectName(QString::fromUtf8("m_selEptAuth"));

        gridLayout_6->addWidget(m_selEptAuth, 1, 3, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_11, 1, 4, 1, 1);


        horizontalLayout->addLayout(gridLayout_6);

        groupBox = new QGroupBox(tabpage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_13 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_13, 1, 0, 1, 1);

        m_addauthBtn = new QPushButton(groupBox);
        m_addauthBtn->setObjectName(QString::fromUtf8("m_addauthBtn"));

        gridLayout_2->addWidget(m_addauthBtn, 1, 1, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_14, 1, 2, 1, 1);

        m_delauthBtn = new QPushButton(groupBox);
        m_delauthBtn->setObjectName(QString::fromUtf8("m_delauthBtn"));

        gridLayout_2->addWidget(m_delauthBtn, 1, 3, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(9, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_15, 1, 4, 1, 1);

        m_aplauthBtn = new QPushButton(groupBox);
        m_aplauthBtn->setObjectName(QString::fromUtf8("m_aplauthBtn"));

        gridLayout_2->addWidget(m_aplauthBtn, 1, 5, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_16, 1, 6, 1, 1);

        m_userTW = new QTreeWidget(groupBox);
        m_userTW->setObjectName(QString::fromUtf8("m_userTW"));

        gridLayout_2->addWidget(m_userTW, 0, 0, 1, 7);


        horizontalLayout->addWidget(groupBox);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);
        m_tabWidget->addTab(tabpage, QString());

        gridLayout_7->addWidget(m_tabWidget, 0, 0, 1, 1);

        QuserWnd->setCentralWidget(centralwidget);
        toolBar = new QToolBar(QuserWnd);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(24, 24));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        QuserWnd->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionLogin);
        toolBar->addAction(actionExit);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionLoad);

        retranslateUi(QuserWnd);

        m_tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QuserWnd);
    } // setupUi

    void retranslateUi(QMainWindow *QuserWnd)
    {
        QuserWnd->setWindowTitle(QApplication::translate("QuserWnd", "\350\247\222\350\211\262\346\235\203\351\231\220\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        actionLogin->setText(QApplication::translate("QuserWnd", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLogin->setToolTip(QApplication::translate("QuserWnd", "\347\224\250\346\210\267\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("QuserWnd", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("QuserWnd", "\351\200\200\345\207\272\347\250\213\345\272\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave->setText(QApplication::translate("QuserWnd", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("QuserWnd", "\344\277\235\345\255\230\346\233\264\346\224\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionLoad->setText(QApplication::translate("QuserWnd", "\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLoad->setToolTip(QApplication::translate("QuserWnd", "\345\212\240\350\275\275\346\233\264\346\224\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox_2->setTitle(QApplication::translate("QuserWnd", "\345\237\272\346\234\254\346\235\203\351\231\220", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_roleTW->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("QuserWnd", "\346\235\203\351\231\220\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        m_rolelAll->setText(QApplication::translate("QuserWnd", "\345\205\250\351\200\211", 0, QApplication::UnicodeUTF8));
        m_rolelNotAll->setText(QApplication::translate("QuserWnd", "\345\205\250\344\270\215\351\200\211", 0, QApplication::UnicodeUTF8));
        roleGroupBox->setTitle(QApplication::translate("QuserWnd", "\350\247\222\350\211\262\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        m_addRoleBtn->setText(QApplication::translate("QuserWnd", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        m_delRoleBtn->setText(QApplication::translate("QuserWnd", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        m_appBtn->setText(QApplication::translate("QuserWnd", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = m_roleTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("QuserWnd", "\346\223\215\344\275\234\345\221\230\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = m_roleTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("QuserWnd", "\350\247\222\350\211\262\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab), QApplication::translate("QuserWnd", "\350\247\222\350\211\262", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = m_userTbWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("QuserWnd", "\347\224\250\346\210\267\345\220\215", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = m_userTbWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("QuserWnd", "\346\217\217\350\277\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = m_userTbWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("QuserWnd", "\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = m_userTbWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("QuserWnd", "\347\261\273\345\210\253\346\217\217\350\277\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = m_userTbWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("QuserWnd", "\350\247\222\350\211\262\347\261\273\345\210\253", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = m_userTbWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem7->setText(QApplication::translate("QuserWnd", "\347\273\204\351\225\277", 0, QApplication::UnicodeUTF8));
        m_addUserBtn->setText(QApplication::translate("QuserWnd", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        m_delUserBtn->setText(QApplication::translate("QuserWnd", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        m_modUserBtn->setText(QApplication::translate("QuserWnd", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_2), QApplication::translate("QuserWnd", "\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = m_authTW->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("QuserWnd", "\350\212\202\347\202\271/\346\235\203\351\231\220", 0, QApplication::UnicodeUTF8));
        m_selAllAuth->setText(QApplication::translate("QuserWnd", "\345\205\250\351\200\211", 0, QApplication::UnicodeUTF8));
        m_selEptAuth->setText(QApplication::translate("QuserWnd", "\345\205\250\344\270\215\351\200\211", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("QuserWnd", "\347\224\250\346\210\267\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        m_addauthBtn->setText(QApplication::translate("QuserWnd", "\346\267\273\345\212\240", 0, QApplication::UnicodeUTF8));
        m_delauthBtn->setText(QApplication::translate("QuserWnd", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        m_aplauthBtn->setText(QApplication::translate("QuserWnd", "\345\272\224\347\224\250", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = m_userTW->headerItem();
        ___qtreewidgetitem2->setText(1, QApplication::translate("QuserWnd", "\346\217\217\350\277\260", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("QuserWnd", "\347\224\250\346\210\267\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tabpage), QApplication::translate("QuserWnd", "\346\235\203\351\231\220\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("QuserWnd", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QuserWnd: public Ui_QuserWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUSERWND_H
