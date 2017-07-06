#pragma once
#include "Camera.h"
class CanonCamera :
	public ActiveCamera
{
public:
	CanonCamera();
	~CanonCamera();

	int takePicture();
};

