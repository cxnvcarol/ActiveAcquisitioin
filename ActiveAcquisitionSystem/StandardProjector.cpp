/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "StandardProjector.h"
using namespace std;

StandardProjector::StandardProjector()
	: imageLabel(new QLabel)
{
	nObservers = 0;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	imageLabel->setBackgroundRole(QPalette::Dark);
	setCentralWidget(imageLabel);
	projectionTimer = new QTimer(this);	
	projectionTimer->setSingleShot(true);
	connect(projectionTimer, SIGNAL(timeout()), this, SLOT(advanceProjectionSequence()));
	
}

StandardProjector::~StandardProjector()
{
	if (playingSequence)
	{
		playingSequence = false;
	}
}

bool StandardProjector::loadAndDisplayImageFile(const QString &fileName)
{
	QImageReader reader(fileName);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	if (newImage.isNull()) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot load %1: %2")
			.arg(QDir::toNativeSeparators(fileName), reader.errorString()));
		return false;
	}

	setImage(newImage);

	return true;
}
void StandardProjector::setCurrentProjection(int projectionIndex)
{
	if (projections.size() > projectionIndex)
	{
		setImage(projections[projectionIndex].image);
	}
}
void StandardProjector::setImage(const QImage &newImage)
{
	image = newImage;
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->adjustSize();
	repaint();
}

void StandardProjector::paintEvent(QPaintEvent * e)
{
	QMainWindow::paintEvent(e);
}

void StandardProjector::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_Escape)
	{
		close();
	}
}

void StandardProjector::playProjectionSequence(int n)
{
	if (!playingSequence)
	{
		LOGEXEC("playProjectionSequence called, repeat for %d times", n);//TODO Fix, n repetitions is ignored. (inf)
		
		playingSequence = true;		
		currentProjectionIndex = 0;
		projectionTimer->start(0);
		//advanceProjectionSequence();//first call starts the following timed projections.
	}
}
void StandardProjector::notifyPlayToObservers()
{
	for (ActiveCamera* ci : observerCams)
	{
		ci->notifyStartProjectionSequence();
	}
}
void StandardProjector::registerCameraObserver(ActiveCamera * cam)
{
	observerCams.push_back(cam);
}

void StandardProjector::loadProjectionSettings(const char* projectionsConfig)
{
	if (projections.empty())
	{
		LOGERR("No projections folder loaded yet");
		return;
	}
	QFile f(projectionsConfig);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		QString line = in.readLine();
		while (line.startsWith("#")) { line = in.readLine(); }
		QStringList  fields = line.split(",");
		f.close();
		if (fields.size() == 3)//"Normal mode" always for the pattern mode
		{
			loadSimpleFormatSettings(projectionsConfig);
		}
		else//then DLP format
		{
			//use standar gral load//TODO Improve: saving a new file shouldn-t be necessary
			QString converted = QString(projectionsConfig).replace(".proj", "-conv.txt");
			dLPToSimpleProjectionsSettings(projectionsConfig, converted);
			loadSimpleFormatSettings(converted.toStdString().c_str());
		}
	}
	else {
		LOGERR("Unable to open configuration file for the projector");
	}
	
}

void StandardProjector::loadSimpleFormatSettings(const char* projectionsConfig)
{
//1. Read text file, split each line, save projection in array.
QFile f(projectionsConfig);
if (f.open(QIODevice::ReadOnly))
{
	QTextStream in(&f);
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		if (line.startsWith("#")) { continue; }
		LOGEXEC("loading %s", line.toStdString().c_str());
		QStringList  fields = line.split(",");
		if (fields.size() != 3)
		{
			LOGERR("incorrect number of arguments for the sequence projection");
			continue;
		}
		//projections.
		int index = indexOfProjection(fields[0]);
		if (index < 0)
		{
			LOGERR("incorrect projection setting in line: %s", line.toStdString().c_str());
			continue;

		}
		bool ok = true;
		long us = fields[1].toLong(&ok);

		if (!ok)
		{
			LOGERR("incorrect projection setting in line: %s", line.toStdString().c_str());
			continue;
		}
		bool camTrigger = fields[2].toInt(&ok);
		if (!ok)
		{
			LOGERR("incorrect projection setting in line: %s", line.toStdString().c_str());
			continue;
		}
		projectionsSequence.push_back({ index,us,camTrigger });
		LOGEXEC("projection load ok");
	}
	f.close();
}
}
void StandardProjector::advanceProjectionSequence()
{
	if (currentProjectionIndex == projectionsSequence.size())
	{
		projectionTimer->stop();
		currentProjectionIndex = 0;

		refresh();
		for (ActiveCamera* ci : observerCams)
		{
			ci->notifyStopProjectionSequence();
		}
		playingSequence = false;
		LOGEXEC("end of projections");
		//TODO Fix here, n repetitions is ignored. (inf)
		return;
	}
	Projection projection = projectionsSequence[currentProjectionIndex];
	
	
	int idx = projectionsSequence[currentProjectionIndex].ProjectedImgIndex;
	LOGEXEC("setting the next projection..%d, index %d", currentProjectionIndex,idx);
	setImage(projections[idx].image);
	
	refresh();

	if (projection.triggerCam)
	{
		for(ActiveCamera* obs: observerCams)
		{
			try {
				int result=obs->takeSinglePicture();
				LOGEXEC("result capturing picture: %d", result);
			}
			catch (...)
			{
				LOGERR("something wrong capturing picture!!");
			}			
		}
	}

	long tms = projectionsSequence[currentProjectionIndex].usTime / 1000;//precision is at best in ms for StandardProjectors.
	currentProjectionIndex++;
	projectionTimer->start(tms);//timer is attached to this method again.
	
}

void StandardProjector :: refresh()
{
	if (parentApp)
	{
		try {
			parentApp->processEvents();
		}
		catch (...) { LOGERR("no qt-events processed"); }

	}
}
void StandardProjector::showInFullProjection()
{
	showFullScreen();
	
	QRect rec = QApplication::desktop()->screenGeometry(screenIndex);
	setGeometry(rec);
}
void StandardProjector::setScreenIndex(int ind) {
	screenIndex = ind;
	dev_id= std::to_string(ind);

	int screenCount = QApplication::desktop()->screenCount();
	if (screenIndex >= screenCount)
	{
		printf("\n[WARNING!] Selected screen %d not available, switching to screen #%d\n", screenIndex, screenCount - 1);
		screenIndex = screenCount - 1;
	}
}
