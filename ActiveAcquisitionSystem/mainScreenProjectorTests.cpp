#include <iostream>
#include <stdlib.h>     /* atexit */

#include "AcquisitionDeviceManager.h"
#include "ParameterParser.h"
#include "StandardProjector.h"


using namespace std;
QApplication* a;
ParameterParser parser;
AcquisitionDeviceManager *mng;

void atExitFn()
{
	if (mng)
	{
		printf("deleting devManager\n\n");
		delete mng;
	}
}

int main(int argc, char *argv[])
{
	atexit(atExitFn);
	a= new QApplication(argc, argv);
	//2 standard projectors configured with parameter parser!
	//when finished projections pause for enter in command line.

	
	parser.parseParameters(argc, argv);
	
	mng = new AcquisitionDeviceManager();


	for (int i = 0;i < parser.countProjectors;i++)
	{
		StandardProjector* sp = new StandardProjector();
		sp->setScreenIndex(parser.projectedScreen[i]);
		sp->loadProjectionsFolder(parser.projectionsFolder[i].c_str());
		sp->loadProjectionSettings(parser.projectionsConfig[i].c_str());
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
	int result = a->exec();

	//atExitFn();
	printf("\n\n\nenter to finish!\n\n");	
	fflush(stdout);
	cin.get();
	
	return result;

}