#pragma once
#include <string>
#include <vector>
#include <QtWidgets>
#include <QImage>

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
	void loadProjectionSettings(const QString projectionsConfig);
	int loadProjectionsFolder(const QString &folderPath);

	virtual void playProjectionSequence();//play already loaded sequence

protected:
	std::vector<ProjectedImg> projections;
	std::vector<Projection> sequenceProjections;





};

