#include "EDSWrapper.h"
#include "debugMacros.h"

map<int, string> EDSWrapper::errors = { { EDS_ERRORID_MASK,"EDS_ERRORID_MASK" },

{ EDS_CMP_ID_CLIENT_COMPONENTID,"EDS_CMP_ID_CLIENT_COMPONENTID" },
{ EDS_CMP_ID_LLSDK_COMPONENTID,"EDS_CMP_ID_LLSDK_COMPONENTID" },
{ EDS_CMP_ID_HLSDK_COMPONENTID,"EDS_CMP_ID_HLSDK_COMPONENTID" },

{ EDS_ERR_OK,"EDS_ERR_OK" },

{ EDS_ERR_UNIMPLEMENTED,"EDS_ERR_UNIMPLEMENTED" },
{ EDS_ERR_INTERNAL_ERROR,"EDS_ERR_INTERNAL_ERROR" },
{ EDS_ERR_MEM_ALLOC_FAILED,"EDS_ERR_MEM_ALLOC_FAILED" },
{ EDS_ERR_MEM_FREE_FAILED,"EDS_ERR_MEM_FREE_FAILED" },
{ EDS_ERR_OPERATION_CANCELLED,"EDS_ERR_OPERATION_CANCELLED" },
{ EDS_ERR_INCOMPATIBLE_VERSION,"EDS_ERR_INCOMPATIBLE_VERSION" },
{ EDS_ERR_NOT_SUPPORTED,"EDS_ERR_NOT_SUPPORTED" },
{ EDS_ERR_UNEXPECTED_EXCEPTION,"EDS_ERR_UNEXPECTED_EXCEPTION" },
{ EDS_ERR_PROTECTION_VIOLATION,"EDS_ERR_PROTECTION_VIOLATION" },
{ EDS_ERR_MISSING_SUBCOMPONENT,"EDS_ERR_MISSING_SUBCOMPONENT" },
{ EDS_ERR_SELECTION_UNAVAILABLE,"EDS_ERR_SELECTION_UNAVAILABLE" },

{ EDS_ERR_FILE_IO_ERROR,"EDS_ERR_FILE_IO_ERROR" },
{ EDS_ERR_FILE_TOO_MANY_OPEN,"EDS_ERR_FILE_TOO_MANY_OPEN" },
{ EDS_ERR_FILE_NOT_FOUND,"EDS_ERR_FILE_NOT_FOUND" },
{ EDS_ERR_FILE_OPEN_ERROR,"EDS_ERR_FILE_OPEN_ERROR" },
{ EDS_ERR_FILE_CLOSE_ERROR,"EDS_ERR_FILE_CLOSE_ERROR" },
{ EDS_ERR_FILE_SEEK_ERROR,"EDS_ERR_FILE_SEEK_ERROR" },
{ EDS_ERR_FILE_TELL_ERROR,"EDS_ERR_FILE_TELL_ERROR" },
{ EDS_ERR_FILE_READ_ERROR,"EDS_ERR_FILE_READ_ERROR" },
{ EDS_ERR_FILE_WRITE_ERROR,"EDS_ERR_FILE_WRITE_ERROR" },
{ EDS_ERR_FILE_PERMISSION_ERROR,"EDS_ERR_FILE_PERMISSION_ERROR" },
{ EDS_ERR_FILE_DISK_FULL_ERROR,"EDS_ERR_FILE_DISK_FULL_ERROR" },
{ EDS_ERR_FILE_ALREADY_EXISTS,"EDS_ERR_FILE_ALREADY_EXISTS" },
{ EDS_ERR_FILE_FORMAT_UNRECOGNIZED,"EDS_ERR_FILE_FORMAT_UNRECOGNIZED" },
{ EDS_ERR_FILE_DATA_CORRUPT,"EDS_ERR_FILE_DATA_CORRUPT" },
{ EDS_ERR_FILE_NAMING_NA,"EDS_ERR_FILE_NAMING_NA" },

{ EDS_ERR_DIR_NOT_FOUND,"EDS_ERR_DIR_NOT_FOUND" },
{ EDS_ERR_DIR_IO_ERROR,"EDS_ERR_DIR_IO_ERROR" },
{ EDS_ERR_DIR_ENTRY_NOT_FOUND,"EDS_ERR_DIR_ENTRY_NOT_FOUND" },
{ EDS_ERR_DIR_ENTRY_EXISTS,"EDS_ERR_DIR_ENTRY_EXISTS" },
{ EDS_ERR_DIR_NOT_EMPTY,"EDS_ERR_DIR_NOT_EMPTY" },

{ EDS_ERR_PROPERTIES_UNAVAILABLE,"EDS_ERR_PROPERTIES_UNAVAILABLE" },
{ EDS_ERR_PROPERTIES_MISMATCH,"EDS_ERR_PROPERTIES_MISMATCH" },
{ EDS_ERR_PROPERTIES_NOT_LOADED,"EDS_ERR_PROPERTIES_NOT_LOADED" },

{ EDS_ERR_INVALID_PARAMETER,"EDS_ERR_INVALID_PARAMETER" },
{ EDS_ERR_INVALID_HANDLE,"EDS_ERR_INVALID_HANDLE" },
{ EDS_ERR_INVALID_POINTER,"EDS_ERR_INVALID_POINTER" },
{ EDS_ERR_INVALID_INDEX,"EDS_ERR_INVALID_INDEX" },
{ EDS_ERR_INVALID_LENGTH,"EDS_ERR_INVALID_LENGTH" },
{ EDS_ERR_INVALID_FN_POINTER,"EDS_ERR_INVALID_FN_POINTER" },
{ EDS_ERR_INVALID_SORT_FN,"EDS_ERR_INVALID_SORT_FN" },

{ EDS_ERR_DEVICE_NOT_FOUND,"EDS_ERR_DEVICE_NOT_FOUND" },
{ EDS_ERR_DEVICE_BUSY,"EDS_ERR_DEVICE_BUSY" },
{ EDS_ERR_DEVICE_INVALID,"EDS_ERR_DEVICE_INVALID" },
{ EDS_ERR_DEVICE_EMERGENCY,"EDS_ERR_DEVICE_EMERGENCY" },
{ EDS_ERR_DEVICE_MEMORY_FULL,"EDS_ERR_DEVICE_MEMORY_FULL" },
{ EDS_ERR_DEVICE_INTERNAL_ERROR,"EDS_ERR_DEVICE_INTERNAL_ERROR" },
{ EDS_ERR_DEVICE_INVALID_PARAMETER,"EDS_ERR_DEVICE_INVALID_PARAMETER" },
{ EDS_ERR_DEVICE_NO_DISK,"EDS_ERR_DEVICE_NO_DISK" },
{ EDS_ERR_DEVICE_DISK_ERROR,"EDS_ERR_DEVICE_DISK_ERROR" },
{ EDS_ERR_DEVICE_CF_GATE_CHANGED,"EDS_ERR_DEVICE_CF_GATE_CHANGED" },
{ EDS_ERR_DEVICE_DIAL_CHANGED,"EDS_ERR_DEVICE_DIAL_CHANGED" },
{ EDS_ERR_DEVICE_NOT_INSTALLED,"EDS_ERR_DEVICE_NOT_INSTALLED" },
{ EDS_ERR_DEVICE_STAY_AWAKE,"EDS_ERR_DEVICE_STAY_AWAKE" },
{ EDS_ERR_DEVICE_NOT_RELEASED,"EDS_ERR_DEVICE_NOT_RELEASED" },

{ EDS_ERR_STREAM_IO_ERROR,"EDS_ERR_STREAM_IO_ERROR" },
{ EDS_ERR_STREAM_NOT_OPEN,"EDS_ERR_STREAM_NOT_OPEN" },
{ EDS_ERR_STREAM_ALREADY_OPEN,"EDS_ERR_STREAM_ALREADY_OPEN" },
{ EDS_ERR_STREAM_OPEN_ERROR,"EDS_ERR_STREAM_OPEN_ERROR" },
{ EDS_ERR_STREAM_CLOSE_ERROR,"EDS_ERR_STREAM_CLOSE_ERROR" },
{ EDS_ERR_STREAM_SEEK_ERROR,"EDS_ERR_STREAM_SEEK_ERROR" },
{ EDS_ERR_STREAM_TELL_ERROR,"EDS_ERR_STREAM_TELL_ERROR" },
{ EDS_ERR_STREAM_READ_ERROR,"EDS_ERR_STREAM_READ_ERROR" },
{ EDS_ERR_STREAM_WRITE_ERROR,"EDS_ERR_STREAM_WRITE_ERROR" },
{ EDS_ERR_STREAM_PERMISSION_ERROR,"EDS_ERR_STREAM_PERMISSION_ERROR" },
{ EDS_ERR_STREAM_COULDNT_BEGIN_THREAD,"EDS_ERR_STREAM_COULDNT_BEGIN_THREAD" },
{ EDS_ERR_STREAM_BAD_OPTIONS,"EDS_ERR_STREAM_BAD_OPTIONS" },
{ EDS_ERR_STREAM_END_OF_STREAM,"EDS_ERR_STREAM_END_OF_STREAM" },

{ EDS_ERR_COMM_PORT_IS_IN_USE,"EDS_ERR_COMM_PORT_IS_IN_USE" },
{ EDS_ERR_COMM_DISCONNECTED,"EDS_ERR_COMM_DISCONNECTED" },
{ EDS_ERR_COMM_DEVICE_INCOMPATIBLE,"EDS_ERR_COMM_DEVICE_INCOMPATIBLE" },
{ EDS_ERR_COMM_BUFFER_FULL,"EDS_ERR_COMM_BUFFER_FULL" },
{ EDS_ERR_COMM_USB_BUS_ERR,"EDS_ERR_COMM_USB_BUS_ERR" },

{ EDS_ERR_USB_DEVICE_LOCK_ERROR,"EDS_ERR_USB_DEVICE_LOCK_ERROR" },
{ EDS_ERR_USB_DEVICE_UNLOCK_ERROR,"EDS_ERR_USB_DEVICE_UNLOCK_ERROR" },

{ EDS_ERR_STI_UNKNOWN_ERROR,"EDS_ERR_STI_UNKNOWN_ERROR" },
{ EDS_ERR_STI_INTERNAL_ERROR,"EDS_ERR_STI_INTERNAL_ERROR" },
{ EDS_ERR_STI_DEVICE_CREATE_ERROR,"EDS_ERR_STI_DEVICE_CREATE_ERROR" },
{ EDS_ERR_STI_DEVICE_RELEASE_ERROR,"EDS_ERR_STI_DEVICE_RELEASE_ERROR" },
{ EDS_ERR_DEVICE_NOT_LAUNCHED,"EDS_ERR_DEVICE_NOT_LAUNCHED" },

{ EDS_ERR_ENUM_NA,"EDS_ERR_ENUM_NA" },
{ EDS_ERR_INVALID_FN_CALL,"EDS_ERR_INVALID_FN_CALL" },
{ EDS_ERR_HANDLE_NOT_FOUND,"EDS_ERR_HANDLE_NOT_FOUND" },
{ EDS_ERR_INVALID_ID,"EDS_ERR_INVALID_ID" },
{ EDS_ERR_WAIT_TIMEOUT_ERROR,"EDS_ERR_WAIT_TIMEOUT_ERROR" },

{ EDS_ERR_SESSION_NOT_OPEN,"EDS_ERR_SESSION_NOT_OPEN" },
{ EDS_ERR_INVALID_TRANSACTIONID,"EDS_ERR_INVALID_TRANSACTIONID" },
{ EDS_ERR_INCOMPLETE_TRANSFER,"EDS_ERR_INCOMPLETE_TRANSFER" },
{ EDS_ERR_INVALID_STRAGEID,"EDS_ERR_INVALID_STRAGEID" },
{ EDS_ERR_DEVICEPROP_NOT_SUPPORTED,"EDS_ERR_DEVICEPROP_NOT_SUPPORTED" },
{ EDS_ERR_INVALID_OBJECTFORMATCODE,"EDS_ERR_INVALID_OBJECTFORMATCODE" },
{ EDS_ERR_SELF_TEST_FAILED,"EDS_ERR_SELF_TEST_FAILED" },
{ EDS_ERR_PARTIAL_DELETION,"EDS_ERR_PARTIAL_DELETION" },
{ EDS_ERR_SPECIFICATION_BY_FORMAT_UNSUPPORTED,"EDS_ERR_SPECIFICATION_BY_FORMAT_UNSUPPORTED" },
{ EDS_ERR_NO_VALID_OBJECTINFO,"EDS_ERR_NO_VALID_OBJECTINFO" },
{ EDS_ERR_INVALID_CODE_FORMAT,"EDS_ERR_INVALID_CODE_FORMAT" },
{ EDS_ERR_UNKNOWN_VENDOR_CODE,"EDS_ERR_UNKNOWN_VENDOR_CODE" },
{ EDS_ERR_CAPTURE_ALREADY_TERMINATED,"EDS_ERR_CAPTURE_ALREADY_TERMINATED" },
{ EDS_ERR_PTP_DEVICE_BUSY,"EDS_ERR_PTP_DEVICE_BUSY" },
{ EDS_ERR_INVALID_PARENTOBJECT,"EDS_ERR_INVALID_PARENTOBJECT" },
{ EDS_ERR_INVALID_DEVICEPROP_FORMAT,"EDS_ERR_INVALID_DEVICEPROP_FORMAT" },
{ EDS_ERR_INVALID_DEVICEPROP_VALUE,"EDS_ERR_INVALID_DEVICEPROP_VALUE" },
{ EDS_ERR_SESSION_ALREADY_OPEN,"EDS_ERR_SESSION_ALREADY_OPEN" },
{ EDS_ERR_TRANSACTION_CANCELLED,"EDS_ERR_TRANSACTION_CANCELLED" },
{ EDS_ERR_SPECIFICATION_OF_DESTINATION_UNSUPPORTED,"EDS_ERR_SPECIFICATION_OF_DESTINATION_UNSUPPORTED" },
{ EDS_ERR_NOT_CAMERA_SUPPORT_SDK_VERSION,"EDS_ERR_NOT_CAMERA_SUPPORT_SDK_VERSION" },

{ EDS_ERR_UNKNOWN_COMMAND,"EDS_ERR_UNKNOWN_COMMAND" },
{ EDS_ERR_OPERATION_REFUSED,"EDS_ERR_OPERATION_REFUSED" },
{ EDS_ERR_LENS_COVER_CLOSE,"EDS_ERR_LENS_COVER_CLOSE" },
{ EDS_ERR_LOW_BATTERY,"EDS_ERR_LOW_BATTERY" },
{ EDS_ERR_OBJECT_NOTREADY,"EDS_ERR_OBJECT_NOTREADY" },
{ EDS_ERR_CANNOT_MAKE_OBJECT,"EDS_ERR_CANNOT_MAKE_OBJECT" },
{ EDS_ERR_MEMORYSTATUS_NOTREADY,"EDS_ERR_MEMORYSTATUS_NOTREADY" },

{ EDS_ERR_TAKE_PICTURE_AF_NG,"EDS_ERR_TAKE_PICTURE_AF_NG" },
{ EDS_ERR_TAKE_PICTURE_RESERVED,"EDS_ERR_TAKE_PICTURE_RESERVED" },
{ EDS_ERR_TAKE_PICTURE_MIRROR_UP_NG,"EDS_ERR_TAKE_PICTURE_MIRROR_UP_NG" },
{ EDS_ERR_TAKE_PICTURE_SENSOR_CLEANING_NG,"EDS_ERR_TAKE_PICTURE_SENSOR_CLEANING_NG" },
{ EDS_ERR_TAKE_PICTURE_SILENCE_NG,"EDS_ERR_TAKE_PICTURE_SILENCE_NG" },
{ EDS_ERR_TAKE_PICTURE_NO_CARD_NG,"EDS_ERR_TAKE_PICTURE_NO_CARD_NG" },
{ EDS_ERR_TAKE_PICTURE_CARD_NG,"EDS_ERR_TAKE_PICTURE_CARD_NG" },
{ EDS_ERR_TAKE_PICTURE_CARD_PROTECT_NG,"EDS_ERR_TAKE_PICTURE_CARD_PROTECT_NG" },
{ EDS_ERR_TAKE_PICTURE_MOVIE_CROP_NG,"EDS_ERR_TAKE_PICTURE_MOVIE_CROP_NG" },
{ EDS_ERR_TAKE_PICTURE_STROBO_CHARGE_NG,"EDS_ERR_TAKE_PICTURE_STROBO_CHARGE_NG" },
{ EDS_ERR_TAKE_PICTURE_NO_LENS_NG,"EDS_ERR_TAKE_PICTURE_NO_LENS_NG" },
{ EDS_ERR_TAKE_PICTURE_SPECIAL_MOVIE_MODE_NG,"EDS_ERR_TAKE_PICTURE_SPECIAL_MOVIE_MODE_NG" },
{ EDS_ERR_TAKE_PICTURE_LV_REL_PROHIBIT_MODE_NG,"EDS_ERR_TAKE_PICTURE_LV_REL_PROHIBIT_MODE_NG" },

{ EDS_ERR_LAST_GENERIC_ERROR_PLUS_ONE,"EDS_ERR_LAST_GENERIC_ERROR_PLUS_ONE" }
};



