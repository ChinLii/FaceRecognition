/********************************************************************************
** Form generated from reading UI file 'facecaptures.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACECAPTURES_H
#define UI_FACECAPTURES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceCapturesClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FaceCapturesClass)
    {
        if (FaceCapturesClass->objectName().isEmpty())
            FaceCapturesClass->setObjectName(QStringLiteral("FaceCapturesClass"));
        FaceCapturesClass->resize(600, 400);
        menuBar = new QMenuBar(FaceCapturesClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FaceCapturesClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FaceCapturesClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FaceCapturesClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FaceCapturesClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FaceCapturesClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FaceCapturesClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FaceCapturesClass->setStatusBar(statusBar);

        retranslateUi(FaceCapturesClass);

        QMetaObject::connectSlotsByName(FaceCapturesClass);
    } // setupUi

    void retranslateUi(QMainWindow *FaceCapturesClass)
    {
        FaceCapturesClass->setWindowTitle(QApplication::translate("FaceCapturesClass", "FaceCaptures", 0));
    } // retranslateUi

};

namespace Ui {
    class FaceCapturesClass: public Ui_FaceCapturesClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACECAPTURES_H
