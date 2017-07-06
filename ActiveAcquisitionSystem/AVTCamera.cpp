#include "AVTCamera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
using namespace AVT::VmbAPI;

AVTCamera::AVTCamera()
{
//	*sys = VimbaSystem::GetInstance();  // Get a reference to the VimbaSystem singleton
	//VmbErrorType    err = *sys.Startup();               // Initialize the Vimba API
}


AVTCamera::~AVTCamera()
{
}

bool AVTCamera::loadSettings()
{
	


	return true;
}

bool AVTCamera::detectCameras(AVTCamera* avtList)
{
	return true;
}