EDSWrapper::EDSWrapper()//TODO Review... should I make sure it's a singleton?
{
	EdsError err = EDS_ERR_OK;
	cameraList = NULL;
	countCameras = 0;
	isSDKLoaded = false;
	// Initialize SDK
	err = EdsInitializeSDK();
	if (err == EDS_ERR_OK)
	{
		isSDKLoaded = true;
		updateCameraList();
	}
}
EDSWrapper::~EDSWrapper()
{
	// Terminate SDK
	if (isSDKLoaded)
	{
		EdsTerminateSDK();
		isSDKLoaded = false;
	}
}
void EDSWrapper::sampleRun(EdsCameraRef *currentCamera)
{
	// ! first camera
	EdsError err = EDS_ERR_OK;
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(*currentCamera, kEdsObjectEvent_All, handleObjectEventHERE, this);
		printf("object callback set\n\n");
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetPropertyEventHandler(*currentCamera, kEdsPropertyEvent_All,
			handlePropertyEvent, this);
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetCameraStateEventHandler(*currentCamera, kEdsStateEvent_All,
			handleStateEvent, this);
	}
	fflush(stderr);
	
	printf("\n\n----------------------------------------------------------------------\n\n");
	/*
	EdsOpenSession(*currentCamera);
	err=takeSinglePicture(*currentCamera);
	if (err != EDS_ERR_OK)
	{
		LOGERR("error taking picture!");		
	}
	printf("\nabout to close\n");
	EdsCloseSession(*currentCamera);
	// Release camera
	if (*currentCamera != NULL)
	{
		//EdsRelease(*currentCamera);
	}
	*/
}

