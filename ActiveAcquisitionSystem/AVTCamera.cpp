//#define AVT_DEBUG

#include "AVTCamera.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <QString>
#include <QFile>

#include <qdatastream.h>
#include "Vimba/VmbImageTransformHelper.hpp"
#include "debugMacros.h"

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

AVTCamera::AVTCamera(CameraPtr avtCam):pCam(avtCam), settingsLoaded(false)
{
	hwTriggerSupported = true;
	outputFolder = ".";
	playingProjectionSequence = false;
	indexPicture = 0;

	frameObserver = new FrameObserverAVT(avtCam, this);

	VmbErrorType err = pCam->Open(VmbAccessModeFull);
	if (err == VmbErrorSuccess)
	{
		FeaturePtr feature;
		pCam->GetFeatureByName("DeviceID",feature);

		feature->GetValue(dev_id);
		//Review set name and other features (inf)
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

		
		apiFlag = false;

		throw std::exception();
	}

	ss.str("");
	ss << "--> Feature values have been loaded from given XML file '" << configXml << "' to camera id "<<dev_id.c_str();
	std::cout << ss.str() << std::endl;

	pCam->Close();

	settingsLoaded = true;
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
			string pName = outputFolder + "/" + QString("%1").arg(indexPicture, 3,10, QChar('0')).toStdString() + ".png";//this is 2 digits for the picture name in base 10
			
			convertedImage.save(QString(pName.c_str()), "PNG");
			indexPicture++;
			LOGEXEC("%s%s", "png saved in ", pName.c_str());
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
int AVTCamera::takeSinglePicture()//only used in software synchronization mode (standard projector as master)
{
	LOGEXEC("Trying to take picture");
	//return 0;
	
	FeaturePtr pFeat;
	VmbErrorType err = pCam->GetFeatureByName("TriggerSoftware", pFeat);
	err = pFeat->RunCommand();
		
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
	
	return 0;
	
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

	LOGEXEC("result releasing buffer: %d", error);
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
	if (VmbErrorSuccess == pCam->GetFeatureByName("AcquisitionMode", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("Continuous"))
		{
			LOGERR("unable to set Continuous mode for the avt camera %s", dev_id.c_str());
		}
	}
	//review in case of bugs... Force the triggersource, review if working fine. (i.e, triggerselector, triggermode, triggersource...  is it the right order??
	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerSelector", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("AcquisitionStart"))
		{
			LOGERR("unable to set triggerselector to AcquisitionStart for the avt camera %s", dev_id.c_str());
		}
	}

	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerMode", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("Off"))
		{
			LOGERR("unable to set triggermode off for the avt camera %s", dev_id.c_str());
		}
	}

	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerSelector", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("AcquisitionStop"))
		{
			LOGERR("unable to set triggerselector to AcquisitionStop for the avt camera %s", dev_id.c_str());
		}
	}

	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerMode", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("Off"))
		{
			LOGERR("unable to set triggermode off for the avt camera %s", dev_id.c_str());
		}
	}


	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerSelector", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("FrameStart"))
		{
			LOGERR("unable to set triggerselector to FrameStart for the avt camera %s", dev_id.c_str());
		}
	}
	if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerMode", pFeature))
	{
		if (VmbErrorSuccess != pFeature->SetValue("On"))
		{
			LOGERR("unable to set FRAMESTART triggermode on for the avt camera %s", dev_id.c_str());
		}
	}


	if (syncmode == SyncMode::SOFTWARE_MODE)
	{
		if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerSource", pFeature))
		{
			if (VmbErrorSuccess != pFeature->SetValue("Software"))
			{
				LOGERR("unable to set triggersource to Software for the avt camera %s", dev_id.c_str());
			}
		}
	}
	else if (syncmode == SyncMode::HARDWARE_MODE)
	{
		if (VmbErrorSuccess == pCam->GetFeatureByName("TriggerSource", pFeature))
		{
			if (VmbErrorSuccess != pFeature->SetValue("Line1"))
			{
				LOGERR("unable to set triggersource to Line1 for the avt camera %s", dev_id.c_str());
			}
		}
	}
	//end looking here.


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
	LOGEXEC("result prepare capture: %d", error);
	return error;
}

bool AVTCamera::notifyStopProjectionSequence()
{
	if (ActiveCamera::notifyStopProjectionSequence())
	{
		FeaturePtr pFeat;
		VmbErrorType err = pCam->GetFeatureByName("AcquisitionStop", pFeat);
		err = pFeat->RunCommand();
		if (VmbErrorSuccess != err)
		{
			LOGERR("error in stoping acquisition: ");
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

		LOGEXEC("about to release AVT buffer");
		VmbError_t error = releaseBuffer();
		try {
			pCam->Close();
		}
		catch (...)
		{
			LOGERR("deleting but already closed");
		}
		return true;
	}
	else return false;
}

bool AVTCamera::notifyStartProjectionSequence()
{
	if (ActiveCamera::notifyStartProjectionSequence())
	{
		VmbErrorType err = prepareCapture();
		if (VmbErrorSuccess != err)
		{
			LOGERR("error in preparing acquisition: ");
			return err;
		}
		FeaturePtr pFeat;
		err = pCam->GetFeatureByName("AcquisitionStart", pFeat);
		err = pFeat->RunCommand();
		if (VmbErrorSuccess != err)
		{
			LOGERR("error in startnig acquisition: ");
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
		LOGEXEC("starting cam good.");
		if (VmbErrorSuccess != err)
		{
			LOGERR("error preparing capture");//: %d\n", err);
		}
		else {
			LOGEXEC("Camera prepared for capturing");
		}
		return true;

	}
	else return false;
	

	
}
