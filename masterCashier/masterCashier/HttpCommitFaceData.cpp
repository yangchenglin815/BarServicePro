#include "HttpCommitFaceData.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>

#include "appconfig.h"
#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"
#include "logging.hpp"

using namespace dm::server;

HttpCommitFaceData::HttpCommitFaceData()
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_SHttpCommitFaceDataOrderAction);
}

HttpCommitFaceData::~HttpCommitFaceData()
{

}

void HttpCommitFaceData::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("message").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpCommitFaceData 返回参数json格式错误");
		//AppConfigHandler.Info(QStringLiteral("HttpCommitFaceData 返回参数json格式错误").toStdString());
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		//AppConfigHandler.Info(this->getTestUrl().toStdString());
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	//AppConfigHandler.Info("HttpCommitFaceData" + strResult.toStdString());
	Log::Info("HttpCommitFaceData %s", strResult.toStdString().c_str());
	int nRespondCode = sc.property("data").property("code").toInt32();
	HttpSignal::instance()->sigOnGetCommitFaceDataRespond(nCode, sMsg, nRespondCode);
	this->deleteLater();
}

void HttpCommitFaceData::processError()
{
	HttpSignal::instance()->sigOnGetCommitFaceDataRespond(e_connectServerError, c_sNetError, -1);
	this->deleteLater();
}
