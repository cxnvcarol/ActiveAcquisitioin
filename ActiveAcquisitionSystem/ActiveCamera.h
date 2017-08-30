#pragma once
#include <string>
#include "debugMacros.h"

struct CameraSettings {//review... delete? it was meant to be used in the most general case, but the settings files are used directly for the avt, while the canons are set manually.

};
enum class SyncMode {
	SOFTWARE_MODE,
	HARDWARE_MODE
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
	virtual bool notifyStopProjectionSequence();
	virtual bool notifyStartProjectionSequence();
	std::string getDevId();

	bool supportsHardwareTrigger();

	virtual SyncMode getSyncMode() { return syncmode; }
	virtual void setSyncMode(SyncMode sync) { syncmode= sync; }
protected:
	std::string dev_id;
	std::string outputFolder;
	int indexPicture;
	bool playingProjectionSequence;
	bool hwTriggerSupported;

	SyncMode syncmode = SyncMode::SOFTWARE_MODE;
};



