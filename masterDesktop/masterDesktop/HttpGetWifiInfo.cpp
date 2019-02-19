#include "HttpGetWifiInfo.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpGetWifiInfo::HttpGetWifiInfo(QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetWifiInfoAction);
	QString allParam = QString("barId=%1").arg(sBarId);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpGetWifiInfo::~HttpGetWifiInfo()
{

}

void HttpGetWifiInfo::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpGetWifiInfo 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpGetWifiInfo %s", strResult.toStdString().c_str());

	QString wifiName = sc.property("data").property("wifiName").toString();
	QString wifiKey = sc.property("data").property("wifiPassword").toString();
	HttpSignal::instance()->sigOnGetWifiInfo(nCode, sMsg, wifiName, wifiKey);
	this->deleteLater();
}

void HttpGetWifiInfo::processError()
{
	HttpSignal::instance()->sigOnGetWifiInfo(e_connectServerError, c_sNetError, "", "");
	this->deleteLater();
}
