#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);
	ui.actionViewWireframe->setChecked(true);
	ui.actionViewScopeCoordinateSystem->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGARules, SIGNAL(triggered()), this, SLOT(onOpenCGARules()));
	connect(ui.actionViewWireframe, SIGNAL(triggered()), this, SLOT(onViewWireframe()));
	connect(ui.actionViewScopeCoordinateSystem, SIGNAL(triggered()), this, SLOT(onViewScopeCoordinateSystem()));

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

void MainWindow::onViewWireframe() {
	glWidget->showWireframe = ui.actionViewWireframe->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewScopeCoordinateSystem() {
	glWidget->showScopeCoordinateSystem = ui.actionViewScopeCoordinateSystem->isChecked();
	glWidget->updateGL();
}