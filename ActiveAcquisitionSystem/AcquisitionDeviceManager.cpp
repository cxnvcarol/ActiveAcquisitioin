#include "AcquisitionDeviceManager.h"
#include <qdebug.h>
#include <vector>


using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{
	initializeAPIs();
}


AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
	//release memory, close api's objects
	sistema->Shutdown();
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
	cameraList = detectAVTCameras();
	//TODO Complete!
	//1. detect canons, detect nikons, then allocate the cameraList matching the count of objects.
	// So far just fillsN the list with the avt!	
	numCams = sizeof(cameraList);//review here (likely not the right size)
}

void AcquisitionDeviceManager::detectProjectors()
{
	//TODO Implement
}
std::vector<AVTCamera*> AcquisitionDeviceManager::getCameras()
{
	return cameraList;
}
ActiveCamera AcquisitionDeviceManager::getCamera(string dev_id)
{
	return NULL;
}
AVTCamera* AcquisitionDeviceManager::getCamera(int index)
{
	if (numCams > index)
	{
		return cameraList[index];
	}
	throw "NO CAMERA";
}

std::vector<Projector*> AcquisitionDeviceManager::getProjectors()
{
	return projectors;
}
Projector* AcquisitionDeviceManager::getProjector(string dev_id)
{
	return NULL;
}


vector<AVTCamera*> AcquisitionDeviceManager::detectAVTCameras()
{
	AVT::VmbAPI::CameraPtrVector cameras;
	vector<AVTCamera*> avtList;
	int count = 0;
	if (!vimbaError)
	{
		sistema->GetCameras(cameras);
		count = cameras.size();
		qDebug("Num AVT cameras: %d\n", count);
		//avtList = new AVTCamera[cameras.size()];
		avtList.resize(cameras.size());
		
		CameraPtr sh;
		if (count > 0)
		{
			testValidCameraPtr(cameras[0]);
			for (int i = 0;i < count ; i++)
			{
				avtList[i] = new AVTCamera(cameras[i]);
				avtList[i]->setName("myname");//TODO Correct this!
			}
		}		
	}

	//return count;
	return avtList;
}

bool AcquisitionDeviceManager::testValidCameraPtr(CameraPtr sh)
{
	try {
		VmbErrorType err = sh->Open(VmbAccessModeFull);
		string nm;
		sh->GetName(nm);
		printf("Valid cameraptr: %s\n",nm.c_str());
		sh->Close();
		return true;
	}
	catch(exception ex){
		printf("invalid cameraptr");
		return false;
	}
}