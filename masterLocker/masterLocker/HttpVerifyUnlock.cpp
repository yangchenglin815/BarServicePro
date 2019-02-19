#include "HttpVerifyUnlock.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "logging.hpp"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"

HttpVerifyUnlock::HttpVerifyUnlock(QString sPcName, QString sBarID, QString sMac, QString nCode)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpGetOrderVerifyUnlockAction);
	QString allParam = QString("pcName=%1&barId=%2&pcMac=%3&code=%4").arg(sPcName).arg(sBarID).arg(sMac).arg(nCode);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpVerifyUnlock::~HttpVerifyUnlock()
{

}

void HttpVerifyUnlock::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info(QStringLiteral("HttpVerifyUnlock 返回参数json格式错误").toStdString().c_str());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info(this->getTestUrl().toStdString().c_str());
	}

	Log::Info(strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnGetVerifyUnlockCode(nCode, sMsg);
	this->deleteLater();
}

void HttpVerifyUnlock::processError()
{
	HttpSignal::instance()->sigOnGetVerifyUnlockCode(e_connectServerError, c_sNetError);
	this->deleteLater();
}