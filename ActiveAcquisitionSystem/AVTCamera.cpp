//#define AVT_DEBUG

#include "AVTCamera.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <QString>
#include <QFile>

#include <qdatastream.h>
#include "../External/Vimba/VmbImageTransformHelper.hpp"

using namespace AVT::VmbAPI;
using namespace std;

template <class T>
void DeleteArray(T *pArray)
{
	delete[] pArray;
}


void PrintFtrInfo(const FeaturePtr &fs)
{
	std::string s;
	try {
		fs->GetValue(s);
		printf("Ftr: %s\n", s.c_str());
	}
	catch (exception e)
	{
		LOGERR("Ftr failed!");
	}	
}
AVTCamera::AVTCamera()
{

}

AVTCamera::AVTCamera(CameraPtr avtCam):pCam(avtCam), playingProjectionSequence(false), indexPicture(0), settingsLoaded(false)
{
	outputFolder = ".";
	frameObserver = new FrameObserverAVT(avtCam, this);

	VmbErrorType err = pCam->Open(VmbAccessModeFull);
	if (err == VmbErrorSuccess)
	{
		FeaturePtr feature;
		pCam->GetFeatureByName("DeviceID",feature);

		feature->GetValue(dev_id);
		//TODO set name and other features (inf)
	}
	pCam->Close();
#ifdef AVT_DEBUG
	VmbErrorType err = pCam->Open(VmbAccessModeFull);
	if (err == VmbErrorSuccess)
	{

		FeaturePtr feature;
		pCam->GetID(dev_id);
		FeaturePtrVector fs;
		pCam->GetFeatures(fs);
		for_each(fs.begin(), fs.end(), PrintFtrInfo);


		
		pCam->Close();
	}

#endif // AVT_DEBUG

}
AVTCamera::~AVTCamera()
{
	printf("del cam\n");
	VmbError_t error = releaseBuffer();

	try {
		pCam->Close();
	}
	catch (...)
	{
		printf("deleting but already closed");
	}
	
}
bool AVTCamera::loadSettings(std::string configXml)
{
	if (settingsLoaded)
		return false;//meaning only load settings once, not overwritting allowed.
	VmbErrorType err = pCam->Open(VmbAccessModeFull);
	std::stringstream ss;
	bool apiFlag = false;
	bool cameraFlag = false;
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not open camera [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		throw std::exception();
	}

	//  create settings struct to determine behaviour during loading
	VmbFeaturePersistSettings_t settingsStruct;
	settingsStruct.loggingLevel = 4;
	settingsStruct.maxIterations = 5;
	settingsStruct.persistType = VmbFeaturePersistNoLUT;

	//  re-load saved settings from file
	err = pCam->LoadCameraSettings(configXml, &settingsStruct);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not load camera settings to file '" << configXml << "' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review catcherror&logging! (inf-10)
		err = sys.Shutdown();
		if (VmbErrorSuccess != err)
		{
		ss.str("");
		ss << "Could not shutdown Vimba [error code: " << err << "]";
		std::cout << ss.str() << std::endl;
		}
		*/
		apiFlag = false;

		throw std::exception();
	}

	ss.str("");
	ss << "--> Feature values have been loaded from given XML file '" << configXml << "'";
	std::cout << ss.str() << std::endl;

	pCam->Close();


	settingsLoaded = true;

	err = prepareCapture();
	if (VmbErrorSuccess != err)
	{
		LOGERR("error preparing capture");//: %d\n", err);
	}
	else {
		LOGEXEC("Camera prepared for capturing");
	}

	return true;
}