EdsError EDSCALLBACK EDSWrapper::handleObjectEventHERE(EdsObjectEvent event,EdsBaseRef object,EdsVoid * context)
{
	LOGEXEC("Objecthandle!!!::%d",event);
	EdsError err = EDS_ERR_OK;
	// do something

	EDSWrapper* caller = (EDSWrapper*)context;

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
EdsError EDSCALLBACK EDSWrapper::handleStateEvent(EdsStateEvent event,
	EdsUInt32 parameter,
	EdsVoid * context)
{
	LOGEXEC("Statehandle::%d", event);
	EdsError err = EDS_ERR_OK;
	// do something
	return err;
}
EdsError EDSCALLBACK EDSWrapper::handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID id,
	EdsUInt32 parameter,
	EdsVoid * context)
{
	//LOGEXEC("propertyhandle::%d", event);
	printf("propHandle;");
	EdsError err = EDS_ERR_OK;
	// do something
	return err;
}
EdsError EDSWrapper::updateCameraList() {

	EdsError err = EDS_ERR_OK;
	// Get camera list
	err = EdsGetCameraList(&cameraList);

	// Get number of cameras
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildCount(cameraList, &countCameras);
	}
	return err;
}

EdsCameraListRef* EDSWrapper::getCameraList()
{
	if (cameraList == NULL)
		updateCameraList();
	return &cameraList;
}

