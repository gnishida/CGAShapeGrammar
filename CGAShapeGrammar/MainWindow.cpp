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
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewLineRendering()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));
	connect(ui.actionGenerateImages, SIGNAL(triggered()), this, SLOT(onGenerateImages()));
	connect(ui.actionHoge, SIGNAL(triggered()), this, SLOT(onHoge()));

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
	this->setWindowTitle("CGA Shape Grammar - " + new_filename);
}

void MainWindow::onViewWireframe() {
	glWidget->showWireframe = ui.actionViewWireframe->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewLineRendering() {
	glWidget->renderingMode = GLWidget3D::RENDERING_MODE_LINE;
	glWidget->updateGL();
}

void MainWindow::onViewShadow() {
	glWidget->renderManager.useShadow = ui.actionViewShadow->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewRefresh() {
	if (fileLoaded) {
		glWidget->loadCGA(filename.toUtf8().data());
	}
}

void MainWindow::onGenerateImages() {
	glWidget->generateImages();
}

void MainWindow::onHoge() {
	glWidget->hoge();
}