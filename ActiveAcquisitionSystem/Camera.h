#pragma once
#include <string>
#include <qdatetime.h>

#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QDateTime::currentDateTime().time().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QDateTime::currentDateTime().time().msecsSinceStartOfDay(),(msg), __LINE__)


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



