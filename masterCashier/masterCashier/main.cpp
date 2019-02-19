#include "masterCashier.h"
#include "windows.h"
#include "dump.h"
#include "appconfig.h"
#include "KillExe.h"
#include "logging.hpp"
#include <QtWidgets/QApplication>

KillExe kill_exe_;

int main(int argc, char *argv[])
{
	EnableAutoDump();
	kill_exe_.KillProcess(_T("masterCashier.exe"));
	Sleep(500);

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/masterCashier/");
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

// 	QString logName = initFilePath + "/log/daily.txt";
// 	dm::server::AppConfigHandler.InitialiseLog(logName.toStdString());
// 	dm::server::AppConfigHandler.Info("Init ClinetLog");

	dir.mkdir(initFilePath + "/Dump");

	QApplication a(argc, argv);
	masterCashier w;

	return a.exec();
}