int EDSWrapper::getCameraCount()
{
	if (cameraList == NULL)
	{
		updateCameraList();
	}
	return countCameras;
}
/*
EdsError EDSWrapper::getFirstCamera(EdsCameraRef *camera)
{
	EdsError err = EDS_ERR_OK;
	if (cameraList == NULL)
		updateCameraList();
	
	
	if (countCameras== 0)
	{
		err = EDS_ERR_DEVICE_NOT_FOUND;
	}
	// Get first camera retrieved
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(cameraList, 0, camera);
	}
	// Release camera list
	if (cameraList != NULL)
	{
		EdsRelease(cameraList);
		cameraList = NULL;
	}
	return err;
}
*/

EdsCameraRef * EDSWrapper::getCamera(int index)
{
	EdsCameraRef * camera = (EdsCameraRef*)malloc(sizeof(EdsCameraRef));
	EdsError err = EDS_ERR_OK;
	if (cameraList == NULL)
		updateCameraList();


	if (countCameras == 0)
	{
		err = EDS_ERR_DEVICE_NOT_FOUND;
	}
	
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(cameraList, index, camera);
	}
	// Release camera list//Review, commented from the original. why to release the camera list??
	/*
	if (cameraList != NULL)
	{
		EdsRelease(cameraList);
		cameraList = NULL;
	}
	*/
	if (err != EDS_ERR_OK)
	{
		LOGERR("error getting canon camera");
	}
	return camera;
}

