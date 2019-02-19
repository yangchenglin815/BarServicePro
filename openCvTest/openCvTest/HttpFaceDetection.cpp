#include "HttpFaceDetection.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>
#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"

HttpFaceDetection::HttpFaceDetection(QString filePath)
{
	setReqUrlAddress("http://202.91.251.122:8050/qynetbarsystem/upload/face/faceDetect");
	setFilePath(filePath);
// 	QString allParam = QString("file=%1").arg(commonFun::readImageData());
// 	QByteArray array;
// 	array.append(allParam);
// 	setReqParams(array);
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
		qDebug() << QStringLiteral("HttpFaceDetection 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		qDebug() << this->getTestUrl();
	}

	qDebug() << "HttpFaceDetection" + strResult;
	int nRespondCode = sc.property("data").property("code").toInt32();
	HttpSignal::instance()->sigOnGetFaceDetectionRespond(nCode, sMsg, nRespondCode);
	this->deleteLater();
}

void HttpFaceDetection::processError()
{
	HttpSignal::instance()->sigOnGetFaceDetectionRespond(e_connectServerError, c_sNetError, -1);
	this->deleteLater();
}
