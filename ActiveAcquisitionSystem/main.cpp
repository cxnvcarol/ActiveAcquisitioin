//Defines the entry point for the console application.
//

#include <QtWidgets/QApplication>
#include "StandardProjector.h"
#include "AcquisitionDeviceManager.h"
#include "ParameterParser.h"
#include <iostream>

using namespace std;
QApplication* a;
ParameterParser parser;
AcquisitionDeviceManager *mng;

bool mngDeleted = false;

void atExitFn()
{
	if (!mngDeleted)
	{
		mngDeleted = true;
		printf("deleting devManager\n\n");
		delete mng;
	}
}

int main(int argc, char *argv[])
{
	atexit(atExitFn);
	a = new QApplication(argc, argv);

	//WF: 1. Parse parameters
	ParameterParser parser;
	parser.parseParameters(argc, argv);

	mng = new AcquisitionDeviceManager();
	if (parser.helpPrinted)
	{

		printf("\n\n\nenter to finish!\n\n");
		fflush(stdout);
		cin.get();
		return 0;
	}
	//WF: 2. Detect devices
	std::vector<ActiveCamera*> cameraList = parser.onlyIncludedAvtSpecified ? mng->detectAllCameras(parser.onlyIncludedAvt) : mng->detectAllCameras();
	int countDLPs=mng->detectDLPs();


	////WF: 2.1 Initialize & configure projectors passed by parameter
	for (int i = 0;i < parser.countProjectors;i++)
	{
		//check if this is the main projector
		
		try {//if Standard projector the ref is an integer:
			int screenIndex = (stoi(parser.refToProjector[i].c_str()));
			StandardProjector* sp = new StandardProjector();
			sp->setScreenIndex(screenIndex);
			sp->loadProjectionsFolder(parser.projectionsFolder[i].c_str());
			sp->loadProjectionSettings(parser.projectionsConfig[i].c_str());
			sp->showInFullProjection();

			mng->addStandardProjector(sp);
		}
		catch (std::exception const & e)
		{
			//if not integer, then is a reference to a dlp projector, or the projector does not exist
			Projector *p = mng->getProjector(parser.refToProjector[i]);
			if (p != NULL)
			{
				p->loadProjectionsFolder(parser.projectionsFolder[i].c_str());
				p->loadProjectionSettings(parser.projectionsConfig[i].c_str());
			}
		}

	}
	Projector *p = mng->getProjector(parser.mainProjectorRef);
	if (p != NULL)
	{
		mng->setMainProjector(p);
	}
	else {
		
		if (parser.refToProjector.empty())
		{
			LOGERR("No projections settings found. Use the option -p");
			printf("\n\n\nenter to finish!\n\n");
			fflush(stdout);
			cin.get();
			return 0;
		}
		else mng->setMainProjector(mng->getProjector(parser.refToProjector[0]));//using first projector as main if not specified in parameters (i.e. 4th field after -p)
	}

	Projector *mainProjector= mng->getMainProjector();

	////WF: 3. Configure and prepare cameras (xml files for avts)
	//NOTE: It includes only specified cameras if it's explicit by parameter (some can be detected in the network but far away from my experimental setup, therefore: unnecessary bandwith load & resources usage).
	
	if (cameraList.size() == 0)
	{
		printf("\nno Cameras detected. Enter to continue");
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
					//WF: 3.1. Load settings for specified camIDs
					bool res = ((ActiveCamera*)*it)->loadSettings(parser.cameraConfigXml[i]);//WF: 3.1. Load settings for specified ids
					if (res)
					{
						printf("xml load succeeded with devid %s\n", ((ActiveCamera*)*it)->getDevId().c_str());
					}
					else printf("xml settings failed to load with devid %s\n", ((ActiveCamera*)*it)->getDevId().c_str());
				}
				else {
					LOGEXEC("Camera with id %s not found", camID.c_str());
				}
			}
		}
		if (countCameraConfigs > 0)
		{
			for (int i = 0;i<cameraList.size();i++)
			{
				//WF: 3.1. Load settings for all others
				bool res = cameraList[i]->loadSettings(parser.cameraConfigXml[0]);
				if (res)
				{
					LOGEXEC("file settings load succeeded with first config file for cam %s", cameraList[i]->getDevId().c_str());
				}
			}
		}
		for (int i = 0;i < cameraList.size();i++)
		{
			//WF: 3.2. Prepare cameras for capturing
			string ouputCam = parser.outputFolder + "/dev_" + cameraList[i]->getDevId();
			if (!QDir(ouputCam.c_str()).exists())
			{
				QDir().mkdir(ouputCam.c_str());
			}
			cameraList[i]->setOutputFolder(ouputCam);
			//VmbErrorType err = cameraList[i]->prepareCapture();

			////WF: 4. register cameras as observers of the main projector (if multiple projectors, is either specified by parameter(postion 3 after -p) or the first in the list)
			
			//Review: should other projectors be on slave-mode? also observers of the main projector or triggered by hardware?
			if (mainProjector != NULL)
			{
				if (mainProjector->triggersByHardware())
				{
					if (cameraList[i]->supportsHardwareTrigger())
					{
						mainProjector->registerCameraObserver(cameraList[i]);
						cameraList[i]->setSyncMode(SyncMode::HARDWARE_MODE);
					}
				}
				else mainProjector->registerCameraObserver(cameraList[i]);
			}			
		}

		////WF: 5. Play synchronized projections:
		//TODO: x completeness:: read and use # of repetitions as parameter of the program. (inf+100)

		if (mainProjector != NULL)
		{
			mainProjector->notifyPlayToObservers();
		}		
		
		//better to have a getAllProjectors().. https://stackoverflow.com/questions/3177241/what-is-the-best-way-to-concatenate-two-vectors
		for (Projector *p : mng->getStandardProjectors())
		{
			p->playProjectionSequence(1);
		}
		for (Projector *p : mng->getDLPs())
		{
			p->playProjectionSequence(1);
		}
	}

	printf("\n\njust before executing!\n\n");
	int result = a->exec();

	printf("\n\n\nenter to finish!\n\n");
	fflush(stdout);
	cin.get();
	return result;

}
