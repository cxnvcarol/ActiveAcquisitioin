

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
#include "CanonCamera.h"
#include "WorkingSession.h"
#include "EDSWrapper.h"

#include "StandardProjector.h"

#include "AcquisitionDeviceManager.h"

using namespace std;


int testIdentifyCameras();
void testCanonTakingPicture();


void testCanonTakingPicture()
{
	WorkingSession ws;
	EDSWrapper eds;


	int cc = eds.getCameraCount();

	qDebug("Canons: %d", cc);
	eds.sampleRun();

	CanonCamera cac;
	cac.takeSinglePicture();
}