bool AVTCamera::setFrame(const AVT::VmbAPI::FramePtr &frame)
{
	LOGEXEC("settingFrame");
	VmbUchar_t          *imgData = NULL;

	VmbPixelFormatType   pixelFormat;
	VmbUint32_t             nWidth = 0, nHeight = 0;

	if (VmbErrorSuccess == frame->GetWidth(nWidth) &&
		VmbErrorSuccess == frame->GetHeight(nHeight) &&
		VmbErrorSuccess == frame->GetBuffer(imgData) &&
		VmbErrorSuccess == frame->GetPixelFormat(pixelFormat))
	{
		VmbUint32_t nSize;
		if (VmbErrorSuccess != frame->GetImageSize(nSize))
			return false;
		try
		{
			QSharedPointer<unsigned char> m_pFrame = QSharedPointer<unsigned char>(new unsigned char[nSize], DeleteArray<unsigned char>);
			memcpy(m_pFrame.data(), imgData, nSize);//Shared pointer is to be able to reuse the original in the queue
			QImage convertedImage(nWidth, nHeight, QImage::Format_RGB32);
			VmbError_t error;
			try
			{
				error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, pixelFormat);
				//error = AVT::VmbImageTransform(convertedImage, imgData, nWidth, nHeight, pixelFormat);
			}
			catch (...)
			{

				error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, AVT::GetCompatibleMonoPixelFormatForRaw(pixelFormat));
				//error = AVT::VmbImageTransform(convertedImage, imgData, nWidth, nHeight, AVT::GetCompatibleMonoPixelFormatForRaw(pixelFormat));
			}
			if (VmbErrorSuccess != error)
			{
				printf("error converting image %d\n", error);
				return error;
			}
			QString pName = QString(outputFolder.c_str()) + "/" + QString("%1").arg(indexPicture, 2, 10, QChar('0')) + ".png";
			
			convertedImage.save(pName, "PNG");
			indexPicture++;
			char msg[120];
			sprintf(msg,"%s%s", "png saved in ", pName.toStdString().c_str());
			LOGEXEC(msg);
			qDebug();

			return error;

		}
		catch (...)
		{
			LOGERR("Oh oh! failed saving picture");
			return false;
		}

		return true;
	}

	return false;
}
int AVTCamera::takeSinglePicture()
{
	LOGEXEC("Trying to take picture");
	//return 0;
	
	FeaturePtr pFeat;
	VmbErrorType err = pCam->GetFeatureByName("AcquisitionStart", pFeat);
	err = pFeat->RunCommand();//TODO In principle I should make sure of setting the correct feature to SingleShot before running the command. (3)
		
	if (VmbErrorSuccess != err)
	{
		LOGERR("error in acquisition");//: %d\n", err);
		return err;
	}
	else {
		bool bIsCommandDone = false;
		do
		{
			if (VmbErrorSuccess != pFeat->IsCommandDone(bIsCommandDone))
			{
				LOGERR("ohoh!");
				break;
			}
		} while (false == bIsCommandDone);
	}
	LOGEXEC("picture taken without issues");

	err = pCam->GetFeatureByName("AcquisitionStop", pFeat);
	err = pFeat->RunCommand();
	if (VmbErrorSuccess != err)
	{
		LOGERR("error in stoping: ");
		return err;
	}
	else {
		bool bIsCommandDone = false;
		do
		{
			if (VmbErrorSuccess != pFeat->IsCommandDone(bIsCommandDone))
			{
				LOGERR("ohoh!");
				break;
			}
		} while (false == bIsCommandDone);
	}
	LOGEXEC("stoping good.");
	return 0;
	
}


bool AVTCamera::syncProjectionSequence(Projector * p)//FIX REMOVE?? (clean)
{	
	if (playingProjectionSequence)
	{
		qDebug("projectionSequence already playing");
		return false;
	}
	playingProjectionSequence = true;
	indexPicture = 0;
	//TODO!!! Verify workflow and complete. For now is just to keep track of the number of pictures! (2)
	return false;
}


VmbError_t AVTCamera::releaseBuffer(void)
{
	LOGEXEC("releasing buffer");
	//m_pFrameObs->Stopping();	
	
	VmbError_t error = pCam->FlushQueue();
	if (VmbErrorSuccess == error)
		error = pCam->EndCapture();
	
	if (VmbErrorSuccess == error)
		try {
		//error = pCam->RevokeAllFrames();//FIX Remove or not? it was throwing unhandled exception! But closing camera afterwards seems enough.
	}
	catch (...) { LOGERR("revoking failed"); }

	char msg[120];
	sprintf(msg, "result releasing buffer: %d", error);
	LOGEXEC(msg);
	return error;
}

VmbErrorType AVTCamera::prepareCapture(void)
{
	VmbErrorType error = VmbErrorSuccess;

	error = pCam->Open(VmbAccessModeFull);
	std::stringstream ss;
	bool apiFlag = false;
	bool cameraFlag = false;
	if (VmbErrorSuccess != error)
	{
		LOGEXEC("not opening");
		return error;
	}


	FeaturePtr pFeature;
	VmbInt64_t nPayload = 0;
	vector <FramePtr> frames;
	error = pCam->GetFeatureByName("PayloadSize", pFeature);
	VmbUint32_t nCounter = 0;
	if (VmbErrorSuccess == error)
	{
		error = pFeature->GetValue(nPayload);
		if (VmbErrorSuccess == error)
		{			
			frames.resize(FRAME_BUFFER_COUNT);

			for (int i = 0; i<frames.size(); i++)
			{
				try
				{
					frames[i] = FramePtr(new Frame(nPayload));
					nCounter++;
				}
				catch (std::bad_alloc&)
				{
					frames.resize((VmbInt64_t)(nCounter * 0.7));
					break;
				}
				//IFrameObserverPtr pObserver(this);
				IFrameObserverPtr pObserver(frameObserver);
				error = frames[i]->RegisterObserver(pObserver);
				if (VmbErrorSuccess != error)
				{
					return error;
				}
			}

			for (int i = 0; i<frames.size(); i++)
			{
				error = pCam->AnnounceFrame(frames[i]);
				if (VmbErrorSuccess != error)
				{
					return error;
				}
			}
			if (VmbErrorSuccess == error)
			{
				error = pCam->StartCapture();
				if (VmbErrorSuccess != error)
				{
					return error;
				}
			}			
			for (int i = 0; i<frames.size(); i++)
			{
				error = pCam->QueueFrame(frames[i]);
				if (VmbErrorSuccess != error)
				{
					return error;
				}
			}
			
		}
		else
		{
			return error;
		}
	}
	else
	{
		return error;
	}
	char msg[120];
	sprintf(msg, "result prepare capture: %d", error);
	LOGEXEC(msg);
	return error;
}