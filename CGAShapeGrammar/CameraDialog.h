#ifndef CAMERADIALOG_H
#define CAMERADIALOG_H

#include <QDialog>
#include "ui_CameraDialog.h"

class CameraDialog : public QDialog {
	Q_OBJECT

public:
	Ui::CameraDialog ui;

public:
	CameraDialog(QWidget *parent = 0);
	~CameraDialog();

public slots:
	void onOK();
	void onCancel();

};

#endif // CAMERADIALOG_H
