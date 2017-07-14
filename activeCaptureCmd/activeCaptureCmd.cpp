// activeCaptureCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include ""

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
int main(int argc, char *argv[])
{	
	printf("# args: %d\n", argc);
	for (int i = 1;i < argc;i++)
	{
		printf(argv[i]);
		printf(" ");
	}
	ParamCase paramCase = ParamCase::DEFAULT;
	int countParamCase = 0;
	int countProjections = 0;

	int nCams = 0;
	string cameraConfigXml[5];//max 5 cams
	string projectionsFolder[2];//Path to the folder with pngs to be projected. max 2 projs (projector + mask)
	string projectionsConfig[2];//text files with configuration of time x each projection.
	int projectionScreen[2] = { 1,2 };//screen identfier, 2nd and 3rd screen by default.
	string outputFolder;

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
				countProjections++;
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
				projectionsFolder[countProjections-1] = param;
				break;
			case 1:
				projectionsConfig[countProjections-1] = param;
				break;
			case 2:
				int screenId = (atoi(argv[i]));
				projectionScreen[countProjections-1]=screenId;
			}
			break;
		case ParamCase::OUTPUT_FOLDER:
			outputFolder = param;
			break;
		}
		countParamCase++;

	}
	printf("\n\nEnter to finish");
	
	cin.get();
    return 0;
}

