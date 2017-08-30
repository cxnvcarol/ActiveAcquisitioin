//Defines the entry point for the console application.
//

#include <QtWidgets/QApplication>
#include "StandardProjector.h"
#include "AcquisitionDeviceManager.h"
#include "ParameterParser.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//WF: 1. Parse parameters
	ParameterParser parser;
	parser.parseParameters(argc, argv);


	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();

	////WF: 2. Initialize & configure desired standard projectors (extra screens)
	
	StandardProjector mainProjector;//important to call in main function (or keep the reference to mainProjector)
	int c = mainProjector.loadProjectionsFolder(parser.projectionsFolder[0].c_str());
	mainProjector.loadProjectionSettings(parser.projectionsConfig[0].c_str());
	mainProjector.setScreenIndex(parser.projectedScreen[0]);
	mainProjector.showInFullProjection();

	////WF: 3. Detect all avt cameras, configure with passed configuration file & prepare for capturing
	//NOTE: It includes only specified cameras if it's explicit by parameter (some can be detected in the network but far away from my experimental setup, therefore: unnecessary bandwith load & resources usage).
	std::vector<ActiveCamera*> cameraList = parser.onlyIncludedAvtSpecified ? mng->detectAllCameras(parser.onlyIncludedAvt) : mng->detectAllCameras();
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
			for (int i = 0;i<cameraList.size();i++)
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
