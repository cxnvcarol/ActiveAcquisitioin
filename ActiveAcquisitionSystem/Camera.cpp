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

void ActiveCamera::loadSettings(CameraSettings cs)
{
	throw "Not implemented";
}