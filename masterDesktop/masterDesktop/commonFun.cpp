#include "commonFun.h"
#include "appConfigIni.h"
#include "enumData.h"
#include "constData.h"

#include <QHostInfo>

QString commonFun::getRequestUrl()
{
	QString sUrl;
	int nType = appConfigIni::getServerType().toInt();
	switch (nType)
	{
	case t_formalServer:
		sUrl = c_sFormalServerUrl;
		break;
	case t_testServer:
		sUrl = c_sTestServerUrl;
		break;
	case t_localServer:
		sUrl = c_sLocalServerUrl;
		break;
	default:
		break;
	}
	return sUrl;
}

bool commonFun::getSearchResult(QString searchText, QString goodsName, QString goodsCode)
{
	if (goodsName.contains(searchText))
	{
		return true;
	}

	QStringList list = goodsCode.split(",");
	if (list.size() > 0)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i] == searchText)
			{
				return true;
			}
		}
	}
	return false;
}

QString commonFun::getLocalmachineName()
{
	QString machineName = QHostInfo::localHostName();
	return machineName;
}

QString commonFun::getHostMacAddress()
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

QString commonFun::getHostIpAddress()
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

int commonFun::secondsToMinutes(int seconds)
{
	return seconds / 60;
}
