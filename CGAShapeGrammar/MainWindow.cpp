#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	QActionGroup* renderingModeGroup = new QActionGroup(this);
	renderingModeGroup->addAction(ui.actionViewBasicRendering);
	renderingModeGroup->addAction(ui.actionViewSSAO);
	renderingModeGroup->addAction(ui.actionViewLineRendering);
	renderingModeGroup->addAction(ui.actionViewSketchyRendering);

	//ui.actionViewSSAO->setChecked(true);
	//ui.actionViewShadow->setChecked(true);
	ui.actionViewBasicRendering->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenCGA, SIGNAL(triggered()), this, SLOT(onOpenCGA()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewBasicRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSSAO, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));

	connect(ui.actionGenerateBuildingImages, SIGNAL(triggered()), this, SLOT(onGenerateBuildingImages()));
	connect(ui.actionGenerateBuildingImages2, SIGNAL(triggered()), this, SLOT(onGenerateBuildingImages2()));
	connect(ui.actionGenerateRoofImages, SIGNAL(triggered()), this, SLOT(onGenerateRoofImages()));
	connect(ui.actionGenerateRoofImages2, SIGNAL(triggered()), this, SLOT(onGenerateRoofImages2()));
	connect(ui.actionGenerateWindowImages, SIGNAL(triggered()), this, SLOT(onGenerateWindowImages()));
	connect(ui.actionGenerateWindowImages2, SIGNAL(triggered()), this, SLOT(onGenerateWindowImages2()));
	connect(ui.actionGenerateLedgeImages, SIGNAL(triggered()), this, SLOT(onGenerateLedgeImages()));
	connect(ui.actionGenerateLedgeImages2, SIGNAL(triggered()), this, SLOT(onGenerateLedgeImages2()));
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
	if (ui.actionViewBasicRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_BASIC;
	} else if (ui.actionViewSSAO->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SSAO;
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
	//glWidget->generateBuildingImages(256, 256, false);
}

void MainWindow::onGenerateBuildingImages2() {
	//glWidget->generateBuildingImages(128, 128, true);
}

void MainWindow::onGenerateRoofImages() {
	//glWidget->generateRoofImages(256, 256, false);
}

void MainWindow::onGenerateRoofImages2() {
	//glWidget->generateRoofImages(128, 128, true);
}

void MainWindow::onGenerateWindowImages() {
	//glWidget->generateWindowImages(256, 256, false);
}

void MainWindow::onGenerateWindowImages2() {
	//glWidget->generateWindowImages(128, 128, true);
}

void MainWindow::onGenerateLedgeImages() {
	//glWidget->generateLedgeImages(256, 256, false);
}

void MainWindow::onGenerateLedgeImages2() {
	//glWidget->generateLedgeImages(128, 128, true);
}

void MainWindow::onTest() {
	glWidget->test();
}

