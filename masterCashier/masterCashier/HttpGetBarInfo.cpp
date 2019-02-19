#include "HttpGetBarInfo.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpGetBarInfo::HttpGetBarInfo(QString nBarId)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderBarNameAction);
	QString allParam = QString("barId=%1").arg(nBarId);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetBarInfo::~HttpGetBarInfo()
{

}

void HttpGetBarInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpGetBarInfo 返回参数json格式错误").toStdString());
		Log::Info("HttpGetBarInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpGetBarInfo" + strResult.toStdString());
	Log::Info("HttpGetBarInfo %s", strResult.toStdString().c_str());
	QString barName = sc.property("data").property("barName").toString();
	HttpSignal::instance()->sigOnGetBarInfo(nCode, sMsg, barName);
	this->deleteLater();
}

void HttpGetBarInfo::processError()
{
	HttpSignal::instance()->sigOnGetBarInfo(e_connectServerError, c_sNetError, "");
	this->deleteLater();
}
