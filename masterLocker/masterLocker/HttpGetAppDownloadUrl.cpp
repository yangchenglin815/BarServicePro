#include "HttpGetAppDownloadUrl.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "logging.hpp"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"


HttpGetAppDownloadUrl::HttpGetAppDownloadUrl(QString nAction)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderQrCodeAction);
	QString allParam = QString("action=%1").arg(nAction);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetAppDownloadUrl::~HttpGetAppDownloadUrl()
{

}

void HttpGetAppDownloadUrl::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpGetAppDownloadUrl 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info(this->getTestUrl().toStdString().c_str());
	}

	Log::Info(strResult.toStdString().c_str());
	QString downUrl = sc.property("data").property("downLoadUrl").toString();
	HttpSignal::instance()->sigOnGetDownloadUrl(nCode, sMsg, downUrl);
	this->deleteLater();
}

void HttpGetAppDownloadUrl::processError()
{
	HttpSignal::instance()->sigOnGetDownloadUrl(e_connectServerError, c_sNetError, "");
	this->deleteLater();
}
