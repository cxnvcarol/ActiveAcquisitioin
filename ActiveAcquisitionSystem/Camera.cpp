#include "Camera.h"
#include <string>
using namespace std;
#include <QDir>

ActiveCamera::ActiveCamera(string dev_id)
{
	
}

ActiveCamera::ActiveCamera()
{

}
ActiveCamera::~ActiveCamera()
{
}
int ActiveCamera::takeSinglePicture()
{
	throw "Not implemented";
}

bool ActiveCamera::loadSettings(CameraSettings cs)//GRAL CASE (makes more sense for canon & nikon)
{
	throw "Not implemented";
}


bool ActiveCamera::loadSettings(std::string pathToSettings)
{
	throw "Not implemented";
}

std::string ActiveCamera::getDevId()
{
	return dev_id;
}

void ActiveCamera::setOutputFolder(std::string folder)
{
	outputFolder = folder;
	if (!QDir(folder.c_str()).exists())
		QDir().mkdir(folder.c_str());
}

void ActiveCamera::notifyStopProjectionSequence() {
	//TODO Evaluate need of this fn! (inf) --> needed when integrated with GUI to handle multiple capture-runs into the user-session
	playingProjectionSequence = false;
	indexPicture = 0;
};


void ActiveCamera::notifyStartProjectionSequence()//TODO Review: verify workflow. do I need the reference to the projector
{
	if (playingProjectionSequence)
	{
		LOGEXEC("projectionSequence already playing");
	}
	playingProjectionSequence = true;
	indexPicture = 0;
}