#pragma once

#define FRAME_BUFFER_COUNT 5

#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
//#include <string>
using namespace AVT::VmbAPI;
using namespace std;
class AVTCamera :
	public ActiveCamera, virtual public IFrameObserver
{
public:
	//void triggerCamera();
	AVTCamera(CameraPtr avtCam);
	AVTCamera();
	~AVTCamera();

	bool loadSettings(std::string configPath);
	virtual void FrameReceived(const FramePtr pFrame);
	virtual int takeSinglePicture();

	bool setFrame(const AVT::VmbAPI::FramePtr & frame);

	
	
	CameraPtr getAVTPointer() { return pCam; }
	void setName(string n) {name = n; }
	string getName() { 	return name; }

private:
	string name;
	string model;
	string serial;
	string interfaceID;//i.e. ETHERNET for GigE cameras

	std::string ipAddress;

	CameraPtr pCam;


	VmbError_t releaseBuffer(void);

	VmbError_t prepareCapture(void);
	
};
