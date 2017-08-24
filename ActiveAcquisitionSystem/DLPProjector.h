#pragma once
#include "Projector.h"
#include "Texas/HiresLib/usb.h"
class DLPProjector :
	public Projector
{
public:
	DLPProjector();
	DLPProjector(hid_device* hid);
	~DLPProjector();

	void playProjectionSequence(int n);
	void registerCameraObserver(ActiveCamera * cam);
	void loadProjectionSettings(const QString projectionsConfig);

	void setStatus(int statusLCR, uchar SysStatus, uchar HWStatus, uchar MainStatus);

	hid_device* getHidDevice() { return hidHandle; }

private:
	hid_device* hidHandle;

};

