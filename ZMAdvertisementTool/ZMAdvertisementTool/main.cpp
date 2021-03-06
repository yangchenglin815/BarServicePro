#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QSettings>
#include <QPropertyAnimation>

#ifdef _WIN32
#include "windows.h"
#endif
#include "dump.h"
#include "KillExe.h"
#include "logging.hpp"

KillExe kill_exe_;

int main(int argc, char *argv[])
{
	EnableAutoDump();
	kill_exe_.KillProcess(_T("ZMAdvertisementTool.exe"));
	Sleep(500);

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/ZMAdvertisementTool/");
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
	MainWindow w;
// 	QPropertyAnimation *an = new QPropertyAnimation(&w, "windowOpacity");
// 	an->setDuration(1000);
// 	an->setStartValue(0);
// 	an->setEndValue(1);
// 	an->start();
// 	w.show();
	return a.exec();
}
