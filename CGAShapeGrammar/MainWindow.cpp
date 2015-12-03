#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	QActionGroup* renderingModeGroup = new QActionGroup(this);
	renderingModeGroup->addAction(ui.actionViewRegularRendering);
	renderingModeGroup->addAction(ui.actionViewWireframe);
	renderingModeGroup->addAction(ui.actionViewLineRendering);
	renderingModeGroup->addAction(ui.actionViewSketchyRendering);

	ui.actionViewWireframe->setChecked(true);
	ui.actionViewShadow->setChecked(false);
	ui.actionViewRegularRendering->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGA, SIGNAL(triggered()), this, SLOT(onOpenCGA()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewRegularRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewWireframe, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));

	connect(ui.actionGenerateBuildingImages, SIGNAL(triggered()), this, SLOT(onGenerateBuildingImages()));
	connect(ui.actionGenerateRoofImages, SIGNAL(triggered()), this, SLOT(onGenerateRoofImages()));
	connect(ui.actionGenerateWindowImages, SIGNAL(triggered()), this, SLOT(onGenerateWindowImages()));
	connect(ui.actionGenerateLedgeImages, SIGNAL(triggered()), this, SLOT(onGenerateLedgeImages()));
	connect(ui.actionGenerateSimpleShapeImages, SIGNAL(triggered()), this, SLOT(onGenerateSimpleShapeImages()));
	connect(ui.actionTest, SIGNAL(triggered()), this, SLOT(onTest()));

	glWidget = new GLWidget3D();
	setCentralWidget(glWidget);

	fileLoaded = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::onOpenCGA() {
	QString new_filename = QFileDialog::getOpenFileName(this, tr("Open CGA file..."), "", tr("CGA Files (*.xml)"));
	if (new_filename.isEmpty()) return;

	fileLoaded = true;
	filename = new_filename;
	glWidget->loadCGA(filename.toUtf8().data());
	this->setWindowTitle("CGA Shape Grammar - " + new_filename);
}

void MainWindow::onViewShadow() {
	glWidget->renderManager.useShadow = ui.actionViewShadow->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewRendering() {
	if (ui.actionViewRegularRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_REGULAR;
	} else if (ui.actionViewWireframe->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_WIREFRAME;
	} else if (ui.actionViewLineRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;
	} else {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SKETCHY;
	}
	glWidget->updateGL();
}

void MainWindow::onViewRefresh() {
	if (fileLoaded) {
		glWidget->loadCGA(filename.toUtf8().data());
	}
}

void MainWindow::onGenerateBuildingImages() {
	//glWidget->generateBuildingImages(128, 128, true);
	glWidget->generateBuildingImages(256, 256, false);
}

void MainWindow::onGenerateRoofImages() {
	//glWidget->generateRoofImages(128, 128, true);
	glWidget->generateRoofImages(256, 256, false);
}

void MainWindow::onGenerateWindowImages() {
	//glWidget->generateWindowImages(128, 128, true);
	glWidget->generateWindowImages(256, 256, false);
}

void MainWindow::onGenerateLedgeImages() {
	//glWidget->generateLedgeImages(128, 128, true);
	glWidget->generateLedgeImages(256, 256, false);
}

void MainWindow::onGenerateSimpleShapeImages() {
	glWidget->generateSimpleShapeImages(512, 512, 0.25);
}

void MainWindow::onTest() {
	glWidget->test();
}

