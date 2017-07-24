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
	//AVTCamera *list;
	//int count= detectAVTCameras(cameraList);
	cameraList = detectAVTCameras();
	//TODO Complete!
	//1. detect canons, detect nikons, then allocate the cameraList matching the count of objects.
	// So far just fillsN the list with the avt!

	//cameraList = (AVTCamera*)calloc(count, sizeof(AVTCamera));//
	//cameraList = (ActiveCamera*)calloc(count, sizeof(AVTCamera));//
	

	//
	//cameraList.reserve(count);
	/*
	for (int i = 0;i < count;i++)//GOOD PRACTICE CHECK... there must be a better way to copy this, or not?
	//int i = 0;
	//for (vector<ActiveCamera*>::iterator it = cameraList.begin() ; it != cameraList.end(); ++it)
	
	{
		//*cameraList[i] = list[i];//TODO Dying here :'(
		cameraList.push_back(&list[i]);
		//*camli[i] = list[i];
		//*it = &list[i];
		//i++;

	}
	*/	 
	
	//TODO Remove this test:
	string as = cameraList[0].getAnyStr();
	
	numCams = sizeof(cameraList);//review here (likely not the right size)
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
//ActiveCamera* AcquisitionDeviceManager::getCamera(int index)
AVTCamera AcquisitionDeviceManager::getCamera(int index)
{
	if (numCams > index)
	{
		return cameraList[index];
	}
	throw "NO CAMERA";
}

Projector* AcquisitionDeviceManager::getProjectors()
{
	throw "nop";
}
Projector* AcquisitionDeviceManager::getProjector(string dev_id)
{
	return NULL;
}


AVTCamera* AcquisitionDeviceManager::detectAVTCameras()
{
	AVTCamera* avtList;
	int count = 0;
	if (!vimbaError)
	{
		sistema->GetCameras(cameras);
		count = cameras.size();
		qDebug("Num AVT cameras: %d\n", count);

		avtList = new AVTCamera[cameras.size()];
		
		CameraPtr sh;
		if (count > 0)
		{
			testValidCameraPtr(cameras[0]);
			for (int i = 0;i < count ; i++)
			{
				avtList[i].setCameraPtr(cameras[i]);
				avtList[i].setName("myname");
			}
			avtList[0].getName();
			testValidCameraPtr(avtList[0].getAVTPointer());
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
void AcquisitionDeviceManager::loadXmlSettingsToFirstAVTCamera()
{
	//TODO Obviously re-write properly!. This is just an ultra-short-term solution.

}