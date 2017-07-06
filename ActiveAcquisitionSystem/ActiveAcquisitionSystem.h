#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ActiveAcquisitionSystem.h"

class ActiveAcquisitionSystem : public QMainWindow
{
	Q_OBJECT

public:
	ActiveAcquisitionSystem(QWidget *parent = Q_NULLPTR);

private:
	Ui::ActiveAcquisitionSystemClass ui;
};
