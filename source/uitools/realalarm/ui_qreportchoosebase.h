/********************************************************************************
** Form generated from reading UI file 'qreportchoosebase.ui'
**
** Created: Fri Apr 7 09:21:04 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QREPORTCHOOSEBASE_H
#define UI_QREPORTCHOOSEBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_reportChooseDlg
{
public:
    QGridLayout *gridLayout;
    QPushButton *m_okBtn;
    QSpacerItem *horizontalSpacer;
    QListWidget *m_reportList;
    QPushButton *m_cancelBtn;

    void setupUi(QDialog *reportChooseDlg)
    {
        if (reportChooseDlg->objectName().isEmpty())
            reportChooseDlg->setObjectName(QString::fromUtf8("reportChooseDlg"));
        reportChooseDlg->resize(262, 300);
        gridLayout = new QGridLayout(reportChooseDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_okBtn = new QPushButton(reportChooseDlg);
        m_okBtn->setObjectName(QString::fromUtf8("m_okBtn"));

        gridLayout->addWidget(m_okBtn, 3, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(79, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        m_reportList = new QListWidget(reportChooseDlg);
        m_reportList->setObjectName(QString::fromUtf8("m_reportList"));

        gridLayout->addWidget(m_reportList, 0, 0, 1, 4);

        m_cancelBtn = new QPushButton(reportChooseDlg);
        m_cancelBtn->setObjectName(QString::fromUtf8("m_cancelBtn"));

        gridLayout->addWidget(m_cancelBtn, 3, 1, 1, 1);


        retranslateUi(reportChooseDlg);

        QMetaObject::connectSlotsByName(reportChooseDlg);
    } // setupUi

    void retranslateUi(QDialog *reportChooseDlg)
    {
        reportChooseDlg->setWindowTitle(QApplication::translate("reportChooseDlg", "\351\200\211\346\213\251\346\212\245\350\241\250", 0, QApplication::UnicodeUTF8));
        m_okBtn->setText(QApplication::translate("reportChooseDlg", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        m_cancelBtn->setText(QApplication::translate("reportChooseDlg", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class reportChooseDlg: public Ui_reportChooseDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QREPORTCHOOSEBASE_H
