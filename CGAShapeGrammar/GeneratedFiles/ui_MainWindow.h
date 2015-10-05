/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Oct 5 16:24:50 2015
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionViewWireframe;
    QAction *actionOpenCGARules;
    QAction *actionViewScopeCoordinateSystem;
    QAction *actionViewRefresh;
    QAction *actionViewShadow;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(795, 575);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionViewWireframe = new QAction(MainWindowClass);
        actionViewWireframe->setObjectName(QString::fromUtf8("actionViewWireframe"));
        actionViewWireframe->setCheckable(true);
        actionOpenCGARules = new QAction(MainWindowClass);
        actionOpenCGARules->setObjectName(QString::fromUtf8("actionOpenCGARules"));
        actionViewScopeCoordinateSystem = new QAction(MainWindowClass);
        actionViewScopeCoordinateSystem->setObjectName(QString::fromUtf8("actionViewScopeCoordinateSystem"));
        actionViewScopeCoordinateSystem->setCheckable(true);
        actionViewRefresh = new QAction(MainWindowClass);
        actionViewRefresh->setObjectName(QString::fromUtf8("actionViewRefresh"));
        actionViewShadow = new QAction(MainWindowClass);
        actionViewShadow->setObjectName(QString::fromUtf8("actionViewShadow"));
        actionViewShadow->setCheckable(true);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 795, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpenCGARules);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionViewWireframe);
        menuView->addAction(actionViewShadow);
        menuView->addAction(actionViewScopeCoordinateSystem);
        menuView->addSeparator();
        menuView->addAction(actionViewRefresh);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "CGA Shape Grammar", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionViewWireframe->setText(QApplication::translate("MainWindowClass", "Wireframe", 0, QApplication::UnicodeUTF8));
        actionViewWireframe->setShortcut(QApplication::translate("MainWindowClass", "W", 0, QApplication::UnicodeUTF8));
        actionOpenCGARules->setText(QApplication::translate("MainWindowClass", "Open CGA Rules", 0, QApplication::UnicodeUTF8));
        actionOpenCGARules->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionViewScopeCoordinateSystem->setText(QApplication::translate("MainWindowClass", "Scope Coordinate System", 0, QApplication::UnicodeUTF8));
        actionViewScopeCoordinateSystem->setShortcut(QApplication::translate("MainWindowClass", "C", 0, QApplication::UnicodeUTF8));
        actionViewRefresh->setText(QApplication::translate("MainWindowClass", "Refresh", 0, QApplication::UnicodeUTF8));
        actionViewRefresh->setShortcut(QApplication::translate("MainWindowClass", "F5", 0, QApplication::UnicodeUTF8));
        actionViewShadow->setText(QApplication::translate("MainWindowClass", "Shadow", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
