#pragma once
#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
//#include <string>

class AVTCamera :
	public ActiveCamera
{
public:
	//AVTCamera();
	AVTCamera(AVT::VmbAPI::CameraPtr avtCam);
	~AVTCamera();

	void setCameraPtr(AVT::VmbAPI::CameraPtr avtCam);


	bool loadSettings();


private:
//	VimbaSystem& sys;

	std::string name;
	std::string model;
	std::string serial;
	std::string interfaceID;//i.e. ETHERNET for GigE cameras

	AVT::VmbAPI::CameraPtr pCam;
	
};

