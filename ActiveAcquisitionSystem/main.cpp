

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
using namespace std;

void testShowFullScreen();
int testIdentifyCameras();


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ActiveAcquisitionSystem w;
	w.show();
	//testShowFullScreen();
	StandardProjector iv;//important to call in main function (or keep the reference to iv)
	iv.showInFullProjection();

	string wharever = "yea";
	return a.exec();

}


int testIdentifyCameras()
{
	//TODO
	//Goal:: identify all connected cameras and fill array of cameras
	ActiveCamera *cameraList;
	cameraList = new ActiveCamera[5];//TODO Get the number first, then fill the array.

	return sizeof(cameraList);//todo check actual returned number!
}

void testCanonTakingPicture()
{

	WorkingSession ws;
	EDSWrapper eds;


	int cc = eds.getCameraCount();

	qDebug("Canons: %d", cc);
	eds.sampleRun();

	CanonCamera cac;
	cac.takePicture();
}