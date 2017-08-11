//TODO define log-macros in external file... use glog instead? (inf-50)
#define LOGEXEC(msg) printf("\n[%d]::%s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)
#define LOGERR(msg) printf("\n[%d]:ERROR!!!:: %s : %d\n", QTime::currentTime().msecsSinceStartOfDay(),(msg), __LINE__)

//

#include <QtWidgets/QApplication>
#include "StandardProjector.h"
#include "AcquisitionDeviceManager.h"
#include <iostream>
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
	printf("\nUsage:\n\nactiveAcquisition.exe -p [PathToProjectedPictures] [PathToProjectionSettingsFile] -c [pathToCameraSettingsFile] -o [ExistingOutputFolder]\n\n");
	printf("\n---ProjectionSettingsFile: text file, each line is as follows: [pictureFileName],[projectedTimeInMicroseconds],[1|0 indicating triggering of connected cameras]\n");
	printf("\n---CameraSettingsFile: xml file for AVT Cameras, the format is the same generated by the VIMBA Viewer proprietary software\n");
	//, "Example:\n\n");
}

int mainNewTest(int argc, char *argv[])
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

	int nCams = 0;
	string cameraConfigXml[5];//max 5 cams
	string projectionsFolder[2];//Path to the folder with pngs to be projected. max 2 projs (projector + mask)
	string projectionsConfig[2];//text files with configuration of time x each projection.
	int projectionScreen[2] = { 1,2 };//screen identfier, 2nd and 3rd screen by default.
	string outputFolder = "./";

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
			cameraConfigXml[countParamCase] = param;
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
			break;
		}
		countParamCase++;

	}

	

	StandardProjector iv;//important to call in main function (or keep the reference to iv)	
	int c = iv.loadProjectionsFolder(projectionsFolder[0].c_str());
	iv.loadProjectionSettings(projectionsConfig[0].c_str());
	iv.setScreen(projectionScreen[0]);

	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();
	std::vector<AVTCamera*> cameraList = mng->detectAVTCameras();
	if (cameraList.size() == 0)
	{
		printf("\nno AVT Cameras detected. Enter to continue");
	}
	else {
		//printf("the cam name: %s\n", cameraList[0]->getName().c_str());

		//////
		//camera related: 1. get first avt detected, load
		bool res = cameraList[0]->loadSettings(cameraConfigXml[0]);
		res ? printf("xml load succeeded\n") : printf("something failed loading camera settings");
		cameraList[0]->setOutputFolder(outputFolder);
		VmbErrorType err = cameraList[0]->prepareCapture();

		if (VmbErrorSuccess != err)
		{
			printf("something wrong preparing the capture");
			return err;
		}

		iv.showInFullProjection();
		iv.playProjectionSequence(1, cameraList[0]);
	}


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