#pragma once
#include "VimbaCPP/Include/VimbaCPP.h"
#include "Camera.h"
#include "AVTCamera.h"
#include "Projector.h"


#define vimbaError (VmbErrorSuccess!=err)


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
	ActiveCamera getCamera(std::string dev_id);
	ActiveCamera getCamera(int index);
	Projector* getProjector(std::string dev_id);
	Projector* getProjectors();

	void loadXmlSettingsToFirstAVTCamera();

	int getCountCameras() { return numCams; }

private:
	AVT::VmbAPI::VimbaSystem* sistema;
	VmbErrorType    err;
	ActiveCamera* cameraList;

	int numCams;


	//AVTCamera* detectAVTCameras();
	int detectAVTCameras(AVTCamera* avtList);
};

