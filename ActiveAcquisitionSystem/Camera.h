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
	virtual void loadSettings(CameraSettings cs);
	virtual void loadSettings(std::string pathToSettings);

private:
	std::string dev_id;
};



