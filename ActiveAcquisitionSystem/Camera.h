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
	virtual void setOutputFolder(std::string folder);
	virtual bool loadSettings(CameraSettings cs);
	virtual bool loadSettings(std::string pathToSettings);

	std::string getDevId();
protected:
	std::string dev_id;
	std::string outputFolder;
};



