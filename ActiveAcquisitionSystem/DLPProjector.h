#pragma once
#include "Projector.h"
class DLPProjector :
	public Projector
{
public:
	DLPProjector();
	~DLPProjector();

	void playProjectionSequence(int n);
	void registerCameraObserver(ActiveCamera * cam);
	void loadProjectionSettings(const QString projectionsConfig);
};

