/********************************************************************************
** Form generated from reading UI file 'hisquery_modify_base.ui'
**
** Created: Fri Apr 7 09:20:44 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISQUERY_MODIFY_BASE_H
#define UI_HISQUERY_MODIFY_BASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Event_modify_dialog_base
{
public:
    QGridLayout *gridLayout;
    QLabel *textLabel1;
    QComboBox *m_pGroup_combobox;
    QLabel *textLabel2;
    QComboBox *m_pEvent_sort_combobox;
    QLabel *textLabel3;
    QComboBox *m_pEvent_type_combobox;
    QLabel *textLabel4;
    QHBoxLayout *horizontalLayout_2;
    QDateTimeEdit *m_pDatetime_edit;
    QLineEdit *m_pMs_edit;
    QLabel *textLabel5;
    QComboBox *m_pRtu_combobox;
    QLabel *textLabel6;
    QComboBox *m_pObject_combobox;
    QLabel *textLabel7;
    QLineEdit *m_pEvent_desc_edit;
    QLabel *textLabel8;
    QLineEdit *m_pValue_edit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_pModify_button;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *m_pDelete_button;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *m_pAdd_button;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *Event_modify_dialog_base)
    {
        if (Event_modify_dialog_base->objectName().isEmpty())
            Event_modify_dialog_base->setObjectName(QString::fromUtf8("Event_modify_dialog_base"));
        Event_modify_dialog_base->resize(349, 374);
        gridLayout = new QGridLayout(Event_modify_dialog_base);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel1 = new QLabel(Event_modify_dialog_base);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 0, 1, 1);

        m_pGroup_combobox = new QComboBox(Event_modify_dialog_base);
        m_pGroup_combobox->setObjectName(QString::fromUtf8("m_pGroup_combobox"));

        gridLayout->addWidget(m_pGroup_combobox, 0, 1, 1, 1);

        textLabel2 = new QLabel(Event_modify_dialog_base);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout->addWidget(textLabel2, 1, 0, 1, 1);

        m_pEvent_sort_combobox = new QComboBox(Event_modify_dialog_base);
        m_pEvent_sort_combobox->setObjectName(QString::fromUtf8("m_pEvent_sort_combobox"));

        gridLayout->addWidget(m_pEvent_sort_combobox, 1, 1, 1, 1);

        textLabel3 = new QLabel(Event_modify_dialog_base);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout->addWidget(textLabel3, 2, 0, 1, 1);

        m_pEvent_type_combobox = new QComboBox(Event_modify_dialog_base);
        m_pEvent_type_combobox->setObjectName(QString::fromUtf8("m_pEvent_type_combobox"));

        gridLayout->addWidget(m_pEvent_type_combobox, 2, 1, 1, 1);

        textLabel4 = new QLabel(Event_modify_dialog_base);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        gridLayout->addWidget(textLabel4, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        m_pDatetime_edit = new QDateTimeEdit(Event_modify_dialog_base);
        m_pDatetime_edit->setObjectName(QString::fromUtf8("m_pDatetime_edit"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pDatetime_edit->sizePolicy().hasHeightForWidth());
        m_pDatetime_edit->setSizePolicy(sizePolicy);
        m_pDatetime_edit->setCalendarPopup(true);

        horizontalLayout_2->addWidget(m_pDatetime_edit);

        m_pMs_edit = new QLineEdit(Event_modify_dialog_base);
        m_pMs_edit->setObjectName(QString::fromUtf8("m_pMs_edit"));
        m_pMs_edit->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(m_pMs_edit);


        gridLayout->addLayout(horizontalLayout_2, 3, 1, 1, 1);

        textLabel5 = new QLabel(Event_modify_dialog_base);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setWordWrap(false);

        gridLayout->addWidget(textLabel5, 4, 0, 1, 1);

        m_pRtu_combobox = new QComboBox(Event_modify_dialog_base);
        m_pRtu_combobox->setObjectName(QString::fromUtf8("m_pRtu_combobox"));

        gridLayout->addWidget(m_pRtu_combobox, 4, 1, 1, 1);

        textLabel6 = new QLabel(Event_modify_dialog_base);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setWordWrap(false);

        gridLayout->addWidget(textLabel6, 5, 0, 1, 1);

        m_pObject_combobox = new QComboBox(Event_modify_dialog_base);
        m_pObject_combobox->setObjectName(QString::fromUtf8("m_pObject_combobox"));

        gridLayout->addWidget(m_pObject_combobox, 5, 1, 1, 1);

        textLabel7 = new QLabel(Event_modify_dialog_base);
        textLabel7->setObjectName(QString::fromUtf8("textLabel7"));
        textLabel7->setWordWrap(false);

        gridLayout->addWidget(textLabel7, 6, 0, 1, 1);

        m_pEvent_desc_edit = new QLineEdit(Event_modify_dialog_base);
        m_pEvent_desc_edit->setObjectName(QString::fromUtf8("m_pEvent_desc_edit"));

        gridLayout->addWidget(m_pEvent_desc_edit, 6, 1, 1, 1);

        textLabel8 = new QLabel(Event_modify_dialog_base);
        textLabel8->setObjectName(QString::fromUtf8("textLabel8"));
        textLabel8->setWordWrap(false);

        gridLayout->addWidget(textLabel8, 7, 0, 1, 1);

        m_pValue_edit = new QLineEdit(Event_modify_dialog_base);
        m_pValue_edit->setObjectName(QString::fromUtf8("m_pValue_edit"));

        gridLayout->addWidget(m_pValue_edit, 7, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_pModify_button = new QPushButton(Event_modify_dialog_base);
        m_pModify_button->setObjectName(QString::fromUtf8("m_pModify_button"));

        horizontalLayout->addWidget(m_pModify_button);

        horizontalSpacer_2 = new QSpacerItem(25, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        m_pDelete_button = new QPushButton(Event_modify_dialog_base);
        m_pDelete_button->setObjectName(QString::fromUtf8("m_pDelete_button"));

        horizontalLayout->addWidget(m_pDelete_button);

        horizontalSpacer_3 = new QSpacerItem(26, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        m_pAdd_button = new QPushButton(Event_modify_dialog_base);
        m_pAdd_button->setObjectName(QString::fromUtf8("m_pAdd_button"));

        horizontalLayout->addWidget(m_pAdd_button);

        horizontalSpacer_4 = new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout, 8, 0, 1, 2);

        gridLayout->setRowStretch(0, 1);
        QWidget::setTabOrder(m_pGroup_combobox, m_pEvent_sort_combobox);
        QWidget::setTabOrder(m_pEvent_sort_combobox, m_pEvent_type_combobox);
        QWidget::setTabOrder(m_pEvent_type_combobox, m_pDatetime_edit);
        QWidget::setTabOrder(m_pDatetime_edit, m_pMs_edit);
        QWidget::setTabOrder(m_pMs_edit, m_pRtu_combobox);
        QWidget::setTabOrder(m_pRtu_combobox, m_pObject_combobox);
        QWidget::setTabOrder(m_pObject_combobox, m_pEvent_desc_edit);
        QWidget::setTabOrder(m_pEvent_desc_edit, m_pValue_edit);
        QWidget::setTabOrder(m_pValue_edit, m_pModify_button);
        QWidget::setTabOrder(m_pModify_button, m_pDelete_button);
        QWidget::setTabOrder(m_pDelete_button, m_pAdd_button);

        retranslateUi(Event_modify_dialog_base);

        QMetaObject::connectSlotsByName(Event_modify_dialog_base);
    } // setupUi

    void retranslateUi(QDialog *Event_modify_dialog_base)
    {
        Event_modify_dialog_base->setWindowTitle(QApplication::translate("Event_modify_dialog_base", "\345\216\206\345\217\262\344\272\213\351\241\271\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("Event_modify_dialog_base", "\345\210\206\347\273\204\345\272\217\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("Event_modify_dialog_base", "\344\272\213\351\241\271\345\210\206\347\261\273\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("Event_modify_dialog_base", "\344\272\213\351\241\271\347\261\273\345\236\213\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("Event_modify_dialog_base", "\344\272\213\351\241\271\346\227\266\351\227\264\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("Event_modify_dialog_base", "\344\272\213\351\241\271\347\273\204\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel6->setText(QApplication::translate("Event_modify_dialog_base", "\345\257\271\350\261\241\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel7->setText(QApplication::translate("Event_modify_dialog_base", "\344\272\213\351\241\271\346\217\217\350\277\260\357\274\232", 0, QApplication::UnicodeUTF8));
        textLabel8->setText(QApplication::translate("Event_modify_dialog_base", "\345\200\274\357\274\232", 0, QApplication::UnicodeUTF8));
        m_pModify_button->setText(QApplication::translate("Event_modify_dialog_base", "\344\277\256\346\224\271", 0, QApplication::UnicodeUTF8));
        m_pDelete_button->setText(QApplication::translate("Event_modify_dialog_base", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        m_pAdd_button->setText(QApplication::translate("Event_modify_dialog_base", "\345\242\236\345\212\240", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Event_modify_dialog_base: public Ui_Event_modify_dialog_base {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISQUERY_MODIFY_BASE_H
