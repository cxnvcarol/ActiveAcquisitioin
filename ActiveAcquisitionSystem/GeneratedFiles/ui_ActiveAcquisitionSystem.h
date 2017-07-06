/********************************************************************************
** Form generated from reading UI file 'ActiveAcquisitionSystem.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTIVEACQUISITIONSYSTEM_H
#define UI_ACTIVEACQUISITIONSYSTEM_H

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

class Ui_ActiveAcquisitionSystemClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ActiveAcquisitionSystemClass)
    {
        if (ActiveAcquisitionSystemClass->objectName().isEmpty())
            ActiveAcquisitionSystemClass->setObjectName(QStringLiteral("ActiveAcquisitionSystemClass"));
        ActiveAcquisitionSystemClass->resize(600, 400);
        menuBar = new QMenuBar(ActiveAcquisitionSystemClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ActiveAcquisitionSystemClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ActiveAcquisitionSystemClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ActiveAcquisitionSystemClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ActiveAcquisitionSystemClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ActiveAcquisitionSystemClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ActiveAcquisitionSystemClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ActiveAcquisitionSystemClass->setStatusBar(statusBar);

        retranslateUi(ActiveAcquisitionSystemClass);

        QMetaObject::connectSlotsByName(ActiveAcquisitionSystemClass);
    } // setupUi

    void retranslateUi(QMainWindow *ActiveAcquisitionSystemClass)
    {
        ActiveAcquisitionSystemClass->setWindowTitle(QApplication::translate("ActiveAcquisitionSystemClass", "ActiveAcquisitionSystem", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ActiveAcquisitionSystemClass: public Ui_ActiveAcquisitionSystemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTIVEACQUISITIONSYSTEM_H
