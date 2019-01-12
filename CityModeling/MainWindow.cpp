#include "MainWindow.h"
#include <QFileDialog>

#ifndef M_PI
#define M_PI	3.1415926535
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	QActionGroup* renderingModeGroup = new QActionGroup(this);
	renderingModeGroup->addAction(ui.actionViewBasicRendering);
	renderingModeGroup->addAction(ui.actionViewSSAO);
	renderingModeGroup->addAction(ui.actionViewContourRendering);
	renderingModeGroup->addAction(ui.actionViewLineRendering);
	renderingModeGroup->addAction(ui.actionViewHatching);
	renderingModeGroup->addAction(ui.actionViewSketchyRendering);

	ui.actionViewShadow->setChecked(true);
	ui.actionViewBasicRendering->setChecked(true);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionOpenOSM, SIGNAL(triggered()), this, SLOT(onOpenOSM()));
	connect(ui.actionSaveImage, SIGNAL(triggered()), this, SLOT(onSaveImage()));
	connect(ui.actionSaveOBJ, SIGNAL(triggered()), this, SLOT(onSaveOBJ()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewBasicRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSSAO, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewContourRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewHatching, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));

	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);

	fileLoaded = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onOpenOSM() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open OSM file..."), "", tr("OSM Files (*.osm)"));
	if (filename.isEmpty()) return;

	glWidget->loadOSM(filename.toUtf8().data());
	this->setWindowTitle("City Modeling - " + filename);
}

void MainWindow::onSaveImage() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image file..."), "", tr("Image Files (*.png)"));
	if (filename.isEmpty()) return;

	glWidget->grabFrameBuffer().save(filename);
}

void MainWindow::onSaveOBJ() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save OBJ file..."), "", tr("OJB Files (*.obj)"));
	if (filename.isEmpty()) return;

	glWidget->saveOBJ(filename);
}

void MainWindow::onViewShadow() {
	glWidget->renderManager.useShadow = ui.actionViewShadow->isChecked();
	glWidget->updateGL();
}

void MainWindow::onViewRendering() {
	if (ui.actionViewBasicRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_BASIC;
	}
	else if (ui.actionViewSSAO->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SSAO;
	}
	else if (ui.actionViewContourRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_CONTOUR;
	}
	else if (ui.actionViewLineRendering->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_LINE;
	}
	else if (ui.actionViewHatching->isChecked()) {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_HATCHING;
	}
	else {
		glWidget->renderManager.renderingMode = RenderManager::RENDERING_MODE_SKETCHY;
	}
	glWidget->update();
}


