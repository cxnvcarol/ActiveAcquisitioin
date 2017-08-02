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
		printf("Ftr failed!");
	}	
}
AVTCamera::AVTCamera() : IFrameObserver(CameraPtr())
{

}

AVTCamera::AVTCamera(CameraPtr avtCam) : IFrameObserver(avtCam),playingProjectionSequence(false), indexPicture(0), outputFolder("./")
{
	pCam = avtCam;
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

		/*//TODO Review catcherror&logging!
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

	return true;
}

void AVTCamera::FrameReceived(const FramePtr frame)
{
	VmbFrameStatusType statusType = VmbFrameStatusInvalid;

	if (VmbErrorSuccess == frame->GetReceiveStatus(statusType))
	{
		/* ignore any incompletely frame */
		if (VmbFrameStatusComplete != statusType)
		{
			printf("oh oh! incomplete frame\n");
			pCam->QueueFrame(frame);
			return;
		}
		setFrame(frame);
		indexPicture++;
	}
	pCam->QueueFrame(frame);
}
int AVTCamera::takeSinglePicture()
{
	
	VmbErrorType err = pCam->Open(VmbAccessModeFull);
	std::stringstream ss;
	bool apiFlag = false;
	bool cameraFlag = false;
	if (VmbErrorSuccess != err)
	{
		return err;		
	}

	prepareCapture();
	if (VmbErrorSuccess != err)
	{
		printf("something wrong preparing the capture");
		return err;
	}

	FeaturePtr pFeat;
	err = pCam->GetFeatureByName("AcquisitionStart", pFeat);
	err = pFeat->RunCommand();//TODO In principle I should make sure of setting the correct feature to SingleShot before running the command. TODO Test what if multiple pictures!?

	
	if (VmbErrorSuccess != err)
	{
		return err;
	}
	else {
		bool bIsCommandDone = false;
		do
		{
			if (VmbErrorSuccess != pFeat->IsCommandDone(bIsCommandDone))
			{
				printf("ohoh!");
				break;
			}
		} while (false == bIsCommandDone);
	}
	printf("picture taken without issues.\n");
//	err = pCam->Close();
	if (VmbErrorSuccess != err)
	{
		return err;
	}
	return 0;
	
}

/*
VmbError_t AVTCamera::processFrame()
{
	//TODO... remove?
	QImage convertedImage(nWidth, nHeight, QImage::Format_RGB32);
	VmbError_t error;
	try
	{
		error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, pixelFormat);
	}
	catch (...)
	{
		
		error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, AVT::GetCompatibleMonoPixelFormatForRaw(pixelFormat));
	}
	if (VmbErrorSuccess != error)
	{
		printf("error converting image %d\n", error);		
		return error;
	}
	
	convertedImage.save("./result111.png","PNG");
	printf("png saved");
	return error;
}
*/

bool AVTCamera::setFrame(const AVT::VmbAPI::FramePtr &frame)
{
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
			/*
			QString s = "./";
			s.append("\\");
			s.append("imname").append(QString::number(55)).append(".bin");//TODO Verify name.... cool, we are good here!. BUT IT IS a useless format!!


			//// image processing takes shorter than the FPS with no converting
			printf("it is about to save the rawfile %s\n", s.toStdString().c_str());
			
			m_pFrame = QSharedPointer<unsigned char>(new unsigned char[nSize], DeleteArray<unsigned char>);
			memcpy(m_pFrame.data(), imgData, nSize);
			
			// saving Raw Data 
			QFile rawFile(s);
			rawFile.open(QIODevice::WriteOnly);
			QDataStream out(&rawFile);
			out.writeRawData((const char*)m_pFrame.data(), nSize);
			rawFile.close();
			// Now save a usable picture:
			processFrame();//TODO Check if I should/can do it without class properties!
			*/
			//QSharedPointer<unsigned char> m_pFrame = QSharedPointer<unsigned char>(new unsigned char[nSize], DeleteArray<unsigned char>);
			//memcpy(m_pFrame.data(), imgData, nSize);//TODO Review: do i need the copy? in a shared pointer??
			QImage convertedImage(nWidth, nHeight, QImage::Format_RGB32);
			VmbError_t error;
			try
			{
				//error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, pixelFormat);
				error = AVT::VmbImageTransform(convertedImage, imgData, nWidth, nHeight, pixelFormat);
			}
			catch (...)
			{

				//error = AVT::VmbImageTransform(convertedImage, m_pFrame.data(), nWidth, nHeight, AVT::GetCompatibleMonoPixelFormatForRaw(pixelFormat));
				error = AVT::VmbImageTransform(convertedImage, imgData, nWidth, nHeight, AVT::GetCompatibleMonoPixelFormatForRaw(pixelFormat));
			}
			if (VmbErrorSuccess != error)
			{
				printf("error converting image %d\n", error);
				return error;
			}
			QString pName = QString(outputFolder.c_str())+"/"+QString("%1").arg(indexPicture, 5, 10, QChar('0'))+".png";
			convertedImage.save("./result111.png", "PNG");
			convertedImage.save(pName, "PNG");
			qDebug("png saved in ");
			qDebug(pName.toStdString().c_str());
			return error;
			
		}
		catch (...)
		{
			printf("Oh oh! failed saving picture\n");
			return false;
		}

		return true;
	}

	return false;
}

void AVTCamera::setOutputFolder(std::string folder)
{
	outputFolder = folder;
	//TODO (1) Evaluate if folder exists, if not create it!
}

bool AVTCamera::syncProjectionSequence(Projector * p)
{
	
	if (playingProjectionSequence)
	{
		qDebug("projectionSequence already playing");
		return false;
	}
	playingProjectionSequence = true;
	indexPicture = 0;
	//TODO Verify workflow and complete. For now is just to keep track of the number of pictures!
	return false;
}


VmbError_t AVTCamera::releaseBuffer(void)
{
	printf("releasing buffer\n");
	//m_pFrameObs->Stopping();
	FeaturePtr pFeature;
	VmbError_t error = pCam->GetFeatureByName("AcquisitionStop", pFeature);
	error = pFeature->RunCommand();
	error = pCam->EndCapture();
	if (VmbErrorSuccess == error)
		error = pCam->FlushQueue();
	if (VmbErrorSuccess == error)
		error = pCam->RevokeAllFrames();

	return error;
}

VmbError_t AVTCamera::prepareCapture(void)
{
	FeaturePtr pFeature;
	VmbInt64_t nPayload = 0;
	vector <FramePtr> frames;
	VmbError_t error = pCam->GetFeatureByName("PayloadSize", pFeature);
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
				IFrameObserverPtr pObserver(this);
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

	return error;
}
