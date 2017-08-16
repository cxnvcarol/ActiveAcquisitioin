#include "CanonCamera.h"

CanonCamera::CanonCamera()
{
}

void CanonCamera::setEdsCameraRef(EdsCameraRef *camref)
{
	//TODO LOOK HERE! Initialize other properties: dev_id at least!!
	//EdsError err = EdsGetPropertyData(*camera, kEdsPropID_ProductName, 0, dataSize, Tv);
	EdsDataType dataType;
	EdsUInt32 dataSize;
	EdsError err = EdsGetPropertySize(*camref, kEdsPropID_ProductName, 0, &dataType, &dataSize);
	if (err == EDS_ERR_OK)
	{
		EdsChar *productname=(EdsChar *)calloc(dataSize,sizeof(EdsChar));
		err = EdsGetPropertyData(*camref, kEdsPropID_Tv, 0, dataSize, productname);
		printf("\nprinting canon name>>");
		printf(productname);
		printf("\n\n\n");
	}
}


CanonCamera::~CanonCamera()
{
}

int CanonCamera::takeSinglePicture()
{
	//TODO redirect or clean fn
	return 0;
}
