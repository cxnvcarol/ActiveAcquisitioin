#include <iostream>

#include "AcquisitionDeviceManager.h"
#include "ParameterParser.h"
#include "StandardProjector.h"


using namespace std;


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//2 standard projectors configured with parameter parser!
	//when finished projections pause for enter in command line.

	ParameterParser parser;
	parser.parseParameters(argc, argv);
	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();


	for (int i = 0;i < parser.countProjectors;i++)
	{
		StandardProjector* sp = new StandardProjector();
		sp->setScreenIndex(parser.projectedScreen[i]);
		sp->loadProjectionSettings(parser.projectionsConfig[0].c_str());
		mng->addStandardProjector(sp);

		sp->showInFullProjection();
	}
	for (StandardProjector *p: mng->getStandardProjectors())
	{
		p->playProjectionSequence(1);
	}

	//TODO safe termination of program:
	//CMD CLOSE:http://www.cplusplus.com/reference/cstdlib/atexit/
	//QT Window close:https://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
	//int result = 0;
	printf("\n\njust before executing!\n\n");
	int result = a.exec();

	printf("\n\n\nenter to finish!\n\n");
	fflush(stdout);
	cin.get();
	printf("just before delete manager\n\n");
	delete mng;
	return result;

}