

/* Workflow:
1. Initialize APIs
2. Recognize and list devices (cameras and projectors)
3. Open GUI elements and load identified list.
4. Set listeners:
a. when new camera appears
b. when new projector appears
c. when buttons: load projections-sequence, load caera-settings for selected camera(s?), change mode
...The modes.. camera-live-play mode (x testing), only projection mode(x testing), sync-mode (actual active acquisition)



*/

#include "ActiveAcquisitionSystem.h"
#include <QtWidgets/QApplication>
//
#include "WorkingSession.h"
#include "EDSWrapper.h"

#include "StandardProjector.h"

//#include "AcquisitionDeviceManager.h"
#include <iostream>

#include "CanonCamera.h"

using namespace std;



void main()
{
	WorkingSession ws;
	EDSWrapper eds;

	int cc = eds.getCameraCount();

	qDebug("Canons: %d", cc);
	fflush(stdout);
	//cin.get();
	if (cc > 0)
	{
		EdsCameraRef *c = eds.getCamera(0);
		
		eds.sampleRun(c);		
		printf("\nopensession:%d\n\n", EdsOpenSession(*c));
		
		int pd=0;
		EdsError e = EdsGetPropertyData(*c, kEdsPropID_SaveTo, 0, sizeof(pd), &pd);
		printf("\ngetSaveToResult: %d with error %d\n\n", pd, e);

		int p = kEdsSaveTo_Both;
		e = EdsSetPropertyData(*c, kEdsPropID_SaveTo, 0, sizeof(p), &p);
		printf("\nsetSaveToResult: with error %d\n\n", e);

		e = EdsGetPropertyData(*c, kEdsPropID_SaveTo, 0, sizeof(pd), &pd);
		printf("\ngetSaveToResult: %d with error %d\n\n", pd, e);

		EdsUInt32 tv;
		e = EdsGetPropertyData(*c, kEdsPropID_Tv, 0, sizeof(tv), &tv);
		printf("\ngetTV: %d with error %ld\n\n", tv, e);

		printf("\nclosesession:%d\n\n", EdsCloseSession(*c));
		//kEdsPropID_SaveTo///TODO, use this?
		/*
		CanonCamera* canon = new CanonCamera(c);
		//gral wf: identify, load settings, set outputfolder, takesinglepicture, (callback: savepicture)
		canon->setOutputFolder("C:\\Users\\naranjo\\Pictures\\canonTest");//TODO Test with fixed path, remove later.
		//canon->takeSinglePicture();

		*/
		
		while (1)
		{
			/*
			printf("enter to open");
			cin.get();
			printf("\nopensession:%d\n\n", EdsOpenSession(*c));
			for (int i : {1, 2, 3, 4, 5})
			{
				printf("enter to take");
				cin.get();
				eds.takeSinglePicture(*c);
				printf("enter to continue");
				cin.get();
			}
			printf("\nclosesession:%d\n\n", EdsCloseSession(*c));
			*/
			printf("\nopensession:%d\n\n", EdsOpenSession(*c));
			for (int i : {1, 2, 3, 4, 5})
			{
				printf("enter to take %d\n",i);
				std::cin.get();
				
				eds.takeSinglePicture(*c);
				//printf("enter to close");
				//std::cin.get();
				//TODO?? USE EdsGetPropertyData to determine the sleep time before closing??
				
			}
			std::cin.get();
			printf("\nclosesession:%d\n\n", EdsCloseSession(*c));
			
			printf("\nopensession:%d\n\n", EdsOpenSession(*c));//it triggers the events to save pictures
			
			printf("enter to close");
			std::cin.get();
			printf("\nclosesession:%d\n\n", EdsCloseSession(*c));
		}
		
	}
	printf("Enter to leave\n");
	std::cin.get();
	
	
}