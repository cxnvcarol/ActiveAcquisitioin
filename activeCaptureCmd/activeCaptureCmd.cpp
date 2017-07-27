// activeCaptureCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "StandardProjector.h"
//#include "AVTCamera.h"
#include "AcquisitionDeviceManager.h"
#include "activeCaptureCmd.h"
//todo check and use vector<> instead of pointer arrays.
enum class ParamCase {
	DEFAULT,
	CAMERA_CONFIGS,
	PROJECTION_CONFIGS,
	OUTPUT_FOLDER
};

using namespace std;

static void printHelp()
{
	//TODO Complete here!!
	printf("Help text goes here!");
}

void initActiveCapture(int nCamsToConfigure, string cameraXmls[], int nProjectors, string projectionsFolder[], string projectionsConfig[],
	int projectionScreen[], string outputFolder)
{
	//Procedure:
	/*
	1. Initialize selected APIS (vimba, no need x canon/nikon/dlp)
	2. select upto nCams vimba from identified list
	3. load xmls to vimba
	
	In parallel (?)
	2. identify  nProjectors < # of screens
	3. Load pictures x each projectionsFolder
	4. Load Projections config. to ProjectorClass - get total projection time.

	then:

	5. set #pictures to capture in AVT Cams to match longest projection total time + alpha.
	6. avts -> start capturing & start projecting
	7. transfer captured pictures to output folder
	8. write xml output with capturing data (compliant with 3digify format!)
	
	*/
	//1.


	/*
	//The following is working fine so far.
	*/
	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();//API Inintialization is into the constructor
	mng->detectCameras();

	int nCams = nCamsToConfigure > mng->getCountCameras() ?mng->getCountCameras() : nCamsToConfigure;


	printf("There are %d detected cams and I'm trying to load %d xml files", mng->getCountCameras(), nCamsToConfigure);

	for (int i = 0;i < nCams;i++)
	{
		AVTCamera *ac = mng->getCamera(0);
		bool result = ac->loadSettings(cameraXmls[i]);
		printf("loading for cam # %d: %s\n", i, result ? "true" : "false");
	}
}

int main(int argc, char *argv[])
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
	string outputFolder="./";

	for (int i=1;i<argc;i++)
	{		
		if (argv[i][0] == '-')
		{
			//param=param.substr(1, param.size());
			if (!_strnicmp("-h", argv[i],2))
			{
				printHelp();
				break;
			}
			else if (!_strnicmp("-c", argv[i],2))
			{
				paramCase = ParamCase::CAMERA_CONFIGS;
			}
			else if (!_strnicmp("-p", argv[i],2))
			{
				paramCase = ParamCase::PROJECTION_CONFIGS;
				countProjectors++;
			}
			else if (!_strnicmp("-o", argv[i],2))
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
				projectionsFolder[countProjectors-1] = param;
				break;
			case 1:
				projectionsConfig[countProjectors-1] = param;
				break;
			case 2:
				int screenId = (atoi(argv[i]));
				projectionScreen[countProjectors-1]=screenId;
			}
			break;
		case ParamCase::OUTPUT_FOLDER:
			outputFolder = param;
			break;
		}
		countParamCase++;

	}

	//initActiveCapture(nCams, cameraConfigXml, countProjectors, projectionsFolder, projectionsConfig,projectionScreen,outputFolder);//TODO Never worked nicely
		

	StandardProjector iv;//important to call in main function (or keep the reference to iv)	
	int c=iv.loadProjectionsFolder("C:\\Users\\naranjo\\Pictures");
	iv.loadProjectionSettings(projectionsConfig[0].c_str());
	
	AcquisitionDeviceManager *mng = new AcquisitionDeviceManager();
	std::vector<AVTCamera*> cameraList=mng->detectAVTCameras();
	if (cameraList.size() == 0)
	{
		printf("\nno AVT Cameras detected. Enter to continue");
		
		/*
		delete mng;
		cin.get();		
		return 0;
		*/
	}
	else {
		printf("the cam name: %s\n", cameraList[0]->getName().c_str());

		//////

		cameraList[0]->loadSettings(cameraConfigXml[0]);
		AVT::VmbAPI::CameraPtr cam = cameraList[0]->getAVTPointer();
		VmbErrorType err = cam->Open(VmbAccessModeFull);
		if (vimbaError)
		{
			printf("Alles kaput, close and try again X/");
			cin.get();
			return -1;
		}
		//TODO 2. wait for camready & capture picture!!
		//camera related: 1. get first avt detected, load
		printf("xml load succeeded\n");
		err = cam->Close();
	}
	

	
	iv.showInFullProjection();
	iv.playProjectionSequence(1);//play sequence n times //TODO Send ref. to camera to trigger capture.
	int result = a.exec();
	delete mng;
	return result;
}