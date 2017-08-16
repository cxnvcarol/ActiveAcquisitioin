#pragma once
#include "Camera.h"
#include "EDSWrapper.h"

class CanonCamera :
	public ActiveCamera
{
public:
	CanonCamera();
	void setEdsCameraRef(EdsCameraRef *camera);
	~CanonCamera();

	int takeSinglePicture();

private:
	EdsCameraRef *camera;
};

