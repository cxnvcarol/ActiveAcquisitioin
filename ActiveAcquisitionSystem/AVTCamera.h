#pragma once
#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
//#include <string>

class AVTCamera :
	public ActiveCamera
{
public:
	AVTCamera();
	AVTCamera(AVT::VmbAPI::CameraPtr avtCam);
	~AVTCamera();

	void setCameraPtr(AVT::VmbAPI::CameraPtr avtCam);
	bool loadSettings();
	bool loadSettings(std::string configPath);


	std::string getAnyStr()
	{
		if (pCam == NULL)
		{
			std::string ns = "no se cuando paso, ni como sucedio, lo unico que yo se es que me muero por tii.";
			printf("ns: %s\n", ns.c_str());
			return ns;
		}
		pCam->Open(VmbAccessModeFull);
		
		pCam->Close();
		return "any string";
	}
	AVT::VmbAPI::CameraPtr getAVTPointer() { 
		return pCam; }

	void setName(std::string n) {name = n; }
	std::string getName() { 
		const char* n = name.c_str();
		printf("%s:\n", n);

		return name; }

private:
//	VimbaSystem& sys;

	std::string name;
	std::string model;
	std::string serial;
	std::string interfaceID;//i.e. ETHERNET for GigE cameras

	std::string ipAddress;

	AVT::VmbAPI::CameraPtr pCam;
	
};

