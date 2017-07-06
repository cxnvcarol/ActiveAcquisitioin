#include "AcquisitionDeviceManager.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;
using namespace std;

AcquisitionDeviceManager::AcquisitionDeviceManager()
{
	VimbaSystem &sistema=VimbaSystem::GetInstance();
	VmbErrorType    err;
}


AcquisitionDeviceManager::~AcquisitionDeviceManager()
{
}
