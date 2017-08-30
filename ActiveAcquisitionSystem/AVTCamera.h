#pragma once

#define FRAME_BUFFER_COUNT 5

#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <qimage.h>
#include "Projector.h"
#include "FrameObserverAVT.h"

//#include <string>
using namespace AVT::VmbAPI;
using namespace std;
enum class SyncMode {
	SOFTWARE_MODE,
	HARDWARE_MODE
};

class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera(CameraPtr avtCam);
	AVTCamera();
	~AVTCamera();

	bool loadSettings(std::string configPath);	
	int takeSinglePicture();//Review, should it be virtual?

	CameraPtr getAVTPointer() { return pCam; }
	void setName(string n) {name = n; }
	string getName() { 	return name; }
	
	

	bool setFrame(const AVT::VmbAPI::FramePtr & frame);
	VmbErrorType prepareCapture(void);

	bool notifyStopProjectionSequence();
	bool notifyStartProjectionSequence();
	void setSyncMode(SyncMode s) { syncmode = s; }
	SyncMode getSyncMode() { return syncmode; }

private:
	string name;
	string model;
	string serial;
	string interfaceID;//i.e. ETHERNET for GigE cameras

	std::string ipAddress;
	CameraPtr pCam;
	
	VmbError_t releaseBuffer(void);
	
	

	FrameObserverAVT* frameObserver;

	bool settingsLoaded;
	SyncMode syncmode = SyncMode::SOFTWARE_MODE;
	
};
