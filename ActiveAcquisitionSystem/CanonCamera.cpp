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
	err = EdsCloseSession(*camera);
	
}


CanonCamera::~CanonCamera()
{
	EdsCloseSession(*camera);
	
}
int CanonCamera::prepareCapture()
{
	EdsError err = EDS_ERR_OK;
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(*camera, kEdsObjectEvent_All, handleObjectEvent, this);//TODO LOOK HERE... fix, refactor to use as independent of wrapper as possible
	}
	//EdsError err = EdsOpenSession(*camera);//Open and close only when taking picture!!
	return 0;
}

int CanonCamera::takeSinglePicture()
{
	//TODO redirect or clean fn


	return 0;
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
		
		err = EdsCreateFileStream(dirItemInfo.szFileName,
			kEdsFileCreateDisposition_CreateAlways,
			//kEdsFile_CreateAlways,
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

		//TODO LOOK HERE, HOW TO SAVE THE PICTURE??
		EdsImageRef* outImage;
		EdsCreateImageRef(stream, outImage);
	}
	
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}