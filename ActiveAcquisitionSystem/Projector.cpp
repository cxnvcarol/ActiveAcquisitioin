#include "Projector.h"
#include <string>

using namespace std;


Projector::Projector()
{
	playingSequence = false;
}


Projector::~Projector()
{
}
string Projector::getDev_id()
{
	return "";//TODO Identify projector somehow (? serial number? device name? MAC-Address?) (inf-1)
}

int Projector::indexOfProjection(const QString imgName)
{
	for (int i = 0;i < projections.size();i++)
	{
		if (projections[i].name == imgName)
		{
			return i;
		}
	}
	return -1;
}


void Projector::registerCameraObserver(ActiveCamera * cam)
{
	throw "not implemented";
}

void Projector::loadProjectionSettings(const QString projectionsConfig)
{
	throw "not implemented";

}

int Projector::loadProjectionsFolder(const QString & folderPath)
{
	if (!folderPath.isEmpty())
	{
		QDir dir(folderPath);
		QStringList filter;
		filter << QLatin1String("*.png");
		filter << QLatin1String("*.jpeg");
		filter << QLatin1String("*.jpg");
		filter << QLatin1String("*.bmp");
		dir.setNameFilters(filter);
		QFileInfoList filelistinfo = dir.entryInfoList();
		for(const QFileInfo &fileinfo : filelistinfo) {
			QString imageFile = fileinfo.absoluteFilePath();
			QStringList list = imageFile.split(QRegExp("[\\/]+"));

			//imageFile is the image path, just put your load image code here
			QImageReader reader(imageFile);
			QImage newImage = reader.read();
			projections.push_back({ list[list.size() - 1],imageFile,newImage });
		}
	}
	return projections.size();
}
void Projector::playProjectionSequence(int n)
{
	throw "not implemented";
}
