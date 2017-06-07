/********************************************************************************
** Form generated from reading UI file 'qzonewndbase.ui'
**
** Created: Fri Apr 7 09:20:26 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QZONEWNDBASE_H
#define UI_QZONEWNDBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
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

class Ui_QZoneWndBase
{
public:
    QAction *action_login;
    QAction *action_exit;
    QAction *action_save;
    QAction *action_load;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QTabWidget *m_tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_8;
    QTableWidget *m_zoneTable;
    QSpacerItem *verticalSpacer;
    QPushButton *m_addZoneBtn;
    QPushButton *m_modZoneBtn;
    QPushButton *m_delZoneBtn;
    QWidget *tab_2;
    QGridLayout *gridLayout_7;
    QTreeWidget *m_userCfgLV;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_6;
    QTreeWidget *m_userZoneLV;
    QPushButton *m_userZoneSelAll;
    QPushButton *m_userZoneNotSelAll;
    QPushButton *m_userAppSel;
    QPushButton *m_userAppAll;
    QWidget *tab_3;
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QTreeWidget *m_zoneCfgLV;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QTreeWidget *m_zoneLV;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLineEdit *m_objLE;
    QPushButton *m_ObjFuzzySel;
    QPushButton *m_objSelAll;
    QPushButton *m_objNotSelAll;
    QPushButton *m_appAll;
    QPushButton *m_appSel;
    QWidget *tab_4;
    QGridLayout *gridLayout;
    QTreeWidget *m_zoneViewLV;
    QToolBar *toolBar;

    void setupUi(QMainWindow *QZoneWndBase)
    {
        if (QZoneWndBase->objectName().isEmpty())
            QZoneWndBase->setObjectName(QString::fromUtf8("QZoneWndBase"));
        QZoneWndBase->resize(641, 550);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/zonemng/png/zonemng.png"), QSize(), QIcon::Normal, QIcon::Off);
        QZoneWndBase->setWindowIcon(icon);
        action_login = new QAction(QZoneWndBase);
        action_login->setObjectName(QString::fromUtf8("action_login"));
        action_login->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/zonemng/png/login.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_login->setIcon(icon1);
        action_exit = new QAction(QZoneWndBase);
        action_exit->setObjectName(QString::fromUtf8("action_exit"));
        action_exit->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/zonemng/png/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_exit->setIcon(icon2);
        action_save = new QAction(QZoneWndBase);
        action_save->setObjectName(QString::fromUtf8("action_save"));
        action_save->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/zonemng/png/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_save->setIcon(icon3);
        action_load = new QAction(QZoneWndBase);
        action_load->setObjectName(QString::fromUtf8("action_load"));
        action_load->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/zonemng/png/loadrtdb.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_load->setIcon(icon4);
        centralwidget = new QWidget(QZoneWndBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(9, 9, -1, -1);
        m_tabWidget = new QTabWidget(centralwidget);
        m_tabWidget->setObjectName(QString::fromUtf8("m_tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_8 = new QGridLayout(tab);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        m_zoneTable = new QTableWidget(tab);
        if (m_zoneTable->columnCount() < 2)
            m_zoneTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_zoneTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_zoneTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (m_zoneTable->rowCount() < 3)
            m_zoneTable->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_zoneTable->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        m_zoneTable->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        m_zoneTable->setVerticalHeaderItem(2, __qtablewidgetitem4);
        m_zoneTable->setObjectName(QString::fromUtf8("m_zoneTable"));
        m_zoneTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_8->addWidget(m_zoneTable, 0, 0, 4, 1);

        verticalSpacer = new QSpacerItem(20, 338, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer, 3, 1, 1, 1);

        m_addZoneBtn = new QPushButton(tab);
        m_addZoneBtn->setObjectName(QString::fromUtf8("m_addZoneBtn"));
        m_addZoneBtn->setMinimumSize(QSize(91, 30));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/zonemng/png/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_addZoneBtn->setIcon(icon5);

        gridLayout_8->addWidget(m_addZoneBtn, 0, 1, 1, 1);

        m_modZoneBtn = new QPushButton(tab);
        m_modZoneBtn->setObjectName(QString::fromUtf8("m_modZoneBtn"));
        m_modZoneBtn->setMinimumSize(QSize(91, 30));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/zonemng/png/modify.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_modZoneBtn->setIcon(icon6);

        gridLayout_8->addWidget(m_modZoneBtn, 1, 1, 1, 1);

        m_delZoneBtn = new QPushButton(tab);
        m_delZoneBtn->setObjectName(QString::fromUtf8("m_delZoneBtn"));
        m_delZoneBtn->setMinimumSize(QSize(91, 30));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/zonemng/png/remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_delZoneBtn->setIcon(icon7);

        gridLayout_8->addWidget(m_delZoneBtn, 2, 1, 1, 1);

        m_tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_7 = new QGridLayout(tab_2);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        m_userCfgLV = new QTreeWidget(tab_2);
        m_userCfgLV->setObjectName(QString::fromUtf8("m_userCfgLV"));
        m_userCfgLV->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);

        gridLayout_7->addWidget(m_userCfgLV, 0, 0, 1, 1);

        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_6 = new QGridLayout(groupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        m_userZoneLV = new QTreeWidget(groupBox);
        m_userZoneLV->setObjectName(QString::fromUtf8("m_userZoneLV"));

        gridLayout_6->addWidget(m_userZoneLV, 0, 0, 1, 3);

        m_userZoneSelAll = new QPushButton(groupBox);
        m_userZoneSelAll->setObjectName(QString::fromUtf8("m_userZoneSelAll"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_userZoneSelAll->sizePolicy().hasHeightForWidth());
        m_userZoneSelAll->setSizePolicy(sizePolicy);
        m_userZoneSelAll->setMinimumSize(QSize(91, 30));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/zonemng/png/selall.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_userZoneSelAll->setIcon(icon8);

        gridLayout_6->addWidget(m_userZoneSelAll, 1, 1, 1, 1);

        m_userZoneNotSelAll = new QPushButton(groupBox);
        m_userZoneNotSelAll->setObjectName(QString::fromUtf8("m_userZoneNotSelAll"));
        sizePolicy.setHeightForWidth(m_userZoneNotSelAll->sizePolicy().hasHeightForWidth());
        m_userZoneNotSelAll->setSizePolicy(sizePolicy);
        m_userZoneNotSelAll->setMinimumSize(QSize(91, 30));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/zonemng/png/notselall.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_userZoneNotSelAll->setIcon(icon9);

        gridLayout_6->addWidget(m_userZoneNotSelAll, 1, 2, 1, 1);

        m_userAppSel = new QPushButton(groupBox);
        m_userAppSel->setObjectName(QString::fromUtf8("m_userAppSel"));
        sizePolicy.setHeightForWidth(m_userAppSel->sizePolicy().hasHeightForWidth());
        m_userAppSel->setSizePolicy(sizePolicy);
        m_userAppSel->setMinimumSize(QSize(91, 30));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/zonemng/png/appsel.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_userAppSel->setIcon(icon10);

        gridLayout_6->addWidget(m_userAppSel, 2, 1, 1, 1);

        m_userAppAll = new QPushButton(groupBox);
        m_userAppAll->setObjectName(QString::fromUtf8("m_userAppAll"));
        sizePolicy.setHeightForWidth(m_userAppAll->sizePolicy().hasHeightForWidth());
        m_userAppAll->setSizePolicy(sizePolicy);
        m_userAppAll->setMinimumSize(QSize(91, 30));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/zonemng/png/appall.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_userAppAll->setIcon(icon11);

        gridLayout_6->addWidget(m_userAppAll, 2, 2, 1, 1);


        gridLayout_7->addWidget(groupBox, 0, 1, 1, 1);

        m_tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_5 = new QGridLayout(tab_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        splitter = new QSplitter(tab_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        m_zoneCfgLV = new QTreeWidget(splitter);
        m_zoneCfgLV->setObjectName(QString::fromUtf8("m_zoneCfgLV"));
        splitter->addWidget(m_zoneCfgLV);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        m_zoneLV = new QTreeWidget(groupBox_2);
        m_zoneLV->setObjectName(QString::fromUtf8("m_zoneLV"));

        gridLayout_4->addWidget(m_zoneLV, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        m_objLE = new QLineEdit(groupBox_3);
        m_objLE->setObjectName(QString::fromUtf8("m_objLE"));

        gridLayout_3->addWidget(m_objLE, 0, 0, 2, 2);

        m_ObjFuzzySel = new QPushButton(groupBox_3);
        m_ObjFuzzySel->setObjectName(QString::fromUtf8("m_ObjFuzzySel"));
        m_ObjFuzzySel->setMinimumSize(QSize(0, 30));
        m_ObjFuzzySel->setMaximumSize(QSize(16777215, 30));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/zonemng/png/fuzzy.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_ObjFuzzySel->setIcon(icon12);
        m_ObjFuzzySel->setIconSize(QSize(20, 20));

        gridLayout_3->addWidget(m_ObjFuzzySel, 0, 2, 2, 1);

        m_objSelAll = new QPushButton(groupBox_3);
        m_objSelAll->setObjectName(QString::fromUtf8("m_objSelAll"));
        m_objSelAll->setMinimumSize(QSize(91, 30));
        m_objSelAll->setIcon(icon8);

        gridLayout_3->addWidget(m_objSelAll, 2, 1, 1, 1);

        m_objNotSelAll = new QPushButton(groupBox_3);
        m_objNotSelAll->setObjectName(QString::fromUtf8("m_objNotSelAll"));
        m_objNotSelAll->setMinimumSize(QSize(91, 30));
        m_objNotSelAll->setIcon(icon9);

        gridLayout_3->addWidget(m_objNotSelAll, 2, 2, 1, 1);

        m_appAll = new QPushButton(groupBox_3);
        m_appAll->setObjectName(QString::fromUtf8("m_appAll"));
        m_appAll->setMinimumSize(QSize(91, 30));
        m_appAll->setIcon(icon11);

        gridLayout_3->addWidget(m_appAll, 3, 1, 1, 1);

        m_appSel = new QPushButton(groupBox_3);
        m_appSel->setObjectName(QString::fromUtf8("m_appSel"));
        m_appSel->setMinimumSize(QSize(91, 30));
        m_appSel->setIcon(icon10);

        gridLayout_3->addWidget(m_appSel, 3, 2, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 1, 0, 1, 1);

        splitter->addWidget(groupBox_2);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        m_tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout = new QGridLayout(tab_4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_zoneViewLV = new QTreeWidget(tab_4);
        m_zoneViewLV->setObjectName(QString::fromUtf8("m_zoneViewLV"));

        gridLayout->addWidget(m_zoneViewLV, 0, 0, 1, 1);

        m_tabWidget->addTab(tab_4, QString());

        gridLayout_2->addWidget(m_tabWidget, 0, 1, 1, 1);

        QZoneWndBase->setCentralWidget(centralwidget);
        toolBar = new QToolBar(QZoneWndBase);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setIconSize(QSize(24, 24));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        QZoneWndBase->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(action_login);
        toolBar->addAction(action_exit);
        toolBar->addAction(action_save);
        toolBar->addAction(action_load);

        retranslateUi(QZoneWndBase);

        m_tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(QZoneWndBase);
    } // setupUi

    void retranslateUi(QMainWindow *QZoneWndBase)
    {
        QZoneWndBase->setWindowTitle(QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\347\256\241\347\220\206", 0, QApplication::UnicodeUTF8));
        action_login->setText(QApplication::translate("QZoneWndBase", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_login->setToolTip(QApplication::translate("QZoneWndBase", "\347\224\250\346\210\267\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_exit->setText(QApplication::translate("QZoneWndBase", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_exit->setToolTip(QApplication::translate("QZoneWndBase", "\351\200\200\345\207\272\347\250\213\345\272\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_save->setText(QApplication::translate("QZoneWndBase", "\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_save->setToolTip(QApplication::translate("QZoneWndBase", "\344\277\235\345\255\230\346\233\264\346\224\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_load->setText(QApplication::translate("QZoneWndBase", "\345\212\240\350\275\275", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_load->setToolTip(QApplication::translate("QZoneWndBase", "\345\212\240\350\275\275\345\210\260\345\256\236\346\227\266\345\272\223", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        QTableWidgetItem *___qtablewidgetitem = m_zoneTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\217\267", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = m_zoneTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = m_zoneTable->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("QZoneWndBase", "1", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = m_zoneTable->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("QZoneWndBase", "2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = m_zoneTable->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("QZoneWndBase", "3", 0, QApplication::UnicodeUTF8));
        m_addZoneBtn->setText(QApplication::translate("QZoneWndBase", "\346\267\273    \345\212\240", 0, QApplication::UnicodeUTF8));
        m_modZoneBtn->setText(QApplication::translate("QZoneWndBase", "\344\277\256    \346\224\271", 0, QApplication::UnicodeUTF8));
        m_delZoneBtn->setText(QApplication::translate("QZoneWndBase", "\345\210\240    \351\231\244", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab), QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_userCfgLV->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("QZoneWndBase", "\347\224\250\346\210\267\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = m_userZoneLV->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_userZoneSelAll->setText(QApplication::translate("QZoneWndBase", "\345\205\250      \351\200\211", 0, QApplication::UnicodeUTF8));
        m_userZoneNotSelAll->setText(QApplication::translate("QZoneWndBase", "\345\205\250  \344\270\215  \351\200\211", 0, QApplication::UnicodeUTF8));
        m_userAppSel->setText(QApplication::translate("QZoneWndBase", "\345\272\224\347\224\250\346\211\200\351\200\211", 0, QApplication::UnicodeUTF8));
        m_userAppAll->setText(QApplication::translate("QZoneWndBase", "\345\272\224\347\224\250\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_2), QApplication::translate("QZoneWndBase", "\347\224\250\346\210\267", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = m_zoneCfgLV->headerItem();
        ___qtreewidgetitem2->setText(1, QApplication::translate("QZoneWndBase", "\346\211\200\345\261\236\347\233\221\346\216\247\345\214\272", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("QZoneWndBase", "\345\257\271\350\261\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = m_zoneLV->headerItem();
        ___qtreewidgetitem3->setText(0, QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("QZoneWndBase", "\345\257\271\350\261\241\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        m_ObjFuzzySel->setText(QApplication::translate("QZoneWndBase", "\346\250\241\347\263\212\351\200\211\346\213\251", 0, QApplication::UnicodeUTF8));
        m_objSelAll->setText(QApplication::translate("QZoneWndBase", "\345\205\250      \351\200\211", 0, QApplication::UnicodeUTF8));
        m_objNotSelAll->setText(QApplication::translate("QZoneWndBase", "\345\205\250  \344\270\215  \351\200\211", 0, QApplication::UnicodeUTF8));
        m_appAll->setText(QApplication::translate("QZoneWndBase", "\345\272\224\347\224\250\345\205\250\351\203\250", 0, QApplication::UnicodeUTF8));
        m_appSel->setText(QApplication::translate("QZoneWndBase", "\345\272\224\347\224\250\346\211\200\351\200\211", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_3), QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\345\210\222\345\210\206", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = m_zoneViewLV->headerItem();
        ___qtreewidgetitem4->setText(0, QApplication::translate("QZoneWndBase", "\345\257\271\350\261\241\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(tab_4), QApplication::translate("QZoneWndBase", "\347\233\221\346\216\247\345\214\272\346\265\217\350\247\210", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("QZoneWndBase", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QZoneWndBase: public Ui_QZoneWndBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QZONEWNDBASE_H
