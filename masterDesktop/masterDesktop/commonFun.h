#pragma once
#include <QString>
#include <QStringList>

class commonFun
{
public:
	static QString getRequestUrl();
	static bool getSearchResult(QString searchText, QString goodsName, QString goodsCode);
	static QString getLocalmachineName();
	static QString getHostMacAddress();
	static QString getHostIpAddress();
	static int secondsToMinutes(int seconds);
};

