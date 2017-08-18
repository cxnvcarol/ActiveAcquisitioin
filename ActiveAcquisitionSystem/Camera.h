#pragma once
#include <string>
#include "debugMacros.h"

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
	virtual void notifyStopProjectionSequence();
	virtual void notifyStartProjectionSequence();
	std::string getDevId();
protected:
	std::string dev_id;
	std::string outputFolder;
	int indexPicture;
	bool playingProjectionSequence;
};



