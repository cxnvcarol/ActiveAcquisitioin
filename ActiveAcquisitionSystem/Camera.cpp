#include "Camera.h"
#include <string>
using namespace std;


ActiveCamera::ActiveCamera(string dev_id)
{
	
}

ActiveCamera::ActiveCamera()
{

}
ActiveCamera::~ActiveCamera()
{
}
int ActiveCamera::takePicture()
{
	throw "Not implemented";
}

bool ActiveCamera::loadSettings(CameraSettings cs)//GRAL CASE (makes more sense for canon & nikon)
{
	throw "Not implemented";
}


bool ActiveCamera::loadSettings(std::string pathToSettings)
{
	throw "Not implemented";
}
