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


StandardProjector::StandardProjector()
	: imageLabel(new QLabel)
	, scrollArea(new QScrollArea)
{
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setVisible(false);
	setCentralWidget(scrollArea);

	resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
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

	scrollArea->setVisible(true);
	imageLabel->adjustSize();
}
/*
static void doPlay( int n)
{
	for (int i = 0;i < n;i++)
	{
		printf("i got the number %d\n", n);
		fflush(stdout);
		Sleep(500);
	}
}
*/

void playInThread(StandardProjector* pr,int n)
{
	//pr->playProjectionSequence(n);
	for (int i = 0;i < n;i++)
	{
		//Here play the right sequence!
		//no uS supported
		std::vector<Projection> seq = pr->getPprojectionsSequence();
		for (int j = 0;j < seq.size();j++)
		{
			Projection proj = seq[j];
			
			//bool trigger = pr.triggerCam;
			//TODO Read and use trigger condition!
			pr->setCurrentProjection(proj.ProjectedImgIndex);
			Sleep(proj.usTime / 1000);
		}

	}
	pr->playingSequence = false;
}
void StandardProjector::playProjectionSequence(int n)
{
	if (!playingSequence)
	{
		printf("playProjectionSequence called, repeat for %d times\n", n);
		playingSequence = true;
		std::thread theThread(playInThread, this, n);
		theThread.detach();
		return;
	}
}


void StandardProjector::fitToWindow()
{
	scrollArea->setWidgetResizable(true);
}

void StandardProjector::showInFullProjection()
{
	showFullScreen();
	int screenCount = QApplication::desktop()->screenCount();
	QRect rec = QApplication::desktop()->screenGeometry(0);
	QRect rec2 = QApplication::desktop()->screenGeometry(1);
	int x = rec.center().rx();
	int x2 = rec2.center().rx();
	//TODO Select rec according to the right-est x coordinate, or according to the parameter in the constructor (pass it!)
	setGeometry(rec);

	//TODO Show black at first!
	this->fitToWindow();
}