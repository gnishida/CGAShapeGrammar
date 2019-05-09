/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
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
    QAction *actionViewBasicRendering;
    QAction *actionViewSSAO;
    QAction *actionViewLineRendering;
    QAction *actionViewSketchyRendering;
    QAction *actionViewRefresh;
    QAction *actionGenerateWindowImages;
    QAction *actionGenerateBuildingImages;
    QAction *actionTest;
    QAction *actionGenerateSimpleShapeImages;
    QAction *actionGenerateLedgeImages;
    QAction *actionGenerateRoofImages;
    QAction *actionGenerateRoofImages2;
    QAction *actionGenerateWindowImages2;
    QAction *actionGenerateLedgeImages2;
    QAction *actionGenerateBuildingImages2;
    QAction *actionViewHatching;
    QAction *actionRotationStart;
    QAction *actionRotationEnd;
    QAction *actionSaveGeometry;
    QAction *actionViewContourRendering;
    QAction *actionCamera;
    QAction *actionSaveImage;
    QAction *actionSaveOBJ;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuTools;
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
        actionViewBasicRendering = new QAction(MainWindowClass);
        actionViewBasicRendering->setObjectName(QStringLiteral("actionViewBasicRendering"));
        actionViewBasicRendering->setCheckable(true);
        actionViewSSAO = new QAction(MainWindowClass);
        actionViewSSAO->setObjectName(QStringLiteral("actionViewSSAO"));
        actionViewSSAO->setCheckable(true);
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
        actionGenerateRoofImages2 = new QAction(MainWindowClass);
        actionGenerateRoofImages2->setObjectName(QStringLiteral("actionGenerateRoofImages2"));
        actionGenerateWindowImages2 = new QAction(MainWindowClass);
        actionGenerateWindowImages2->setObjectName(QStringLiteral("actionGenerateWindowImages2"));
        actionGenerateLedgeImages2 = new QAction(MainWindowClass);
        actionGenerateLedgeImages2->setObjectName(QStringLiteral("actionGenerateLedgeImages2"));
        actionGenerateBuildingImages2 = new QAction(MainWindowClass);
        actionGenerateBuildingImages2->setObjectName(QStringLiteral("actionGenerateBuildingImages2"));
        actionViewHatching = new QAction(MainWindowClass);
        actionViewHatching->setObjectName(QStringLiteral("actionViewHatching"));
        actionViewHatching->setCheckable(true);
        actionRotationStart = new QAction(MainWindowClass);
        actionRotationStart->setObjectName(QStringLiteral("actionRotationStart"));
        actionRotationEnd = new QAction(MainWindowClass);
        actionRotationEnd->setObjectName(QStringLiteral("actionRotationEnd"));
        actionSaveGeometry = new QAction(MainWindowClass);
        actionSaveGeometry->setObjectName(QStringLiteral("actionSaveGeometry"));
        actionViewContourRendering = new QAction(MainWindowClass);
        actionViewContourRendering->setObjectName(QStringLiteral("actionViewContourRendering"));
        actionViewContourRendering->setCheckable(true);
        actionCamera = new QAction(MainWindowClass);
        actionCamera->setObjectName(QStringLiteral("actionCamera"));
        actionSaveImage = new QAction(MainWindowClass);
        actionSaveImage->setObjectName(QStringLiteral("actionSaveImage"));
        actionSaveOBJ = new QAction(MainWindowClass);
        actionSaveOBJ->setObjectName(QStringLiteral("actionSaveOBJ"));
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
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(actionOpenCGA);
        menuFile->addAction(actionSaveImage);
        menuFile->addAction(actionSaveOBJ);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuView->addAction(actionViewShadow);
        menuView->addSeparator();
        menuView->addAction(actionViewBasicRendering);
        menuView->addAction(actionViewSSAO);
        menuView->addAction(actionViewContourRendering);
        menuView->addAction(actionViewLineRendering);
        menuView->addAction(actionViewHatching);
        menuView->addAction(actionViewSketchyRendering);
        menuView->addSeparator();
        menuView->addAction(actionViewRefresh);
        menuView->addSeparator();
        menuView->addAction(actionRotationStart);
        menuView->addAction(actionRotationEnd);
        menuTools->addAction(actionCamera);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "CGA Shape Grammar", 0));
        actionOpenCGA->setText(QApplication::translate("MainWindowClass", "Open CGA", 0));
        actionOpenCGA->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionViewShadow->setText(QApplication::translate("MainWindowClass", "Shadow", 0));
        actionViewBasicRendering->setText(QApplication::translate("MainWindowClass", "Basic Rendering", 0));
        actionViewSSAO->setText(QApplication::translate("MainWindowClass", "SSAO", 0));
        actionViewLineRendering->setText(QApplication::translate("MainWindowClass", "Line Rendering", 0));
        actionViewSketchyRendering->setText(QApplication::translate("MainWindowClass", "Sketchy Rendering", 0));
        actionViewRefresh->setText(QApplication::translate("MainWindowClass", "Refresh", 0));
        actionViewRefresh->setShortcut(QApplication::translate("MainWindowClass", "F5", 0));
        actionGenerateWindowImages->setText(QApplication::translate("MainWindowClass", "Generate Window Images (256x256 color)", 0));
        actionGenerateBuildingImages->setText(QApplication::translate("MainWindowClass", "Generate Building Images (256x256 color)", 0));
        actionTest->setText(QApplication::translate("MainWindowClass", "Test", 0));
        actionGenerateSimpleShapeImages->setText(QApplication::translate("MainWindowClass", "Generate Simple Shape Images", 0));
        actionGenerateLedgeImages->setText(QApplication::translate("MainWindowClass", "Generate Ledge Images (256x256 color)", 0));
        actionGenerateRoofImages->setText(QApplication::translate("MainWindowClass", "Generate Roof Images (256x256 color)", 0));
        actionGenerateRoofImages2->setText(QApplication::translate("MainWindowClass", "Generate Roof Images (128x128 grayscale)", 0));
        actionGenerateWindowImages2->setText(QApplication::translate("MainWindowClass", "Generate Window Images (128x128 grayscale)", 0));
        actionGenerateLedgeImages2->setText(QApplication::translate("MainWindowClass", "Generate Ledge Images (128x128 grayscale)", 0));
        actionGenerateBuildingImages2->setText(QApplication::translate("MainWindowClass", "Generate Building Images (128x128 grayscale)", 0));
        actionViewHatching->setText(QApplication::translate("MainWindowClass", "Hatching", 0));
        actionRotationStart->setText(QApplication::translate("MainWindowClass", "Rotation Start", 0));
        actionRotationEnd->setText(QApplication::translate("MainWindowClass", "Rotation End", 0));
        actionSaveGeometry->setText(QApplication::translate("MainWindowClass", "Save Geometry", 0));
        actionSaveGeometry->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+S", 0));
        actionViewContourRendering->setText(QApplication::translate("MainWindowClass", "Contour Rendering", 0));
        actionCamera->setText(QApplication::translate("MainWindowClass", "Camera", 0));
        actionSaveImage->setText(QApplication::translate("MainWindowClass", "Save Image", 0));
        actionSaveOBJ->setText(QApplication::translate("MainWindowClass", "Save OBJ", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuView->setTitle(QApplication::translate("MainWindowClass", "View", 0));
        menuTools->setTitle(QApplication::translate("MainWindowClass", "Tools", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
