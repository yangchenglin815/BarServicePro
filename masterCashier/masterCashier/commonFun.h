#pragma once
#include <QString>
#include <QStringList>

class commonFun
{
public:
	static QString getRequestUrl();
	static QString getBookingErrMsg(int nCode);
	static bool getSearchResult(QString searchText, QString goodsName, QString goodsCode);
	static QString getPayWay(int index);
	static QString getImageFileName(QString filePath);
};

