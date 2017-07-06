#pragma once
#include "Camera.h"
class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera();
	~AVTCamera();


	bool loadSettings();

	static bool detectCameras(AVTCamera* avtList);

private:
//	VimbaSystem& sys;
};