EdsError EDSWrapper::getTv(EdsCameraRef camera, EdsUInt32 *Tv)
{
	EdsError err = EDS_ERR_OK;
	EdsDataType dataType;
	EdsUInt32 dataSize;
	err = EdsGetPropertySize(camera, kEdsPropID_Tv, 0, &dataType, &dataSize);
	if (err == EDS_ERR_OK)
	{
		err = EdsGetPropertyData(camera, kEdsPropID_Tv, 0, dataSize, Tv);
	}
	return err;
}

EdsError EDSWrapper::getTvDesc(EdsCameraRef camera, EdsPropertyDesc *TvDesc)
{
	EdsError err = EDS_ERR_OK;
	err = EdsGetPropertyDesc(camera, kEdsPropID_Tv, TvDesc);
	return err;
}

EdsError EDSWrapper::setTv(EdsCameraRef camera, EdsUInt32 TvValue)
{
	EdsError err = EdsSetPropertyData(camera, kEdsPropID_Tv, 0, sizeof(TvValue), &TvValue);
	return err;
}

EdsError EDSWrapper::downloadImage(EdsDirectoryItemRef directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	err = EdsGetDirectoryItemInfo(directoryItem, &dirItemInfo);
	// Create file stream for transfer destination
	if (err == EDS_ERR_OK)
	{
		char* filename = dirItemInfo.szFileName;//:: Working! saving in local execution path. this function is just for test. See version in CanonCamera using correct path and naming
		printf("\noutput filname?: %s\n",filename);
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
	}
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}

