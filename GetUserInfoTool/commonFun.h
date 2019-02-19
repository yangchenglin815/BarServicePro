#pragma once
#include <QString>
#include <QStringList>

class commonFun
{
public:
	static QString getRequestUrl();
	static QString getBookingErrMsg(int nCode);
};

