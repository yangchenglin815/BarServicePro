#include "HttpGetBarName.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "logging.hpp"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"

HttpGetBarName::HttpGetBarName(QString nBarId)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderBarNameAction);
	QString allParam = QString("barId=%1").arg(nBarId);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetBarName::~HttpGetBarName()
{

}

void HttpGetBarName::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpGetBarName 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info(this->getTestUrl().toStdString().c_str());
	}

	Log::Info(strResult.toStdString().c_str());
	QString barName = sc.property("data").property("barName").toString();
	HttpSignal::instance()->sigOnGetBarName(nCode, sMsg, barName);
	this->deleteLater();
}

void HttpGetBarName::processError()
{
	HttpSignal::instance()->sigOnGetBarName(e_connectServerError, c_sNetError, "");
	this->deleteLater();
}
