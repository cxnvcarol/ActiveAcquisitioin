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
}

void AcquisitionDeviceManager::detectCameras()
{
	AVTCamera* list;
	detectAVTCameras(list);

}

void AcquisitionDeviceManager::detectProjectors()
{

}
ActiveCamera* AcquisitionDeviceManager::getCameras()
{
	throw "nop";
}
ActiveCamera* AcquisitionDeviceManager::getCamera(string dev_id)
{
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


void AcquisitionDeviceManager::detectAVTCameras(AVTCamera* avtList)
{
	//TODO 
	if (!vimbaError)
	{
		CameraPtrVector cameras;

		sistema->GetCameras(cameras);
		qDebug("Num AVT cameras: %d\n", cameras.size());
	}

}