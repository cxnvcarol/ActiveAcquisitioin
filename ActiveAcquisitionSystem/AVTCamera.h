#pragma once
#include "Camera.h"
//#include <string>

class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera();
	~AVTCamera();


	bool loadSettings();


private:
//	VimbaSystem& sys;

	std::string name;
	std::string model;
	std::string serial;
	std::string interfaceID;//i.e. ETHERNET for GigE cameras
	
};

