#pragma once

class AcquisitionDeviceManager
{
	/*
	Initialize and handle APIs for devices and their events.
	*/
public:
	AcquisitionDeviceManager();
	~AcquisitionDeviceManager();

private:
	VimbaSystem* sistema;
	VmbErrorType    err;
	
};

