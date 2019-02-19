#include "HttpGetBookSeatStatus.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "logging.hpp"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"


HttpGetBookSeatStatus::HttpGetBookSeatStatus(QString sPcName, QString sBarID, QString sMac)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderBookSeatAction);
	QString allParam = QString("pcName=%1&barId=%2&pcMac=%3&pcStatus=1").arg(sPcName).arg(sBarID).arg(sMac);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetBookSeatStatus::~HttpGetBookSeatStatus()
{

}

void HttpGetBookSeatStatus::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpGetBookSeatStatus 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info(this->getTestUrl().toStdString().c_str());
	}

	Log::Info(strResult.toStdString().c_str());
	int nStatus = sc.property("data").toInt32();
	HttpSignal::instance()->sigOnGetBookSeatStatus(nCode, sMsg, nStatus);
	this->deleteLater();
}

void HttpGetBookSeatStatus::processError()
{
	HttpSignal::instance()->sigOnGetBookSeatStatus(e_connectServerError, c_sNetError, -1);
	this->deleteLater();
}
