#include "DLPProjector.h"
#include "Texas/HiresLib/API.h"



DLPProjector::DLPProjector()
{
	//detect serial id and set it.
	//call setStatus here?
}

DLPProjector::DLPProjector(hid_device * hid, std::string path):m_patternImageChange(false)
{
	hidHandle = hid;
	pathHid = path;
}



DLPProjector::~DLPProjector()
{
}

void DLPProjector::playProjectionSequence(int n)
{
	unsigned int repeat;//TODO review this. How to use it? replace with n param??
	/*
	if (ui->repeat_radioButton->isChecked())
		repeat = 0;
	else
		*/
		repeat = dlp_pattern_elements.size();

	if (LCR_SetPatternConfig(dlp_pattern_elements.size(), repeat,hidHandle)<0)
	{
		LOGERR("Error in setting LUT Configuration!");
		return;
	}
	if (LCR_PatternDisplay(0x2,hidHandle) < 0)
		LOGERR("Unable to stat pattern display");
}

void DLPProjector::registerCameraObserver(ActiveCamera * cam)
{
	//TODO are cameras triggered by hardware? then do nothing. Add to list if not (or add anyway and do the check during AVT Triggering??.
}
void DLPProjector::loadDLPProjectionsSettings(const QString projectionsConfig)
{
	QFile settingsFile(projectionsConfig);
	if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		LOGERR("Unable to open the settings file");
		return;
	}

	//first clear the existing patterns on window

	if (dlp_pattern_elements.size() > 0)
		dlp_pattern_elements.clear();

	QDir dir = QFileInfo(settingsFile).absoluteDir();
	QString m_ptnSettingPath = dir.absolutePath();
	

	QTextStream in(&settingsFile);

	QString line;

	
	line = in.readLine();

	while (!in.atEnd())
	{
		PatternElement pattern;

		line = in.readLine();

		if (line == "\n" || line == "") //error checking for empty lines
			continue;

		QStringList list = line.split(",");

		if (list.size() == 0)
			continue;

		pattern.name = list[0];

		QString pFileStr = m_ptnSettingPath + "/" + list[0];

		QFile patternFile(pFileStr);

		//check if the pattern image file exists
		if (!patternFile.exists())
		{

			LOGERR("the pattern image file %s does not exist", pFileStr.toStdString().c_str());

			return;
		}


		//backward capability for adding from List
		//where only the image file is given

		pattern.name = pFileStr;
		pattern.bits = (list.size() >= 2) ? list[1].toInt() : 1;
		pattern.exposure = list[2].toInt();//pattern.exposure = (list.size() >= 3) ? list[2].toInt() : GetMinExposure(1);//TODO VERIFY THIS!, check GetMinExposure
		pattern.darkPeriod = (list.size() >= 4) ? list[3].toInt() : 0;
		pattern.color = (list.size() >= 5) ? PatternElement::Color(list[4].toInt()) : PatternElement::RED;
		pattern.trigIn = (list.size() >= 6) ? list[5].toInt() : false;
		pattern.trigOut2 = (list.size() >= 7) ? list[6].toInt() : true;

		dlp_pattern_elements.push_back(pattern);
		m_patternImageChange = true;

	}
	
	settingsFile.close();
}
void DLPProjector::loadProjectionSettings(const QString projectionsConfig)
{
	QFile f(projectionsConfig);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line = in.readLine();
		QStringList  fields = line.split(",");
		f.close();
		if (fields.size() == 1)//"Normal mode" always for the pattern mode
		{
			loadDLPProjectionsSettings(projectionsConfig);//TODO Review... should I update my own Projections vector anyway?? (certainly yes to use as hdmi eventually)
			if (LCR_SetMode(0x3,hidHandle) < 0)
			{
				LOGERR("Unable to switch to pattern from Memory mode");
				return;
			}
			updateLUT();
		}
		else if (fields.size() == 3)
		{
			Projector::loadProjectionSettings(projectionsConfig);//use standar gral load

		}
		else{
			LOGERR("Configuration file format not recognized");
		}
	}
	else {
		LOGERR("Unable to open configuration file for the projector");
	}

}
//TODO Review: override method to load folder with pictures!... or not? - for memory pattern mode, so the LUT load doesn't take that long!

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
	return true;
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
	return true;
}

void DLPProjector::updateLUT()
{
	int totalSplashImages = 0;
	int ret;
	QTime waitEndTime;
	char errStr[255];

	if (dlp_pattern_elements.size() <= 0)
	{
		LOGERR("No pattern sequence to send");
		return;
	}

	LCR_ClearPatLut();

	/*
	if (!m_videoPatternMode)
	{
		if (calculateSplashImageDetails(&totalSplashImages))
			return;
	}
	*/

	for (int i = 0; i < dlp_pattern_elements.size(); i++)
	{
		if (LCR_AddToPatLut(i, dlp_pattern_elements[i].exposure, true, dlp_pattern_elements[i].bits, dlp_pattern_elements[i].color, dlp_pattern_elements[i].trigIn, dlp_pattern_elements[i].darkPeriod, dlp_pattern_elements[i].trigOut2, dlp_pattern_elements[i].splashImageIndex, dlp_pattern_elements[i].splashImageBitPos)<0)
		{
			LOGERR( "Unable to add pattern number %d to the LUT", i);
			
			break;
		}
	}

	if (LCR_SendPatLut(hidHandle) < 0)
	{
		LOGERR("Sending pattern LUT failed!");
		return;
	}

	/*//redo
	if (ui->repeat_radioButton->isChecked())
		ret = LCR_SetPatternConfig(dlp_pattern_elements.size(), 0);
	else
		*/
	ret = LCR_SetPatternConfig(dlp_pattern_elements.size(), dlp_pattern_elements.size(),hidHandle);
	if (ret < 0)
	{
		LOGERR("Sending pattern LUT size failed!");
		return;
	}

	//review. NO SPLASH IMAGES ARE CONSIDERED SO FAR. patternMemory_radioButton=on the fly mode, but splash imgs are 0!
	/*if (ui->patternMemory_radioButton->isChecked() && m_patternImageChange)
	{
		if (updatePatternMemory(totalSplashImages, false) == 0)
		{
			m_patternImageChange = false;
		}
	}
	*/

}
