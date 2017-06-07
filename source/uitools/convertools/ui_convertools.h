/********************************************************************************
** Form generated from reading UI file 'convertools.ui'
**
** Created: Fri Apr 7 09:19:54 2017
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERTOOLS_H
#define UI_CONVERTOOLS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_convertbase
{
public:
    QAction *actionOpenDir;
    QAction *actionSearch;
    QAction *actionExit;
    QAction *actionConvert;
    QAction *actionOpenFile;
    QAction *actionDelete;
    QAction *actionAbout;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QListWidget *m_sourceList;
    QListWidget *m_outputList;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QToolBar *toolBar;

    void setupUi(QMainWindow *convertbase)
    {
        if (convertbase->objectName().isEmpty())
            convertbase->setObjectName(QString::fromUtf8("convertbase"));
        convertbase->resize(500, 447);
        actionOpenDir = new QAction(convertbase);
        actionOpenDir->setObjectName(QString::fromUtf8("actionOpenDir"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenDir->setIcon(icon);
        actionSearch = new QAction(convertbase);
        actionSearch->setObjectName(QString::fromUtf8("actionSearch"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSearch->setIcon(icon1);
        actionExit = new QAction(convertbase);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionConvert = new QAction(convertbase);
        actionConvert->setObjectName(QString::fromUtf8("actionConvert"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConvert->setIcon(icon3);
        actionOpenFile = new QAction(convertbase);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/open_file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon4);
        actionDelete = new QAction(convertbase);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDelete->setIcon(icon5);
        actionAbout = new QAction(convertbase);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(convertbase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_sourceList = new QListWidget(centralwidget);
        m_sourceList->setObjectName(QString::fromUtf8("m_sourceList"));

        gridLayout->addWidget(m_sourceList, 0, 0, 1, 1);

        m_outputList = new QListWidget(centralwidget);
        m_outputList->setObjectName(QString::fromUtf8("m_outputList"));

        gridLayout->addWidget(m_outputList, 0, 1, 1, 1);

        convertbase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(convertbase);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 500, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        convertbase->setMenuBar(menubar);
        toolBar = new QToolBar(convertbase);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        convertbase->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menu->addAction(actionOpenFile);
        menu->addAction(actionOpenDir);
        menu->addSeparator();
        menu->addAction(actionExit);
        menu_2->addAction(actionDelete);
        menu_2->addSeparator();
        menu_3->addAction(actionConvert);
        menu_4->addAction(actionAbout);
        toolBar->addAction(actionOpenFile);
        toolBar->addAction(actionOpenDir);
        toolBar->addAction(actionDelete);
        toolBar->addSeparator();
        toolBar->addAction(actionConvert);
        toolBar->addSeparator();
        toolBar->addAction(actionExit);

        retranslateUi(convertbase);

        QMetaObject::connectSlotsByName(convertbase);
    } // setupUi

    void retranslateUi(QMainWindow *convertbase)
    {
        convertbase->setWindowTitle(QApplication::translate("convertbase", "\350\275\254\346\215\242\345\267\245\345\205\267", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setText(QApplication::translate("convertbase", "\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271", 0, QApplication::UnicodeUTF8));
        actionOpenDir->setShortcut(QApplication::translate("convertbase", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionSearch->setText(QApplication::translate("convertbase", "\346\237\245\346\211\276\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        actionSearch->setShortcut(QApplication::translate("convertbase", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("convertbase", "\351\200\200\345\207\272", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("convertbase", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionConvert->setText(QApplication::translate("convertbase", "GDF\350\275\254BGF", 0, QApplication::UnicodeUTF8));
        actionOpenFile->setText(QApplication::translate("convertbase", "\346\211\223\345\274\200\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        actionOpenFile->setShortcut(QApplication::translate("convertbase", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("convertbase", "\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("convertbase", "\345\205\263\344\272\216", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("convertbase", "\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("convertbase", "\347\274\226\350\276\221", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("convertbase", "\346\223\215\344\275\234", 0, QApplication::UnicodeUTF8));
        menu_4->setTitle(QApplication::translate("convertbase", "\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("convertbase", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class convertbase: public Ui_convertbase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTOOLS_H
