#include "HttpCancelOrder.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpCancelOrder::HttpCancelOrder(QString sBarId, QString sOrderId, QString sPasswd)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderCancelOrderAction);
	QString allParam = QString("barId=%1&id=%2&shopownerPwd=%3").arg(sBarId).arg(sOrderId).arg(sPasswd);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpCancelOrder::~HttpCancelOrder()
{

}

void HttpCancelOrder::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpCancelOrder 返回参数json格式错误").toStdString());
		Log::Info("HttpCancelOrder 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}
	//AppConfigHandler.Info("HttpCancelOrder" + strResult.toStdString());
	Log::Info("HttpCancelOrder %s", strResult.toStdString().c_str());

	HttpSignal::instance()->sigOnGetCancelOrderRespond(nCode, sMsg);
	this->deleteLater();
}

void HttpCancelOrder::processError()
{
	HttpSignal::instance()->sigOnGetCancelOrderRespond(e_connectServerError, c_sNetError);
	this->deleteLater();
}
