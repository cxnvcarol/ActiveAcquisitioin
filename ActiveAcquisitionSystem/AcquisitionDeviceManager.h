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
	
	std::vector<AVTCamera*> getCameras();
	ActiveCamera getCamera(std::string dev_id);
	//ActiveCamera* getCamera(int index);
	AVTCamera* getCamera(int index);
	Projector* getProjector(std::string dev_id);
	std::vector<Projector*> getProjectors();

	std::vector<AVTCamera*> detectAVTCameras();
	static bool testValidCameraPtr(AVT::VmbAPI::CameraPtr sh);

	int getCountCameras() {
		
		return numCams; }

private:
	AVT::VmbAPI::VimbaSystem* sistema;
	VmbErrorType    err;
	std::vector<AVTCamera*> cameraList;
	std::vector<Projector*> projectors;

	int numCams;


	//AVTCamera* detectAVTCameras();
};

