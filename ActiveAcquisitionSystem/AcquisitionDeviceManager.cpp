#include "AcquisitionDeviceManager.h"
#include <qdebug.h>
#include <vector>
#include "debugMacros.h"

#include "Texas/HiresLib/usb.h"
#include "Texas/HiresLib/API.h"
#include "DLPProjector.h"


using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{	
	initializeAPIs();
}
AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
	//release dlp related resources:
	for (DLPProjector* dlp : dlps)
	{
		USB_Close(dlp->getHidDevice());
	}
	USB_Exit();

	//avt release memory, close api's objects
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
int AcquisitionDeviceManager::detectDLPs()
{
	if (dlps.size()==0)
	{
		struct hid_device_info* devs= hid_enumerate(MY_VID, MY_PID);
		struct hid_device_info* cur_dev = devs;
		while (cur_dev) {
			printf("DLP Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
				cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
			printf("\n");
			printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
			printf("  Product:      %ls\n", cur_dev->product_string);
			printf("\n");
			//hid_device *hid=hid_open(cur_dev->vendor_id, cur_dev->product_id, cur_dev->serial_number);
			hid_device *hid = hid_open_path(cur_dev->path);
			if (hid != NULL)
			{
				DLPProjector *dlp = new DLPProjector(hid, cur_dev->path);
				if (setDLPStatus(dlp))
				{
					LOGEXEC("dlp status was ok");
					dlps.push_back(dlp);

				}
				else delete dlp;
			}
			cur_dev = cur_dev->next;
			//TODO Review, is that enough??
		}
		hid_free_enumeration(devs);		
	}
	return dlps.size();
}

void AcquisitionDeviceManager::addStandardProjector(StandardProjector * p)
{
	standardProjectors.push_back(p);
}

bool AcquisitionDeviceManager::setDLPStatus(DLPProjector *dlp)
{
	uchar HWStatus, SysStatus, MainStatus;
	BootLoaderStaus BLStatus;
	int statusLCR = LCR_GetStatus(&HWStatus, &SysStatus, &MainStatus,dlp->getHidDevice());
	if (statusLCR == 0)
	{
		dlp->setStatus(statusLCR, HWStatus, SysStatus, MainStatus);
		return true;
	}
	else if (LCR_GetBLStatus(&BLStatus, dlp->getHidDevice()) == 0)
	{
		//This means the device is in boot mode
		//Review... do something else?
	}
	else
	{
		USB_Close(dlp->getHidDevice());
	}
	return false;
}


bool AcquisitionDeviceManager::initializeAPIs()
{
	//vimba
	vimbaSys = &AVT::VmbAPI::VimbaSystem::GetInstance();
	err = vimbaSys->Startup();

	//canon
	edsWrapper = new EDSWrapper();

	//texas dlp
	USB_Init();

	return true;

}
void AcquisitionDeviceManager::endAPIs()
{
	//to be called at the end of the session
	//return true;
	try {
		vimbaSys->Shutdown();
		//review End edsdk??
	}
	catch(exception e)
	{ 
		printf("oh! not shutting down?");
	}

}

void AcquisitionDeviceManager::detectProjectors()
{
	throw "not implemented. Most likely deprecated: use other methods";
	//TODO Implement: detect # of screens (each is a projector) + DLPs connected (inf-1). Remove?
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

Projector* AcquisitionDeviceManager::getProjector(string dev_id)
{
	return NULL;
}

std::vector<StandardProjector*> AcquisitionDeviceManager::getStandardProjectors()
{
	return standardProjectors;
}

vector<ActiveCamera*> AcquisitionDeviceManager::detectAllCameras()
{
	//review: is this the most efficient?	
	for(AVTCamera* c : detectAVTCameras())
	{
		cameraList.push_back(c);
	}	
	
	for (CanonCamera* c : detectCanonCameras())
	{
		cameraList.push_back(c);
	}
	numCams = cameraList.size();
	return cameraList;
}

std::vector<ActiveCamera*> AcquisitionDeviceManager::detectAllCameras(std::vector<string> onlyInclude)
{
	if (onlyInclude.empty())
	{
		return detectAllCameras();
	}
	else {
		//review: is this the most efficient?	
		for (AVTCamera* c : detectAVTCameras(onlyInclude))
		{
			cameraList.push_back(c);
		}

		for (CanonCamera* c : detectCanonCameras())
		{
			cameraList.push_back(c);
		}
		numCams = cameraList.size();
		return cameraList;
	}
}

vector<AVTCamera*> AcquisitionDeviceManager::detectAVTCameras(std::vector<string> onlyInclude)
{
	if (onlyInclude.empty())
	{
		return detectAVTCameras();
	}
	AVT::VmbAPI::CameraPtrVector cameras;
	vector<AVTCamera*> avtList;
	int count = 0;
	if (!vimbaError)
	{
		vimbaSys->GetCameras(cameras);
		count = cameras.size();
		LOGEXEC("Num total AVT cameras: %d\n", count);

		CameraPtr sh;
		if (count > 0)
		{
#ifdef AVT_DEBUG
			testValidCameraPtr(cameras[0]);
#endif // AVT_DEBUG
			for (int i = 0;i < count; i++)
			{
				//TODO Review it works fine.
				AVTCamera* avt = new AVTCamera(cameras[i]);
				string camID=avt->getDevId();
				//if (onlyInclude.find)
				auto it = find_if(onlyInclude.begin(), onlyInclude.end(), [&camID](string obj) {return obj == camID;});

				if (it != onlyInclude.end())//i.e. found
				{
					avt->getDevId();
					avtList.push_back(avt);
				}
				else {
					delete avt;
				}

				
			}
		}
	}
	return avtList;
}

vector<AVTCamera*> AcquisitionDeviceManager::detectAVTCameras()
{
	AVT::VmbAPI::CameraPtrVector cameras;
	vector<AVTCamera*> avtList;
	int count = 0;
	if (!vimbaError)
	{
		vimbaSys->GetCameras(cameras);
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

std::vector<CanonCamera*> AcquisitionDeviceManager::detectCanonCameras()
{
	std::vector<CanonCamera*> canons;
	int canonsCount = edsWrapper->getCameraCount();

	for (int i = 0;i < canonsCount;i++)
	{
		CanonCamera canon;
		canon.setEdsCameraRef(edsWrapper->getCamera(i));
		canons.push_back(&canon);
	}
	return canons;
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