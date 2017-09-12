#pragma once
#include "VimbaCPP/Include/VimbaCPP.h"
#include "ActiveCamera.h"
#include "AVTCamera.h"
//#include "Projector.h"
#include "StandardProjector.h"
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
	
	std::vector<ActiveCamera*> getCameras();
	//ActiveCamera* getCamera(std::string dev_id);//Review: implement! Likely it will be useful
	ActiveCamera* getCamera(int index);
	Projector* getProjector(std::string dev_id);
	//std::vector<Projector*> getProjectors();
	std::vector<StandardProjector*> getStandardProjectors();

	std::vector<AVTCamera*> detectAVTCameras();
	std::vector<AVTCamera*> detectAVTCameras(std::vector<string> onlyInclude);
	std::vector<CanonCamera*> detectCanonCameras();
	std::vector<ActiveCamera*> detectAllCameras();
	std::vector<ActiveCamera*> detectAllCameras(std::vector<string> onlyInclude);
	static bool testValidCameraPtr(AVT::VmbAPI::CameraPtr sh);

	int getCountCameras() { return numCams; }

	int detectDLPs();

	std::vector<DLPProjector*> getDLPs() {
		return dlps;
	};

	void addStandardProjector(StandardProjector* p);

	/*
	StandardProjector* getFirstStdProjector() {
		return standardProjectors.empty() ? NULL : standardProjectors[0];
	}
	*/

	void setMainProjector(Projector* p) {
		mainProjector = p;
	};
	Projector* getMainProjector() {
		return mainProjector;
	};
private:
	AVT::VmbAPI::VimbaSystem* vimbaSys;
	VmbErrorType    err;
	std::vector<ActiveCamera*> cameraList;

	std::vector<StandardProjector*> standardProjectors;
	std::vector<DLPProjector*> dlps;
	int numCams;
	EDSWrapper* edsWrapper;
	
	bool setDLPStatus(DLPProjector *dlp);

	Projector* mainProjector;
};

