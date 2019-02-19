#pragma once
#include <QString>
#include <QNetworkInterface>  
#include <QList>  

static QString iniPath_;

class appConfigIni
{
public:
	//获取服务器IP地址
	static QString getServerIP();
	static void setServerIP(QString serverIP);
	//获取服务器端口
	static QString getServerPort();
	static void setServerPort(QString serverPort);
	//
	static void setIniPath(QString path);
	//获取设备相关信息
	static QString getHostIpAddress();
	static QString getHostMacAddress();
	static QString getLocalmachineName();
	//获取服务环境类型
	static QString getServerType();
	//获取barId
	static QString getBarId();
};

