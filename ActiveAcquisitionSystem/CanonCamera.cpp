#include "CanonCamera.h"

CanonCamera::CanonCamera()
{
}

CanonCamera::CanonCamera(EdsCameraRef  *camera)
{
	setEdsCameraRef(camera);
	hwTriggerSupported = false;
}

void CanonCamera::setEdsCameraRef(EdsCameraRef* camref)
{
	camera = camref;

	EdsError err = EdsOpenSession(*camera);
	EdsDataType dataType;
	EdsUInt32 dataSize;// = 200;
	err = EdsGetPropertySize(*camera, kEdsPropID_BodyIDEx, 0, &dataType, &dataSize);
	
	if (err == EDS_ERR_OK)
	{
		EdsChar *serial=(EdsChar *)calloc(dataSize,sizeof(EdsChar));
		err = EdsGetPropertyData(*camera, kEdsPropID_BodyIDEx, 0, dataSize, serial);//kEdsDataType_String
		if (err == EDS_ERR_OK)
		{
			LOGEXEC("CanonCamera serial>>%s", serial);
			dev_id = serial;
			printf("\n\n\n");
			//Review Complete other important properties!
		}
		
	}
	else {
		LOGERR("Error reading serial from canon:%d", err);
	}
	if (err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(*camera, kEdsObjectEvent_All, handleObjectEvent, this);
		printf("object callback set\n\n");
	}
	err = EdsCloseSession(*camera);
	
}


CanonCamera::~CanonCamera()
{
	EdsCloseSession(*camera);
	
}
int CanonCamera::prepareCapture()
{
	EdsError err = EdsOpenSession(*camera);
	if (err = EDS_ERR_OK)
	{
		int p = kEdsSaveTo_Both;
		err = EdsSetPropertyData(*camera, kEdsPropID_SaveTo, 0, sizeof(p), &p);
	}
	else { LOGERR("couldn't open camera session: %d",err); }
	////TODO IMPORTANT Read important properties before capturing!! (needed for further processing!!!) - aperture, exposure time, etc (1)
	return err;
}

int CanonCamera::takeSinglePicture()
{
	//PRE: assumed open camera already
	LOGEXEC("taking canon picture!");
	//err = EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);//TODO FIX check camera type and use this command for its supported cams:
	/*
	EOS-1D
	Mark III, EOS 40D, EOS - 1Ds Mark III,
	EOS DIGITAL REBEL Xsi / 450D / Kiss
	X2, EOS DIGITAL REBEL XS / 1000D /
	KISS F.
	*/
	EdsError err = EdsSendCommand(*camera, kEdsCameraCommand_PressShutterButton
		, kEdsCameraCommand_ShutterButton_Completely);
	err = EdsSendCommand(*camera, kEdsCameraCommand_PressShutterButton
		, kEdsCameraCommand_ShutterButton_OFF);
	if (err == EDS_ERR_OK){LOGEXEC("Canon picture taking");}
	else LOGERR("error taking canon picture:%d", err);

	return err;
}

bool CanonCamera::notifyStopProjectionSequence()
{
	if (ActiveCamera::notifyStopProjectionSequence())//if it was in fact opened
	{
		Sleep(50);//time to react for the last capture
		EdsError err = EdsCloseSession(*camera);
		if (err == EDS_ERR_OK)
		{
			LOGEXEC("Canon session closed");
		}
		else { LOGERR("error closing canon session:%d", err); }


		err = EdsOpenSession(*camera);
		if (err == EDS_ERR_OK)
		{
			Sleep(300);//time to receive object events
			err = EdsCloseSession(*camera);
			if (err == EDS_ERR_OK)
			{
				LOGEXEC("Canon session closed");
			}
			else { LOGERR("error closing canon session:%d", err); }
		}
		else { LOGERR("error openning canon session:%d", err); }
		
		return true;
	}
	return false;
	
}

bool CanonCamera::notifyStartProjectionSequence()
{
	if(ActiveCamera::notifyStartProjectionSequence())
	{ 
		return prepareCapture()==EDS_ERR_OK;
	}
	else { return false; }
}

EdsError EDSCALLBACK CanonCamera::handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context)
{
	EdsError err = EDS_ERR_OK;
	// do something

	CanonCamera* caller = (CanonCamera*)context;

	switch (event)
	{
	case kEdsObjectEvent_DirItemRequestTransfer:
	case kEdsObjectEvent_DirItemCreated:
		printf("downloading image");
		err=caller->downloadImage(object);
		break;
	default:
		break;
	}

	// Object must be released
	if (object)
	{
		EdsRelease(object);
	}
	return err;
}

EdsError CanonCamera::downloadImage(EdsDirectoryItemRef directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	err = EdsGetDirectoryItemInfo(directoryItem, &dirItemInfo);
	// Create file stream for transfer destination
	if (err == EDS_ERR_OK)
	{
		//char* filename = dirItemInfo.szFileName;
		std::string savepath = outputFolder + "/" + QString("%1").arg(indexPicture, 3, 10, QChar('0')).toStdString() + ".png";//this is 2 digits for the picture name in base 10
		LOGEXEC("saved at: %s", savepath.c_str());
		err = EdsCreateFileStream(dirItemInfo.szFileName,
			kEdsFileCreateDisposition_CreateAlways,
			kEdsAccess_ReadWrite, &stream);
	}
	// Download image
	if (err == EDS_ERR_OK)
	{
		err = EdsDownload(directoryItem, dirItemInfo.size, stream);
	}
	// Issue notification that download is complete
	if (err == EDS_ERR_OK)
	{
		err = EdsDownloadComplete(directoryItem);
	}
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}