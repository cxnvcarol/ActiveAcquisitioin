#include <iostream>

#include "AcquisitionDeviceManager.h"


using namespace std;

void main()
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

	printf("# of dlps: %d\n\nEnter to close", count);

	cin.get();

}