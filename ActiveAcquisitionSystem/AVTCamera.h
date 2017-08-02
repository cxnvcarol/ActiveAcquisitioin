#pragma once

#define FRAME_BUFFER_COUNT 5

#include "Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <qimage.h>
//#include <QtCore/QSharedPointer>
#include "Projector.h"

//#include <string>
using namespace AVT::VmbAPI;
using namespace std;
class AVTCamera :
	public ActiveCamera, virtual public IFrameObserver
{
public:
	//void triggerCamera();
	AVTCamera(CameraPtr avtCam);
	AVTCamera();
	~AVTCamera();

	bool loadSettings(std::string configPath);
	virtual void FrameReceived(const FramePtr pFrame);
	virtual int takeSinglePicture();

	

	

	bool setFrame(const AVT::VmbAPI::FramePtr & frame);
	void setOutputFolder(std::string folder);
	
	
	CameraPtr getAVTPointer() { return pCam; }
	void setName(string n) {name = n; }
	string getName() { 	return name; }
	bool syncProjectionSequence(Projector *p);
	void stopProjectionSequence() {
		//TODO Evaluate need of this fn!
		playingProjectionSequence = false;
		indexPicture = 0;
	};

private:
	string name;
	string model;
	string serial;
	string interfaceID;//i.e. ETHERNET for GigE cameras

	std::string ipAddress;

	CameraPtr pCam;

	string outputFolder;


	VmbError_t releaseBuffer(void);

	VmbError_t prepareCapture(void);
	//VmbError_t processFrame();
	//QSharedPointer<unsigned char> m_pFrame;
	bool playingProjectionSequence;
	int indexPicture;
};
