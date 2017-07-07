#pragma once
#include "VimbaCPP/Include/VimbaCPP.h"
#include "Camera.h"
#include "Projector.h"
class AcquisitionDeviceManager
{
	/*
	Initialize and handle APIs for devices and their events.
	*/
public:
	AcquisitionDeviceManager();
	~AcquisitionDeviceManager();
	bool initializeAPIs();
	void endAPIs();
	void detectCameras();
	void detectProjectors();
	
	ActiveCamera* getCameras();
	ActiveCamera* getCamera(std::string dev_id);
	Projector* getProjector(std::string dev_id);
	Projector* getProjectors();

private:
	AVT::VmbAPI::VimbaSystem* sistema;
	VmbErrorType    err;
	ActiveCamera* cameraList;
};

