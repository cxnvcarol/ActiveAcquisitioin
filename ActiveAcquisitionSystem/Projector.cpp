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
	return "";//TODO Identify projector somehow (? serial number? device name?)
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


void Projector::loadProjectionSettings(const QString projectionsConfig)
{
	//1. Read text file, split each line, save projection in array.
	QFile f(projectionsConfig);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line;
		while (!in.atEnd()) {
			line = in.readLine();
			QStringList  fields = line.split(",");
			if (fields.size() != 3)
			{
				throw "incorrect number of arguments for the sequence projection";
			}
			//projections.
			Projection *found;
			//std::find(projections.begin(), projections.end(), found);
			int index = indexOfProjection(fields[0]);
			if (index < 0)
			{
				throw ("incorrect projection setting in line: " + line);
			}
			bool ok;
			long us = fields[1].toLong(&ok);
			if (!ok)
			{
				throw ("incorrect projection setting in line: " + line);
			}
			bool camTrigger = fields[2].toInt(&ok);
			if (!ok)
			{
				throw ("incorrect projection setting in line: " + line);
			}
			projectionsSequence.push_back({index,us,camTrigger});

			
		}
		f.close();
	}

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
		foreach(const QFileInfo &fileinfo, filelistinfo) {
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
