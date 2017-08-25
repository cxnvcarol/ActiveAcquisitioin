#include "DLPProjector.h"
#include "Texas/HiresLib/API.h"



DLPProjector::DLPProjector()
{
	//detect serial id and set it.
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
	//TODO read DLP Config txt convention. later take care of convertion with my own.

	//wf: load images to the dlp and set the sequence play
	//KEY PARAMETERS: # OF BITS, COLOR OF PROJECTION (if pictures are not grayscale I should convert them.
}

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
