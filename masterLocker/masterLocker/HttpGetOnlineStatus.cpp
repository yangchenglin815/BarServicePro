#include "HttpGetOnlineStatus.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "logging.hpp"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"


HttpGetOnlineStatus::HttpGetOnlineStatus(QString sPcName, QString sBarID, QString sMac, QString nOnlineFlag)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderOnlineStatusAction);
	QString allParam = QString("pcName=%1&barId=%2&pcMac=%3&computerState=%4").arg(sPcName).arg(sBarID).arg(sMac).arg(nOnlineFlag);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetOnlineStatus::~HttpGetOnlineStatus()
{

}

void HttpGetOnlineStatus::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpGetOnlineStatus 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info(this->getTestUrl().toStdString().c_str());
	}

	Log::Info(strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnReportOnlineStatus(nCode, sMsg);
	this->deleteLater();
}

void HttpGetOnlineStatus::processError()
{
	HttpSignal::instance()->sigOnReportOnlineStatus(e_connectServerError, c_sNetError);
	this->deleteLater();
}
