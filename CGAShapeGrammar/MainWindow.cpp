#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);
	ui.actionViewWireframe->setChecked(true);
	ui.actionViewShadow->setChecked(false);
	ui.actionViewScopeCoordinateSystem->setChecked(false);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGARules, SIGNAL(triggered()), this, SLOT(onOpenCGARules()));
	connect(ui.actionViewWireframe, SIGNAL(triggered()), this, SLOT(onViewWireframe()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewScopeCoordinateSystem, SIGNAL(triggered()), this, SLOT(onViewScopeCoordinateSystem()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));

	glWidget = new GLWidget3D();
	setCentralWidget(glWidget);

	fileLoaded = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpenCGARules() {
	QString new_filename = QFileDialog::getOpenFileName(this, tr("Open CGA file..."), "", tr("CGA Files (*.xml)"));
	if (new_filename.isEmpty()) return;

	fileLoaded = true;
	filename = new_filename;
	glWidget->loadCGA(filename.toUtf8().data());
}

void MainWindow::onViewWireframe() {
	glWidget->showWireframe = ui.actionViewWireframe->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewShadow() {
	glWidget->useShadow = ui.actionViewShadow->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewScopeCoordinateSystem() {
	glWidget->showScopeCoordinateSystem = ui.actionViewScopeCoordinateSystem->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewRefresh() {
	if (fileLoaded) {
		glWidget->loadCGA(filename.toUtf8().data());
	}
}