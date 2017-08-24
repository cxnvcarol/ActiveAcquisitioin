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
	printf("Load settings with CameraSettings object");
	return false;
}


bool ActiveCamera::loadSettings(std::string pathToSettings)
{
	printf("Load settings with file not implemented");
	return false;
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

bool ActiveCamera::notifyStopProjectionSequence() {
	if (playingProjectionSequence)
	{
		playingProjectionSequence = false;
		indexPicture = 0;
		return true;
	}
	else return false;//if already stopped
};


bool ActiveCamera::notifyStartProjectionSequence()//Review: verify workflow. do I need the reference to the projector?
{
	if (playingProjectionSequence)
	{
		LOGEXEC("projectionSequence already playing");
		return false;
	}
	playingProjectionSequence = true;
	indexPicture = 0;
	return true;
}