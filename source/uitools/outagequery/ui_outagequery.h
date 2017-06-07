/********************************************************************************
** Form generated from reading UI file 'outagequery.ui'
**
** Created: Fri Apr 7 09:20:28 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTAGEQUERY_H
#define UI_OUTAGEQUERY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WindowBase
{
public:
    QAction *action_read;
    QAction *action_Exit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_7;
    QTableWidget *m_powerCutTable;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTabWidget *m_tabWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *m_stTreeWidget;
    QHBoxLayout *horizontalLayout;
    QToolButton *m_allStBtn;
    QToolButton *m_stQueryBtn;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *m_AreaTreeWidget;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *m_allAreaBtn;
    QToolButton *m_AreaQueryBtn;
    QWidget *tab_4;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_9;
    QTreeWidget *m_feederTreeWidget;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *m_allfeederBtn;
    QToolButton *m_feederQueryBtn;
    QToolBar *toolBar;
    QToolBar *timetoolbar;

    void setupUi(QMainWindow *WindowBase)
    {
        if (WindowBase->objectName().isEmpty())
            WindowBase->setObjectName(QString::fromUtf8("WindowBase"));
        WindowBase->setWindowModality(Qt::NonModal);
        WindowBase->resize(1327, 629);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/outagequery/images/outagequery.png"), QSize(), QIcon::Normal, QIcon::Off);
        WindowBase->setWindowIcon(icon);
        action_read = new QAction(WindowBase);
        action_read->setObjectName(QString::fromUtf8("action_read"));
        action_read->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/dbquery/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/outagequery/images/confirm.png"), QSize(), QIcon::Normal, QIcon::On);
        action_read->setIcon(icon1);
        action_Exit = new QAction(WindowBase);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        action_Exit->setCheckable(true);
        action_Exit->setEnabled(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/outagequery/images/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Exit->setIcon(icon2);
        centralwidget = new QWidget(WindowBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_7 = new QVBoxLayout(centralwidget);
        verticalLayout_7->setSpacing(5);
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        m_powerCutTable = new QTableWidget(centralwidget);
        if (m_powerCutTable->columnCount() < 11)
            m_powerCutTable->setColumnCount(11);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        m_powerCutTable->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        m_powerCutTable->setObjectName(QString::fromUtf8("m_powerCutTable"));
        m_powerCutTable->setAutoFillBackground(false);
        m_powerCutTable->setStyleSheet(QString::fromUtf8("background-image: url(:/dbquery/images/bk.png);"));

        verticalLayout_7->addWidget(m_powerCutTable);

        WindowBase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WindowBase);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1327, 23));
        WindowBase->setMenuBar(menubar);
        statusbar = new QStatusBar(WindowBase);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WindowBase->setStatusBar(statusbar);
        dockWidget = new QDockWidget(WindowBase);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy);
        dockWidget->setMinimumSize(QSize(280, 240));
        dockWidget->setAcceptDrops(false);
        dockWidget->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 0, 0, 0);
        m_tabWidget = new QTabWidget(dockWidgetContents);
        m_tabWidget->setObjectName(QString::fromUtf8("m_tabWidget"));
        m_tabWidget->setTabShape(QTabWidget::Rounded);
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        m_stTreeWidget = new QTreeWidget(tab_2);
        m_stTreeWidget->setObjectName(QString::fromUtf8("m_stTreeWidget"));

        verticalLayout_3->addWidget(m_stTreeWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_allStBtn = new QToolButton(tab_2);
        m_allStBtn->setObjectName(QString::fromUtf8("m_allStBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_allStBtn->sizePolicy().hasHeightForWidth());
        m_allStBtn->setSizePolicy(sizePolicy1);
        m_allStBtn->setMinimumSize(QSize(120, 30));
        m_allStBtn->setMaximumSize(QSize(120, 30));
        m_allStBtn->setCheckable(true);

        horizontalLayout->addWidget(m_allStBtn);

        m_stQueryBtn = new QToolButton(tab_2);
        m_stQueryBtn->setObjectName(QString::fromUtf8("m_stQueryBtn"));
        m_stQueryBtn->setMinimumSize(QSize(120, 30));
        m_stQueryBtn->setMaximumSize(QSize(120, 30));

        horizontalLayout->addWidget(m_stQueryBtn);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        m_tabWidget->addTab(tab_2, QString());
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        verticalLayout_5 = new QVBoxLayout(tab_1);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        m_AreaTreeWidget = new QTreeWidget(tab_1);
        m_AreaTreeWidget->setObjectName(QString::fromUtf8("m_AreaTreeWidget"));

        verticalLayout_2->addWidget(m_AreaTreeWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        m_allAreaBtn = new QToolButton(tab_1);
        m_allAreaBtn->setObjectName(QString::fromUtf8("m_allAreaBtn"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_allAreaBtn->sizePolicy().hasHeightForWidth());
        m_allAreaBtn->setSizePolicy(sizePolicy2);
        m_allAreaBtn->setMinimumSize(QSize(120, 30));
        m_allAreaBtn->setMaximumSize(QSize(120, 30));
        m_allAreaBtn->setCheckable(true);

        horizontalLayout_2->addWidget(m_allAreaBtn);

        m_AreaQueryBtn = new QToolButton(tab_1);
        m_AreaQueryBtn->setObjectName(QString::fromUtf8("m_AreaQueryBtn"));
        sizePolicy2.setHeightForWidth(m_AreaQueryBtn->sizePolicy().hasHeightForWidth());
        m_AreaQueryBtn->setSizePolicy(sizePolicy2);
        m_AreaQueryBtn->setMinimumSize(QSize(120, 30));
        m_AreaQueryBtn->setMaximumSize(QSize(120, 30));

        horizontalLayout_2->addWidget(m_AreaQueryBtn);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_5->addLayout(verticalLayout_2);

        m_tabWidget->addTab(tab_1, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout = new QGridLayout(tab_4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        m_feederTreeWidget = new QTreeWidget(tab_4);
        m_feederTreeWidget->setObjectName(QString::fromUtf8("m_feederTreeWidget"));

        verticalLayout_9->addWidget(m_feederTreeWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        m_allfeederBtn = new QToolButton(tab_4);
        m_allfeederBtn->setObjectName(QString::fromUtf8("m_allfeederBtn"));
        m_allfeederBtn->setMinimumSize(QSize(120, 30));
        m_allfeederBtn->setMaximumSize(QSize(120, 30));
        m_allfeederBtn->setCheckable(true);

        horizontalLayout_4->addWidget(m_allfeederBtn);

        m_feederQueryBtn = new QToolButton(tab_4);
        m_feederQueryBtn->setObjectName(QString::fromUtf8("m_feederQueryBtn"));
        m_feederQueryBtn->setMinimumSize(QSize(120, 30));
        m_feederQueryBtn->setMaximumSize(QSize(120, 30));

        horizontalLayout_4->addWidget(m_feederQueryBtn);


        verticalLayout_9->addLayout(horizontalLayout_4);


        gridLayout->addLayout(verticalLayout_9, 0, 0, 1, 1);

        m_tabWidget->addTab(tab_4, QString());

        verticalLayout->addWidget(m_tabWidget);

        dockWidget->setWidget(dockWidgetContents);
        WindowBase->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        toolBar = new QToolBar(WindowBase);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        WindowBase->addToolBar(Qt::TopToolBarArea, toolBar);
        timetoolbar = new QToolBar(WindowBase);
        timetoolbar->setObjectName(QString::fromUtf8("timetoolbar"));
        WindowBase->addToolBar(Qt::TopToolBarArea, timetoolbar);

        toolBar->addAction(action_read);
        toolBar->addAction(action_Exit);
        toolBar->addSeparator();

        retranslateUi(WindowBase);

        m_tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(WindowBase);
    } // setupUi

    void retranslateUi(QMainWindow *WindowBase)
    {
        WindowBase->setWindowTitle(QApplication::translate("WindowBase", "\345\201\234\347\224\265\347\256\241\347\220\206\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        action_read->setText(QApplication::translate("WindowBase", "\350\257\273\345\217\226\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_read->setToolTip(QApplication::translate("WindowBase", "\350\257\273\345\217\226\346\225\260\346\215\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Exit->setText(QApplication::translate("WindowBase", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = m_powerCutTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("WindowBase", "\345\256\242\346\210\267\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = m_powerCutTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("WindowBase", "\345\256\242\346\210\267\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = m_powerCutTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("WindowBase", "\346\211\200\345\261\236\345\216\202\347\253\231", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = m_powerCutTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("WindowBase", "\345\201\234\347\224\265\345\274\200\345\247\213\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = m_powerCutTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("WindowBase", "\345\201\234\347\224\265\347\273\223\346\235\237\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = m_powerCutTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("WindowBase", "\345\201\234\347\224\265\346\227\266\351\225\277", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = m_powerCutTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("WindowBase", "\345\201\234\347\224\265\345\274\200\345\205\263", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = m_powerCutTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("WindowBase", "\345\201\234\347\224\265\347\261\273\345\236\213", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = m_powerCutTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("WindowBase", "\351\246\210\347\272\277\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = m_powerCutTable->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("WindowBase", "\345\214\272\345\237\237\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = m_powerCutTable->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("WindowBase", "\344\270\242\345\244\261\350\264\237\350\215\267\345\256\271\351\207\217(kW)", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_stTreeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("WindowBase", "\345\216\202\347\253\231", 0, QApplication::UnicodeUTF8));
        m_allStBtn->setText(QApplication::translate("WindowBase", "\346\211\200\346\234\211\345\216\202\347\253\231", 0, QApplication::UnicodeUTF8));
        m_stQueryBtn->setText(QApplication::translate("WindowBase", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_2), QApplication::translate("WindowBase", "\345\216\202\347\253\231", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = m_AreaTreeWidget->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("WindowBase", "\345\214\272\345\237\237\344\277\241\346\201\257", 0, QApplication::UnicodeUTF8));
        m_allAreaBtn->setText(QApplication::translate("WindowBase", "\346\211\200\346\234\211\345\214\272\345\237\237", 0, QApplication::UnicodeUTF8));
        m_AreaQueryBtn->setText(QApplication::translate("WindowBase", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_1), QApplication::translate("WindowBase", "\345\214\272\345\237\237", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = m_feederTreeWidget->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("WindowBase", "\351\246\210\347\272\277", 0, QApplication::UnicodeUTF8));
        m_allfeederBtn->setText(QApplication::translate("WindowBase", "\346\211\200\346\234\211\351\246\210\347\272\277", 0, QApplication::UnicodeUTF8));
        m_feederQueryBtn->setText(QApplication::translate("WindowBase", "\346\237\245\350\257\242", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_4), QApplication::translate("WindowBase", "\351\246\210\347\272\277", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("WindowBase", "toolBar", 0, QApplication::UnicodeUTF8));
        timetoolbar->setWindowTitle(QApplication::translate("WindowBase", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WindowBase: public Ui_WindowBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTAGEQUERY_H
