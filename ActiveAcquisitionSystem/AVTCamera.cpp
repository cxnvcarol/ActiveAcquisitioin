#include "AVTCamera.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>

using namespace AVT::VmbAPI;
using namespace std;

void PrintFtrInfo(const FeaturePtr &fs)
{
	std::string s;
	try {
		fs->GetValue(s);
		printf("Ftr: %s\n", s);
	}
	catch (exception e)
	{
		printf("Ftr failed!");
	}
	
}
AVTCamera::AVTCamera(CameraPtr avtCam)
{
	pCam = avtCam;
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
}
AVTCamera::AVTCamera()
{
}
/* //removed to avoid unitialized objects
AVTCamera::AVTCamera()
{
}
*/
AVTCamera::~AVTCamera()
{
	
}
void AVTCamera::setCameraPtr(CameraPtr avtCam)
{
	pCam = avtCam;
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
		/*//TODO Review!
		err = sys.Shutdown();
		if (VmbErrorSuccess != err)
		{
		ss.str("");
		ss << "Could not shutdown Vimba [error code: " << err << "]";
		std::cout << ss.str() << std::endl;
		}
		*/

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

		/*//TODO Review!
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



	return true;
}

bool AVTCamera::loadSettings()
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
		/*//TODO Review!
		err = sys.Shutdown();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not shutdown Vimba [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		*/

		throw std::exception();
	}

	//  get camera id
	std::string cameraId;
	err = pCam->GetID(cameraId);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not get camera id [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		/*//TODO Review!
		err = sys.Shutdown();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not shutdown Vimba [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		*/

		throw std::exception();
	}

	cameraFlag = true;
	ss.str("");
	ss << "--> Camera with id '" << cameraId << "' has been opened";
	std::cout << ss.str() << std::endl;

	//  create xml file name
	ss.str("");
	ss << cameraId << ".xml";
	std::string xmlFile = ss.str();

	//  -------------------------------------------------------------------------------------
	//  setup load/save settings behaviour:
	//      there are three different ways in VimbaCPP to setup the behaviour for
	//      loading and saving feature values with load/save settings implementation.
	//      SaveCameraSettings() and LoadCameraSettings() can be called either with
	//      an created struct of type 'VmbFeaturePersistSettings_t' or not. The third
	//      alternative is to call LoadSaveSettingsSetup() beforehand and provide the
	//      same parameters as it will be done by the struct.
	//
	//      (1) default usage:
	//          pCam->SaveCameraSettings( xmlFile );
	//          pCam->LoadCameraSettings( xmlFile );
	//
	//      (2) usage with settings struct:
	//          VmbFeaturePersistSettings_t settingsStruct;
	//          settingsStruct.loggingLevel = 4;                        //  set logging level (0:info only, 1: with errors, 2: with warnings, 3: with debug, 4: with traces)
	//          settingsStruct.maxIterations = 5;                       //  since its difficult to catch all feature dependencies during loading multiple
	//  iterations are used (compare desired value with camera value and write it to camera)
	//          settingsStruct.persistType = VmbFeaturePersistNoLUT;    //  set which features shall be persisted (saved to XML):
	//  VmbFeaturePersistAll: all features shall be persisted (including LUTs).
	//  VmbFeaturePersistStreamable: only streamable features shall be persisted.
	//  VmbFeaturePersistNoLUT: all features shall be persisted except for LUT,
	//  which is the recommended setting, because it might be very time consuming.
	//          pCam->SaveCameraSettings( xmlFile, &settingsStruct );
	//          pCam->LoadCameraSettings( xmlFile, &settingsStruct );
	//
	//      (3) usage with setup method:
	//          pCam->LoadSaveSettingsSetup( VmbFeaturePersistNoLUT, 5, 4 );
	//          pCam->SaveCameraSettings( xmlFile );
	//          pCam->LoadCameraSettings( xmlFile );
	//  -------------------------------------------------------------------------------------

	//  call VimbaCPP method for saving all feature values
	err = pCam->SaveCameraSettings(xmlFile);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not save camera settings to file '" << xmlFile << "' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;
		/*//TODO Review!
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
	ss << "--> Feature values have been saved to '" << xmlFile << "'";
	std::cout << ss.str() << std::endl;

	//  get feature selector for user set
	FeaturePtr feature;
	err = pCam->GetFeatureByName("UserSetSelector", feature);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not get feature 'UserSetSelector' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review!
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

	//  set value of selector to 'Default'
	err = feature->SetValue("Default");
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not set value of feature 'UserSetSelector' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review!
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

	//  get feature command 'UserSetLoad'
	err = pCam->GetFeatureByName("UserSetLoad", feature);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not get feature 'UserSetLoad' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review!
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

	//  load selected user set
	err = feature->RunCommand();
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not run command 'UserSetLoad' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review!
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

	std::cout << "--> All feature values have been restored to default" << std::endl;

	//  create settings struct to determine behaviour during loading
	VmbFeaturePersistSettings_t settingsStruct;
	settingsStruct.loggingLevel = 4;
	settingsStruct.maxIterations = 5;
	settingsStruct.persistType = VmbFeaturePersistNoLUT;

	//  re-load saved settings from file
	err = pCam->LoadCameraSettings(xmlFile, &settingsStruct);
	if (VmbErrorSuccess != err)
	{
		ss.str("");
		ss << "Could not load camera settings to file '" << xmlFile << "' [error code: " << err << "]";
		std::cout << ss.str() << std::endl;

		err = pCam->Close();
		if (VmbErrorSuccess != err)
		{
			ss.str("");
			ss << "Could not close camera [error code: " << err << "]";
			std::cout << ss.str() << std::endl;
		}
		cameraFlag = false;

		/*//TODO Review!
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
	ss << "--> Feature values have been loaded from given XML file '" << xmlFile << "'";
	std::cout << ss.str() << std::endl;
	


	return true;
}
