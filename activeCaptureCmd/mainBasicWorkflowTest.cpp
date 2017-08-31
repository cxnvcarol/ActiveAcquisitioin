//Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "StandardProjector.h"
#include "AcquisitionDeviceManager.h"
//#include "activeCaptureCmd.h"
#include "ParameterParser.h"

using namespace std;


/*
Use case 1: Active acquisition using standard projectors, canond and AVT cameras, synchronized by software.

Params:
1. (optional) configuration xml files for the AVT cameras with (optional) corresponding ID
- If xml config is passed without corresponding identifier, the configuration is used for all identified cameras not configured explicitely otherwise.

2. projections configuration for each projector (A projector is treated as a standard screen - VGA, HDMI or Displayport)
- Maximum of 2 projectors supported (to simulate Projector and Mask roles)
- The first configured projector behaves as master triggering the cameras.
- If configured, the second projector performs asynchrounsly without communication with the cameras or the first projector (i.e., 3rd column of outputTrigger is ignored.)

WORKFLOW:
1. Identify cameras and configure them according to parameters. Except:
- AcquisitionMode is forced to SingleFrame
- TriggerInput is off
- StreamBytesPerSecond is limited to a maximum of 1GBPS/#AVTConnected (Assuming always a GigE ethernet card available on the PC)

2. Load projections folder and setup sequence projections according to parameters.

3. Run synchronized acquisition: When

*/
int mainUseCase1(int argc, char *argv[])//test with one standard projector and mixed multiple cameras.
{
	QApplication a(argc, argv);

	//WF: 1. Parse parameters
	ParameterParser parser;
	parser.parseParameters(argc, argv);


	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();
	
	////WF: 2. Initialize & configure desired projectors (extra screens)
	
	StandardProjector mainProjector;//important to call in main function (or keep the reference to mainProjector)
	int c = mainProjector.loadProjectionsFolder(parser.projectionsFolder[0].c_str());
	mainProjector.loadProjectionSettings(parser.projectionsConfig[0].c_str());
	mainProjector.setScreenIndex(atoi(parser.refToProjector[0].c_str()));
	mainProjector.showInFullProjection();
	
	////WF: 3. Detect all avt cameras, configure with passed configuration file & prepare for capturing
	//NOTE: It includes only specified cameras if it's explicit by parameter (some can be detected in the network but far away from my experimental setup, therefore: unnecessary bandwith load & resources usage).
	std::vector<ActiveCamera*> cameraList = parser.onlyIncludedAvtSpecified ? mng->detectAllCameras(parser.onlyIncludedAvt): mng->detectAllCameras();
	if (cameraList.size() == 0)
	{
		printf("\nno AVT Cameras detected. Enter to continue");
	}
	else {
		int countCameraConfigs = parser.countCameraConfigs;
		for (int i = 0;i < countCameraConfigs;i++)
		{
			if (!parser.cameraConfigId[i].empty())
			{
				string camID = parser.cameraConfigId[i];
				auto it = find_if(cameraList.begin(), cameraList.end(), [&camID](ActiveCamera* obj) {return obj->getDevId() == camID;});

				if (it != cameraList.end())
				{
					bool res = ((ActiveCamera*)*it)->loadSettings(parser.cameraConfigXml[i]);
					res ? printf("xml load succeeded with devid\n") : printf("xml settings failed to load with devid");
				}
			}
		}
		if (countCameraConfigs > 0)
		{
			for(int i=0;i<cameraList.size();i++)
			{
				//WF: 3.1. Load settings
				bool res = cameraList[i]->loadSettings(parser.cameraConfigXml[0]);
				res ? printf("xml load succeeded\n") : printf("xml settings load failed or were already set");
			}
		}
		for (int i = 0;i<cameraList.size();i++)
		{
			//WF: 3.2. Prepare cameras for capturing
			string ouputCam = parser.outputFolder + "/dev_" + cameraList[i]->getDevId();
			if (!QDir(ouputCam.c_str()).exists())
			{
				QDir().mkdir(ouputCam.c_str());
			}
			cameraList[i]->setOutputFolder(ouputCam);
			//VmbErrorType err = cameraList[i]->prepareCapture();

			////WF: 4. register cameras as observers of the master(first) projector (if multiple projectors, one is master, others should be slaves as well!!)
			mainProjector.registerCameraObserver(cameraList[i]);
		}
		
		////WF: 5. Play synchronized projections:
		//TODO: x completeness:: read and use # of repetitions as parameter of the program. (inf+100)
		mainProjector.playProjectionSequence(1);		
	}
	
	printf("\n\njust before executing!\n\n");
	int result = a.exec();	
	
	printf("\n\n\nenter to finish!\n\n");
	fflush(stdout);
	cin.get();
	printf("just before delete manager\n\n");
	delete mng;
	return result;
	
}
