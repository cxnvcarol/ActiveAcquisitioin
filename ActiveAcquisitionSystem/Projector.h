#pragma once
#include <string>
#include <vector>
#include <QtWidgets>
#include <QImage>
#include "Camera.h"
#include "debugMacros.h"


struct ProjectedImg {
	QString name;
	QString fullPath;
	QImage image;
} ;

struct Projection {
	int ProjectedImgIndex;
	long usTime;
	bool triggerCam;
};

class Projector
{
public:
	Projector();
	~Projector();

	std::string getDev_id();
	int indexOfProjection(const QString imgName);
	std::vector<Projection> getPprojectionsSequence() {
		return projectionsSequence;
	}

	int loadProjectionsFolder(const QString &folderPath);

	virtual void registerCameraObserver(ActiveCamera * cam);
	virtual void loadProjectionSettings(const QString projectionsConfig);//Loading custom settings format so far.//TODO Examine previous DLP Format!

	virtual void playProjectionSequence(int n);//plays already loaded sequence
protected:
	std::vector<ProjectedImg> projections;
	std::vector<Projection> projectionsSequence;
	bool playingSequence;

	std::vector<ActiveCamera *> observerCams;
	int nObservers;
	


};

