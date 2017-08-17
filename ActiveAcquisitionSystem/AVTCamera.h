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
	virtual int takeSinglePicture();

	CameraPtr getAVTPointer() { return pCam; }
	void setName(string n) {name = n; }
	string getName() { 	return name; }
	bool syncProjectionSequence(Projector *p);
	void stopProjectionSequence() {
		//TODO Evaluate need of this fn! (inf) --> needed when integrated with GUI to handle multiple capture-runs into the user-session
		playingProjectionSequence = false;
		indexPicture = 0;
	};

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
	bool playingProjectionSequence;
	int indexPicture;

	FrameObserverAVT* frameObserver;

	bool settingsLoaded;
};
