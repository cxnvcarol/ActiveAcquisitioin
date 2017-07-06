#include "AcquisitionDeviceManager.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{
	sistema = &VimbaSystem::GetInstance();
	err = sistema->Startup();
}


AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
}
