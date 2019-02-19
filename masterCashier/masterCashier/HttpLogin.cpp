#include "HttpLogin.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "appConfigIni.h"
#include "logging.hpp"

using namespace dm::server;

HttpLogin::HttpLogin(QString nUser, QString nPasswd, int nType)
{
	m_loginType = nType;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderLoginAction);
	QString allParam = QString("phone=%1&pwd=%2&barId=%3").arg(nUser).arg(nPasswd).arg(appConfigIni::getBarId());
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpLogin::~HttpLogin()
{

}

void HttpLogin::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpLogin 返回参数json格式错误").toStdString());
		Log::Info("HttpLogin 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpLogin" + strResult.toStdString());
	Log::Info("HttpLogin %s", strResult.toStdString().c_str());
	QString userId = sc.property("data").property("id").toString();
	QString userName = sc.property("data").property("name").toString();
	if (m_loginType == t_login)
	{
		HttpSignal::instance()->sigOnLogin(nCode, sMsg, userName, userId);
	}
	else if (m_loginType == t_switch)
	{
		HttpSignal::instance()->sigOnSwitch(nCode, sMsg, userName, userId);
	}
	else if (m_loginType == t_shift)
	{
		HttpSignal::instance()->sigOnShiftLogin(nCode, sMsg, userName, userId);
	}
	this->deleteLater();
}

void HttpLogin::processError()
{
	if (m_loginType == t_login)
	{
		HttpSignal::instance()->sigOnLogin(e_connectServerError, c_sNetError, "", "");
	}
	else if (m_loginType == t_switch)
	{
		HttpSignal::instance()->sigOnSwitch(e_connectServerError, c_sNetError, "", "");
	}
	else if (m_loginType == t_shift)
	{
		HttpSignal::instance()->sigOnShiftLogin(e_connectServerError, c_sNetError, "", "");
	}
	this->deleteLater();
}
