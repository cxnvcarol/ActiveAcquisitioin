#include "ParameterParser.h"
#include "debugMacros.h"
#include <qdir.h>
#include <qstring.h>


static void printHelp()
{
	//TODO Complete helptext here!! Example of config files (inf-10)
	printf("\nUsage:\n\nactiveAcquisition.exe -p [PathToProjectedPictures] [PathToProjectionSettingsFile] [selectedScreenIndex: {0,1,2}, default:1] -c [pathToCameraSettingsFile] -o [ExistingOutputFolder] -i [separatedByCommasAVTIdsToincludeOnly] \n\n");
	printf("\n---ProjectionSettingsFile: text file, each line is as follows: [pictureFileName],[projectedTimeInMicroseconds],[1|0 indicating triggering of connected cameras]\n");
	printf("\n---CameraSettingsFile: xml file for AVT Cameras, the format is the same generated by the VIMBA Viewer proprietary software\n");
	//, "Example:\n\n");
}
ParameterParser::ParameterParser()
{
}


ParameterParser::~ParameterParser()
{
}

void ParameterParser::parseParameters(int argc, char * argv[])
{
	printf("# args: %d\n", argc);
	for (int i = 1;i < argc;i++)
	{
		printf(argv[i]);
		printf(" ");
	}
	ParamCase paramCase = ParamCase::DEFAULT;
	int countParamCase = 0;
	
	countProjectors = 0;
	countCameraConfigs = 0;

	for (int i = 1;i < argc;i++)
	{
		if (argv[i][0] == '-')
		{
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
			else if (!_strnicmp("-i", argv[i], 2))
			{
				paramCase = ParamCase::ONLY_INCLUDE_AVT;
			}
			/*
			else if (!_strnicmp("-s", argv[i], 2))
			{
				paramCase = ParamCase::SYNC_MODE;
			}
			*/
			countParamCase = 0;
			continue;
		}
		string param(argv[i]);

		switch (paramCase)
		{		
			/*
			case ParamCase::SYNC_MODE:
			{
				if (param.find("h") != std::string::npos || param.find("H") != std::string::npos || param == "1")
				{
					hwSyncMode = true;
				}
				else {
					hwSyncMode = false;//default mode: software sync
				}
				break;
			}
			*/
			case ParamCase::CAMERA_CONFIGS:
			{
				switch (countParamCase)
				{
				case 0:
					cameraConfigXml.push_back(param);
				case 1:
					cameraConfigId.push_back(param);
				}
				break;
			}
			case ParamCase::PROJECTION_CONFIGS:
			{
				switch (countParamCase)
				{
				case 0:
					//projectionsFolder[countProjectors - 1] = param;
					projectionsFolder.push_back(param);
					break;
				case 1:
					//projectionsConfig[countProjectors - 1] = param;
					projectionsConfig.push_back(param);
					break;
				case 2:
					//int screenIndex = (atoi(argv[i]));
					//projectedScreen[countProjectors - 1] = screenIndex;
					//projectedScreen.push_back(screenIndex);
					refToProjector.push_back(argv[i]);
				case 3:
					mainProjectorRef = refToProjector[refToProjector.size() - 1];
				}
				break;
			}
			case ParamCase::OUTPUT_FOLDER:
			{
				outputFolder = param;

				if (!QDir(outputFolder.c_str()).exists())
				{
					QDir().mkdir(outputFolder.c_str());
				}

				break;
			}
			case ParamCase::ONLY_INCLUDE_AVT:
			{
				QStringList list = QString(param.c_str()).split(",");
				for (QString avtID : list)
				{
					onlyIncludedAvt.push_back(avtID.toStdString());
				}

				if (!onlyIncludedAvt.empty())
				{
					onlyIncludedAvtSpecified = true;
				}

				break;
			}
			case ParamCase::DEFAULT:
			{
				printf("Wrong arguments");
				printHelp();
				break;
			}		
		}
		countParamCase++;

	}

	if (mainProjectorRef.empty()&&countProjectors>0)
	{
		mainProjectorRef = refToProjector[0];
	}

}
