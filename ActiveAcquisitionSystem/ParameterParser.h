#pragma once
#include <vector>
#include <string.h>
using namespace std;
class ParameterParser
{
public:
	ParameterParser();
	~ParameterParser();
	void parseParameters(int argc, char *argv[]);


	//Review better practice: encapsulate members by making them private and provide getters and setters.
	int countProjectors;//TODO Review.. separate count btn standard and dlp projectors??
	int countCameraConfigs;
	
	string outputFolder = ".";
	vector<string> cameraConfigXml;//Review, should I limit the # cameras?
	vector<string>  cameraConfigId;
	vector<string> projectionsFolder;//Path to the folder with pngs to be projected. Unlimited # of projectors(gral case) but in practice we have max 2 projs!! (projector + mask)
	vector<string> projectionsConfig;//text files with configuration of time x each projection.
	vector<int> projectedScreen;// = { 1,2 };//screen identfier, 2nd and 3rd screen by default.
private:

	enum class ParamCase {
		DEFAULT,
		CAMERA_CONFIGS,
		PROJECTION_CONFIGS,
		OUTPUT_FOLDER
	};

	
	
};

