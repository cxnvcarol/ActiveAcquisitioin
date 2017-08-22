/**
EDS Canon DLSR cameras
**/
#include "EDSDK\EDSDK.h"
#include <map>

#include "Camera.h"
using namespace std;
class EDSWrapper
{
public:
	EDSWrapper();
	~EDSWrapper();

	static map<int, string> errors;

	static EdsError EDSCALLBACK handleObjectEventHERE(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
	static EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event, EdsUInt32 evData, EdsVoid * context);
	//static EdsError EDSCALLBACK handleStateEvent(EdsPropertyEvent event, EdsPropertyID property, EdsUInt32 parameter, EdsVoid * context);
	static EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID id, EdsUInt32 parameter, EdsVoid * context);
	EdsCameraRef* getCamera(int index);
	
	EdsError updateCameraList();
	EdsCameraListRef* getCameraList();
	int getCameraCount();
	void sampleRun(EdsCameraRef *c);

//the following could be static??
	EdsError getTv(EdsCameraRef camera, EdsUInt32 *Tv);
	EdsError getTvDesc(EdsCameraRef camera, EdsPropertyDesc *TvDesc);
	EdsError setTv(EdsCameraRef camera, EdsUInt32 TvValue);
	EdsError getVolume(EdsCameraRef camera, EdsVolumeRef * volume);
	EdsError getDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef * directoryItem);
	EdsError takeSinglePicture(EdsCameraRef camera);
	EdsError startLiveview(EdsCameraRef camera);
	EdsError downloadEvfData(EdsCameraRef camera);
	EdsError endLiveview(EdsCameraRef camera);

private:
	EdsCameraListRef cameraList;
	bool isSDKLoaded;
	EdsUInt32 countCameras;

	EdsError downloadImage(EdsDirectoryItemRef directoryItem);

	

	
	
};

