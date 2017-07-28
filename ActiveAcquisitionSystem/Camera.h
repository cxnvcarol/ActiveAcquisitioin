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


	virtual int takeSinglePicture();
	virtual bool loadSettings(CameraSettings cs);
	virtual bool loadSettings(std::string pathToSettings);
protected:
	std::string dev_id;
};



