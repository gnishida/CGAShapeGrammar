#include "CameraDialog.h"

CameraDialog::CameraDialog(QWidget *parent) : QDialog(parent) {
	ui.setupUi(this);

	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

CameraDialog::~CameraDialog() {
}

void CameraDialog::onOK() {
	accept();
}

void CameraDialog::onCancel() {
	reject();
}
