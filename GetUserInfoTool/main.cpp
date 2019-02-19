#include "getuserinfotool.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include "logging.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/GetUserInfo/");
	if (!dir.exists(initFilePath + "/log"))
	{
		if (!dir.mkpath(initFilePath + "/log"))
		{
			return -1;
		}
	}

	QString log_name = "/log.txt";
	QString log_path = initFilePath + "/log" + log_name;
	std::string logpath = log_path.toLocal8Bit().data();
	Log::Initialise(logpath);
	Log::SetThreshold(Log::LOG_TYPE_DEBUG);

	GetUserInfoTool w;
	w.show();
	return a.exec();
}
