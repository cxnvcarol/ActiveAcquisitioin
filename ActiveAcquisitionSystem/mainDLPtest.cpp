#include <iostream>

#include "AcquisitionDeviceManager.h"


using namespace std;

void mainDLPTest()
{
	//workflow:
	/*
	1. detect # of dlps & print important info
	2. load and display slow sequence with dlp-format file
	3. stop
	*/

	printf("Test DLP Devices\n\n");
	AcquisitionDeviceManager manager;

	int count=manager.detectDLPs();

	printf("# of dlps: %d\n\nEnter to continue", count);
	cin.get();
	std::vector<DLPProjector*> ds=manager.getDLPs();
	for (DLPProjector* d : ds)
	{
		d->loadProjectionsFolder("dlptest");
		d->loadProjectionSettings("dlptest\\tankTest.txt");
	}
	for (DLPProjector* d : ds)
	{
		d->playProjectionSequence(1);
	}
	printf("\napparently sequence started\nEnter to close");
	cin.get();
	

}