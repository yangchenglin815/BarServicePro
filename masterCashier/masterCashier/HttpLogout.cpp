#include "HttpLogout.h"
#include <QScriptEngine>
#include <QScriptValue>

#include "appconfig.h"
#include "enumData.h"
#include "constData.h"
#include "HttpSignal.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpLogout::HttpLogout(int type)
{
	m_type = type;
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderLogoutAction);
	QByteArray array;
	setReqParams(array);
}

HttpLogout::~HttpLogout()
{

}

void HttpLogout::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpLogout 返回参数json格式错误").toStdString());
		Log::Info("HttpLogout 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpLogout" + strResult.toStdString());
	Log::Info("HttpLogout %s", strResult.toStdString().c_str());
	if (m_type == t_switch)
	{
		HttpSignal::instance()->sigOnLogout(nCode, sMsg);
	}
	else if (m_type == t_shift)
	{
		HttpSignal::instance()->sigOnShiftLogout(nCode, sMsg);
	}
	
	this->deleteLater();
}

void HttpLogout::processError()
{
	if (m_type == t_switch)
	{
		HttpSignal::instance()->sigOnLogout(e_connectServerError, c_sNetError);
	}
	else if (m_type == t_shift)
	{
		HttpSignal::instance()->sigOnShiftLogout(e_connectServerError, c_sNetError);
	}
	this->deleteLater();
}
