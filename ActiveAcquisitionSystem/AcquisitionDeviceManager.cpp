#include "AcquisitionDeviceManager.h"
#include <qdebug.h>
#include <vector>
#include "debugMacros.h"

using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{
	
	initializeAPIs();
}
AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
	//release memory, close api's objects
	try {
		for(ActiveCamera* c: cameraList)
		{
			delete c;
		}
	}
	catch (...)
	{
		LOGERR("oh! not closing camera");
	}
	endAPIs();
}


bool AcquisitionDeviceManager::initializeAPIs()
{
	//vimba
	sistema = &AVT::VmbAPI::VimbaSystem::GetInstance();
	err = sistema->Startup();
	
	//texas dlp


	//canon
	//edsWrapper = new EDSWrapper();

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
	{ 
		printf("oh! not shutting down?");
	}

}

void AcquisitionDeviceManager::detectProjectors()
{
	//TODO Implement: detect # of screens (each is a projector) + DLPs connected (inf-1)
}
std::vector<ActiveCamera*> AcquisitionDeviceManager::getCameras()
{
	return cameraList;
}
ActiveCamera* AcquisitionDeviceManager::getCamera(string dev_id)
{
	return NULL;
}
ActiveCamera* AcquisitionDeviceManager::getCamera(int index)
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

vector<ActiveCamera*> AcquisitionDeviceManager::detectAllCameras()
{
	//review: is this the most efficient?	
	for each(AVTCamera* c in detectAVTCameras())
	{
		cameraList.push_back(c);
	}
	//TODO ADD canon cameras. LOOK HERE!!
	/*
	
	int canonsCount = edsWrapper->getCameraCount();
	
	for (int i = 0;i < canonsCount;i++)
	{
		//CanonCamera* canon=new CanonCamera(edsWrapper->getCamera(i));
		CanonCamera canon;
		canon.setEdsCameraRef(edsWrapper->getCamera(i));
		cameraList.push_back(&canon);
	}
	*/

	numCams = cameraList.size();
	return cameraList;
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
		avtList.resize(cameras.size());
		
		CameraPtr sh;
		if (count > 0)
		{
#ifdef AVT_DEBUG
			testValidCameraPtr(cameras[0]);
#endif // AVT_DEBUG
		for (int i = 0;i < count ; i++)
			{
				avtList[i] = new AVTCamera(cameras[i]);
			}
		}		
	}
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