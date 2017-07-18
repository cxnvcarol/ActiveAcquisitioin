#include "AcquisitionDeviceManager.h"
#include <qdebug.h>

using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{
	initializeAPIs();
}


AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
}

bool AcquisitionDeviceManager::initializeAPIs()
{
	//vimba
	sistema = &AVT::VmbAPI::VimbaSystem::GetInstance();
	err = sistema->Startup();

	
	//texas dlp


	//canon


	return true;

}
void AcquisitionDeviceManager::endAPIs()
{
	//to be called at the end of the session
	//return true;
	try {
		sistema->Shutdown();
	}
	catch(exception e)
	{ }


}

void AcquisitionDeviceManager::detectCameras()
{
	AVTCamera *list;
	int count= detectAVTCameras(list);
	//TODO Complete!
	//1. detect canons, detect nikons, then allocate the cameraList matching the count of objects.
	// So far just fillsN the list with the avt!

	//cameraList = (ActiveCamera*)calloc(count, sizeof(ActiveCamera));//
	cameraList = new ActiveCamera[count];
	
	for (int i = 0;i < count;i++)//GOOD PRACTICE CHECK... Must be a better way to copy this, or not?
	{
		cameraList[i] = list[i];
	}
	 
	numCams = count;

}

void AcquisitionDeviceManager::detectProjectors()
{

}
ActiveCamera* AcquisitionDeviceManager::getCameras()
{
	throw "nop";
}
ActiveCamera AcquisitionDeviceManager::getCamera(string dev_id)
{
	return NULL;
}
ActiveCamera AcquisitionDeviceManager::getCamera(int index)
{
	if (numCams > index)
		return cameraList[index];
	return NULL;
}

Projector* AcquisitionDeviceManager::getProjectors()
{
	throw "nop";
}
Projector* AcquisitionDeviceManager::getProjector(string dev_id)
{
	return NULL;
}


int AcquisitionDeviceManager::detectAVTCameras(AVTCamera* avtList)
{
	//AVTCamera* avtList;
	//TODO 
	int count = 0;
	if (!vimbaError)
	{
		CameraPtrVector cameras;

		sistema->GetCameras(cameras);
		count = cameras.size();
		qDebug("Num AVT cameras: %d\n", count);

		//avtList = new AVTCamera[cameras.size()];
		avtList = (AVTCamera*)calloc(count, sizeof(AVTCamera));

		for (int i = 0;i < count;i++)
		{
			avtList[i]= AVTCamera(cameras[i]);
		}		
	}
	return count;
}

void AcquisitionDeviceManager::loadXmlSettingsToFirstAVTCamera()
{
	//TODO Obviously re-write properly!. This is just an ultra-short-term solution.

}