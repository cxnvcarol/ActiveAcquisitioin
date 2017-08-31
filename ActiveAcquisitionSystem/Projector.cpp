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
	return dev_id;
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

void Projector::loadProjectionSettings(const char* projectionsConfig)
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

void Projector::notifyPlayToObservers()
{
	LOGERR("Not implemented");
}

bool Projector::triggersByHardware()
{
	return false;//by default
}


bool Projector::dLPToSimpleProjectionsSettings(QString filePathIn, QString filePathOut)
{
	QFile f(filePathIn);
	QFile outputFile(filePathOut);
	if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		LOGERR("Unable to open file for saving: %s", filePathOut.toStdString().c_str());
		return false;
	}
	QTextStream out(&outputFile);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line = in.readLine();
		while (line.startsWith("#")) { line = in.readLine(); }
		QStringList  fields = line.split(",");
		if (fields.size() != 1)//"Normal mode" always for the pattern mode
		{
			return false;
		}
		while (!in.atEnd()) {
			line = in.readLine();
			if (line.startsWith("#")) { continue; }
			fields = line.split(",");
			if (fields.size() != 7)
			{
				LOGERR("expecting 7 comma-separated values");
				continue;
			}
			out << fields[0] << ",";
			out << fields[2] << ",";
			out << fields[6] << "\n";
		}
	}
	outputFile.close();
	f.close();
	return true;
}

bool Projector::simpleToDLPProjectionsSettings(QString filePathIn, QString filePathOut)
{
	QFile f(filePathIn);
	QFile outputFile(filePathOut);
	if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		LOGERR("Unable to open file for saving: %s", filePathOut.toStdString().c_str());
		return false;
	}
	QTextStream out(&outputFile);
	out << "Normal Mode\n";
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line;

		while (!in.atEnd()) {
			line = in.readLine();
			if (line.startsWith("#")) { continue; }
			QStringList  fields = line.split(",");
			if (fields.size() != 3)
			{
				LOGERR("expecting 3 comma-separated values");
				continue;
			}
			out << fields[0] << ",";
			out << "8" << ",";
			out << fields[1] << ",";
			out << "0" << ",";
			out << "7" << ",";
			out << "0" << ",";
			out << fields[2] << "\n";
		}
	}
	outputFile.close();
	f.close();
	return true;
}
