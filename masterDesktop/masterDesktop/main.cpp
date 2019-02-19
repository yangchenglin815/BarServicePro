#include "masterDesktop.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QTextCodec>
#include <QSettings>
#include <QTranslator>
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
	kill_exe_.KillProcess(_T("masterDesktop.exe"));
	Sleep(500);

#ifdef _WIN32
	SetConsoleOutputCP(65001);
	CONSOLE_FONT_INFOEX info = {0};
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = 16;
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
#endif

	QDir dir;
	QString initFilePath = QDir::cleanPath(QDir::homePath() + "/AppData/Local/masterDesktop/");
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
	QTranslator translator;
	QString path = QApplication::applicationDirPath() + "/masterdesktop_zh.qm";
	translator.load(path);
	a.installTranslator(&translator);

	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();

	masterDesktop w;
	w.move(screenRect.width() - w.width(), 0);
	QPropertyAnimation *an = new QPropertyAnimation(&w, "windowOpacity");
	an->setDuration(1000);
	an->setStartValue(0);
	an->setEndValue(1);
	an->start();
	w.show();

	return a.exec();
}
