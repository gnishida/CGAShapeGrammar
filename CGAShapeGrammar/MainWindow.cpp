#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);
	ui.actionShowWireframe->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGARules, SIGNAL(triggered()), this, SLOT(onOpenCGARules()));
	connect(ui.actionShowWireframe, SIGNAL(triggered()), this, SLOT(onShowWireframe()));

	glWidget = new GLWidget3D();
	setCentralWidget(glWidget);
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpenCGARules() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open CGA file..."), "", tr("CGA Files (*.xml)"));
	if (filename.isEmpty()) return;

	glWidget->loadCGA(filename.toUtf8().data());
}

void MainWindow::onShowWireframe() {
	glWidget->showWireframe = ui.actionShowWireframe->isChecked();
	glWidget->updateGL();
}