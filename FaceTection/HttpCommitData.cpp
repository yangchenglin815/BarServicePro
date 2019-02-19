#include "HttpCommitData.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>

#include "enumData.h"
#include "HttpSignal.h"
#include "constData.h"
#include "commonFun.h"
#include "logging.hpp"
#include "appConfigIni.h"

HttpCommitData::HttpCommitData(QString sIpAdress, QString sPort)
{
	setReqUrlAddress(commonFun::getRequestUrl() + c_sHttpCommitDataOrderAction);
	QByteArray array = createJsonData(sIpAdress, sPort);
	setReqParams(array);
}

HttpCommitData::~HttpCommitData()
{

}

void HttpCommitData::processResponse(QString strResult)
{
	QScriptEngine engine;
	QScriptValue sc = engine.evaluate("value=" + strResult);
	int nCode = sc.property("code").toInt32();
	QString sMsg = sc.property("message").toString();
	if (!sc.property("code").isValid())
	{
		Log::Info("HttpCommitData 返回参数json格式错误");
		nCode = e_connectServerError;
		sMsg = c_sServerJsonError;
	}
	if (nCode != e_success)
	{
		Log::Info("%s", this->getTestUrl().toStdString().c_str());
	}

	Log::Info("HttpCommitData %s", strResult.toStdString().c_str());
	HttpSignal::instance()->sigOnGetCommitDataRespond(nCode, sMsg);
	this->deleteLater();
}

void HttpCommitData::processError()
{
	HttpSignal::instance()->sigOnGetCommitDataRespond(e_connectServerError, c_sNetError);
	this->deleteLater();
}

QByteArray HttpCommitData::createJsonData(QString ipAdress, QString port)
{
	QByteArray byte_array;
	QJsonObject jsonObject;
	jsonObject.insert("ip", ipAdress);
	jsonObject.insert("port", port);
	jsonObject.insert("barId", appConfigIni::getBarId());
	jsonObject.insert("barKey", appConfigIni::getNetBarKey());

	QJsonDocument document;
	document.setObject(jsonObject);
	byte_array = document.toJson(QJsonDocument::Compact);
	return byte_array;
}
