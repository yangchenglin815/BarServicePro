#include "HttpCommitFaceData.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>
#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"

HttpCommitFaceData::HttpCommitFaceData()
{
	setReqUrlAddress("http://202.91.251.122:8050/qynetbarsystem/uploads/face/facePerson");
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
		qDebug() << QStringLiteral("HttpCommitFaceData 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		qDebug() << this->getTestUrl();
	}

	qDebug() << "HttpCommitFaceData" + strResult;
	int nRespondCode = sc.property("data").property("code").toInt32();
	HttpSignal::instance()->sigOnGetCommitFaceDataRespond(nCode, sMsg, nRespondCode);
	this->deleteLater();
}

void HttpCommitFaceData::processError()
{
	HttpSignal::instance()->sigOnGetCommitFaceDataRespond(e_connectServerError, c_sNetError, -1);
	this->deleteLater();
}
