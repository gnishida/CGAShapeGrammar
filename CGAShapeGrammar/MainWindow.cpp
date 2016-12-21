#include "MainWindow.h"
#include <QFileDialog>
#include "OBJWriter.h"
#include "CameraDialog.h"

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
	connect(ui.actionOpenCGA, SIGNAL(triggered()), this, SLOT(onOpenCGA()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewBasicRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSSAO, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewContourRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewHatching, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewRefresh, SIGNAL(triggered()), this, SLOT(onViewRefresh()));
	connect(ui.actionRotationStart, SIGNAL(triggered()), this, SLOT(onRotationStart()));
	connect(ui.actionRotationEnd, SIGNAL(triggered()), this, SLOT(onRotationEnd()));
	connect(ui.actionCamera, SIGNAL(triggered()), this, SLOT(onCamera()));

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

void MainWindow::onViewRefresh() {
	if (fileLoaded) {
		glWidget->loadCGA(filename.toUtf8().data());
	}
}

void MainWindow::onRotationStart() {
	glWidget->rotationStart();
}

void MainWindow::onRotationEnd() {
	glWidget->rotationEnd();
}

void MainWindow::camera_update() {
	glWidget->camera.yrot += 0.02;
	glWidget->camera.updateMVPMatrix();
	glWidget->update();
}

void MainWindow::onCamera() {
	CameraDialog dlg;

	dlg.ui.lineEditXRot->setText(QString::number(glWidget->camera.xrot));
	dlg.ui.lineEditYRot->setText(QString::number(glWidget->camera.yrot));
	dlg.ui.lineEditZRot->setText(QString::number(glWidget->camera.zrot));
	dlg.ui.lineEditFOV->setText(QString::number(glWidget->camera.fovy));
	dlg.ui.lineEditOx->setText(QString::number(glWidget->camera.center.x));
	dlg.ui.lineEditOy->setText(QString::number(glWidget->camera.center.y));
	dlg.ui.lineEditX->setText(QString::number(glWidget->camera.pos.x));
	dlg.ui.lineEditY->setText(QString::number(glWidget->camera.pos.y));
	dlg.ui.lineEditZ->setText(QString::number(glWidget->camera.pos.z));
	if (dlg.exec()) {
		glWidget->camera.xrot = dlg.ui.lineEditXRot->text().toFloat();
		glWidget->camera.yrot = dlg.ui.lineEditYRot->text().toFloat();
		glWidget->camera.zrot = dlg.ui.lineEditZRot->text().toFloat();
		glWidget->camera.fovy = dlg.ui.lineEditFOV->text().toFloat();
		glWidget->camera.center.x = dlg.ui.lineEditOx->text().toFloat();
		glWidget->camera.center.y = dlg.ui.lineEditOy->text().toFloat();
		glWidget->camera.pos.x = dlg.ui.lineEditX->text().toFloat();
		glWidget->camera.pos.y = dlg.ui.lineEditY->text().toFloat();
		glWidget->camera.pos.z = dlg.ui.lineEditZ->text().toFloat();

		glWidget->camera.updatePMatrix(glWidget->width(), glWidget->height());
		glWidget->update();
	}
}