#pragma once
#include <QString>
#include <QNetworkInterface>
#include <QList>

static QString iniPath_;

class appConfigIni
{
public:
	//
	static void setIniPath(QString path);
	//获取设备相关信息
	static QString getHostIpAddress();
	static QString getHostMacAddress();
	static QString getLocalmachineName();
	//获取服务环境类型
	static QString getServerType();
};

