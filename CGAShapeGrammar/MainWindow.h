#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
		
private:
	Ui::MainWindowClass ui;
	GLWidget3D* glWidget;
	bool fileLoaded;
	QString filename;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void onOpenCGA();
	void onSaveGeometry();
	void onViewShadow();
	void onViewRendering();
	void onViewRefresh();
	void onRotationStart();
	void onRotationEnd();
	void onGenerateBuildingImages();
	void onGenerateBuildingImages2();
	void onGenerateRoofImages();
	void onGenerateRoofImages2();
	void onGenerateWindowImages();
	void onGenerateWindowImages2();
	void onGenerateLedgeImages();
	void onGenerateLedgeImages2();
	void onTest();
	void camera_update();
};

#endif // MAINWINDOW_H
