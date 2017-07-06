#pragma once

struct CameraSettings {

};

class ActiveCamera
{

public:
	ActiveCamera();
	virtual ~ActiveCamera();

//	template <typename CameraSettings>;

	virtual int takePicture();
	virtual void loadSettings(CameraSettings cs);
};



