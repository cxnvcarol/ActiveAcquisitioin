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
class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera(CameraPtr avtCam);
	AVTCamera();
	~AVTCamera();

	bool loadSettings(std::string configPath);	
	int takeSinglePicture();//TODO Review, should it be virtual?

	CameraPtr getAVTPointer() { return pCam; }
	void setName(string n) {name = n; }
	string getName() { 	return name; }
	
	

	bool setFrame(const AVT::VmbAPI::FramePtr & frame);
	VmbErrorType prepareCapture(void);//TODO make this member of ActiveCamera, so the cameras are only taken during capturing instead of the whole program execution: to use with GUI

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
};
