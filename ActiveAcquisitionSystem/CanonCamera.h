#pragma once
#include "Camera.h"
//#include "EDSWrapper.h"
#include "../External/EDSDK/EDSDK.h"

class CanonCamera :
	public ActiveCamera
{
public:
	CanonCamera();
	CanonCamera(EdsCameraRef *camera);
	void setEdsCameraRef(EdsCameraRef *camera);
	~CanonCamera();

	//int takeSinglePicture();

private:
	EdsCameraRef* camera;
};

