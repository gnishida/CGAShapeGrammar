#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
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
	connect(ui.actionOpenCGARules, SIGNAL(triggered()), this, SLOT(onOpenCGARules()));
	connect(ui.actionViewShadow, SIGNAL(triggered()), this, SLOT(onViewShadow()));
	connect(ui.actionViewRegularRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewWireframe, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewLineRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
	connect(ui.actionViewSketchyRendering, SIGNAL(triggered()), this, SLOT(onViewRendering()));
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

void MainWindow::onGenerateImages() {
	glWidget->generateImages();
}

void MainWindow::onHoge() {
	glWidget->hoge();
}