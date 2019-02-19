#include "mainwindow.h"
#include "dump.h"
#include "KillExe.h"
//#include "appconfig.h"

#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include <QApplication>
#include <string>
#include <QDateTime>
#include "logging.hpp"

#pragma comment(lib, "WINMM.LIB")

int main(int argc, char *argv[])
{
	EnableAutoDump();

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/Call-Server/");
	if (!dir.exists(initFilePath + "/log"))
	{
		if (!dir.mkpath(initFilePath + "/log"))
		{
			return -1;
		}
	}

	QString log_name = QString("/log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
	QString log_path = initFilePath + "/log" + log_name;
	std::string logpath = log_path.toLocal8Bit().data();
	Log::Initialise(logpath);
	Log::SetThreshold(Log::LOG_TYPE_DEBUG);

// 	QString logName = QStringLiteral("%1/log/daily.txt").arg(initFilePath);
// 	AppConfigHandler.InitialiseLog(logName.toStdString());
// 	AppConfigHandler.Info("server start!");

	dir.mkdir(initFilePath + "/Dump");
	dir.mkdir(initFilePath + "/Audio");
	//initFilePath.append("/Host-Server.ini");
	if (!dir.exists(initFilePath))
	{
		std::ofstream init_file(initFilePath.toLocal8Bit());
	}

    QApplication a(argc, argv);
    MainWindow w;
	w.resize(480, 580);
	w.show();

    return a.exec();
}
