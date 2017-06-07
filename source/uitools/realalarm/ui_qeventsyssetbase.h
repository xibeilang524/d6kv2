/********************************************************************************
** Form generated from reading UI file 'qeventsyssetbase.ui'
**
** Created: Fri Apr 7 09:21:04 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QEVENTSYSSETBASE_H
#define UI_QEVENTSYSSETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QEventSysSetBase
{
public:
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer;
    QTabWidget *tabWidget;
    QWidget *Widget8;
    QVBoxLayout *verticalLayout;
    QLabel *label1;
    QHBoxLayout *horizontalLayout;
    QCheckBox *m_waitforConfirmChkBox;
    QLabel *textLabel5;
    QSpinBox *m_eventFontSpinBox;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *m_keepUnConfirmedEventChkBox;
    QLabel *textLabel1_2;
    QSpinBox *m_eventNumSpinBox;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *m_AutoConfirmDelChkBox;
    QLabel *textLabel4;
    QComboBox *m_eventColorCB1;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *m_chkNeedMatch;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *m_showJXEvent;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label2;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *m_eventToneChkBox;
    QLabel *textLabel3_3_2_2;
    QLineEdit *m_toneDev;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *m_msgChkBox;
    QLabel *textLabel3_3_2;
    QLineEdit *m_serial_no;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *m_rcvmsgChkBox;
    QLabel *textLabel3_3_2_3;
    QLineEdit *m_rcvmsgTime;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *m_eventPrintChkBox;
    QLabel *textLabel1;
    QLineEdit *m_printer;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *m_dbgChkBox;
    QSpacerItem *horizontalSpacer_6;
    QWidget *Widget9;
    QGridLayout *gridLayout_7;
    QComboBox *m_eventColorCB2;
    QListWidget *m_colorList;
    QWidget *TabPage;
    QGridLayout *gridLayout_8;
    QTreeWidget *m_colList;
    QPushButton *m_colUpBtn;
    QSpacerItem *verticalSpacer_2;
    QPushButton *m_colDownBtn;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer;
    QWidget *TabPage_2;
    QGridLayout *gridLayout_9;
    QTableWidget *m_soundTable;
    QWidget *TabPage_3;
    QGridLayout *gridLayout_10;
    QTableWidget *m_phoneNoTable;
    QWidget *m_reportTab;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *m_reportTree;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *m_reportFileBtn;
    QCheckBox *m_isReport;
    QRadioButton *m_hBtn;
    QRadioButton *m_vBtn;
    QPushButton *buttonCancel;
    QPushButton *buttonOk;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *QEventSysSetBase)
    {
        if (QEventSysSetBase->objectName().isEmpty())
            QEventSysSetBase->setObjectName(QString::fromUtf8("QEventSysSetBase"));
        QEventSysSetBase->resize(405, 434);
        gridLayout_4 = new QGridLayout(QEventSysSetBase);
        gridLayout_4->setContentsMargins(15, 15, 15, 15);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(0);
        gridLayout_4->setVerticalSpacing(6);
        horizontalSpacer = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 0, 1, 1);

        tabWidget = new QTabWidget(QEventSysSetBase);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setElideMode(Qt::ElideNone);
        Widget8 = new QWidget();
        Widget8->setObjectName(QString::fromUtf8("Widget8"));
        verticalLayout = new QVBoxLayout(Widget8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label1 = new QLabel(Widget8);
        label1->setObjectName(QString::fromUtf8("label1"));

        verticalLayout->addWidget(label1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_waitforConfirmChkBox = new QCheckBox(Widget8);
        m_waitforConfirmChkBox->setObjectName(QString::fromUtf8("m_waitforConfirmChkBox"));

        horizontalLayout->addWidget(m_waitforConfirmChkBox);

        textLabel5 = new QLabel(Widget8);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textLabel5->sizePolicy().hasHeightForWidth());
        textLabel5->setSizePolicy(sizePolicy);
        textLabel5->setWordWrap(false);

        horizontalLayout->addWidget(textLabel5);

        m_eventFontSpinBox = new QSpinBox(Widget8);
        m_eventFontSpinBox->setObjectName(QString::fromUtf8("m_eventFontSpinBox"));
        m_eventFontSpinBox->setMinimum(5);

        horizontalLayout->addWidget(m_eventFontSpinBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        m_keepUnConfirmedEventChkBox = new QCheckBox(Widget8);
        m_keepUnConfirmedEventChkBox->setObjectName(QString::fromUtf8("m_keepUnConfirmedEventChkBox"));

        horizontalLayout_2->addWidget(m_keepUnConfirmedEventChkBox);

        textLabel1_2 = new QLabel(Widget8);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        sizePolicy.setHeightForWidth(textLabel1_2->sizePolicy().hasHeightForWidth());
        textLabel1_2->setSizePolicy(sizePolicy);
        textLabel1_2->setWordWrap(false);

        horizontalLayout_2->addWidget(textLabel1_2);

        m_eventNumSpinBox = new QSpinBox(Widget8);
        m_eventNumSpinBox->setObjectName(QString::fromUtf8("m_eventNumSpinBox"));
        m_eventNumSpinBox->setMinimum(10);
        m_eventNumSpinBox->setMaximum(10000);
        m_eventNumSpinBox->setSingleStep(10);
        m_eventNumSpinBox->setValue(10);

        horizontalLayout_2->addWidget(m_eventNumSpinBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        m_AutoConfirmDelChkBox = new QCheckBox(Widget8);
        m_AutoConfirmDelChkBox->setObjectName(QString::fromUtf8("m_AutoConfirmDelChkBox"));
        m_AutoConfirmDelChkBox->setChecked(false);

        horizontalLayout_3->addWidget(m_AutoConfirmDelChkBox);

        textLabel4 = new QLabel(Widget8);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        sizePolicy.setHeightForWidth(textLabel4->sizePolicy().hasHeightForWidth());
        textLabel4->setSizePolicy(sizePolicy);
        textLabel4->setWordWrap(false);

        horizontalLayout_3->addWidget(textLabel4);

        m_eventColorCB1 = new QComboBox(Widget8);
        m_eventColorCB1->setObjectName(QString::fromUtf8("m_eventColorCB1"));

        horizontalLayout_3->addWidget(m_eventColorCB1);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        m_chkNeedMatch = new QCheckBox(Widget8);
        m_chkNeedMatch->setObjectName(QString::fromUtf8("m_chkNeedMatch"));

        horizontalLayout_9->addWidget(m_chkNeedMatch);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        m_showJXEvent = new QCheckBox(Widget8);
        m_showJXEvent->setObjectName(QString::fromUtf8("m_showJXEvent"));

        horizontalLayout_8->addWidget(m_showJXEvent);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_8);

        label2 = new QLabel(Widget8);
        label2->setObjectName(QString::fromUtf8("label2"));

        verticalLayout->addWidget(label2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        m_eventToneChkBox = new QCheckBox(Widget8);
        m_eventToneChkBox->setObjectName(QString::fromUtf8("m_eventToneChkBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_eventToneChkBox->sizePolicy().hasHeightForWidth());
        m_eventToneChkBox->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(m_eventToneChkBox);

        textLabel3_3_2_2 = new QLabel(Widget8);
        textLabel3_3_2_2->setObjectName(QString::fromUtf8("textLabel3_3_2_2"));
        sizePolicy.setHeightForWidth(textLabel3_3_2_2->sizePolicy().hasHeightForWidth());
        textLabel3_3_2_2->setSizePolicy(sizePolicy);
        textLabel3_3_2_2->setMinimumSize(QSize(80, 0));
        textLabel3_3_2_2->setMaximumSize(QSize(80, 16777215));
        textLabel3_3_2_2->setWordWrap(false);

        horizontalLayout_4->addWidget(textLabel3_3_2_2);

        m_toneDev = new QLineEdit(Widget8);
        m_toneDev->setObjectName(QString::fromUtf8("m_toneDev"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_toneDev->sizePolicy().hasHeightForWidth());
        m_toneDev->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(m_toneDev);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        m_msgChkBox = new QCheckBox(Widget8);
        m_msgChkBox->setObjectName(QString::fromUtf8("m_msgChkBox"));
        sizePolicy1.setHeightForWidth(m_msgChkBox->sizePolicy().hasHeightForWidth());
        m_msgChkBox->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(m_msgChkBox);

        textLabel3_3_2 = new QLabel(Widget8);
        textLabel3_3_2->setObjectName(QString::fromUtf8("textLabel3_3_2"));
        sizePolicy.setHeightForWidth(textLabel3_3_2->sizePolicy().hasHeightForWidth());
        textLabel3_3_2->setSizePolicy(sizePolicy);
        textLabel3_3_2->setMinimumSize(QSize(80, 0));
        textLabel3_3_2->setMaximumSize(QSize(80, 16777215));
        textLabel3_3_2->setWordWrap(false);

        horizontalLayout_5->addWidget(textLabel3_3_2);

        m_serial_no = new QLineEdit(Widget8);
        m_serial_no->setObjectName(QString::fromUtf8("m_serial_no"));
        sizePolicy2.setHeightForWidth(m_serial_no->sizePolicy().hasHeightForWidth());
        m_serial_no->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(m_serial_no);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        m_rcvmsgChkBox = new QCheckBox(Widget8);
        m_rcvmsgChkBox->setObjectName(QString::fromUtf8("m_rcvmsgChkBox"));
        sizePolicy1.setHeightForWidth(m_rcvmsgChkBox->sizePolicy().hasHeightForWidth());
        m_rcvmsgChkBox->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(m_rcvmsgChkBox);

        textLabel3_3_2_3 = new QLabel(Widget8);
        textLabel3_3_2_3->setObjectName(QString::fromUtf8("textLabel3_3_2_3"));
        sizePolicy.setHeightForWidth(textLabel3_3_2_3->sizePolicy().hasHeightForWidth());
        textLabel3_3_2_3->setSizePolicy(sizePolicy);
        textLabel3_3_2_3->setMinimumSize(QSize(80, 0));
        textLabel3_3_2_3->setMaximumSize(QSize(80, 16777215));
        textLabel3_3_2_3->setWordWrap(false);

        horizontalLayout_6->addWidget(textLabel3_3_2_3);

        m_rcvmsgTime = new QLineEdit(Widget8);
        m_rcvmsgTime->setObjectName(QString::fromUtf8("m_rcvmsgTime"));
        sizePolicy2.setHeightForWidth(m_rcvmsgTime->sizePolicy().hasHeightForWidth());
        m_rcvmsgTime->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(m_rcvmsgTime);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        m_eventPrintChkBox = new QCheckBox(Widget8);
        m_eventPrintChkBox->setObjectName(QString::fromUtf8("m_eventPrintChkBox"));
        sizePolicy1.setHeightForWidth(m_eventPrintChkBox->sizePolicy().hasHeightForWidth());
        m_eventPrintChkBox->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(m_eventPrintChkBox);

        textLabel1 = new QLabel(Widget8);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        sizePolicy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
        textLabel1->setSizePolicy(sizePolicy);
        textLabel1->setMinimumSize(QSize(80, 0));
        textLabel1->setMaximumSize(QSize(80, 16777215));
        textLabel1->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        textLabel1->setWordWrap(false);

        horizontalLayout_7->addWidget(textLabel1);

        m_printer = new QLineEdit(Widget8);
        m_printer->setObjectName(QString::fromUtf8("m_printer"));
        sizePolicy2.setHeightForWidth(m_printer->sizePolicy().hasHeightForWidth());
        m_printer->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(m_printer);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        m_dbgChkBox = new QCheckBox(Widget8);
        m_dbgChkBox->setObjectName(QString::fromUtf8("m_dbgChkBox"));
        sizePolicy1.setHeightForWidth(m_dbgChkBox->sizePolicy().hasHeightForWidth());
        m_dbgChkBox->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(m_dbgChkBox);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_10);

        tabWidget->addTab(Widget8, QString());
        Widget9 = new QWidget();
        Widget9->setObjectName(QString::fromUtf8("Widget9"));
        gridLayout_7 = new QGridLayout(Widget9);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, -1, 0, 9);
        m_eventColorCB2 = new QComboBox(Widget9);
        m_eventColorCB2->setObjectName(QString::fromUtf8("m_eventColorCB2"));

        gridLayout_7->addWidget(m_eventColorCB2, 0, 0, 1, 1);

        m_colorList = new QListWidget(Widget9);
        m_colorList->setObjectName(QString::fromUtf8("m_colorList"));

        gridLayout_7->addWidget(m_colorList, 1, 0, 1, 1);

        tabWidget->addTab(Widget9, QString());
        TabPage = new QWidget();
        TabPage->setObjectName(QString::fromUtf8("TabPage"));
        gridLayout_8 = new QGridLayout(TabPage);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setHorizontalSpacing(9);
        gridLayout_8->setVerticalSpacing(6);
        gridLayout_8->setContentsMargins(0, -1, 0, 9);
        m_colList = new QTreeWidget(TabPage);
        m_colList->setObjectName(QString::fromUtf8("m_colList"));

        gridLayout_8->addWidget(m_colList, 0, 0, 5, 1);

        m_colUpBtn = new QPushButton(TabPage);
        m_colUpBtn->setObjectName(QString::fromUtf8("m_colUpBtn"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(m_colUpBtn->sizePolicy().hasHeightForWidth());
        m_colUpBtn->setSizePolicy(sizePolicy3);

        gridLayout_8->addWidget(m_colUpBtn, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_2, 2, 1, 1, 1);

        m_colDownBtn = new QPushButton(TabPage);
        m_colDownBtn->setObjectName(QString::fromUtf8("m_colDownBtn"));
        sizePolicy3.setHeightForWidth(m_colDownBtn->sizePolicy().hasHeightForWidth());
        m_colDownBtn->setSizePolicy(sizePolicy3);

        gridLayout_8->addWidget(m_colDownBtn, 3, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_3, 4, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 83, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_8->addItem(verticalSpacer, 0, 1, 1, 1);

        tabWidget->addTab(TabPage, QString());
        TabPage_2 = new QWidget();
        TabPage_2->setObjectName(QString::fromUtf8("TabPage_2"));
        gridLayout_9 = new QGridLayout(TabPage_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, -1, 0, 9);
        m_soundTable = new QTableWidget(TabPage_2);
        if (m_soundTable->columnCount() < 1)
            m_soundTable->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_soundTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        m_soundTable->setObjectName(QString::fromUtf8("m_soundTable"));

        gridLayout_9->addWidget(m_soundTable, 0, 0, 1, 1);

        tabWidget->addTab(TabPage_2, QString());
        TabPage_3 = new QWidget();
        TabPage_3->setObjectName(QString::fromUtf8("TabPage_3"));
        gridLayout_10 = new QGridLayout(TabPage_3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, -1, 0, 9);
        m_phoneNoTable = new QTableWidget(TabPage_3);
        if (m_phoneNoTable->columnCount() < 2)
            m_phoneNoTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_phoneNoTable->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_phoneNoTable->setHorizontalHeaderItem(1, __qtablewidgetitem2);
        m_phoneNoTable->setObjectName(QString::fromUtf8("m_phoneNoTable"));

        gridLayout_10->addWidget(m_phoneNoTable, 0, 0, 1, 1);

        tabWidget->addTab(TabPage_3, QString());
        m_reportTab = new QWidget();
        m_reportTab->setObjectName(QString::fromUtf8("m_reportTab"));
        verticalLayout_2 = new QVBoxLayout(m_reportTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        m_reportTree = new QTreeWidget(m_reportTab);
        m_reportTree->setObjectName(QString::fromUtf8("m_reportTree"));
        m_reportTree->header()->setDefaultSectionSize(80);

        verticalLayout_2->addWidget(m_reportTree);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);

        m_reportFileBtn = new QPushButton(m_reportTab);
        m_reportFileBtn->setObjectName(QString::fromUtf8("m_reportFileBtn"));

        horizontalLayout_11->addWidget(m_reportFileBtn);

        m_isReport = new QCheckBox(m_reportTab);
        m_isReport->setObjectName(QString::fromUtf8("m_isReport"));

        horizontalLayout_11->addWidget(m_isReport);

        m_hBtn = new QRadioButton(m_reportTab);
        m_hBtn->setObjectName(QString::fromUtf8("m_hBtn"));
        m_hBtn->setChecked(true);

        horizontalLayout_11->addWidget(m_hBtn);

        m_vBtn = new QRadioButton(m_reportTab);
        m_vBtn->setObjectName(QString::fromUtf8("m_vBtn"));

        horizontalLayout_11->addWidget(m_vBtn);


        verticalLayout_2->addLayout(horizontalLayout_11);

        tabWidget->addTab(m_reportTab, QString());

        gridLayout_4->addWidget(tabWidget, 0, 0, 1, 8);

        buttonCancel = new QPushButton(QEventSysSetBase);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        gridLayout_4->addWidget(buttonCancel, 1, 3, 1, 1);

        buttonOk = new QPushButton(QEventSysSetBase);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        gridLayout_4->addWidget(buttonOk, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        retranslateUi(QEventSysSetBase);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QEventSysSetBase);
    } // setupUi

    void retranslateUi(QDialog *QEventSysSetBase)
    {
        QEventSysSetBase->setWindowTitle(QApplication::translate("QEventSysSetBase", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabWidget->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label1->setText(QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8));
        m_waitforConfirmChkBox->setText(QApplication::translate("QEventSysSetBase", "\344\273\205\346\230\276\347\244\272\345\276\205\347\241\256\350\256\244\344\272\213\351\241\271", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\345\255\227\344\275\223\357\274\232", 0, QApplication::UnicodeUTF8));
        m_keepUnConfirmedEventChkBox->setText(QApplication::translate("QEventSysSetBase", "\344\277\235\347\225\231\345\276\205\347\241\256\350\256\244\344\272\213\351\241\271", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("QEventSysSetBase", "\346\234\200\345\244\232\344\270\252\346\225\260\357\274\232", 0, QApplication::UnicodeUTF8));
        m_AutoConfirmDelChkBox->setText(QApplication::translate("QEventSysSetBase", "\347\241\256\350\256\244\345\220\216\350\207\252\345\212\250\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\351\242\234\350\211\262\357\274\232", 0, QApplication::UnicodeUTF8));
        m_eventColorCB1->clear();
        m_eventColorCB1->insertItems(0, QStringList()
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\345\210\206\347\261\273\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\347\261\273\345\236\213\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\345\216\202\347\253\231\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\347\272\247\345\210\253\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
        );
        m_chkNeedMatch->setText(QApplication::translate("QEventSysSetBase", "\350\207\252\345\212\250\346\220\234\347\264\242\345\214\271\351\205\215", 0, QApplication::UnicodeUTF8));
        m_showJXEvent->setText(QApplication::translate("QEventSysSetBase", "\346\230\276\347\244\272\346\243\200\344\277\256\344\272\213\351\241\271", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("QEventSysSetBase", "\345\205\266\344\273\226", 0, QApplication::UnicodeUTF8));
        m_eventToneChkBox->setText(QApplication::translate("QEventSysSetBase", "\350\257\255\351\237\263\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
        textLabel3_3_2_2->setText(QApplication::translate("QEventSysSetBase", "\350\257\255\351\237\263\350\256\276\345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        m_toneDev->setText(QString());
        m_msgChkBox->setText(QApplication::translate("QEventSysSetBase", "\347\237\255\344\277\241\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
        textLabel3_3_2->setText(QApplication::translate("QEventSysSetBase", "\347\237\255\344\277\241\350\256\276\345\244\207\357\274\232", 0, QApplication::UnicodeUTF8));
        m_serial_no->setText(QString());
        m_rcvmsgChkBox->setText(QApplication::translate("QEventSysSetBase", "\347\237\255\344\277\241\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        textLabel3_3_2_3->setText(QApplication::translate("QEventSysSetBase", "\347\237\255\344\277\241\347\241\256\350\256\244\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        m_rcvmsgTime->setText(QString());
        m_eventPrintChkBox->setText(QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QEventSysSetBase", "\346\211\223\345\215\260\346\234\272\357\274\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_printer->setToolTip(QApplication::translate("QEventSysSetBase", "LPT1-n,\\\\server\\share_printer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_printer->setText(QApplication::translate("QEventSysSetBase", "LPT1", 0, QApplication::UnicodeUTF8));
        m_dbgChkBox->setText(QApplication::translate("QEventSysSetBase", "\350\260\203\350\257\225\350\276\223\345\207\272", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Widget8), QApplication::translate("QEventSysSetBase", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        m_eventColorCB2->clear();
        m_eventColorCB2->insertItems(0, QStringList()
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\345\210\206\347\261\273\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\347\261\273\345\236\213\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\345\216\202\347\253\231\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QEventSysSetBase", "\344\272\213\351\241\271\347\272\247\345\210\253\351\242\234\350\211\262", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(Widget9), QApplication::translate("QEventSysSetBase", "\351\242\234\350\211\262\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_colList->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("QEventSysSetBase", "\345\210\227\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_colUpBtn->setText(QApplication::translate("QEventSysSetBase", "\345\220\221\344\270\212", 0, QApplication::UnicodeUTF8));
        m_colDownBtn->setText(QApplication::translate("QEventSysSetBase", "\345\220\221\344\270\213", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TabPage), QApplication::translate("QEventSysSetBase", "\346\230\276\347\244\272\345\210\227", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = m_soundTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("QEventSysSetBase", "\346\212\245\350\255\246\346\254\241\346\225\260", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TabPage_2), QApplication::translate("QEventSysSetBase", "\350\257\255\351\237\263\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = m_phoneNoTable->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("QEventSysSetBase", "\346\211\213\346\234\272\345\217\267\347\240\201", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = m_phoneNoTable->horizontalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("QEventSysSetBase", "\350\257\246\347\273\206\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TabPage_3), QApplication::translate("QEventSysSetBase", "\347\237\255\344\277\241\346\212\245\350\255\246", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem1 = m_reportTree->headerItem();
        ___qtreewidgetitem1->setText(2, QApplication::translate("QEventSysSetBase", "\346\212\245\350\241\250\347\233\256\345\275\225", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(1, QApplication::translate("QEventSysSetBase", "\347\261\273\345\236\213\345\217\267", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem1->setText(0, QApplication::translate("QEventSysSetBase", "\345\216\202\347\253\231\345\220\215\347\247\260", 0, QApplication::UnicodeUTF8));
        m_reportFileBtn->setText(QApplication::translate("QEventSysSetBase", "\351\200\211\346\213\251\346\212\245\350\241\250", 0, QApplication::UnicodeUTF8));
        m_isReport->setText(QApplication::translate("QEventSysSetBase", "\346\212\245\350\241\250\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        m_hBtn->setText(QApplication::translate("QEventSysSetBase", "\346\250\252\345\220\221\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        m_vBtn->setText(QApplication::translate("QEventSysSetBase", "\347\272\265\345\220\221\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(m_reportTab), QApplication::translate("QEventSysSetBase", "\346\212\245\350\241\250\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("QEventSysSetBase", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QString());
        buttonOk->setText(QApplication::translate("QEventSysSetBase", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QString());
    } // retranslateUi

};

namespace Ui {
    class QEventSysSetBase: public Ui_QEventSysSetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QEVENTSYSSETBASE_H
