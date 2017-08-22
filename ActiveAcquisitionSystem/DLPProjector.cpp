#include "DLPProjector.h"



DLPProjector::DLPProjector()
{
	//detect serial id and set it.
}


DLPProjector::~DLPProjector()
{
}

void DLPProjector::playProjectionSequence(int n)
{
	//TODO send command to play sequence n times.
}

void DLPProjector::registerCameraObserver(ActiveCamera * cam)
{
	//TODO are cameras triggered by hardware? then do nothing. Add to list if not.
}

void DLPProjector::loadProjectionSettings(const QString projectionsConfig)
{
	//TODO read DLP Config txt convention. later take care of convertion with my own.

	//wf: load images to the dlp and set the sequence play
	//KEY PARAMETERS: # OF BITS, COLOR OF PROJECTION (if pictures are not grayscale I should convert them.
}
