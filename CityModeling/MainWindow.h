#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "GLWidget3D.h"
#include <QKeyEvent>

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

protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
void onOpenOSM();
	void onSaveImage();
	void onSaveOBJ();
	void onViewShadow();
	void onViewRendering();
};

#endif // MAINWINDOW_H
