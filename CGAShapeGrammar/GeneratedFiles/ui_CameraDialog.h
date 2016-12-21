/********************************************************************************
** Form generated from reading UI file 'CameraDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERADIALOG_H
#define UI_CAMERADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CameraDialog
{
public:
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;
    QLabel *label;
    QLineEdit *lineEditXRot;
    QLabel *label_2;
    QLineEdit *lineEditYRot;
    QLabel *label_3;
    QLineEdit *lineEditZRot;
    QLabel *label_4;
    QLineEdit *lineEditFOV;
    QLabel *label_5;
    QLineEdit *lineEditOx;
    QLabel *label_6;
    QLineEdit *lineEditOy;
    QLabel *label_7;
    QLineEdit *lineEditX;
    QLabel *label_8;
    QLineEdit *lineEditY;
    QLabel *label_9;
    QLineEdit *lineEditZ;

    void setupUi(QDialog *CameraDialog)
    {
        if (CameraDialog->objectName().isEmpty())
            CameraDialog->setObjectName(QStringLiteral("CameraDialog"));
        CameraDialog->resize(217, 343);
        pushButtonOK = new QPushButton(CameraDialog);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setGeometry(QRect(10, 300, 91, 31));
        pushButtonCancel = new QPushButton(CameraDialog);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(110, 300, 91, 31));
        label = new QLabel(CameraDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 20, 47, 21));
        lineEditXRot = new QLineEdit(CameraDialog);
        lineEditXRot->setObjectName(QStringLiteral("lineEditXRot"));
        lineEditXRot->setGeometry(QRect(100, 20, 91, 20));
        label_2 = new QLabel(CameraDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 50, 47, 21));
        lineEditYRot = new QLineEdit(CameraDialog);
        lineEditYRot->setObjectName(QStringLiteral("lineEditYRot"));
        lineEditYRot->setGeometry(QRect(100, 50, 91, 20));
        label_3 = new QLabel(CameraDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 80, 47, 21));
        lineEditZRot = new QLineEdit(CameraDialog);
        lineEditZRot->setObjectName(QStringLiteral("lineEditZRot"));
        lineEditZRot->setGeometry(QRect(100, 80, 91, 20));
        label_4 = new QLabel(CameraDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 110, 47, 21));
        lineEditFOV = new QLineEdit(CameraDialog);
        lineEditFOV->setObjectName(QStringLiteral("lineEditFOV"));
        lineEditFOV->setGeometry(QRect(100, 110, 91, 20));
        label_5 = new QLabel(CameraDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 140, 47, 21));
        lineEditOx = new QLineEdit(CameraDialog);
        lineEditOx->setObjectName(QStringLiteral("lineEditOx"));
        lineEditOx->setGeometry(QRect(100, 140, 91, 20));
        label_6 = new QLabel(CameraDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 170, 47, 21));
        lineEditOy = new QLineEdit(CameraDialog);
        lineEditOy->setObjectName(QStringLiteral("lineEditOy"));
        lineEditOy->setGeometry(QRect(100, 170, 91, 20));
        label_7 = new QLabel(CameraDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(40, 200, 47, 21));
        lineEditX = new QLineEdit(CameraDialog);
        lineEditX->setObjectName(QStringLiteral("lineEditX"));
        lineEditX->setGeometry(QRect(100, 200, 91, 20));
        label_8 = new QLabel(CameraDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(40, 230, 47, 21));
        lineEditY = new QLineEdit(CameraDialog);
        lineEditY->setObjectName(QStringLiteral("lineEditY"));
        lineEditY->setGeometry(QRect(100, 230, 91, 20));
        label_9 = new QLabel(CameraDialog);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(40, 260, 47, 21));
        lineEditZ = new QLineEdit(CameraDialog);
        lineEditZ->setObjectName(QStringLiteral("lineEditZ"));
        lineEditZ->setGeometry(QRect(100, 260, 91, 20));

        retranslateUi(CameraDialog);

        QMetaObject::connectSlotsByName(CameraDialog);
    } // setupUi

    void retranslateUi(QDialog *CameraDialog)
    {
        CameraDialog->setWindowTitle(QApplication::translate("CameraDialog", "CameraDialog", 0));
        pushButtonOK->setText(QApplication::translate("CameraDialog", "OK", 0));
        pushButtonCancel->setText(QApplication::translate("CameraDialog", "Cancel", 0));
        label->setText(QApplication::translate("CameraDialog", "X rot:", 0));
        label_2->setText(QApplication::translate("CameraDialog", "Y rot:", 0));
        label_3->setText(QApplication::translate("CameraDialog", "Z rot:", 0));
        label_4->setText(QApplication::translate("CameraDialog", "FOV:", 0));
        label_5->setText(QApplication::translate("CameraDialog", "Ox:", 0));
        label_6->setText(QApplication::translate("CameraDialog", "Oy:", 0));
        label_7->setText(QApplication::translate("CameraDialog", "X:", 0));
        label_8->setText(QApplication::translate("CameraDialog", "Y:", 0));
        label_9->setText(QApplication::translate("CameraDialog", "Z:", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraDialog: public Ui_CameraDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERADIALOG_H
