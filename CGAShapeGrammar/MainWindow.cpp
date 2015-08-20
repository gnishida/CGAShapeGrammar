#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);
	ui.actionShowWireframe->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionShowWireframe, SIGNAL(triggered()), this, SLOT(onShowWireframe()));

	glWidget = new GLWidget3D();
	setCentralWidget(glWidget);
}

MainWindow::~MainWindow() {
}

void MainWindow::onShowWireframe() {
	glWidget->showWireframe = ui.actionShowWireframe->isChecked();
	glWidget->updateGL();
}