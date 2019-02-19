#include "HttpCheckAcceptor.h"
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

HttpCheckAcceptor::HttpCheckAcceptor(QString sPhone, QString sPwd)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpOrderCheckAcceptor);
	QString allParam = QString("phone=%1&pwd=%2").arg(sPhone).arg(sPwd);
	QByteArray array;
	array.append(allParam);
	setReqParams(array);
}

HttpCheckAcceptor::~HttpCheckAcceptor()
{

}

void HttpCheckAcceptor::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("desc").toString();
	if (!sc.property("code").isValid())
	{
		//AppConfigHandler.Info(QStringLiteral("HttpCheckAcceptor 返回参数json格式错误").toStdString());
		Log::Info("HttpCheckAcceptor 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}
	//AppConfigHandler.Info("HttpCheckAcceptor" + strResult.toStdString());
	Log::Info("HttpCheckAcceptor %s", strResult.toStdString().c_str());

	HttpSignal::instance()->sigOnCheckAcceptorResult(nCode, sMsg);
	this->deleteLater();
}

void HttpCheckAcceptor::processError()
{
	HttpSignal::instance()->sigOnCheckAcceptorResult(e_connectServerError, c_sNetError);
	this->deleteLater();
}
