#include "CallServerIni.h"

#include <QSettings>
#include <QFile>
#include <QDir>

QString CallServerIniPath = QDir::homePath() + "/AppData/Local/Call-Server/Host-Server.ini";

QString CallServerIni::getServerIP()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("server/ip", "").toString();
}

void CallServerIni::setServerIP(QString serverIP)
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	settings.setValue("server/ip", serverIP);
}

QString CallServerIni::getServerPort()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("server/port", "").toString();
}

void CallServerIni::setServerPort(QString serverPort)
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	settings.setValue("server/port", serverPort);
}

bool CallServerIni::getAutoExecFlag()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("General/isAutoExec", "").toBool();
}

void CallServerIni::setAutoExecFlag(bool isAutoExec)
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	settings.setValue("General/isAutoExec", isAutoExec);
}

int CallServerIni::getCallVoiceValue()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("General/callVoice", 0).toInt();
}

void CallServerIni::setCallVoiceValue(int value)
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	settings.setValue("General/callVoice", value);
}

int CallServerIni::getPlayVoiceValue()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("General/playVoice", 0).toInt();
}

void CallServerIni::setPlayVoiceValue(int value)
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	settings.setValue("General/playVoice", value);
}

QString CallServerIni::getWebPort()
{
	QSettings settings(CallServerIniPath_, QSettings::IniFormat);
	return settings.value("websocket/port", "").toString();
}

void CallServerIni::setIniPath(QString path)
{
	CallServerIniPath_ = path;
}
