#include "masterlocker.h"
#include <QtWidgets/QApplication>

#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>
#include <QDateTime>
#include <iostream>
#include <string>
#include "dump.h"
#include "KillExe.h"
#include "logging.hpp"

#pragma comment(lib, "WINMM.LIB")

int main(int argc, char *argv[])
{
	EnableAutoDump();

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/masterLocker/");
	if (!dir.exists(initFilePath + "/log"))
	{
		if (!dir.mkpath(initFilePath + "/log"))
		{
			return -1;
		}
	}

	QString logName = initFilePath + "/log/daily_" + QDateTime::currentDateTime().toString("yyyyMMddhhmm");
	logName += ".txt";
	Log::Initialise(logName.toStdString().c_str());
	Log::SetThreshold(Log::LOG_TYPE_DEBUG);

	dir.mkdir(initFilePath + "/Dump");

	QApplication a(argc, argv);
	masterLocker w;
	return a.exec();
}
