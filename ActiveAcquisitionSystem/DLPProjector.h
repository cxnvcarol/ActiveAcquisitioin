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

	std::string getDev_id() { return pathHid; }


	static bool dLPToSimpleProjectionsSettings(QString filePathIn, QString filePathOut);
	static bool simpleToDLPProjectionsSettings(QString filePathIn, QString filePathOut);


private:
	hid_device* hidHandle;

	std::vector<PatternElement> dlp_pattern_elements;

	bool m_patternImageChange;//todo revieew.. for what?

	void updateLUT();
	std::string pathHid;

};

