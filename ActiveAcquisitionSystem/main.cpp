#include "ActiveAcquisitionSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ActiveAcquisitionSystem w;
	w.show();
	return a.exec();
}
