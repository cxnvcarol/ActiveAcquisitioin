#include "DLPProjector.h"
#include "Texas/HiresLib/API.h"



DLPProjector::DLPProjector()
{
	//detect serial id and set it.
	//call setStatus here?
}

DLPProjector::DLPProjector(hid_device * hid)
{
	hidHandle = hid;
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
	//TODO are cameras triggered by hardware? then do nothing. Add to list if not (or add anyway and do the check during AVT Triggering??.
}

void DLPProjector::loadProjectionSettings(const QString projectionsConfig)
{
	//TODO LOOK HERE! Load settings for the dlp: understand QT format and send commands to the DLP! with current settings!!
	//TODO read DLP Config txt convention. later take care of convertion with my own.
	//1. Read text file, split each line, save projection in array.
	QFile f(projectionsConfig);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line = in.readLine();
		QStringList  fields = line.split(",");
		if (fields.size() != 1)//"Normal mode" always for the pattern mode
		{
			return;// TODO LOOK HERE... instead of returning, redirect to the "simple settings parser" and configure with default values!
		}
		while (!in.atEnd()) {
			line = in.readLine();
			fields = line.split(",");
			if (fields.size() != 7)
			{
				LOGERR("ProjectionsSettings file format is not recognized - line: %s", line);
				throw "ProjectionsSettings file format is not recognized";
			}
			//projections.
			int index = indexOfProjection(fields[0]);//TODO read dlp order here!. Save other settings to load later to the DLP
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
			projectionsSequence.push_back({ index,us,camTrigger });
		}
		f.close();
	}


	//wf: load images to the dlp and set the sequence play
	//KEY PARAMETERS: # OF BITS, COLOR OF PROJECTION (if pictures are not grayscale I should convert them.
}
//TODO override method to load folder with pictures!

void DLPProjector::setStatus(int statusLCR, uchar SysStatus, uchar HWStatus, uchar MainStatus)
{
	bool internalMemTest = SysStatus & BIT0;
	bool internalInit = HWStatus & BIT0;
	bool incompatibleASICorDMD = HWStatus & BIT1;
	bool slaveReady = HWStatus & BIT4;
	bool DMDResetWaveformControllerErr = HWStatus & BIT2;
	bool forcedSwapErr = HWStatus & BIT3;
	bool seqAbortStatus = HWStatus & BIT6;
	bool seqErr = HWStatus & BIT7;
	bool DMDParkStatus = MainStatus & BIT0;
	bool sequencerRunState = MainStatus & BIT1;
	bool videoState = !(MainStatus & BIT2);
	bool extSourceLocked = MainStatus & BIT3;
	//TODO Complete, query other properties here
}

bool DLPProjector::dLPToSimpleProjectionsSettings(QString filePathIn, QString filePathOut)
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
		QString line= in.readLine();
		QStringList  fields = line.split(",");
		if (fields.size() != 1)//"Normal mode" always for the pattern mode
		{
			return false;
		}
		while (!in.atEnd()) {
			line = in.readLine();
			fields = line.split(",");
			if (fields.size() != 7)
			{
				return false;
			}
			out << fields[0] << ",";
			out << fields[2] << ",";
			out << fields[6] << "\n";
		}
	}
	outputFile.close();
	f.close();
}

bool DLPProjector::simpleToDLPProjectionsSettings(QString filePathIn, QString filePathOut)
{
	QFile f(filePathIn);
	QFile outputFile(filePathOut);
	if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		LOGERR("Unable to open file for saving: %s",filePathOut.toStdString().c_str());
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
			QStringList  fields = line.split(",");
			if (fields.size() != 3)
			{
				return false;
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

}
