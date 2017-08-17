

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

#include "AcquisitionDeviceManager.h"
#include <iostream>

#include "CanonCamera.h"

using namespace std;



void main()
{
	WorkingSession ws;
	EDSWrapper eds;

	int cc = eds.getCameraCount();

	qDebug("Canons: %d", cc);
	eds.sampleRun(eds.getCamera(0));

	fflush(stdout);
	//cin.get();
	if (cc > 0)
	{
		CanonCamera* canon = new CanonCamera(eds.getCamera(0));
		//gral wf: identify, load settings, set outputfolder, takesinglepicture, (callback: savepicture)
		canon->setOutputFolder("C:\\Users\\naranjo\\Pictures\\canonTest");//TODO Test with fixed path, remove later.
		//canon->takeSinglePicture();
	}
	printf("Enter to leave\n");
	cin.get();
}