EdsError EDSWrapper::getVolume(EdsCameraRef camera, EdsVolumeRef * volume)
{
	EdsError err = EDS_ERR_OK;
	EdsUInt32 count = 0;
	// Get the number of camera volumes
	err = EdsGetChildCount(camera, &count);
	if (err == EDS_ERR_OK && count == 0)
		err = EDS_ERR_DIR_NOT_FOUND;

	// Get initial volume
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(camera, 0, volume);
	}
	return err;
}

EdsError EDSWrapper::getDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef * directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsDirectoryItemRef dirItem = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	EdsUInt32 count = 0;
	// Get number of items under the volume
	err = EdsGetChildCount(volume, &count);
	if (err == EDS_ERR_OK && count == 0)
	{
		err = EDS_ERR_DIR_NOT_FOUND;
	}
	// Get DCIM folder
	for (int i = 0; i < count && err == EDS_ERR_OK; i++)
	{
		// Get the ith item under the specifed volume
		if (err == EDS_ERR_OK)
		{
			err = EdsGetChildAtIndex(volume, i, &dirItem);
		}
		// Get retrieved item information
		if (err == EDS_ERR_OK)
		{
			err = EdsGetDirectoryItemInfo(dirItem, &dirItemInfo);
		}
		// Indicates whether or not the retrieved item is a DCIM folder.
		if (err == EDS_ERR_OK)
		{
			if (stricmp(dirItemInfo.szFileName, "DCIM") == 0 &&
				dirItemInfo.isFolder == true)
			{
				*directoryItem = dirItem;
				break;
			}
		}
		// Release retrieved item
		if (dirItem)
		{
			EdsRelease(dirItem);
			dirItem = NULL;
		}
	}
	return err;
}

