#include "HttpRefund.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpRefund::HttpRefund(QString sBarId, QString sOrderId, QString sRemark)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderRefundAction);
	QString allParam = QString("barId=%1&id=%2&remark=%3&orderFrom=2").arg(sBarId).arg(sOrderId).arg(sRemark);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpRefund::~HttpRefund()
{

}

void HttpRefund::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpRefund 返回参数json格式错误").toStdString());
		Log::Info("HttpRefund 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpRefund" + strResult.toStdString());
	Log::Info("HttpRefund %s", strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnGetRefundRespond(nCode, sMsg);
	this->deleteLater();
}

void HttpRefund::processError()
{
	HttpSignal::instance()->sigOnGetRefundRespond(e_connectServerError, c_sNetError);
	this->deleteLater();
}
