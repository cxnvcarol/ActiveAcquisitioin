#include <iostream>
#include <stdlib.h>     /* atexit */

#include "AcquisitionDeviceManager.h"
#include "ParameterParser.h"
#include "StandardProjector.h"


using namespace std;
/*
QApplication* a;
ParameterParser parser;
AcquisitionDeviceManager *mng;

bool mngDeleted=false;

void atExitFn()
{
	if (!mngDeleted)
	{
		mngDeleted = true;
		printf("deleting devManager\n\n");
		delete mng;
	}
}
*/
void atExitFn();//external fwd declaration

int mainStdProjector(int argc, char *argv[])
{
	QApplication* a;
	ParameterParser parser;
	AcquisitionDeviceManager *mng;

	bool mngDeleted = false;
	

	atexit(atExitFn);
	a= new QApplication(argc, argv);
	//2 standard projectors configured with parameter parser!
	//when finished projections pause for enter in command line.

	
	parser.parseParameters(argc, argv);
	
	mng = new AcquisitionDeviceManager();


	for (int i = 0;i < parser.countProjectors;i++)
	{
		StandardProjector* sp = new StandardProjector();
		sp->setScreenIndex(atoi(parser.refToProjector[i].c_str()));
		sp->loadProjectionsFolder(parser.projectionsFolder[i].c_str());
		sp->loadProjectionSettings(parser.projectionsConfig[i].c_str());
		mng->addStandardProjector(sp);

		sp->showInFullProjection();
	}
	for (StandardProjector *p: mng->getStandardProjectors())
	{
		p->playProjectionSequence(1);
	}

	printf("\n\njust before executing!\n\n");
	int result = a->exec();

	//atExitFn();
	printf("\n\n\nenter to finish!\n\n");	
	fflush(stdout);
	cin.get();
	
	return result;
}