//REVIEW: Deprecated, use the one in CanonCamera. This version of the method is only used for the mainCanonTest (not part of the final solution)
EdsError EDSWrapper::takeSinglePicture(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;

	if (err == EDS_ERR_OK)
	{
		printf("taking pictre!\n");

		
		err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton
			, kEdsCameraCommand_ShutterButton_Completely);

		err = EdsSendCommand(camera, kEdsCameraCommand_PressShutterButton
			, kEdsCameraCommand_ShutterButton_OFF);
		printf("shutterOf!\n");
	}
	if (err != EDS_ERR_OK) {LOGERR("error taking picture:%d", err);}
	
	return err;
}

EdsError EDSWrapper::startLiveview(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	// Get the output device for the live view image
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	// PC live view starts by setting the PC as the output device for the live view image.
	if (err == EDS_ERR_OK)
	{
		device |= kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}
	return err;
	// A property change event notification is issued from the camera if property settings are made successfully.
	// Start downloading of the live view image once the property change notification arrives.
}
EdsError EDSWrapper::downloadEvfData(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;

	EdsStreamRef stream = NULL;
	EdsEvfImageRef evfImage = NULL;
	// Create memory stream.
	err = EdsCreateMemoryStream(0, &stream);
	// Create EvfImageRef.
	if (err == EDS_ERR_OK)
	{
		err = EdsCreateEvfImageRef(stream, &evfImage);
	}
	// Download live view image data.
	if (err == EDS_ERR_OK)
	{
		err = EdsDownloadEvfImage(camera, evfImage);
	}
	// Get the incidental data of the image.
	if (err == EDS_ERR_OK)
	{
		// Get the zoom ratio
		EdsUInt32 zoom;
		EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0, sizeof(zoom), &zoom);
		// Get the focus and zoom border position
		EdsPoint point;
		EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0, sizeof(point), &point);
	}
	//
	// Display image
	//
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	// Release evfImage
	if (evfImage != NULL)
	{
		EdsRelease(evfImage);
		evfImage = NULL;
	}
	return err;
}
EdsError EDSWrapper::endLiveview(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	// Get the output device for the live view image
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	// PC live view ends if the PC is disconnected from the live view image output device.
	if (err == EDS_ERR_OK)
	{
		device &= ~kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}
	return err;
}