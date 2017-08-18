#include "CanonCamera.h"

CanonCamera::CanonCamera()
{
}

CanonCamera::CanonCamera(EdsCameraRef  *camera)
{
	setEdsCameraRef(camera);
	//TODO Prepare camera: set event handlers, open session, etc. etc.
	//
}

void CanonCamera::setEdsCameraRef(EdsCameraRef* camref)
{
	camera = camref;

	//TODO Separate the following in a prepareCapture function
	
	/* //TODO Review: when do I need property and state callbacks??
	if (err == EDS_ERR_OK)
	{
		err = EdsSetPropertyEventHandler(*camera, kEdsPropertyEvent_All,
			handlePropertyEvent, this);
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetPropertyEventHandler(*camera, kEdsStateEvent_All,
			handleStateEvent, this);
	}

	*/
	///
	
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
		}
		
	}
	else {
		LOGERR("Error reading serial from canon:%d", err);
	}
	if (err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(*camera, kEdsObjectEvent_All, handleObjectEvent, this);//TODO LOOK HERE... fix, refactor to use as independent of wrapper as possible
	}
	err = EdsCloseSession(*camera);
	
}


CanonCamera::~CanonCamera()
{
	EdsCloseSession(*camera);
	
}
int CanonCamera::prepareCapture()
{
	
	//EdsError err = EdsOpenSession(*camera);//Open and close only when taking picture so multiple canons can be supported!!
	return 0;
}

int CanonCamera::takeSinglePicture()
{
	EdsError err = EdsOpenSession(*camera);

	if (err == EDS_ERR_OK)
	{
		fprintf(stderr, "taking pictre!\n");

		//err = EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);//TODO FIX check camera type and use this command for its supported cams:
		/*
		EOS-1D
		Mark III, EOS 40D, EOS - 1Ds Mark III,
		EOS DIGITAL REBEL Xsi / 450D / Kiss
		X2, EOS DIGITAL REBEL XS / 1000D /
		KISS F.
		*/
		err = EdsSendCommand(*camera, kEdsCameraCommand_PressShutterButton
			, kEdsCameraCommand_ShutterButton_Completely);

		//LOGERR("resultShutterBtn:%d", err);
		err = EdsSendCommand(*camera, kEdsCameraCommand_PressShutterButton
			, kEdsCameraCommand_ShutterButton_OFF);
		//LOGERR("resultShutterBtnOf:%d", err);

	}
	////
	// Close session with camera
	if (err == EDS_ERR_OK)
	{
		err = EdsCloseSession(*camera);
		if (err == EDS_ERR_OK)
		{
			LOGEXEC("Canon picture taken");
		}
		else LOGERR("error closing canon session:%d", err);
	}
	else LOGERR("error taking canon picture:%d", err);

	return err;
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
		caller->downloadImage(object);
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
		std::string savepath = outputFolder + "/" + QString("%1").arg(indexPicture, 2, 10, QChar('0')).toStdString() + ".png";//this is 2 digits for the picture name in base 10
		LOGEXEC("output file?: %s", savepath.c_str());
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