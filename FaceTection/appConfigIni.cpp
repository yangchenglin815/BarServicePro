#include "appConfigIni.h"

#include <QSettings>
#include <QFile>
#include <QDir>
#include <QHostInfo>

QString appConfigIni::getServerIP()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("server/ip", "").toString();
}

void appConfigIni::setServerIP(QString serverIP)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("server/ip", serverIP);
}

QString appConfigIni::getServerPort()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("server/port", "").toString();
}

void appConfigIni::setServerPort(QString serverPort)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("server/port", serverPort);
}

QString appConfigIni::getUserName()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/user", "").toString();
}

void appConfigIni::setUserName(QString nUser)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("client/user", nUser);
}

QString appConfigIni::getUserId()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/userId", "").toString();
}

void appConfigIni::setUserId(QString nUserId)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("client/userId", nUserId);
}

QString appConfigIni::getParentId()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/parentId", "").toString();
}

void appConfigIni::setParentId(QString parentId)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("client/parentId", parentId);
}

void appConfigIni::setIniPath(QString path)
{
	iniPath_ = path;
}

QString appConfigIni::getHostIpAddress()
{
	QString strIpAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// 获取第一个本主机的IPv4地址  
	int nListSize = ipAddressesList.size();
	for (int i = 0; i < nListSize; ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) {
			strIpAddress = ipAddressesList.at(i).toString();
			break;
		}
	}
	// 如果没有找到，则以本地IP地址为IP  
	if (strIpAddress.isEmpty())
		strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
	return strIpAddress;
}

QString appConfigIni::getHostMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表  
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址  
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

QString appConfigIni::getLocalmachineName()
{
	QString machineName = QHostInfo::localHostName();
	return machineName;
}

QString appConfigIni::getIndex()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/index", "").toString();
}

QString appConfigIni::getWSUrl()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/wsUrl", "").toString();
}

QString appConfigIni::getServerType()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/type", "").toString();
}

QString appConfigIni::getBarId()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/barId", "").toString();
}

QString appConfigIni::getNetBarKey()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/barKey", "").toString();
}

QString appConfigIni::getWebPort()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("websocket/port", "").toString();
}

QString appConfigIni::getBarName()
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	return settings.value("client/barName", "").toString();
}

void appConfigIni::setBarName(QString barName)
{
	QSettings settings(iniPath_, QSettings::IniFormat);
	settings.setValue("client/barName", barName);
}
