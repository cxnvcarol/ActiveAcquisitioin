// activeCaptureCmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

enum class ParamCase {
	DEFAULT,
	CAMERA_CONFIGS,
	PROJECTION_CONFIGS,
	OUTPUT_FOLDER
};

using namespace std;

void printHelp()
{
	//TODO Complete here!!
	printf("Help text goes here!");
}
int main(int argc, char *argv[])
{





	
	printf("# args: %d\n", argc);
	ParamCase paramCase = ParamCase::DEFAULT;
	for (char i=1;i<argc;i++)
	{
		switch (paramCase)
		{
		case ParamCase::DEFAULT:
			printf("Wrong arguments");
			printHelp();
			break;

		}

	}
	printf("Press any letter to finish");
	
	cin.get();
    return 0;
}

