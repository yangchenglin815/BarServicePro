#include "HttpFaceDetection.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>

#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"
#include "logging.hpp"

HttpFaceDetection::HttpFaceDetection(QString filePath)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpFaceTectionOrderAction);
	setFilePath(filePath);
}

HttpFaceDetection::~HttpFaceDetection()
{

}

void HttpFaceDetection::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("message").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpFaceDetection 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpFaceDetection %s", strResult.toStdString().c_str());
	int nRespondCode = sc.property("data").property("code").toInt32();
	HttpSignal::instance()->sigOnGetFaceDetectionRespond(nCode, sMsg, nRespondCode);
	this->deleteLater();
}

void HttpFaceDetection::processError()
{
	HttpSignal::instance()->sigOnGetFaceDetectionRespond(e_connectServerError, c_sNetError, -1);
	this->deleteLater();
}
