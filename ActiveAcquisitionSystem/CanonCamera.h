#pragma once
#include "Camera.h"
#include "EDSDK\EDSDK.h"

class CanonCamera :
	public ActiveCamera
{
public:
	CanonCamera();
	CanonCamera(EdsCameraRef *camera);
	void setEdsCameraRef(EdsCameraRef *camera);
	~CanonCamera();

	int takeSinglePicture();

	bool notifyStopProjectionSequence();

	bool notifyStartProjectionSequence();

	int prepareCapture();

private:
	EdsCameraRef* camera;

	static EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
	EdsError downloadImage(EdsDirectoryItemRef directoryItem);
};

