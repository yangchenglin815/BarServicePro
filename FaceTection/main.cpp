#include "faceTection.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <Windows.h>

#include "dump.h"
#include "KillExe.h"
#include "logging.hpp"

KillExe kill_exe_;

int main(int argc, char *argv[])
{
	//init Dump
	EnableAutoDump();
	kill_exe_.KillProcess(_T("FaceTection.exe"));
	Sleep(500);

	//init Log
	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/FaceTection/");
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
	dir.mkdir(initFilePath + "/Dump");

	QApplication a(argc, argv);
	FaceTection w;

	return a.exec();
}
