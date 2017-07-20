#pragma once
#include <string>

struct CameraSettings {

};

class ActiveCamera
{

public:
	ActiveCamera(std::string dev_id);
	ActiveCamera();
	virtual ~ActiveCamera();

//	template <typename CameraSettings>;

	virtual int takePicture();
	virtual bool loadSettings(CameraSettings cs);
	virtual bool loadSettings(std::string pathToSettings);
protected:
	std::string dev_id;
};



