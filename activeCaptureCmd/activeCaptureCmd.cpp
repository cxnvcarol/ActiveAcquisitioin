// activeCaptureCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "StandardProjector.h"
#include "AcquisitionDeviceManager.h"
#include "activeCaptureCmd.h"
enum class ParamCase {
	DEFAULT,
	CAMERA_CONFIGS,
	PROJECTION_CONFIGS,
	OUTPUT_FOLDER
};

using namespace std;

static void printHelp()
{
	//TODO Complete helptext here!! Example of config files (inf+1)
	printf("\nUsage:\n\nactiveAcquisition.exe -p [PathToProjectedPictures] [PathToProjectionSettingsFile] [selectedScreen: {0,1,2}, def:1] -c [pathToCameraSettingsFile] -o [ExistingOutputFolder]\n\n");
	printf("\n---ProjectionSettingsFile: text file, each line is as follows: [pictureFileName],[projectedTimeInMicroseconds],[1|0 indicating triggering of connected cameras]\n");
	printf("\n---CameraSettingsFile: xml file for AVT Cameras, the format is the same generated by the VIMBA Viewer proprietary software\n");
		//, "Example:\n\n");
}


int mainAVT(int argc, char *argv[])//SingleAVTCapture
{
	QApplication a(argc, argv);
	printf("# args: %d\n", argc);
	for (int i = 1;i < argc;i++)
	{
		printf(argv[i]);
		printf(" ");
	}
	ParamCase paramCase = ParamCase::DEFAULT;
	int countParamCase = 0;
	int countProjectors = 0;
	int countCameraConfigs = 0;

	int nCams = 0;
	string cameraConfigXml[5];//max 5 cams
	string cameraConfigId[5] = {"","","","",""};//max 5 cams
	string projectionsFolder[2];//Path to the folder with pngs to be projected. max 2 projs (projector + mask)
	string projectionsConfig[2];//text files with configuration of time x each projection.
	int projectionScreen[2] = { 1,2 };//screen identfier, 2nd and 3rd screen by default.
	string outputFolder = ".";


	//WF: 1. Parse parameters
	for (int i = 1;i < argc;i++)
	{
		if (argv[i][0] == '-')
		{
			//param=param.substr(1, param.size());
			if (!_strnicmp("-h", argv[i], 2))
			{
				printHelp();
				break;
			}
			else if (!_strnicmp("-c", argv[i], 2))
			{
				paramCase = ParamCase::CAMERA_CONFIGS;
				countCameraConfigs++;
			}
			else if (!_strnicmp("-p", argv[i], 2))
			{
				paramCase = ParamCase::PROJECTION_CONFIGS;
				countProjectors++;
			}
			else if (!_strnicmp("-o", argv[i], 2))
			{
				paramCase = ParamCase::OUTPUT_FOLDER;
			}
			countParamCase = 0;
			continue;
		}
		string param(argv[i]);

		switch (paramCase)
		{
		case ParamCase::DEFAULT:
			printf("Wrong arguments");
			printHelp();
			break;


		case ParamCase::CAMERA_CONFIGS:
			switch (countParamCase)
			{
			case 0:
				cameraConfigXml[countCameraConfigs - 1] = param;
			case 1:
				cameraConfigId[countCameraConfigs - 1] = param;
			}
			nCams++;
			break;
		case ParamCase::PROJECTION_CONFIGS:
			switch (countParamCase)
			{
			case 0:
				projectionsFolder[countProjectors - 1] = param;
				break;
			case 1:
				projectionsConfig[countProjectors - 1] = param;
				break;
			case 2:
				int screenId = (atoi(argv[i]));
				projectionScreen[countProjectors - 1] = screenId;
			}
			break;
		case ParamCase::OUTPUT_FOLDER:
			outputFolder = param;

			if (!QDir(outputFolder.c_str()).exists())
			{
				QDir().mkdir(outputFolder.c_str());
			}

			break;
		
		}
		countParamCase++;

	}
	
	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();

	//TODO BIG! Support DLP Projector through USB!!

	////WF: 2. Initialize & configure desired projectors (extra screens)
	//TODO Create as many standardProjectors as configured by parameter. countProjectors (?)
	//TODO Use upper projector for general case (or not?) -- hopefully not need, but... https://stackoverflow.com/questions/30150115/cant-cast-superclass-to-subclass
	StandardProjector mainProjector;//important to call in main function (or keep the reference to mainProjector)	
	int c = mainProjector.loadProjectionsFolder(projectionsFolder[0].c_str());
	mainProjector.loadProjectionSettings(projectionsConfig[0].c_str());
	mainProjector.showInFullProjection(projectionScreen[0]);
	
	////WF: 3. Detect all avt cameras, configure with passed configuration file & prepare for capturing
	std::vector<ActiveCamera*> cameraList = mng->detectAllCameras();
	if (cameraList.size() == 0)
	{
		printf("\nno AVT Cameras detected. Enter to continue");
	}
	else {
		for (int i = 0;i < countCameraConfigs;i++)
		{
			if (!cameraConfigId[i].empty())
			{
				string camID = cameraConfigId[i];
				auto it = find_if(cameraList.begin(), cameraList.end(), [&camID](ActiveCamera* obj) {return obj->getDevId() == camID;});

				if (it != cameraList.end())
				{
					bool res = ((ActiveCamera*)*it)->loadSettings(cameraConfigXml[i]);
					res ? printf("xml load succeeded with devid\n") : printf("xml settings failed to load with devid");
				}
			}
		}
		if (countCameraConfigs > 0)
		{
			for(int i=0;i<cameraList.size();i++)
			{
				//WF: 3.1. Load settings
				bool res = cameraList[i]->loadSettings(cameraConfigXml[0]);
				res ? printf("xml load succeeded\n") : printf("xml settings load failed or were already set");
			}
		}
		for (int i = 0;i<cameraList.size();i++)
		{
			//WF: 3.2. Prepare cameras for capturing
			string ouputCam = outputFolder + "/dev_" + cameraList[i]->getDevId();
			if (!QDir(ouputCam.c_str()).exists())
			{
				QDir().mkdir(ouputCam.c_str());
			}
			cameraList[i]->setOutputFolder(ouputCam);
			//VmbErrorType err = cameraList[i]->prepareCapture();

			////WF: 4. register cameras as observers of the master(first) projector (if multiple projectors, one is master, others should be slaves as well!!)
			mainProjector.registerCameraObserver(cameraList[i]);
		}
		//TODO: Register slave projectors as observers of main projector. (Applied when using a second projector as a mask! (DMD Mask!!))
		////WF: 5. Play synchronized projections:
		//TODO: x completeness:: read and use # of repetitions as parameter of the program. (inf+100)
		mainProjector.playProjectionSequence(1);		
	}
	
	//TODO safe termination of program:
	//CMD CLOSE:http://www.cplusplus.com/reference/cstdlib/atexit/
	//QT Window close:https://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
	//int result = 0;
	printf("\n\njust before executing!\n\n");
	int result = a.exec();	
	
	printf("\n\n\nenter to finish!\n\n");
	fflush(stdout);
	cin.get();
	printf("just before delete manager\n\n");
	delete mng;
	return result;
	
}
