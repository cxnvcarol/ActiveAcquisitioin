#pragma once
#include "VimbaCPP/Include/VimbaCPP.h"
#include "Camera.h"
#include "AVTCamera.h"
#include "Projector.h"
#include "CanonCamera.h"
#include "EDSWrapper.h"
#include "DLPProjector.h"

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
	void detectProjectors();
	
	std::vector<ActiveCamera*> getCameras();
	ActiveCamera* getCamera(std::string dev_id);
	ActiveCamera* getCamera(int index);
	Projector* getProjector(std::string dev_id);
	std::vector<Projector*> getProjectors();

	std::vector<AVTCamera*> detectAVTCameras();
	std::vector<ActiveCamera*> detectAllCameras();
	static bool testValidCameraPtr(AVT::VmbAPI::CameraPtr sh);

	int getCountCameras() {
		
		return numCams; }

private:
	AVT::VmbAPI::VimbaSystem* sistema;
	VmbErrorType    err;
	std::vector<ActiveCamera*> cameraList;
	std::vector<Projector*> projectors;//TODO... remove this and separate btn screen and dlp projectors?? 
	std::vector<DLPProjector*> dlps;
	int numCams;

	EDSWrapper* edsWrapper;

	int detectDLPs();
	bool setDLPStatus(DLPProjector *dlp);
};

