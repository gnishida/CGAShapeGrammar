/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionOpenCGA;
    QAction *actionExit;
    QAction *actionViewShadow;
    QAction *actionViewRegularRendering;
    QAction *actionViewWireframe;
    QAction *actionViewLineRendering;
    QAction *actionViewSketchyRendering;
    QAction *actionViewRefresh;
    QAction *actionGenerateWindowImages;
    QAction *actionGenerateBuildingImages;
    QAction *actionTest;
    QAction *actionGenerateSimpleShapeImages;
    QAction *actionGenerateLedgeImages;
    QAction *actionGenerateRoofImages;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTool;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(800, 600);
        actionOpenCGA = new QAction(MainWindowClass);
        actionOpenCGA->setObjectName(QStringLiteral("actionOpenCGA"));
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionViewShadow = new QAction(MainWindowClass);
        actionViewShadow->setObjectName(QStringLiteral("actionViewShadow"));
        actionViewShadow->setCheckable(true);
        actionViewRegularRendering = new QAction(MainWindowClass);
        actionViewRegularRendering->setObjectName(QStringLiteral("actionViewRegularRendering"));
        actionViewRegularRendering->setCheckable(true);
        actionViewWireframe = new QAction(MainWindowClass);
        actionViewWireframe->setObjectName(QStringLiteral("actionViewWireframe"));
        actionViewWireframe->setCheckable(true);
        actionViewLineRendering = new QAction(MainWindowClass);
        actionViewLineRendering->setObjectName(QStringLiteral("actionViewLineRendering"));
        actionViewLineRendering->setCheckable(true);
        actionViewSketchyRendering = new QAction(MainWindowClass);
        actionViewSketchyRendering->setObjectName(QStringLiteral("actionViewSketchyRendering"));
        actionViewSketchyRendering->setCheckable(true);
        actionViewRefresh = new QAction(MainWindowClass);
        actionViewRefresh->setObjectName(QStringLiteral("actionViewRefresh"));
        actionGenerateWindowImages = new QAction(MainWindowClass);
        actionGenerateWindowImages->setObjectName(QStringLiteral("actionGenerateWindowImages"));
        actionGenerateBuildingImages = new QAction(MainWindowClass);
        actionGenerateBuildingImages->setObjectName(QStringLiteral("actionGenerateBuildingImages"));
        actionTest = new QAction(MainWindowClass);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        actionGenerateSimpleShapeImages = new QAction(MainWindowClass);
        actionGenerateSimpleShapeImages->setObjectName(QStringLiteral("actionGenerateSimpleShapeImages"));
        actionGenerateLedgeImages = new QAction(MainWindowClass);
        actionGenerateLedgeImages->setObjectName(QStringLiteral("actionGenerateLedgeImages"));
        actionGenerateRoofImages = new QAction(MainWindowClass);
        actionGenerateRoofImages->setObjectName(QStringLiteral("actionGenerateRoofImages"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuTool = new QMenu(menuBar);
        menuTool->setObjectName(QStringLiteral("menuTool"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTool->menuAction());
        menuFile->addAction(actionOpenCGA);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionViewShadow);
        menuView->addSeparator();
        menuView->addAction(actionViewRegularRendering);
        menuView->addAction(actionViewWireframe);
        menuView->addAction(actionViewLineRendering);
        menuView->addAction(actionViewSketchyRendering);
        menuView->addSeparator();
        menuView->addAction(actionViewRefresh);
        menuTool->addAction(actionGenerateBuildingImages);
        menuTool->addAction(actionGenerateRoofImages);
        menuTool->addAction(actionGenerateWindowImages);
        menuTool->addAction(actionGenerateLedgeImages);
        menuTool->addAction(actionGenerateSimpleShapeImages);
        menuTool->addSeparator();
        menuTool->addAction(actionTest);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        actionOpenCGA->setText(QApplication::translate("MainWindowClass", "Open CGA", 0));
        actionOpenCGA->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionViewShadow->setText(QApplication::translate("MainWindowClass", "Shadow", 0));
        actionViewRegularRendering->setText(QApplication::translate("MainWindowClass", "Regular Rendering", 0));
        actionViewWireframe->setText(QApplication::translate("MainWindowClass", "Wireframe", 0));
        actionViewLineRendering->setText(QApplication::translate("MainWindowClass", "Line Rendering", 0));
        actionViewSketchyRendering->setText(QApplication::translate("MainWindowClass", "Sketchy Rendering", 0));
        actionViewRefresh->setText(QApplication::translate("MainWindowClass", "Refresh", 0));
        actionViewRefresh->setShortcut(QApplication::translate("MainWindowClass", "F5", 0));
        actionGenerateWindowImages->setText(QApplication::translate("MainWindowClass", "Generate Window Images", 0));
        actionGenerateBuildingImages->setText(QApplication::translate("MainWindowClass", "Generate Building Images", 0));
        actionTest->setText(QApplication::translate("MainWindowClass", "Test", 0));
        actionGenerateSimpleShapeImages->setText(QApplication::translate("MainWindowClass", "Generate Simple Shape Images", 0));
        actionGenerateLedgeImages->setText(QApplication::translate("MainWindowClass", "Generate Ledge Images", 0));
        actionGenerateRoofImages->setText(QApplication::translate("MainWindowClass", "Generate Roof Images", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0));
        menuTool->setTitle(QApplication::translate("MainWindowClass", "Tool", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
