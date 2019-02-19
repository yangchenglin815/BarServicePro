#include "HttpSignIn.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

HttpSignIn::HttpSignIn(LoginUserInfo userInfo, QString sBarId /*= appConfigIni::getBarId()*/)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpSignInAction);
	QString allParam = QString("barId=%1&userId=%2&idCard=%3").arg(sBarId).arg(userInfo.sUserId).arg(userInfo.sIdCard);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpSignIn::~HttpSignIn()
{

}

void HttpSignIn::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpSignIn 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpSignIn %s", strResult.toStdString().c_str());

	HttpSignal::instance()->sigOnSignIn(nCode, sMsg);
	this->deleteLater();
}

void HttpSignIn::processError()
{
	HttpSignal::instance()->sigOnSignIn(e_connectServerError, c_sNetError);
	this->deleteLater();
}
