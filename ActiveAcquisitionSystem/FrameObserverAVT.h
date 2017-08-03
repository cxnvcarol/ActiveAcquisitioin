#pragma once

#include "VimbaCPP/Include/VimbaCPP.h"
//#include "AVTCamera.h"
class AVTCamera;
using namespace AVT::VmbAPI;
class FrameObserverAVT :
	virtual public IFrameObserver
{
public:
	FrameObserverAVT(CameraPtr avtCam, AVTCamera *p);
	FrameObserverAVT();
	~FrameObserverAVT();

	virtual void FrameReceived(const FramePtr pFrame);
	AVTCamera * parent;

private:
	CameraPtr pCam;
};

