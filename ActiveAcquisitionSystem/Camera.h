#pragma once
#include <string>


#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)


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



