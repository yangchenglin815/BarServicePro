#include "configIni.h"

#include <QSettings>
#include <QFile>

QString configIni::getUserName()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("config/user", "").toString();
}

void configIni::setUserName(QString user)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("config/user", user);
}

QString configIni::getPasswd()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("config/passwd", "").toString();
}

void configIni::setPasswd(QString passwd)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("config/passwd", passwd);
}

QString configIni::getBarId()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("config/barId", "").toString();
}

void configIni::setBarId(QString barId)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("config/barId", barId);
}

QString configIni::getServerIP()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("server/ip", "").toString();
}

void configIni::setServerIP(QString barId)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("server/ip", barId);
}

QString configIni::getServerPort()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("server/port", "").toString();
}

void configIni::setServerPort(QString barId)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("server/port", barId);
}

void configIni::setIniPath(QString path)
{
	iniPath_ = path;
}

QString configIni::getServerType()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/type", "").toString();
}
