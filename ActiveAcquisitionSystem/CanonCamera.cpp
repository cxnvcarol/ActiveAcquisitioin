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
	EdsError err = EDS_ERR_OK;
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		//err = EdsSetObjectEventHandler(*camera, kEdsObjectEvent_All, handleObjectEvent, this);//TODO LOOK HERE... fix, refactor to use as independent of wrapper as possible
	}
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

	err = EdsOpenSession(*camera);
	EdsDataType dataType;
	EdsUInt32 dataSize;// = 200;
	err = EdsGetPropertySize(*camera, kEdsPropID_BodyIDEx, 0, &dataType, &dataSize);
	
	if (err == EDS_ERR_OK)
	{
		EdsChar *serial=(EdsChar *)calloc(dataSize,sizeof(EdsChar));
		err = EdsGetPropertyData(*camera, kEdsPropID_BodyIDEx, 0, dataSize, serial);//kEdsDataType_String
		if (err == EDS_ERR_OK)
		{
			printf("\nprinting canon name>>");
			printf(serial);
			dev_id = serial;
			printf("\n\n\n");
		}
		
	}
	else {
		LOGERR("Error reading serial from canon:%d", err);
	}
	
}


CanonCamera::~CanonCamera()
{
	EdsCloseSession(*camera);
	
}
/*
int CanonCamera::takeSinglePicture()
{
	//TODO redirect or clean fn
	return 0;
}
*/
