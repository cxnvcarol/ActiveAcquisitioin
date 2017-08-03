#include "FrameObserverAVT.h"
#include "AVTCamera.h"


FrameObserverAVT::FrameObserverAVT(CameraPtr avtCam, AVTCamera*p) : IFrameObserver(avtCam),pCam(avtCam),parent(p)
{
}

FrameObserverAVT::FrameObserverAVT() : IFrameObserver(CameraPtr())
{
}

FrameObserverAVT::~FrameObserverAVT() 
{
	printf("del frameobserver\n");
}


void FrameObserverAVT::FrameReceived(const FramePtr frame)
{
	VmbFrameStatusType statusType = VmbFrameStatusInvalid;

	if (VmbErrorSuccess == frame->GetReceiveStatus(statusType))
	{
		/* ignore any incompletely frame */
		if (VmbFrameStatusComplete != statusType)
		{
			printf("oh oh! incomplete frame\n");
			pCam->QueueFrame(frame);
			return;
		}
		parent->setFrame(frame);
		
	}
	pCam->QueueFrame(frame);
}