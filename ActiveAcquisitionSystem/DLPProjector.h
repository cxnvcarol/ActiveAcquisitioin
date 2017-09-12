#pragma once
#include "Projector.h"
#include "Texas/HiresLib/usb.h"
#include "Texas/patternelement.h"

class DLPProjector :
	public Projector
{
public:
	DLPProjector();
	DLPProjector(hid_device* hid, std::string path);
	~DLPProjector();

	void playProjectionSequence(int n);
	void registerCameraObserver(ActiveCamera * cam);
	void loadProjectionSettings(const char* projectionsConfig);
	void loadDLPProjectionsSettings(const QString projectionsConfig);

	void setStatus(int statusLCR, uchar SysStatus, uchar HWStatus, uchar MainStatus);

	hid_device* getHidDevice() { return hidHandle; }


	void notifyPlayToObservers();


	bool triggersByHardware()
	{
		//TODO Review: Not necessarily always true: I could diplay the sequence through USB without any (hw) synchronization and trigger the cameras by software every time it should according to the configured time(!?)... Therefore, implement the capturing sequence for "orphaned/independent" Camera.
		return true;
	}

private:
	hid_device* hidHandle;

	std::vector<PatternElement> dlp_pattern_elements;

	bool m_patternImageChange;//todo review.. for what is being used?

	void updateLUT();
	std::string pathHid;

};

