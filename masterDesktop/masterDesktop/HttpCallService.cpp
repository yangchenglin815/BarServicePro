#include "HttpCallService.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpCallService::HttpCallService(LoginUserInfo userInfo, QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpCallServiceAction);
	QString allParam = QString("barId=%1&shoperId=%2&shoperName=%3&seatNum=%4&notifyType=1").arg(sBarId).arg(userInfo.sUserId).arg(userInfo.sName).arg(commonFun::getLocalmachineName());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpCallService::~HttpCallService()
{

}

void HttpCallService::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpCallService 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpCallService %s", strResult.toStdString().c_str());

	HttpSignal::instance()->sigOnCallService(nCode, sMsg);
	this->deleteLater();
}

void HttpCallService::processError()
{
	HttpSignal::instance()->sigOnCallService(e_connectServerError, c_sNetError);
	this->deleteLater();
}
